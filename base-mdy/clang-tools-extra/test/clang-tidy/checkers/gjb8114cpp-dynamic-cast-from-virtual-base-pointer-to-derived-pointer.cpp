// RUN: %check_clang_tidy %s gjb8114cpp-dynamic-cast-from-virtual-base-pointer-to-derived-pointer %t

//违背示例
#include <iostream> 
using namespace std; 
class B
{
public:
B(void);
virtual~B(void);
virtual int g(int a=0); 
private:
int b;
};
B::B(void) : b(1)
{}
B::~B(void)
{}
int B::g(int a)
{return (a+b);}
class D :public virtual B
{
public: 
D(void);
virtual~D(void);
virtual int g(int a=0); 
private:
int d;
};
D::D(void):B(),d(2)
{}
D::~D(void)
{}
int D::g(int a)
{return(a+d);}
int main(void)
{
D d;
B &b=d; 
B *pb=&d;
D *pd1=reinterpret_cast<D*>(pb); //违背1 
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 虚拟基类指针转换为派生类指针必须使用dynamic_cast转换 [gjb8114cpp-dynamic-cast-from-virtual-base-pointer-to-derived-pointer]
D &pd2=reinterpret_cast<D&>(*pb); //违背2 
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 虚拟基类指针转换为派生类指针必须使用dynamic_cast转换 [gjb8114cpp-gjb8114cpp-dynamic-cast-from-virtual-base-pointer-to-derived-pointer]
return (0);
}

//遵循示例
// #include <iostream> 
// using namespace std; 
// class B
// {
// public:
// B(void);
// virtual~B(void);
// virtual int g(int a=0); 
// private:
// int b;};
// B::B(void) : b(1)
// {}
// B::~B(void)
// {}
// int B::g(int a)
// {return (a+b);}
// class D :public virtual B
// {
// public:
// D(void);
// virtual~D(void);
// virtual int g(int a=0);
// private:
// int d;
// };
// D::D(void):B(),d(2) 
// {}
// D::~D(void)
// {}
// int D::g(int a)
// {return(a+d);}
// int main(void)
// {
// D d;
// B &b=d; 
// B *pb=&d;
// D *pd1=dynamic_cast<D*>(pb); //遵循1 
// D &pd2=dynamic_cast<D&>(*pb); //遵循2 
// return (0);
// }