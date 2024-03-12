// RUN: %check_clang_tidy %s misrac2012-line-comments-used %t
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void R_3_1(void)
{
	/* some comment, end comment marker accidentally omitted
	<<New Page>>
	Perform_Critical_Safety_Function(X);
	/* this comment is non-compliant */

	int16_t x;
	int16_t y = 3;
	int16_t z = 5;

	/* Following is Non-compliant */
	// CHECK-MESSAGES: :[[@LINE+1]]:11: warning: 同一个注释不应该同时使用//和/*或*/ [misrac2012-line-comments-used]
	x = y // /* 违背1
		+ z
		// */  违背2
	// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 同一个注释不应该同时使用//和/*或*/ [misrac2012-line-comments-used]
		;
}

void R_3_2(void)
{
	bool b = true;
	int32_t x;
	int32_t y = 10;

	/* Non-compliant - logically equivalent to x = y + 1 */
	// CHECK-MESSAGES: :[[@LINE+1]]:10: warning: 不应该在行注释中使用行连接符'\' [misrac2012-line-comments-used]
	x = y + // 违背3 \
          + z
		+1;


	/* Compliant - intention is made explicit             */
	x = y + // 遵循1
			// + z
		+1;


	// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: 不应该在行注释中使用行连接符'\' [misrac2012-line-comments-used]
	x = 0; // 违背4 \
    if ( b )
	{
	}
}