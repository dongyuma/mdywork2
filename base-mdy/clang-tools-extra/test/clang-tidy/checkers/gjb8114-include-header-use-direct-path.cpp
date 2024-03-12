// RUN: %check_clang_tidy %s gjb8114-include-header-use-direct-path %t

// CSR-20 GJB 8114 R-1-1-21
// 禁止在#include语句中使用绝对路径名

#include</usr/include/stdio.h>
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 禁止在#include语句中使用绝对路径 [gjb8114-include-header-use-direct-path]
#include<string.h>
#include "stdlib.h"
#include "/usr/include/math.h"
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 禁止在#include语句中使用绝对路径 [gjb8114-include-header-use-direct-path]
// #include <C:\string.h>
// #include <D:\string.h>
