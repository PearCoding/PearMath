//#define PM_NO_SIMD
#include "PearMath.h"
#include "Rect.h"
#include "Region.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_TEST_UNIT(str) \
	printf("%s Tests>>\n", str); \
	int ut_Count = 0; \
	int ut_I; \
	float ut_LastTime; \
	bool ut_Failure = false;

#define START_TEST(str) \
	ut_LastTime = 0.0f; \
	printf("\t%d: %s\n", ++ut_Count, str); \
for (ut_I = 0; ut_I < TEST_LOOP_COUNT; ++ut_I) \
	{

#define TEST(f) \
	f;

#define CHECK_TEST3(val, a1, a2, a3) \
if (!(val)) \
	{ \
	printf("\t\tTest failed:\n\t\t\tOp1: %s\n\t\t\tOp2: %s\n\t\t\tR: %s\n", to_str(a1, 0), to_str(a2, 1), to_str(a3, 2)); \
	ut_Failure = true; \
	break; \
	}

#define CHECK_TEST2(val, a1, a2) \
if (!(val)) \
	{ \
	printf("\t\tTest failed:\n\t\t\tOp1: %s\n\t\t\tR: %s\n", to_str(a1, 0), to_str(a2, 1)); \
	ut_Failure = true; \
	break; \
	}

#define CHECK_TEST1(val, a1) \
if (!(val)) \
	{ \
	printf("\t\tTest failed:\n\t\t\tR: %s\n", to_str(a1, 0)); \
	ut_Failure = true; \
	break; \
	}

#define END_TEST() \
	}

#define TEST_UNIT_RESULT !ut_Failure

#define ABSOLUTE_ERROR (0.0001f)
#define TEST_LOOP_COUNT (5000)

bool vectorTest();
bool matrixTest();
bool quaternionTest();
bool frameTest();

using namespace PM;

char str_buffer[3][1024];
const char* to_str(float f, int i)
{
	sprintf(str_buffer[i], "%f", f);
	return str_buffer[i];
}

const char* to_str(vec& v, int i)
{
	sprintf(str_buffer[i], "[%f, %f, %f, %f]", pm_GetX(v), pm_GetY(v), pm_GetZ(v), pm_GetW(v));
	return str_buffer[i];
}

const char* to_str(mat& m, int i)
{
	memset(str_buffer[i], 0, 1024);
	sprintf(str_buffer[i], "\n{\n\t%f, %f, %f, %f\n\t%f, %f, %f, %f\n\t%f, %f, %f, %f\n\t%f, %f, %f, %f\n}",
		m[0][0], m[0][1], m[0][2], m[0][3],
		m[1][0], m[1][1], m[1][2], m[1][3], 
		m[2][0], m[2][1], m[2][2], m[2][3],
		m[3][0], m[3][1], m[3][2], m[3][3]);
	return str_buffer[i];
}

const char* to_str(frame& f, int i)
{
	memset(str_buffer[i], 0, 1024);
	sprintf(str_buffer[i], "\n{\nUp:\t\t%f, %f, %f, %f\nForward:\t%f, %f, %f, %f\nOrigin:\t\t%f, %f, %f, %f\n}",
		pm_GetX(f.Up), pm_GetY(f.Up), pm_GetZ(f.Up), pm_GetW(f.Up),
		pm_GetX(f.Forward), pm_GetY(f.Forward), pm_GetZ(f.Forward), pm_GetW(f.Forward),
		pm_GetX(f.Origin), pm_GetY(f.Origin), pm_GetZ(f.Origin), pm_GetW(f.Origin));
	return str_buffer[i];
}

int main()
{
	printf("------------------------------------------------------------------------\n");
	printf("Tests for the math library PearMath.\n");
	printf("------------------------------------------------------------------------\n");
#ifdef PEAR3D_USE_SIMD
	printf("SIMD support: true\n");
#else
	printf("SIMD support: false\n");
#endif
	printf("------------------------------------------------------------------------\n");

	bool ok = true;
	if (!vectorTest())
	{
		ok = false;
	}

	printf("\n------------------------------------------------------------------------\n");
	if (!matrixTest())
	{
		ok = false;
	}

	printf("\n------------------------------------------------------------------------\n");
	if (!quaternionTest())
	{
		ok = false;
	}

	printf("\n------------------------------------------------------------------------\n");
	if (!frameTest())
	{
		ok = false;
	}

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
		vec v = pm_Set(10, 20, 30, 40);
	TEST(float var = pm_GetX(v));
	CHECK_TEST2(var == 10, v, 10)
		END_TEST()

		START_TEST("(GET) Vector.Y")
		vec v = pm_Set(20, 30, 40, 50);
	TEST(float var = pm_GetY(v));
	CHECK_TEST2(var == 30, v, 30)
		END_TEST()

		START_TEST("(GET) Vector.Z");
	vec v = pm_Set(30, 40, 50, 60);
	TEST(float var = pm_GetZ(v));
	CHECK_TEST2(var == 50, v, 50)
		END_TEST()

		START_TEST("(GET) Vector.W");
	vec v = pm_Set(40, 50, 60, 70);
	TEST(float var = pm_GetW(v));
	CHECK_TEST2(var == 70, v, 70)
		END_TEST()

		START_TEST("(SET) Vector.X");
	TEST(vec v = pm_SetX(v, 10));
	CHECK_TEST2(pm_GetX(v) == 10, v, 10)
	END_TEST()

		START_TEST("(SET) Vector.Y");
	TEST(vec v = pm_SetY(v, 20));
	CHECK_TEST2(pm_GetY(v) == 20, v, 20)
	END_TEST()

		START_TEST("(SET) Vector.Z");
	TEST(vec v = pm_SetZ(v, 30));
	CHECK_TEST2(pm_GetZ(v) == 30, v, 30)
	END_TEST()

		START_TEST("(SET) Vector.W");
	TEST(vec v = pm_SetW(v, 40));
	CHECK_TEST2(pm_GetW(v) == 40, v, 40)
	END_TEST()

		START_TEST("Vector + float");
	vec op1 = pm_Set(10, 20, 30, 40);
	TEST(vec r = pm_Add(op1, pm_FillVector(60)));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(70, 80, 90, 100)), op1, 60, r);
	END_TEST()

		START_TEST("Vector + Vector");
	vec op1 = pm_Set(10, 20, 30, 40);
	vec op2 = pm_Set(40, 30, 20, 10);
	TEST(vec r = pm_Add(op1, op2));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(50, 50, 50, 50)), op1, op2, r)
		END_TEST()

		START_TEST("Vector - float");
	vec op1 = pm_Set(10, 20, 30, 40);
	TEST(vec r = pm_Subtract(op1, pm_FillVector(60)));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(-50, -40, -30, -20)), op1, 60, r)
		END_TEST()

		START_TEST("Vector - Vector");
	vec op1 = pm_Set(50, 60, 70, 80);
	vec op2 = pm_Set(60, 20, 10, 40);
	TEST(vec r = pm_Subtract(op1, op2));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(-10, 40, 60, 40)), op1, op2, r)
		END_TEST()

		START_TEST("Vector * float");
	vec op1 = pm_Set(10, 20, 30, 40);
	TEST(vec r = pm_Scale(op1, 60));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(600, 1200, 1800, 2400)), op1, 60, r)
		END_TEST()

		START_TEST("Vector * Vector");
	vec op1 = pm_Set(50, 60, 70, 80);
	vec op2 = pm_Set(60, 20, 10, 40);
	TEST(vec r = pm_Multiply(op1, op2));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(3000, 1200, 700, 3200)), op1, op2, r)
		END_TEST()

		START_TEST("Vector / float");
	vec op1 = pm_Set(10, 20, 30, 40);
	TEST(vec r = pm_Divide(op1, pm_FillVector(5)));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(2, 4, 6, 8)), op1, 5, r)
		END_TEST()

		START_TEST("Reciprocal Vector");
	vec op1 = pm_Set(10, 20, 40, 80);
	TEST(vec r = pm_Reciprocal(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(0.1f, 0.05f, 0.025f, 0.0125f), pm_FillVector(ABSOLUTE_ERROR)), op1, r)
		END_TEST()

		START_TEST("SQRT Vector");
	vec op1 = pm_Set(10, 20, 40, 80);
	TEST(vec r = pm_Sqrt(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(3.16227766016838f, 4.47213595499958f, 6.324555320336759f, 8.944271909999159f), pm_FillVector(ABSOLUTE_ERROR)), op1, r)
		END_TEST()

		START_TEST("Reciprocal SQRT Vector");
	vec op1 = pm_Set(10, 20, 40, 80);
	TEST(vec r = pm_ReciprocalSqrt(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(0.31622776601684f, 0.22360679774998f, 0.15811388300842f, 0.11180339887499f), pm_FillVector(ABSOLUTE_ERROR)), op1, r)
		END_TEST()

		START_TEST("Vector ^ Vector");
	vec op1 = pm_Set(10, 20, 40, 80);
	vec op2 = pm_Set(1, 2, 4, 8);
	TEST(vec r = pm_Pow(op1, op2));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(10, 400, 2560000, 1677721600000000)), op1, op2, r)
		END_TEST()

		START_TEST("Exp Vector");
	vec op1 = pm_Set(10, 20, 40, 80);
	TEST(vec r = pm_Exp(op1));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(22026.46579480672f, 4.8516519540979028e8f, 2.3538526683702e17f, 5.5406223843935098e34f)), op1, r)
		END_TEST()

		START_TEST("Log Vector");
	vec op1 = pm_Set(10, 20, 40, 80);
	TEST(vec r = pm_Log(op1));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(2.302585092994046f, 2.995732273553991f, 3.688879454113936f, 4.382026634673881f)), op1, r)
		END_TEST()

		START_TEST("Sin Vector");
	vec op1 = pm_Set(2.5, -2.5, 3.1, 0.5);
	TEST(vec r = pm_Sin(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(0.59847214410396f, -0.59847214410396f, 0.04158066243329f, 0.4794255386042f), pm_FillVector(ABSOLUTE_ERROR)), op1, r)
		END_TEST()

		START_TEST("Cos Vector");
	vec op1 = pm_Set(2.5, -2.5, 3.1, 0.5);
	TEST(vec r = pm_Cos(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(-0.80114361554693f, -0.80114361554693f, -0.99913515027328f, 0.87758256189037f), pm_FillVector(ABSOLUTE_ERROR)), op1, r)
		END_TEST()

		START_TEST("Tan Vector");
	vec op1 = pm_Set(0.1, 0, 1.0, 0.5);
	TEST(vec r = pm_Tan(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(0.10033467208545f, 0, 1.557407724654902f, 0.54630248984379f), pm_FillVector(ABSOLUTE_ERROR)), op1, r)
		END_TEST()

		START_TEST("ASin Vector");
	vec op1 = pm_Set(1, 0, 0.5, -0.5);
	TEST(vec r = pm_ASin(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(1.570796326794897f, 0, 0.5235987755983f, -0.5235987755983f), pm_FillVector(ABSOLUTE_ERROR)), op1, r)
		END_TEST()

		START_TEST("ACos Vector");
	vec op1 = pm_Set(1, 0, 0.5, -0.5);
	TEST(vec r = pm_ACos(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(0, 1.570796326794897f, 1.047197551196598f, 2.094395102393196f), pm_FillVector(ABSOLUTE_ERROR)), op1, r)
		END_TEST()

		START_TEST("ATan Vector");
	vec op1 = pm_Set(PM_PI_F, 1, 2, 0);
	TEST(vec r = pm_ATan(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(1.262627255678912f, 0.78539816339745f, 1.10714871779409f, 0), pm_FillVector(ABSOLUTE_ERROR)), op1, r)
		END_TEST()

		START_TEST("LERP Vector");
	vec op1 = pm_Set(20, 0, 2.0, 0.5);
	vec op2 = pm_Set(40, 20, 4.0, 1.0);
	TEST(vec r = pm_Lerp(op1, op2, pm_FillVector(0.5f)));
	CHECK_TEST3(pm_IsNearlyEqual(r, pm_Set(30, 10, 3.0, 0.75f), pm_FillVector(ABSOLUTE_ERROR)), op1, op2, r)
		END_TEST()

		START_TEST("NLERP Vector");
	vec op1 = pm_Set(20, 0, 2.0, 0.5);
	vec op2 = pm_Set(40, 20, 4.0, 1.0);
	TEST(vec r = pm_NLerp(op1, op2, pm_FillVector(0.5f)));
	CHECK_TEST3(pm_IsNearlyEqual(r, pm_Set(0.94417967980259f, 0.3147265599342f, 0.094417967980259f, 0.023604491995065f), pm_FillVector(ABSOLUTE_ERROR)), op1, op2, r)
		END_TEST()

		START_TEST("SLERP Vector");
	vec op1 = pm_Set(0.66666666666667f, 0, 0.66666666666667f, 0.33333333333333f);
	vec op2 = pm_Set(0.27735009811261f, 0.55470019622523f, 0.55470019622523f, 0.55470019622523f);
	TEST(vec r = pm_SLerp(op1, op2, pm_FillVector(0.5f)));
	CHECK_TEST3(pm_IsNearlyEqual(r, pm_Set(0.50610424149908f, 0.29738467847613f, 0.65479658073714f, 0.47609062960663f), pm_FillVector(ABSOLUTE_ERROR)), op1, op2, r)
		END_TEST()

		START_TEST("Saturate Vector");
	vec op1 = pm_Set(20, 0, 2.0, 0.5);
	TEST(vec r = pm_Saturate(op1));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(1, 0, 1, 0.5f)), op1, r)
		END_TEST()

		START_TEST("Dot Vector4D");
	vec op1 = pm_Set(20, 0, 2.0, 0.5);
	vec op2 = pm_Set(20, 5, 10.0, 1);
	TEST(float r = pm_Dot4D(op1, op2));
	CHECK_TEST3(r == 420.5f, op1, op2, r)
		END_TEST()

		START_TEST("Magnitude Vector4D");
	vec op1 = pm_Set(0.66666666666667f, 0, 0.66666666666667f, 0.33333333333333f);
	TEST(float r = pm_Magnitude4D(op1));
	CHECK_TEST2(r == 1, op1, r)
		END_TEST()

		START_TEST("Magnitude Squared Vector4D");
	vec op1 = pm_Set(20, 0, 0, 0);
	TEST(float r = pm_MagnitudeSqr4D(op1));
	CHECK_TEST2(r == 400, op1, r)
		END_TEST()

		START_TEST("Normalize Vector4D");
	vec op1 = pm_Set(1, 1, 1, 1);
	TEST(vec r = pm_Normalize4D(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(0.5f, 0.5f, 0.5f, 0.5f), pm_FillVector(ABSOLUTE_ERROR)), op1, r)
		END_TEST()

		START_TEST("Dot Vector3D");
	vec op1 = pm_Set(20, 0, 2.0, 0);
	vec op2 = pm_Set(20, 5, 10.0, 0);
	TEST(float r = pm_Dot3D(op1, op2));
	CHECK_TEST3(r == 420, op1, op2, r)
		END_TEST()

		START_TEST("Cross Vector3D");
	vec op1 = pm_Set(1, 0, 2);
	vec op2 = pm_Set(2, 3, 5);
	TEST(vec r = pm_Cross3D(op1, op2));
	CHECK_TEST3(pm_IsNearlyEqual(r, pm_Set(-6, -1, 3), pm_FillVector(ABSOLUTE_ERROR)), op1, op2, r)
		END_TEST()

		START_TEST("Magnitude Vector3D");
	vec op1 = pm_Set(1, 0, 0);
	TEST(float r = pm_Magnitude3D(op1));
	CHECK_TEST2(r == 1, op1, r)
		END_TEST()

		START_TEST("Magnitude Squared Vector3D");
	vec op1 = pm_Set(2, 0, 0);
	TEST(float r = pm_MagnitudeSqr3D(op1));
	CHECK_TEST2(r == 4, op1, r)
		END_TEST()

		START_TEST("Normalize Vector3D");
	vec op1 = pm_Set(20, 0, 0);
	TEST(vec r = pm_Normalize3D(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(1, 0, 0), pm_FillVector(ABSOLUTE_ERROR)), op1, r)
		END_TEST()

		START_TEST("Dot Vector2D");
	vec op1 = pm_Set(20, 0);
	vec op2 = pm_Set(20, 5);
	TEST(float r = pm_Dot2D(op1, op2));
	CHECK_TEST3(r == 400, op1, op2, r)
		END_TEST()

		START_TEST("Magnitude Vector2D");
	vec op1 = pm_Set(1, 0);
	TEST(float r = pm_Magnitude2D(op1));
	CHECK_TEST2(r == 1, op1, r)
		END_TEST()

		START_TEST("Magnitude Squared Vector2D");
	vec op1 = pm_Set(2, 0);
	TEST(float r = pm_MagnitudeSqr2D(op1));
	CHECK_TEST2(r == 4, op1, r)
		END_TEST()

		START_TEST("Normalize Vector2D");
	vec op1 = pm_Set(20, 0);
	TEST(vec r = pm_Normalize2D(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(1, 0, 0), pm_FillVector(ABSOLUTE_ERROR)), op1, r)
		END_TEST()

		return TEST_UNIT_RESULT;
}

bool matrixTest()
{
	INIT_TEST_UNIT("Matrix");

	mat testOp1 = pm_Set(0.0, 1.0, 0.0, 2.0,
		2.0, 0.0, 1.0, 0.0,
		0.0, 1.0, 0.0, 2.0,
		2.0, 0.0, 1.0, 0.0);

	mat testOp2 = pm_Set(3.0, 1.0, 1.0, 2.0,
		2.0, 5.0, 1.0, 1.0,
		2.0, 1.0, 4.0, 2.0,
		2.0, 40.0, 1.0, 50.0);

	START_TEST("Identity Matrix");
	TEST(mat r = pm_Identity());
	CHECK_TEST2(pm_IsEqual(r, pm_Set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)), testOp1, r);
	END_TEST()

		START_TEST("Zero Matrix");
	TEST(mat r = pm_ZeroMatrix());
	CHECK_TEST2(pm_IsEqual(r, pm_FillMatrix(0)), testOp1, r);
	END_TEST()

		START_TEST("Translation Matrix");
	TEST(mat r = pm_Translation(pm_Set(3, 2, 6, 1)));
	CHECK_TEST1(pm_IsEqual(r, pm_Set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 3, 2, 6, 1)), r);
	END_TEST()

		//TODO: Should have a better precision...
		START_TEST("Rotation Quaternion Matrix");
	TEST(mat r = pm_Rotation(pm_Set(0, 0.949f, 0, 0.316f)));
	CHECK_TEST1(pm_IsNearlyEqual(r, pm_Set(-0.8f, 0, 0.6f, 0, 0, 1, 0, 0, -0.6f, 0, -0.8f, 0, 0, 0, 0, 1), 0.002f), r);
	END_TEST()

		START_TEST("Rotation Axis Matrix");
	TEST(mat r = pm_Rotation(pm_Set(3, 2, 6, 1), pm_DegToRad(45)));
	CHECK_TEST1(pm_IsNearlyEqual(r, pm_Set(
		3.343145751f, -2.485281374f, 6.686291501f, 0,
		6, 1.878679656f, 1.393398282f, 0,
		3.857864376f, 5.636038969f, 11.25126266, 0,
		0, 0, 0, 1), ABSOLUTE_ERROR), r);
	END_TEST()

		START_TEST("Scaling Matrix");
	TEST(mat r = pm_Scaling(pm_Set(3, 2, 6, 1)));
	CHECK_TEST1(pm_IsEqual(r, pm_Set(3, 0, 0, 0, 0, 2, 0, 0, 0, 0, 6, 0, 0, 0, 0, 1)), r);
	END_TEST()

		START_TEST("(SET) Matrix");
	TEST(mat r = pm_Set(0.0, 1.0, 0.0, 2.0,
		2.0, 0.0, 1.0, 0.0,
		0.0, 1.0, 0.0, 2.0,
		2.0, 0.0, 1.0, 0.0));
	CHECK_TEST2(pm_IsEqual(r, testOp1), testOp1, r);
	END_TEST()

		START_TEST("Matrix + Matrix");
	TEST(mat r = pm_Add(testOp1, testOp2));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(3, 2, 1, 4, 4, 5, 2, 1, 2, 2, 4, 4, 4, 40, 2, 50)), testOp1, testOp2, r);
	END_TEST()

		START_TEST("Matrix - Matrix");
	TEST(mat r = pm_Subtract(testOp1, testOp2));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(-3, 0, -1, 0, 0, -5, 0, -1, -2, 0, -4, 0, 0, -40, 0, -50)), testOp1, testOp2, r);
	END_TEST()

		START_TEST("Matrix * Matrix [Per element]");
	TEST(mat r = pm_MultiplyElement(testOp1, testOp2));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(0, 1, 0, 4, 4, 0, 1, 0, 0, 1, 0, 4, 4, 0, 1, 0)), testOp1, testOp2, r);
	END_TEST()

		START_TEST("Matrix / Matrix");
	TEST(mat r = pm_Divide(testOp1, testOp2));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0)), testOp1, testOp2, r);
	END_TEST()

		START_TEST("Matrix * Matrix");
	TEST(mat r = pm_Multiply(testOp1, testOp2));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(6, 85, 3, 101, 8, 3, 6, 6, 6, 85, 3, 101, 8, 3, 6, 6)), testOp1, testOp2, r);
	END_TEST()

		START_TEST("Transpose Matrix");
	TEST(mat r = pm_Transpose(testOp1));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(0, 2, 0, 2, 1, 0, 1, 0, 0, 1, 0, 1, 2, 0, 2, 0)), testOp1, r);
	END_TEST()

		START_TEST("Inverse Matrix");
	TEST(mat r = pm_Inverse(testOp2));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(0.38317757009346f, 0.040053404539386f, -0.10280373831776f, -0.012016021361816f, -0.13084112149533f, 0.21628838451268f, -0.021806853582555f, 0.0017801513128616f, -0.20560747663551f, 0.013351134846462f, 0.29906542056075f, -0.0040053404539386f, 0.093457943925234f, -0.17489986648865f, 0.015576323987539f, 0.019136626613262f), ABSOLUTE_ERROR), testOp2, r);
	END_TEST()

		START_TEST("Perspective Projection Matrix");
	TEST(mat r = pm_Perspective(1, 1, 1, 10.0f));
	CHECK_TEST1(pm_IsNearlyEqual(r, pm_Set(2, 0, 0, 0, 0, 2, 0, 0, 0, 0, -1.1111f, -1, 0, 0, -1.1111f, 0), ABSOLUTE_ERROR), r);
	END_TEST()

		START_TEST("Orthographic Projection Matrix");
	TEST(mat r = pm_Orthographic(1, 1, 1, 10.0f));
	CHECK_TEST1(pm_IsNearlyEqual(r, pm_Set(2, 0, 0, 0, 0, 2, 0, 0, 0, 0, -0.1111f, 0, 0, 0, -0.1111f, 1), ABSOLUTE_ERROR), r);
	END_TEST()

		return TEST_UNIT_RESULT;
}

bool quaternionTest()
{
	INIT_TEST_UNIT("Quaternion");

	START_TEST("Multiply Quaternion");
	quat op1 = pm_Set(5, 1, 5, 2);
	quat op2 = pm_Set(2, 5, 3, 1);
	TEST(quat r = pm_MultiplyQuat(op1, op2));
	CHECK_TEST3(pm_IsEqual(r, pm_Set(-13, 6, 34, -28)), op1, op2, r)
		END_TEST()

		START_TEST("Conjugate Quaternion");
	quat op1 = pm_Set(5, 1, 5, 2);
	TEST(quat r = pm_ConjugateQuat(op1));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(-5, -1, -5, 2)), op1, r)
		END_TEST()

		START_TEST("Inverse Quaternion");
	quat op1 = pm_Set(5, 1, 5, 2);
	TEST(quat r = pm_InverseQuat(op1));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(-0.090909090909091f, -0.018181818181818f, -
		0.090909090909091f, 0.036363636363636f), pm_FillVector(ABSOLUTE_ERROR)), op1, r)
		END_TEST()

		return TEST_UNIT_RESULT;
}

bool frameTest()
{
	INIT_TEST_UNIT("Frame");

	frame f;
	f.Up = pm_Set(0, 1, 1);
	f.Forward = pm_Set(1, 0, 1);
	f.Origin = pm_Set(5, 7, 3);

	START_TEST("(GET) Frame.XAxis");
	TEST(vec r = pm_GetXAxis(f));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(1, 1, -1, 0)), f, r)
		END_TEST()

		START_TEST("(GET) Frame.YAxis");
	TEST(vec r = pm_GetYAxis(f));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(0, 1, 1, 0)), f, r)
		END_TEST()

		START_TEST("(GET) Frame.ZAxis");
	TEST(vec r = pm_GetZAxis(f));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(1, 0, 1, 0)), f, r)
		END_TEST()

		START_TEST("Normalize Frame");
	TEST(frame r = pm_Normalize(f));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(pm_Set(5, 7, 3), pm_Set(0, 0.7071067912f, 0.7071067912f), pm_Set(0.816528f, -0.408264f, 0.408264f)), ABSOLUTE_ERROR), f, r)
		END_TEST()

		START_TEST("Translate World Frame");
	TEST(frame r = pm_TranslateWorld(f, pm_Set(5, 2, 6)));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(pm_Set(10, 9, 9), pm_Set(0, 1, 1, 0), pm_Set(1, 0, 1, 0))), f, r)
		END_TEST()

		START_TEST("Translate Local Frame");
	TEST(frame r = pm_TranslateLocal(f, pm_Set(5, 2, 6)));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(pm_Set(16, 14, 6), pm_Set(0, 1, 1, 0), pm_Set(1, 0, 1, 0))), f, r)
		END_TEST()

		START_TEST("Move Forward Frame");
	TEST(frame r = pm_MoveForward(f, 2));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(pm_Set(7, 7, 5), pm_Set(0, 1, 1, 0), pm_Set(1, 0, 1, 0))), f, r)
		END_TEST()

		START_TEST("Move Backward Frame");
	TEST(frame r = pm_MoveBackward(f, 2));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(pm_Set(3, 7, 1), pm_Set(0, 1, 1, 0), pm_Set(1, 0, 1, 0))), f, r)
		END_TEST()

		START_TEST("Move Right Frame");
	TEST(frame r = pm_MoveRight(f, 2));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(pm_Set(7, 9, 1), pm_Set(0, 1, 1, 0), pm_Set(1, 0, 1, 0))), f, r)
		END_TEST()

		START_TEST("Move Left Frame");
	TEST(frame r = pm_MoveLeft(f, 2));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(pm_Set(3, 5, 5), pm_Set(0, 1, 1, 0), pm_Set(1, 0, 1, 0))), f, r)
		END_TEST()

		START_TEST("Move Up Frame");
	TEST(frame r = pm_MoveUp(f, 2));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(pm_Set(5, 9, 5), pm_Set(0, 1, 1, 0), pm_Set(1, 0, 1, 0))), f, r)
		END_TEST()

		START_TEST("Move Down Frame");
	TEST(frame r = pm_MoveDown(f, 2));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(pm_Set(5, 5, 1), pm_Set(0, 1, 1, 0), pm_Set(1, 0, 1, 0))), f, r)
		END_TEST()

		START_TEST("Matrix Frame");
	TEST(mat r = pm_ToMatrix(f));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(
		1, 1, -1, 0,
		0, 1, 1, 0,
		1, 0, 1, 0,
		5, 7, 3, 1)), f, r);
	END_TEST()

		START_TEST("Camera Matrix Frame");
	TEST(mat r = pm_ToCameraMatrix(f));
	CHECK_TEST2(pm_IsEqual(r, pm_Set(
		-1, -1, 1, 0,
		0, 1, 1, 0,
		-1, 0, -1, 0,
		-5, -7, -3, 1)), f, r);
	END_TEST()

		f.Up = pm_Set(0, 1, 0);
	f.Forward = pm_Set(0, 0, 1);
	vec v = pm_GetXAxis(f);
	START_TEST("Rotate Local X Frame");
	TEST(frame r = pm_RotateLocalX(f, pm_DegToRad(90)));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(pm_Set(5, 7, 3), pm_Set(0, 0, -1, 0), pm_Set(0, 1, 0, 0)), ABSOLUTE_ERROR), f, r)
		END_TEST()

		START_TEST("Rotate Local Y Frame");
	TEST(frame r = pm_RotateLocalY(f, pm_DegToRad(90)));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(pm_Set(5, 7, 3), pm_Set(0, 1, 0, 0), pm_Set(-1, 0, 0, 0)), ABSOLUTE_ERROR), f, r)
		END_TEST()

		START_TEST("Rotate Local Z Frame");
	TEST(frame r = pm_RotateLocalZ(f, pm_DegToRad(90)));
	CHECK_TEST2(pm_IsNearlyEqual(r, pm_Set(pm_Set(5, 7, 3), pm_Set(1, 0, 0, 0), pm_Set(0, 0, 1, 0)), ABSOLUTE_ERROR), f, r)
		END_TEST()

		//TODO: Add test for other rotations...
		return TEST_UNIT_RESULT;
}