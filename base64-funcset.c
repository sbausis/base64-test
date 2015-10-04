
#include "base64-funcset.h"

//##############################################################################

int PRINT_DEBUG = 0;

//##############################################################################

base64_encode_table gpsBase64EncodingTable;
base64_encode_length_func *GetRequiredEncodeLength;
base64_encode_func *Encode;
base64_decode_char_func *DecodeChar;
base64_decode_func *Decode;

void set_func_set(base64_func_set funcset) {
	gpsBase64EncodingTable = *funcset.gpsBase64EncodingTable;
	GetRequiredEncodeLength = funcset.GetRequiredEncodeLength;
	Encode = funcset.Encode;
	DecodeChar = funcset.DecodeChar;
	Decode = funcset.Decode;
}

//##############################################################################
