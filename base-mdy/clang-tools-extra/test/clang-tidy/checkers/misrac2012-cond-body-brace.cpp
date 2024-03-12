// RUN: %check_clang_tidy %s misrac2012-cond-body-brace %t

void fun()
{
    int number1=3;
    int number2=4;
    while(number1)
      number1--;  //违背1
      // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 迭代状态和选择状态的内容应该是复合状态 [misrac2012-cond-body-brace]
      number1--;
    while(number1)
    {  //遵循1
      number1--;
      number1--;
    }
    if(number1)
      if(number2)  //违背2
      // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 迭代状态和选择状态的内容应该是复合状态 [misrac2012-cond-body-brace]
        number1--;  //违背3
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 迭代状态和选择状态的内容应该是复合状态 [misrac2012-cond-body-brace]
      else
        number2--;  //违背4
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 迭代状态和选择状态的内容应该是复合状态 [misrac2012-cond-body-brace]
    if(number1)
    {  //遵循2
      if(number2)
      {  //遵循3
         number1--;
      }
      else
      {  //遵循4
         number2--;
      }
    }
    for(int i=0;i<5;i++)
      number1--;  //违背5
      // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 迭代状态和选择状态的内容应该是复合状态 [misrac2012-cond-body-brace]
    for(int i=0;i<5;i++)
    {  //遵循5
        number1--;
    }
    while(number1);  //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:19: warning: 迭代状态和选择状态的内容应该是复合状态 [misrac2012-cond-body-brace]
    {
      number1--;
    }
    while(number1)
    {  //遵循6

    }
    do
    {  //遵循7
        number1--;
    } while (number1>0);
    do
        number1--;  //违背7
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 迭代状态和选择状态的内容应该是复合状态 [misrac2012-cond-body-brace]
    while (number1>0);
    if(number1)
    {
      number1--;
    }
    else if(number2)
    {  //遵循8
      number2--;
    }
    else
    {
      number1--;
      number2--;
    }

}