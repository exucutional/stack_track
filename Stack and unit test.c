/*
 ============================================================================
 Name        : Stack and unit test.c
 Author      : Exdev
 Version     : 0.5
 ============================================================================
 */

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <locale.h>

#define CAPACITY 100
#define ERROR -1
#define UNIT_TEST_COMPARE(p1, op, p2)	\
{	\
	if (p1 op p2)	\
	{	\
		printf("[test passed]\n");	\
	} else	\
	{	\
		printf("[test failed] : " #p1 " isn't " #op " " #p2 "\n");	\
		return ERROR;	\
	}	\
}

struct stack
{
	int size;
	double *data;
};

int unit_test();
int stackctor(struct stack *thestk);
int stack_push_back(struct stack *thestk, double value);
double stack_pop_back(struct stack *thestk);
int stack_clear(struct stack *thestk);
void stackdctor(struct stack *thestk);

int main()
{
	unit_test();
	return 0;
}

int stackctor(struct stack *thestk)
{
	thestk->size = 0;
	thestk->data = (double*) calloc(CAPACITY, sizeof(double));
	if(thestk->data == NULL)
	{
		return ERROR;
	} else
	{
		return 1;
	}
}

int stack_push_back(struct stack *thestk, double value)
{
	if (thestk->size < CAPACITY  && isfinite(value))
	{
		thestk->data[thestk->size++] = value;
		return 1;
	} else
	{
		return ERROR;
	}
}

double stack_pop_back(struct stack *thestk)
{
	if (thestk->size > 0)
	{
		return thestk->data[--thestk->size];
	} else
	{
		return ERROR;
	}
}

int stack_clear(struct stack *thestk)
{
	while (thestk->size > 0)
	{
		thestk->data[--thestk->size] = 0;
	}
	if (thestk->size == 0)
	{
		return 1;
	} else
	{
		return ERROR;
	}
}

void stackdctor(struct stack *thestk)
{
	free(thestk->data);
	thestk->size = 0;
}

int unit_test()
{
	struct stack thestk;
	UNIT_TEST_COMPARE(stackctor(&thestk), ==, 1);
	for (int i = 0; i < CAPACITY; i++)
	{
		UNIT_TEST_COMPARE(stack_push_back(&thestk, i), ==, 1);
		UNIT_TEST_COMPARE(thestk.data[i], ==, i);
	}
	UNIT_TEST_COMPARE(stack_push_back(&thestk, 1), ==, ERROR);
	UNIT_TEST_COMPARE(stack_pop_back(&thestk), ==, CAPACITY - 1);
	UNIT_TEST_COMPARE(stack_clear(&thestk), ==, 1);
	stackdctor(&thestk);
	UNIT_TEST_COMPARE(thestk.size, ==, 0);
	printf("\nALL TEST PASSED\n");
	return 1;
}
