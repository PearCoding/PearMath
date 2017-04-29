#include "test.h"

bool vectorTest();

using namespace PM;

int main()
{
	printf("------------------------------------------------------------------------\n");
	printf("Tests for the math library PearMath. [vector]\n");
	printf("------------------------------------------------------------------------\n");
#ifdef PM_WITH_SIMD
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

	START_TEST("(GET) Vector.X")
		vec3 v = pm_Set(10, 20, 30);
	TEST(float var = pm_GetX(v));
	CHECK_TEST2(var == 10, v, var, 10)
		END_TEST()

		START_TEST("(GET) Vector.Y")
		vec3 v = pm_Set(20, 30, 40);
	TEST(float var = pm_GetY(v));
	CHECK_TEST2(var == 30, v, var, 30)
		END_TEST()

		START_TEST("(GET) Vector.Z");
	vec3 v = pm_Set(30, 40, 50);
	TEST(float var = pm_GetZ(v));
	CHECK_TEST2(var == 50, v, var, 50)
		END_TEST()

		START_TEST("(SET) Vector.X");
	vec3 v = pm_Set(30, 40, 50);
	TEST(vec3 r = pm_SetX(v, 10));
	CHECK_TEST2(pm_GetX(r) == 10, v, pm_GetX(r), 10)
		END_TEST()

		START_TEST("(SET) Vector.Y");
	vec3 v = pm_Set(30, 40, 50);
	TEST(vec3 r = pm_SetY(v, 20));
	CHECK_TEST2(pm_GetY(r) == 20, v, pm_GetY(r), 20)
		END_TEST()

		START_TEST("(SET) Vector.Z");
	vec3 v = pm_Set(30, 40, 50);
	TEST(vec3 r = pm_SetZ(v, 30));
	CHECK_TEST2(pm_GetZ(r) == 30, v, pm_GetZ(r), 30)
		END_TEST()

	const vec3 top1 = pm_Set(10,20,30);
	START_TEST("Equal Vector")
	TEST(bool b = ALL_EQ(top1,top1));
	CHECK_TEST3(b, top1, top1, b, true)
		END_TEST()

	START_TEST("Not Equal Vector")
	vec3 v = pm_Set(30, 40, 50);
	TEST(bool b = pm_IsSomeTrue(pm_IsNotEqual(top1,v)));
	CHECK_TEST3(b, top1, v, b, true)
		END_TEST()

	START_TEST("Nearly Equal Vector")
		vec3 v = pm_Set(10.000001f, 20.000001f, 30.000001f);
	TEST(bool b = ALL_NEARLY_EQ(top1,v,0.0001f));
	CHECK_TEST3(b, top1, v, b, true)
		END_TEST()

	START_TEST("Greater Vector")
	vec3 v = pm_Set(30, 40, 50);
	TEST(bool b = pm_IsAllTrue(pm_IsGreater(v, top1)));
	CHECK_TEST3(b, v, top1, b, true)
		END_TEST()

	START_TEST("Greater or Equal Vector")
	vec3 v = pm_Set(30, 40, 50);
	TEST(bool b = pm_IsAllTrue(pm_IsGreaterOrEqual(v, top1)));
	CHECK_TEST3(b, v, top1, b, true)
		END_TEST()

	START_TEST("Less Vector")
	vec3 v = pm_Set(30, 40, 50);
	TEST(bool b = pm_IsAllTrue(pm_IsLess(top1,v)));
	CHECK_TEST3(b, top1, v, b, true)
		END_TEST()

	START_TEST("Less or Equal Vector")
	vec3 v = pm_Set(30, 40, 50);
	TEST(bool b = pm_IsAllTrue(pm_IsLessOrEqual(top1, v)));
	CHECK_TEST3(b, top1, v, b, true)
		END_TEST()

		START_TEST("Vector + Vector");
	vec3 op1 = pm_Set(10, 20, 30);
	vec3 op2 = pm_Set(40, 30, 20);
	TEST(vec3 r = pm_Add(op1, op2));
	CHECK_TEST3(ALL_EQ(r, pm_Set(50, 50, 50)), op1, op2, r, pm_Set(50, 50, 50))
		END_TEST()

		START_TEST("Vector - Vector");
	vec3 op1 = pm_Set(50, 60, 70);
	vec3 op2 = pm_Set(60, 20, 10);
	TEST(vec3 r = pm_Subtract(op1, op2));
	CHECK_TEST3(ALL_EQ(r, pm_Set(-10, 40, 60)), op1, op2, r, pm_Set(-10, 40, 60))
		END_TEST()

		START_TEST("Vector * float");
	vec3 op1 = pm_Set(10, 20, 30);
	TEST(vec3 r = pm_Scale(op1, 60));
	CHECK_TEST3(ALL_EQ(r, pm_Set(600, 1200, 1800)), op1, 60, r, pm_Set(600, 1200, 1800))
		END_TEST()

		START_TEST("Vector * Vector");
	vec3 op1 = pm_Set(50, 60, 70);
	vec3 op2 = pm_Set(60, 20, 10);
	TEST(vec3 r = pm_Multiply(op1, op2));
	CHECK_TEST3(ALL_EQ(r, pm_Set(3000, 1200, 700)), op1, op2, r, pm_Set(3000, 1200, 700))
		END_TEST()

		START_TEST("Vector / Vector");
	vec3 op1 = pm_Set(50, 60, 70);
	vec3 op2 = pm_Set(2, 2, 2);
	TEST(vec3 r = pm_Divide(op1, op2));
	CHECK_TEST3(ALL_EQ(r, pm_Set(25, 30, 35)), op1, op2, r, pm_Set(25, 30, 35))
		END_TEST()

		START_TEST("Reciprocal Vector");
	vec3 op1 = pm_Set(10, 20, 40);
	TEST(vec3 r = pm_Reciprocal(op1));
	CHECK_TEST2(ALL_NEARLY_EQ(r, pm_Set(0.1f, 0.05f, 0.025f), ABSOLUTE_ERROR), op1, r, pm_Set(0.1f, 0.05f, 0.025f))
		END_TEST()

		START_TEST("SQRT Vector");
	vec3 op1 = pm_Set(10, 20, 40);
	TEST(vec3 r = pm_Sqrt(op1));
	CHECK_TEST2(ALL_NEARLY_EQ(r, pm_Set(3.16227766016838f, 4.47213595499958f, 6.324555320336759f), ABSOLUTE_ERROR),
		op1, r, pm_Set(3.16227766016838f, 4.47213595499958f, 6.324555320336759f))
		END_TEST()

		START_TEST("Reciprocal SQRT Vector");
	vec3 op1 = pm_Set(10, 20, 40);
	TEST(vec3 r = pm_ReciprocalSqrt(op1));
	CHECK_TEST2(ALL_NEARLY_EQ(r, pm_Set(0.31622776601684f, 0.22360679774998f, 0.15811388300842f), ABSOLUTE_ERROR),
		op1, r, pm_Set(0.31622776601684f, 0.22360679774998f, 0.15811388300842f))
		END_TEST()

		START_TEST("Vector ^ Vector");
	vec3 op1 = pm_Set(10, 20, 40);
	vec3 op2 = pm_Set(1, 2, 4);
	TEST(vec3 r = pm_Pow(op1, op2));
	CHECK_TEST3(ALL_EQ(r, pm_Set(10, 400, 2560000)), op1, op2, r, pm_Set(10, 400, 2560000))
		END_TEST()

		START_TEST("Exp Vector");
	vec3 op1 = pm_Set(10, 20, 40);
	TEST(vec3 r = pm_Exp(op1));
	CHECK_TEST2(ALL_EQ(r, pm_Set(22026.46579480672f, 4.8516519540979028e8f, 2.3538526683702e17f)),
		op1, r, pm_Set(22026.46579480672f, 4.8516519540979028e8f, 2.3538526683702e17f))
		END_TEST()

		START_TEST("Log Vector");
	vec3 op1 = pm_Set(10, 20, 40);
	TEST(vec3 r = pm_Log(op1));
	CHECK_TEST2(ALL_EQ(r, pm_Set(2.302585092994046f, 2.995732273553991f, 3.688879454113936f)),
		op1, r, pm_Set(2.302585092994046f, 2.995732273553991f, 3.688879454113936f))
		END_TEST()

		START_TEST("Sin Vector");
	vec3 op1 = pm_Set(2.5, -2.5, 3.1);
	TEST(vec3 r = pm_Sin(op1));
	CHECK_TEST2(ALL_NEARLY_EQ(r, pm_Set(0.59847214410396f, -0.59847214410396f, 0.04158066243329f), ABSOLUTE_ERROR),
		op1, r, pm_Set(0.59847214410396f, -0.59847214410396f, 0.04158066243329f))
		END_TEST()

		START_TEST("Cos Vector");
	vec3 op1 = pm_Set(2.5, -2.5, 3.1);
	TEST(vec3 r = pm_Cos(op1));
	CHECK_TEST2(ALL_NEARLY_EQ(r, pm_Set(-0.80114361554693f, -0.80114361554693f, -0.99913515027328f), ABSOLUTE_ERROR),
		op1, r, pm_Set(-0.80114361554693f, -0.80114361554693f, -0.99913515027328f))
		END_TEST()

		START_TEST("Tan Vector");
	vec3 op1 = pm_Set(0.1, 0, 1.0);
	TEST(vec3 r = pm_Tan(op1));
	CHECK_TEST2(ALL_NEARLY_EQ(r, pm_Set(0.10033467208545f, 0, 1.557407724654902f), ABSOLUTE_ERROR),
		op1, r, pm_Set(0.10033467208545f, 0, 1.557407724654902f))
		END_TEST()

		START_TEST("ASin Vector");
	vec3 op1 = pm_Set(1, 0, 0.5);
	TEST(vec3 r = pm_ASin(op1));
	CHECK_TEST2(ALL_NEARLY_EQ(r, pm_Set(1.570796326794897f, 0, 0.5235987755983f), ABSOLUTE_ERROR),
		op1, r, pm_Set(1.570796326794897f, 0, 0.5235987755983f))
		END_TEST()

		START_TEST("ACos Vector");
	vec3 op1 = pm_Set(1, 0, 0.5);
	TEST(vec3 r = pm_ACos(op1));
	CHECK_TEST2(ALL_NEARLY_EQ(r, pm_Set(0, 1.570796326794897f, 1.047197551196598f), ABSOLUTE_ERROR),
		op1, r, pm_Set(0, 1.570796326794897f, 1.047197551196598f))
		END_TEST()

		START_TEST("ATan Vector");
	vec3 op1 = pm_Set(PM_PI_F, 1, 2);
	TEST(vec3 r = pm_ATan(op1));
	CHECK_TEST2(ALL_NEARLY_EQ(r, pm_Set(1.262627255678912f, 0.78539816339745f, 1.10714871779409f), ABSOLUTE_ERROR),
		op1, r, pm_Set(1.262627255678912f, 0.78539816339745f, 1.10714871779409f))
		END_TEST()

		START_TEST("LERP Vector");
	vec3 op1 = pm_Set(20, 0, 2.0);
	vec3 op2 = pm_Set(40, 20, 4.0);
	TEST(vec3 r = pm_Lerp(op1, op2, pm_FillVector3D(0.5f)));
	CHECK_TEST3(ALL_NEARLY_EQ(r, pm_Set(30, 10, 3.0), ABSOLUTE_ERROR), op1, op2, r, pm_Set(30, 10, 3.0))
		END_TEST()

		START_TEST("NLERP Vector");
	vec3 op1 = pm_Set(20, 0, 2.0);
	vec3 op2 = pm_Set(40, 20, 4.0);
	TEST(vec3 r = pm_NLerp(op1, op2, pm_FillVector3D(0.5f)));
	CHECK_TEST3(ALL_NEARLY_EQ(r, pm_Set(0.94417967980259f, 0.3147265599342f, 0.094417967980259f), ABSOLUTE_ERROR),
		op1, op2, r, pm_Set(0.94417967980259f, 0.3147265599342f, 0.094417967980259f))
		END_TEST()

		START_TEST("SLERP Vector");
	vec3 op1 = pm_Set(0.66666666666667f, 0, 0.66666666666667f);
	vec3 op2 = pm_Set(0.27735009811261f, 0.55470019622523f, 0.55470019622523f);
	TEST(vec3 r = pm_SLerp(op1, op2, pm_FillVector3D(0.5f)));
	CHECK_TEST3(ALL_NEARLY_EQ(r, pm_Set(0.50610424149908f, 0.29738467847613f, 0.65479658073714f), ABSOLUTE_ERROR),
		op1, op2, r, pm_Set(0.50610424149908f, 0.29738467847613f, 0.65479658073714f))
		END_TEST()

		START_TEST("Saturate Vector");
	vec3 op1 = pm_Set(20, 0, 2.0);
	TEST(vec3 r = pm_Saturate(op1));
	CHECK_TEST2(ALL_EQ(r, pm_Set(1, 0, 1)), op1, r, pm_Set(1, 0, 1))
		END_TEST()

		START_TEST("Dot");
	vec3 op1 = pm_Set(20, 0, 2.0);
	vec3 op2 = pm_Set(20, 5, 10.0);
	TEST(float r = pm_Dot(op1, op2));
	CHECK_TEST3(r == 420, op1, op2, r, 420)
		END_TEST()

		START_TEST("Cross Vector3D");
	vec3 op1 = pm_Set(1, 0, 2);
	vec3 op2 = pm_Set(2, 3, 5);
	TEST(vec3 r = pm_Cross(op1, op2));
	CHECK_TEST3(ALL_NEARLY_EQ(r, pm_Set(-6, -1, 3), ABSOLUTE_ERROR), op1, op2, r, pm_Set(-6, -1, 3))
		END_TEST()

		START_TEST("Magnitude");
	vec3 op1 = pm_Set(1, 0, 0);
	TEST(float r = pm_Magnitude(op1));
	CHECK_TEST2(r == 1, op1, r, 1)
		END_TEST()

		START_TEST("Magnitude Squared");
	vec3 op1 = pm_Set(2, 0, 0);
	TEST(float r = pm_MagnitudeSqr(op1));
	CHECK_TEST2(r == 4, op1, r, 4)
		END_TEST()

		START_TEST("Normalize");
	vec3 op1 = pm_Set(20, 0, 0);
	TEST(vec3 r = pm_Normalize(op1));
	CHECK_TEST2(ALL_NEARLY_EQ(r, pm_Set(1, 0, 0), ABSOLUTE_ERROR), op1, r, pm_Set(1, 0, 0))
		END_TEST()

		START_TEST("Quality Normalize");
	vec3 op1 = pm_Set(20, 0, 0);
	TEST(vec3 r = pm_QualityNormalize(op1));
	CHECK_TEST2(ALL_NEARLY_EQ(r, pm_Set(1, 0, 0), ABSOLUTE_ERROR), op1, r, pm_Set(1, 0, 0))
		END_TEST()

		START_TEST("Fast Normalize");
	vec3 op1 = pm_Set(20, 0, 0);
	TEST(vec3 r = pm_FastNormalize(op1));
	CHECK_TEST2(ALL_NEARLY_EQ(r, pm_Set(1, 0, 0), ABSOLUTE_ERROR), op1, r, pm_Set(1, 0, 0))
		END_TEST()

		return TEST_UNIT_RESULT;
}