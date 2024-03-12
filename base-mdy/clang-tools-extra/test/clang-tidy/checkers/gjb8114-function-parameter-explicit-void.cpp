// RUN: %check_clang_tidy %s gjb8114-function-parameter-explicit-void %t

// CSR-33 GJB R-1-1-23
// 函数参数表为空时，必须使用void明确说明

int fun_GJB8114_2013_1_1_23(); //违背1 
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 函数参数为空时，必须使用void明确说明 [gjb8114-function-parameter-explicit-void]
int datax =0;
int datay = 0; 
int main_GJB8114_2013_1_1_23()//违背2
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 函数参数为空时，必须使用void明确说明 [gjb8114-function-parameter-explicit-void]
{
        int iz;
        datax=1; 
        datay=2; 
        iz=fun_GJB8114_2013_1_1_23();
        return(0);
}


int fun_GJB8114_2013_1_1_23() //违背3 
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 函数参数为空时，必须使用void明确说明 [gjb8114-function-parameter-explicit-void]
{
        int temp;
        temp=2*datax+ 3*datay;
        return temp;
}

union{int num11;char num12;}union_R118;
void static_1122(int,int);
void foo(void);
void static_111(void);
void static_1314(void);
void static_1513(int,int g_num);
void static_1514(int,int myint);
void static_1112(void);
void a_GJB8114_2013_1_1_23();//违背4 
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: 函数参数为空时，必须使用void明确说明 [gjb8114-function-parameter-explicit-void]
void R1113(int);
void R1113(int a);
void a (int* p);
void array_outof_index_GJB8114_2013_1_1_23();//违背5
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: 函数参数为空时，必须使用void明确说明 [gjb8114-function-parameter-explicit-void]

int fun_GJB8114_2013_1_1_23_f(void); //遵循1 
int datax_v2 =0;
int datay_v2 = 0; 
int main_GJB8114_2013_1_1_23_f(void)//遵循2
{
        int iz;
        datax_v2=1; 
        datay_v2=2; 
        iz=fun_GJB8114_2013_1_1_23_f();
        return(0);
}

int fun_GJB8114_2013_1_1_23_f(void) //遵循3 
{
        int temp;
        temp=2*datax_v2+ 3*datay_v2;
        return temp;
}

union{int num11;char num12;}union_R118_v2;
void static_1122(int,int);
void foo(void);
void static_111(void);
void static_1314(void);
void static_1513(int,int g_num);
void static_1514(int,int myint);
void static_1112(void);
void a_GJB8114_2013_1_1_23_f(void);//遵循4 
void R1113(int);
void R1113(int a);
void a (int* p);
void array_outof_index_GJB8114_2013_1_1_23_f(void);//遵循5 

//CSR-33	函数参数为空时,必须使用void明确说明				R-1-1-23	gb8114-Functionvoid		
//CSR-33 GJB 8114-2013 R-1-1-23
//违背示例
int fun();  //违背6
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 函数参数为空时，必须使用void明确说明 [gjb8114-function-parameter-explicit-void]
int datax_v3=0;
int datay_v3=0;
int main(void)
{
    int iz;
    datax_v3=1;
    datay_v3=2;
    iz=fun();
    return (0);
}
int fun()  //违背7
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 函数参数为空时，必须使用void明确说明 [gjb8114-function-parameter-explicit-void]
{
    int temp;
    temp=2*datax_v3+3*datay_v3;
    return temp;
}
/*
//遵循示例
int fun(void);  //遵循1
int datax=0;
int datay=0;
int main(void)
{
    int iz;
    datax=1;
    datay=2;
    iz=fun();
    return (0);
}
int fun(void)  //遵循2
{
    int temp;
    temp=2*datax+3*datay;
    return temp;
}
*/

