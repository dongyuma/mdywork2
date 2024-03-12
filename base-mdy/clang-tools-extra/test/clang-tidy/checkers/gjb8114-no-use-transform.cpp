// RUN: %check_clang_tidy %s gjb8114-no-use-transform %t

//目前仅支持规则中违背1的情况，即支持检测原类型和转换类型相同的情况，暂不支持违背2的情况，即不支持原类型和转换类型不相同，但不写转换符的情况下在后续表达式中也会自动转换的情况
//参考checker:AvoidCStyleCastsCheck
//CSR-223	禁止无实质作用的类型转换	 R-1-10-6	gjb8114-no-use-transform
//CSR-223 GJB 8114-2013 R-1-10-6
//违背示例
int main(void)
{
  unsigned int sx,sy=10,sz;
  sx=(unsigned int) sy +2;  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: 禁止无实质作用的类型转换 [gjb8114-no-use-transform]
  sz=(float)((sx+sy)*2/3);  //遵循1
  int a;
  double b=(double)a;  //遵循2
  double c=1.2;
  double d=(double)c;  //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止无实质作用的类型转换 [gjb8114-no-use-transform]
  long e=5;
  long f;
  f=(long)e;  //违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止无实质作用的类型转换 [gjb8114-no-use-transform]
  double g=(double)(1+2+3);  //遵循3
  g=(double)(1+2+c);  //违背4
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止无实质作用的类型转换 [gjb8114-no-use-transform]
  b=a;
  return (0);
}