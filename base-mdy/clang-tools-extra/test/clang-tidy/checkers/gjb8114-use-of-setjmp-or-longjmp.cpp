// RUN: %check_clang_tidy %s gjb8114-use-of-setjmp-or-longjmp %t

//官方给出的用例除了预期的报错，还会额外报一些错误
//CSR-177	禁止使用setjmp/longjmp				R-1-5-2	gjb8114-use-of-setjmp-or-longjmp		
//CSR-177  GJB 8114-2013 R-1-5-2
//官方违背示例(编译器报错)
/*
#include <math.h>
#include <setjmp.h>
jmp_buf mark;
double fdiv(double a, double b);
int main(void)
{
    int jmpret;
    double r, n1 = 1.0, n2 = 0.0;
    jmpret = setjmp(mark);  //违背1
    if(0 == jmpret)
    {
        r = fdiv(n1, n2);
    }
    else
    {
        return (-1);
    }
    return (0); 
}

double fdiv(double a, double b)
{
    double div = a / b;
    if(fabs(b) < 1e-10)
    {
        longjmp(mark, -1);  //违背2
    }
    return div;
}
*/
//测试用例
#include <stdio.h>
#include <setjmp.h>
 
//记录恢复的位置变量
jmp_buf buf;
 
void test() {
    
    printf("进入test()函数 \n");
    
    //跳转到buf位置变量记录的位置
    longjmp(buf, 1);  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止使用setjmp/longjmp [gjb8114-use-of-setjmp-or-longjmp]
    printf("返回test()函数 \n");
}
 
int main(int argc, const char * argv[]) {
 
    //setjmp(buf)如果在setjmp本函数体内返回，则返回0
    //如果通过longjmp函数返回，则返回非0值
    
    if (setjmp(buf)) {  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止使用setjmp/longjmp [gjb8114-use-of-setjmp-or-longjmp]
        //返回了非0值，通过longjmp函数跳转到setjump函数返回
        printf("main()... setjump()函数通过longjmp返回 \n");
    } else {
        //第一次调用setjmp时，必定正常执行完毕返回，即返回0值
        printf("main()... setjump()函数正常执行完毕返回 \n");
        test();
    }
    
    return 0;
}