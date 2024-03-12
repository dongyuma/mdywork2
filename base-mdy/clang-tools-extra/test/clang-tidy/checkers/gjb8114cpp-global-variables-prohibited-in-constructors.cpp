// RUN: %check_clang_tidy %s gjb8114cpp-global-variables-prohibited-in-constructors %t

//CSR-304	构造函数中禁止使用全局变量		R-2-2-1	 gjb8114cpp-global-variables-prohibited-in-constructors
//CSR-304  GJB 8114-2013 R-2-2-1
//违背示例:
#include <iostream>
using namespace std;
int gVar=10;
class Foo
{
  public:
    Foo(void);
    ~Foo(void);
  private:
    int a;
};
Foo::Foo(void)
{
  a=gVar;  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 构造函数中禁止使用全局变量 [gjb8114cpp-global-variables-prohibited-in-constructors]
  a=2*gVar; //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 构造函数中禁止使用全局变量 [gjb8114cpp-global-variables-prohibited-in-constructors]
  for(int i=0;i<5;i++)
  {
    a=a+gVar;  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 构造函数中禁止使用全局变量 [gjb8114cpp-global-variables-prohibited-in-constructors]
    gVar++;  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 构造函数中禁止使用全局变量 [gjb8114cpp-global-variables-prohibited-in-constructors]
  }
}
Foo::~Foo(void)
{
}
int main(void)
{
  gVar++;  //遵循1
  Foo thef;
  return (0);
}
//遵循示例
//#include<iostream>
//using namespace std;
//int gVar=10;
//class Foo
//{
  //public:
    //Foo(void);
    //explicit Foo(int);
    //~Foo(void);
  //private:
    //int a;
//};
//Foo::Foo(void)
//{
  //a=0;
//}
//Foo::Foo(int var)
//{
  //a=var;
//}
//Foo::~Foo(void)
//{
//}
//int main(void)
//{
  //Foo thef(gVar);  //遵循1
  //return (0);
//}
