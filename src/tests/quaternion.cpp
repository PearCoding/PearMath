//#define PM_NO_SIMD
#include "test.h"

bool quaternionTest();

using namespace PM;

int main()
{
	printf("------------------------------------------------------------------------\n");
	printf("Tests for the math library PearMath. [quaternion] \n");
	printf("------------------------------------------------------------------------\n");
#ifdef PM_WITH_SIMD
	printf("SIMD support: true\n");
#else
	printf("SIMD support: false\n");
#endif
	printf("------------------------------------------------------------------------\n");

	bool ok = quaternionTest();

	printf("\n------------------------------------------------------------------------\n");
	if (!ok)
	{
		printf("Some tests have failures... Maybe the source is broken?\n");
		printf("Please send a bug report to the development team of PearMath.\n");
		printf("More information here: http://pearcoding.eu/projects/pearmath/ \n");
	}
	else
	{
		printf("Everything successful.\n");
		printf("Super!\n");
	}

	printf("------------------------------------------------------------------------\n");
	printf("\n");

#if defined(PM_OS_WINDOWS)
	printf("Press any key to exit...\n");
	system("PAUSE");
#endif
	return ok ? 0 : 1;
}

bool quaternionTest()
{
	INIT_TEST_UNIT("Quaternion");

	START_TEST("Multiply Quaternion");
	quat op1 = pm_Set(5, 1, 5, 2);
	quat op2 = pm_Set(2, 5, 3, 1);
	TEST(quat r = pm_MultiplyQuat(op1, op2));
	CHECK_TEST3(ALL_EQ(r, pm_Set(-13, 6, 34, -28)), op1, op2, r, pm_Set(-13, 6, 34, -28))
		END_TEST()

		START_TEST("Conjugate Quaternion");
	quat op1 = pm_Set(5, 1, 5, 2);
	TEST(quat r = pm_ConjugateQuat(op1));
	CHECK_TEST2(ALL_EQ(r, pm_Set(-5, -1, -5, 2)), op1, r, pm_Set(-5, -1, -5, 2))
		END_TEST()

		START_TEST("Inverse Quaternion");
	quat op1 = pm_Set(5, 1, 5, 2);
	TEST(quat r = pm_InverseQuat(op1));
	CHECK_TEST2(ALL_NEARLY_EQ(r, pm_Set(-0.090909090909091f, -0.018181818181818f, -0.090909090909091f, 0.036363636363636f), ABSOLUTE_ERROR),
		op1, r, pm_Set(-0.090909090909091f, -0.018181818181818f, -0.090909090909091f, 0.036363636363636f))
		END_TEST()

		return TEST_UNIT_RESULT;
}
