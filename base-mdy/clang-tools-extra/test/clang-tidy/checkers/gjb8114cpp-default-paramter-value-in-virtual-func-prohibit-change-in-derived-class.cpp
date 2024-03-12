// RUN: %check_clang_tidy %s gjb8114cpp-default-paramter-value-in-virtual-func-prohibit-change-in-derived-class %t
#include <iostream>
using namespace std;
/// GJB 8114-2013 R-2-4-1 CSR-311	基类虚拟函数的参数缺省值在派生类重写函数中禁止被改变。
class Base
{
public:
    Base(void);
    virtual ~Base(void);
    virtual int g(int a=0);
    virtual int g1(int a = 1, int b = 2);
};
Base::Base(void)
{
}
Base::~Base(void){

}
int Base::g(int a)
{
    return (a +1);
}
int Base::g1(int a, int b)
{
    return a + b;
}
class Derived:public virtual Base
{
public:
    Derived(void);
    virtual ~Derived(void);
    virtual int g(int a = 1); //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:27: warning: 基类虚拟函数的参数缺省值在派生类重写函数中禁止被改变。 [gjb8114cpp-default-paramter-value-in-virtual-func-prohibit-change-in-derived-class]
    virtual int g1(int a = 0, int b = 1);
// CHECK-MESSAGES: :[[@LINE-1]]:28: warning: 基类虚拟函数的参数缺省值在派生类重写函数中禁止被改变。 [gjb8114cpp-default-paramter-value-in-virtual-func-prohibit-change-in-derived-class]
// CHECK-MESSAGES: :[[@LINE-2]]:39: warning: 基类虚拟函数的参数缺省值在派生类重写函数中禁止被改变。 [gjb8114cpp-default-paramter-value-in-virtual-func-prohibit-change-in-derived-class]
};
Derived::Derived(void)
{

}
Derived::~Derived(void)
{

}
int Derived::g(int a)
{
    return (a+11);
}
int Derived::g1(int a , int b )
{
    return a + b;
}
int main(void)
{
    int i;
    Derived d;
    Base &b = d;
    i = b.g();
    i = d.g();
    return (0);
}

class Base1
{
public:
    Base1(void);
    virtual ~Base1(void);
    virtual int g1(int a=0);
    virtual int g2(int a=0);
};
Base1::Base1(void)
{
}
Base1::~Base1(void){

}
int Base1::g1(int a)
{
    return (a +1);
}
int Base1::g2(int a)
{
    return (a +2);
}
class Derived1:public virtual Base1
{
public:
    Derived1(void);
    virtual ~Derived1(void);
    virtual int g1(int a = 0); //遵循1
    virtual int g2(int a); //遵循2
};
Derived1::Derived1(void)
{

}
Derived1::~Derived1(void)
{

}
int Derived1::g1(int a)
{
    return (a+11);
}
int Derived1::g2(int a)
{
    return (a+12);
}