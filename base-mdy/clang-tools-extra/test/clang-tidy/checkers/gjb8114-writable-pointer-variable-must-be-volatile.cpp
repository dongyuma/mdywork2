#define REG 0x40000000
#define REG2 (unsigned int *)0x40000000
unsigned int *puidata = REG;
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 程序外部可以改变的变量,必须使用volatile类型说明 [gjb8114-writable-pointer-variable-must-be-volatile]
unsigned int *puidata2 = REG2;
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 程序外部可以改变的变量,必须使用volatile类型说明 [gjb8114-writable-pointer-variable-must-be-volatile]

int main()
{
	while(puidata)
	{
		int b =0;
	}
	while(puidata2)
	{
		int c =2;
	}
	return 0;
}