// RUN: %check_clang_tidy %s gjb8114cpp-fixed-array-param-passing %t
#include <stdio.h>

void badFixArrayPassing1(int p[10])
{
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		p[i] = p[i] + 1;
	}
}

void badFixArrayPassing2(int *p)
{
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		p[i] = p[i] + 1;
	}
}


void badFixArrayPassing3(int(*p)[10])
{
	int i = 0;
	int j = 0;
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			p[i][j] = p[i][j] + 1;
		}
	}
}


void goodFixArrayPassing1(int(&p)[10])
{
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		p[i] = p[i] + 1;
	}
}


void goodFixArrayPassing2(int(&p)[10][10])
{
	int i = 0;
	int j = 0;
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			p[i][j] = p[i][j] + 1;
		}
	}
}

void R_2_7_1()
{
	int a[10] = { 0,1,2,3,4,5,6,7,8,9 };
	int ma[10][10] = { {0} };
	badFixArrayPassing1(a); //违背1
	// CHECK-MESSAGES: :[[@LINE-1]]:22: warning: 固定长度数组变量做为参数时,函数形参应使用引用 [gjb8114cpp-fixed-array-param-passing]
	badFixArrayPassing2(a); //违背2
	// CHECK-MESSAGES: :[[@LINE-1]]:22: warning: 固定长度数组变量做为参数时,函数形参应使用引用 [gjb8114cpp-fixed-array-param-passing]
	badFixArrayPassing3(ma); //违背3
	// CHECK-MESSAGES: :[[@LINE-1]]:22: warning: 固定长度数组变量做为参数时,函数形参应使用引用 [gjb8114cpp-fixed-array-param-passing]
	goodFixArrayPassing1(a); //遵循1
	goodFixArrayPassing2(ma);//遵循2
}