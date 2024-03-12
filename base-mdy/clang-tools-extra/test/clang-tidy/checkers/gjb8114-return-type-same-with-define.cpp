// RUN: %check_clang_tidy %s gjb8114-return-type-same-with-define %t

// CSR-200 函数返回值的类型必须与定义一致 GJB8114 R-1-7-10
//违背示例
unsigned int fun(unsigned int par);
int main(void)
{
    unsigned int i=1;
    unsigned int j;
    j=fun(i);
    return (0);
}
unsigned int fun(unsigned int par)
{
    if(1==par)
    {
        return (-1);  //违背1
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 函数返回值的类型必须与定义一致 [gjb8114-return-type-same-with-define]
    }
    else if(2==par)
    {
        return (1.5);  //违背2
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 函数返回值的类型必须与定义一致 [gjb8114-return-type-same-with-define]
    }
    else
    {
        return (2*par);
    }
}