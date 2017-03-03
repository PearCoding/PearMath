//#define PM_NO_SIMD
#include "test.h"

bool matrixTest();

using namespace PM;

int main()
{
	printf("------------------------------------------------------------------------\n");
	printf("Tests for the math library PearMath [matrix].\n");
	printf("------------------------------------------------------------------------\n");
#ifdef PM_USE_SIMD
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

	const mat4 testOp1 = pm_Create(0.0, 1.0, 0.0, 2.0,
		2.0, 0.0, 1.0, 0.0,
		0.0, 1.0, 1.0, 2.0,
		2.0, 0.0, 1.0, 1.0);

	const mat4 testOp2 = pm_Create(3.0, 1.0, 1.0, 2.0,
		2.0, 5.0, 1.0, 1.0,
		2.0, 1.0, 4.0, 2.0,
		2.0, 2.0, 3.0, 1.0);

	START_TEST("Identity Matrix");
	TEST(mat4 r = pm_Identity4());
	CHECK_TEST2(pm_IsEqual(r, pm_Create(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)),
		testOp1, r, pm_Create(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
	END_TEST()

		START_TEST("Zero Matrix");
	TEST(mat4 r = pm_ZeroMatrix4());
	CHECK_TEST2(pm_IsEqual(r, pm_FillMatrix4(0)),
		testOp1, r, pm_FillMatrix4(0));
	END_TEST()

		START_TEST("Translation Matrix");
	TEST(mat4 r = pm_Translation(pm_Set(3, 2, 6)));
	CHECK_TEST1(pm_IsEqual(r, pm_Create(1, 0, 0, 3, 0, 1, 0, 2, 0, 0, 1, 6, 0, 0, 0, 1)), r, pm_Create(1, 0, 0, 3, 0, 1, 0, 2, 0, 0, 1, 6, 0, 0, 0, 1));
	END_TEST()

		//TODO: Should have a better precision...
		START_TEST("Rotation Quaternion Matrix");
	TEST(mat4 r = pm_Rotation(pm_Set(0, 0.949f, 0, 0.316f)));
	CHECK_TEST1(pm_IsNearlyEqual(r, pm_Create(-0.8f, 0, 0.6f, 0, 0, 1, 0, 0, -0.6f, 0, -0.8f, 0, 0, 0, 0, 1), 0.002f), r, pm_Create(-0.8f, 0, 0.6f, 0, 0, 1, 0, 0, -0.6f, 0, -0.8f, 0, 0, 0, 0, 1));
	END_TEST()

		START_TEST("Rotation Axis Matrix");
	TEST(mat4 r = pm_Rotation(pm_Set(3, 2, 6), pm_DegToRad(45)));
	CHECK_TEST1(pm_IsNearlyEqual(r, pm_Create(
		3.343145751f, -2.485281374f, 6.686291501f, 0,
		6, 1.878679656f, 1.393398282f, 0,
		3.857864376f, 5.636038969f, 11.25126266, 0,
		0, 0, 0, 1), ABSOLUTE_ERROR), r,
		pm_Create(3.343145751f, -2.485281374f, 6.686291501f, 0,
		6, 1.878679656f, 1.393398282f, 0,
		3.857864376f, 5.636038969f, 11.25126266, 0,
		0, 0, 0, 1));
	END_TEST()

		START_TEST("Scaling Matrix");
	TEST(mat4 r = pm_Scaling(pm_Set(3, 2, 6)));
	CHECK_TEST1(pm_IsEqual(r, pm_Create(3, 0, 0, 0, 0, 2, 0, 0, 0, 0, 6, 0, 0, 0, 0, 1)), r, pm_Create(3, 0, 0, 0, 0, 2, 0, 0, 0, 0, 6, 0, 0, 0, 0, 1));
	END_TEST()

		START_TEST("(GET) Matrix");
	TEST(float r = pm_GetIndex(testOp1, 1, 2));
	CHECK_TEST2(r == 1, testOp1, r, 1);
	END_TEST()

		START_TEST("(GET) Matrix");
	TEST(vec4 r = pm_GetRow(testOp1, 2));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(0.0, 1.0, 1.0, 2.0)), testOp1, r, pm_Set(0.0, 1.0, 1.0, 2.0));
	END_TEST()

		START_TEST("(GET) Matrix");
	TEST(vec4 r = pm_GetColumn(testOp1, 2));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(0.0, 1.0, 1.0, 1.0)), testOp1, r, pm_Set(0.0, 1.0, 1.0, 1.0));
	END_TEST()

		START_TEST("Translation Decompose");
		mat4 op1 = pm_Translation(pm_Set(3, 2, 6));
	TEST(vec3 r = pm_DecomposeTranslation(op1));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(3, 2, 6)), op1, r, pm_Set(3, 2, 6));
	END_TEST()

		START_TEST("Rotation Decompose");
		mat4 op1 = pm_Rotation(pm_Set(0, 0.949f, 0, 0.316f));
	TEST(quat r = pm_DecomposeRotation(op1));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(0, 0.949f, 0, 0.316f)), op1, r, pm_Set(0, 0.949f, 0, 0.316f));
	END_TEST()

		START_TEST("Scale Decompose");
		mat4 op1 = pm_Scaling(pm_Set(3, 2, 6));
	TEST(vec3 r = pm_DecomposeScale(op1));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(3, 2, 6)), op1, r, pm_Set(3, 2, 6));
	END_TEST()

		START_TEST("Decompose (ELEM)");// TODO
		mat4 op1 = pm_Create(-2,-1,2,0,  -2,1,-1,0,  0,0,1,0,  0,0,0,1);
		quat resR = pm_RotationMatrix(pm_Create(1/1.4142f, -1/1.4142f,0,0,  1/1.4142f,1/1.4142f,0,0,  0,0,1,0,  0,0,0,1));
	TEST(vec3 t = pm_DecomposeTranslation(op1));
	CHECK_TEST2(pm_IsEqual(t, pm_Set(0, 0, 0)), op1, t, pm_Set(0, 0, 0));
	TEST(quat r = pm_DecomposeRotation(op1));
	CHECK_TEST2(pm_IsEqual(r, resR), op1, r, resR);
	TEST(vec3 s = pm_DecomposeScale(op1));
	CHECK_TEST2(pm_IsEqual(s, pm_Set(-2*1.4142f, 1.4142f, 2.44949f)), op1, s, pm_Set(-2*1.4142f, 1.4142f, 2.44949f));
	END_TEST()

		START_TEST("Decompose 2 (ELEM)");// TODO
		mat4 op1 = pm_Create(1,0,0,0,  0,1,0,1,  0,0,-1,-5,  0,0,0,1);
	TEST(vec3 t = pm_DecomposeTranslation(op1));
	CHECK_TEST2(pm_IsEqual(t, pm_Set(0, 1, -5)), op1, t, pm_Set(0, 1, -5));
	TEST(quat r = pm_DecomposeRotation(op1));
	CHECK_TEST2(pm_IsEqual(r, pm_IdentityQuat()), op1, r, pm_IdentityQuat());
	TEST(vec3 s = pm_DecomposeScale(op1));
	CHECK_TEST2(pm_IsEqual(s, pm_Set(1,1,-1)), op1, s, pm_Set(1,1,-1));
	END_TEST()

		START_TEST("Matrix + Matrix");
	TEST(mat4 r = pm_Add(testOp1, testOp2));
	CHECK_TEST3(pm_IsEqual(r, pm_Create(3,2,1,4,4,5,2,1,2,2,5,4,4,2,4,2)), testOp1, testOp2, r, pm_Create(3,2,1,4,4,5,2,1,2,2,5,4,4,2,4,2));
	END_TEST()

		START_TEST("Matrix - Matrix");
	TEST(mat4 r = pm_Subtract(testOp1, testOp2));
	CHECK_TEST3(pm_IsEqual(r, pm_Create(-3,0,-1,0,0,-5,0,-1,-2,0,-3,0,0,-2,-2,0)), testOp1, testOp2, r, pm_Create(-3,0,-1,0,0,-5,0,-1,-2,0,-3,0,0,-2,-2,0));
	END_TEST()

		START_TEST("Matrix * Matrix");
	TEST(mat4 r = pm_Multiply(testOp1, testOp2));
	CHECK_TEST3(pm_IsEqual(r, pm_Create(0, 1, 0, 4, 4, 0, 1, 0, 0, 1, 4, 4, 4, 0, 3, 1)), testOp1, testOp2, r, pm_Create(0, 1, 0, 4, 4, 0, 1, 0, 0, 1, 4, 4, 4, 0, 3, 1));
	END_TEST()

		START_TEST("Matrix / Matrix");
	TEST(mat4 r = pm_Divide(testOp1, testOp2));
	CHECK_TEST3(pm_IsEqual(r, pm_Create(0,1,0,1,1,0,1,0,0,1,0.25,1,1,0,1/3.0,1)), testOp1, testOp2, r, pm_Create(0,1,0,1,1,0,1,0,0,1,0.25,1,1,0,1/3.0,1));
	END_TEST()

		START_TEST("Matrix * f");
	TEST(mat4 r = pm_Scale(testOp1, 2));
	CHECK_TEST3(pm_IsEqual(r, pm_Create(0,2,0,4,4,0,2,0,0,2,2,4,4,0,2,2)), testOp1, testOp2, r, pm_Create(0,2,0,4,4,0,2,0,0,2,2,4,4,0,2,2));
	END_TEST()

		START_TEST("Matrix x Matrix");
	TEST(mat4 r = pm_Product(testOp1, testOp2));
	CHECK_TEST3(pm_IsEqual(r, pm_Create(6,9,7,3,8,3,6,6,8,10,11,5,10,5,9,7)), testOp1, testOp2, r, pm_Create(6,9,7,3,8,3,6,6,8,10,11,5,10,5,9,7));
	END_TEST()

		START_TEST("Matrix x Vector");
	TEST(vec4 r = pm_Product(testOp1, pm_Set(0,0,1,1)));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(2, 1, 3, 2)), testOp1, pm_Set(0, 0, 1, 1), r, pm_Set(2, 1, 3, 2));
	END_TEST()

		START_TEST("Transpose Matrix");
	TEST(mat4 r = pm_Transpose(testOp1));
	CHECK_TEST2(pm_IsEqual(r, pm_Create(0,2,0,2,1,0,1,0,0,1,1,1,2,0,2,1)), testOp1, r, pm_Create(0,2,0,2,1,0,1,0,0,1,1,1,2,0,2,1));
	END_TEST()

		START_TEST("Determinant Matrix");
	TEST(float r = pm_Determinant(testOp1));
	CHECK_TEST2(r == -2, testOp1, r, -2);
	END_TEST()

		START_TEST("Inverse Matrix");
	TEST(mat4 r = pm_Inverse(testOp1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Create(0.5,0.5,-0.5,0,1,2,0,-2,-1,0,1,0,0,-1,0,1), ABSOLUTE_ERROR),
		testOp1, r, pm_Create(0.5,0.5,-0.5,0,1,2,0,-2,-1,0,1,0,0,-1,0,1));
	END_TEST()

		START_TEST("Perspective Projection Matrix");
	TEST(mat4 r = pm_Perspective(1, 1, 1, 10.0f));
	CHECK_TEST1(pm_IsNearlyEqual(r, pm_Create(2, 0, 0, 0, 0, 2, 0, 0, 0, 0, -1.1111f, -1, 0, 0, -1.1111f, 0), ABSOLUTE_ERROR), r, pm_Create(2, 0, 0, 0, 0, 2, 0, 0, 0, 0, -1.1111f, -1, 0, 0, -1.1111f, 0));
	END_TEST()

		START_TEST("Orthographic Projection Matrix");
	TEST(mat4 r = pm_Orthographic(1, 1, 1, 10.0f));
	CHECK_TEST1(pm_IsNearlyEqual(r, pm_Create(2, 0, 0, 0, 0, 2, 0, 0, 0, 0, -0.1111f, 0, 0, 0, -0.1111f, 1), ABSOLUTE_ERROR), r, pm_Create(2, 0, 0, 0, 0, 2, 0, 0, 0, 0, -0.1111f, 0, 0, 0, -0.1111f, 1));
	END_TEST()

		return TEST_UNIT_RESULT;
}