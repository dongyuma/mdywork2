// RUN: %check_clang_tidy %s gjb8114cpp-dynamic-assign-member-has-copy-constructor-and-overload %t

#include <iostream>
#include <string.h>
using namespace std;

class A
// CHECK-MESSAGES: :[[@LINE-7]]:1: warning: 含有动态分配成员的类，必须编写拷贝构造函数，并重载赋值操作符,此错误为没有拷贝构造函数 [gjb8114cpp-dynamic-assign-member-has-copy-constructor-and-overload]
{
public:
	A(char* cstr);
	//A(const A& ca);
	A(void);
	~A(void);
	void operator=(const A& ca);

private:
	char* str;

};
A::A(char* cstr)
{
	str = new char[20];
	strncpy(str, cstr, 20);
}
/*
A::A(const A& ca) 
{
	str = new char[20];
	strncpy(str, ca.str, 20);
}
*/ 
// 违背1

void A::operator=(const A& ca) 
{
	if (str == NULL)
	{
		str = new char[20];
	}

	strncpy(str, ca.str, 20);
}
A::A(void) :str(new char[20])
{
	strncpy(str, "Welcome!", 20);
}

A::~A(void)
{
	delete str;
	str == NULL;
}


class B
	// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 含有动态分配成员的类，必须编写拷贝构造函数，并重载赋值操作符,此错误为没有拷贝构造函数 [gjb8114cpp-dynamic-assign-member-has-copy-constructor-and-overload]
{
public:
	B(char* cstr);
	B(const B& ca);
	B(void);
	~B(void);
	//void operator=(const B& ca);

private:
	char* str;

};
B::B(char* cstr)
{
	str = new char[20];
	strncpy(str, cstr, 20);
}

B::B(const B& ca)
{
	str = new char[20];
	strncpy(str, ca.str, 20);
}
/*
void B::operator=(const B& ca)
{
	if (str == NULL)
	{
		str = new char[20];
	}

	strncpy(str, ca.str, 20);
}*/
// 违背2

B::B(void) :str(new char[20])
{
	strncpy(str, "Welcome!", 20);
}

B::~B(void)
{
	delete str;
	str == NULL;
}



class C
{
public:
	C(char* cstr);
	C(const C& ca);
	C(void);
	~C(void);
	void operator=(const C& ca);

private:
	char* str;

};
C::C(char* cstr)
{
	str = new char[20];
	strncpy(str, cstr, 20);
}

C::C(const C& ca) //遵循1
{
	str = new char[20];
	strncpy(str, ca.str, 20);
}

void C::operator=(const C& ca) //遵循2
{
	if (str == NULL)
	{
		str = new char[20];
	}

	strncpy(str, ca.str, 20);
}

C::C(void) :str(new char[20])
{
	strncpy(str, "Welcome!", 20);
}

C::~C(void)
{
	delete str;
	str == NULL;
}


typedef struct RJZ_demo  //遵循
{
	float x;
	int y;
}ST_DEMO;

ST_DEMO st_test;

struct DINFO{
	int wCMD;
	int unINFO1;
	int unINFO2;
	int unINFO3;
};