// RUN: %check_clang_tidy %s gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator %t

//CSR-259	禁止在运算表达式或函数调用参数中使用++或--操作符				R-1-6-5	gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator
//CSR-259 GJB 8114-2013 R-1-6-5
//违背示例
int fun(int p);
int main(void)
{
    int x=1,y=2,z=3;
    int r;
    y=y+(x++);  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    z=z+(++y);  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    z=fun(--z); //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    r=fun(z--); //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    int data1=++x; //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    data1=++x+(++z)+y++; //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    // CHECK-MESSAGES: :[[@LINE-2]]:16: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    // CHECK-MESSAGES: :[[@LINE-3]]:21: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    r=fun(z++-++y);  //违背7
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    // CHECK-MESSAGES: :[[@LINE-2]]:15: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    data1=++x+(x++)-(z--)-(--z)+x+y+z;  //违背8
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    // CHECK-MESSAGES: :[[@LINE-2]]:16: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    // CHECK-MESSAGES: :[[@LINE-3]]:22: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    // CHECK-MESSAGES: :[[@LINE-4]]:28: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    data1=x%y--;  //违背9
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    data1=x*++y;  //违背10
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    data1=x/z--;  //违背11
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 禁止在运算表达式中或函数调用参数中使用++或--操作符 [gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator]
    return (0);
}
int fun(int p)
{
    int ret;
    ret=2*p;
    return ret;
}

//遵循示例
int fun_1(int p);
int main_1(void)
{
    int x=1,y=2,z=3;
    int r;
    y=y+x;
    x++;  //遵循1
    y++;  //遵循2
    z=z+y;
    z--;  //遵循3
    z=fun_1(z);
    r=fun_1(z);
    z--;  //遵循4
    return (0);
}
int fun_1(int p)
{
    int ret;
    ret=2*p;
    return ret;
}
