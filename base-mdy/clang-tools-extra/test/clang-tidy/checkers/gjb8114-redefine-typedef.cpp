// RUN: %check_clang_tidy %s gjb8114-redefine-typedef %t

//CSR-17	用typedef自定义的类型禁止被重新定义		4.1.1.17	gb5369-TypedefRedefine	R-1-1-3	gjb8114-redefine-typedef
//CSR-17 GJB 8114-2013 R-1-1-3
//违背示例
typedef int mytype;
typedef short k;
int main(void)
{
    typedef float mytype;  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 用typedef自定义的类型禁止被重新定义 [gjb8114-redefine-typedef]
    mytype x=1.0;
    typedef double k;  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 用typedef自定义的类型禁止被重新定义 [gjb8114-redefine-typedef]
    typedef unsigned int w;
    return (0);
}
typedef double w;  //违背3
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 用typedef自定义的类型禁止被重新定义 [gjb8114-redefine-typedef]