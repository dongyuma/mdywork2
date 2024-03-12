// RUN: %check_clang_tidy %s misrac2012-restrict-qualifier-pointer %t
#include <stdio.h>
#include <stdint.h>
#include <string.h>

//#include "R_08_14.h"

static void R_08_14_bad1(void *restrict p) //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:42: warning: 禁止使用restrict修饰符 [misrac2012-restrict-qualifier-pointer]
{

}

static void R_08_14_bad2(const void *restrict p) //违背2
// CHECK-MESSAGES: :[[@LINE-1]]:47: warning: 禁止使用restrict修饰符 [misrac2012-restrict-qualifier-pointer]
{

}

static void R_08_14_good(void *p)  //遵循1
{
	char *ptr = NULL; //遵循2
	int test;
}

