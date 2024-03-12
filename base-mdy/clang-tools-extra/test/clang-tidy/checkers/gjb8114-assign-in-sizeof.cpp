// RUN: %check_clang_tidy %s gjb8114-assign-in-sizeof %t

//CSR-263	禁止在sizeof中使用赋值				R-1-6-13	gjb8114-assign-in-sizeof
//CSR-263  GJB 8114-2013 R-1-6-13
//违背示例
int main(void)
{
  int x=1,y=3,z=7;
  int ilen;
  ilen=sizeof(x=y);  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 禁止在sizeof中使用赋值 [gjb8114-assign-in-sizeof]
  ilen=sizeof(x=y=z);  //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 禁止在sizeof中使用赋值 [gjb8114-assign-in-sizeof]
  ilen=sizeof(y=5);  //违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 禁止在sizeof中使用赋值 [gjb8114-assign-in-sizeof]
  ilen=sizeof(((y=5)));  //违背4
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 禁止在sizeof中使用赋值 [gjb8114-assign-in-sizeof]
  int data = sizeof(x) + alignof(x);  //遵循1
  ilen=sizeof(int);  //遵循2
  ilen=sizeof(2+3.14);  //遵循3
  return (0);
}