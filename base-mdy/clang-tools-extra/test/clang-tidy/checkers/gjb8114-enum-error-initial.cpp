// RUN: %check_clang_tidy %s gjb8114-enum-error-initial %t

//CSR-46	枚举元素定义中的初始化必须完整				R-1-11-4	gjb8114-enum-error-initial
//CSR-46  GJB 8114-2013 R-1-11-4
//违背和遵循示例
int main(void)
{
  enum Etype{  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 枚举元素定义中的初始化必须完整 [gjb8114-enum-error-initial]
    RED,
    WHITE=0,
    BLUE
  }edata;
  enum Etype2{  //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 枚举元素定义中的初始化必须完整 [gjb8114-enum-error-initial]
    YELLOW,
    PINK=2,
    GREEN=3
  }edata1;
  enum Etype3{  //遵循1(初始化全部元素)
    BLACK=3,
    WHITE1=6,
    WHITE2=9
  }edata2;
  enum Etype4{  //遵循2(初始化第一个元素)
    ORANGE0=1,
    ORANGE1,
    ORANGE2,
    ORANGE3,
    ORANGE4
  }edata3;
  enum Etype5{  //违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 枚举元素定义中的初始化必须完整 [gjb8114-enum-error-initial]
    BROWN=1,
    BROWN1=2,
    BROWN2=6,
    BROWN3
  }edata4;
  enum Etype6{  //违背4
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 枚举元素定义中的初始化必须完整 [gjb8114-enum-error-initial]
    PEWTER=3,
    PEWTER1=4,
    PEWTER2,
    PEWTER3=3
  }edata5;
  edata1=GREEN;
  edata=BLUE;
  return (0);
}