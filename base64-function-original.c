
#include "base64-function-original.h"

static const char base64_encode_table_original[64] = {
       'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
       'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',  'h',
       'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
       'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};


static int GetRequiredEncodeLength(int aiSrcLen, int aiEncodeFlags) {
       int iRet = aiSrcLen * 4 / 3;      // Durch Codierung wachsen Daten um 33%

       if ((aiEncodeFlags & DT_BASE64_FLAG_NOPAD) == 0) // Ende des Streams mittels '=' kenzeichnen
       {
             iRet += aiSrcLen % 3;
       }

       int iCRLFs = iRet / 76 + 1;       // Keine Zeile darf mehr als 76 Zeichen haben
       int iOnLastLine = iRet % 76;

       if (iOnLastLine) {
             if (iOnLastLine % 4) {
                    iRet += 4 - (iOnLastLine % 4);
             }
       }

       iCRLFs *= 2;

       if ((aiEncodeFlags & DT_BASE64_FLAG_NOCRLF) == 0) {
             iRet += iCRLFs;
       }

       return iRet;
}

static int Encode(const char* apbySrc, int aiSrcLen, char* apsDest, int* apiDestMaxCount, int aiEncodeFlags) {

	if (!apbySrc) {
		if (PRINT_DEBUG) printf("no Input ...\n");
		return 0;
	}
	
	if (!apsDest) {
		if (PRINT_DEBUG) printf("no Output ...\n");
		return 0;
	}
	
	if (!apiDestMaxCount) {
		if (PRINT_DEBUG) printf("no Output count ...\n");
		return 0;
	}
	
	int requiredLength = base64_encode_length_original(aiSrcLen, aiEncodeFlags);
	if (*apiDestMaxCount < requiredLength) {
		if (PRINT_DEBUG) printf("output to short - %d < %d ...\n", *apiDestMaxCount, requiredLength);
		return 0;
	}
	if (PRINT_DEBUG) printf("input length %d ...\n", aiSrcLen);
	if (PRINT_DEBUG) printf("output length %d ...\n", requiredLength);
	
	int iWritten = 0;
	int iLen1           = (aiSrcLen / 3) * 4;      // Stream wird um 33% laenger
	int iLen2           = iLen1 / 76;              // Keine Zeile mit mehr als 76 Zeichen
	int iLen3           = 19;                      // aus 19 Bytes werden 19*4 Bytes ->> eine Zeile mit 76 Zeichen
	
	if (PRINT_DEBUG) printf("len1 - %d ...\n", iLen1);
	if (PRINT_DEBUG) printf("len2 - %d ...\n", iLen2);
	if (PRINT_DEBUG) printf("len3 - %d ...\n", iLen3);
	
	for (int i = 0; i <= iLen2; i++) {
		if (i == iLen2) {
			iLen3 = (iLen1 % 76) / 4;
			if (PRINT_DEBUG) printf("len3 - %d ...\n", iLen3);
		}
	
		for (int j = 0; j < iLen3; j++) {
			unsigned long ulCurr = 0;
			for (int n = 0; n < 3; n++) {
				ulCurr |= *apbySrc++;
				ulCurr <<= 8;
				if (PRINT_DEBUG) printf("iulCurr - %08lx ...\n", ulCurr);
			}
			for (int k = 0; k < 4; k++) {	
				unsigned char b = (unsigned char)(ulCurr >> 26);
				unsigned char c = base64_encode_table_original[b];
				if (PRINT_DEBUG) printf("in - %i => 0x%02x => '%c' ...\n", b, b, b);
				if (PRINT_DEBUG) printf("out - %i => 0x%02x => '%c' ...\n", c, c, c);
				*apsDest++ = c;
				if (PRINT_DEBUG) printf("oulCurr - %08lx ...\n", ulCurr);
				ulCurr <<= 6;
				if (PRINT_DEBUG) printf("oulCurr - %08lx ...\n", ulCurr);
			}
		}
	
		iWritten += iLen3 * 4;
	
		if ((aiEncodeFlags & DT_BASE64_FLAG_NOCRLF) == 0)     // 76 Zeichen für eine Zeile, also "\r\n"
		{
			*apsDest++ = '\r';
			*apsDest++ = '\n';
			iWritten += 2;
		}
	}
	
	if (iWritten && (aiEncodeFlags & DT_BASE64_FLAG_NOCRLF) == 0) {
		apsDest -= 2;
		iWritten -= 2;
	}
	
	iLen2 = aiSrcLen % 3 ? aiSrcLen % 3 + 1 : 0;   // Ist noch etwas übrig?
	if (iLen2) {
		unsigned long ulCurr = 0;
		for (int n = 0; n < 3; n++) {
			if (n < (aiSrcLen % 3))                               // wenn noch zeichen vorhanden
			{
				ulCurr |= *apbySrc++;    // zeichen aufnehmen, sonst mit Nullen befüllen
			}
			ulCurr <<= 8;
		}
		for (int k = 0; k < iLen2; k++) {
			unsigned char b = (unsigned char)(ulCurr >> 26);
			*apsDest++ = base64_encode_table_original[b];
			ulCurr <<= 6;
		}
	
		iWritten += iLen2;
		if ((aiEncodeFlags & DT_BASE64_FLAG_NOPAD) == 0) // Endmarkierung
		{
			iLen3 = iLen2 ? 4 - iLen2 : 0;
			for (int j = 0; j < iLen3; j++) {
				*apsDest++ = '=';
			}
			iWritten += iLen3;
		}
	}
	
	*apiDestMaxCount = iWritten;
	return 1;
}


static int DecodeChar(unsigned int auiChar) {
	if (PRINT_DEBUG) printf("char %c ...\n", auiChar);
	if (auiChar >= 'A' && auiChar <= 'Z') {
		if (PRINT_DEBUG) printf("ret %02x ...\n", auiChar - 'A' + 0);
		return auiChar - 'A' + 0;    // 0 range starts at 'A'
	}
	if (auiChar >= 'a' && auiChar <= 'z') {
		if (PRINT_DEBUG) printf("ret %02x ...\n", auiChar - 'a' + 26);
		return auiChar - 'a' + 26;    // 26 range starts at 'a'
	}
	if (auiChar >= '0' && auiChar <= '9') {
		if (PRINT_DEBUG) printf("ret %02x ...\n", auiChar - '0' + 52);
		return auiChar - '0' + 52;    // 52 range starts at '0'
	}
	if (auiChar == '+') {
		if (PRINT_DEBUG) printf("ret 62 ...\n");
		return 62;
	}
	if (auiChar == '/') {
		if (PRINT_DEBUG) printf("ret 63 ...\n");
		return 63;
	}
	if (PRINT_DEBUG) printf("ret -41 ...\n");
	return -1;
}

static int Decode(const char* apsSrc, int aiSrcMaxCount, char* apbyDest, int* apiDestLen) {
	// walk the source buffer
	// each four character sequence is converted to 3 bytes
	// CRLFs and =, and any characters not in the encoding table
	// are skiped
	
	if (!apsSrc || !apiDestLen) {
		return 0;
	}
	
	const char* psSrcEnd = apsSrc + aiSrcMaxCount;
	int iWritten = 0;
	
	int bOverflow = (apbyDest == NULL) ? 1 : 0;
	
	while (apsSrc < psSrcEnd) {
		unsigned long ulCurr = 0;
		int i;
		int iBits = 0;
		for (i = 0; i < 4; i++) {
			if (apsSrc >= psSrcEnd) {
				break;
			}
			int iCh = base64_decode_char_original(*apsSrc);
			apsSrc++;
			if (iCh == -1) {
				// skip this char
				i--;
				continue;
			}
			ulCurr <<= 6;
			ulCurr |= iCh;
			iBits += 6;
		}
		
		if (!bOverflow && iWritten + (iBits / 8) > (*apiDestLen)) {
		       bOverflow = 1;
		}
		
		// dwCurr has the 3 bytes to write to the output buffer
		// left to right
		ulCurr <<= 24 - iBits;
		for (i = 0; i < iBits / 8; i++) {
			if (!bOverflow) {
				if (PRINT_DEBUG) printf("0x%02x\n", (char)((ulCurr & 0x00ff0000) >> 16));
				*apbyDest = (char)((ulCurr & 0x00ff0000) >> 16);
				apbyDest++;
			}
			ulCurr <<= 8;
			iWritten++;
		}
	}
	
	*apiDestLen = iWritten;
	
	if (bOverflow) {
		if (apbyDest) {
			return -1;
		}
		
		return 0;
	}
	
	return 1;
}

base64_func_set base64_fixed_func_set = {
	GetRequiredEncodeLength,
	Encode,
	DecodeChar,
	Decode
};