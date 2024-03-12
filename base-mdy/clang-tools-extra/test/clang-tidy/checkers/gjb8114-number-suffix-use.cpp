// RUN: %check_clang_tidy %s gjb8114-number-suffix-use %t

//CSR-108	数值类型后缀必须使用大写字母				R-1-8-5	gjb8114-number-suffix-use
//CSR-108 GJB 8114-2013 R-1-8-5
//违背与遵循示例
unsigned int ucV1=0U;  //遵循1
unsigned int data1=0LU;  //遵循2
unsigned int ucV2=0u;  //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:19: warning: 数字类型后缀必须使用大写字母 [gjb8114-number-suffix-use]
long lV1=0L;  //遵循3
long lV2=0l;  //违背2
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 数字类型后缀必须使用大写字母 [gjb8114-number-suffix-use]
long lV3=0lU;  //违背3
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 数字类型后缀必须使用大写字母 [gjb8114-number-suffix-use]
long lV4=0xabcabL;  //遵循4
unsigned short lV5=0xabfefeu;  //违背4
// CHECK-MESSAGES: :[[@LINE-1]]:20: warning: 数字类型后缀必须使用大写字母 [gjb8114-number-suffix-use]
unsigned short lV6=1231Lu;  //违背5
// CHECK-MESSAGES: :[[@LINE-1]]:20: warning: 数字类型后缀必须使用大写字母 [gjb8114-number-suffix-use]
unsigned int lV7=213L;  //遵循5
float fV1=0.0F;  //遵循6
float fV2=0.0f;  //违背6
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 数字类型后缀必须使用大写字母 [gjb8114-number-suffix-use]
double ldV1=0.0L;   //遵循7
double ldV2=1.23231l;      //违背7
// CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 数字类型后缀必须使用大写字母 [gjb8114-number-suffix-use]
double ldV3=1.213132e10L;  //遵循8
double ldV4=3.14e9f;  //违背8
// CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 数字类型后缀必须使用大写字母 [gjb8114-number-suffix-use]
int ldV5=2332UL;  //遵循9
