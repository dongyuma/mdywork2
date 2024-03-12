// RUN: %check_clang_tidy %s gjb8114-floatliteral-to-int %t

//CSR-255	禁止将浮点常数赋给整型变量				R-1-6-1	gb8114-floatliteral-to-int	
//CSR-255  GJB 8114-2013 R-1-6-1
//违背示例
int main_1(void)
{
    int idata = 3;  //遵循1
    int idata1=(0.142857);  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止将浮点常数赋值给整型变量 [gjb8114-floatliteral-to-int]
    int idata2=0.38387;  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止将浮点常数赋值给整型变量 [gjb8114-floatliteral-to-int]
    unsigned int idata4=0.1212;  //违背2-1
    // CHECK-MESSAGES: :[[@LINE-1]]:18: warning: 禁止将浮点常数赋值给整型变量 [gjb8114-floatliteral-to-int]
    float idata5=0.13;  //遵循3
    int idata6=idata5;  //遵循4
    double number=1.7789;  //遵循5
    int idata7=35;  //遵循6
    idata6=1.23;  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止将浮点常数赋值给整型变量 [gjb8114-floatliteral-to-int]
    idata=(1.2343);  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止将浮点常数赋值给整型变量 [gjb8114-floatliteral-to-int]
    idata=1e7;  //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止将浮点常数赋值给整型变量 [gjb8114-floatliteral-to-int]
    int idata8=1e5;  //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止将浮点常数赋值给整型变量 [gjb8114-floatliteral-to-int]
    return (0);
}
