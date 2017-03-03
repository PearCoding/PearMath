#pragma once

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
	f

#define CHECK_TEST3(val, a1, a2, a3, res) \
if (!(val)) \
	{ \
	printf("\t\tTest failed:\n\t\t\tOp1: %s\n\t\t\tOp2: %s\n\t\t\tR: %s\n\t\t\tExpected: %s\n", to_str(a1, 0), to_str(a2, 1), to_str(a3, 2), to_str(res, 3)); \
	ut_Failure = true; \
	break; \
	}

#define CHECK_TEST2(val, a1, a2, res) \
if (!(val)) \
	{ \
	printf("\t\tTest failed:\n\t\t\tOp1: %s\n\t\t\tR: %s\n\t\t\tExpected: %s\n", to_str(a1, 0), to_str(a2, 1), to_str(res, 2)); \
	ut_Failure = true; \
	break; \
	}

#define CHECK_TEST1(val, a1, res) \
if (!(val)) \
	{ \
	printf("\t\tTest failed:\n\t\t\tR: %s\n\t\t\tExpected: %s\n", to_str(a1, 0), to_str(res, 1)); \
	ut_Failure = true; \
	break; \
	}

#define END_TEST() \
	}

#define TEST_UNIT_RESULT !ut_Failure

#define ABSOLUTE_ERROR (0.0001f)
#define TEST_LOOP_COUNT (5000)

char str_buffer[4][1024];
const char* to_str(float f, int i)
{
	sprintf(str_buffer[i], "%f", f);
	return str_buffer[i];
}

const char* to_str(const PM::vec2& v, int i)
{
	sprintf(str_buffer[i], "[%f, %f]", pm_GetX(v), pm_GetY(v));
	return str_buffer[i];
}

const char* to_str(const PM::vec3& v, int i)
{
	sprintf(str_buffer[i], "[%f, %f, %f]", pm_GetX(v), pm_GetY(v), pm_GetZ(v));
	return str_buffer[i];
}

const char* to_str(const PM::vec4& v, int i)
{
	sprintf(str_buffer[i], "[%f, %f, %f, %f]", pm_GetX(v), pm_GetY(v), pm_GetZ(v), pm_GetW(v));
	return str_buffer[i];
}

const char* to_str(const PM::mat2& m, int i)
{
	sprintf(str_buffer[i], "\n{\n\t%f, %f\n\t%f, %f\n}",
		pm_GetIndex(m, 0,0), pm_GetIndex(m, 0,1),
		pm_GetIndex(m, 1,0), pm_GetIndex(m, 1,1));
	return str_buffer[i];
}

const char* to_str(const PM::mat3& m, int i)
{
	sprintf(str_buffer[i], "\n{\n\t%f, %f, %f\n\t%f, %f, %f\n\t%f, %f, %f\n}",
		pm_GetIndex(m, 0,0), pm_GetIndex(m, 0,1), pm_GetIndex(m, 0,2),
		pm_GetIndex(m, 1,0), pm_GetIndex(m, 1,1), pm_GetIndex(m, 1,2),
		pm_GetIndex(m, 2,0), pm_GetIndex(m, 2,1), pm_GetIndex(m, 2,2));
	return str_buffer[i];
}

const char* to_str(const PM::mat4& m, int i)
{
	sprintf(str_buffer[i], "\n{\n\t%f, %f, %f, %f\n\t%f, %f, %f, %f\n\t%f, %f, %f, %f\n\t%f, %f, %f, %f\n}",
		pm_GetIndex(m, 0,0), pm_GetIndex(m, 0,1), pm_GetIndex(m, 0,2) ,pm_GetIndex(m, 0,3),
		pm_GetIndex(m, 1,0), pm_GetIndex(m, 1,1), pm_GetIndex(m, 1,2) ,pm_GetIndex(m, 1,3),
		pm_GetIndex(m, 2,0), pm_GetIndex(m, 2,1), pm_GetIndex(m, 2,2) ,pm_GetIndex(m, 2,3),
		pm_GetIndex(m, 3,0), pm_GetIndex(m, 3,1), pm_GetIndex(m, 3,2) ,pm_GetIndex(m, 3,3));
	return str_buffer[i];
}

const char* to_str(PM::frame f, int i)
{
	memset(str_buffer[i], 0, 1024);
	sprintf(str_buffer[i], "\n{\nUp:\t\t%f, %f, %f\nForward:\t%f, %f, %f\nOrigin:\t\t%f, %f, %f\n}",
		pm_GetX(f.Up), pm_GetY(f.Up), pm_GetZ(f.Up),
		pm_GetX(f.Forward), pm_GetY(f.Forward), pm_GetZ(f.Forward),
		pm_GetX(f.Origin), pm_GetY(f.Origin), pm_GetZ(f.Origin));
	return str_buffer[i];
}