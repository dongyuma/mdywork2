// RUN: %check_clang_tidy %s gjb8114-char-symbol %t

//CSR-15	用于数值计算的字符型变量必须明确定义是有符号还是无符号		4.1.1.15	gb5369-NameForbiddenReuse	R-1-1-20	gjb8114-char-symbol
//CSR-15  GJB 8114-2013 R-1-1-20
//违背示例
int main_1(void)
{
  char i;  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 用于数值计算的字符型变量必须明确定义是有符号还是无符号 [gjb8114-char-symbol]
  char i2;  //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 用于数值计算的字符型变量必须明确定义是有符号还是无符号 [gjb8114-char-symbol]
  char i3;  //违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 用于数值计算的字符型变量必须明确定义是有符号还是无符号 [gjb8114-char-symbol]
  char i4;  //违背4
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 用于数值计算的字符型变量必须明确定义是有符号还是无符号 [gjb8114-char-symbol]
  char i5;  //违背5
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 用于数值计算的字符型变量必须明确定义是有符号还是无符号 [gjb8114-char-symbol]
  char i6;  //违背6
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 用于数值计算的字符型变量必须明确定义是有符号还是无符号 [gjb8114-char-symbol]
  char i7;  //违背7
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 用于数值计算的字符型变量必须明确定义是有符号还是无符号 [gjb8114-char-symbol]
  char i8;  //违背8
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 用于数值计算的字符型变量必须明确定义是有符号还是无符号 [gjb8114-char-symbol]
  char i9;  //违背9
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 用于数值计算的字符型变量必须明确定义是有符号还是无符号 [gjb8114-char-symbol]
  int j;
  i3 = (char)0xFF;  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 用于数值计算的字符型变量必须明确定义是有符号还是无符号 [gjb8114-char-symbol]
  unsigned char m = i;
  j = i2 + 1;//第一种情况 
  //j = 1 + i;//第二种情况
  // j=!i;//第三种情况
  i = i<<2;
  i4++;
  ++i5;
  i6--;
  --i7;
  int y = ~i8;
  int z = m + i9;
  if (j ++)
  {
    return (1);
  }
  
  switch(i)
  case 1 :
    break;
  return (0);
}

//遵循示例
int main_2(void)
{
  unsigned char i;  //遵循1
  int j;
  i = (unsigned char)0xFF;  //遵循2
  j = i + 1;
  return (0);
}
