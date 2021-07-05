/*!
 * \brief stack_t.c
 * \author Exdev
 * \version 1.0.0
 */

#include "stack_t.h"

const size_t STACK_T_INIT_CPT = 100;

int stack_ctor(struct stack *thestk)
{
	thestk->size = 0;
	thestk->hash = 0;
	thestk->capacity = STACK_T_INIT_CPT;
	thestk->data = (data_t*) calloc(thestk->capacity + 2 * MARK, sizeof(data_t));
	if(thestk->data == NULL)
	{
		return -1;
	}
	STACK_MARK_UPDATE();
	return 0;
}

int stack_push(struct stack *thestk, data_t value)
{
	if (thestk->size < thestk->capacity - 2 * MARK)
	{
		STACK_MARK_CHECK();
		STACK_HASH_CHECK();
		thestk->data[thestk->size++ + MARK] = value;
		STACK_MARK_UPDATE();
		STACK_HASH_UPDATE();
		return 0;
	}
	thestk->data = (data_t*) realloc(thestk->data, thestk->capacity * 2 * sizeof(data_t));
	if (thestk->data == NULL)
	{
		return -1;
	}
	STACK_MARK_CHECK();
	STACK_HASH_CHECK();
	thestk->data[thestk->size++ + MARK] = value;
	STACK_MARK_UPDATE();
	STACK_HASH_UPDATE();
	thestk->capacity = thestk->capacity * 2;
	return 0;
}

int stack_pop(struct stack *thestk, data_t *buf)
{
	if (thestk->size > 0)
	{
		STACK_MARK_CHECK();
		STACK_HASH_CHECK();
		*buf = thestk->data[--thestk->size + MARK];
		STACK_MARK_UPDATE();
		STACK_HASH_UPDATE();
		return 0;
	}
	return -1;
}

int stack_clear(struct stack *thestk)
{
	while (thestk->size > 0)
	{
		thestk->data[--thestk->size] = 0;
	}
	if (thestk->size == 0)
	{
		return 0;
	}
	return -1;
}

void stack_dtor(struct stack *thestk)
{
	free(thestk->data);
	thestk->size = 0;
	thestk->mark_beg = 0;
	thestk->mark_end = 0;
}

uint64_t gnu_hash(void *buf, size_t size)
{
	uint64_t hash = 0;
	data_t *data = buf;
	for (int i = MARK; i < size + MARK; i++)
	{
		hash = hash * 33 + data[i];
	}
	return hash;
}

uint64_t stack_mark(uintptr_t value)
{
	return (value << 5) + value;
}

void stack_dump(struct stack *thestk, char *reason)
{
	uint64_t temp = 0;
	printf("\n==================================================\n");
	printf("Stack [%p]\n", thestk);
	printf("Dump reason : %s\n", reason);
	printf("Size : %ld\n", thestk->size);
	printf("Capacity : %ld\n", thestk->capacity);
	printf("==================================================\n");

	#ifdef STACK_PROT_MARK
	printf("Mark begin :    %lx\n", thestk->mark_beg);
	if (thestk->mark_beg == (temp = stack_mark((uintptr_t)&thestk->size)))
	{
		printf("                |ok\n");
	}
	else
	{
		printf("                |error\n");
	}
	printf("Expected mark : %lx\n", temp);

	printf("----------------------------------------\n");

	printf("Mark end :      %lx\n", thestk->mark_end);
	if (thestk->mark_end == (temp = stack_mark((uintptr_t)&thestk->size)))
	{
		printf("                |ok\n");
	}
	else
	{
		printf("                |error\n");
	}
	printf("Expected mark : %lx\n", temp);

	printf("==================================================\n");
	#endif

	#ifdef STACK_PROT_HASH
	printf("Hash :          %lx\n", thestk->hash);
	if (thestk->hash == (temp = gnu_hash(thestk->data, thestk->size)))
	{
		printf("                |ok\n");
	}
	else
	{
		printf("                |error\n");
	}
	printf("Expected hash : %lx\n", temp);

	printf("==================================================\n");
	#endif
	printf("Data[%ld] [%p]\n", thestk->capacity, thestk->data);

	if (thestk->data == NULL)
	{
		return;
	}

	for (int i = 0; i < thestk->capacity; i++)
	{
		#ifdef STACK_PROT_MARK
		if (i == 0 || i == thestk->size + MARK)
		{
			if (thestk->data[i] == (temp = stack_mark((uintptr_t)&thestk->data)))
			{
				printf("*[%d] Mark %lx --ok-- %lx[expected]\n", i, thestk->data[i], temp);
			}
			else
			{
				printf("*[%d] Mark %lx --error-- %lx[expected]\n", i, thestk->data[i], temp);
			}
		}
		else
		#endif
		printf("*[%d] %ld\n",i ,thestk->data[i]);
	}
	printf("==================================================\n");
}

int stack_check(struct stack *thestk)
{
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
	STACK_HASH_CHECK();
	STACK_MARK_CHECK();
	return 0;
}
