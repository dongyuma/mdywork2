// RUN: %check_clang_tidy %s gjb8114-madongyu-c++vertify %t
// 6.1.1.1 准则R-2-1-1
// 含有动态分配成员的类，必须编写拷贝构造函数，并重载赋值操作符。
// 遵循示例:
#include <iostream>
using namespace std;
class A
{
public:
  A (char *cstr);
  A (const A &ca);
  A (void);
  ~A (void);
  operator=(const A &ca);
private:
  char *str;
};
A::A (char *cstr)
{
  str=new char[20];
  strncpy(str,cstr,20);
}
A::A(const A &ca) //遵循1
{
  str=new char[20];
  strncpy(str,ca.str,20);
}

A::operator=(const A &ca) //遵循2
{
  if(NULL==str)
  {
    str= new char[20];
  }
  strncpy(str,ca.str,20);
}
A::A(void) : str(new char[20])
{
  strncpy(str,"Welcome!",20);
}
A::~A(void)
{
  delete[] str;
  str=NULL;
}
int main(void)
{
  A a("Hello world!");
  A b(a);
  A c;
  c = b;
  return(0);
}//strncpy那里有红线，A那里有红线
//6.1.1.2 准则R-2-1-2
//虚拟基类指针转换为派生类指针必须使用dynamic_cast转换。
//违背示例:
#include<iostream>
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
B::B(void):b(1)
{
}
B::~B(void)
{
}
int B::g(int a)
{
  return(a+b);
}
class D:public virtual B
{
public:
  D(void);
  virtual~D(void);
  virtual int g(int a=0);
private:
  int d;
};
D::D(void) : B(),d(2)
{
}
D::~D(void)
{
}
int D::g(int a)
{
  return(a+d);
}
int main(void)
{
  D d;
  B &b = d;
  B *pb = &d;
  D *pd1 = reinterpret_cast<D*>(pb); //违背1
  D &pd2=reinterpret_cast<D&>(*pb); //违背2
  return(0);
}
//遵循示例:
#include<iostream>
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
B::B(void):b(1)
{
}
B::~B(void)
{
}
int B::g(int a)
{
  return(a+b);
}
class D:public virtual B
{
public:
  D(void);
  virtual~D(void);
  virtual int g(int a=0);
private:
  int d;
};
D::D(void) : B(),d(2)
{
}
D::~D(void)
{
}
int D::g(int a)
{
  return(a+d);
}
int main(void)
{
  D d;
  B &b=d;
  B *pb=&d;
  D *pdl = dynamic_cast<D*>(pb); //遵循1
  D &pd2=dynamic_cast<D&>(*pb); //遵循2
  return(0);
}
// 6.1.1.3准则R-2-1-3
// 菱形层次结构的派生设计，对基类派生必须使用virtual说明。
// 违背示例:
#include <iostream>
using namespace std;
class A
{
public:
  A(void);
  void SetA(int);
private:
  int a;
};
A::A(void):a(0)
{
}
void A::SetA(int va)
{
  a= va;
}
class B1 : public A //违背1
{
public:
  B1(void) ;
  void SetB1 (int);
private:
  int b1;
};
B1::B1(void) : A(), b1(0)
{
}
void B1::SetB1(int vb)
{
  b1= vb;
  SetA(b1+1);
}
class B2 : public A //违背2
{
public:
  B2(void);
  void SetB2(int);
private:
  int b2;
};
B2::B2(void) : A(), b2(0)
{
}
void B2::SetB2(int vb)
{
  b2=vb;
  SetA(b2+2);
}
class D : public B1, public B2
{
public:
  D(void);
private:
  int d;
};
D::D(void) : B1(), B2(), d(0)
{
}
int main(void)
{
  D thed;
  thed.SetB1(1);
  thed.SetB2(2);
  return(0);
}
// 遵循示例
#include <iostream>
using namespace std;
class A
{
public:
  A(void);
  void SetA(int);
private:
  int a;
};
A::A(void):a(0)
{
}
void A::SetA(int va)
{
  a= va;
}
class B1 : public virtual A //遵循1
{
public:
  B1(void) ;
  void SetB1 (int);
private:
  int b1;
};
B1::B1(void) : A(), b1(0)
{
}
void B1::SetB1(int vb)
{
  b1= vb;
  SetA(b1+1);
}
class B2 : public virtual A //遵循2
{
public:
  B2(void);
  void SetB2(int);
private:
  int b2;
};
B2::B2(void) : A(), b2(0)
{
}
void B2::SetB2(int vb)
{
  b2=vb;
  SetA(b2+2);
}
class D : public B1, public B2
{
public:
  D(void);
private:
  int d;
};
D::D(void) : A(), B1(), B2(), d(0)
{
}
int main(void)
{
  D thed;
  thed.SetB1(1);
  thed.SetB2(2);
  return(0);
}
// 6.1.1.4准则R-2-1-4
// 抽象类中的复制操作符重载必须是保护的或私有的。
// 违背示例:
#include<iostream>
using namespace std;
class B
{
public:
  B(void);
  virtual~B(void);
  virtual void f(void)=0;
  B & operator=(B const &rhs);//违背1
private:
  int kind;
};
B::B(void):kind(0)
{
}
B::~B(void)
{
}
B & B::operator=(B const &rhs)
{
  kind=rhs.kind;
  return(*this);
}
class D:public B
{
public:
  D(void);
  virtual~D(void);
  virtual void f(void) {};
  D & operator=(D const & rhs);
private:
  int member;
};
D::D(void):B(),member(0)
{
}
D::~D(void)
{
}
D&D::operator=(D const &rhs)
{
  member=rhs.member;
  return(*this);
}
int main(void)
{
  D d1;
  D d2;
  B &b1=d1;
  B &b2=d2;
  b1=b2;
  return(0);
}
// 遵循示例：
#include<iostream>
using namespace std;
class B
{
public:
  B(void);
  virtual~B(void);
  virtual void f(void)=0;
protected:
  B & operator=(B const &rhs);//遵循1
private:
  int kind;
};
B::B(void):kind(0)
{
}
B::~B(void)
{
}
B & B::operator=(B const &rhs)
{
  kind=rhs.kind;
  return(*this);
}
class D:public B
{
public:
  D(void);
  virtual~D(void);
  virtual void f(void) {};
  D & operator=(D const & rhs);
private:
  int member;
};
D::D(void):B(),member(0)
{
}
D::~D(void)
{
}
D&D::operator=(D const &rhs)
{
  member=rhs.member;
  return(*this);
}
int main(void)
{
  D d1;
  D d2;
  B &b1=d1;
  B &b2=d2;
  b1=b2;                           //由于采取了保护，此用法被禁止，编译报错
  return(0);
}
// 6.1.2建议准则
// 6.1.2.1准则A-2-1-1
// 谨慎使用派生类由虚拟基类派生。
// 6.1.2.2准则A-2-1-2
// 谨慎使用内联函数。
// 6.2 构造函数
// 6.2.1 强制准则
// 6.2.1.1 准则R-2-2-1
// 构造函数中禁止使用全局变量。
// 违背示例：
#include<iostream>
using namespace std;
int gVar= 10;
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
  a =gVar;//违背1
}
Foo::~Foo(void)
{
}
int main(void)
{
  Foo thef;
  return (0);
}
// 遵循示例：
#include<iostream>
using namespace std;
int gVar= 10;
class Foo
{
public:
  Foo(void);
  explicit Foo(int);
  ~Foo(void);
private:
  int a;
};
Foo::Foo(void)
{
  a =0;
}
Foo::Foo(int var)
{
  a =var;
}
Foo::~Foo(void)
{
}
int main(void)
{
  Foo thef(gVar); //遵循1
  return (0);
}
// 6.2.1.2准则R-2-2-2
// 类中必须明确定义缺省构造函数。
// 违背示例:
#include <iostream>
using namespace std;
class Foo //违背1
{
public:
  ~Foo(void);
  void SetVar(int);
private:
  int a;
};
Foo::~Foo(void)
{
}
void Foo::SetVar(int var)
{
  a = var;
}
int main(void)
{
  Foo thef;
  thef.SetVar(2);
  return(0);
}
//遵循示例：
#include <iostream>
using namespace std;
class Foo
{
public:
  Foo(void);   //遵循1
  ~Foo(void);
  void SetVar(int);
private:
  int a;
};
Foo::~Foo(void)
{
}
void Foo::SetVar(int var)
{
  a = var;
}
int main(void)
{
  Foo thef;
  thef.SetVar(2);
  return(0);
}
// 6.2.1.3准则R-2-2-3
// 单参数构造函数必须使用explicit声明。
// 违背示例:
#include<iostream>
using namespace std;
class Foo
{
public:
  Foo(void);
  Foo(int var); //违背1
  int getVar(void);
  ~Foo(void);
private:
  int a;
};
Foo::Foo(void)
{
  a=0;
}
Foo::Foo(int var)
{
  a=var;
}
int Foo::getVar(void)
{
  return a;
}
Foo::~Foo(void)
{
}
void f(Foo);
int main(void)
{
  int i =1;
  f(i);
  return(0);
}
void f(Foo thef)
{
  int j;
  //.....
  j=thef.getVar();
}
//遵循示例：
#include<iostream>
using namespace std;
class Foo
{
public:
  Foo(void);
  explicit Foo(int var); //遵循1
  int getVar(void);
  ~Foo(void);
private:
  int a;
};
Foo::Foo(void)
{
  a=0;
}
Foo::Foo(int var)
{
  a=var;
}
int Foo::getVar(void)
{
  return a;
}
Foo::~Foo(void)
{
}
void f(int);
int main(void)
{
  Foo thef(1);
  int i =1;
  f(i);
  return(0);
}
void f(Foo thef)
{
  int j;
  //.....
  j=thef.getVar();
}
// 6.2.1.4 准则R-2-2-4
// 类中所有成员变量必须在构造函数中初始化。
// 违背示例:
#include <iostream>
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
{
  a=0;
}
Foo::Foo(int varb) //违背2
{
  b=varb;
}
int main(void)
{
  Foo thef1,thef2(1);
  return(0);
}
// 遵循示例：
#include <iostream>
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
{
  a=0;
  b=0;
}
Foo::Foo(int varb) //违背2
{
  a=0;
  b=varb;
}
int main(void)
{
  Foo thef1,thef2(1);
  return(0);
}
// 6.2.1.5 准则R-2-2-5
// 派生类构造的数必须在初始化列表中说明直接基类构造函数。
// 违背示例:
#include<iostream>
using namespace std;
class Document
{
public:
  int docid;
  Document(void):docid(0)
  {
  }
  explicit Document(int);
};
Document::Document(int var)
{
  docid=var;
}
class Book :public Document
{
  public:
  int bookid;
  Book(void):bookid(1) //违背1
  {
  }
};
int main(void)
{
  Book mybook;
  return(0);
}
//遵循示例：
#include<iostream>
using namespace std;
class Document
{
public:
  int docid;
  Document(void):docid(0)
  {
  }
  explicit Document(int);
};
Document::Document(int var)
{
  docid=var;
}
class Book :public Document
{
public:
  int bookid;
  Book(void):Document(1),bookid(1) //遵循1
  {
  }
};
int main(void)
{
  Book mybook;
  return(0);
}
// 6.3 析构函数
// 6.3.1 强制准则
// 6.3.1.1 准则R-2-3-1
// 具有虚拟成员函数的类，析构函数必须是虚拟的。
// 违背示例:
#include <iostream>
using namespace std;
class B
{
public:
B(void);
~B(void); //违背1
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
  ~D(void); //违背2
  virtual void f(int a);
private:
  int* md;
  D(const D&);
  D& operator=(const D&);
};
D::D(int a):B(),md(new int)
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
  return(0);
}
//遵循示例：
#include <iostream>
using namespace std;
class B
{
public:
  B(void);
  virtual~B(void); //遵循1
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
  virtual~D(void); //遵循2
  virtual void f(int a);
private:
  int* md;
  D(const D&);
  D& operator=(const D&);
};
D::D(int a):B(),md(new int)
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
  return(0);
}
// 6.3.1.2 准则R-2-3-2
// 析构函数中禁止存在不是由自身捕获处理的异常。
// 违背示例:
#include<iostream>
using namespace std;
class Foo
{
public:
  int a;
  Foo(void);
  ~Foo(void);
};
Foo::Foo(void)
{
  a=1;
}
Foo::~Foo(void)
{
  if(1==a)
  {
    throw 0;
  }     //违背1
}
int main(void)
{
  try
  {
    Foo f;
  }
  catch(int e)
  {
    //....
  }
  return(0);
}
//遵循示例：
#include<iostream>
using namespace std;
class Foo
{
public:
  int a;
  Foo(void);
  ~Foo(void);
};
Foo::Foo(void)
{
  a=1;
}
Foo::~Foo(void)
{
  try
  {
    if(1==a)
    {
    throw 0;
    }
  }
  catch (int e) //遵循1
  {
    //......
  }
}
int main(void)
{
  try
  {
    Foo f;
  }
  catch(int e)
  {
    //....
  }
  return(0);
}
// 6.4虚拟函数
// 6.4.1 强制准则
// 6411准则R-2-4-1
// 基类虚拟函数的参数缺省值在派生类重写函数中禁止被改变。
// 违背示例:
#include<iostream>
using namespace std;
class Base
{
public:
  Base(void);
  virtual~Base(void);
  virtual int g(int a=0);
};
Base::Base(void)
{
}
Base::~Base(void)
{
}
int Base::g(int a)
{
  return (a+1);
}
class Derived :public virtual Base
{
public:
  Derived(void);
  virtual~Derived(void);
  virtual int g(int a =1); //违背1
};
Derived::Derived(void)
{
}
Derived::~Derived(void)
{
}
int Derived::g(int a)
{
  return(a+11);
}
int main(void)
{
  int i;
  Derived d;
  Base &b=d;
  i=b.g();
  i=d.g();
  return(0);
}
// 遵循示例:
#include<iostream>
using namespace std;
class Base
{
public:
  Base(void);
  virtual~Base(void);
  virtual int g1(int a=0);
  virtual int g2(int a=0);
};
Base::Base(void)
{
}
Base::~Base(void)
{
}
int Base::g1(int a)
{
  return (a+1);
}
int Base::g2(int a)
{
  return (a+2);
}
class Derived :public Base
{
public:
  Derived(void);
  virtual~Derived(void);
  virtual int g1(int a =0); //遵循1
  virtual int g2(int a);
};
Derived::Derived(void)
{
}
Derived::~Derived(void)
{
}
int Derived::g1(int a)
{
  return(a+11);
}
int Derived::g2(int a)
{
  return(a+12);
}
int main(void)
{
  int i,j;
  Derived d;
  Base &b=d;
  i=b.g1();
  i=d.g1();
  j=b.g2();
  j=d.g2(0);
  return(0);
}
//  6.4.1.2准则R-2-4-2
// 派生类对基类虚拟函数重写的声明必须使用virtual显示说明。
// 违背示例:
#include<iostream>
using namespace std;
class Base
{
public:
Base(void);
virtual~Base(void);
virtual int g(int a=0);
};
Base::Base(void)
{
}
Base::~Base(void)
{
}
int Base::g(int a)
{
  return(a+1);
}
class Derived:public Base
{
public:
  Derived(void);
  virtual~Derived(void);
  int g(int a=0); //违背1
};
Derived::Derived(void)
{
}
Derived::~Derived(void)
{
}
int Derived::g(int a)
{
  return(a+2);
}
int main(void)
{
  int i,j;
  Derived d;
  Base &b=d;
  i=b.g();
  j=d.g();
  return(0);
}
// //遵循示例：
#include<iostream>
using namespace std;
class Base
{
public:
  Base(void);
  virtual~Base(void);
  virtual int g(int a=0);
};
Base::Base(void)
{
}
Base::~Base(void)
{
}
int Base::g(int a)
{
  return(a+1);
}
class Derived:public Base
{
public:
  Derived(void);
  virtual~Derived(void);
  virtual int g(int a=0); //遵循1
};
Derived::Derived(void)
{
}
Derived::~Derived(void)
{
}
int Derived::g(int a)
{
  return(a+2);
}
int main(void)
{
  int i,j;
  Derived d;
  Base &b=d;
  i=b.g();
  j=d.g();
  return(0);
}
// 6.4.1.3准则R-2-4-3
// 禁止非纯虚函数被纯虚拟函数重写。
// 违背示例:
#include<iostream>
using namespace std;
class A
{
public:
  A(void);
  virtual~A(void);
  virtual void foo(void)= 0;
};
A::A(void)
{
}
A::~A(void)
{
}
class B: public A
{
public:
  B(void);
  virtual~B(void);
  virtual void foo(void);
};
B::B(void)
{
}
B::~B(void)
{
}
void B::foo(void)
{
}
class C: public B
{
public:
  C(void);
  virtual~C(void);
  virtual void foo(void)=0; //违背1
};
C::C(void)
{
}
C::~C(void)
{
}
int main(void)
{
  B myb;
  myb.foo();
  return(0);
}
// 6.5 类型转换
// 6.5.1 强制准则
// 6.5.1.1准则R-2-5-1
// 禁止将不相关的指针类型强制转换为对象指针类型。
// 违背示例:
#include<iostream>
using namespace std;
struct S
{
  int i;
  int j;
  int k;
};
class C
{
public:
  int i;
  int j;
  int k;
  C(void);
  virtual~C(void);
};
C::C(void):i(0),j(0),k(0)
{
}
C::~C(void)
{
}
int main(void)
{
  S *s=new S;
  s->i=0;
  s->j=0;
  s->k=0;
  C*c=reinterpret_cast<C *>(s); //违背1
  //...
  return(0);
}
// 6.5.1.2 R-2-5-1
// 指针或引用的类型转换中禁止移除const或volatile属性。
// 违背示例:
#include<iostream>
using namespace std;
class A
{
public:
  A(void);
  ~A(void);
  explicit A (int);
  int ma;
};
A::A(void)
{
  ma=0;
}
A::A(int a)
{
  ma=a;
}
A::~A(void)
{
}
int main(void)
{
  A const a1 = A(10);
  A *a2=const_cast<A*>(&a1);//违背1
  a2->ma=11;
  A &a3=const_cast<A&>(a1);//违背2
  a3.ma=12;
  return(0);
}
// 6.5.2建议准则A-2-5-1
// 建议使用C++的类型转换操作符，避免使用C的类型转换形式。
// 6.6内存释放
// 6.6.1强制准则
// 6.6.1.1准则R-2-6-1
// 使用new分配的内存空间，用完后必须使用delete释放。
// 违背示例:
#include<iostream>
using namespace std;
void fun1(void)
{
  int *p=new int; //违背1
  *p=1;
  //……
}
void fun2(void)
{
  int*p=new int[3]; //违背2
  p[0]=1;
  p[1]= 2;
  p[2]= 3;
  //.....
}
int main(void)
{
  fun1();
  fun2();
  return(0);
}
//遵循示例：
#include<iostream>
using namespace std;
void funl(void)
{
  int *p=new int;
  *p=1;
  delete p;     //遵循1
  p=NULL;
  //……
}
void fun2(void)
{
  int*p=new int[3];
  p[0]=1;
  p[1]= 2;
  p[2]= 3;
  delete [] p;    //遵循2
  p=NULL;
}
int main(void)
{
  fun1();
  fun2();
  return(0);
}
// 6.6.1.2准则R-2-6-2
// 必须使用delete[]释放new[]分配的内存空间。
// 违背示例:
#include<iostream>
using namespace std;
void fun(void)
{
  int *p=new int[3];
  p[0]=1;
  p[1]=2;
  p[2]=3;
  delete p; //违背1
  p=NULL;
}
int main(void)
{
  fun();
  return(0);
}
//遵循示例：
#include<iostream>
using namespace std;
void fun(void)
{
  int *p=new int[3];
  p[0]=1;
  p[1]=2;
  p[2]=3;
  delete[] p; //遵循1
  p=NULL;
}
int main(void)
{
  fun();
  return(0);
}

// 6.6.1.3准则R-2-6-3
// 被delete的指针必须指向最初new分配的地址。
// 违背示例:
#include <iostream>
using namespace std;
int fun(void);
int main(void)
{
  int i;
  i=fun();
  return (0);
}
int fun(void)
{
  int *p=new int[3];
  if(NULL==p)
  {
    return(-1);
  }
  else 
  {
    *p=1;
    p++;
    *p=2;
    delete[] p;//违背1
    p=NULL;
  }
  return (0);
}
//遵循示例：
#include <iostream>
using namespace std;
int fun(void);
int main(void)
{
  int i;
  i=fun();
  return (0);
}
int fun(void)
{
  int *p=new int[3];
  int *pbak=p;
  if(NULL==p)
  {
    return(-1);
  }
  else 
  {
    *p=1;
    p++;
    *p=2;
    delete[] pbak;  //遵循1
    pbak=NULL;
  }
  return (0);
}
// 6.7函数定义与使用
// 6.7.1强制准则
// 6.7.1.1准则R-2-7-1
// 函数中固定长度数组变量的传递必须使用引用方式。
// 违背示例:
#include<iostream>
using namespace std;
void fun1(int p[10]) //违背1
{
  int i;
  for(i=0;i<10;i++)
  {
    p[i]=p[i]+ 1;
  }
}
void fun2(int *p) //违背2
{
  int i;
  for(i=0;i<10;i++)
  {
    p[i] = p[i] + 1;
  }
}
int main(void)
{
  int a[10]={0,1,2,3,4,5,6,7,8,9};
  fun1(a);
  fun2(a);
  return(0);
}
//遵循示例：
#include<iostream>
using namespace std;
void fun(int (&p)[10]) //遵循1
{
  int i;
  for(i=0;i<10;i++)
  {
    p[i]=p[i]+ 1;
  }
}
int main(void)
{
  int a[10]={0,1,2,3,4,5,6,7,8,9};
  fun(a);
  return(0);
}
// 6.7.1.2准则R-2-7-2
// 定义为const的成员函数禁止返回非const的指针或引用。
// 违背示例:
#include<iostream>
using namespace std;
class A
{
public:
  A(int n_,int*b_);
  A(const A &ca);
  A(void);
  ~A(void);
  operator=(const A &ca);
  int *get_b(void) const;
private:
  int n;
  int *b;
};
A::A(int n_,int *b_)
{
  n=n_;
  b=new int[n];
  for(int i=0;i<n;i++)
  {
    b[i]=b_[i];
  }
}
A::A(const A &ca)
{
  n=ca.n;
  b=new int[n];
  for(int i=0;i<n;i++)
  {
    b[i]=ca.b[i];
  }
}
A::operator=(const A &ca)
{
  n=ca.n;
  b=new int[n];
  for(int i=0;i<n;i++)
  {
    b[i]=ca.b[i];
  }
}
A::A(void):b(new int[0]),n(1)
{
  b[0]= 0;
}
A::~A(void)
{
  delete[] b;
  b=NULL;
}
int *A::get_b(void) const //违背1
{
  return b;
}
int main(void)
{
  int i=0;
  int b[2] ={1,2};
  A const a(2,b);
  a.get_b()[0]=i;
  return(0);
}//有问题？//operator那里有红线，A那里有红线
//遵循示例：
#include<iostream>
using namespace std;
class A
{
public:
  A(int n_,int*b_);
  A(const A &ca);
  A(void);
  ~A(void);
  operator=(const A &ca);
  const int *get_b(void) const;
private:
  int n;
  int *b;
};
A::A(int n_,int *b_)
{
  n=n_;
  b=new int[n];
  for(int i=0;i<n;i++)
  {
    b[i]=b_[i];
  }
}
A::A(const A &ca)
{
  n=ca.n;
  b=new int[n];
  for(int i=0;i<n;i++)
  {
    b[i]=ca.b[i];
  }
}
A::operator=(const A &ca)
{
  n=ca.n;
  b=new int[n];
  for(int i=0;i<n;i++)
  {
    b[i]=ca.b[i];
  }
}
A::A(void):b(new int[0]),n(1)
{
  b[0]= 0;
}
A::~A(void)
{
  delete[] b;
  b=NULL;
}
const int *A::get_b(void) const //遵循1
{
  return b;
}
int main(void)
{
  int i=0;
  int b[2] ={1,2};
  A const a(2,b);
  i=a.get_b()[0];
  return(0);
}//有问题？//operator那里有红线，A那里有红线
// 6.7.1.3  准则R-2-7-3
// 禁止可导致非资源性对象数据被外部修改的成员函数返回。
// 违背示例:
#include<iostream>
using namespace std;
class A
{
public:
  A(int n_,int*b_);
  A(const A &ca);
  A(void);
  ~A(void);
  operator=(const A &ca);
  int &get_n(void);
private:
  int n; int *b;
};

A::A(int n_,int *b_)
{
  n=n_;
  b=new int[n];
  for(int i=0;i<n;i++)
  {
    b[i]=b_[i];
  }
}
A::A(const A &ca)
{
  n=ca.n;
  b=new int[n];
  for(int i=0;i<n;i++)
  {
    b[i]=ca.b[i];
  }
}
A::operator=(const A &ca)
{
  n=ca.n;
  b=new int[n];
  for(int i=0;i<n;i++)
  {
    b[i]= ca.b[i];
  }
}
A::A(void):b(new int[0]),n(1)
{
  b[0]= 0;
}
A::~A(void)
{
  delete[] b;
  b=NULL;
}
int &A::get_n(void) //违背1
{
  return n;
}
int main(void)
{
  int b[2]={1,2};
  A a(2,b);
  int &n= a.get_n();
  n=10;
  return(0);
}//有问题？//operator那里有红线，A那里有红线
//遵循示例：
#include<iostream>
using namespace std;
class A
{
public:
  A(int n_,int *b_);
  A(const A &ca);
  A(void);
  ~A(void);
  operator=(const A &ca);
  int get_n(void);
private:
  int n; int *b;
};
A::A(int n_,int *b_)
{
  n=n_;
  b=new int[n];
  for(int i=0;i<n;i++)
  {
    b[i]=b_[i];
  }
}
A::A(const A &ca)
{
  n=ca.n;
  b=new int[n];
  for(int i=0;i<n;i++)
  {
    b[i]=ca.b[i];
  }
}
A::operator=(const A &ca)
{
  n=ca.n;
  b=new int[n];
  for(int i=0;i<n;i++)
  {
    b[i]= ca.b[i];
  }
}
A::A(void):b(new int[0]),n(1)
{
  b[0]= 0;
}
A::~A(void)
{
  delete[] b;
  b=NULL;
}
int A::get_n(void) //遵循1
{
  return n;
}
int main(void)
{
  int b[2]={1,2};
  A a(2,b);
  int n= a.get_n();
  //...
  return(0);
}//有问题？//operator那里有红线，A那里有红线

// 6.7.2 建议准则
// 6.7.2.1 准则A-2-7-1
// 类中雨数的实现代码避免在类定义的内部定义。
// 说明性示例:
#include<iostream>
using namespace std;
class Foo
{
public:
  Foo(void);
  ~Foo(void);
  void SetVar(int var); //遵循1
private:
  int a;
};
Foo::Foo(void)
{
  a=0;
}
Foo::~Foo(void)
{
}
void Foo::SetVar(int var)
{
  a=var;
}
int main(void)
{
  Foo thef;
  thef.SetVar(2);
  return(0);
}
// 6.7.2.2准则A-2-7-2
// 函数中的指针或引用参数如果不是修改项建议使用const说明。
// 6.8异常处理
// 6.8.1强制准则
// 6.8.1.1 准则R-2-8-1
// 捕获的顺序必须按由派生类到基类的次序排序。
// 违背示例:
#include<iostream>
using namespace std;
class Document
{
public:
  Document(void):docid(0)
  {
  }
  int get_docid(void);
private:
  int docid;
};
int Document::get_docid(void)
{
  return docid;
}
class Book : public Document
{
public:
  Book(void):Document(),bookid(0)
  {
  }
  int get_bookid(void);
private:
  int bookid;
};
int Book::get_bookid(void)
{
  return bookid;
}
int main(void)
{
  Book mybook;
  try
  {
    throw mybook;
  }
  catch(Document &d) //违背1
  {
    cout<<d.get_docid()<<endl;
  }
  catch(Book &b) //违背2
  {
    cout<<b.get_bookid()<<endl;
  }
  return(0);
}
//遵循示例：
#include<iostream>
using namespace std;
class Document
{
public:
  Document(void):docid(0)
  {
  }
  int get_docid(void);
private:
  int docid;
};
int Document::get_docid(void)
{
  return docid;
}
class Book : public Document
{
public:
  Book(void):Document(),bookid(0)
  {
  }
  int get_bookid(void);
private:
  int bookid;
};
int Book::get_bookid(void)
{
  return bookid;
}
int main(void)
{
  Book mybook;
  try
  {
    throw mybook;
  }
  catch(Book &b) //遵循1
  {
    cout<<b.get_bookid()<<endl;
  }
  catch(Document &d) //遵循2
  {
    cout<<d.get_docid()<<endl;
  }
  return(0);
}
// 6.8.1.2 准则R-2-8-2
// 每个指定的抛出必须有与之匹配的捕获。
// 违背示例:
#include<iostream>
using namespace std;
class A1
{
public:
  A1(void)
  {
  }
};
class A2
{
  public:
  A2(void)
  {
  }
};
int main(void)
{
  int i=0;
  //..
  try
  {
    if(i>0)
    {
      throw A1();
    }
    else
    {
      throw A2();//违背1
    }
  }
  catch(A1 &)
  {
    cout<<"exception: A1"<<endl;
  }
  return(0);
}
// 遵循示例：
#include<iostream>
using namespace std;
class A1
{
  public:
  A1(void)
  {
  }
};
class A2
{
public:
  A2(void)
  {
  }
};
int main(void)
{
  int i=0;
  //..
  try
  {
    if(i>0)
    {
      throw A1();//遵循1
    }
    else
    {
      throw A2();//遵循2
    }
  }
  catch(A1 &)
  {
    cout<<"exception: A1"<<endl;
  }
  catch(A2 &)
  {
    cout<<"exception: A2"<<endl;
  }
  return(0);
}
// 6.8.1.3准则R-2-8-3
// 异常抛出的对象必须使用引用方式捕获。
// 违背示例:
#include <iostream>
using namespace std;
class Document
{
public:
  Document(void):docid(0)
  {
  }
  int get_docid(void);
private:
  int docid;
};
int Document::get_docid(void)
{
  return docid;
}
int main(void)
{
  try
  {
    Document mydoc1;
    throw &mydoc1; //违背1
  }
  catch(Document *d)//违背2
  {
    cout<<d->get_docid()<<endl;
  }
  return(0);
}
//遵循示例：
#include <iostream>
using namespace std;
class Document
{
public:
  Document(void):docid(0)
  {
  }
  int get_docid(void);
private:
  int docid;
};
int Document::get_docid(void)
{
  return docid;
}
int main(void)
{
  try
  {
    Document mydoc1;
    throw mydoc1; //遵循1
  }
  catch(Document &d)//遵循2
  {
    cout<<d.get_docid()<<endl;
  }
  return(0);
}
// 6.8.1.4准则R-2-8-4
// 缺省捕获必须放在所有指定捕获之后。
// 违背示例:
#include<iostream>
using namespace std;
class Document
{
public:
  Document(void):docid(0)
  {
  }
  int get_docid(void);
private:
  int docid;
};
int Document::get_docid(void)
{
  return docid;
}
int main(void)
{
  try
  {
    Document mydoc1;
    throw mydoc1;
  }
  catch(...) //违背1
  {
    cout<<"缺省捕获"<<endl;
  }
  catch(Document &d)
  {
    cout<<d.get_docid()<<endl;
  }
  return(0);
}//有问题？ document
//遵循示例:
#include<iostream>
using namespace std;
class Document
{
public:
  Document(void):docid(0)
  {
  }
  int get_docid(void);
private:
  int docid;
};

int Document::get_docid(void)
{
  return docid;
}
int main(void)
{
  try
  {
    Document mydoc1;
    throw mydoc1;
  }
  catch(Document &d)
  {
    cout<<d.get_docid()<<endl;
  }
  catch(...) //遵循1
  {
    cout<<"缺省捕获"<<endl;
  }
  return(0);
}
// 6.8.1.5准则R-2-8-5
// 禁止显式直接抛出NULL。
// 违背示例:
#include<iostream>
using namespace std;
int main(void)
{
  try
  {
    throw NULL; //违背1
  }
  catch(int)
  {
    //...
  }
  catch(const char *)
  {
    //...
  }
  return(0);
}
// 遵循示例：
#include<iostream>
using namespace std;
int main(void)
{
  char *p=NULL;
  try
  {
    throw (p);
    throw (static_cast<const char *>(NULL)); //遵循1
  }
  catch(int)
  {
  //...
  }
  catch(const char *)
  {
  //...
  }
  return(0);
}

// 6.8.2建议准则
// 6.8.2.1准则A-2-8-1
// 建议在所有指定捕获之后使用缺省捕获防范遗漏的异常。、
// 6.8.2.2准则A-2-8-2
// 谨慎对指针类型进行抛出捕获。
// 说明性示例:
#include<iostream>
using namespace std;
int main(void)
{
  int i;
  try
  {
    int a1=1;
    int *a2=new int(2);
    if(i>10)
    {
      throw &a1;
    }
    else
    {
      throw a2;
    }
  }
  catch(int *b)
  {
    cout<<b<<endl;
    //提示1:是否需要释放动态分配的内存?
  }
  return(0);
}

// 6.9 其他条款
// 6.9.1 强制准则R-2-9-1
// 模板的声明、定义与实现必须在同一个文件之中。
// 6.9.2 建议准则
// 6.9.2.1 准则A-2-9-1
// 建议模板参数列表中的类型参数使用typename关键字说明。
// 6.9.2.2 准则A-2-9-2
// 建议除常数指针外，const说明均在类型说明的最外层。
// const int *p1=&a;//推荐，指针常数
// int const *p2=&a; //不推荐，指针常数
//  int *const p3=&a;//只能如此，常数指针
//   int const b=1; //不推荐
// const int c=2; //推荐
// 6.9.2.3准则A-2-9-3
// 建议不要对“&&”、“Ⅱ”、“,”进行操作符重载。
// 6.9.2.4准则A-2-9-4
// 建议不使用以.h为后缀的头文件。
// 说明性示例:
#include<iostream.h> //提示1
int main(void)
{
  cout<<"Hello!"<<endl;
  return(0);
}


// //遵循示例:
#include<iostream> //遵循1
using namespace std;
int main(void)
{
  cout<<"Hello!"<<endl;
  return(0);
}
