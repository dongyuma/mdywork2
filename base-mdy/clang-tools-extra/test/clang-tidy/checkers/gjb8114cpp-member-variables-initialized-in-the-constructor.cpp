// RUN: %check_clang_tidy %s gjb8114cpp-member-variables-initialized-in-the-constructor %t

//CSR-307	类中所有成员变量必须在构造函数中初始化。     R-2-2-4
//CSR-307 GJB 8114-2013 R-2-2-4 
//违背示例:
#include<iostream>
using namespace std; 
class Foo
{
public:
  int a;
  Foo(void);
  explicit Foo(int);
private:
  int b;
};
Foo::Foo(void) //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 类中所有成员变量必须在构造函数中初始化。 [gjb8114cpp-member-variables-initialized-in-the-constructor]
{
  a=0;
}
Foo::Foo(int varb) //违背2 
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 类中所有成员变量必须在构造函数中初始化。 [gjb8114cpp-member-variables-initialized-in-the-constructor]
{
  b=varb;
}
int main(void)
{
  Foo thefl,thef2(1);
  return(0);
}

// 遵循示例:
// #include<iostream> 
// using namespace std;
// class Foo
// {
// public:
//   int a;
//   Foo(void);
//   explicit Foo(int);
// private:
//   int b;
// };
// Foo::Foo(void) //遵循1 
// {
//   a=0;
//   b=0;
// }
// Foo::Foo(int varb) //遵循2 
// {
//   a=0;
//   b=varb;
// }
// int main(void)
// {
//   Foo thefl,thef2(1); 
//   return(0);
// }
