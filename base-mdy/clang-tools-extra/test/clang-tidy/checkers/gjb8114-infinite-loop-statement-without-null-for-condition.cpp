// RUN: %check_clang_tidy %s gjb8114-infinite-loop-statement-without-null-for-condition %t

#include<stdio.h>

//CSR-181	无限循环必须使用while(1)语句,禁止使用for(;;)等其它形式的语句				R-1-9-4	gjb8114-infinite-loop-statement-without-null-for-condition	
//CSR-181 GJB 8114-2013 R-1-9-4
//违背示例
int Gstate=0;
int main_4(void)
{
  for(;;)  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 无限循环必须使用while(1)语句,禁止使用for(;;)等其它形式的语句 [gjb8114-infinite-loop-statement-without-null-for-condition]
  {
    //...
    if(1==Gstate)
    {
      break;
    }
  }
  return (0);
}

int main_5(void)
{
  int x;
  for(;;)  //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 无限循环必须使用while(1)语句,禁止使用for(;;)等其它形式的语句 [gjb8114-infinite-loop-statement-without-null-for-condition]
  {
    switch(x)
    {
      case 1:
      break;
      case 2:
      break;
    }
  }
}
int main_6(void)
{
  for(;;)  //违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 无限循环必须使用while(1)语句,禁止使用for(;;)等其它形式的语句 [gjb8114-infinite-loop-statement-without-null-for-condition]
  {
    printf("aaa");
  }
  return (0);
}

//遵循示例
int Gstate1=0;
int main_7(void)
{
  while(1)  //遵循1
  {
    //...
    if(1==Gstate1)
    {
        break;
    }
  }
  return (0);
}

