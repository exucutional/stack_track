/// You can use doxygen-style for this.
/*
 ============================================================================
 Name        : Stack.c
 Author      : Exdev
 Version     : 0.7.1
 ============================================================================
 */

#include <stdio.h>
#include <malloc.h> 	/// included in stdlib
#include <stdlib.h>
#include <stdbool.h> 	/// why
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>

typedef uint64_t data_t;

/// Avoid defines of const values
// cosnt size_t STACK_T_INIT_CPT = 100;
#define INITIAL_CAPACITY 100

/// There must be: 
// #define name(args)	\
// do {			\
// 	same code;	\
// }			\
// while (0) // ';' manually
//
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


/// These definitions must be written in separate .h (header) file
/// example: 	stack_t.h (only definitions, use header protection)
/// 		stack_t.c (only code of stack_t functions defined in
///			stack_t.h)
///////////////////////////////////////////////////////////////////////////////////////////

/// Header protection (in header file)
// #ifndef _STACK_T_H_
// #define _STACK_T_H_
//	your code
// #endif

/// stack_t (c-linux-like) or Stack (cpp-like) name is better
struct stack
{
	uintptr_t mark_stk_begin;	/// This name is too long.
					/// maybe mark_beg and mark_end
	int size;			/// size_t
	int capacity;			/// size_t
	uint64_t hash;
	data_t *data;
	uintptr_t mark_stk_end;
};


int unit_test();						/// stack_unit_test
int stackctor(struct stack *thestk);				/// stack_ctor
int stack_push_back(struct stack *thestk, data_t value);	/// You can just use stack_push 
								/// I recommend data_t buf
int stack_pop_back(struct stack *thestk, data_t *buf);		/// stack_pop
int stack_clear(struct stack *thestk);
int stack_check(struct stack *thestk);
void stack_dump(struct stack *thestk, char *reason);		/// int stack_dump ;)
uint64_t stack_gnu_hash(void *array, int size);			/// gnu_hash(void *buf, size_t size),
								/// it can be used separately.
								/// note buf, it's not an array
uintptr_t stack_mark(uintptr_t value);
void stackdctor(struct stack *thestk);				/// stack_dtor
///////////////////////////////////////////////////////////////////////////////////////////


/// main() in main.c, separated from stack_t.c
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
	/// It's old style, you need no typecast here
	thestk->data = (data_t*) calloc(thestk->capacity + 2, sizeof(data_t));
	if(thestk->data == NULL)
	{
		return -1;
	} else	/// You need no else after return
	{
		thestk->data[0] = stack_mark((uintptr_t)&thestk->data);
		thestk->data[1] = thestk->data[0];
		return 0;
	}
}
//==========================================================================
//
/// You must rewrite this func
int stack_push_back(struct stack *thestk, data_t value)
{
	/// Why is isfinite here?
	if (thestk->size < thestk->capacity - 2  && isfinite((double)value))
	{
		thestk->data[thestk->size + 2] = thestk->data[thestk->size + 1];
		/// ++ +, avoid this
		thestk->data[thestk->size++ + 1] = value;


		/// Nope, nope, nope. 
		/// example:
		//  
		//  in .h:
		//  #define STACK_PROT      // simple checks
		//  #define STACK_PROT_MARK // marks
		//  #define STACK_PROT_HASH // hash
		//  // or comment them to skip tests
		//
		//  List:
		//  STACK_CHECK();
		//  STACK_MARK_CHECK();
		//  STACK_MARK_UPDATE();
		//  STACK_HASH_CHECK();
		//  STACK_HASH_UPDATE();
		//
		//  example:
		//
		//  #ifdef STACK_PROT_HASH
		//  #define STACK_HASH_CHECK()	\
		//  ...
		//  #define STACK_HASH_UPDATE() \
		//  ...
		//  #else
		//  #define STACK_HASH_CHECK() ;
		//  #define STACK_HASH_UPDATE() ;
		//  #endif
		//
		//  Than you get STACK_CHECK() that you must place
		//  in every stack_t-function
		//
		//  Example - my version of stack_pop
		//
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
	//1	2	3	4  - too many tabs, this is bad code
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

/// My version
int stack_pop(struct stack_t *s, sdata_t *buf)
{
	STACK_CHECK();
	STACK_HASH_CHECK();
	STACK_MARK_CHECK();
	if (s->size < 2) {
		assert(!"stack_pop with empty stack");
		return -1;
	}
	sdata_t tmp = s->data[1 + s->size--];
	STACK_MARK_UPDATE();
	STACK_HASH_UPDATE();
	return tmp;
}


//==========================================================================
/// one line -> memset()
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
	UNIT_TEST_COMPARE(stack_check(&thestk), ==, 0);
	stack_dump(&thestk, "check");
	UNIT_TEST_COMPARE(stack_pop_back(&thestk, &buf), ==, 0);
	UNIT_TEST_COMPARE(buf, ==, 3234);
	UNIT_TEST_COMPARE(stack_clear(&thestk), ==, 0);
	stackdctor(&thestk);
	UNIT_TEST_COMPARE(thestk.size, ==, 0);
	printf("\nALL TEST PASSED\n");
	return 0;
}
//==========================================================================
uint64_t stack_gnu_hash(void *array, int size)
{
	/// init magic value lost
	uint64_t hash = 0; 
	data_t *data = array;
	for (int i = 1; i < size + 1; i++)
	{
		hash = hash * 33 + data[i];
	}
	/// just return hash;
	// return hash;
	return hash & 0xffffffffffffffff;
}
//==========================================================================
uint64_t stack_mark(uintptr_t value)
{
	return (value / 33 + value % 33) & 0xffffffffffffffff;
	/// Now you dont understand meaning of 33 and 0xff...f mask
	// here:
	// return value << 5 + value
}
//==========================================================================
/// Absolute overengineering
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
