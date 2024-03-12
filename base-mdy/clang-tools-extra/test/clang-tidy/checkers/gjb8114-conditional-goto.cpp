// RUN: %check_clang_tidy %s gjb8114-conditional-goto %t

//CSR-176	禁止从复合语句外goto到复合语句内,或由下向上goto				R-1-5-1	 gjb8114-conditional-goto
//CSR-176  GJB 8114-2013 R-1-5-1
//违背与遵循示例
int main_1(void)
{
    int i = -2,j = -2;
    int k;
    //...
L0:
    i = i + 1;
    if(i < 0)
    {
        //······
        goto L0;  //违背1
        // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: 禁止从下往上goto [gjb8114-conditional-goto]
    }
    if(j < 0)
    {
        k = -10;
        j = 0;
        goto L1;  //违背2
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止从复合语句外goto到复合语句内 [gjb8114-conditional-goto]
    }

    for(k=0;k<10;k++)
    {
L1:
        j = j + k;
    }
    
    for(k=0;k<10;k++)
    {
        goto L2;  //遵循1
        j=j-1;
L2:
        j = j + k;
    }
    goto L3;  //遵循2
L3:
    j=j+2;
    for(k=0;k<10;k++)
    {
        if(k==2)
        {
            goto L4;  //遵循3
        }
        j--;
L4:
        j++;
    }
    for(k=0;k<10;k++)
    {
L5:
        j--;
    }
    goto L5;  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止从复合语句外goto到复合语句内 [gjb8114-conditional-goto]
    // CHECK-MESSAGES: :[[@LINE-2]]:10: warning: 禁止从下往上goto [gjb8114-conditional-goto]
    return (0);
}

