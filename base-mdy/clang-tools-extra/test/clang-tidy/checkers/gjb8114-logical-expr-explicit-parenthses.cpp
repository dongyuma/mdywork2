// RUN: %check_clang_tidy %s gjb8114-logical-expr-explicit-parenthses %t

// CSR-93 GJB 8114 R-1-2-4
// 引起二义性理解的逻辑表达式，必须使用括号显式说明优先级顺序
 
int main(void)
{
    int i=0, j=1, k=2;
    int a=0, b=0;
    if((0==i) && (1==j) || (2==k))
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 引起二义性理解的逻辑表达式必须使用括号显式说明优先顺序 [gjb8114-logical-expr-explicit-parenthses]
    {
        a = 0;
    }
    else if((0==i) || (1==j) && (2==k))
    // CHECK-MESSAGES: :[[@LINE-1]]:23: warning: 引起二义性理解的逻辑表达式必须使用括号显式说明优先顺序 [gjb8114-logical-expr-explicit-parenthses]
    {
        a = 1;
    }
    else if(!a||b)
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 引起二义性理解的逻辑表达式必须使用括号显式说明优先顺序 [gjb8114-logical-expr-explicit-parenthses]
    {
        b = 0;
    }
    else if(!a&&b)
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 引起二义性理解的逻辑表达式必须使用括号显式说明优先顺序 [gjb8114-logical-expr-explicit-parenthses]
    {
        b = 1;
    }
    else if(!a || ((i==0) && (j==1)))
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 引起二义性理解的逻辑表达式必须使用括号显式说明优先顺序 [gjb8114-logical-expr-explicit-parenthses]
    {
        b = 0;
    }
    else if(!a && ((i==0) || (j==1)))
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 引起二义性理解的逻辑表达式必须使用括号显式说明优先顺序 [gjb8114-logical-expr-explicit-parenthses]
    {
        b = 1;
    }  
    else if((0==i) && ((1==j) || (2==k)))
    {
        a = 0;
    }
    else if(((0==i) || (1==j)) && (2==k))
    {
        a = 1;
    }
    else if((!a) || ((i==0) && (j==1)))
    {
        b = 0;
    }
    else if((!a) && ((i==0) || (j==1)))
    {
        b = 1;
    } 
    else
    { }

    return 0;
}
