// RUN: %check_clang_tidy %s gjb8114-constant-logical %t

//CSR-70	禁止对常数值做逻辑非的运算		4.6.1.11	BinaryOperatorChecker	R-1-6-10	gjb8114-constant-logical
//CSR-70  GJB 8114-2013 R-1-6-10
//违背示例
int main_1(void)
{
  int i=0,j=0;
  if(i==!1)  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止对常数值做逻辑非的运算 [gjb8114-constant-logical]
  {
      j=1;
  }
  return (0);
}
 
//遵循示例
int main_2(void)
{
  int i=0,j=0;
  if(i!=1)  //遵循1
  {
      j=1;
  }
  return (0);
}
int main_3(void)
{
  int i=0,j=0,x;
  if(i==!x)  //遵循2
  {
      j=1;
  }
  return (0);
}
