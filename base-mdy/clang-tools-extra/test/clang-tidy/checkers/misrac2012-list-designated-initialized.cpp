// RUN: %check_clang_tidy %s misrac2012-list-designated-initialized %t

#include <stdint.h>

static uint16_t glob_arr[3] = { 0u };
static uint16_t *glob_p = glob_arr;

static void f4(void)
{
	uint16_t a[2] = { [0] = *glob_p++,[0] = 1u };     //违背1
}

void R_9_4(void)
{
	int32_t a1[5] = { -5, -4, -3, -2, -1 };                      //遵循
	int32_t a2[5] = { [0] = -5,[1] = -4,[2] = -3,[3] = -2,[4] = -1 };  //遵循
	int32_t a3[5] = { [0] = -5,[1] = -4,[2] = -3,[2] = -2,[4] = -1 };  //违背2

	struct mystruct
	{
		int32_t a;
		int32_t b;
		int32_t c;
		int32_t d;
	};


	struct mystruct s1 = { 100, -1, 42, 999 };

	struct mystruct s2 = { .a = 100, .b = -1, .c = 42, .d = 999 };        //遵循


	struct mystruct s3 = { .a = 100, .b = -1, .a = 42, .d = 999 };         //违背3

	f4();

}
