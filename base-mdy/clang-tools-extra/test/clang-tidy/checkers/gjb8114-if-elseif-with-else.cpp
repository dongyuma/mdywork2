// RUN: %check_clang_tidy %s gjb8114-if-elseif-with-else %t

//CSR-161	在if… else if语句中必须使用else分支		4.3.1.2	ConditionalBranchChecker	R-1-4-1	gjb8114-if-elseif-with-else	Rule 15.7	misrac2012-Ifelseif
//CSR-161 GJB 8114-2013 R-1-4-1
//违背示例
int main_GJB8114_2013_1_4_1(void)
{
    int i = 0, j = 0;
    double x = 0.0;
    //..
    if (0 == i) 
    {
        x = 1.0;
    }
    else if (1 == i)
    {
        x = 2.0;
    } //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 在if-else if语句中必须使用else分支 [gjb8114-if-elseif-with-else]

    if ( 0 == j)
    {
        x = x + 5.0;
    }
    return (0);
}

//遵循示例
int main_GJB8114_2013_1_4_1_f(void)
{
    int i = 0, j =0;
    double x = 0.0;
    //..
    if (0 == i)
    {
        x = 1.0;
    }
    else if (1 == i)
    {
        x = 2.0;
    }
    else //遵循1
    {
        x = 0.0;
    }

    if ( 0 == j)
    {
        x = x + 5.0;
    }
    return (0);
}


