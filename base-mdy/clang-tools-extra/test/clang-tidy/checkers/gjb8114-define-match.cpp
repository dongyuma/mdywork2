// // RUN: %check_clang_tidy %s gjb8114-define-match %t

// // CSR-25	函数中的#define和#undef必须配对使用	 R-1-1-6	gjb8114-define-match
// // CSR-25  GJB 8114-2013 R-1-1-6 	
// 违背示例
unsigned int fun1(void);
unsigned int fun2(void); 
int main(void)
{
  unsigned int a1=0U; 
  unsigned int a2= 0U; 
  unsigned int a3=0U; 
  a1=fun1(); 
  a2=fun2();  
  a3=a1+a2; 
  return(0);
}
unsigned int fun1(void)
{
  unsigned int x=0U;
  #define BLOCKDEF 2U //违背1 
  // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 函数中的#define和#undef必须配对使用 [gjb8114-define-match]
  x=x+BLOCKDEF; 
  return x;
}
unsigned int fun2(void)
{
  unsigned int x=0U;
  x=x+BLOCKDEF;
  #undef BLOCKDEF //违背2 
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 函数中的#define和#undef必须配对使用 [gjb8114-define-match]
  return x;
}

unsigned int fun3(void)
{
  unsigned int x=0U;
  #define BLOCKDEF3 3U 
  x=x+BLOCKDEF3;
  #undef BLOCKDEF3 
  #define BLOCKDEF3 3U //违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 函数中的#define和#undef必须配对使用 [gjb8114-define-match]
  return x;
}

int fun4(void)
{
  #undef BLOCKDEF3 //违背4
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 函数中的#define和#undef必须配对使用 [gjb8114-define-match]
  unsigned int a1=0U; 
  unsigned int a2= 0U; 
  unsigned int a3=0U;
  a1=fun1(); 
  a2=fun2(); 
  a3=a1+a2; 
  return(0);
}


// //遵循示例
// unsigned int fun1(void);
// unsigned int fun2(void); 
// int main(void)
// {
//   unsigned int a1=0U; 
//   unsigned int a2= 0U; 
//   unsigned int a3=0U; 
//   a1=fun1(); 
//   a2=fun2(); 
//   a3=a1+a2; 
//   return(0);
// }
// unsigned int fun1(void)
// {
//   unsigned int x=0U;
//   #define BLOCKDEF1 2U   //遵循1 
//   x=x+BLOCKDEF1;
//   #undef BLOCKDEF1       //遵循2
//   return x;
// }
// unsigned int fun2(void)
// {
//   unsigned int x=0U;
//   #define BLOCKDEF2 2U   //遵循3
//   x=x+BLOCKDEF2;
//   #undef BLOCKDEF2       //遵循4
//   return x;
// }
// unsigned int fun3(void)
// {
//   unsigned int x=0U;
//    #define BLOCKDEF3 3U //遵循5
//   x=x+BLOCKDEF3;
//   #undef BLOCKDEF3 //遵循6
//   #define BLOCKDEF3 3U //遵循7
//   return x;
// }
