#ifndef BASE64_FUNCTION_H
#define BASE64_FUNCTION_H

#include <stdio.h>

//##############################################################################

enum DT_BASE64_FLAGS {
	DT_BASE64_FLAG_NONE = 0,
	DT_BASE64_FLAG_NOPAD = 1,
	DT_BASE64_FLAG_NOCRLF = 2
};

//##############################################################################

typedef const char (*base64_encode_table);

typedef int (base64_encode_length_func)(int aiSrcLen, int aiEncodeFlags);
typedef int (base64_encode_func)(const char* apbySrc, int aiSrcLen, char* apsDest, int* apiDestMaxCount, int aiEncodeFlags);

typedef int (base64_decode_char_func)(unsigned int auiChar);
typedef int (base64_decode_func)(const char* apsSrc, int aiSrcMaxCount, char* apbyDest, int* apiDestLen);

//##############################################################################

#endif // BASE64_FUNCTION_H
