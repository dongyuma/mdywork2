// RUN: %check_clang_tidy %s gjb8114-function-declare-different-with-prototype %t

//CSR-28	函数声明必须与函数原型一致(注:一致性要求包括函数类型、参数类型、参数名。)				R-1-1-14
//CSR-28 GJB 8114-2013 R-1-1-14
//违背示例
//函数返回类型的一致性检查编译器支持，此checker也可以检测，但由于编译器报错原因，测试用例中未包含返回类型不同的测试用例
int fun1(short height); //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明必须与函数原型一致 [gjb8114-function-declare-different-with-prototype]
int fun2(int width); //违背2
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明必须与函数原型一致 [gjb8114-function-declare-different-with-prototype]
int fun3(int height); //违背3
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明必须与函数原型一致 [gjb8114-function-declare-different-with-prototype]
int fun3(double width); //违背4
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明必须与函数原型一致 [gjb8114-function-declare-different-with-prototype]
int fun1(int height);  //遵循1
int fun2(int height);  //遵循2
int fun3(int width);  //遵循3
int fun1(short width);  //违背5
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明必须与函数原型一致 [gjb8114-function-declare-different-with-prototype]
int main_2(void)
{
  int i,j1,j2,j3; 
  i=1000; 
  j1=fun1(i);
  j2=fun2(i); 
  j3=fun3(i); 
  return(0);
}
int fun1(int height)
{
  int h;
  h=height+10; 
  return h;
}
int fun2(int height)
{
  int h;
  h=height-10;
  return h;
}
int fun3(int width)
{
  int w;
  w = width-10;
  return w;
}
int fun1(short height); //违背6
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明必须与函数原型一致 [gjb8114-function-declare-different-with-prototype]
int fun1(long height);  //违背7
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明必须与函数原型一致 [gjb8114-function-declare-different-with-prototype]
int fun3(double width);  //违背8
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 函数声明必须与函数原型一致 [gjb8114-function-declare-different-with-prototype]