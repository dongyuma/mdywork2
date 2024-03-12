// RUN: %check_clang_tidy %s gjb8114-ban-bit-logical %t

//CSR-258	禁止对逻辑表达式进行位运算				R-1-6-4	gjb8114-ban-bit-logical
//CSR-258  GJB 8114-2013 R-1-6-4
//违背示例
int main_1(void) 
{
  int x = 0, y = 1,z = 2;
  if((x == 1) | (y == 2))  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 禁止对逻辑表达式进行位运算 [gjb8114-ban-bit-logical]
  {
    z = 3;
  }
  if((x == 3) & (y == 4))  //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 禁止对逻辑表达式进行位运算 [gjb8114-ban-bit-logical]
  {
    z = 5;
  }
  if(~(x==3))  //违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: 禁止对逻辑表达式进行位运算 [gjb8114-ban-bit-logical]
  {
    z = 5;
  }
  if((x==3)<<(y==4))  //违背4
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止对逻辑表达式进行位运算 [gjb8114-ban-bit-logical]
  {
    z = 5;
  }
  if((x == 3) & 1)  //违背5
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 禁止对逻辑表达式进行位运算 [gjb8114-ban-bit-logical]
  {
    z = 5;
  }
  if(0x2 & 0x80 == 0x80)//违背6(位运算符&右侧存在逻辑表达式)
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 禁止对逻辑表达式进行位运算 [gjb8114-ban-bit-logical]
  {
    z=5;
  }
  if(x==1 | 1)//违背7(位运算符&左侧存在逻辑表达式)
  // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止对逻辑表达式进行位运算 [gjb8114-ban-bit-logical]
  {
    z=5;
  }
  return (0);
}


//遵循示例
int main_2(void)
{
  int x = 0, y = 1, z = 2;
  if((x == 1) || (y == 2))  //遵循1
  {
    z = 3;
  }
  if((x == 3) && (y == 4))  //遵循2
  {
    z = 5;
  }
  if((0x2 & 0x80 & 0x60 ) == 0x80)//遵循3
  {
  }
  if(0x2 & 0x80 & 0x60 )//遵循4
  {
  }
  if((0x2) & (0x80) & (0x60) )//遵循5
  {
  }
  return (0);
}

