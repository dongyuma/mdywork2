// RUN: %check_clang_tidy %s gjb8114-only-default-in-switch %t

//CSR-165	禁止switch语句中只包含default语句		4.3.1.6	ConditionalBranchChecker	R-1-4-5	gjb8114-only-default-in-switch		
//CSR-165 GJB 8114-2013 R-1-4-5
//违背示例
int main(void)
{
    int i=0;
    int k=0;
    switch(i)
    {
    default:  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: switch语句中只包含default语句 [gjb8114-only-default-in-switch]
        break;
    }           
    switch(i)
    {
        case 1:
          k=1;
          break;
        case 2:
          k=2;
          break;
        default:  //遵循1
          k=4;
          break;
    }
    switch(i)
    {
        default:  //违背2
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: switch语句中只包含default语句 [gjb8114-only-default-in-switch]
          break;
    }
    switch(i)
    {
        case 1:
          k=1;
          break;
        default:  //遵循2
          break;
    }
    switch(i)
    {
      case 1:
        k=1;
        switch(i)
        {
          case 1:
            k=1;
            break;
          default:  //遵循3
            break;
        }
        break;
    }
    return (0);
}
