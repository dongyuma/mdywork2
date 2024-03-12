// RUN: %check_clang_tidy %s gjb8114cpp-single-parameter-constructor-set-explicit %t

//CSR-304	单参数构造函数必须使用explicit声明		R-2-2-3	 gjb8114cpp-single-parameter-constructor-set-explicit-check
//CSR-304	GJB 8114-2013 R-2-2-3
//违背示例:
class Foo
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 单参数构造函数必须使用explicit声明 [gjb8114cpp-single-parameter-constructor-set-explicit-check]
{
public:
    Foo(void);
    Foo(int var); //违反 
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 单参数构造函数必须使用explicit声明 [gjb8114cpp-single-parameter-constructor-set-explicit-check]
    int getVar(void);
    Foo(int var,int var2); 
    ~Foo(void);
private:
    int a;
};
Foo::Foo(void)
{
    a= 0;    
}
Foo::Foo(int var)
{
    a = var;
}
Foo::Foo(int var, int var2)
{
    a = var;
    a = var2;
}
int Foo::getVar(void)
{
    return a;
}
Foo::~Foo(void){};
void f(Foo);
int main()
{
    int i=1;
    f(i);
    return (0);

}
void f(Foo thef)
{
    int j;
    j = thef.getVar();
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