// RUN: %check_clang_tidy %s bz12-infinitel-loop %t

int main()
{
    int loop_variable;
    int number_1,number_2,result;
    while(loop_variable<10)  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 该循环为无限循环 [bz12-infinitel-loop]
    {
        result=number_1+number_2;
    }
    while(loop_variable<10)  //遵循1
    {
        result=number_1+number_2;
        break;
    }
    while(loop_variable<10)  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 该循环为无限循环 [bz12-infinitel-loop]
    {
        switch(number_1)
        {
            case 1:
              result=1;
              break;
            case 2:
              result=2;
              break;
        }
    }
    do{  //遵循2
        result=number_1+number_2;
        break;
    }while(loop_variable<10);
    while(loop_variable<10)  //遵循3
    {
        break;
        switch(number_1)
        {
            case 1:
              result=1;
              break;
            case 2:
              result=2;
              break;
        }
    }
    while(loop_variable<10)  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 该循环为无限循环 [bz12-infinitel-loop]
    {
        for(int i=0;i<5;i++)
        {
            break;
            switch(number_1)
            {
                case 1:
                  result=1;
                  break;
                case 2:
                  result=2;
                  break;
            }
        }
    }
    for(int i=0;i<10;i++)  //遵循4
    {
        result=number_1+number_2;
    }
    do{  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 该循环为无限循环 [bz12-infinitel-loop]
        result=number_1+number_2;
    }while(loop_variable<10);
    do{  //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 该循环为无限循环 [bz12-infinitel-loop]
        result=number_1+number_2;
        switch(number_1)
        {
            case 1:
              result=1;
              break;
            case 2:
              result=2;
              break;
        }
    }while(loop_variable<10);
}