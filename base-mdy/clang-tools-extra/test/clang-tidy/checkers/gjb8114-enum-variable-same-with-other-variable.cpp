// RUN: %check_clang_tidy %s gjb8114-enum-variable-same-with-other-variable %t

//CSR-249	禁止变量名与枚举元素同名		4.15.1.1	gb5369-Function-Parameters	R-1-13-5	gjb8114-enum-variable-same-with-other-variable
//CSR-249 GJB 8114-2013 R-1-13-5
//违背示例
enum Name_type {first=0,second} EnumVar;
int main_1(void)
{
    unsigned int second=0;  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:18: warning: 禁止变量名与枚举元素同名 [gjb8114-enum-variable-same-with-other-variable]
    double first=3.14;  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止变量名与枚举元素同名 [gjb8114-enum-variable-same-with-other-variable]
    short third=7;  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止变量名与枚举元素同名 [gjb8114-enum-variable-same-with-other-variable]
    int fourth=9;  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止变量名与枚举元素同名 [gjb8114-enum-variable-same-with-other-variable]
    float fifth=5;  //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止变量名与枚举元素同名 [gjb8114-enum-variable-same-with-other-variable]
    int sixth;  //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止变量名与枚举元素同名 [gjb8114-enum-variable-same-with-other-variable]
    double seventh;  //违背7
    // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止变量名与枚举元素同名 [gjb8114-enum-variable-same-with-other-variable]
    return (0);
}
enum Name_type1{third=3,fourth=4,fifth=5,sixth=6,seventh};
