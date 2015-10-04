#ifndef BASE64_FUNCTION_H
#define BASE64_FUNCTION_H

#include <stdio.h>

//typedef const char (base64_encode_table)[64];

typedef int (base64_encode_length_func)(int aiSrcLen, int aiEncodeFlags);
typedef int (base64_encode_func)(const char* apbySrc, int aiSrcLen, char* apsDest, int* apiDestMaxCount, int aiEncodeFlags);

typedef int (base64_decode_char_func)(unsigned int auiChar);
typedef int (base64_decode_func)(const char* apsSrc, int aiSrcMaxCount, char* apbyDest, int* apiDestLen);

typedef struct {
	base64_encode_length_func *GetRequiredEncodeLength;
	base64_encode_func *Encode;
	base64_decode_char_func *Decode;
	base64_decode_func *Decode;
} base64_func_set;

#endif // BASE64_FUNCTION_H
