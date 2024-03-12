// RUN: %check_clang_tidy %s gjb8114-unsigned-compare-with-signed %t

//CSR-228	禁止无符号数与有符号数之间的直接比较				R-1-12-5	gjb8114-unsigned-compare-with-signed		
//CSR-228 GJB 8114-2013 R-1-12-5
//违背示例
int TestCaseUsignedCompareWithSigned(void){
    unsigned int x;
    int y ,i; 
    x=2;
    y=-2;
    if(y<x) //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 禁止无符号数与有符号数之间的直接比较 [gjb8114-unsigned-compare-with-signed]
    {
        i=0;
    }
    else
    {
        i=1;
    }
    return(0);
}

int TestCaseUsignedCompareWithSigned2(void){
    unsigned int x;
    int y ,i; 
    x=2;
    y=-2;
    if(y<x) //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 禁止无符号数与有符号数之间的直接比较 [gjb8114-unsigned-compare-with-signed]
    {
        i=0;
    }
    else if((x)>(y)) //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 禁止无符号数与有符号数之间的直接比较 [gjb8114-unsigned-compare-with-signed]
    {
        i=1;
    }
    else if(x == (y)) //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 禁止无符号数与有符号数之间的直接比较 [gjb8114-unsigned-compare-with-signed]
    {
        i=1;
    }
    else if(x >= (y)) //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 禁止无符号数与有符号数之间的直接比较 [gjb8114-unsigned-compare-with-signed]
    {
        i=1;
    }
    else if(y <= (x)) //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 禁止无符号数与有符号数之间的直接比较 [gjb8114-unsigned-compare-with-signed]
    {
        i=1;
    }
    else if(y != (x)) //违背7
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 禁止无符号数与有符号数之间的直接比较 [gjb8114-unsigned-compare-with-signed]
    {
        i=1;
    }
    else if(x !=0) //违背8
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 禁止无符号数与有符号数之间的直接比较 [gjb8114-unsigned-compare-with-signed]
    {
        i=1;
    }
    else if(y !=0) 
    {
        i=1;
    }
    
    return(0);
}

// 遵循示例
int main(void){
    unsigned int x;
    int y ,i; 
    x=2;
    y=-2;
    if(y < (int)x) //遵循1
    {
        i=0;
    }
    else
    {
        i=1;
    }
    return(0);
}
