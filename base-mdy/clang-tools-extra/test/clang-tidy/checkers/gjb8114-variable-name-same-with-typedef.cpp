// RUN: %check_clang_tidy %s gjb8114-variable-name-same-with-typedef %t

//CSR-270	禁止变量名与typedef自定义的类型名同名				R-1-13-6	gjb8114-variable-name-same-with-typedef	Rule 5.6	misrac2012-UniqueTypedef
//CSR-270 GJB 8114-2013 R-1-13-6
//违背示例
typedef unsigned int TData;
unsigned int fun(unsigned int var);
unsigned int fun1(int data)
{
    float TData=9.6;  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止变量名与typedef自定义的类型名同名 [gjb8114-variable-name-same-with-typedef]
}
int main_2(void)
{
    unsigned int datax=1,datay;
    datay=fun(datax);
    return (0);
}
unsigned int fun(unsigned int var)
{
    unsigned int TData=var+1;  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止变量名与typedef自定义的类型名同名 [gjb8114-variable-name-same-with-typedef]
    short TData1=5;  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止变量名与typedef自定义的类型名同名 [gjb8114-variable-name-same-with-typedef]
    return (2*TData);
}
void fun2(int TData1)  //违背4
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止变量名与typedef自定义的类型名同名 [gjb8114-variable-name-same-with-typedef]
{
    TData1++;
}
typedef double TData1;
double fun3(void)
{
    int TData1=4;  //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止变量名与typedef自定义的类型名同名 [gjb8114-variable-name-same-with-typedef]
    return 0.9;
}