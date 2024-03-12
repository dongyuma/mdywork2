// RUN: %check_clang_tidy %s gjb8114-end-with-break %t

//CSR-174	switch中的case和default必须以break或return终止,共用case必须加以明确注释				R-1-4-7	gjb8114-end-with-break		
//CSR-174  GJB 8114-2013 R-1-4-7
//违背和遵循示例
int main_1(void)
{
    int i = 0;
    int j = 0;
    int k = 0;
    //...
    switch (i)
    {
    case 1:  //遵循1
        j = 1;
        break;
    case 2:  //遵循2
        break;
    case 3:  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
        j = 3;
    case 4: //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
        j = 4;
    case 5:  /*shared*/  //遵循3
    case 6: //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
    case 7: //遵循4
        j = 7;
        break;
    case 8: //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
    case 9: //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
    case 10: //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
    case 11: /*shared*/ //遵循5
    case 12: //遵循6
        j = 9;
        break;
    default: //遵循7
    {
        j = 10;
        break;
    }
    }
    switch (i);
    switch (i)
        return (0);
}
int main_2(void)
{
    int i = 0;
    int j = 0;
    int k = 0;
    //...
    switch (i)
    {
    case 1:  //违背7
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
        if (j > 0)
        {
            k = 1;
            break;
        }
        k = 2;
    case 2:  //违背8
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
    case 3:  //违背9
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
        j = 3;
    case 4:  //违背10
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
        k = 4;
    case 5:
        j = 5;
        break;
    case 6:  //遵循8
    {
        j = 6;
        break;
    }
    default: //违背11
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
        j = -1;
    }
    switch (i)
    {
    case 5:  //遵循9
        j = 5;
        break;
    case 6:  //违背12
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
    {
        j = 6;
    }
    case 7:  //遵循10
    {
        j = 7;
        break;
    }
    default: //违背13
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
        j = -1;
    }
    switch (i)
    {
    case 1:  //遵循11
        j = 1;
        break;
    case 2:  //遵循12
    {
        j = 1;
        break;
    }
    case 3:  //违背14
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
        j = 3;
        j++;
    case 4:  //违背15
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
    {
        j = 3;
        j++;
    }
    default:  //遵循13
    {
        j = 1;
        break;
    }
    }
    return (0);
}
//遵循示例
int main_3(void)
{
    int i = 0;
    int j = 0;
    int k = 0;
    //...
    switch (i)
    {
    case 1:  //遵循1
        if (j > 0)
        {
            k = 1;
            break;
        }
        k = 2;
        break;
    case 2:  /*shared*/  //遵循2
    case 3:              //遵循3
    {
        j = 3;
    }
    break;
    case 4:  /*shared*/  //遵循4
        k = 4;
    case 5:
        j = 5;
        break;
    default:  //遵循5
        j = -1;
        break;
    }
    return (0);
}


int main_4(void)
{
    int i = 0;
    int j = 0;
    int k = 0;
    //...
    switch (i)
    {
    case 1:  //遵循1
        j = 1;
        return 0;
    case 2:  //遵循2
        return 0;
    case 3:  //违背16
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
        j = 3;
    case 4: //违背17
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
        j = 4;
    case 5:  /*shared*/  //遵循3
    case 6: //违背18
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
    case 7: //遵循4
        j = 7;
        return 0;
    case 8: //违背19
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
    case 9: //违背20
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
    case 10: //违背21
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
    case 11: /*shared*/ //遵循5
    case 12: //遵循6
        j = 9;
        return 0;
    default: //遵循7
    {
        j = 10;
        return 0;
    }
    }
    switch (i);
    switch (i)
        return (0);
}
int main_5(void)
{
    int i = 0;
    int j = 0;
    int k = 0;
    //...
    switch (i)
    {
    case 1:  //违背22
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
        if (j > 0)
        {
            k = 1;
            return 0;
        }
        k = 2;
    case 2:  //违背23
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
    case 3:  //违背24
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
        j = 3;
    case 4:  //违背25
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
        k = 4;
    case 5:
        j = 5;
        return 0;
    case 6:  //遵循8
    {
        j = 6;
        return 0;
    }
    default: //违背26
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
        j = -1;
    }
    switch (i)
    {
    case 5:  //遵循9
        j = 5;
        return 0;
    case 6:  //违背27
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
    {
        j = 6;
    }
    case 7:  //遵循10
    {
        j = 7;
        return 0;
    }
    default: //违背28
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
        j = -1;
    }
    switch (i)
    {
    case 1:  //遵循11
        j = 1;
        return 0;
    case 2:  //遵循12
    {
        j = 1;
        return 0;
    }
    case 3:  //违背29
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
        j = 3;
        j++;
    case 4:  //违背30
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 不以break或return结尾 [gjb8114-end-with-break]
    {
        j = 3;
        j++;
    }
    default:  //遵循13
    {
        j = 1;
        return 0;
    }
    }
    return (0);
}
//遵循示例
int main_6(void)
{
    int i = 0;
    int j = 0;
    int k = 0;
    //...
    switch (i)
    {
    case 1:  //遵循1
        if (j > 0)
        {
            k = 1;
            return 0;
        }
        k = 2;
        return 0;
    case 2:  /*shared*/  //遵循2
    case 3:              //遵循3
    {
        j = 3;
    }
    return 0;
    case 4:  /*shared*/  //遵循4
        k = 4;
    case 5:
        j = 5;
        return 0;
    default:  //遵循5
        j = -1;
        return 0;
    }
    return (0);
}