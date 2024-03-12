// RUN: %check_clang_tidy %s gjb8114-function-declare-different-with-definition %t

//CSR-27	函数声明中必须对参数类型进行声明,并带有变量名				R-1-1-13	gjb8114-function-declare-different-with-definition
//CSR-27 GJB 8114-2013 R-1-1-13
//违背示例
// int fun1(int c,int d);  //违背1
// // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明中必须对参数类型进行声明,并带有变量名 [gjb8114-function-declare-different-with-definition]
// int fun2(int,int);  //违背2
// // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明中必须对参数类型进行声明,并带有变量名 [gjb8114-function-declare-different-with-definition]
// int fun1(int a,double b);  //违背3
// // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明中必须对参数类型进行声明,并带有变量名 [gjb8114-function-declare-different-with-definition]
// int fun1(int a,int b,int c);  //违背4
// // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明中必须对参数类型进行声明,并带有变量名 [gjb8114-function-declare-different-with-definition]
// int fun1(double a,double b);  //违背5
// // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明中必须对参数类型进行声明,并带有变量名 [gjb8114-function-declare-different-with-definition]
// int fun1(int a,int b,double c); //违背6
// // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明中必须对参数类型进行声明,并带有变量名 [gjb8114-function-declare-different-with-definition]
// int fun1();  //违背7
// // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明中必须对参数类型进行声明,并带有变量名 [gjb8114-function-declare-different-with-definition]
// int fun1(void); //违背8
// // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明中必须对参数类型进行声明,并带有变量名 [gjb8114-function-declare-different-with-definition]
// int fun1(int a,int b);  //遵循1
// int fun3();  //遵循2
// int fun3(void); //遵循3
// int fun3(int a);  //违背9
// // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明中必须对参数类型进行声明,并带有变量名 [gjb8114-function-declare-different-with-definition]
// int fun3(int a,double b);  //违背10
// // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明中必须对参数类型进行声明,并带有变量名 [gjb8114-function-declare-different-with-definition]
// int main_1(void)
// {
//     int a,b,c,d;
//     a=10;
//     b=5;
//     c=fun1(a,b);
//     d=fun2(a,b);
//     return (0);
// }
// int fun1(int a,int b)
// {
//     int ret;
//     ret=a+b;
//     return ret;
// }
// int fun2(int a,int b)
// {
//     int ret;
//     ret=a-b;
//     return ret;
// }
// int fun3(void)
// {
//     int ret;
//     ret=1;
//     return ret;
// }
// int fun2(int,int);  //违背11
// // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明中必须对参数类型进行声明,并带有变量名 [gjb8114-function-declare-different-with-definition]
// int fun2();  //违背12
// // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明中必须对参数类型进行声明,并带有变量名 [gjb8114-function-declare-different-with-definition]
// int fun2(int a,int b,double);  //违背13
// // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明中必须对参数类型进行声明,并带有变量名 [gjb8114-function-declare-different-with-definition]
// int fun2(int a,double b);  //违背14
// // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明中必须对参数类型进行声明,并带有变量名 [gjb8114-function-declare-different-with-definition]
// int fun2(int a,int);  //违背15
// // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明中必须对参数类型进行声明,并带有变量名 [gjb8114-function-declare-different-with-definition]
// int fun3(); //遵循4
// int fun3(void);  //遵循5

int fun1(); //违背1
int fun2(int,int); //违背2
// int fun1(int a, int b); //遵循1
// int fun2(int a, int b); //遵循2
int main_1(void)
{
    int a,b,c,d;
    a=10;
    b=5;
    c=fun1(a,b);
    d=fun2(a,b);
    return (0);
}
int fun1(int a,int b)
{
    int ret;
    ret=a+b;
    return ret;
}
int fun2(int a,int b)
{
    int ret;
    ret=a-b;
    return ret;
}

