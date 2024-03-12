// RUN: %check_clang_tidy %s gjb8114-use-function-as-pointer %t

//CSR-139	函数指针的使用必须加以&明确说明				R-1-3-2	gjb8114-use-function-as-pointer
//CSR-139 GJB 8114-2013 R-1-3-2
//违背示例
#include<stdlib.h>
int fun(int para1,int para2);
void fuc(int nValue,int (*pointer)(int,int))
{
    nValue--;
}
void fud(int nValue,int (*pointer)(int,int),int (*pointer1)(int,int))
{
    nValue--;
}
int main(void)
{
    int a=2,b=1,c=0;
    if(NULL==fun)  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: 函数指针的使用必须加以&说明 [gjb8114-use-function-as-pointer]
    {
        return (-1);
    }
    else
    {
        int(*p1)(int,int)=&fun;  //遵循1
        int(*p2)(int,int)=fun;  //违背2
        // CHECK-MESSAGES: :[[@LINE-1]]:27: warning: 函数指针的使用必须加以&说明 [gjb8114-use-function-as-pointer]
        c=p1(a,b);
    }
    int (*pf)(int,int);
    pf=fun;  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 函数指针的使用必须加以&说明 [gjb8114-use-function-as-pointer]
    pf=&fun; //遵循2
    fuc(1,fun);  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 函数指针的使用必须加以&说明 [gjb8114-use-function-as-pointer]
    fud(3,fun,pf);  //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 函数指针的使用必须加以&说明 [gjb8114-use-function-as-pointer]
    fud(3,&fun,fun);  //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 函数指针的使用必须加以&说明 [gjb8114-use-function-as-pointer]
    if(NULL==pf)
    {
        return (-1);
    }
    if(nullptr==pf)
    {
        return (-1);
    }
    if(fun==pf)  //违背7
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 函数指针的使用必须加以&说明 [gjb8114-use-function-as-pointer]
    {
        return (-1);
    }
    if(fun==nullptr)  //违背8
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 函数指针的使用必须加以&说明 [gjb8114-use-function-as-pointer]
    {
        return (-1);
    }
    if(fun==NULL)  //违背9
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 函数指针的使用必须加以&说明 [gjb8114-use-function-as-pointer]
    {
        return (-1);
    }
    if(&fun==pf)  //遵循3
    {
        return (-1);
    }
    int(*p3)(int,int);
    p3=&fun;  //遵循4
    p3=fun;  //违背10
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 函数指针的使用必须加以&说明 [gjb8114-use-function-as-pointer]
    p3=pf=fun;  //违背11
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 函数指针的使用必须加以&说明 [gjb8114-use-function-as-pointer]
    return (0);
}
int fun(int para1,int para2)
{
    return (para1-para2);
}

/*
//遵循示例
#include<stdlib.h>
int fun(int para1,int para2);
int main(void)
{
    int a=2,b=1,c=0;
    if(NULL==&fun)  //遵循1
    {
        return (-1);
    }
    else
    {
        int(*p)(int,int)=&fun;  //遵循2
        c=p(a,b);
    }
    return (0);
}
int fun(int para1,int para2)
{
    return (para1-para2);
}
*/