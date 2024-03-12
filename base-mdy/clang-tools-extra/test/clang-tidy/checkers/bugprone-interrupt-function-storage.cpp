// RUN: %check_clang_tidy %s bugprone-interrupt-function-storage %t

// #include "decl.h"
// #include "CY_Global.h"
// #include "CY_Math.h"
// #include "typedef.h"


interrupt void	ISR_NULL(void) 
{

}

interrupt void ISR_DPRM(void)
{   	

}

interrupt void ISR_SecondUart(void)
{

}

void DoNothing()
{
	
}

int func1()
{
	
}

interrupt void ISR_1553RT(void)
{

}

interrupt void ISR_5ms(void)
{

}




// double a_1[3];
// double a_2[3];
// double b_1[3];
// double b_2[3];
// void func_1() //
// {
// 	unsigned char i;
// 	for (i=0;i<3;i++)
// 	{
// 		b_1[i] = a_1[i]/0.02;
// 		b_2[i] = b_2[i]/0.02;
// 	}
// 	for (i=0;i<5;i++){
// 		if(i > 3) {
// 			int x = x / (i -4);
// 		}
// 	}
// }


// #define _NUM (3)
// const unsigned int lst[_NUM] = {1,2,3};
// void func_2_1(unsigned char c) 
// {
// 	unsigned short us;
// 	unsigned int ui;
// 	if (c > _NUM + 1)
// 	{
// 		return;
// 	}
// 	ui = lst[c-1];  //buffer overflow
// }


// unsigned short us_96[96]={0};
// unsigned short us_5[5]={0};
// void func_2_2()    
// {
// 	memset(us_96,0xFFFF,96); //memset
// 	memset(&(us_5[2]),0x00,3);
// }


// void func_2_3(void) 
// {
// 	double a[9];
// 	double b[3];
// 	double cc[3]={0.0,0.0,0.0};

// 	b[0]=1.0;
// 	b[1]=2.0;
// 	b[2]=3.0;

// 	a[0]=4.0;
// 	a[1]=5.0;
// 	a[2]=6.0;
// 	a[3]=7.0;
// 	a[4]=8.0;
// 	a[5]=9.0;
// 	a[6]=10.0;
// 	a[7]=11.0;
// 	a[8]=12.0;

// 	JZXC(cc,a,b,3,3,3);
// }


// void func_3(char c) 
// {
// 	/*float dividend = 1000.0;
// 	float ret;
// 	ret = dividend / 0.0;*/

// 	int dividend = 1000;
// 	int ret;
// 	ret = dividend / c;  //Division by zero
// }

// void func_4() 
// {
// 	int ret;
// 	int a = 0;
// 	int flag = 1;
// 	while (flag)  //The condition is true and can't jump out
// 	{
// 		a ++; 
// 	}
// 	ret = a;
// }

// void func_5()
// {
// 	float f_1 = 1;
// 	float f_2 = 1.0;
// 	if (f_1 == f_2) //
// 	{
// 		f_2 = 1;
// 	}
// }

// void func_6() 
// {
// 	int aa = 0;
// 	int bb = 1;
// 	if (aa = bb) //==
// 	{
// 		aa = 1;
// 	}
// }

// void func_7()
// {
// 	 int si_7 = -1;
// 	 unsigned int ui_7 = 1;
// 	 if (si_7 < ui_7) //Loss of Sign in implicit conversion
// 	 {
// 		 si_7 = 2;
// 	 }
// }

// void func_8() 
// {
// 	int a = 1;
// 	int ret;
// 	ret = a << -1;  //��λ���Ǹ���

// }

// #define Min(x,y) (x<y?x:y)
// #define Max(x,y) (x>y?x:y)
// void func_9() 
// {
// 	int a_9 = 2;
// 	int b_9 = 3;
// 	int c_9 = 4;
// 	c_9 = Min(a_9+b_9,c_9);  //
// }

// void func_10()  
// {
// 	double rr = 0;
// 	unsigned int ui_10[10];
// 	unsigned int uiTemp = 5;
// 	ui_10[0] = 3;
// 	ui_10[1] = 1;
// 	ui_10[2] = 6;
// 	rr = (uiTemp-ui_10[0])/(ui_10[2]-ui_10[1]);
// }

// void func_10_1() 
// {
// 	unsigned int ui_50[50];
// 	signed short ss_30[30];
// 	int i;
// 	ss_30[0]=-2;
// 	ss_30[1]=4;
// 	for (i=0;i<5;i++)
// 	{
// 		ui_50[28+i]=ss_30[2*i]|(ss_30[2*i+1]<<16);
// 	}
// }

// void func_10_2() 
// {
// 	static int si = -6;
// 	static unsigned int index = 3;
// 	si /= index;
// }

// void func_10_3()
// {
// 	unsigned int time1;
// 	unsigned int time2;
// 	time1 = 100;
// 	time2 = 200;
// 	if ((time1 - time2) > 5000)
// 	{
// 		time1 = 0;
// 	}
// }



// void func_11()  
// {

// }


// extern int *extern_001_glb_buf;
// extern int *extern_001_glb_float;
// extern int extern_001_glb_var3;
// extern float extern_001_glb_var4;
// extern char extern_001_glb_var5;
// void func_12() 
// {

// }

// #define FZ1 1
// #define FZ2 2
// #define FZ3 3
// void func_13(unsigned short msg) 
// {
// 	switch(msg)
// 	{
// 	case FZ1:
// 		{
// 			func_1();
// 			break;
// 		}
// 	case FZ2:
// 		{
// 			func_1();
// 		}
// 	case FZ3:
// 		{
// 			func_2_2();
// 			break;
// 		}
// 	default:
// 		{
// 			break;
// 		}
// 	}
// }

// #define YES 0x1111
// #define NO 0x2222
// unsigned int ui_1 = NO;
// signed int si_1 = YES;
// signed int si_2 = NO;
// void func_14() 
// {
// 	if ((NO == si_2)&&(ui_1 == YES)&&(YES == si_1)) //
// 	{
// 		ui_1 = NO;
// 	}
// }

// unsigned int testnum_x = 0;
// void func_15()
// {
// 	unsigned int i;
// 	float f_temp = 0.0;
// 	for (i = 0; i < 1; i++)
// 	{
// 		testnum_x++;
// 	}

// 	if (testnum_x == 0)
// 	{
		
// 	}
// 	else 
// 	{
// 		f_temp = f_temp / (testnum_x - 1);
// 	}
// }



// void main(void)
// {
// 	func_1();
// 	func_2_1(3);
// 	func_2_2();
// 	func_2_3();
// 	//func_3(1);
// 	func_4();
// 	func_5();
// 	func_6();
// 	func_7();
// 	func_8();
// 	func_9();
// 	func_10();
// 	func_10_1();
// 	func_10_2();
// 	func_10_3();
// 	func_11();
// 	func_12();
// 	func_13(FZ1);
// 	func_13(FZ2);
// 	func_14();
// 	func_15();

// }
