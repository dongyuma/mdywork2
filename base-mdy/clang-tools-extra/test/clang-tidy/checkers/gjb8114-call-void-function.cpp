// RUN: %check_clang_tidy %s gjb8114-call-void-function %t

//CSR-207	无返回值函数,调用时禁止再用（void）重复说明				R-1-7-12	gjb8114-call-void-function		
//CSR-207 GJB 8114-2013 R-1-7-12
//违背示例
void func(int para)
{
    int i=para+1;
    //...
}
double fund(double para)
{
    para=para+1.0;
    return para;
}
int main(void)
{
    int local=0;
    double d=0.25;
    (void)func(local);            //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 无返回值函数，调用时禁止再用（void）重复说明 [gjb8114-call-void-function]
    func(local);                  //遵循1
    fund(d);                      //遵循2
    (double)fund(d);              //遵循3
    int number=(double)d;         //遵循4
    (void)fund(d);                //遵循5
    (     void)func(local);       //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 无返回值函数，调用时禁止再用（void）重复说明 [gjb8114-call-void-function]
    (      void   )  func(local); //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 无返回值函数，调用时禁止再用（void）重复说明 [gjb8114-call-void-function]
    return (0);
}
/*
//遵循示例
void func(int para)
{
    int i=para+1;
    //...
}
int main(void)
{
    int local=0;
    func(local);        //遵循1
    return (0);
}
*/
