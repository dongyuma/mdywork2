// RUN: %check_clang_tidy %s gjb8114-global-or-static-variable-depend-on-system %t

//CSR-50	变量初始化禁止隐含依赖于系统的缺省值		4.1.1.14  R-1-11-2
//CSR-50 GJB 8114-2013 R-1-11-2
//仅检测全局变量和静态变量在定义时未赋值的情况，即将未赋值的全局变量和静态变量视为隐含依赖于系统的缺省值
//违背示例
int Gstate;  //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 变量初始化禁止隐含依赖于系统的缺省值 [gjb8114-global-or-static-variable-depend-on-system]
int fun()
{
  return 1;
}
int data1;  //违背2
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 变量初始化禁止隐含依赖于系统的缺省值 [gjb8114-global-or-static-variable-depend-on-system]
double data2;  //违背3
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 变量初始化禁止隐含依赖于系统的缺省值 [gjb8114-global-or-static-variable-depend-on-system]
short data3;  //违背4
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 变量初始化禁止隐含依赖于系统的缺省值 [gjb8114-global-or-static-variable-depend-on-system]
int data4=2;  //遵循1
double data5=1.3;  //遵循2
int main(void)
{
  static int StateN=fun();  //遵循3
  int data6=3;  //遵循4
  int data7;  //遵循5
  static short data8;  //违背5
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 变量初始化禁止隐含依赖于系统的缺省值 [gjb8114-global-or-static-variable-depend-on-system]
  static int data9;  //违背6
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 变量初始化禁止隐含依赖于系统的缺省值 [gjb8114-global-or-static-variable-depend-on-system]
  static int data10=5;  //遵循6
  if(1==Gstate)
  {
    StateN=StateN+1;
  }
  return (0);
}
static int data11;  //违背7
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 变量初始化禁止隐含依赖于系统的缺省值 [gjb8114-global-or-static-variable-depend-on-system]
static double data12=1.2;  //遵循7

int a1[10];
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 变量初始化禁止隐含依赖于系统的缺省值 [gjb8114-global-or-static-variable-depend-on-system]
int a2[10] = {0};