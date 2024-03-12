// RUN: %check_clang_tidy %s gjb8114-code-before-include %t

//CSR-94	禁止在头文件前有可执行代码		4.2.1.5		R-1-2-3	gb8114-Banexecodebeforeinclude		
//CSR-94  GJB 8114-2013 R-1-2-3
//违背示例
//禁止在头文件前有可执行代码，顾名思义，那就是在最后出现的include前面不能有可执行代码，checker只会在最后一个include上面报错
int main_1(void)
{
    #include<b.h>
    #include "a.h"  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止在头文件前有可执行代码 [gjb8114-code-before-include]
    int a=1;
    int ix = 0;
    int iy = 1;
    int iz = 2;
    idata = ix + iy + iz;
    return (0);
}
int main_2(void)
{
    int ix = 0;
    int iy = 1;
    int iz = 2;
    return (0);
}