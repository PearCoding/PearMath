#include "test.h"

bool vectorTest();

using namespace PM;

int main()
{
	printf("------------------------------------------------------------------------\n");
	printf("Tests for the math library PearMath. [vector]\n");
	printf("------------------------------------------------------------------------\n");
#ifdef PM_USE_SIMD
	printf("SIMD support: true\n");
#else
	printf("SIMD support: false\n");
#endif
	printf("------------------------------------------------------------------------\n");

	bool ok = vectorTest();

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

bool vectorTest()
{
	INIT_TEST_UNIT("Vector");

	const vec2 top1 = pm_Set(10, 20);

	START_TEST("(GET) Vector.X")
	TEST(float var = pm_GetX(top1));
	CHECK_TEST2(var == 10, top1, var, 10)
		END_TEST()

		START_TEST("(GET) Vector.Y")
		vec2 v = pm_Set(20, 30);
	TEST(float var = pm_GetY(v));
	CHECK_TEST2(var == 30, v, var, 30)
		END_TEST()

		START_TEST("(SET) Vector.X");
		vec2 v = pm_Set(20, 30);
	TEST(v = pm_SetX(v, 10));
	CHECK_TEST2(pm_GetX(v) == 10, v, pm_GetX(v), 10)
		END_TEST()

		START_TEST("(SET) Vector.Y");
		vec2 v = pm_Set(20, 30);
	TEST(v = pm_SetY(v, 20));
	CHECK_TEST2(pm_GetY(v) == 20, v, pm_GetY(v), 20)
		END_TEST()

		START_TEST("Vector + Vector");
	vec2 op2 = pm_Set(40, 30);
	TEST(vec2 r = pm_Add(top1, op2));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(50, 50)), top1, op2, r, pm_Set(50, 50))
		END_TEST()

		START_TEST("Vector - Vector");
	vec2 op1 = pm_Set(50, 60);
	vec2 op2 = pm_Set(60, 20);
	TEST(vec2 r = pm_Subtract(op1, op2));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(-10, 40)), op1, op2, r, pm_Set(-10, 40))
		END_TEST()

		START_TEST("Vector * float");
	TEST(vec2 r = pm_Scale(top1, 60));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(600, 1200)), top1, 60, r, pm_Set(600, 1200))
		END_TEST()

		START_TEST("Vector * Vector");
	vec2 op1 = pm_Set(50, 60);
	vec2 op2 = pm_Set(60, 20);
	TEST(vec2 r = pm_Multiply(op1, op2));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(3000, 1200)), op1, op2, r, pm_Set(3000, 1200))
		END_TEST()

		START_TEST("Vector / Vector");
	TEST(vec2 r = pm_Divide(top1, pm_Set(1, 2)));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(10,10)), top1, 5, r, pm_Set(10,10))
		END_TEST()

		START_TEST("Reciprocal Vector");
	TEST(vec2 r = pm_Reciprocal(top1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(0.1f, 0.05f), ABSOLUTE_ERROR), top1, r, pm_Set(0.1f, 0.05f))
		END_TEST()

	
		START_TEST("SQRT Vector");
	TEST(vec2 r = pm_Sqrt(top1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(3.16227766016838f, 4.47213595499958f), ABSOLUTE_ERROR),
		top1, r, pm_Set(3.16227766016838f, 4.47213595499958f))
		END_TEST()

		START_TEST("Reciprocal SQRT Vector");
	TEST(vec2 r = pm_ReciprocalSqrt(top1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(0.31622776601684f, 0.22360679774998f), ABSOLUTE_ERROR),
		top1, r, pm_Set(0.31622776601684f, 0.22360679774998f))
		END_TEST()

		START_TEST("Vector ^ Vector");
	vec2 op2 = pm_Set(1, 2);
	TEST(vec2 r = pm_Pow(top1, op2));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(10, 400)), top1, op2, r, pm_Set(10, 400))
		END_TEST()

		START_TEST("Exp Vector");
	TEST(vec2 r = pm_Exp(top1));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(22026.46579480672f, 4.8516519540979028e8f)),
		top1, r, pm_Set(22026.46579480672f, 4.8516519540979028e8f))
		END_TEST()

		START_TEST("Log Vector");
	TEST(vec2 r = pm_Log(top1));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(2.302585092994046f, 2.995732273553991f)),
		top1, r, pm_Set(2.302585092994046f, 2.995732273553991f))
		END_TEST()

		START_TEST("Sin Vector");
	vec2 op1 = pm_Set(2.5, -2.5);
	TEST(vec2 r = pm_Sin(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(0.59847214410396f, -0.59847214410396f), ABSOLUTE_ERROR),
		op1, r, pm_Set(0.59847214410396f, -0.59847214410396f))
		END_TEST()

		START_TEST("Cos Vector");
	vec2 op1 = pm_Set(2.5, -2.5);
	TEST(vec2 r = pm_Cos(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(-0.80114361554693f, -0.80114361554693f), ABSOLUTE_ERROR),
		op1, r, pm_Set(-0.80114361554693f, -0.80114361554693f))
		END_TEST()

		START_TEST("Tan Vector");
	vec2 op1 = pm_Set(0.1, 0);
	TEST(vec2 r = pm_Tan(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(0.10033467208545f, 0), ABSOLUTE_ERROR),
		op1, r, pm_Set(0.10033467208545f, 0))
		END_TEST()

		START_TEST("ASin Vector");
	vec2 op1 = pm_Set(1, 0);
	TEST(vec2 r = pm_ASin(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(1.570796326794897f, 0), ABSOLUTE_ERROR),
		op1, r, pm_Set(1.570796326794897f, 0))
		END_TEST()

		START_TEST("ACos Vector");
	vec2 op1 = pm_Set(1, 0);
	TEST(vec2 r = pm_ACos(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(0, 1.570796326794897f), ABSOLUTE_ERROR),
		op1, r, pm_Set(0, 1.570796326794897f))
		END_TEST()

		START_TEST("ATan Vector");
	vec2 op1 = pm_Set(PM_PI_F, 1);
	TEST(vec2 r = pm_ATan(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(1.262627255678912f, 0.78539816339745f), ABSOLUTE_ERROR),
		op1, r, pm_Set(1.262627255678912f, 0.78539816339745f))
		END_TEST()

		START_TEST("LERP Vector");
	vec2 op1 = pm_Set(20, 0);
	vec2 op2 = pm_Set(40, 20);
	TEST(vec2 r = pm_Lerp(op1, op2, pm_FillVector2D(0.5f)));
	CHECK_TEST3(pm_IsNearlyEqual(r, pm_Set(30, 10), ABSOLUTE_ERROR), op1, op2, r, pm_Set(30, 10))
		END_TEST()

		START_TEST("NLERP Vector");
	vec2 op1 = pm_Set(20, 0);
	vec2 op2 = pm_Set(40, 20);
	TEST(vec2 r = pm_NLerp(op1, op2, pm_FillVector2D(0.5f)));
	CHECK_TEST3(pm_IsNearlyEqual(r, pm_Set(0.94417967980259f, 0.3147265599342f), ABSOLUTE_ERROR),
		op1, op2, r, pm_Set(0.94417967980259f, 0.3147265599342f))
		END_TEST()

		START_TEST("SLERP Vector");
	vec2 op1 = pm_Set(0.66666666666667f, 0);
	vec2 op2 = pm_Set(0.27735009811261f, 0.55470019622523f);
	TEST(vec2 r = pm_SLerp(op1, op2, pm_FillVector2D(0.5f)));
	CHECK_TEST3(pm_IsNearlyEqual(r, pm_Set(0.50610424149908f, 0.29738467847613f), ABSOLUTE_ERROR),
		op1, op2, r, pm_Set(0.50610424149908f, 0.29738467847613f))
		END_TEST()

		START_TEST("Saturate Vector");
	vec2 op1 = pm_Set(20, 0);
	TEST(vec2 r = pm_Saturate(op1));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(1, 0)), op1, r, pm_Set(1, 0))
		END_TEST()

		START_TEST("Dot");
	vec2 op1 = pm_Set(20, 0);
	vec2 op2 = pm_Set(20, 5);
	TEST(float r = pm_Dot(op1, op2));
	CHECK_TEST3(r == 400, op1, op2, r, 400)
		END_TEST()

		START_TEST("Magnitude");
	vec2 op1 = pm_Set(1, 0);
	TEST(float r = pm_Magnitude(op1));
	CHECK_TEST2(r == 1, op1, r, 1)
		END_TEST()

		START_TEST("Magnitude Squared");
	vec2 op1 = pm_Set(2, 0);
	TEST(float r = pm_MagnitudeSqr(op1));
	CHECK_TEST2(r == 4, op1, r, 4)
		END_TEST()

		START_TEST("Normalize");
	vec2 op1 = pm_Set(0, 20);
	TEST(vec2 r = pm_Normalize(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(0, 1), ABSOLUTE_ERROR), op1, r, pm_Set(0, 1))
		END_TEST()

		START_TEST("Quality Normalize");
	vec2 op1 = pm_Set(0, 20);
	TEST(vec2 r = pm_QualityNormalize(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(0, 1), ABSOLUTE_ERROR), op1, r, pm_Set(0, 1))
		END_TEST()

		START_TEST("Fast Normalize");
	vec2 op1 = pm_Set(0, 20);
	TEST(vec2 r = pm_FastNormalize(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(0, 1), ABSOLUTE_ERROR), op1, r, pm_Set(0, 1))
		END_TEST()

		return TEST_UNIT_RESULT;
}