// RUN: %check_clang_tidy %s misrac2012-declaration-of-array-parameter-shall-not-contain-static-keyword %t
#include <stdio.h>
#include <stdlib.h>
// FIXME: Add something that triggers the check here.
// static uint16_t total ( uint16_t n,
//                         const uint16_t a [ static 20 ] /* Non-compliant */ )
// {
//     return 1;
// }
// static uint16_t total1 ( uint16_t n,
//                         uint16_t a [5 ] /* Non-compliant */ )
// {
//     return 1;
// }
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'f' is insufficiently awesome [misrac2012-declaration-of-array-parameter-shall-not-contain-static-keyword]

// FIXME: Verify the applied fix.
//   * Make the CHECK patterns specific enough and try to make verified lines
//     unique to avoid incorrect matches.
//   * Use {{}} for regular expressions.
// CHECK-FIXES: {{^}}void awesome_f();{{$}}

// FIXME: Add something that doesn't trigger the check here.
void awesome_f2();
