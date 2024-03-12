// RUN: %check_clang_tidy %s gjb8114cpp-default-constructor-must-exist %t

#include <iostream>
using namespace std;
class Foo
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 类中必须明确定义缺省构造函数。 [gjb8114cpp-default-constructor-must-exist]
{
public:
    ~Foo(void);
    void SetVar(int);
private:
    int a;
};
Foo::~Foo(void){}
void Foo::SetVar(int var)
{
    a = var;
}
int main(void)
{
    Foo thef;
    thef.SetVar(2);
    return (0);
}

class Foo2
{
public:
    Foo2(void); //遵循1
    ~Foo2(void);
    void SetVar(int);
private:
    int a;
};
Foo2::Foo2(void)
{
    a = 0;
}
Foo2::~Foo2(void){}
void Foo2::SetVar(int var)
{
    a = var;
}
int main1(void)
{
    Foo2 thef;
    thef.SetVar(2);
    return (0);
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