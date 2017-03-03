//#define PM_NO_SIMD
#include "test.h"

bool frameTest();

using namespace PM;

int main()
{
	printf("------------------------------------------------------------------------\n");
	printf("Tests for the math library PearMath [frame].\n");
	printf("------------------------------------------------------------------------\n");
#ifdef PM_USE_SIMD
	printf("SIMD support: true\n");
#else
	printf("SIMD support: false\n");
#endif
	printf("------------------------------------------------------------------------\n");

	bool ok = frameTest();

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

bool frameTest()
{
	INIT_TEST_UNIT("Frame");

	frame f;
	f.Up = pm_Set(0, 1, 1);
	f.Forward = pm_Set(1, 0, 1);
	f.Origin = pm_Set(5, 7, 3);

	START_TEST("(GET) Frame.XAxis");
	TEST(vec3 r = pm_GetXAxis(f));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(1, 1, -1)), f, r, pm_Set(1, 1, -1))
		END_TEST()

		START_TEST("(GET) Frame.YAxis");
	TEST(vec3 r = pm_GetYAxis(f));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(0, 1, 1)), f, r, pm_Set(0, 1, 1))
		END_TEST()

		START_TEST("(GET) Frame.ZAxis");
	TEST(vec3 r = pm_GetZAxis(f));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(1, 0, 1)), f, r, pm_Set(1, 0, 1))
		END_TEST()

		START_TEST("Normalize Frame");
	TEST(frame r = pm_Normalize(f));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(pm_Set(5, 7, 3), pm_Set(0, 0.7071067912f, 0.7071067912f), pm_Set(0.816528f, -0.408264f, 0.408264f)), ABSOLUTE_ERROR),
		f, r, pm_Set(pm_Set(5, 7, 3), pm_Set(0, 0.7071067912f, 0.7071067912f), pm_Set(0.816528f, -0.408264f, 0.408264f)))
		END_TEST()

		START_TEST("Translate World Frame");
	TEST(frame r = pm_TranslateWorld(f, pm_Set(5, 2, 6)));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(pm_Set(10, 9, 9), pm_Set(0, 1, 1), pm_Set(1, 0, 1))), f, r, pm_Set(pm_Set(10, 9, 9), pm_Set(0, 1, 1), pm_Set(1, 0, 1)))
		END_TEST()

		START_TEST("Translate Local Frame");
	TEST(frame r = pm_TranslateLocal(f, pm_Set(5, 2, 6)));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(pm_Set(16, 14, 6), pm_Set(0, 1, 1), pm_Set(1, 0, 1))), f, r, pm_Set(pm_Set(16, 14, 6), pm_Set(0, 1, 1), pm_Set(1, 0, 1)))
		END_TEST()

		START_TEST("Move Forward Frame");
	TEST(frame r = pm_MoveForward(f, 2));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(pm_Set(7, 7, 5), pm_Set(0, 1, 1), pm_Set(1, 0, 1))), f, r, pm_Set(pm_Set(7, 7, 5), pm_Set(0, 1, 1), pm_Set(1, 0, 1)))
		END_TEST()

		START_TEST("Move Backward Frame");
	TEST(frame r = pm_MoveBackward(f, 2));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(pm_Set(3, 7, 1), pm_Set(0, 1, 1), pm_Set(1, 0, 1))), f, r, pm_Set(pm_Set(3, 7, 1), pm_Set(0, 1, 1), pm_Set(1, 0, 1)))
		END_TEST()

		START_TEST("Move Right Frame");
	TEST(frame r = pm_MoveRight(f, 2));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(pm_Set(7, 9, 1), pm_Set(0, 1, 1), pm_Set(1, 0, 1))), f, r, pm_Set(pm_Set(7, 9, 1), pm_Set(0, 1, 1), pm_Set(1, 0, 1)))
		END_TEST()

		START_TEST("Move Left Frame");
	TEST(frame r = pm_MoveLeft(f, 2));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(pm_Set(3, 5, 5), pm_Set(0, 1, 1), pm_Set(1, 0, 1))), f, r, pm_Set(pm_Set(3, 5, 5), pm_Set(0, 1, 1), pm_Set(1, 0, 1)))
		END_TEST()

		START_TEST("Move Up Frame");
	TEST(frame r = pm_MoveUp(f, 2));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(pm_Set(5, 9, 5), pm_Set(0, 1, 1), pm_Set(1, 0, 1))), f, r, pm_Set(pm_Set(5, 9, 5), pm_Set(0, 1, 1), pm_Set(1, 0, 1)))
		END_TEST()

		START_TEST("Move Down Frame");
	TEST(frame r = pm_MoveDown(f, 2));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(pm_Set(5, 5, 1), pm_Set(0, 1, 1), pm_Set(1, 0, 1))), f, r, pm_Set(pm_Set(5, 5, 1), pm_Set(0, 1, 1), pm_Set(1, 0, 1)))
		END_TEST()

		START_TEST("Matrix Frame");
	TEST(mat4 r = pm_ToMatrix(f));
	CHECK_TEST2(pm_IsEqual(r, pm_Create(
		1, 1, -1, 5,
		0, 1, 1, 7,
		1, 0, 1, 3,
		0, 0, 0, 1)), f, r,
		pm_Create(
		1, 1, -1, 5,
		0, 1, 1, 7,
		1, 0, 1, 3,
		0, 0, 0, 1));
	END_TEST()

		START_TEST("Camera Matrix Frame");
	TEST(mat4 r = pm_ToCameraMatrix(f));
	CHECK_TEST2(pm_IsEqual(r, pm_Create(
		-1, -1, 1, -5,
		0, 1, 1, -7,
		-1, 0, -1, -3,
		0, 0, 0, 1)), f, r,
		pm_Create(
		-1, -1, 1, -5,
		0, 1, 1, -7,
		-1, 0, -1, -3,
		0, 0, 0, 1));
	END_TEST()

	f.Up = pm_Set(0, 1, 0);
	f.Forward = pm_Set(0, 0, 1);
	START_TEST("Rotate Local X Frame");
	TEST(frame r = pm_RotateLocalX(f, pm_DegToRad(90)));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(pm_Set(5, 7, 3), pm_Set(0, 0, -1), pm_Set(0, 1, 0)), ABSOLUTE_ERROR),
		f, r, pm_Set(pm_Set(5, 7, 3), pm_Set(0, 0, -1), pm_Set(0, 1, 0)))
		END_TEST()

		START_TEST("Rotate Local Y Frame");
	TEST(frame r = pm_RotateLocalY(f, pm_DegToRad(90)));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(pm_Set(5, 7, 3), pm_Set(0, 1, 0), pm_Set(-1, 0, 0)), ABSOLUTE_ERROR),
		f, r, pm_Set(pm_Set(5, 7, 3), pm_Set(0, 1, 0), pm_Set(-1, 0, 0)))
		END_TEST()

		START_TEST("Rotate Local Z Frame");
	TEST(frame r = pm_RotateLocalZ(f, pm_DegToRad(90)));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(pm_Set(5, 7, 3), pm_Set(1, 0, 0), pm_Set(0, 0, 1)), ABSOLUTE_ERROR),
		f, r, pm_Set(pm_Set(5, 7, 3), pm_Set(1, 0, 0), pm_Set(0, 0, 1)))
		END_TEST()

		//TODO: Add test for other rotations...
		return TEST_UNIT_RESULT;
}