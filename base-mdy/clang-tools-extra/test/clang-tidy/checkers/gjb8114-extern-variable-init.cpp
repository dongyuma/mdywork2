// RUN: %check_clang_tidy %s gjb8114-extern-variable-init %t

//CSR-32  GJB 8114-2013 R-1-1-19
// 禁止使用extern声明对变量初始化

//违背示例1
extern int a = 1;  //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止使用extern声明对变量初始化 [gjb8114-extern-variable-init]
int main(void)
{
    a=3;
    return (0);
}

//遵循示例1
extern int a_v2;  //遵循1
int main_v2(void)
{
    a_v2=3;
    return (0);
}

//违背示例2
extern int a_v3 = 1;  //违背2
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止使用extern声明对变量初始化 [gjb8114-extern-variable-init]
int main_v3(void)
{
    a_v3=3;
    return (0);
}
