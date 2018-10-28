/*
 ============================================================================
 * \brief stack_unit_test.c
 * \author Exdev
 * \version 1.0
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>
#include "stack_t.h"



#define UNIT_TEST_COMPARE(p1, op, p2)	\
do \
{	\
	if (p1 op p2)	\
	{	\
		printf("[test passed]\n");	\
	}	\
	else	\
	{	\
		printf("[test failed] : " #p1 " not " #op " " #p2 "\n");	\
		stack_dump(&thestk, "error");	\
		return -1;	\
	}	\
}	\
while(0)

int stack_unit_test();

int main()
{
	stack_unit_test();
	return 0;
}

int stack_unit_test()
{
	struct stack thestk;
	data_t buf = 0;
	UNIT_TEST_COMPARE(stack_ctor(&thestk), ==, 0);
	UNIT_TEST_COMPARE(stack_check(&thestk), ==, 0);
	for (int i = 0; i < thestk.capacity - 2 * MARK; i++)
	{
		UNIT_TEST_COMPARE(stack_push(&thestk, i * 33), ==, 0);
		UNIT_TEST_COMPARE(thestk.data[i + MARK], ==, i * 33);
	}
	UNIT_TEST_COMPARE(stack_check(&thestk), ==, 0);
	stack_dump(&thestk, "check");
	UNIT_TEST_COMPARE(stack_pop(&thestk, &buf), ==, 0);
	UNIT_TEST_COMPARE(stack_clear(&thestk), ==, 0);
	stack_dtor(&thestk);
	UNIT_TEST_COMPARE(thestk.size, ==, 0);
	printf("\nALL TEST PASSED\n");
	return 0;
}

