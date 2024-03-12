// RUN: %check_clang_tidy %s gjb8114-use-of-gets %t

//CSR-268	禁止使用gets函数,应使用fgets函数替代		R-1-6-18	gjb8114-use-of-gets
//CSR-268  GJB 8114-2013 R-1-6-18
//违背示例
#include<stdio.h>
#include<string.h>
int main(void)
{
  char line[5]={0};
  char line1[10];
  printf("Input a string:");
  if(NULL == gets(line))  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: 禁止使用gets函数,应使用fgets函数替代 [gjb8114-use-of-gets]
  {
    return (-1);
  }
  gets(line1);  //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止使用gets函数,应使用fgets函数替代 [gjb8114-use-of-gets]
  char buffer[256];
  while(gets(buffer)!=NULL)  //违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止使用gets函数,应使用fgets函数替代 [gjb8114-use-of-gets]
  {
    puts(buffer);
  }
  char buffer1[256];
  while(strcmp(gets(buffer1),"")!=0)  //违背4
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 禁止使用gets函数,使用fgets函数替代 [gjb8114-use-of-gets]
  {
    puts(buffer1);
  }
  char str[20];
  printf("请输入一个字符串:");
  fgets(str, 7, stdin);  //遵循1
  return (0);
}