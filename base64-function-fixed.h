#ifndef BASE64_FUNCTION_FIXED_H
#define BASE64_FUNCTION_FIXED_H

#include <stdio.h>

#include "base64-function.h"
#include "base64-funcset.h"

//##############################################################################

extern base64_encode_table base64_encode_table_fixed;

extern base64_encode_length_func base64_encode_length_fixed;
extern base64_encode_func base64_encode_fixed;

extern base64_decode_char_func base64_decode_char_fixed;
extern base64_decode_func base64_decode_fixed;

extern base64_func_set base64_fixed_func_set;

//##############################################################################

extern base64_func_set base64_fixed_func_set;

//##############################################################################

#endif // BASE64_FUNCTION_FIXED_H
