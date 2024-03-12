// RUN: %check_clang_tidy %s gjb8114cpp-each-specific-throw-has-catch %t

// FIXME: Add something that triggers the check here.
#include <iostream>
using namespace std;
class A1
{
public:
	A1(void) {}

};

class A2
{
public:
	A2(void) {}

};
int main(void)
{
	int i = 0;
	//...
	try
	{
		if (i > 0)
		{
			throw A1();
		}
		else
		{
			throw A2();  //违背1
			// CHECK-MESSAGES: :[[@LINE-1]]:4: warning: 每个指定的抛出必须有与之匹配的捕获 [gjb8114cpp-each-specific-throw-has-catch]
		}
	}
	catch (A1&)
	{
		cout << "exception : A1" << endl;
	}
	return (0);
}
/*
#include <iostream>
using namespace std;
class A1
{
public:
	A1(void) {}

};

class A2
{
public:
	A2(void) {}

};
int main(void)
{
	int i = 0;
	//...
	try
	{
		if (i > 0)
		{
			throw A1(); //遵循1
		}
		else
		{
			throw A2();  //遵循2
		}
	}
	catch (A1&)
	{
		cout << "exception : A1" << endl;
	}
	catch (A2&)
	{
		cout << "exception : A2" << endl;
	}
	return (0);
}
*/