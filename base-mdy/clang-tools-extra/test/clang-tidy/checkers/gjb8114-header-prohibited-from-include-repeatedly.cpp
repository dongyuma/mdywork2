// RUN: %check_clang_tidy %s gjb8114-header-prohibited-from-include-repeatedly %t

//违背示例
#include <stdio.h>
#include "file2.h"
#include "file1.h" //违背1  
//#include "file3.h" 
int main(void)
{ 
  a=3; 
  return (0);
}

// 遵循示例1
// #ifndef FILE1_H         
// #define FILE1_H
// #include "file1.h" 
// #endif
// int main(void)
// {
//   a=3; 
//   return (0);
// }

// 遵循示例2
// #pragma once
// #include "file1.h" 

// int main(void)
// {
//   a=3; 
//   return (0);
// }
