// RUN: %check_clang_tidy %s gjb8114-pointer-beyond-two-levels %t

//CSR-137	禁止指针的指针超过两级		4.4.1.2	BinaryOperatorChecker	R-1-3-1	gjb8114-pointer-beyond-two-levels		
//CSR-137 GJB 8114-2013 R-1-3-1
//违背示例
#include<stdlib.h>
int main(void)
{
    unsigned int array[5]={0,1,2,3,4};
    unsigned int *p1_ptr=NULL,**p2_ptr=NULL;
    unsigned int ***p3_ptr=NULL;  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:21: warning: 禁止指针的指针超过两级 [gjb8114-pointer-beyond-two-levels]
    unsigned int data[5];
    unsigned int i;
    p1_ptr=array;
    p2_ptr=&p1_ptr;
    p3_ptr=&p2_ptr;
    for(i=0;i<5;i++)
    {
        data[i]=*(**p3_ptr+i);
    }
    return (0);
}