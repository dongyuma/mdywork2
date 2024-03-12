// RUN: %check_clang_tidy %s scope-IntegerDivLossAccuracy %t

// 将整型相除的结果赋值给浮点变量
// AS-19  scope-IntegerDivLossAccuracy

// 遵循示例
void func_3(int a) 
{
	int dividend = 1000;
	int ret;
	ret = dividend / a;
}

// 违背示例
void func_10()  
{
	double rr = 0;
	unsigned int ui_10[10];
	unsigned int uiTemp = 5;
	ui_10[0] = 3;
	ui_10[1] = 1;
	ui_10[2] = 6;
	rr = (uiTemp-ui_10[0])/(ui_10[2]-ui_10[1]);
    // CHECK-MESSAGES: :[[@LINE-1]]:24: warning: 使用整数除法而意外损失了算术运算精度 [scope-IntegerDivLossAccuracy]
}
