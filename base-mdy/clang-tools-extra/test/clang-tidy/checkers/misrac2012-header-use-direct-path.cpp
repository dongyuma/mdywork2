// RUN: %check_clang_tidy %s misrac2012-header-use-direct-path %t

//CSR-97	头文件名禁止使用 "'"、 "\" 和 "/*"字符。4.2.1.8 GJB5359 

//违背示例:
#include"C:\Users\Administrator.DESKTOP-AUDUQRF\Desktop\a.h" 
#include"D:'a.h'"
#include"E:/*"

int main(void)
{
  int data = 0;
  return(0);
}

