// RUN: %check_clang_tidy %s gjb8114cpp-virtual-destructors-in-class-with-virtual-functions %t

//CSR-309	具有虚拟成员函数的类，析构函数必须是虚拟的		R-2-3-1	 gjb8114cpp-virtual-destructors-in-class-with-virtual-functions
//CSR-309  GJB 8114-2013 R-2-3-1
//违背示例:
#include<iostream>
using namespace std;
class B
{
  public:
    B(void);
    ~B(void);  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 具有虚拟成员函数的类，析构函数必须是虚拟的 [gjb8114cpp-virtual-destructors-in-class-with-virtual-functions]
    virtual void f(int a)=0;
  protected:
    B(const B&);
    B& operator=(const B&);
};
B::B(void)
{
}
B::~B(void)
{
}
class D:public B
{
  public:
    D(void);
    explicit D(int a);
    ~D(void);  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 具有虚拟成员函数的类，析构函数必须是虚拟的 [gjb8114cpp-virtual-destructors-in-class-with-virtual-functions]
    virtual void f(int a);
  private:
    int* md;
    D(const D&);
    D& operator=(const D&);
};
D::D(int a) : B(),md(new int)
{
  *md=a;
}
void D::f(int a)
{
  *md=a;
}
D::~D(void)
{
  delete md;
  md=NULL;
}
int main(void)
{
  B* d=new D(1);
  d->f(2);
  delete d;
  d=NULL;
  return (0);
}
// 遵循示例
// #include<iostream>
// using namespace std;
// class B
// {
//   public:
//     B(void);
//     virtual~B(void);  //遵循1
//     virtual void f(int a)=0;
//   protected:
//     B(const B&);
//     B& operator=(const B&);
// };
// B::B(void)
// {
// }
// B::~B(void)
// {
// }
// class D:public B
// {
//   public:
//     D(void);
//     explicit D(int a);
//     virtual~D(void);  //遵循2
//     virtual void f(int a);
//   private:
//     int* md;
//     D(const D&);
//     D& operator=(const D&);
// };
// D::D(int a) : B(),md(new int)
// {
//   *md=a;
// }
// void D::f(int a)
// {
//   *md=a;
// }
// D::~D(void)
// {
//   delete md;
//   md=NULL;
// }
// int main(void)
// {
//   B* d=new D(1);
//   d->f(2);
//   delete d;
//   d=NULL;
//   return (0);
// }
