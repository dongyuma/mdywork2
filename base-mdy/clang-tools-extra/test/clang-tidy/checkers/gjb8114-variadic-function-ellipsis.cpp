// RUN: %check_clang_tidy %s gjb8114-variadic-function-ellipsis %t

// GJB-8114 R-1-7-15
// 禁止在函数参数中使用省略号

// 违背示例1
int fun_v1(int datax, ...);   //违背
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止在函数参数表中使用省略号 [gjb8114-variadic-function-ellipsis]
int main_v1(int argc, char* argv[])
{
	int ix, iy, iz;
	ix = 1;
	iy = 2;
	iz = fun_v1(ix, iy);
	return 0;
}

int fun_v1(int datax, ...) 
{
	int temp;
	temp = 2 * datax;
	return temp;
}
// CHECK-MESSAGES: :[[@LINE-6]]:5: warning: 禁止在函数参数表中使用省略号 [gjb8114-variadic-function-ellipsis]

// 违背示例2
int fun_v2(int datax, ...)  //违背
{
	int temp;
	temp = 2 * datax;
	return temp;
}
// CHECK-MESSAGES: :[[@LINE-6]]:5: warning: 禁止在函数参数表中使用省略号 [gjb8114-variadic-function-ellipsis]
int main_v2(int argc, char* argv[])
{
	int ix, iy, iz;
	ix = 1;
	iy = 2;
	iz = fun_v2(ix, iy);
	return 0;
}

// 遵循示例1
int fun_v3(int datax, int datay);  //遵循

int main_v3(int argc, char* argv[])
{
	int ix, iy, iz;
	ix = 1;
	iy = 2;
	iz = fun_v3(ix, iy);
	return 0;
}

int fun_v3(int datax, int datay)
{
	int temp;
	temp = 2 * datax;
	return temp;
}