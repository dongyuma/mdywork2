// RUN: %check_clang_tidy %s gjb8114-nested-annotation %t

//CSR-248	禁止嵌套注释		4.10.1.1	gb5369-Nested-Comment	R-1-2-6	gjb8114-nested-annotation		
//CSR-248 GJB 8114-2013 R-1-2-6
//违背示例
int main_1(void)
{
  int local=0;
  int sign=0;
  /*                                               //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止嵌套注释 [gjb8114-nested-annotation]
    note for code1
  /*                
    note for code2
  */
  return (0);
}

int main_2(void)
{
  int local=0;
  int sign=0;
  /*                                               //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止嵌套注释 [gjb8114-nested-annotation]
    note for code1
  /*                
    note for code2
  */
  // */
  return (0);
}

//此种情况不在检测范围之内
// int main_2(void)
// {
//   int local=0;
//   int sign=0;
//   /*                                               
//   */                
//     note for code2
//   */
//   return (0);
// }


//遵循示例
int main_3(void)
{
  int local=0;
  int sign=0;
  /*                                               //遵循1
    note for code1
  */
  return (0);
}


