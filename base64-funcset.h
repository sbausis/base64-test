#ifndef BASE64_FUNCTION_H
#define BASE64_FUNCTION_H

#include <stdio.h>

#include "base64-function.h"

//##############################################################################

typedef struct {
	base64_encode_table *gpsBase64EncodingTable;
	base64_encode_length_func *GetRequiredEncodeLength;
	base64_encode_func *Encode;
	base64_decode_char_func *DecodeChar;
	base64_decode_func *Decode;
} base64_func_set;

//##############################################################################

extern int PRINT_DEBUG;

extern base64_encode_table gpsBase64EncodingTable;
extern base64_encode_length_func *GetRequiredEncodeLength;
extern base64_encode_func *Encode;
extern base64_decode_char_func *DecodeChar;
extern base64_decode_func *Decode;

extern void set_func_set(base64_func_set funcset);

//##############################################################################

#endif // BASE64_FUNCTION_H
