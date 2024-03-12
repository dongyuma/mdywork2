// RUN: %check_clang_tidy %s gjb8114-shift-signed-number %t

///CSR-62	GJB 8114-2013 A-1-6-1 谨慎对有符号整型量进行位运算

void shiftFunction() {
    int x = 100;
    int y;
    y = x << 4;//违背1
	// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 谨慎对有符号整型量进行位运算 [gjb8114-shift-signed-number]
    y = x <<= 4; //违背2
	// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 谨慎对有符号整型量进行位运算 [gjb8114-shift-signed-number]
    y = x >> 4; //违背3
	// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 谨慎对有符号整型量进行位运算 [gjb8114-shift-signed-number]
    y = x >>= 4; //违背4
	// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 谨慎对有符号整型量进行位运算 [gjb8114-shift-signed-number]
    unsigned m = 100;
    unsigned n;
    n = m << 10;
    n = m <<= 10;
    n = m >> 10;
    n = m >>= 10;

    long a = 100;
    long b;
    b = a << 10; //违背5
	// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 谨慎对有符号整型量进行位运算 [gjb8114-shift-signed-number]
    b = a <<= 10; //违背6
	// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 谨慎对有符号整型量进行位运算 [gjb8114-shift-signed-number]
    b = a >> 10; //违背7
	// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 谨慎对有符号整型量进行位运算 [gjb8114-shift-signed-number]
    b = a >>= 10; //违背8
	// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 谨慎对有符号整型量进行位运算 [gjb8114-shift-signed-number]
}


void bitFunction() {
    int x = 100;
    int y =5;
    int z = 0;
    if (x | y)//违背9
	// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 谨慎对有符号整型量进行位运算 [gjb8114-shift-signed-number]
    {
        z = 3;
    }
    if (x & y)//违背10
	// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 谨慎对有符号整型量进行位运算 [gjb8114-shift-signed-number]
    {
        z = 3;
    }
    if (x ^ y)//违背11
	// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 谨慎对有符号整型量进行位运算 [gjb8114-shift-signed-number]
    {
        z = 3;
    }
}
