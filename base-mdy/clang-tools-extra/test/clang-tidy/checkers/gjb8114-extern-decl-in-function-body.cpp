// RUN: %check_clang_tidy %s gjb8114-extern-decl-in-function-body %t

// GJB 8114 R-1-1-17
// 禁止在函数内部使用外部声明

extern int zdata1;
extern int fadd1(int,int);
int main(void)
{
    extern int zdata;  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 禁止在函数内部使用外部声明变量 [gjb8114-extern-decl-in-function-body]
    extern int fadd(int,int);  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 禁止在函数内部使用外部声明函数 [gjb8114-extern-decl-in-function-body]
    int fadd3(int,int);
    int x,y;
    x=1;
    y=2;
    zdata=fadd(x,y);
    return (0);
}
int fadd3(int x,int y){
  return x+y;
}
