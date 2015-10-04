#ifndef BASE64_TEST_H
#define BASE64_TEST_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "base64-function-original.h"
#include "base64-function-fixed.h"

enum DT_BASE64_FLAGS {
	DT_BASE64_FLAG_NONE = 0,
	DT_BASE64_FLAG_NOPAD = 1,
	DT_BASE64_FLAG_NOCRLF = 2
};

extern int PRINT_DEBUG;
//extern const char gpsBase64EncodingTable[64];

//extern int GetRequiredEncodeLength(int aiSrcLen, int aiEncodeFlags/*DT_BASE64_FLAG_NONE*/);
//extern int DecodeChar(unsigned int auiChar);

#endif // BASE64_TEST_H
