// RUN: %check_clang_tidy %s misrac2012-escape-sequence-termination %t

#include <string.h>
#include <stdint.h>


void R_04_01(void)
{
	const char *s1 = "\x41g";     //违背1
	const char *s2 = "\x41" "g";  //遵循
	const char *s3 = "\x41\x67";  //遵循 以另一个转义字符终止

	s3 = "\\123"; //遵循

	int32_t c1 = '\141t';       //违背2
	int32_t c2 = '\141\t';      //遵循
}