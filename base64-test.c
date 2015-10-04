
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "base64-funcset.h"
#include "base64-function.h"
#include "base64-function-original.h"
#include "base64-function-fixed.h"

//##############################################################################

void test_hex_print(const char* string, int len) {
	printf("[%d]:0x", len);
	for (int i=0; i<len; i++) {
		printf("%02x", (string[i]&0xff));
	}
}

//##############################################################################

void test_encode_string_funcset(const char* string, int len, base64_func_set funcset, int flags) {
	
	set_func_set(funcset);
	
	printf(" -> encode : [%d]:\"%s\" => ", len, string);
	
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

void test_decode_string_funcset(const char* string, int len, base64_func_set funcset) {
	
	set_func_set(funcset);
	
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

//##############################################################################

void test_encode_orignal() {
	
	printf("\n################################################################################\n");
	printf("ENCODING - test string - output should be \"YWJjZGU=\" ...\n\n");
	test_encode_string_funcset("abcde\0", 5, base64_original_func_set, DT_BASE64_FLAG_NONE);
	
}
void test_encode_orignal_short() {
	
	printf("\n################################################################################\n");
	printf("ENCODING - test effect on shorter strings - output should be shorter than \"YWJjZGU\" ...\n\n");
	test_encode_string_funcset("abcd\0", 4, base64_original_func_set, DT_BASE64_FLAG_NONE);
	test_encode_string_funcset("abc\0", 3, base64_original_func_set, DT_BASE64_FLAG_NONE);
	test_encode_string_funcset("ab\0", 2, base64_original_func_set, DT_BASE64_FLAG_NONE);
	test_encode_string_funcset("a\0", 1, base64_original_func_set, DT_BASE64_FLAG_NONE);
	
}
void test_encode_orignal_long() {
	
	printf("\n################################################################################\n");
	printf("ENCODING - test effect on longer strings - output should be bigger than \"YWJjZGU\" ...\n\n");
	test_encode_string_funcset("abcdefghijklmnopqrstuvwxyz\0", 26, base64_original_func_set, DT_BASE64_FLAG_NONE);
	test_encode_string_funcset("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\0", 52, base64_original_func_set, DT_BASE64_FLAG_NONE);
	test_encode_string_funcset("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/\0", 64, base64_original_func_set, DT_BASE64_FLAG_NONE);
	
}

void test_encode_fixed() {
	
	printf("\n################################################################################\n");
	printf("ENCODING - test string - output should be \"YWJjZGU=\" ...\n\n");
	test_encode_string_funcset("abcde\0", 5, base64_fixed_func_set, DT_BASE64_FLAG_NONE);
	
}
void test_encode_fixed_short() {
	
	printf("\n################################################################################\n");
	printf("ENCODING - test effect on shorter strings - output should be shorter than \"YWJjZGU\" ...\n\n");
	test_encode_string_funcset("abcd\0", 4, base64_fixed_func_set, DT_BASE64_FLAG_NONE);
	test_encode_string_funcset("abc\0", 3, base64_fixed_func_set, DT_BASE64_FLAG_NONE);
	test_encode_string_funcset("ab\0", 2, base64_fixed_func_set, DT_BASE64_FLAG_NONE);
	test_encode_string_funcset("a\0", 1, base64_fixed_func_set, DT_BASE64_FLAG_NONE);
	
}
void test_encode_fixed_long() {
	
	printf("\n################################################################################\n");
	printf("ENCODING - test effect on longer strings - output should be bigger than \"YWJjZGU\" ...\n\n");
	test_encode_string_funcset("abcdefghijklmnopqrstuvwxyz\0", 26, base64_fixed_func_set, DT_BASE64_FLAG_NONE);
	test_encode_string_funcset("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\0", 52, base64_fixed_func_set, DT_BASE64_FLAG_NONE);
	test_encode_string_funcset("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/\0", 64, base64_fixed_func_set, DT_BASE64_FLAG_NONE);
	
}

int main (int argc, char *argv[]) {
	
	if (argc>2 && strcmp(argv[2], "debug")==0) {
		PRINT_DEBUG = 1;
	}
	
	if (argc>1 && strcmp(argv[1], "q")==0) return 0;
	
	else if (argc>1 && strcmp(argv[1], "e")==0)  test_encode_orignal();
	else if (argc>1 && strcmp(argv[1], "e2")==0) test_encode_orignal_short();
	else if (argc>1 && strcmp(argv[1], "e3")==0) test_encode_orignal_long();
	
	else if (argc>1 && strcmp(argv[1], "E")==0)  test_encode_fixed();
	else if (argc>1 && strcmp(argv[1], "E2")==0) test_encode_fixed_short();
	else if (argc>1 && strcmp(argv[1], "E3")==0) test_encode_fixed_long();
	
	else if (argc>1 && strcmp(argv[1], "d")==0)  test_decode_orignal();
	else if (argc>1 && strcmp(argv[1], "d2")==0) test_decode_orignal_short();
	else if (argc>1 && strcmp(argv[1], "d3")==0) test_decode_orignal_long();
	
	else if (argc>1 && strcmp(argv[1], "D")==0)  test_decode_fixed();
	else if (argc>1 && strcmp(argv[1], "D2")==0) test_decode_fixed_short();
	else if (argc>1 && strcmp(argv[1], "D3")==0) test_decode_fixed_long();
	
	/*if (argc>1 && strcmp(argv[1], "e2")==0) {
		
		test_encode_orignal();
		test_encode_orignal_short();
		test_encode_orignal_long();
		
		printf("################################################################################\n");
		printf("\n");
		
	}
	else if (argc>1 && strcmp(argv[1], "e2")==0) {
		
		test_encode_fixed();
		test_encode_fixed_short();
		test_encode_fixed_long();
		
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
	*/
	//test_encode_string_funcset("abcde\0", 5, base64_original_func_set, DT_BASE64_FLAG_NONE);
	//test_encode_string_funcset("abcde\0", 5, base64_fixed_func_set, DT_BASE64_FLAG_NONE);
	//test_decode_string_funcset("YWJjZGU=\0", 8, base64_original_func_set);
	//test_decode_string_funcset("YWJjZGU=\0", 8, base64_fixed_func_set);
	
	return 0;
	
}

//##############################################################################
