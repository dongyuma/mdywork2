// RUN: %check_clang_tidy %s gjb8114-float-equal %t

//CSR-226	禁止对浮点数进行是否相等的比较				R-1-12-3	gjb8114-float-equal
//CSR-226 GJB 8114-2013 R-1-12-3
//违背示例
int main_1(void)
{
  int i,j;
  int P=1000;
  float d=0.435;
  if(435==(P*d))  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止对浮点数进行是否相等的比较 [gjb8114-float-equal]
  {
    i=1;
  } 
  else
  {
    i=2;
  }
  if(435!=(P*d))  //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止对浮点数进行是否相等的比较 [gjb8114-float-equal]
  {
    j=1;
  } 
  else
  {
    j=2;
  }
  if (i != j)
  {
    i=3;
  }
  
  return (0);
}

//遵循示例
#include<math.h>
int main_2(void)
{
  int i,j;
  int P=1000;
  float d=0.435;
  if(fabs(435-(P*d))<1e-4)  //遵循1
  {
    i=1;
  } 
  else
  {
    i=2;
  }
  if((435>=(P*d)+ 1e-4) || (435<=(P*d)- 1e-4))  //遵循2
  {
    j=1;
  } 
  else
  {
    j=2;
  }
  return (0);
}
