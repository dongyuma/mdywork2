// RUN: %check_clang_tidy %s gjb8114-unreachable-code %t

//CSR-105	禁止不可达语句		R-1-8-1	 gjb8114-unreachable-code
//CSR-105  GJB 8114-2013 R-1-8-1
//违背示例:
int fun(void);
int main(void)
{
    if(0)
    {
        int m=1;  //违背1
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止不可达语句 [gjb8114-unreachable-code]
    }
    int local=0;
    int para=0;
    switch (para)
    {
        local=para;  //违背2
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止不可达语句 [gjb8114-unreachable-code]
        fun();  //违背3
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止不可达语句 [gjb8114-unreachable-code]
        case 1:
          para++;
          break;
        default:
          local++;
          break;
        local=para;  //违背4
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止不可达语句 [gjb8114-unreachable-code]
    }
    return local;
    para++;  //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止不可达语句 [gjb8114-unreachable-code]
    switch(para)  //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止不可达语句 [gjb8114-unreachable-code]
    {
      case 888:
        local++;  //违背7
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止不可达语句 [gjb8114-unreachable-code]
        break;
      case 999:
        local--;  //违背8
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止不可达语句 [gjb8114-unreachable-code]
        break;
    }
}
int fun(void)
{
    if(!1)
    {
      int data=0;  //违背9
      // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 禁止不可达语句 [gjb8114-unreachable-code]
      data++;  //违背10
      // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 禁止不可达语句 [gjb8114-unreachable-code]
    }
    return 1;
}