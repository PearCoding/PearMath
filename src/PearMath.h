/*
* Copyright(c) 2014-2015, �mercan Yazici <pearcoding AT gmail.com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met :
*
*		1.	Redistributions of source code must retain the above copyright notice,
*			this list of conditions and the following disclaimer.
*
*		2.	Redistributions in binary form must reproduce the above copyright
*			notice, this list of conditions and the following disclaimer in the
*			documentation and / or other materials provided with the distribution.
*
*		3.	Neither the name of the copyright owner may be used
*			to endorse or promote products derived from this software without
*			specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT OWNER BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*/

#pragma once

#define PM_STRINGIFY(str) #str
#define PM_DOUBLEQUOTE(str) PM_STRINGIFY(str)

#define PM_NAME_STRING		"PearMath"
#define PM_VENDOR_STRING	"PearMath project 2014-2015"

#define PM_VERSION_MAJOR	1
#define PM_VERSION_MINOR	1
#define PM_VERSION_STRING 	PM_DOUBLEQUOTE(PM_VERSION_MAJOR) "." PM_DOUBLEQUOTE(PM_VERSION_MINOR)
#define PM_VERSION 			0x0101
#define PM_VERSION_CHECK(major, minor) (((major) << 8) | (minor))

//OS
#if defined(__linux) || defined(linux)
# define PM_OS_LINUX
# define PM_OS_NAME "Linux"
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) || defined(__TOS_WIN__)
# define PM_OS_WINDOWS
# if !defined(Win64) && !defined(_WIN64)
#  define PM_OS_WINDOWS_32
#  define PM_OS_NAME "Microsoft Windows 32 Bit"
# else
#  define PM_OS_WINDOWS_64
#  define PM_OS_NAME "Microsoft Windows 64 Bit"
# endif
#else
# define PM_OS_UNKNOWN
# define PM_OS_NAME "Unknown"
#endif

//Compilier
#if defined(__CYGWIN__)
# define PM_CC_CYGWIN
# define PM_CC_NAME "Cygwin"
#endif

#if defined(__GNUC__)
# define PM_CC_GNU
# define PM_CC_NAME "GNU C/C++"
#endif

#if defined(__MINGW32__)
# define PM_CC_MINGW32
# if !defined(PM_CC_GNU)
#  define PM_CC_NAME "MinGW 32"
# else
#  undef PM_CC_NAME
#  define PM_CC_NAME "GNU C/C++(MinGW 32)"
# endif
#endif

#if defined(__INTEL_COMPILER)
# define PM_CC_INTEL
# define PM_CC_NAME "Intel C/C++"
#endif

#if defined(_MSC_VER)
# define PM_CC_MSC
# define PM_CC_NAME "Microsoft Visual C++"
#endif

#if !defined(PM_CC_NAME)
# define PM_CC_NAME "Unknown compiler"
#endif

#if defined(PM_DEBUG)
# define PM_BUILDVARIANT_NAME "Debug"
#else
# ifndef PM_RELEASE
#  define PM_RELEASE
# endif
# define PM_BUILDVARIANT_NAME "Release"
#endif

#ifdef PM_CC_MSC
# define PM_DEBUG_BREAK() __debugbreak()
# define PM_ALIGN(x) __declspec(align(x))
#else//FIXME: Really use cpu dependent assembler?
# define PM_DEBUG_BREAK() __asm__ __volatile__ ("int $0x03")
# define PM_ALIGN(x) __attribute__((aligned(x)))
#endif

#ifndef PM_NO_ASSERTS
# define PM_ASSERT(cond) \
	do \
		{ \
		if(!(cond)) \
				{ \
			PM_DEBUG_BREAK(); \
				} \
		} while(0)
#else
# define PM_ASSERT(cond)
#endif

#if defined(PM_DEBUG)
# define PM_DEBUG_ASSERT(cond) PM_ASSERT(cond)
#else
# define PM_DEBUG_ASSERT(cond) PM::pm_Noop()
#endif

#include <cmath>
//#include <cstring>

#ifndef PM_MATH_NO_INLINE
# define PM_MATH_INLINE inline
#else
# define PM_MATH_INLINE
#endif

#if !defined(PM_NO_SIMD) && (defined(__SSE__) || _M_IX86_FP == 1 || defined(_M_X64 ) || defined(PM_FORCE_SSE))
# include <xmmintrin.h>
# define PM_USE_SSE
#endif

#if !defined(PM_NO_SIMD) && (defined(__SSE2__) || _M_IX86_FP == 2 || defined(_M_X64 ) || defined(PM_FORCE_SSE2))
# include <emmintrin.h>
# define PM_USE_SSE2
#endif

#if !defined(PM_NO_SIMD) && (defined(__SSE3__) || defined(PM_FORCE_SSE3))
# include <pmmintrin.h>
# define PM_USE_SSE3
#endif

#if !defined(PM_NO_SIMD) && (defined(__SSSE3__) || defined(PM_FORCE_SSSE3))
# include <tmmintrin.h>
# define PM_USE_SSSE3
#endif

#if !defined(PM_NO_SIMD) && (defined(__SSE4__) || defined(PM_FORCE_SSE4))
# include <smmintrin.h>
# define PM_USE_SSE4
#endif

#if defined(PM_USE_SSE) || \
		defined(PM_USE_SSE2) || \
		defined(PM_USE_SSE3) || \
		defined(PM_USE_SSSE3) || \
		defined(PM_USE_SSE4)
# ifndef PM_USE_SIMD
#  define PM_USE_SIMD
# endif
#endif

#define PM_PI				(3.1415926535897932384626433832795)
#define PM_2_PI				(6.283185307179586476925286766559)
#define PM_4_PI				(12.566370614359172953850573533118)
#define PM_PI_2_DIV			(1.5707963267948966192313216916398)
#define PM_PI_4_DIV			(0.78539816339744830961566084581988)
#define PM_INV_PI			(0.31830988618379067153776752674503)
#define PM_2_PI_360_DIV		(0.01745329251994329576923690768489)
#define PM_INV_2_PI_360_DIV	(57.295779513082320876798154814105)

#define PM_PI_F					(3.14159265358979323846f)
#define PM_2_PI_F				(6.28318530717958647693f)
#define PM_4_PI_F				(12.56637061435917295f)
#define PM_PI_2_DIV_F			(1.570796326794896619f)
#define PM_PI_4_DIV_F			(0.785398163397448309f)
#define PM_INV_PI_F				(0.318309886183790671f)
#define PM_2_PI_360_DIV_F		(0.017453292519943295f)
#define PM_INV_2_PI_360_DIV_F	(57.29577951308232087f)

#ifndef PM_MATH_SQRT_APPROXIMATION_ERROR
# define PM_MATH_SQRT_APPROXIMATION_ERROR (-0x4C000)
#endif

#ifndef PM_MATH_SIN_QUAL
# ifdef PM_FAST_MATH
#  define PM_MATH_SIN_QUAL 0
# else
#  define PM_MATH_SIN_QUAL 1
# endif
#endif

#ifndef PM_MATH_COS_QUAL
# ifdef PM_FAST_MATH
#  define PM_MATH_COS_QUAL 0
# else
#  define PM_MATH_COS_QUAL 1
# endif
#endif

#ifndef PM_MATH_RECIPROCALSQRT_QUAL
# ifdef PM_FAST_MATH
#  define PM_MATH_RECIPROCALSQRT_QUAL 0
# else
#  define PM_MATH_RECIPROCALSQRT_QUAL 1
# endif
#endif

#ifndef PM_VECTOR_SLERP_THRESHOLD
# define PM_VECTOR_SLERP_THRESHOLD (0.9995f)
#endif

#ifndef PM_EPSILON
# include <limits>
# define PM_EPSILON (std::numeric_limits<float>::epsilon())
#endif


/**
 * ATTENTION: Many 2D and 3D functions are constructed to change the other "unused" parts of the 4D vector.
 * Don't use these functions as an element wise operation.
 */
namespace PM
{
	inline void pm_Noop()
	{
	}

	template<typename T>
	inline T pm_MaxT(T a, T b) { return (a > b) ? a : b; }
	template<typename T>
	inline T pm_MinT(T a, T b) { return (a < b) ? a : b; }
	template<typename T>
	inline T pm_ClampT(T a, T min, T max) { return pm_MaxT<T>(min, pm_MinT<T>(max, a)); }
	template<typename T>
	inline T pm_SignT(T a) { return (a < 0) ? -1 : ((a > 0) ? 1 : 0); }

	template<typename T>
	inline void pm_SinCosT(T v, T& sin, T& cos)
	{
		// We hope for the optimization...
		sin = std::sin(v);
		cos = std::cos(v);
	}

	typedef signed char int8;
	typedef unsigned char uint8;

	typedef signed short int16;
	typedef unsigned short uint16;

	typedef signed int int32;
	typedef unsigned int uint32;

#if defined __GLIBC_HAVE_LONG_LONG
	__extension__ typedef signed long long int int64;
	__extension__ typedef unsigned long long int uint64;
#else
	typedef signed long int int64;
	typedef unsigned long int uint64;
#endif

#ifdef PM_USE_SIMD
	typedef __m128 vec;
#else
	struct PM_ALIGN(16) vec
	{
		float v[4];

		inline float& operator [] (int index)
		{
			PM_DEBUG_ASSERT(index < 4);
			return v[index];
		}

		inline float operator [] (int index) const
		{
			PM_DEBUG_ASSERT(index < 4);
			return v[index];
		}

		inline operator float* ()
		{
			return v;
		}

		inline operator const float* () const
		{
			return v;
		}
	};
#endif

	typedef vec vec4;
	typedef vec vec3;
	typedef vec vec2;
	typedef vec quat;
	typedef vec color;

	struct PM_ALIGN(16) avec//Array vector
	{
		float v[4];
	};

	struct PM_ALIGN(16) mat4
	{
		union
		{
			vec v[4];
			float m[4][4];
		};

		inline float* operator [] (int index)
		{
			PM_DEBUG_ASSERT(index < 4);
			return m[index];
		}

		inline const float* operator [] (int index) const
		{
			PM_DEBUG_ASSERT(index < 4);
			return m[index];
		}
	};

	typedef mat4 mat;
	// Memory?
	typedef mat mat4x3;
	typedef mat mat3;
	typedef mat mat2;

	struct PM_ALIGN(16) frame
	{
		vec3 Origin;
		vec3 Forward;
		vec3 Up;
	};

	//Vector
	void pm_AlignedCopy(vec& vdst, const vec& vsrc);
	void pm_Copy(vec& vdst, const vec& vsrc);

	vec pm_Zero();
	vec pm_One();
	vec pm_Set(float x, float y, float z = 0, float w = 0);

	vec pm_SetIndex(const vec& v, int index, float x);
	vec pm_SetX(const vec& v, float x);
	vec pm_SetY(const vec& v, float y);
	vec pm_SetZ(const vec& v, float z);
	vec pm_SetW(const vec& v, float w);
	float pm_GetIndex(const vec& v, int index);
	float pm_GetX(const vec& v);
	float pm_GetY(const vec& v);
	float pm_GetZ(const vec& v);
	float pm_GetW(const vec& v);

	vec pm_FillVector(float val);

	void pm_Vec2Array(const vec& v, float* dst);

	vec pm_IsEqualv(const vec& v1, const vec& v2);
	bool pm_IsEqual(const vec& v1, const vec& v2);
	vec pm_IsNearlyEqualv(const vec& v1, const vec& v2, const vec& delta);
	bool pm_IsNearlyEqual(const vec& v1, const vec& v2, const vec& delta);
	vec pm_IsNotEqualv(const vec& v1, const vec& v2);
	bool pm_IsNotEqual(const vec& v1, const vec& v2);
	vec pm_IsLessv(const vec& v1, const vec& v2);
	bool pm_IsLess(const vec& v1, const vec& v2);
	vec pm_IsLessOrEqualv(const vec& v1, const vec& v2);
	bool pm_IsLessOrEqual(const vec& v1, const vec& v2);
	vec pm_IsGreaterv(const vec& v1, const vec& v2);
	bool pm_IsGreater(const vec& v1, const vec& v2);
	vec pm_IsGreaterOrEqualv(const vec& v1, const vec& v2);
	bool pm_IsGreaterOrEqual(const vec& v1, const vec& v2);
	vec pm_IsInBounds(const vec& v, const vec& bounds);

	vec pm_Negate(const vec& v);
	vec pm_Add(const vec& v1, const vec& v2);
	vec pm_Subtract(const vec& v1, const vec& v2);
	vec pm_Multiply(const vec& v1, const vec& v2);//Component wise
	vec pm_Divide(const vec& v1, const vec& v2);
	vec pm_Scale(const vec& v, float factor);
	vec pm_MultiplyAdd(const vec& v1, const vec& v2, const vec& v3);
	vec pm_Reciprocal(const vec& v);
	vec pm_Reciprocal(const vec& v, int n);// Newton-Raphson
	vec pm_Sqrt(const vec& v);
	vec pm_ReciprocalSqrt(const vec& v);
	vec pm_Pow(const vec& v1, const vec& v2);
	vec pm_Exp(const vec& v);
	vec pm_Log(const vec& v);
	vec pm_Sin(const vec& v);
	vec pm_Cos(const vec& v);
	void pm_SinCos(const vec& v, vec& sin, vec& cos);
	vec pm_Tan(const vec& v);
	vec pm_ASin(const vec& v);
	vec pm_ACos(const vec& v);
	vec pm_ATan(const vec& v);

	vec pm_Lerp(const vec& v1, const vec& v2, const vec& t);
	vec pm_NLerp(const vec& v1, const vec& v2, const vec& t);
	vec pm_SLerp(const vec& v1, const vec& v2, const vec& t);

	vec pm_Max(const vec& v1, const vec& v2);
	vec pm_Min(const vec& v1, const vec& v2);
	vec pm_Clamp(const vec& v, const vec& min, const vec& max);
	vec pm_Saturate(const vec& v);

	vec4 pm_Load4D(const float src[4]);
	void pm_Store4D(const vec4& v, float dst[4]);
	float pm_Dot4D(const vec4& v1, const vec4& v2);
	float pm_Magnitude4D(const vec4& v);
	float pm_MagnitudeSqr4D(const vec4& v);
	vec4 pm_Normalize4D(const vec4& v);
	vec4 pm_QualityNormalize4D(const vec4& v);
	vec4 pm_FastNormalize4D(const vec4& v);

	vec3 pm_Load3D(const float src[3]);
	void pm_Store3D(const vec3& v, float dst[3]);
	float pm_Dot3D(const vec3& v1, const vec3& v2);
	vec3 pm_Cross3D(const vec3& v1, const vec3& v2);
	float pm_Magnitude3D(const vec3& v);
	float pm_MagnitudeSqr3D(const vec3& v);
	vec3 pm_Normalize3D(const vec3& v);
	vec3 pm_QualityNormalize3D(const vec2& v);
	vec3 pm_FastNormalize3D(const vec2& v);

	vec2 pm_Load2D(const float src[2]);
	void pm_Store2D(const vec2& v, float dst[2]);
	float pm_Dot2D(const vec2& v1, const vec2& v2);
	float pm_Magnitude2D(const vec2& v);
	float pm_MagnitudeSqr2D(const vec2& v);
	vec2 pm_Normalize2D(const vec2& v);
	vec2 pm_QualityNormalize2D(const vec2& v);
	vec2 pm_FastNormalize2D(const vec2& v);

	//Matrix
	mat pm_LoadMatrix(const float* src);
	void pm_StoreMatrix(const mat& src, float* dst);

	mat pm_Identity();
	mat pm_ZeroMatrix();
	mat pm_Translation(const vec& v);
	mat pm_Rotation(const quat& v);
	mat pm_Rotation(const vec& v, float angle);
	mat pm_RotationYawPitchRoll(const vec& v);
	mat pm_Scaling(const vec3& v);

	mat pm_Set2D(const vec& r1, const vec& r2);
	mat pm_Set2D(float m00, float m01, float m10, float m11);

	mat pm_Set3D(const vec& r1, const vec& r2, const vec& r3);
	mat pm_Set3D(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22);

	mat pm_Set4D(const vec& r1, const vec& r2, const vec& r3, const vec& r4);
	mat pm_Set4D(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	float pm_Get(const mat& m, int x, int y);
	vec pm_GetRow(const mat& m, int y);
	vec pm_GetColumn(const mat& m, int x);
	
	vec pm_DecomposeTranslation(const mat& m);
	vec3 pm_DecomposeScale(const mat& m);
	quat pm_DecomposeRotation(const mat& m);
	void pm_Decompose(const mat& m, vec& t, vec3& s, quat& r);
	mat pm_FillMatrix(float val);

	mat pm_IsEqualv(const mat& m1, const mat& m2);
	bool pm_IsEqual(const mat& m1, const mat& m2);
	mat pm_IsNearlyEqualv(const mat& m1, const mat& m2, const mat& delta);
	mat pm_IsNearlyEqualv(const mat& m1, const mat& m2, float delta);
	bool pm_IsNearlyEqual(const mat& m1, const mat& m2, const mat& delta);
	bool pm_IsNearlyEqual(const mat& m1, const mat& m2, float delta);
	mat pm_IsNotEqualv(const mat& m1, const mat& m2);
	bool pm_IsNotEqual(const mat& m1, const mat& m2);

	mat pm_Negate(const mat& m);
	mat pm_Add(const mat& m1, const mat& m2);
	mat pm_Add(const mat& m, float s);
	mat pm_Subtract(const mat& m1, const mat& m2);
	mat pm_Subtract(const mat& m, float s);
	mat pm_Multiply(const mat& m1, const mat& m2);
	vec pm_Multiply(const mat& m, const vec& v);
	mat pm_MultiplyElement(const mat& m1, const mat& m2);
	mat pm_Multiply(const mat& m, float s);
	mat pm_Divide(const mat& m1, const mat& m2);
	mat pm_Divide(const mat& m, float s);
	mat pm_Transpose(const mat& m);

	mat pm_Inverse2D(const mat& m, float* determinant = 0);
	mat pm_Inverse3D(const mat& m, float* determinant = 0);
	mat pm_Inverse4D(const mat& m, float* determinant = 0);

	float pm_Determinant2D(const mat& m);
	float pm_Determinant3D(const mat& m);
	float pm_Determinant4D(const mat& m);

	vec pm_Transform(const mat& m, const vec& v);

	mat pm_Perspective(float width, float height, float near, float far);
	mat pm_Orthographic(float width, float height, float near, float far);

	//Quaternion
	quat pm_IdentityQuat();
	quat pm_MultiplyQuat(const quat& q1, const quat& q2);
	quat pm_ConjugateQuat(const quat& q);
	quat pm_InverseQuat(const quat& q);

	quat pm_SLerpQuat(const quat& q1, const quat& q2, const vec& t);

	quat pm_RotationQuatFromXYZ(float angle_x, float angle_y, float angle_z);
	quat pm_RotationQuatFromXYZ(const vec3& angles);
	vec3 pm_RotationQuatToXYZ(const quat& rot);
	quat pm_RotationAxis(const vec& axis, float angle);
	quat pm_RotationMatrixNormalized(const mat& m);
	quat pm_RotationMatrix(const mat& m);
	vec pm_RotateWithQuat(const quat& rot, const vec& v);

	quat pm_RotateFromTo(const vec3& from, const vec3& to);
	quat pm_RotateFromTo(const vec3& from, const vec3& to, const vec3& fallback);

	//Color

	vec pm_SetR(const color& v, float x);
	vec pm_SetG(const color& v, float y);
	vec pm_SetB(const color& v, float z);
	vec pm_SetA(const color& v, float w);
	float pm_GetR(const color& v);
	float pm_GetG(const color& v);
	float pm_GetB(const color& v);
	float pm_GetA(const color& v);

	//Frame
	vec3 pm_GetXAxis(const frame& f);
	vec3 pm_GetYAxis(const frame& f);
	vec3 pm_GetZAxis(const frame& f);

	frame pm_IdentityFrame();
	frame pm_Set(const vec& origin, const vec& up, const vec& forward);

	frame pm_IsEqualv(const frame& f1, const frame& f2);
	bool pm_IsEqual(const frame& f1, const frame& f2);
	frame pm_IsNearlyEqualv(const frame& f1, const frame& f2, const frame& delta);
	frame pm_IsNearlyEqualv(const frame& f1, const frame& f2, float delta);
	bool pm_IsNearlyEqual(const frame& f1, const frame& f2, const frame& delta);
	bool pm_IsNearlyEqual(const frame& f1, const frame& f2, float delta);
	frame pm_IsNotEqualv(const frame& f1, const frame& f2);
	bool pm_IsNotEqual(const frame& f1, const frame& f2);

	frame pm_Normalize(const frame& f);

	frame pm_TranslateWorld(const frame& f, const vec3& v);
	frame pm_TranslateLocal(const frame& f, const vec3& v);

	frame pm_MoveForward(const frame& f, float s);
	frame pm_MoveBackward(const frame& f, float s);
	frame pm_MoveUp(const frame& f, float s);
	frame pm_MoveDown(const frame& f, float s);
	frame pm_MoveRight(const frame& f, float s);
	frame pm_MoveLeft(const frame& f, float s);

	mat pm_ToMatrix(const frame& f, bool rotationOnly = false);
	mat pm_ToCameraMatrix(const frame& f, bool rotationOnly = false);

	frame pm_RotateLocalX(const frame& f, float angle);
	frame pm_RotateLocalY(const frame& f, float angle);
	frame pm_RotateLocalZ(const frame& f, float angle);

	frame pm_RotateWorld(const frame& f, float angle, const vec3& axis);
	frame pm_RotateLocal(const frame& f, float angle, const vec3& axis);

	vec3 pm_LocalToWorld(const frame& f, const vec3& v, bool rotationOnly = false);
	vec3 pm_WorldToLocal(const frame& f, const vec3& v);

	vec3 pm_TransformPoint(const frame& f, const vec3& off);
	vec3 pm_Rotate(const frame& f, const vec3& v);

	//Scalar
	inline float pm_DegToRad(float x)
	{
		return x*PM_2_PI_360_DIV_F;
	}

	inline float pm_RadToDeg(float x)
	{
		return x*PM_INV_2_PI_360_DIV_F;
	}

	inline float pm_HourToDeg(float x)
	{
		return x * (1.0f / 15.0f);
	}

	inline float pm_HourToRad(float x)
	{
		return pm_DegToRad(pm_HourToDeg(x));
	}

	inline float pm_DegToHour(float x)
	{
		return x * 15.0f;
	}

	inline float pm_RadToHour(float x)
	{
		return pm_DegToHour(pm_RadToDeg(x));
	}

	template<typename T>
	inline T pm_NextPowerOf2(T x)
	{
		T val = 1;
		while (val < x)
		{
			val <<= 1;
		}

		return val;
	}

	inline float pm_Reciprocal(float f, int n)
	{
		if (n <= 1)
			return 1.0f / f;
		else
		{
			float x = f*0.25f;

			for (int i = 0; i < n; ++i)
				x = x*(2 - f*x);

			return x;
		}
	}

#define _PM_MATH_INCLUDED_
# include "MathVector.inl"
# include "MathMatrix.inl"
# include "MathQuaternion.inl"
# include "MathColor.inl"
# include "MathFrame.inl"
#undef _PM_MATH_INCLUDED_
}
