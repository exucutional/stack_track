/*
 ============================================================================
 Name        : Stack.c
 Author      : Exdev
 Version     : 0.7
 ============================================================================
 */

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>

typedef uint64_t data_t;

#define INITIAL_CAPACITY 100
#define UNIT_TEST_COMPARE(p1, op, p2)	\
{	\
	if (p1 op p2)	\
	{	\
		printf("[test passed]\n");	\
	} else	\
	{	\
		printf("[test failed] : " #p1 " not " #op " " #p2 "\n");	\
		stack_dump(&thestk, "error");	\
		return -1;	\
	}	\
}

struct stack
{
	uintptr_t mark_stk_begin;
	int size;
	int capacity;
	uint64_t hash;
	data_t *data;
	uintptr_t mark_stk_end;
};



int unit_test();
int stackctor(struct stack *thestk);
int stack_push_back(struct stack *thestk, data_t value);
int stack_pop_back(struct stack *thestk, data_t *buf);
int stack_clear(struct stack *thestk);
int stack_check(struct stack *thestk);
void stack_dump(struct stack *thestk, char *reason);
uint64_t stack_gnu_hash(void *array, int size);
uintptr_t stack_mark(uintptr_t value);
void stackdctor(struct stack *thestk);

//==========================================================================
int main()
{
	unit_test();
	return 0;
}
//==========================================================================

int stackctor(struct stack *thestk)
{
	thestk->size = 0;
	thestk->hash = 0;
	thestk->capacity = INITIAL_CAPACITY;
	thestk->mark_stk_begin = stack_mark((uintptr_t)&thestk->size);
	thestk->mark_stk_end = thestk->mark_stk_begin;
	thestk->data = (data_t*) calloc(thestk->capacity + 2, sizeof(data_t));
	if(thestk->data == NULL)
	{
		return -1;
	} else
	{
		thestk->data[0] = stack_mark((uintptr_t)&thestk->data);
		thestk->data[1] = thestk->data[0];
		return 0;
	}
}
//==========================================================================
int stack_push_back(struct stack *thestk, data_t value)
{
	if (thestk->size < thestk->capacity - 2  && isfinite((double)value))
	{
		thestk->data[thestk->size + 2] = thestk->data[thestk->size + 1];
		thestk->data[thestk->size++ + 1] = value;
		thestk->hash = stack_gnu_hash(thestk->data, thestk->size);
		return 0;
	} else
	{
		if (!isfinite((double)value))
		{
			return 1;
		} else
		{
			thestk->data = (data_t*) realloc(thestk->data, thestk->capacity * 2 * sizeof(data_t));
			if (thestk->data == NULL)
			{
				return 2;
			} else
			{
				thestk->mark_stk_begin = stack_mark((uintptr_t)&thestk->size);
				thestk->mark_stk_end = thestk->mark_stk_begin;
				thestk->data[thestk->size + 2] = thestk->data[thestk->size + 1];
				thestk->data[thestk->size++ + 1] = value;
				thestk->hash = stack_gnu_hash(thestk->data, thestk->size);
				thestk->capacity = thestk->capacity * 2;
				return 0;
			}
		}
	}
}
//==========================================================================
int stack_pop_back(struct stack *thestk, data_t *buf)
{
	if (thestk->size > 0)
	{
		*buf = thestk->data[--thestk->size + 1];
		thestk->hash = stack_gnu_hash(thestk->data, thestk->size);
		thestk->data[thestk->size + 1] = thestk->data[thestk->size + 2];
		thestk->data[thestk->size + 2] = 0;
		return 0;
	} else
	{
		return -1;
	}
}
//==========================================================================
int stack_clear(struct stack *thestk)
{
	while (thestk->size > 0)
	{
		thestk->data[--thestk->size] = 0;
	}
	if (thestk->size == 0)
	{
		return 0;
	} else
	{
		return -1;
	}
}
//==========================================================================
void stackdctor(struct stack *thestk)
{
	free(thestk->data);
	thestk->size = 0;
	thestk->mark_stk_begin = 0;
	thestk->mark_stk_end = 0;
}
//==========================================================================
int unit_test()
{
	struct stack thestk;
	data_t buf = 0;
	UNIT_TEST_COMPARE(stackctor(&thestk), ==, 0);
	UNIT_TEST_COMPARE(stack_check(&thestk), ==, 0);
	for (int i = 1; i < thestk.capacity - 1; i++)
	{
		UNIT_TEST_COMPARE(stack_push_back(&thestk, i * 33), ==, 0);
		UNIT_TEST_COMPARE(thestk.data[i], ==, i * 33);
	}
	stack_dump(&thestk, "check");
	UNIT_TEST_COMPARE(stack_pop_back(&thestk, &buf), ==, 0);
	UNIT_TEST_COMPARE(buf, ==, 3234);
	UNIT_TEST_COMPARE(stack_clear(&thestk), ==, 0);
	stackdctor(&thestk);
	UNIT_TEST_COMPARE(thestk.size, ==, 0);
	printf("\nALL TEST PASSED\n");
	return 1;
}
//==========================================================================
uint64_t stack_gnu_hash(void *array, int size)
{
	uint64_t hash = 0;
	data_t *data = array;
	for (int i = 1; i < size + 1; i++)
	{
		hash = hash * 33 + data[i];
	}
	return hash & 0xffffffffffffffff;
}
//==========================================================================
uint64_t stack_mark(uintptr_t value)
{
	return (value / 33 + value % 33) & 0xffffffffffffffff;
}
//==========================================================================
void stack_dump(struct stack *thestk, char *reason)
{
	uint64_t temp = 0;
	printf("\n==================================================\n");
	printf("Stack [%p]\n", thestk);
	printf("Dump reason : %s\n", reason);
	printf("Size : %d\n", thestk->size);
	printf("Capacity : %d\n", thestk->capacity);
	printf("==================================================\n");

	printf("Mark begin :    %lx\n", thestk->mark_stk_begin);
	if (thestk->mark_stk_begin == (temp = stack_mark((uintptr_t)&thestk->size)))
	{
		printf("                |ok\n");
	} else
	{
		printf("                |error\n");
	}
	printf("Expected mark : %lx\n", temp);

	printf("----------------------------------------\n");

	printf("Mark end :      %lx\n", thestk->mark_stk_end);
	if (thestk->mark_stk_end == (temp = stack_mark((uintptr_t)&thestk->size)))
	{
		printf("                |ok\n");
	} else
	{
		printf("                |error\n");
	}
	printf("Expected mark : %lx\n", temp);

	printf("----------------------------------------\n");

	printf("Hash :          %lx\n", thestk->hash);
	if (thestk->hash == (temp = stack_gnu_hash(thestk->data, thestk->size)))
	{
		printf("                |ok\n");
	} else
	{
		printf("                |error\n");
	}
	printf("Expected hash : %lx\n", temp);

	printf("==================================================\n");

	printf("Data[%d] [%p]\n", thestk->capacity, thestk->data);

	if (thestk->data == NULL)
	{
		return;
	}

	for (int i = 0; i < thestk->capacity; i++)
	{
		if (i == 0 || i == thestk->size + 1)
		{
			if (thestk->data[i] == (temp = stack_mark((uintptr_t)&thestk->data)))
			{
				printf("*[%d] Mark %lx --ok-- %lx[expected]\n", i, thestk->data[i], temp);
			} else
			{
				printf("*[%d] Mark %lx --error-- %lx[expected]\n", i, thestk->data[i], temp);
			}
		} else
		printf("*[%d] %ld\n",i ,thestk->data[i]);
	}
	printf("==================================================\n");
}
//==========================================================================
int stack_check(struct stack *thestk)
{
	uint64_t temp = 0;
	if (thestk->data == NULL)
	{
		return 1;
	}
	if (thestk->size < 0)
	{
		return 2;
	}
	if (thestk->capacity < 0)
	{
		return 3;
	}
	if (thestk->mark_stk_begin != (temp = stack_mark((uintptr_t)&thestk->size)))
	{
		return 4;
	}
	if (thestk->mark_stk_end != temp)
	{
		return 5;
	}
	if (thestk->hash != (temp = stack_gnu_hash(thestk->data, thestk->size)))
	{
		return 6;
	}
	return 0;
}
