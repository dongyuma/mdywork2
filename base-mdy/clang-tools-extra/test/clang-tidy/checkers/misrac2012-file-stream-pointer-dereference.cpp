// RUN: %check_clang_tidy %s misrac2012-file-stream-pointer-dereference %t
#include <stdio.h>

void R_22_05()
{
	int a = 10;
	int *ip = &a;
	FILE *pf1 = tmpfile();
	const FILE *pf2;


	FILE  f3, f4;

	pf2 = pf1;      //遵循1
	f3 = *pf2;      //违背1
	// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 禁止对文件流指针进行解引用 [misrac2012-file-stream-pointer-dereference]
	f4 = *pf1;      //违背2
	// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 禁止对文件流指针进行解引用 [misrac2012-file-stream-pointer-dereference]


	int c = *ip;    //遵循2
}