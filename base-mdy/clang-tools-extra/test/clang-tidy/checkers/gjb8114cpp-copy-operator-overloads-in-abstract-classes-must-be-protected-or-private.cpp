//RUN: %check_clang_tidy %s gjb8114cpp-copy-operator-overloads-in-abstract-classes-must-be-protected-or-private %t

// 违背示例 
#include <iostream> 
using namespace std; 
class B
{
  public:
  B(void);
  virtual~B(void);
  virtual void f(void)=0;
  B &operator=(B const &rhs); //违背1 
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 抽象类中的复制操作符重载必须是保护的或私有的 [gjb8114cpp-copy-operator-overloads-in-abstract-classes-must-be-protected-or-private]
  private:
  int kind;
};
B::B(void) :kind(0)
{}
B::~B(void)
{}
B & B::operator = (B const &rhs)
{
  kind=rhs.kind; 
  return (*this);
}

class D : public B
{
public:
  D(void);
  virtual~D(void);
  virtual void f(void) {};
  D &operator=(D const & rhs); 
private:
  int member;
};
D::D(void):B(),member(0)
{}
D::~D(void)
{}
D & D::operator=(D const &rhs)
{
  member=rhs.member; 
  return (*this);
}
int main(void)
{
  D d1; 
  D d2;
  B &b1=d1;
  B &b2=d2;
  b1=b2; //违背2 
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 抽象类中的复制操作符重载必须是保护的或私有的 [gjb8114cpp-copy-operator-overloads-in-abstract-classes-must-be-protected-or-private]
  return (0);
}

// //遵循示例
// #include <iostream> 
// using namespace std; 
// class B
// {
// public:
// B(void);
// virtual~B(void);
// virtual void f(void)=0;
// protected:
// B &operator=(B const &rhs); //遵循1 
// private:
// int kind;
// };
// B::B(void) :kind(0)
// {}
// B::~B(void)
// {}
// B & B::operator = (B const &rhs)
// {kind=rhs.kind; 
// return (*this);}

// class D : public B
// {
//   public:
// D(void);
// virtual~D(void);
// virtual void f(void) {};
// D &operator=(D const & rhs); 
// private:
// int member;
// };
// D::D(void):B(),member(0)
// {}
// D::~D(void)
// {}
// D & D::operator=(D const &rhs)
// {
// member=rhs.member; 
// return (*this);
// }
// int main(void)
// {
// D d1; 
// D d2;
// B &b1=d1;
// B &b2=d2;
// b1=b2; //遵循2   //由于采取了保护，此用法被禁止，编译报错
// return (0);
// }