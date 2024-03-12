// RUN: %check_clang_tidy %s gjb8114-logical-expr-operation-parenthese %t

// GJB 8114 R-1-2-5
// 逻辑判别表达式中的运算项必须要使用括号

int main(void)
{
    unsigned int i, tbc;
    tbc = 0x80;
    if(tbc&0x80 == 0x80)
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 逻辑判别表达式中的运算必须使用括号 [gjb8114-logical-expr-operation-parenthese]
    {
        i = 1;
    }
    else
    {
        i = 2;
    }
    if((tbc&0x80) == 0x80)
    {
        i = 1;
    }
    else
    {
        i = 2;
    }
    while (tbc&0x80 != 0x80)
    // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 逻辑判别表达式中的运算必须使用括号 [gjb8114-logical-expr-operation-parenthese]
    {
        break;
    }
    while (tbc&(0x80 != 0x80))
    {
        break;
    }
    for (unsigned i = 0; i < tbc + 1; i++)
    // CHECK-MESSAGES: :[[@LINE-1]]:30: warning: 逻辑判别表达式中的运算必须使用括号 [gjb8114-logical-expr-operation-parenthese]
    {
        break;
    }
    for (unsigned i = 0; i < (tbc + 1); i++)
    {
        break;
    }
    do
    {
        break;
    } while (tbc * i > 100);
    // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: 逻辑判别表达式中的运算必须使用括号 [gjb8114-logical-expr-operation-parenthese]
    do
    {
        break;
    } while ((tbc * i) > 100);

    return 0;
}
