// RUN: %check_clang_tidy %s gjb8114-keyword-related %t

//CSR-9	禁止重新定义C或C++的关键字			R-1-1-4	gjb8114-keyword-related 
//CSR-9  GJB 8114-2013 R-1-1-4
//违背示例
int main(void)
{
  unsigned int FILE=0; //违背1 
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 禁止重新定义C或C++的关键字 [gjb8114-keyword-related]
  unsigned int cout =0; //违背2 
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 禁止重新定义C或C++的关键字 [gjb8114-keyword-related]
  unsigned int cin =0; //违背3 
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 禁止重新定义C或C++的关键字 [gjb8114-keyword-related]
  unsigned int endl=0; //违背4 
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 禁止重新定义C或C++的关键字 [gjb8114-keyword-related]
  unsigned int a=0; 
  unsigned int b=0; 
  //....
  return(0);
}
