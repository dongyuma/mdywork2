// RUN: %check_clang_tidy %s gjb8114-volatile-variable-repeat-in-binaryoperator %t

//CSR-275	禁止在表达式中出现多个同一volatile类型变量的运算     R-1-13-13
//CSR-275 GJB 8114-2013 R-1-13-13
//违背示例
int main(void)
{
  unsigned int i,z[100];
  volatile unsigned int v=1;
  int k=1;
  for(i=0;i<100;i++)
  {
    z[i]=3*v*v+2*v+i;  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止在表达式中出现多个同一volatile类型变量的运算 [gjb8114-volatile-variable-repeat-in-binaryoperator]
    // CHECK-MESSAGES: :[[@LINE-2]]:14: warning: 禁止在表达式中出现多个同一volatile类型变量的运算 [gjb8114-volatile-variable-repeat-in-binaryoperator]
    // CHECK-MESSAGES: :[[@LINE-3]]:18: warning: 禁止在表达式中出现多个同一volatile类型变量的运算 [gjb8114-volatile-variable-repeat-in-binaryoperator]
    z[i]=k+2*2*k+k*k;  //遵循1
    z[i]=v;  //遵循2
    z[i]=v+v+k+2*k;  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 禁止在表达式中出现多个同一volatile类型变量的运算 [gjb8114-volatile-variable-repeat-in-binaryoperator]
    // CHECK-MESSAGES: :[[@LINE-2]]:12: warning: 禁止在表达式中出现多个同一volatile类型变量的运算 [gjb8114-volatile-variable-repeat-in-binaryoperator]
    k=v;  //遵循3
    z[i]=k+2*2*k+k*k;  //遵循4
  }
  volatile short data1;
  data1=data1*2+v;  //遵循5
  data1=data1*2+data1;  //违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在表达式中出现多个同一volatile类型变量的运算 [gjb8114-volatile-variable-repeat-in-binaryoperator]
  // CHECK-MESSAGES: :[[@LINE-2]]:17: warning: 禁止在表达式中出现多个同一volatile类型变量的运算 [gjb8114-volatile-variable-repeat-in-binaryoperator]
  return (0);
}