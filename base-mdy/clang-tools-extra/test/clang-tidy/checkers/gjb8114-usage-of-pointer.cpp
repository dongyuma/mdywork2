// RUN: %check_clang_tidy %s gjb8114-usage-of-pointer %t

//CSR-136	禁止将参数指针赋值给过程指针		4.4.1.1		R-1-3-3	gjb8114-usage-of-pointer	
//CSR-136 GJB 8114-2013 R-1-3-3
//违背示例
#include<malloc.h>
#include<stdio.h>
int pfun(int *pa);
int main(void)
{
    int data;
    int result;
    result=pfun(&data);
    return (0);
}
int pfun(int *pa)
{
    int i=10;
    i=i+1;
    int z=9;
    pa=&i;  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止对参数指针进行赋值 [gjb8114-usage-of-pointer]
    *pa=5;  //遵循1
    pa=(int *)malloc(sizeof(int)); //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止对参数指针进行赋值 [gjb8114-usage-of-pointer]
    int *data=(int *)malloc(sizeof(int));
    pa=data; //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止对参数指针进行赋值 [gjb8114-usage-of-pointer]
    (pa)=data;  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: 禁止对参数指针进行赋值 [gjb8114-usage-of-pointer]
    if(i==11)
    {
        int *pa;
        pa=&i;  //遵循2
    }
    pa=&z;  //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止对参数指针进行赋值 [gjb8114-usage-of-pointer]
    if(pa=&z) //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 禁止对参数指针进行赋值 [gjb8114-usage-of-pointer]
    {
        i++;
    }
    return i;
}
int pfun1(int **pa)
{
    int i=10;
    i=i+1;
    *pa=&i;  //遵循1
    int *temp=(int *)malloc(sizeof(int));
    int *swap;
    swap=temp;  //遵循2
    swap=&i;  //遵循3
    return i;
}
int pfun2(int pb)
{
    return 1;
}

int pfun3(int a, int*pb, int *pc)
{
    return 1;
}
