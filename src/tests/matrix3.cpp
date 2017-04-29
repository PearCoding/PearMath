//#define PM_NO_SIMD
#include "test.h"

bool matrixTest();

using namespace PM;

int main()
{
	printf("------------------------------------------------------------------------\n");
	printf("Tests for the math library PearMath [matrix].\n");
	printf("------------------------------------------------------------------------\n");
#ifdef PM_WITH_SIMD
	printf("SIMD support: true\n");
#else
	printf("SIMD support: false\n");
#endif
	printf("------------------------------------------------------------------------\n");

	bool ok = matrixTest();

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

bool matrixTest()
{
	INIT_TEST_UNIT("Matrix");

	const mat3 testOp1 = pm_Create(0.0, 1.0, 0.0,
		2.0, 0.0, 1.0,
		0.0, 1.0, 1.0);

	const mat3 testOp2 = pm_Create(3.0, 1.0, 1.0,
		2.0, 5.0, 1.0,
		2.0, 1.0, 4.0);

	START_TEST("Identity Matrix");
	TEST(mat3 r = pm_Identity3());
	CHECK_TEST2(ALL_EQ(r, pm_Create(1, 0, 0, 0, 1, 0, 0, 0, 1)),
		testOp1, r, pm_Create(1, 0, 0, 0, 1, 0, 0, 0, 1));
	END_TEST()

		START_TEST("Zero Matrix");
	TEST(mat3 r = pm_ZeroMatrix3());
	CHECK_TEST2(ALL_EQ(r, pm_FillMatrix3(0)),
		testOp1, r, pm_FillMatrix3(0));
	END_TEST()

		START_TEST("(GET) Matrix");
	TEST(float r = pm_GetIndex(testOp1, 1, 2));
	CHECK_TEST2(r == 1, testOp1, r, 1);
	END_TEST()

		START_TEST("(GET) Matrix");
	TEST(vec3 r = pm_GetRow(testOp1, 2));
	CHECK_TEST2(ALL_EQ(r, pm_Set(0, 1, 1)), testOp1, r, pm_Set(0, 1, 1));
	END_TEST()

		START_TEST("(GET) Matrix");
	TEST(vec3 r = pm_GetColumn(testOp1, 1));
	CHECK_TEST2(ALL_EQ(r, pm_Set(1.0, 0.0, 1.0)), testOp1, r, pm_Set(1.0, 0.0, 1.0));
	END_TEST()

	START_TEST("Equal Matrix");
	TEST(bool b = ALL_EQ(testOp1, testOp1));
	CHECK_TEST3(b, testOp1, testOp1, b, true);
	END_TEST()

	START_TEST("Nearly Equal Matrix");
	mat3 m = pm_Create(0.0, 1.0001f, 0.0,
		2.0001f, 0.0, 1.0001f,
		0.0, 1.0001f, 1.0001f);
	TEST(bool b = ALL_NEARLY_EQ(testOp1, m, 0.001f));
	CHECK_TEST3(b, testOp1, m, b, true);
	END_TEST()

	START_TEST("Not Equal Matrix");
	TEST(bool b = pm_IsSomeTrue(pm_IsNotEqual(testOp1, testOp2)));
	CHECK_TEST3(b, testOp1, testOp2, b, true);
	END_TEST()

		START_TEST("Matrix + Matrix");
	TEST(mat3 r = pm_Add(testOp1, testOp2));
	CHECK_TEST3(ALL_EQ(r, pm_Create(3,2,1,4,5,2,2,2,5)), testOp1, testOp2, r, pm_Create(3,2,1,4,5,2,2,2,5));
	END_TEST()

		START_TEST("Matrix - Matrix");
	TEST(mat3 r = pm_Subtract(testOp1, testOp2));
	CHECK_TEST3(ALL_EQ(r, pm_Create(-3,0,-1,0,-5,0,-2,0,-3)), testOp1, testOp2, r, pm_Create(-3,0,-1,0,-5,0,-2,0,-3));
	END_TEST()

		START_TEST("Matrix * Matrix");
	TEST(mat3 r = pm_Multiply(testOp1, testOp2));
	CHECK_TEST3(ALL_EQ(r, pm_Create(0,1,0,4,0,1,0,1,4)), testOp1, testOp2, r, pm_Create(0,1,0,4,0,1,0,1,4));
	END_TEST()

		START_TEST("Matrix / Matrix");
	TEST(mat3 r = pm_Divide(testOp1, testOp2));
	CHECK_TEST3(ALL_EQ(r, pm_Create(0,1,0,1,0,1,0,1,0.25)), testOp1, testOp2, r, pm_Create(0,1,0,1,0,1,0,1,0.25));
	END_TEST()

		START_TEST("Matrix * f");
	TEST(mat3 r = pm_Scale(testOp1, 2));
	CHECK_TEST3(ALL_EQ(r, pm_Create(0,2,0,4,0,2,0,2,2)), testOp1, testOp2, r, pm_Create(0,2,0,4,0,2,0,2,2));
	END_TEST()

		START_TEST("Matrix x Matrix");
	TEST(mat3 r = pm_Product(testOp1, testOp2));
	CHECK_TEST3(ALL_EQ(r, pm_Create(2,5,1,8,3,6,4,6,5)), testOp1, testOp2, r, pm_Create(2,5,1,8,3,6,4,6,5));
	END_TEST()

		START_TEST("Matrix x Vector");
	TEST(vec3 r = pm_Product(testOp1, pm_Set(0,1,1)));
	CHECK_TEST3(ALL_EQ(r, pm_Set(1,1,2)), testOp1, pm_Set(0,1,1), r, pm_Set(1,1,2));
	END_TEST()

		START_TEST("Transpose Matrix");
	TEST(mat3 r = pm_Transpose(testOp1));
	CHECK_TEST2(ALL_EQ(r, pm_Create(0,2,0,1,0,1,0,1,1)), testOp1, r, pm_Create(0,2,0,1,0,1,0,1,1));
	END_TEST()

		START_TEST("Determinant Matrix");
	TEST(float r = pm_Determinant(testOp1));
	CHECK_TEST2(r == -2, testOp1, r, -2);
	END_TEST()

		START_TEST("Inverse Matrix");
	TEST(mat3 r = pm_Inverse(testOp1));
	CHECK_TEST2(ALL_NEARLY_EQ(r, pm_Create(0.5,0.5,-0.5,1,0,0,-1,0,1), ABSOLUTE_ERROR),
		testOp1, r, pm_Create(0.5,0.5,-0.5,1,0,0,-1,0,1));
	END_TEST()

	return TEST_UNIT_RESULT;
}