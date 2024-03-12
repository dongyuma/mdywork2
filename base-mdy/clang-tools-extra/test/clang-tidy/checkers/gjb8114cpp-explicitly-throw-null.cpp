// RUN: %check_clang_tidy %s gjb8114cpp-explicitly-throw-null %t

#include <iostream>

using namespace std;

int main(void)
{
    try
    {
        throw NULL;//违背
        // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 禁止显式直接抛出NULL [gjb8114cpp-explicitly-throw-null]
        throw (NULL);//违背
        // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 禁止显式直接抛出NULL [gjb8114cpp-explicitly-throw-null]
        throw ((((NULL))));//违背
        // CHECK-MESSAGES: :[[@LINE-1]]:19: warning: 禁止显式直接抛出NULL [gjb8114cpp-explicitly-throw-null]
    }
    catch(int)
    {
        //doNothing
    }
    
    char *p = NULL;
    try
    {
        throw (p);
        throw (static_cast<const char *>(NULL));
    }
    catch(int)
    {
        //doNothing
    }
    
    return 0;
}
