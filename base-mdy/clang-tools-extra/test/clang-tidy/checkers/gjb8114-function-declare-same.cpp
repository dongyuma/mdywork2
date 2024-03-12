// RUN: %check_clang_tidy %s gjb8114-function-declare-same %t

//CSR-204	函数声明和函数定义中的参数类型必须一致				R-1-7-5	gjb8114-function-declare-same
//CSR-204 GJB 8114-2013 R-1-7-5
//违背示例
int comp(float data1,float data2);  //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明和函数定义中的参数类型必须一致 [gjb8114-function-declare-same]
int comp1(int data1,float data2);  //违背2
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明和函数定义中的参数类型必须一致 [gjb8114-function-declare-same]
int comp2(float data1,float data2,float data3);  //遵循1
int comp3(char data1,float data2,float data3);  //违背3
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明和函数定义中的参数类型必须一致 [gjb8114-function-declare-same]
int comp4(int data1,long data2,short data3);  //违背4
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明和函数定义中的参数类型必须一致 [gjb8114-function-declare-same]
int main(void)
{
    int v,d,h;
    v=10;
    d=20;
    h=comp(v,d);
    return (0);
}
int comp(int data1,int data2)  //违背5
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明和函数定义中的参数类型必须一致 [gjb8114-function-declare-same]
{
    int data;
    data=2*data1+data2;
    return data;
}
int comp1(int data1,float data2,int data3)  //违背6
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明和函数定义中的参数类型必须一致 [gjb8114-function-declare-same]
{
    return data3;
}
int comp2(float data1,float data2,float data3)  //遵循2
{
    return 5;
}
int comp3(int data1,float data2,float data3)  //违背7
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明和函数定义中的参数类型必须一致 [gjb8114-function-declare-same]
{
    return 5;
}
int comp4(int data1,int data2,int data3)  //违背8
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明和函数定义中的参数类型必须一致 [gjb8114-function-declare-same]
{
    return 5;
}