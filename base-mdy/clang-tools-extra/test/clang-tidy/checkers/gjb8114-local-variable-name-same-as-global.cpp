// RUN: %check_clang_tidy %s gjb8114-local-variable-name-same-as-global %t

//CSR-250	禁止局部变量与全局变量同名		4.15.1.2	gb5369-Variablename-Duplication	R-1-13-1	gjb8114-Variablename-Duplication		 
//CSR-250 GJB 8114-2013 R-1-13-1
//违背示例
int main_1(void)
{
    int local=0;
    int sign=0;  //违背1
     // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止局部变量与全局变量同名 [gjb8114-local-variable-name-same-as-global]
    //...
    return (0);
}
int sign=0;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 与局部变量冲突的名为sign的全局变量位置 [gjb8114-local-variable-name-same-as-global]
int main_2(void)
{
    int local=0;
    int sign=0;  //违背2
     // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止局部变量与全局变量同名 [gjb8114-local-variable-name-same-as-global]
    //...
    return (0);
}
int main_3(void)
{
    int local=0;
    int sign=0;  //违背3
     // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止局部变量与全局变量同名 [gjb8114-local-variable-name-same-as-global]
    //...
    return (0);
}