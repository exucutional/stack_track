//============================================================================
/*!
 * \brief Square_solution
 * \author Eric
 * \version 1.1.2
 */
//============================================================================


#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <errno.h>
#include <math.h>


#define DEF_SS_TEST(a, b, c, x1_test, x2_test, num_test)				\
{																		\
		if(!SS_test(a, b, c, x1_test, x2_test))							\
		{																\
			printf("Test %d failed\n", num_test);						\
			return 0;													\
		}																\
}

const int SS_SIZE_DATA_TEST = 15;
const int SS_INF_ROOTS = 3;
const int SS_ERROR = -1;

//============================================================================
/*!
 * \brief Roots_determination
 * \param[in] a Coefficent a
 * \param[in] b Coefficent b
 * \param[in] c Coefficent c
 * \param[in,out] x1 Root 1
 * \param[in,out] x2 Root 2
 * \return Number of roots
 */
int Roots_determination(double a, double b, double c, double *x1, double *x2);
//============================================================================

/*!
 * \brief Unit test
 * \return 0 if all test passed
 * \return 1 if not all test passed
 */
int Unit_test();
//============================================================================

/*!
 * \brief SS_test
 * Testing Roots_determination
 * \param[in] a Coefficent a
 * \param[in] b Coefficent b
 * \param[in] c Coefficent c
 * \param[in] x1_test Test root 1
 * \param[in] x2_test Test root 2
 * \return 0 if test failed
 * \return 1 if test passed
 */
int SS_test(double a, double b, double c, double x1_test, double x2_test);
//============================================================================
int main()
{
	double a = 0;
	double b = 0;
	double c = 0;
	double x1 = 0;
	double x2 = 0;

	assert(Unit_test());

	printf("\nSquare equation solution (ax^2 + bx + c)\n");
	printf("(c) Eric, 2018\n\n");
	printf("Enter coefficents a, b, c\n");


	while(scanf("%lf%lf%lf", &a, &b, &c) != 3 || !isfinite(a) || !isfinite(b) || !isfinite(c))
	{
		char temp;
		while ((temp = getchar()) != '\n' && temp != '\0');
		printf("Input error\n");
	}


	switch (Roots_determination(a, b, c, &x1, &x2))
	{
		case 2:
			printf("\nRoot 1 = %lf\nRoot 2 = %lf", x1, x2);
			break;
		case 1:
			printf("\nRoot = %lf", x1);
			break;
		case 0:
			printf("\nNo Roots");
			break;
		case SS_INF_ROOTS:
			printf("Any number");
			break;
		case SS_ERROR:
			printf("ERROR");
			break;
		default:
			break;
	}
	return 0;
}
//============================================================================

int Roots_determination(double a, double b, double c, double *x1, double *x2) {

	if (a == 0 && b == 0 && c == 0)
	{
		return SS_INF_ROOTS;
	}
	if (a == 0 && b == 0)
	{
		return 0;
	}
	if (a == 0)
	{
		*x1 = -c / b;
		if (!isfinite(*x1))
		{
			return SS_ERROR;
		}
		return 1;
	}

	double discriminant = b * b - 4 * a * c;

	if (discriminant > 0)
	{
		*x1 = (-b + sqrt(discriminant)) / 2 / a;
		*x2 = (-b - sqrt(discriminant)) / 2 / a;
		if (!isfinite(*x1) || !isfinite(*x2))
		{
			return SS_ERROR;
		}
		return 2;
	}
	if (discriminant == 0)
	{
		*x1 = -b / 2 / a;
		if (!isfinite(*x1))
		{
			return SS_ERROR;
		}
		return 1;
	}
	if (discriminant < 0)
	{
		return 0;
	}
	return SS_ERROR;
}
//============================================================================

int SS_test(double a, double b, double c, double x1_test, double x2_test)
{
	double x1 = 0;
	double x2 = 0;
	int number_roots = Roots_determination(a, b, c, &x1, &x2);
	if (number_roots == 2 || number_roots == 1)
	{
		if ( (x1 == x1_test && x2 == x2_test) || (x1 == x2_test && x2 == x1_test) )
		{
			return 1;
		}
	}
	switch (number_roots)
	{
		case 0:
			if (a == 0 && b == 0 && c != 0)
			{
				return 1;
			}
			if (b * b - 4 * a * c < 0)
			{
				return 1;
			}
			break;
		case SS_INF_ROOTS:
			if (a == 0 && b == 0 && c == 0)
			{
				return 1;
			}
			break;
		default:
			break;
	}
	return 0;
}
//============================================================================

int Unit_test()
{
	FILE* fin;

	double a = 0, b = 0, c = 0;
	double x1_test = 0, x2_test = 0;

	fin = fopen("Test_data.txt", "r");

	if (fin == NULL)
	{
		printf("Test failed : Couldn't open test data file\n");
		return 0;
	}
	for (int i = 1; i <= SS_SIZE_DATA_TEST; i++)
	{
		fscanf(fin, "%lf %lf %lf %lf %lf", &a, &b, &c, &x1_test, &x2_test);
		DEF_SS_TEST(a, b, c, x1_test, x2_test, i);
	}
	fclose(fin);
	return 1;
}
