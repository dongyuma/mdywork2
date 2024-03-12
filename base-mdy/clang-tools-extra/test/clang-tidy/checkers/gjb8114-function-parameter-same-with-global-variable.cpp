// RUN: %check_clang_tidy %s gjb8114-function-parameter-same-with-global-variable %t

//CSR-251	禁止函数形参与全局变量同名		4.15.1.3	gb5369-Function-Parameters	R-1-13-2	gjb8114-function-parameter-same-with-global-variable
//CSR-251 GJB 8114-2013 R-1-13-2
//违背示例
int func(int sign)  //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 禁止函数形参与全局变量同名 [gjb8114-function-parameter-same-with-global-variable]
{
    int local=0;
    local=sign;
    //...
    return local;
}
int sign=0;
int func2(int sign)  //违背2
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止函数形参与全局变量同名 [gjb8114-function-parameter-same-with-global-variable]
{
    return sign+1;
}
int data=3;
int func3(int data)  //违背3
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止函数形参与全局变量同名 [gjb8114-function-parameter-same-with-global-variable]
{
    return 2;
}
int func4()
{
    int data=7;  //遵循1
}
int main_2(void)
{
    //...
    return (0);
}