/*
 * stack++_t.h
 *
 *  Created on: 18 Feb 2019
 *      Author: exucutional
 */

#ifndef STACK___T_HPP_
#define STACK___T_HPP_

#define STACK_PROT
#define STACK_PROT_MARK
#define STACK_PROT_HASH

#include <iostream>

typedef uint64_t data_t;

class stack
{
private:
	uintptr_t mark_beg;
	size_t size;
	size_t capacity;
	uint64_t hash;
	data_t *data;
	uintptr_t mark_end;
public:
	stack(size_t cap);
	~stack();
	int unit_test();
	int push(data_t value);
	data_t pop();
	int clear();
	int check();
	void dump(char *reason);
	uint64_t gnu_hash(void *buf, size_t size);
	uintptr_t mark(uintptr_t value);
protected:
};

#ifdef STACK_PROT_MARK
#define MARK  1
#define STACK_MARK_CHECK()	\
do	\
{	\
	uint64_t temp = 0;	\
	if (mark_beg != (temp = this->mark((uintptr_t)size)))	\
	{	\
		return 1;	\
	}	\
	if (mark_end != (temp = this->mark((uintptr_t)size)))	\
	{	\
		return 2;	\
	}	\
	if (data[0] != (temp = this->mark((uintptr_t)data)))	\
	{	\
		return 3;	\
	}	\
	if (data[size + 1] != (temp = this->mark((uintptr_t)data)))	\
	{	\
		return 4;	\
	}	\
}	\
while (0)
#define STACK_MARK_UPDATE()	\
do	\
{	\
	mark_beg = this->mark((uintptr_t)size);	\
	mark_end = this->mark((uintptr_t)size);	\
	data[0] = this->mark((uintptr_t)data);	\
	data[size + 1] = this->mark((uintptr_t)data);	\
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
	if (hash != (temp = this->gnu_hash(data, size)))	\
	{	\
		return 1;	\
	}	\
}	\
while (0)
#define STACK_HASH_UPDATE()	\
do	\
{	\
	hash = gnu_hash(data, size);	\
}	\
while (0)
#else
#define STACK_HASH_CHECK()
#define STACK_HASH_UPDATE()
#endif

#endif /* STACK___T_HPP_ */
