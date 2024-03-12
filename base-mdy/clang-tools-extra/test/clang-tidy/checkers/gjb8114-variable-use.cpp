// RUN: %check_clang_tidy %s gjb8114-variable-use %t

// CSR-253	禁止在内部块中重定义已有的变量名				R-1-13-7	gjb8114-variable-use	Rule 5.3	misrac2012-InterNotOverrideExter
// CSR-253 GJB 8114-2013 R-1-13-7
// 违背示例
int main_i16(void)
{
  int i, ix,iy,ip;
  unsigned int test=-2;
  ix = 1; 
  ip = 1;
  if(1 == ix)
  {
    int ip = 0; //违背2 
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止在内部块中重定义已有的变量名 [gjb8114-variable-use]
    for(i=0;i<10;i++)
    {
      ip = ip + ix;
    }
  }
  int test0;
  if(0==i)
  {
    int test1;
    int test0;  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止在内部块中重定义已有的变量名 [gjb8114-variable-use]
    if(i==0)
    {
      int iw,ix;  //违背4
      // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 禁止在内部块中重定义已有的变量名 [gjb8114-variable-use]
    }
    int test2;  //遵循2(外部的test2是在下面定义的，在此处定义test2不能算作重定义已有变量名)
  }
  iy = ip;
  int test2;
  return(0);
}
