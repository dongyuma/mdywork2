// RUN: %check_clang_tidy %s gjb8114-standard-library-function-rename-prohibit %t

//CSR-202	禁止覆盖标准函数库的函数				R-1-7-1	gjb8114-standard-library-function-rename-prohibit		
//CSR-202  GJB 8114-2013 R-1-7-1
//违背示例:
int printf(int a, int b) //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止覆盖标准函数库的函数 [gjb8114-standard-library-function-rename-prohibit]
{
  return((a>b)?a:b);
}
void write(int a, int b) //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: 禁止覆盖标准函数库的函数 [gjb8114-standard-library-function-rename-prohibit]
{
  //..
}
int main(void)
{
  int ret;
  ret =printf(2,3); 
  return(0);
}



