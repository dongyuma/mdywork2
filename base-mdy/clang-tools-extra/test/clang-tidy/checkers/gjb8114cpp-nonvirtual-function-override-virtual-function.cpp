// RUN: %check_clang_tidy %s gjb8114cpp-nonvirtual-function-override-virtual-function %t

//CSR-312	派生类对基类虚拟函数重写的声明必须使用virtual显示说明		R-2-4-2	 gjb8114cpp-nonvirtual-function-override-virtual-function
//CSR-312  GJB 8114-2013 R-2-4-2
//违背示例:

#include <iostream>
#include <string.h>
using namespace std;

class Base
{
public:

	Base(void);
	 ~Base(void);
	virtual int g(int a = 0);
};

Base::Base(void)
{	
}

Base::~Base(void)
{
}

int Base::g(int a )
{
	return (a + 1);
}

class Derived:Base
{
public:

	Derived(void);
	 ~Derived(void);
	int g(int a = 0); //违背1
	 // CHECK-MESSAGES: :[[@LINE-1]]:2: warning: 派生类对基类虚拟函数重写的声明必须使用virtual显示说明 [gjb8114cpp-nonvirtual-function-override-virtual-function]
};

Derived::Derived(void)
{
}

Derived::~Derived(void)
{
}

int Derived::g(int a)
{
	return (a + 1);
}

//遵循示例:
/*
#include <iostream>
#include <string.h>
using namespace std;

class Base
{
public:

	Base(void);
	~Base(void);
	virtual int g(int a = 0);
};

Base::Base(void)
{
}

Base::~Base(void)
{
}

int Base::g(int a)
{
	return (a + 1);
}

class Derived :Base
{
public:

	Derived(void);
	~Derived(void);
	virtual int g(int a = 0); //遵循1
};

Derived::Derived(void)
{
}

Derived::~Derived(void)
{
}

int Derived::g(int a)
{
	return (a + 1);
}
*/