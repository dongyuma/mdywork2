// RUN: %check_clang_tidy %s gjb8114cpp-impure-virtual-override-pure-virtual %t





#include <iostream>
#include <string.h>
using namespace std;

class A
{
public:
	A(void);
	virtual ~A(void);
	virtual void foo(void) = 0;

};

A::A(void)
{
}

A::~A(void)
{
}

class B : public A
{
public:
	B(void);
	virtual ~B(void);
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

class C : public B
{
public:
	C(void);
	virtual ~C(void);
	virtual void foo(void) = 0; //违背1
	// CHECK-MESSAGES: :[[@LINE-1]]:2: 禁止非纯虚函数被纯虚拟函数重写 [gjb8114cpp-impure-virtual-override-pure-virtual]
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

/*
#include <iostream>
#include <string.h>
using namespace std;

class A
{
public:
	A(void);
	virtual ~A(void);
	virtual void foo(void) = 0;

};

A::A(void)
{
}

A::~A(void)
{
}

class B : public A
{
public:
	B(void);
	virtual ~B(void);
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

class C : public B
{
public:
	C(void);
	virtual ~C(void);
	virtual void foo(void) //遵循1

};

C::C(void)
{
}

C::~C(void)
{
}

void C::foo(void)
{
}


int main(void)
{
	B myb;
	myb.foo();
	return(0);
}
*/