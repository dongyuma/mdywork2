// RUN: %check_clang_tidy %s gjb8114-anti-null-switch %t

//CSR-164	禁止使用空switch语句		4.3.1.5	ConditionalBranchChecker	R-1-4-3	gjb8114-anti-null-switch
//CSR-164 GJB 8114-2013 R-1-4-3
//违背示例
int main(void)
{
    int k=0;
    int i=0;
    switch(i)  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止使用空switch [gjb8114-anti-null-switch]
    {
    }
    switch(i);  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止使用空switch [gjb8114-anti-null-switch]
    switch(i)  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止使用空switch [gjb8114-anti-null-switch]
    switch(i)  //遵循1
    {
        case 0:
          k=1;
          break;
        case 1:
          k=2;
          break;
    }
    switch(i)  //遵循2
    {
        case 0:
          k=1;
          break;
        case 1:
          k=2;
          break;
        default:
          k=3;
          break;
    }
    return (0);
}
