// RUN: %check_clang_tidy %s gjb8114-pointer-assign-null-after-free %t

// CSR-142 指针变量被释放后必须置为NULL GJB8114	R-1-3-6
#include <stdlib.h>
#include <malloc.h>
int main(void)
{
    int *x= (int *)malloc(sizeof(int));
    if(NULL != x)
    {
        *x = 1;
        //...
        free(x); //违背1
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 指针变量被释放后必须置为NULL [gjb8114-pointer-assign-null-after-free]
    }
    else
    {
        return (-1);
    }
    return (0);
}

// 遵循示例
int main2(void)
{
    int *x= (int *)malloc(sizeof(int));
    if(NULL != x)
    {
        *x = 1;
        //...
        free(x); 
        x = NULL; //遵循1
    }
    else
    {
        return (-1);
    }
    return (0);
}
