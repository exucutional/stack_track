/*
 * unit_test.cpp
 *
 *  Created on: 18 Feb 2019
 *      Author: exucutional
 */

#include <iostream>
#include "stack++_t.hpp"


int main(void)
{
	stack stk(10);
	stk.push(1);
	stk.push(2);
	stk.push(103);
	std::cout << stk.pop() << std::endl;
	stk.push(12345);
	stk.dump("TEST");
	return EXIT_SUCCESS;
}
