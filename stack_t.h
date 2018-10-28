/*!
 * \brief stack_t.h
 * \author Exdev
 * \version 1.0.0
 */

#ifndef STACK_T_H_
#define STACK_T_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint64_t data_t;

extern const size_t STACK_T_INIT_CPT;

#define STACK_PROT
#define STACK_PROT_MARK
#define STACK_PROT_HASH

#ifdef STACK_PROT_MARK
#define MARK  1
#define STACK_MARK_CHECK()	\
do	\
{	\
	uint64_t temp = 0;	\
	if (thestk->mark_beg != (temp = stack_mark((uintptr_t)&thestk->size)))	\
	{	\
		return 1;	\
	}	\
	if (thestk->mark_end != (temp = stack_mark((uintptr_t)&thestk->size)))	\
	{	\
		return 2;	\
	}	\
	if (thestk->data[0] != (temp = stack_mark((uintptr_t)&thestk->data)))	\
	{	\
		return 3;	\
	}	\
	if (thestk->data[thestk->size + 1] != (temp = stack_mark((uintptr_t)&thestk->data)))	\
	{	\
		return 4;	\
	}	\
}	\
while (0)
#define STACK_MARK_UPDATE()	\
do	\
{	\
	thestk->mark_beg = stack_mark((uintptr_t)&thestk->size);	\
	thestk->mark_end = stack_mark((uintptr_t)&thestk->size);	\
	thestk->data[0] = stack_mark((uintptr_t)&thestk->data);	\
	thestk->data[thestk->size + 1] = stack_mark((uintptr_t)&thestk->data);	\
}	\
while (0)
#else
#define STACK_MARK_CHECK()
#define STACK_MARK_UPDATE()
#define MARK  0
#endif

#ifdef STACK_PROT_HASH
#define STACK_HASH_CHECK()	\
do	\
{	\
	uint64_t temp = 0;	\
	if (thestk->hash != (temp = gnu_hash(thestk->data, thestk->size)))	\
	{	\
		return 1;	\
	}	\
}	\
while (0)
#define STACK_HASH_UPDATE()	\
do	\
{	\
	thestk->hash = gnu_hash(thestk->data, thestk->size);	\
}	\
while (0)
#else
#define STACK_HASH_CHECK()
#define STACK_HASH_UPDATE()
#endif

struct stack
{
	uintptr_t mark_beg;
	size_t size;
	size_t capacity;
	uint64_t hash;
	data_t *data;
	uintptr_t mark_end;
};

int stack_unit_test();
int stack_ctor(struct stack *thestk);
int stack_push(struct stack *thestk, data_t value);
int stack_pop(struct stack *thestk, data_t *buf);
int stack_clear(struct stack *thestk);
int stack_check(struct stack *thestk);
void stack_dump(struct stack *thestk, char *reason);
uint64_t gnu_hash(void *buf, size_t size);
uintptr_t stack_mark(uintptr_t value);
void stack_dtor(struct stack *thestk);

#endif /* STACK_T_H_ */
