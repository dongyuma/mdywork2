// RUN: %check_clang_tidy %s gjb8114-macro-related %t

//CSR-14	禁止将其他标识宏定义为关键字和基本类型		4.1.1.14	gb5369-MacroRelated	R-1-1-2	gjb8114-macro-related		
//CSR-14 GJB 8114-2013 R-1-1-2
//违背示例
#define JUDGE if //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 禁止将其他标识宏定义为关键字和基本类型 [gjb8114-macro-related]
#define int64 long //违背2
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 禁止将其他标识宏定义为关键字和基本类型 [gjb8114-macro-related]
int main_1(void)
{
  int64 i=0;
  JUDGE(0==i)
  {
    i=1;
  }
  return (0);
}
//遵循示例
#define JUDGE_ZERO(i) if(0==(i))  //遵循1
typedef long int64; //遵循2
int main_2(void)
{
  int64 i=0;
  JUDGE_ZERO(i)
  {
    i=1;
  }
  return (0);
}

//CSR-23	禁止通过宏定义改变关键字和基本类型含义				R-1-1-1	gjb8114-macro-related		
//CSR-23 GJB 8114-2013 R-1-1-1
//违背示例
#define long 100  //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止通过宏定义改变关键字和基本类型含义 [gjb8114-macro-related]
int main_3(void)
{
  int i;
  i=long;
  return (0);
}
//遵循示例
#define LONG_NUM 100  //遵循1
int main_4(void)
{
  int i;
  i=LONG_NUM;
  return (0);
}

//CSR-95	以函数形式定义的宏，参数和结果必须用括号括起来		4.2.1.6	gb5369-MacroRelated	R-1-1-7	gjb8114-macro-related	
//CSR-95  GJB 8114-2013 R-1-1-7
//违背示例
#define pabs(x) x>=0?x:-x  //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:22: warning: 以函数形式定义的宏，参数和结果必须用括号括起来 [gjb8114-macro-related]
// CHECK-MESSAGES: :[[@LINE-2]]:24: warning: 以函数形式定义的宏，参数和结果必须用括号括起来 [gjb8114-macro-related]
int main_5(void)
{
  unsigned int result;
  int a=6;
  /*...*/
  result=pabs(a)+1;
  return (0);
}
//遵循示例
#define pabs(x) ((x)>=0?(x):-(x))  //遵循1
int main_6(void)
{
  unsigned int result;
  int a=6;
  /*...*/
  result=pabs(a)+1;
  return (0);
}
