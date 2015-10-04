
#include "base64-test.h"

//##############################################################################

int PRINT_DEBUG = 0;

//##############################################################################

const char gpsBase64EncodingTable[64] = {
       'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
       'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',  'h',
       'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
       'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

//##############################################################################

int GetRequiredEncodeLength(int aiSrcLen, int aiEncodeFlags/*DT_BASE64_FLAG_NONE*/) {
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

int Encode(const char* apbySrc, int aiSrcLen, char* apsDest, int* apiDestMaxCount, int aiEncodeFlags) {
	
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
	
	int requiredLength = GetRequiredEncodeLength(aiSrcLen, aiEncodeFlags);
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
				unsigned char b = (unsigned char)((ulCurr >> 26)&0x3F);
				unsigned char c = gpsBase64EncodingTable[b];
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
			if (PRINT_DEBUG) printf("iulCurr - %08lx ...\n", ulCurr);
		}
		for (int k = 0; k < iLen2; k++) {
			unsigned char b = (unsigned char)((ulCurr >> 26)&0x3F);
			unsigned char c = gpsBase64EncodingTable[b];
			if (PRINT_DEBUG) printf("in - %i => 0x%02x => '%c' ...\n", b, b, b);
			if (PRINT_DEBUG) printf("out - %i => 0x%02x => '%c' ...\n", c, c, c);
			*apsDest++ = c;
			if (PRINT_DEBUG) printf("oulCurr - %08lx ...\n", ulCurr);
			ulCurr <<= 6;
			if (PRINT_DEBUG) printf("oulCurr - %08lx ...\n", ulCurr);
		}
		
		iWritten += iLen2;
		if ((aiEncodeFlags & DT_BASE64_FLAG_NOPAD) == 0) // Endmarkierung
		{
			iLen3 = iLen2 ? 4 - iLen2 : 0;
			for (int j = 0; j < iLen3; j++) {
				*apsDest++ = '=';
				if (PRINT_DEBUG) printf("padding - = ...\n");
			}
			iWritten += iLen3;
		}
	}
	
	*apiDestMaxCount = iWritten;
	return 1;
}

// original
int Encode2(const char* apbySrc, int aiSrcLen, char* apsDest, int* apiDestMaxCount, int aiEncodeFlags) {
	
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
	
	int requiredLength = GetRequiredEncodeLength(aiSrcLen, aiEncodeFlags);
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
				unsigned char c = gpsBase64EncodingTable[b];
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
			*apsDest++ = gpsBase64EncodingTable[b];
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

//##############################################################################

int DecodeChar(unsigned int auiChar) {
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

int Decode(const char* apsSrc, int aiSrcMaxCount, char* apbyDest, int* apiDestLen) {
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
			if (PRINT_DEBUG) printf("in - %02x ...\n", *apsSrc);
			int iCh = DecodeChar(*apsSrc);
			if (PRINT_DEBUG) printf("out - %02x ...\n", iCh);
			apsSrc++;
			if (iCh == -1) {
				// skip this char
				i--;
				if (PRINT_DEBUG) printf("skip ...\n");
				continue;
			}
			ulCurr <<= 6;
			ulCurr |= (iCh&0x3F);
			if (PRINT_DEBUG) printf("oulCurr - %08lx ...\n", ulCurr);
			iBits += 6;
		}
		
		if (!bOverflow && iWritten + (iBits / 8) > (*apiDestLen)) {
		       bOverflow = 1;
		}
		
		// dwCurr has the 3 bytes to write to the output buffer
		// left to right
		ulCurr <<= 24 - iBits;
		for (i = 0; i < iBits / 8; i++) {
			if (PRINT_DEBUG) printf("oulCurr - %08lx ...\n", ulCurr);
			if (!bOverflow) {
				if (PRINT_DEBUG) printf("save 0x%02x\n", (char)((ulCurr & 0x00ff0000) >> 16));
				*apbyDest = (char)((ulCurr & 0x00ff0000) >> 16);
				apbyDest++;
			}
			ulCurr <<= 8;
			if (PRINT_DEBUG) printf("oulCurr - %08lx ...\n", ulCurr);
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

// original
int Decode2(const char* apsSrc, int aiSrcMaxCount, char* apbyDest, int* apiDestLen) {
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
			int iCh = DecodeChar(*apsSrc);
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

int Decode3(const char* apsSrc, int aiSrcMaxCount, char* apbyDest, int* apiDestLen) {
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
			int iCh = DecodeChar(*apsSrc);
			apsSrc++;
			if (iCh == -1) {
				// skip this char
				i--;
				continue;
			}
			ulCurr <<= 6;
			ulCurr |= (iCh&0x3F);
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
				if (PRINT_DEBUG) printf("0x%02x\n", (char)((ulCurr & 0x003f0000) >> 16));
				*apbyDest = (char)((ulCurr & 0x003f0000) >> 16);
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

//##############################################################################

void test_hex_print(const char* string, int len) {
	printf("[%d]:0x", len);
	for (int i=0; i<len; i++) {
		printf("%02x", (string[i]&0xff));
	}
}

void test_encode_string(const char* string, int len) {
	
	printf(" -> encode : [%d]:\"%s\" => ", len, string);
	
	int flags = DT_BASE64_FLAG_NONE;
	
	int encoded_string_length = GetRequiredEncodeLength(len, flags);
	char encoded_string[encoded_string_length + 1];
	
	int ret = Encode(string, len, encoded_string, &encoded_string_length, flags);
	if (ret==0){
		printf("failed encoding ...\n");
		return;
	}
	
	encoded_string[encoded_string_length] = '\0';
	printf("[%d]:\"%s\"\n", encoded_string_length, encoded_string);
	
	printf("           : ");
	test_hex_print(string, len);
	printf(" => ");
	test_hex_print(encoded_string, encoded_string_length);
	printf("\n\n");
	
	return;
}

void test_encode_string2(const char* string, int len) {
	
	printf(" -> encode : [%d]:\"%s\" => ", len, string);
	
	int flags = DT_BASE64_FLAG_NONE;
	
	int encoded_string_length = GetRequiredEncodeLength(len, flags);
	char encoded_string[encoded_string_length + 1];
	
	int ret = Encode2(string, len, encoded_string, &encoded_string_length, flags);
	if (ret==0){
		printf("failed encoding ...\n");
		return;
	}
	
	encoded_string[encoded_string_length] = '\0';
	printf("[%d]:\"%s\"\n", encoded_string_length, encoded_string);
	
	printf("           : ");
	test_hex_print(string, len);
	printf(" => ");
	test_hex_print(encoded_string, encoded_string_length);
	printf("\n\n");
	
	return;
}

void test_decode_string(const char* string, int len) {
	
	printf(" -> decode : [%d]:\"%s\" => ", len, string);
	
	int decoded_string_length = (len*3/4);
	char decoded_string[decoded_string_length + 1];
	
	int ret = Decode(string, len, decoded_string, &decoded_string_length);
	if (ret==0){
		printf("failed encoding ...\n");
		//return;
	}
	
	decoded_string[decoded_string_length] = '\0';
	printf("[%d]:\"%s\"\n", decoded_string_length, decoded_string);
	
	printf("           : ");
	test_hex_print(string, len);
	printf(" => ");
	test_hex_print(decoded_string, decoded_string_length);
	printf("\n\n");
	
	return;
}

void test_decode_string2(const char* string, int len) {
	
	printf(" -> decode : [%d]:\"%s\" => ", len, string);
	
	int decoded_string_length = (len*3/4);
	char decoded_string[decoded_string_length + 1];
	
	int ret = Decode2(string, len, decoded_string, &decoded_string_length);
	if (ret==0){
		printf("failed encoding ...\n");
		//return;
	}
	
	decoded_string[decoded_string_length] = '\0';
	printf("[%d]:\"%s\"\n", decoded_string_length, decoded_string);
	
	printf("           : ");
	test_hex_print(string, len);
	printf(" => ");
	test_hex_print(decoded_string, decoded_string_length);
	printf("\n\n");
	
	return;
}

void test_decode_string3(const char* string, int len) {
	
	printf(" -> decode : [%d]:\"%s\" => ", len, string);
	
	int decoded_string_length = (len*3/4);
	char decoded_string[decoded_string_length + 1];
	
	int ret = Decode3(string, len, decoded_string, &decoded_string_length);
	if (ret==0){
		printf("failed encoding ...\n");
		//return;
	}
	
	decoded_string[decoded_string_length] = '\0';
	printf("[%d]:\"%s\"\n", decoded_string_length, decoded_string);
	
	printf("           : ");
	test_hex_print(string, len);
	printf(" => ");
	test_hex_print(decoded_string, decoded_string_length);
	printf("\n\n");
	
	return;
}

//##############################################################################

void test_encode_string_function(const char* string, int len, base64_encode_func encode) {
	
	printf(" -> encode : [%d]:\"%s\" => ", len, string);
	
	int flags = DT_BASE64_FLAG_NONE;
	
	int encoded_string_length = GetRequiredEncodeLength(len, flags);
	char encoded_string[encoded_string_length + 1];
	
	int ret = encode(string, len, encoded_string, &encoded_string_length, flags);
	if (ret==0){
		printf("failed encoding ...\n");
		return;
	}
	
	encoded_string[encoded_string_length] = '\0';
	printf("[%d]:\"%s\"\n", encoded_string_length, encoded_string);
	
	printf("           : ");
	test_hex_print(string, len);
	printf(" => ");
	test_hex_print(encoded_string, encoded_string_length);
	printf("\n\n");
	
	return;
}

void test_encode_string_funcset(const char* string, int len, int flags, base64_func_set funcset) {
	
	printf(" -> encode : [%d]:\"%s\" => ", len, string);
	
	//int flags = DT_BASE64_FLAG_NONE;
	
	int encoded_string_length = funcset.GetRequiredEncodeLength(len, flags);
	char encoded_string[encoded_string_length + 1];
	
	int ret = funcset.Encode(string, len, encoded_string, &encoded_string_length, flags);
	if (ret==0){
		printf("failed encoding ...\n");
		return;
	}
	
	encoded_string[encoded_string_length] = '\0';
	printf("[%d]:\"%s\"\n", encoded_string_length, encoded_string);
	
	printf("           : ");
	test_hex_print(string, len);
	printf(" => ");
	test_hex_print(encoded_string, encoded_string_length);
	printf("\n\n");
	
	return;
}
//##############################################################################

int main (int argc, char *argv[]) {
	
	if (argc>2 && strcmp(argv[2], "debug")==0) {
		PRINT_DEBUG = 1;
	}
	
	if (argc>1 && strcmp(argv[1], "e")==0) {
		
		printf("\n");
		printf("################################################################################\n");
		printf("ENCODING - test string - output should be \"YWJjZGU=\" ...\n");
		printf("\n");
		test_encode_string("abcde\0", 5);
		
		printf("################################################################################\n");
		printf("ENCODING - test effect on shorter strings - output should be shorter than \"YWJjZGU\" ...\n");
		printf("\n");
		test_encode_string("abcd\0", 4);
		test_encode_string("abc\0", 3);
		test_encode_string("ab\0", 2);
		test_encode_string("a\0", 1);
		
		printf("################################################################################\n");
		printf("ENCODING - test effect on longer strings - output should be bigger than \"YWJjZGU\" ...\n");
		printf("\n");
		test_encode_string("abcdefghijklmnopqrstuvwxyz\0", 26);
		test_encode_string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\0", 52);
		test_encode_string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/\0", 64);
		
		printf("################################################################################\n");
		printf("\n");
		
	}
	else if (argc>1 && strcmp(argv[1], "e2")==0) {
		
		printf("\n");
		printf("################################################################################\n");
		printf("ENCODING2 - test string - output should be \"YWJjZGU=\" ...\n");
		printf("\n");
		test_encode_string2("abcde\0", 5);
		
		printf("################################################################################\n");
		printf("ENCODING2 - test effect on shorter strings - output should be shorter than \"YWJjZGU\" ...\n");
		printf("\n");
		test_encode_string2("abcd\0", 4);
		test_encode_string2("abc\0", 3);
		test_encode_string2("ab\0", 2);
		test_encode_string2("a\0", 1);
		
		printf("################################################################################\n");
		printf("ENCODING2 - test effect on longer strings - output should be bigger than \"YWJjZGU\" ...\n");
		printf("\n");
		test_encode_string2("abcdefghijklmnopqrstuvwxyz\0", 26);
		test_encode_string2("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\0", 52);
		test_encode_string2("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/\0", 64);
		
		printf("################################################################################\n");
		printf("\n");
		
	}
	else if (argc>1 && strcmp(argv[1], "d")==0) {
		
		printf("\n");
		printf("################################################################################\n");
		printf("DECODING - test string - output should be \"abcde\" ...\n");
		printf("\n");
		test_decode_string("YWJjZGU=\0", 8);
		
		printf("################################################################################\n");
		printf("DECODING - test effect on shorter strings - output should be shorter than \"abcde\" ...\n");
		printf("\n");
		test_decode_string("YWJjZA==\0", 8);
		test_decode_string("YWJj\0", 4);
		test_decode_string("YWI=\0", 4);
		test_decode_string("YQ==\0", 4);
		
		printf("################################################################################\n");
		printf("DECODING - test effect on longer strings - output should be bigger than \"abcde\" ...\n");
		printf("\n");
		test_decode_string("YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXo=\0", 36);
		test_decode_string("YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXpBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWg==\0", 72);
		test_decode_string("YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXpBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWjAxMjM0\r\nNTY3ODkrLw==\0", 92);
		
		printf("################################################################################\n");
		printf("\n");
		
	}
	else if (argc>1 && strcmp(argv[1], "d2")==0) {
		
		printf("\n");
		printf("################################################################################\n");
		printf("DECODING - test string - output should be \"abcde\" ...\n");
		printf("\n");
		
		test_decode_string("YWI=\0", 4);
		printf("\n");
		test_decode_string("YWx\0", 4);
		
		printf("################################################################################\n");
		printf("\n");
		
	}
	else if (argc>1 && strcmp(argv[1], "T")==0) {
		
		printf("\n");
		printf("################################################################################\n");
		printf("Some Tests ...\n");
		printf("\n");
		test_encode_string("487447CDB1B74B618D3EDF9BD1843899\0", 32);
		test_encode_string("703DEFE0975F4C85A10C106293B5DFF0\0", 32);
		
		const char str2[17] = {0x48, 0x74, 0x47, 0xCD, 0xB1, 0xB7, 0x4B, 0x61, 0x8D, 0x3E, 0xDF, 0x9B, 0xD1, 0x84, 0x38, 0x99, 0x00};
		test_encode_string(str2, 16);
		
		const char str1[17] = {0x70, 0x3D, 0xEF, 0xE0, 0x97, 0x5F, 0x4C, 0x85, 0xA1, 0x0C, 0x10, 0x62, 0x93, 0xB5, 0xDF, 0xF0, 0x00};
		test_encode_string(str1, 16);
		
		test_decode_string("4f7ecbK6Zhzbj3VZbEwyXl==\0", 24);
		test_decode_string("7KWOQAFQyExkZ9PPtd7Ftw==\0", 24);
		
		const char str3[17] = {0xec, 0xa5, 0x8e, 0x40, 0x01, 0x50, 0xc8, 0x4c, 0x64, 0x67, 0xd3, 0xcf, 0xb5, 0xde, 0xc5, 0xb7, 0x00};
		test_encode_string(str3, 16);
		
		test_decode_string("487447CDB1B74B618D3EDF9BD1843899\0", 32);
		
		printf("################################################################################\n");
		printf("\n");
		
	}
	else if (argc>1 && strcmp(argv[1], "T2")==0) {
		
		printf("\n");
		printf("################################################################################\n");
		printf("Some Tests2 ...\n");
		printf("\n");
		test_encode_string2("487447CDB1B74B618D3EDF9BD1843899\0", 32);
		test_encode_string2("703DEFE0975F4C85A10C106293B5DFF0\0", 32);
		
		const char str2[17] = {0x48, 0x74, 0x47, 0xCD, 0xB1, 0xB7, 0x4B, 0x61, 0x8D, 0x3E, 0xDF, 0x9B, 0xD1, 0x84, 0x38, 0x99, 0x00};
		test_encode_string2(str2, 16);
		
		const char str1[17] = {0x70, 0x3D, 0xEF, 0xE0, 0x97, 0x5F, 0x4C, 0x85, 0xA1, 0x0C, 0x10, 0x62, 0x93, 0xB5, 0xDF, 0xF0, 0x00};
		test_encode_string2(str1, 16);
		
		test_decode_string2("4f7ecbK6Zhzbj3VZbEwyXl==\0", 24);
		test_decode_string2("7KWOQAFQyExkZ9PPtd7Ftw==\0", 24);
		
		const char str3[17] = {0xec, 0xa5, 0x8e, 0x40, 0x01, 0x50, 0xc8, 0x4c, 0x64, 0x67, 0xd3, 0xcf, 0xb5, 0xde, 0xc5, 0xb7, 0x00};
		test_encode_string2(str3, 16);
		
		test_decode_string2("487447CDB1B74B618D3EDF9BD1843899\0", 32);
		
		printf("################################################################################\n");
		printf("\n");
		
	}
	else if (argc>1 && strcmp(argv[1], "T3")==0) {
	
		printf("\n");
		printf("################################################################################\n");
		printf("Some Tests3 ...\n");
		printf("\n");
		
		const char str1[17] = {0xa9, 0x50, 0x31, 0x6d, 0xc1, 0x05, 0x4e, 0x02, 0x94, 0xc8, 0xe9, 0x48, 0xca, 0x5e, 0x55, 0x04, 0x00};
		test_encode_string(str1, 16);
		test_encode_string2(str1, 16);

		test_decode_string("8Ayz56odjTBygBuhWjfrx\0", 21);
		test_decode_string("8Ayz56odjTBygBuhWjfrx===\0", 24);
	
		printf("################################################################################\n");
		printf("\n");
	
	}
	else if (argc>1 && strcmp(argv[1], "T4")==0) {
	
		printf("\n");
		printf("################################################################################\n");
		printf("Some Tests4 ...\n");
		printf("\n");
		
		test_decode_string("4f7ecbK6Zhzbj3VZbEwyXl\0", 22);
		test_decode_string2("4f7ecbK6Zhzbj3VZbEwyXl\0", 22);
		test_decode_string3("4f7ecbK6Zhzbj3VZbEwyXl\0", 22);
	
		printf("################################################################################\n");
		printf("\n");
	
	}
	
	else if (argc>1 && strcmp(argv[1], "t")==0) {
		
		printf("\n");
		printf("################################################################################\n");
		printf("DECODING - Base64-GUID e.g.(\"4f7ecbK6Zhzbj3VZbEwyXl\") - output should be one of the following...\n");
		printf("         - 487447CDB1B74B618D3EDF9BD1843899 / 703DEFE0975F4C85A10C106293B5DFF0\n");
		printf("\n");
		
		test_decode_string("4f7ecbK6Zhzbj3VZbEwyXl\0", 22);
		test_decode_string("4f7ecbK6Zhzbj3VZbEwyXl==\0", 24);
		
		test_decode_string("7KWOQAFQyExkZ9PPtd7Ftw\0", 22);
		test_decode_string("7KWOQAFQyExkZ9PPtd7Ftw==\0", 24);
		
		printf("################################################################################\n");
		printf("\n");
		
	}
	
	
	test_encode_string_function("abcde\0", 5, base64_encode_fixed);
	test_encode_string_function("abcde\0", 5, base64_encode_original);
	test_encode_string_funcset("abcde\0", 5, DT_BASE64_FLAG_NONE, base64_fixed_func_set);
	return 0;
	
}

//##############################################################################
