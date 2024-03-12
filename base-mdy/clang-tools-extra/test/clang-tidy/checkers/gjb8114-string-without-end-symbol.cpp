// RUN: %check_clang_tidy %s gjb8114-string-without-end-symbol %t

//CSR-98     用于表示字符串的数组必须以'\0'结束    R-1-13-16
//CSR-98  GJB 8114-2013 R-1-13-16
//违背示例
#include<stdio.h>
int main(void)
{
  char buf[8];
  buf[0]='y';
  buf[1]='y';
  buf[2]='y';
  buf[3]='\0';  //遵循1
  char buf1[8];
  buf1[0]='y';
  buf1[1]='y';
  buf1[2]='y';  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 用于表示字符串的数组必须以'\0'结束 [gjb8114-string-without-end-symbol]
  char buf2[8];
  buf2[5]='\0';  //遵循2
  buf2[4]='y';
  buf2[3]='y';
  buf2[2]='y';
  buf2[1]='y';
  buf2[0]='y';
  char buf3[8];
  buf3[2]='y';  //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 用于表示字符串的数组必须以'\0'结束 [gjb8114-string-without-end-symbol]
  buf3[1]='y';
  buf3[0]='y';
  char buf4[100]={'a','b','c','d','e'};  //违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:35: warning: 用于表示字符串的数组必须以'\0'结束 [gjb8114-string-without-end-symbol]
  char buf5[4]={'a','b','c','d'};  //违背4
  // CHECK-MESSAGES: :[[@LINE-1]]:29: warning: 用于表示字符串的数组必须以'\0'结束 [gjb8114-string-without-end-symbol]
  char buf6[5]={'a','b','c','d','\0'};  //遵循3
  char buf7[120]={'a','\0','b','c','d'}; //违背5
  // CHECK-MESSAGES: :[[@LINE-1]]:36: warning: 用于表示字符串的数组必须以'\0'结束 [gjb8114-string-without-end-symbol]
  char buf8[20];
  buf8[0]='a';
  buf8[1]='\0';
  buf8[2]='b';  //违背6
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 用于表示字符串的数组必须以'\0'结束 [gjb8114-string-without-end-symbol]
  return (0);
}