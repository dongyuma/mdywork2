// RUN: %check_clang_tidy %s gjb8114-return-local-variable-address %t

//CSR-140	禁止将局部变量地址做为函数返回值返回				R-1-3-4	gjb8114-return-local-variable-address
//CSR-140 GJB 8114-2013 R-1-3-4
//违背示例
#include<stdlib.h>
unsigned int * pfun(unsigned int *pa);
unsigned int Gdata=0;
int main(void)
{
    unsigned int data;
    unsigned int *result=NULL;
    result=pfun(&data);
    return (0);
}
unsigned int * pfun(unsigned int *pa)
{
    unsigned int temp=0;
    *pa=10;
    temp=*pa;
    return &temp;  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止将局部变量地址做为函数返回值返回 [gjb8114-return-local-variable-address]
}
unsigned int * pfun1(unsigned int *pa)
{
    unsigned int temp=0;
    *pa=10;
    temp=*pa;
    return &Gdata;  //遵循1
}
unsigned int * pfun2(unsigned int *pa)
{
    unsigned int temp=0;
    *pa=10;
    temp=*pa;
    return (&temp);  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 禁止将局部变量地址做为函数返回值返回 [gjb8114-return-local-variable-address]
}
unsigned int * pfun3(unsigned int *pa)
{
    unsigned int temp=0;
    *pa=10;
    temp=*pa;
    return (&(temp));  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 禁止将局部变量地址做为函数返回值返回 [gjb8114-return-local-variable-address]
}
unsigned int pfun4(unsigned int *pa)
{
    *pa=10;
    return *pa;
}
static char *R_1_3_4_1()
{
    char charString[] = "helperBad string";
    char *ptrCharString;
    ptrCharString = &charString[1];
    return ptrCharString;   //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止将局部变量地址做为函数返回值返回 [gjb8114-return-local-variable-address]
}
static char *R_1_3_4_2()
{
    char CharTest='a';
    char *ptrCharString;
    ptrCharString = &CharTest;
    return ptrCharString;   //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止将局部变量地址做为函数返回值返回 [gjb8114-return-local-variable-address]
}
static char *R_1_3_4_3()
{
    char CharTest='a';
    char *ptrCharString=&CharTest;
    return ptrCharString;   //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止将局部变量地址做为函数返回值返回 [gjb8114-return-local-variable-address]
}
static char *R_1_3_4_4()
{
    char CharTest[]="abcdefg";
    return &CharTest[2];   //违背7
    // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止将局部变量地址做为函数返回值返回 [gjb8114-return-local-variable-address]
}
static char *R_1_3_4_5()
{
    char CharTest[]="abcdefg";
    char *ptrCharString=&CharTest[0];
    return ptrCharString;   //违背8
    // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止将局部变量地址做为函数返回值返回 [gjb8114-return-local-variable-address]
}
char StringTest[]="abcdefg";
static char *R_1_3_4_6()
{
    char *ptrCharString=&StringTest[0];
    return ptrCharString;   //遵循
}
/*
//遵循示例
#include<stdlib.h>
unsigned int * pfun(unsigned int *pa);
unsigned int Gdata=0;
int main(void)
{
    unsigned int data;
    unsigned int *result=NULL;
    result=pfun(&data);
    return (0);
}
unsigned int * pfun(unsigned int *pa)
{
    *pa=10;
    Gdata=*pa;
    return &Gdata;  //遵循1
}
*/
