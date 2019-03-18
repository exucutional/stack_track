/*
 * stack++_t.cpp
 *
 *  Created on: 18 Feb 2019
 *      Author: exucutional
 */
#include "stack++_t.hpp"

stack::stack(size_t cap):
				mark_beg(0),
				size(0),
				capacity(cap),
				hash(0),
				data(new data_t[cap + 2 * MARK]),
				mark_end(0)
{
	STACK_MARK_UPDATE();
	std::cout << "\nstack ctor\n";
}

stack::~stack()
{
	delete[] data;
	size = 0;
	mark_beg = 0;
	mark_end = 0;
	std::cout << "\nstack dtor\n";
}

int stack::push(data_t value)
{
	if (size < capacity - 2 * MARK) {
		STACK_MARK_CHECK();
		STACK_HASH_CHECK();
		data[size++ + MARK] = value;
		STACK_MARK_UPDATE();
		STACK_HASH_UPDATE();
		return 0;
	}
	data_t* tmpptr = data;
	data = new data_t[2 * capacity + 2 * MARK];
	if (data == NULL)
		return -1;
	for (int i = 0; i <= size; i++)
		data[i] = tmpptr[i];
	delete[] tmpptr;
	STACK_MARK_CHECK();
	STACK_HASH_CHECK();
	data[size++ + MARK] = value;
	STACK_MARK_UPDATE();
	STACK_HASH_UPDATE();
	capacity = capacity * 2;
	return 0;
}

data_t stack::pop(void)
{
	if (size > 0) {
		STACK_MARK_CHECK();
		STACK_HASH_CHECK();
		data_t tmp = data[--size + MARK];
		STACK_MARK_UPDATE();
		STACK_HASH_UPDATE();
		return tmp;
	}
	return ~0;
}

int stack::clear(void)
{
	while (size > 0)
		data[--size] = 0;
	if (size == 0)
		return 0;
	return -1;
}

uint64_t stack::gnu_hash(void *buf, size_t size)
{
	uint64_t hash = 0;
	auto data = static_cast<data_t*>(buf);
	for (int i = MARK; i < size + MARK; i++)
	{
		hash = hash * 33 + data[i];
	}
	return hash;
}

uint64_t stack::mark(uintptr_t value)
{
	return (value << 5) + value;
}

void stack::dump(char *reason)
{
	uint64_t temp = 0;
	printf("\n==================================================\n");
	printf("Stack [%p]\n", this);
	printf("Dump reason : %s\n", reason);
	printf("Size : %ld\n", size);
	printf("Capacity : %ld\n", capacity);
	printf("==================================================\n");

	#ifdef STACK_PROT_MARK
	printf("Mark begin :    %lx\n", mark_beg);
	if (mark_beg == (temp = this->mark((uintptr_t)size)))
	{
		printf("                |ok\n");
	}
	else
	{
		printf("                |error\n");
	}
	printf("Expected mark : %lx\n", temp);

	printf("----------------------------------------\n");

	printf("Mark end :      %lx\n", mark_end);
	if (mark_end == (temp = this->mark((uintptr_t)size)))
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
	printf("Hash :          %lx\n", hash);
	if (hash == (temp = this->gnu_hash(data, size)))
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
	printf("Data[%ld] [%p]\n", capacity, data);

	if (data == NULL)
	{
		return;
	}

	for (int i = 0; i < capacity; i++)
	{
		#ifdef STACK_PROT_MARK
		if (i == 0 || i == size + MARK)
		{
			if (data[i] == (temp = this->mark((uintptr_t)data)))
			{
				printf("*[%d] Mark %lx --ok-- %lx[expected]\n", i, data[i], temp);
			}
			else
			{
				printf("*[%d] Mark %lx --error-- %lx[expected]\n", i, data[i], temp);
			}
		}
		else
		#endif
		printf("*[%d] %ld\n",i ,data[i]);
	}
	printf("==================================================\n");
}

int stack::check(void)
{
	if (data == nullptr)
		return 1;
	if (size < 0)
		return 2;
	if (capacity < 0)
		return 3;
	STACK_HASH_CHECK();
	STACK_MARK_CHECK();
	return 0;
}



