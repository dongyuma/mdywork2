// RUN: %check_clang_tidy %s gjb8114-unsigned-compare-with-zero %t

//CSR-227	禁止对无符号数进行大于等于零或小于零的比较				R-1-12-4	gjb8114-unsigned-compare-with-zero		
//CSR-227 GJB 8114-2013 R-1-12-4
//违背示例
int main(void)
{
    unsigned int x=1,y=2;
    int flag=0;
    if(x>=0)  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 禁止对无符号数进行大于等于零或小于零的比较。 [gjb8114-unsigned-compare-with-zero]
    {
        flag=flag+1;
    }
    if(y<=0)  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 禁止对无符号数进行大于等于零或小于零的比较。 [gjb8114-unsigned-compare-with-zero]
    {
        flag=flag+1;
    }
    if((0)<=(x))  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 禁止对无符号数进行大于等于零或小于零的比较。 [gjb8114-unsigned-compare-with-zero]
    {
        flag=flag+1;
    }
    if(y==0)  //遵循1
    {
        flag=flag+1;
    }
    return (0);
}

