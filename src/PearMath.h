/*
* Copyright(c) 2014-2017, OEmercan Yazici <pearcoding AT gmail.com>
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
#define PM_VENDOR_STRING	"PearMath project 2014-2017"

#define PM_VERSION_MAJOR	1
#define PM_VERSION_MINOR	3
#define PM_VERSION_STRING 	PM_DOUBLEQUOTE(PM_VERSION_MAJOR) "." PM_DOUBLEQUOTE(PM_VERSION_MINOR)
#define PM_VERSION 			0x0103
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
# ifdef PM_CC_NAME
#  undef PM_CC_NAME
# endif
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

#ifndef PM_NO_ASSERTS
#include <assert.h>
# define PM_ASSERT(cond) assert((cond))
#else
# define PM_ASSERT(cond)
#endif

#if defined(PM_DEBUG)
# define PM_DEBUG_ASSERT(cond) PM_ASSERT((cond))
#else
# define PM_DEBUG_ASSERT(cond) PM::pm_Noop()
#endif

#include <cmath>
#include <cstring>

#ifndef PM_MATH_NO_INLINE
# define PM_MATH_INLINE inline
#else
# define PM_MATH_INLINE
#endif

#ifdef PM_WITH_SIMD
# if !defined(PM_WITH_SSE) or !defined(PM_WITH_SSE2)
#  error Need atleast SSE and SSE2 support for SIMD features. Undef PM_WITH_SIMD to proceed, or fix code.
# endif
#endif

#if defined(PM_CC_INTEL) && defined(PM_USE_SVML)
# define PM_WITH_SVML
#endif

#ifdef PM_CC_GNU
# include <x86intrin.h>
#else
# include <intrin.h>
#endif

#ifdef PM_WITH_SVML
# include <immintrin.h>
#endif

// Constants
#define PM_PI				(3.1415926535897932384626433832795)
#define PM_2_PI				(2*PM_PI)
#define PM_4_PI				(4*PM_PI)
#define PM_PI_2_DIV			(PM_PI*0.5)
#define PM_PI_4_DIV			(PM_PI*0.25)
#define PM_INV_PI			(0.31830988618379067153776752674503)
#define PM_INV_2_PI			(PM_INV_PI*0.5)
#define PM_INV_4_PI			(PM_INV_PI*0.25)
#define PM_2_PI_360_DIV		(0.01745329251994329576923690768489)
#define PM_INV_2_PI_360_DIV	(57.295779513082320876798154814105)

#define PM_PI_F					(3.14159265358979323846f)
#define PM_2_PI_F				(2*PM_PI_F)
#define PM_4_PI_F				(4*PM_PI_F)
#define PM_PI_2_DIV_F			(PM_PI_F*0.5)
#define PM_PI_4_DIV_F			(PM_PI_F*0.25)
#define PM_INV_PI_F				(0.318309886183790671f)
#define PM_INV_2_PI_F			(PM_INV_PI_F*0.5f)
#define PM_INV_4_PI_F			(PM_INV_PI_F*0.25f)
#define PM_2_PI_360_DIV_F		(0.017453292519943295f)
#define PM_INV_2_PI_360_DIV_F	(57.29577951308232087f)

#define PM_LOG_2_F			(0.69314718056f)
#define PM_LOG_10_F			(2.30258509299f)
#define PM_INV_LOG_2_F		(1.44269504089f)
#define PM_INV_LOG_10_F		(0.4342944819f)

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
#  define PM_MATH_RECIPROCALSQRT_QUAL 1
# else
#  define PM_MATH_RECIPROCALSQRT_QUAL 2
# endif
#endif

#ifndef PM_VECTOR_SLERP_THRESHOLD
# define PM_VECTOR_SLERP_THRESHOLD (0.9995f)
#endif

#ifndef PM_EPSILON
# include <limits>
# define PM_EPSILON (std::numeric_limits<float>::epsilon())
#endif

#define _PM_SSE_DIM_MASK(D) (1 << (D))

#include <type_traits>
#include <utility>
namespace PM
{
	inline void pm_Noop()
	{
	}

	// Scalars
	template<typename T>
	inline T pm_Max(const T& a, const T& b) { return (a > b) ? a : b; }
	template<typename T>
	inline T pm_Min(const T& a, const T& b) { return (a < b) ? a : b; }
	template<typename T>
	inline T pm_Clamp(const T& a, const T& min, const T& max) { return pm_Max<T>(min, pm_Min<T>(max, a)); }
	template<typename T>
	inline T pm_Sign(T a) { return (a < 0) ? -1 : ((a > 0) ? 1 : 0); }
	template<typename T>
	inline void pm_SinCos(const T& v, T& sin, T& cos)
	{
		// We hope for the optimization...
		sin = std::sin(v);
		cos = std::cos(v);
	}

	template<>
	inline void pm_SinCos<float>(const float& v, float& sin, float& cos)
	{
#if defined(_GNU_SOURCE)
		::sincosf(v, &sin, &cos);
#else
		sin = std::sin(v);
		cos = std::cos(v);
#endif
	}

	template<>
	inline void pm_SinCos<double>(const double& v, double& sin, double& cos)
	{
#if defined(_GNU_SOURCE)
		::sincos(v, &sin, &cos);
#else
		sin = std::sin(v);
		cos = std::cos(v);
#endif
	}

	template<typename T>
	inline T pm_SafeASin(T v)
	{
		return std::asin(pm_Clamp<T>(v, -1, 1));
	}

	template<typename T>
	inline T pm_SafeACos(T v)
	{
		return std::acos(pm_Clamp<T>(v, -1, 1));
	}

	template<typename T>
	inline T pm_SafeSqrt(T v)
	{
		return std::sqrt(pm_Max<T>(v, 0));
	}

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

	// Typedefs
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

	template<int D>
	struct alignas(16) vec
	{
		static constexpr int Dimension = D;
		vec() = default;

	#ifdef PM_WITH_SIMD
		__m128 _vec;
		vec(const __m128& v) : _vec(v) {}
	#else
		float _vec[D];
	#endif
	};

	typedef vec<4> vec4;
	typedef vec<3> vec3;
	typedef vec<2> vec2;
	typedef vec<4> quat;
	typedef vec<4> color;

	template<int D1, int D2>
	struct alignas(16) mat
	{
		static constexpr int Rows = D1;
		static constexpr int Columns = D2;
		typedef vec<D2> row_t;
		typedef vec<D1> column_t;

		vec<D2> v[D1];
	};

	typedef mat<4,4> mat4;
	typedef mat<3,3> mat3;
	typedef mat<2,2> mat2;

	struct frame
	{
		vec3 Origin;
		vec3 Forward;
		vec3 Up;
	};

	// type_traits
	template<class VectorType>
	struct is_vector : std::integral_constant<bool,
		std::is_same<vec4, typename std::remove_cv<VectorType>::type>::value ||
		std::is_same<vec3, typename std::remove_cv<VectorType>::type>::value ||
		std::is_same<vec2, typename std::remove_cv<VectorType>::type>::value> {};

	template<class MatrixType>
	struct is_matrix : std::integral_constant<bool,
		std::is_same<mat4, typename std::remove_cv<MatrixType>::type>::value ||
		std::is_same<mat3, typename std::remove_cv<MatrixType>::type>::value ||
		std::is_same<mat2, typename std::remove_cv<MatrixType>::type>::value> {};

	//Vector
	template<typename VectorType>
	VectorType pm_SetIndex(const VectorType& v, int index, float x);
	template<typename VectorType>
	float pm_GetIndex(const VectorType& v, int index);
	template<typename VectorType>
	VectorType pm_SetX(const VectorType& v, float x);
	template<typename VectorType>
	VectorType pm_SetY(const VectorType& v, float y);
	template<typename VectorType>
	VectorType pm_SetZ(const VectorType& v, float z);
	template<typename VectorType>
	VectorType pm_SetW(const VectorType& v, float w);
	template<typename VectorType>
	float pm_GetX(const VectorType& v);
	template<typename VectorType>
	float pm_GetY(const VectorType& v);
	template<typename VectorType>
	float pm_GetZ(const VectorType& v);
	template<typename VectorType>
	float pm_GetW(const VectorType& v);

	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
		pm_IsEqualv(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, bool>::type
		pm_IsEqual(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
		pm_IsNearlyEqualv(const VectorType& v1, const VectorType& v2, const VectorType& delta);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
		pm_IsNearlyEqualv(const VectorType& v1, const VectorType& v2, float delta);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, bool>::type
		pm_IsNearlyEqual(const VectorType& v1, const VectorType& v2, const VectorType& delta);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, bool>::type
		pm_IsNearlyEqual(const VectorType& v1, const VectorType& v2, const float delta);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
		pm_IsNotEqualv(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, bool>::type
		pm_IsNotEqual(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
		pm_IsLessv(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, bool>::type
		pm_IsLess(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
		pm_IsLessOrEqualv(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, bool>::type
		pm_IsLessOrEqual(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
		pm_IsGreaterv(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, bool>::type
		pm_IsGreater(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
		pm_IsGreaterOrEqualv(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, bool>::type
		pm_IsGreaterOrEqual(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	VectorType pm_IsInBounds(const VectorType& v, const VectorType& bounds);

	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
		pm_Negate(const VectorType& v);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
		pm_Add(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
		pm_Subtract(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
		pm_Multiply(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
		pm_Divide(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
		pm_Scale(const VectorType& v, float factor);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
		pm_MultiplyAdd(const VectorType& v1, const VectorType& v2, const VectorType& v3);
	template<typename VectorType>
	typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
		pm_Reciprocal(const VectorType& v);
	template<typename VectorType>
	VectorType pm_Sqrt(const VectorType& v);
	template<typename VectorType>
	VectorType pm_ReciprocalSqrt(const VectorType& v);
	template<typename VectorType>
	VectorType pm_Pow(const VectorType& v1, const VectorType& v2);
	template<typename VectorType>
	VectorType pm_Exp(const VectorType& v);
	template<typename VectorType>
	VectorType pm_Exp2(const VectorType& v);
	template<typename VectorType>
	VectorType pm_Exp10(const VectorType& v);
	template<typename VectorType>
	VectorType pm_ExpM1(const VectorType& v);
	template<typename VectorType>
	VectorType pm_Log(const VectorType& v);
	template<typename VectorType>
	VectorType pm_Log2(const VectorType& v);
	template<typename VectorType>
	VectorType pm_Log10(const VectorType& v);
	template<typename VectorType>
	VectorType pm_Log1P(const VectorType& v);
	template<typename VectorType>
	VectorType pm_Sin(const VectorType& v);
	template<typename VectorType>
	VectorType pm_Cos(const VectorType& v);
	template<typename VectorType>
	VectorType pm_Tan(const VectorType& v);
	template<typename VectorType>
	VectorType pm_ASin(const VectorType& v);
	template<typename VectorType>
	VectorType pm_ACos(const VectorType& v);
	template<typename VectorType>
	VectorType pm_ATan(const VectorType& v);

	template<typename VectorType>
	VectorType pm_Lerp(const VectorType& v1, const VectorType& v2, const VectorType& t);
	template<typename VectorType>
	VectorType pm_NLerp(const VectorType& v1, const VectorType& v2, const VectorType& t);
	template<typename VectorType>
	VectorType pm_SLerp(const VectorType& v1, const VectorType& v2, const VectorType& t);

	template<typename VectorType>
	VectorType pm_Abs(const VectorType& v);
	template<typename VectorType>
	VectorType pm_Saturate(const VectorType& v);
	template<typename VectorType>
	VectorType pm_Ceil(const VectorType& v);
	template<typename VectorType>
	VectorType pm_Floor(const VectorType& v);

	vec4 pm_Zero4D();
	vec4 pm_One4D();
	vec4 pm_Set(float x, float y, float z, float w);
	vec4 pm_FillVector4(float val);
	vec4 pm_Load4D(const float src[4]);
	void pm_Store4D(const vec4& v, float dst[4]);
	vec4 pm_ExtendTo4D(const vec2& v);
	vec4 pm_ExtendTo4D(const vec3& v);
	float pm_Dot(const vec4& v1, const vec4& v2);
	float pm_Magnitude(const vec4& v);
	float pm_MagnitudeSqr(const vec4& v);
	vec4 pm_Normalize(const vec4& v);
	vec4 pm_QualityNormalize(const vec4& v);
	vec4 pm_FastNormalize(const vec4& v);
	float pm_MaxElement(const vec4& v);
	float pm_MinElement(const vec4& v);
	template<>
	void pm_SinCos<vec4>(const vec4& v, vec4& sin, vec4& cos);
	template<>
	vec4 pm_Max<vec4>(const vec4& v1, const vec4& v2);
	template<>
	vec4 pm_Min<vec4>(const vec4& v1, const vec4& v2);
	template<>
	vec4 pm_Clamp<vec4>(const vec4& v, const vec4& min, const vec4& max);

	vec3 pm_Zero3D();
	vec3 pm_One3D();
	vec3 pm_Set(float x, float y, float z);
	vec3 pm_FillVector3D(float val);
	vec3 pm_Load3D(const float src[3]);
	void pm_Store3D(const vec3& v, float dst[3]);
	vec3 pm_ExtendTo3D(const vec2& v);
	vec3 pm_ShrinkTo3D(const vec4& v);
	float pm_Dot(const vec3& v1, const vec3& v2);
	vec3 pm_Cross(const vec3& v1, const vec3& v2);
	float pm_Magnitude(const vec3& v);
	float pm_MagnitudeSqr(const vec3& v);
	vec3 pm_Normalize(const vec3& v);
	vec3 pm_QualityNormalize(const vec3& v);
	vec3 pm_FastNormalize(const vec3& v);
	float pm_MaxElement(const vec3& v);
	float pm_MinElement(const vec3& v);
	template<>
	void pm_SinCos<vec3>(const vec3& v, vec3& sin, vec3& cos);
	template<>
	vec3 pm_Max<vec3>(const vec3& v1, const vec3& v2);
	template<>
	vec3 pm_Min<vec3>(const vec3& v1, const vec3& v2);
	template<>
	vec3 pm_Clamp<vec3>(const vec3& v, const vec3& min, const vec3& max);

	vec2 pm_Zero2D();
	vec2 pm_One2D();
	vec2 pm_Set(float x, float y);
	vec2 pm_FillVector2D(float val);
	vec2 pm_Load2D(const float src[2]);
	void pm_Store2D(const vec2& v, float dst[2]);
	vec2 pm_ShrinkTo2D(const vec4& v);
	vec2 pm_ShrinkTo2D(const vec3& v);
	float pm_Dot(const vec2& v1, const vec2& v2);
	float pm_Magnitude(const vec2& v);
	float pm_MagnitudeSqr(const vec2& v);
	vec2 pm_Normalize(const vec2& v);
	vec2 pm_QualityNormalize(const vec2& v);
	vec2 pm_FastNormalize(const vec2& v);
	float pm_MaxElement(const vec2& v);
	float pm_MinElement(const vec2& v);
	template<>
	void pm_SinCos<vec2>(const vec2& v, vec2& sin, vec2& cos);
	template<>
	vec2 pm_Max<vec2>(const vec2& v1, const vec2& v2);
	template<>
	vec2 pm_Min<vec2>(const vec2& v1, const vec2& v2);
	template<>
	vec2 pm_Clamp<vec2>(const vec2& v, const vec2& min, const vec2& max);

	//Matrix
	mat4 pm_Identity4();
	mat4 pm_ZeroMatrix4();
	mat4 pm_LoadMatrix4(const float* src);
	void pm_StoreMatrix4(const mat4& src, float* dst);
	mat4 pm_FillMatrix4(float val);
	mat4 pm_Create(const vec4& r1, const vec4& r2, const vec4& r3, const vec4& r4);
	mat4 pm_Create(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);
	vec4 pm_GetColumn(const mat4& m, int x);
	mat4 pm_Product(const mat4& m1, const mat4& m2);
	mat4 pm_Perspective(float width, float height, float near, float far);
	mat4 pm_Orthographic(float width, float height, float near, float far);
	mat4 pm_Inverse(const mat4& m, float* determinant = nullptr);
	float pm_Determinant(const mat4& m);
	mat4 pm_Transpose(const mat4& m);

	mat3 pm_Identity3();
	mat3 pm_ZeroMatrix3();
	mat3 pm_LoadMatrix3(const float* src);
	void pm_StoreMatrix3(const mat3& src, float* dst);
	mat3 pm_FillMatrix3(float val);
	mat3 pm_Create(const vec3& r1, const vec3& r2, const vec3& r3);
	mat3 pm_Create(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22);
	vec3 pm_GetColumn(const mat3& m, int x);
	mat3 pm_Product(const mat3& m1, const mat3& m2);
	mat3 pm_Inverse(const mat3& m, float* determinant = nullptr);
	float pm_Determinant(const mat3& m);
	mat3 pm_Transpose(const mat3& m);

	mat2 pm_Identity2();
	mat2 pm_ZeroMatrix2();
	mat2 pm_LoadMatrix2(const float* src);
	void pm_StoreMatrix2(const mat2& src, float* dst);
	mat2 pm_FillMatrix2(float val);
	mat2 pm_Create(const vec2& r1, const vec2& r2);
	mat2 pm_Create(float m00, float m01, float m10, float m11);
	vec2 pm_GetColumn(const mat2& m, int x);
	mat2 pm_Product(const mat2& m1, const mat2& m2);
	mat2 pm_Inverse(const mat2& m, float* determinant = nullptr);
	float pm_Determinant(const mat2& m);
	mat2 pm_Transpose(const mat2& m);

	template<typename MatrixType>
	MatrixType pm_SetIndex(const MatrixType& m, int x, int y, float f);
	template<typename MatrixType>
	float pm_GetIndex(const MatrixType& m, int x, int y);

	template<typename MatrixType>
	typename MatrixType::row_t pm_GetRow(const MatrixType& m, int x);
	
	mat4 pm_Translation(const vec3& v);
	mat4 pm_Rotation(const quat& v);
	mat4 pm_Scaling(const vec3& v);
	vec3 pm_DecomposeTranslation(const mat4& m);
	vec3 pm_DecomposeScale(const mat4& m);
	quat pm_DecomposeRotation(const mat4& m);
	void pm_Decompose(const mat4& m, vec3& t, vec3& s, quat& r);

	template<typename MatrixType>
	typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
		pm_IsEqualv(const MatrixType& m1, const MatrixType& m2);
	template<typename MatrixType>
	typename std::enable_if<is_matrix<MatrixType>::value, bool>::type
		pm_IsEqual(const MatrixType& m1, const MatrixType& m2);
	template<typename MatrixType>
	typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
		pm_IsNearlyEqualv(const MatrixType& m1, const MatrixType& m2, const MatrixType& delta);
	template<typename MatrixType>
	typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
		pm_IsNearlyEqualv(const MatrixType& m1, const MatrixType& m2, float delta);
	template<typename MatrixType>
	typename std::enable_if<is_matrix<MatrixType>::value, bool>::type
		pm_IsNearlyEqual(const MatrixType& m1, const MatrixType& m2, const MatrixType& delta);
	template<typename MatrixType>
	typename std::enable_if<is_matrix<MatrixType>::value, bool>::type
		pm_IsNearlyEqual(const MatrixType& m1, const MatrixType& m2, float delta);
	template<typename MatrixType>
	typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
		pm_IsNotEqualv(const MatrixType& m1, const MatrixType& m2);
	template<typename MatrixType>
	typename std::enable_if<is_matrix<MatrixType>::value, bool>::type
		pm_IsNotEqual(const MatrixType& m1, const MatrixType& m2);

	template<typename MatrixType>
	typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
		pm_Negate(const MatrixType& m);
	template<typename MatrixType>
	typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
		pm_Add(const MatrixType& m1, const MatrixType& m2);
	template<typename MatrixType>
	typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
		pm_Subtract(const MatrixType& m1, const MatrixType& m2);
	template<typename MatrixType>
	typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
		pm_Multiply(const MatrixType& m1, const MatrixType& m2);
	template<typename MatrixType>
	typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
		pm_Scale(const MatrixType& m, float s);
	template<typename MatrixType>
	typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
		pm_Divide(const MatrixType& m1, const MatrixType& m2);

	template<typename MatrixType>
	typename MatrixType::row_t pm_Product(const MatrixType& m, const typename MatrixType::row_t& v);

	//Quaternion
	quat pm_IdentityQuat();
	quat pm_MultiplyQuat(const quat& q1, const quat& q2);
	quat pm_ConjugateQuat(const quat& q);
	quat pm_InverseQuat(const quat& q);

	quat pm_SLerpQuat(const quat& q1, const quat& q2, const vec4& t);

	quat pm_RotationQuatFromXYZ(float angle_x, float angle_y, float angle_z);
	quat pm_RotationQuatFromXYZ(const vec3& angles);
	vec3 pm_RotationQuatToXYZ(const quat& rot);
	quat pm_RotationAxis(const vec3& axis, float angle);
	quat pm_RotationMatrixNormalized(const mat4& m);
	quat pm_RotationMatrix(const mat4& m);
	vec3 pm_RotateWithQuat(const quat& rot, const vec3& v);

	quat pm_RotateFromTo(const vec3& from, const vec3& to);
	quat pm_RotateFromTo(const vec3& from, const vec3& to, const vec3& fallback);

	//Color
	color pm_SetR(const color& v, float x);
	color pm_SetG(const color& v, float y);
	color pm_SetB(const color& v, float z);
	color pm_SetA(const color& v, float w);
	float pm_GetR(const color& v);
	float pm_GetG(const color& v);
	float pm_GetB(const color& v);
	float pm_GetA(const color& v);

	//Frame
	vec3 pm_GetXAxis(const frame& f);
	vec3 pm_GetYAxis(const frame& f);
	vec3 pm_GetZAxis(const frame& f);

	frame pm_IdentityFrame();
	frame pm_Set(const vec3& origin, const vec3& up, const vec3& forward);

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

	mat4 pm_ToMatrix(const frame& f, bool rotationOnly = false);
	mat4 pm_ToCameraMatrix(const frame& f, bool rotationOnly = false);

	frame pm_RotateLocalX(const frame& f, float angle);
	frame pm_RotateLocalY(const frame& f, float angle);
	frame pm_RotateLocalZ(const frame& f, float angle);

	frame pm_RotateWorld(const frame& f, float angle, const vec3& axis);
	frame pm_RotateLocal(const frame& f, float angle, const vec3& axis);

	vec3 pm_LocalToWorld(const frame& f, const vec3& v, bool rotationOnly = false);
	vec3 pm_WorldToLocal(const frame& f, const vec3& v);

	vec3 pm_TransformPoint(const frame& f, const vec3& off);
	vec3 pm_Rotate(const frame& f, const vec3& v);

#define _PM_MATH_INCLUDED_
# include "MathVector.inl"
# include "MathMatrix.inl"
# include "MathQuaternion.inl"
# include "MathColor.inl"
# include "MathFrame.inl"
#undef _PM_MATH_INCLUDED_
}
