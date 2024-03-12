// RUN: %check_clang_tidy %s gjb8114-no-use-return-value-without-void %t

//仅支持大括号{}、if、else、while、case和default下返回值不被使用的CallExpr(CallExpr外部可包裹多层小括号)
//CSR-206	具有返回值的函数,其返回值如果不被使用,调用时应有void说明				R-1-7-11	gjb8114-no-use-return-value-without-void
//CSR-206 GJB 8114-2013 R-1-7-11
//违背示例
int func(int para)
{
    int stat;
    if(para>=0)
    {
        //...
        stat=1;
    }
    else
    {
        //...
        stat=-1;
    }
    return (stat);
}
int x = 0;
void fun1(int para){
    para++;
}
int main(void)
{
    int data=1;
    int local=0;
    //...
    (func(local));  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有void说明 [gjb8114-no-use-return-value-without-void]
    (((func(local))));  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有void说明 [gjb8114-no-use-return-value-without-void]
    if(func(local))  //遵循1
    {
        func(local);  //违背3
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有void说明 [gjb8114-no-use-return-value-without-void]
    }
    if(data==0)
      (func(local)); //违背4
      // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有void说明 [gjb8114-no-use-return-value-without-void]
    else
      (func(local)); //违背5
      // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有void说明 [gjb8114-no-use-return-value-without-void]
    if(data==0)
      func(local);  //违背6
      // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有void说明 [gjb8114-no-use-return-value-without-void]
    else
      func(local);  //违背7
      // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有void说明 [gjb8114-no-use-return-value-without-void]
    while (data<10)
    {
        (func(local));  //违背8
        // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有void说明 [gjb8114-no-use-return-value-without-void]
        func(local);  //违背9
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有void说明 [gjb8114-no-use-return-value-without-void]
        data++;
        x = func(local);
    }
    while(data++<10)
      func(local);  //违背10
      // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有void说明 [gjb8114-no-use-return-value-without-void]
    while(data++<10)
      (func(local));  //违背11
      // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有void说明 [gjb8114-no-use-return-value-without-void]
    int j=1;
    switch (j)
    {
    case 1:
        func(local);  //违背12
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有void说明 [gjb8114-no-use-return-value-without-void]
        break;
    case 2:
        func(local);  //违背13
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有void说明 [gjb8114-no-use-return-value-without-void]
    default:
        func(local);  //违背14
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有void说明 [gjb8114-no-use-return-value-without-void]
        x = func(local);  
        (((func(local))));  //违背15
        // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有void说明 [gjb8114-no-use-return-value-without-void]
        break;
    case 3:
        ((func(local)));  //违背16
        // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有void说明 [gjb8114-no-use-return-value-without-void]
    }
    int sign=func(local);  //遵循2
    //...
    (void)func(local);  //遵循3
    fun1(sign);  //遵循4
    if(sign==1)
    {
        (void)func(sign);  //遵循5
        sign=func(local);  //遵循6
        sign=func(local)+func(sign);  //遵循7
        int number=func(local);  //遵循8
    }
    else
    {
      sign=func(local)+func(sign); 
    }
    return (0);
}