// RUN: %check_clang_tidy %s gjb8114-variable-declare-same-name-with-function %t

//CSR-1	禁止变量名与函数名同名	4.1.1.1	gb5369-NameForbiddenReuse	R-1-13-3	gjb8114-variable-declare-same-name-with-function		
//CSR-1 GJB 8114-2013 R-1-13-3

//违背示例1
void misdis(void)
{
    //...
}
int main(void)
{
    int misdis;  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 函数名'misdis'不能被重新用作变量名。 [gjb8114-variable-declare-same-name-with-function]
    misdis=0;
    return (0);
}

int fun(void)
{
    int test = 1;
    int misdis;  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 函数名'misdis'不能被重新用作变量名。 [gjb8114-variable-declare-same-name-with-function]
    misdis=0;
    return (0);
}

//违背示例2
int main_v2(void)
{
    int misdis_v2;  //遵循1
    misdis_v2=0;
    return (0);
}

void misdis_v2(void)
{
    //...
}


int fun_v2(void)
{
    int test = 1;
    int misdis_v2;  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 函数名'misdis_v2'不能被重新用作变量名。 [gjb8114-variable-declare-same-name-with-function]
    int misdis;  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 函数名'misdis'不能被重新用作变量名。 [gjb8114-variable-declare-same-name-with-function]
    misdis_v2=0;
    return (0);
}