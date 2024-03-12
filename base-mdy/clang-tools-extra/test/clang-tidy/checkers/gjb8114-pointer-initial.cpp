// RUN: %check_clang_tidy %s gjb8114-pointer-initial %t

//CSR-143	动态分配的指针变量定义时如未被分配空间,必须初始化为NULL				R-1-3-7	gjb8114-pointer-initial
//CSR-143 GJB 8114-2013 R-1-3-7
//CSR-145 GJB 8114-2013 R-1-3-9 空指针必须使用NULL,禁止使用整数型0
//违背示例
#include<stdlib.h>
#include<malloc.h>
int main_1(void)
{
    int *x;   
    int *pointer=0;  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 空指针必须使用NULL,禁止使用整型数0 [gjb8114-pointer-initial]
    int *pointer2=(0);  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 空指针必须使用NULL,禁止使用整型数0 [gjb8114-pointer-initial]
    int a=pointer==NULL?0:1;
    if(pointer!=0)  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 空指针必须使用NULL,禁止使用整型数0 [gjb8114-pointer-initial]
    {
        *x=1;
    }
    if(NULL!=pointer)  //遵循1
    {
        *x=2;
    }
    if(pointer!=NULL)
    {
        *x=150;
    }
    if(x!=nullptr)  //遵循2
    {
        *x=3;
    }
    if(0!=pointer)  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 空指针必须使用NULL,禁止使用整型数0 [gjb8114-pointer-initial]
    {
        *x=4;
    }
    if(pointer!=0 && NULL!=x)  //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 空指针必须使用NULL,禁止使用整型数0 [gjb8114-pointer-initial]
    {
        *pointer=5;
    }
    int *y=NULL;  //遵循3
    int *z=(int *)malloc(sizeof(int)); //遵循4
    int *w=x;  //遵循5
    double data1;  //遵循6
    int number=3;
    double number1=7.4;
    return (0);
}

// #include<stdlib.h>
// #include<malloc.h>
int main_2(void)
{
    int *x;    //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 动态分配的指针变量定义时如未分配空间,必须初始化为NULL [gjb8114-pointer-initial]
    x=(int *)malloc(sizeof(int));
    if(NULL != x)
    {
        *x = 1;
    }
    return (0);
}
int main_3(void)
{
    int *x=NULL;    //遵循1
    x=(int *)malloc(sizeof(int));
    if(NULL != x)
    {
        *x = 1;
    }
    return (0);
}

