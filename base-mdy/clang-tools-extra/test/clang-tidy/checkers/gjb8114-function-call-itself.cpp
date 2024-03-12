// RUN: %check_clang_tidy %s gjb8114-function-call-itself %t

//CSR-208	禁止使用直接或间接自调用函数				R-1-7-16	gjb8114-Banselfcallfunc	Rule 17.2	misrac2012-Banselfcallfunc
//CSR-208  GJB 8114-2013 R-1-7-16
#include<stdio.h>
int Factorial(int n) {
    if (n == 1)
        return 1;
    if(n==9)
    {
        int x=Factorial(7);  //违背1
        // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 禁止递归 [gjb8114-function-call-itself]
        x++;
    }
    int number=5;
    while(number>0)
    {
        number--;
        Factorial(3);    //违背2
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止递归 [gjb8114-function-call-itself]
    }
    return n * Factorial(n - 1);  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 禁止递归 [gjb8114-function-call-itself]
}
void move(int n,char a,char b,char c)
{
    if(n==1)
        printf("\t%c->%c\n",a,c);
    else
    {   
        move(n-1,a,c,b);       //违背4
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止递归 [gjb8114-function-call-itself]
        printf("\t%c->%c\n",a,c);
        move(n-1,b,a,c);       //违背5
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止递归 [gjb8114-function-call-itself]
    }   
}
int main(void)
{
    int circle=150;
    int result=Factorial(150);          //遵循1
    main();  //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止递归 [gjb8114-function-call-itself]
    move(10000,'a','b','c');           //遵循2
    return (0);
}

