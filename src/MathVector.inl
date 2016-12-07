/*
* Copyright(c) 2014, Ömercan Yazici <pearcoding AT gmail.com>
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

#ifndef _PM_MATH_INCLUDED_
# error MathVector.inl should only be included from Math.h
#endif

void PM_MATH_INLINE pm_Copy(vec& vdst, const vec& vsrc)
{
#ifdef PM_WITH_SIMD
	vdst = vsrc;
#else
	memcpy(vdst.v, vsrc.v, sizeof(float) * 4);
#endif
}

vec PM_MATH_INLINE pm_Zero()
{
#ifdef PM_WITH_SIMD
	return _mm_setzero_ps();
#else
	vec v;
	memset(v.v, 0, sizeof(v));
	return v;
#endif
}

vec PM_MATH_INLINE pm_One()
{
#ifdef PM_WITH_SIMD
	return _mm_set1_ps(1.0f);
#else
	vec v;
	v[0] = 1.0f;
	v[1] = 1.0f;
	v[2] = 1.0f;
	v[3] = 1.0f;
	return v;
#endif
}

vec PM_MATH_INLINE pm_Set(float x, float y, float z, float w)
{
#ifdef PM_WITH_SIMD
	return _mm_set_ps(w, z, y, x);
#else
	vec v;
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
	return v;
#endif
}

vec PM_MATH_INLINE pm_SetIndex(const vec& v, int index, float val)
{
#ifdef PM_WITH_SIMD
	vec r = v;
	((float *)&(r))[index] = val;
	return r;
#else
	vec r = v;
	r[index] = val;
	return r;
#endif
}

vec PM_MATH_INLINE pm_SetX(const vec& v, float x)
{
#ifdef PM_WITH_SIMD
	vec r = v;
	((float *)&(r))[0] = x;
	return r;
#else
	vec r = v;
	r[0] = x;
	return r;
#endif
}

vec PM_MATH_INLINE pm_SetY(const vec& v, float y)
{
#ifdef PM_WITH_SIMD
	vec r = v;
	((float *)&(r))[1] = y;
	return r;
#else
	vec r = v;
	r[1] = y;
	return r;
#endif
}

vec PM_MATH_INLINE pm_SetZ(const vec& v, float z)
{
#ifdef PM_WITH_SIMD
	vec r = v;
	((float *)&(r))[2] = z;
	return r;
#else
	vec r = v;
	r[2] = z;
	return r;
#endif
}

vec PM_MATH_INLINE pm_SetW(const vec& v, float w)
{
#ifdef PM_WITH_SIMD
	vec r = v;
	((float *)&(r))[3] = w;
	return r;
#else
	vec r = v;
	r[3] = w;
	return r;
#endif
}

float PM_MATH_INLINE pm_GetIndex(const vec& v, int index)
{
#ifdef PM_WITH_SIMD
	return ((float *)&(v))[index];
#else
	return v[index];
#endif
}

float PM_MATH_INLINE pm_GetX(const vec& v)
{
#ifdef PM_WITH_SIMD
	return ((float *)&(v))[0];
#else
	return v[0];
#endif
}

float PM_MATH_INLINE pm_GetY(const vec& v)
{
#ifdef PM_WITH_SIMD
	return ((float *)&(v))[1];
#else
	return v[1];
#endif
}

float PM_MATH_INLINE pm_GetZ(const vec& v)
{
#ifdef PM_WITH_SIMD
	return ((float *)&(v))[2];
#else
	return v[2];
#endif
}

float PM_MATH_INLINE pm_GetW(const vec& v)
{
#ifdef PM_WITH_SIMD
	return ((float *)&(v))[3];
#else
	return v[3];
#endif
}

vec PM_MATH_INLINE pm_FillVector(float val)
{
#ifdef PM_WITH_SIMD
	return _mm_set1_ps(val);
#else
	vec v;
	v[0] = val;
	v[1] = val;
	v[2] = val;
	v[3] = val;
	return v;
#endif
}

void PM_MATH_INLINE pm_Vec2Array(const vec& v, float* dst)
{
#ifdef PM_WITH_SIMD
	_mm_store_ps(&dst[0], v);
#else
	memcpy(const_cast<float*>(v.v), dst, sizeof(float) * 4);
#endif
}

vec PM_MATH_INLINE pm_IsEqualv(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_cmpeq_ps(v1, v2);
#else
	vec r;
	r[0] = (float)((v1[0] == v2[0]) ? 0xFFFFFFFF : 0);
	r[1] = (float)((v1[1] == v2[1]) ? 0xFFFFFFFF : 0);
	r[2] = (float)((v1[2] == v2[2]) ? 0xFFFFFFFF : 0);
	r[3] = (float)((v1[3] == v2[3]) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

bool PM_MATH_INLINE pm_IsEqual(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return (_mm_movemask_ps(_mm_cmpeq_ps(v1, v2)) & 0xF) == 0xF;
#else
	return (v1[0] == v2[0] && v1[1] == v2[1] && v1[2] == v2[2] && v1[3] == v2[3]);
#endif
}

vec PM_MATH_INLINE pm_IsNearlyEqualv(const vec& v1, const vec& v2, const vec& delta)
{
#ifdef PM_WITH_SIMD
	vec del = _mm_sub_ps(v1, v2);
	vec abs = _mm_max_ps(_mm_sub_ps(_mm_setzero_ps(), del), del);
	return _mm_cmple_ps(abs, delta);
#else
	vec r;
	r[0] = (float)((fabs(v1[0] - v2[0]) <= delta[0]) ? 0xFFFFFFFF : 0);
	r[1] = (float)((fabs(v1[1] - v2[1]) <= delta[1]) ? 0xFFFFFFFF : 0);
	r[2] = (float)((fabs(v1[2] - v2[2]) <= delta[2]) ? 0xFFFFFFFF : 0);
	r[3] = (float)((fabs(v1[3] - v2[3]) <= delta[3]) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

bool PM_MATH_INLINE pm_IsNearlyEqual(const vec& v1, const vec& v2, const vec& delta)
{
#ifdef PM_WITH_SIMD
	vec del = _mm_sub_ps(v1, v2);
	vec abs = _mm_max_ps(_mm_sub_ps(_mm_setzero_ps(), del), del);
	return (_mm_movemask_ps(_mm_cmple_ps(abs, delta)) & 0xF) == 0xF;
#else
	return ((fabsf(v1[0] - v2[0]) <= delta[0]) && (fabsf(v1[1] - v2[1]) <= delta[1]) &&
		(fabsf(v1[2] - v2[2]) <= delta[2]) && (fabsf(v1[3] - v2[3]) <= delta[3]));
#endif
}

vec PM_MATH_INLINE pm_IsNotEqualv(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_cmpneq_ps(v1, v2);
#else
	vec r;
	r[0] = (float)((v1[0] != v2[0]) ? 0xFFFFFFFF : 0);
	r[1] = (float)((v1[1] != v2[1]) ? 0xFFFFFFFF : 0);
	r[2] = (float)((v1[2] != v2[2]) ? 0xFFFFFFFF : 0);
	r[3] = (float)((v1[3] != v2[3]) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

bool PM_MATH_INLINE pm_IsNotEqual(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return (_mm_movemask_ps(_mm_cmpneq_ps(v1, v2)) & 0xF) == 0xF;
#else
	return (v1[0] != v2[0] && v1[1] != v2[1] && v1[2] != v2[2] && v1[3] != v2[3]);
#endif
}

vec PM_MATH_INLINE pm_IsLessv(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_cmplt_ps(v1, v2);
#else
	vec r;
	r[0] = (float)((v1[0] < v2[0]) ? 0xFFFFFFFF : 0);
	r[1] = (float)((v1[1] < v2[1]) ? 0xFFFFFFFF : 0);
	r[2] = (float)((v1[2] < v2[2]) ? 0xFFFFFFFF : 0);
	r[3] = (float)((v1[3] < v2[3]) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

bool PM_MATH_INLINE pm_IsLess(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return (_mm_movemask_ps(_mm_cmplt_ps(v1, v2)) & 0xF) == 0xF;
#else
	return (v1[0] < v2[0] && v1[1] < v2[1] && v1[2] < v2[2] && v1[3] < v2[3]);
#endif
}

vec PM_MATH_INLINE pm_IsLessOrEqualv(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_cmple_ps(v1, v2);
#else
	vec r;
	r[0] = (float)((v1[0] <= v2[0]) ? 0xFFFFFFFF : 0);
	r[1] = (float)((v1[1] <= v2[1]) ? 0xFFFFFFFF : 0);
	r[2] = (float)((v1[2] <= v2[2]) ? 0xFFFFFFFF : 0);
	r[3] = (float)((v1[3] <= v2[3]) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

bool PM_MATH_INLINE pm_IsLessOrEqual(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return (_mm_movemask_ps(_mm_cmple_ps(v1, v2)) & 0xF) == 0xF;
#else
	return (v1[0] <= v2[0] && v1[1] <= v2[1] && v1[2] <= v2[2] && v1[3] <= v2[3]);
#endif
}

vec PM_MATH_INLINE pm_IsGreaterv(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_cmpgt_ps(v1, v2);
#else
	vec r;
	r[0] = (float)((v1[0] > v2[0]) ? 0xFFFFFFFF : 0);
	r[1] = (float)((v1[1] > v2[1]) ? 0xFFFFFFFF : 0);
	r[2] = (float)((v1[2] > v2[2]) ? 0xFFFFFFFF : 0);
	r[3] = (float)((v1[3] > v2[3]) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

bool PM_MATH_INLINE pm_IsGreater(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return (_mm_movemask_ps(_mm_cmpgt_ps(v1, v2)) & 0xF) == 0xF;
#else
	return (v1[0] > v2[0] && v1[1] > v2[1] && v1[2] > v2[2] && v1[3] > v2[3]);
#endif
}

vec PM_MATH_INLINE pm_IsGreaterOrEqualv(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_cmpge_ps(v1, v2);
#else
	vec r;
	r[0] = (float)((v1[0] >= v2[0]) ? 0xFFFFFFFF : 0);
	r[1] = (float)((v1[1] >= v2[1]) ? 0xFFFFFFFF : 0);
	r[2] = (float)((v1[2] >= v2[2]) ? 0xFFFFFFFF : 0);
	r[3] = (float)((v1[3] >= v2[3]) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

bool PM_MATH_INLINE pm_IsGreaterOrEqual(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return (_mm_movemask_ps(_mm_cmpge_ps(v1, v2)) & 0xF) == 0xF;
#else
	return (v1[0] >= v2[0] && v1[1] >= v2[1] && v1[2] >= v2[2] && v1[3] >= v2[3]);
#endif
}

vec PM_MATH_INLINE pm_IsInBounds(const vec& v, const vec& bounds)
{
#ifdef PM_WITH_SIMD
	//Check if less or equal than negate and check again if less or equal. Next "and" both results.
	vec tmp1 = _mm_cmple_ps(v, bounds);
	vec tmp2 = _mm_mul_ps(bounds, _mm_set1_ps(-1.0f));
	tmp2 = _mm_cmple_ps(tmp2, v);
	return _mm_and_ps(tmp1, tmp2);
#else
	vec r;
	r[0] = (float)((v[0] <= bounds[0] && v[0] >= -bounds[0]) ? 0xFFFFFFFF : 0);
	r[1] = (float)((v[1] <= bounds[1] && v[1] >= -bounds[1]) ? 0xFFFFFFFF : 0);
	r[2] = (float)((v[2] <= bounds[2] && v[2] >= -bounds[2]) ? 0xFFFFFFFF : 0);
	r[3] = (float)((v[3] <= bounds[3] && v[3] >= -bounds[3]) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

vec PM_MATH_INLINE pm_Negate(const vec& v)
{
#ifdef PM_WITH_SIMD
	return _mm_sub_ps(_mm_set1_ps(0.0f), v);
#else
	vec r;
	r[0] = -v[0];
	r[1] = -v[1];
	r[2] = -v[2];
	r[3] = -v[3];
	return r;
#endif
}

vec PM_MATH_INLINE pm_Add(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_add_ps(v1, v2);
#else
	vec r;
	r[0] = v1[0] + v2[0];
	r[1] = v1[1] + v2[1];
	r[2] = v1[2] + v2[2];
	r[3] = v1[3] + v2[3];
	return r;
#endif
}

vec PM_MATH_INLINE pm_Subtract(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_sub_ps(v1, v2);
#else
	vec r;
	r[0] = v1[0] - v2[0];
	r[1] = v1[1] - v2[1];
	r[2] = v1[2] - v2[2];
	r[3] = v1[3] - v2[3];
	return r;
#endif
}

vec PM_MATH_INLINE pm_Multiply(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_mul_ps(v1, v2);
#else
	vec r;
	r[0] = v1[0] * v2[0];
	r[1] = v1[1] * v2[1];
	r[2] = v1[2] * v2[2];
	r[3] = v1[3] * v2[3];
	return r;
#endif
}

vec PM_MATH_INLINE pm_Divide(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_div_ps(v1, v2);
#else
	vec r;
	r[0] = v1[0] / v2[0];
	r[1] = v1[1] / v2[1];
	r[2] = v1[2] / v2[2];
	r[3] = v1[3] / v2[3];
	return r;
#endif
}

vec PM_MATH_INLINE pm_Scale(const vec& v, float factor)
{
#ifdef PM_WITH_SIMD
	return _mm_mul_ps(v, _mm_set1_ps(factor));
#else
	vec r;
	r[0] = v[0] * factor;
	r[1] = v[1] * factor;
	r[2] = v[2] * factor;
	r[3] = v[3] * factor;
	return r;
#endif
}

vec PM_MATH_INLINE pm_MultiplyAdd(const vec& v1, const vec& v2, const vec& v3)
{
#ifdef PM_WITH_SIMD
	return _mm_add_ps(_mm_mul_ps(v1, v2), v3);
#else
	vec r;
	r[0] = (v1[0] * v2[0]) + v3[0];
	r[1] = (v1[1] * v2[1]) + v3[1];
	r[2] = (v1[2] * v2[2]) + v3[2];
	r[3] = (v1[3] * v2[3]) + v3[3];
	return r;
#endif
}

vec PM_MATH_INLINE pm_Reciprocal(const vec& v)
{
#ifdef PM_WITH_SIMD
	return _mm_rcp_ps(v);
#else
	//Faster?
	vec r;
	r[0] = 1.0f / v[0];
	r[1] = 1.0f / v[1];
	r[2] = 1.0f / v[2];
	r[3] = 1.0f / v[3];
	return r;
#endif
}

vec PM_MATH_INLINE pm_Reciprocal(const vec& v, int n)
{
	vec x = pm_Scale(v, 0.25f);

	vec v2 = pm_FillVector(2);
	for (int i = 0; i < n; ++i)
		x = pm_Multiply(x, pm_Subtract(v2, pm_Multiply(v, x)));

	return x;
}

/*
 * @todo This is not good enough
 */
vec PM_MATH_INLINE pm_Sqrt(const vec& v)
{
#ifdef PM_WITH_SIMD
	return _mm_sqrt_ps(v);
#else
	return pm_Multiply(v, pm_ReciprocalSqrt(v));
#endif
}

vec PM_MATH_INLINE pm_ReciprocalSqrt(const vec& v)
{
#ifdef PM_WITH_SIMD
	return _mm_rsqrt_ps(v);
#else
	union
	{
		vec V;
		long I[4];
	} val;

	vec half = pm_Scale(v, 0.5f);
	val.V = v;
	val.I[0] = 0x5F3759DF - (val.I[0] >> 1);
	val.I[1] = 0x5F3759DF - (val.I[1] >> 1);
	val.I[2] = 0x5F3759DF - (val.I[2] >> 1);
	val.I[3] = 0x5F3759DF - (val.I[3] >> 1);

# if pm_MATH_RECIPROCALSQRT_QUAL >= 2
	val.V[0] = val.V[0] * (1.5f - half[0] * val.V[0] * val.V[0]);
	val.V[1] = val.V[1] * (1.5f - half[1] * val.V[1] * val.V[1]);
	val.V[2] = val.V[2] * (1.5f - half[2] * val.V[2] * val.V[2]);
	val.V[3] = val.V[3] * (1.5f - half[3] * val.V[3] * val.V[3]);

	val.V[0] = val.V[0] * (1.5f - half[0] * val.V[0] * val.V[0]);
	val.V[1] = val.V[1] * (1.5f - half[1] * val.V[1] * val.V[1]);
	val.V[2] = val.V[2] * (1.5f - half[2] * val.V[2] * val.V[2]);
	val.V[3] = val.V[3] * (1.5f - half[3] * val.V[3] * val.V[3]);

	val.V[0] = val.V[0] * (1.5f - half[0] * val.V[0] * val.V[0]);
	val.V[1] = val.V[1] * (1.5f - half[1] * val.V[1] * val.V[1]);
	val.V[2] = val.V[2] * (1.5f - half[2] * val.V[2] * val.V[2]);
	val.V[3] = val.V[3] * (1.5f - half[3] * val.V[3] * val.V[3]);
# endif

# if pm_MATH_RECIPROCALSQRT_QUAL >= 1
	val.V[0] = val.V[0] * (1.5f - half[0] * val.V[0] * val.V[0]);
	val.V[1] = val.V[1] * (1.5f - half[1] * val.V[1] * val.V[1]);
	val.V[2] = val.V[2] * (1.5f - half[2] * val.V[2] * val.V[2]);
	val.V[3] = val.V[3] * (1.5f - half[3] * val.V[3] * val.V[3]);
# endif

	val.V[0] = val.V[0] * (1.5f - (half[0] * val.V[0] * val.V[0]));
	val.V[1] = val.V[1] * (1.5f - (half[1] * val.V[1] * val.V[1]));
	val.V[2] = val.V[2] * (1.5f - (half[2] * val.V[2] * val.V[2]));
	val.V[3] = val.V[3] * (1.5f - (half[3] * val.V[3] * val.V[3]));
	return val.V;
#endif
}

vec PM_MATH_INLINE pm_Pow(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_pow_ps(v1, v2);
# else
	//I'm not happy with this :(
	return _mm_setr_ps(
		std::pow(pm_GetX(v1), pm_GetX(v2)),
		std::pow(pm_GetY(v1), pm_GetY(v2)),
		std::pow(pm_GetZ(v1), pm_GetZ(v2)),
		std::pow(pm_GetW(v1), pm_GetW(v2)));
	//return pm_Exp(_mm_mul_ps(pm_Log(v1), v2));
# endif
#else
	vec r;
	r[0] = std::pow(v1[0], v2[0]);
	r[1] = std::pow(v1[1], v2[1]);
	r[2] = std::pow(v1[2], v2[2]);
	r[3] = std::pow(v1[3], v2[3]);
	return r;
	//return pm_Exp(pm_Multiply(pm_Log(v1), v2));
#endif
}

/*
 * @todo Implement own approximation...
 */
vec PM_MATH_INLINE pm_Exp(const vec& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_exp_ps(v);
# else
	return _mm_setr_ps(
		std::exp(pm_GetX(v)),
		std::exp(pm_GetY(v)),
		std::exp(pm_GetZ(v)),
		std::exp(pm_GetW(v)));
# endif
#else
	vec r;
	r[0] = std::exp(v[0]);
	r[1] = std::exp(v[1]);
	r[2] = std::exp(v[2]);
	r[3] = std::exp(v[3]);
	return r;
#endif
}

vec PM_MATH_INLINE pm_Exp2(const vec& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_exp2_ps(v);
# else
#  ifdef PM_WITH_CPP11
	return _mm_setr_ps(
		std::exp2(pm_GetX(v)),
		std::exp2(pm_GetY(v)),
		std::exp2(pm_GetZ(v)),
		std::exp2(pm_GetW(v)));
#  else
	return _mm_setr_ps(
		std::exp(pm_GetX(v)*PM_LOG_2_F),
		std::exp(pm_GetY(v)*PM_LOG_2_F),
		std::exp(pm_GetZ(v)*PM_LOG_2_F),
		std::exp(pm_GetW(v)*PM_LOG_2_F));
#  endif//PM_WITH_CPP11
# endif// PM_WITH_SVML
#else
	vec r;
# ifdef PM_WITH_CPP11
	r[0] = std::exp2(v[0]);
	r[1] = std::exp2(v[1]);
	r[2] = std::exp2(v[2]);
	r[3] = std::exp2(v[3]);
# else
	r[0] = std::exp(v[0]*PM_LOG_2_F);
	r[1] = std::exp(v[1]*PM_LOG_2_F);
	r[2] = std::exp(v[2]*PM_LOG_2_F);
	r[3] = std::exp(v[3]*PM_LOG_2_F);
# endif//PM_WITH_CPP11
	return r;
#endif
}

vec PM_MATH_INLINE pm_Exp10(const vec& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_exp10_ps(v);
# else
	return _mm_setr_ps(
		std::exp(pm_GetX(v)*PM_LOG_10_F),
		std::exp(pm_GetY(v)*PM_LOG_10_F),
		std::exp(pm_GetZ(v)*PM_LOG_10_F),
		std::exp(pm_GetW(v)*PM_LOG_10_F));
# endif// PM_WITH_SVML
#else
	vec r;
	r[0] = std::exp(v[0]*PM_LOG_10_F);
	r[1] = std::exp(v[1]*PM_LOG_10_F);
	r[2] = std::exp(v[2]*PM_LOG_10_F);
	r[3] = std::exp(v[3]*PM_LOG_10_F);
	return r;
#endif
}

vec PM_MATH_INLINE pm_ExpM1(const vec& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_expm1_ps(v);
# else
#  ifdef PM_WITH_CPP11
	return _mm_setr_ps(
		std::expm1(pm_GetX(v)),
		std::expm1(pm_GetY(v)),
		std::expm1(pm_GetZ(v)),
		std::expm1(pm_GetW(v)));
#  else
	static const vec minus = _mm_set1_ps(-1.0f); 
	return pm_Add(_mm_setr_ps(
		std::exp(pm_GetX(v)),
		std::exp(pm_GetY(v)),
		std::exp(pm_GetZ(v)),
		std::exp(pm_GetW(v))),
	minus);
#  endif//PM_WITH_CPP11
# endif// PM_WITH_SVML
#else
	vec r;
# ifdef PM_WITH_CPP11
	r[0] = std::expm1(v[0]);
	r[1] = std::expm1(v[1]);
	r[2] = std::expm1(v[2]);
	r[3] = std::expm1(v[3]);
# else
	r[0] = std::exp(v[0]) - 1;
	r[1] = std::exp(v[1]) - 1;
	r[2] = std::exp(v[2]) - 1;
	r[3] = std::exp(v[3]) - 1;
# endif//PM_WITH_CPP11
	return r;
#endif
}

/*
 * @todo Implement own approximation...
 */
vec PM_MATH_INLINE pm_Log(const vec& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_log_ps(v);
# else
	return _mm_setr_ps(
		std::log(pm_GetX(v)),
		std::log(pm_GetY(v)),
		std::log(pm_GetZ(v)),
		std::log(pm_GetW(v)));
# endif//PM_WITH_SVML
#else
	vec r;
	r[0] = std::log(v[0]);
	r[1] = std::log(v[1]);
	r[2] = std::log(v[2]);
	r[3] = std::log(v[3]);
	return r;
#endif
}

vec PM_MATH_INLINE pm_Log2(const vec& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_log2_ps(v);
# else
#  ifdef PM_WITH_CPP11
	return _mm_setr_ps(
		std::log2(pm_GetX(v)),
		std::log2(pm_GetY(v)),
		std::log2(pm_GetZ(v)),
		std::log2(pm_GetW(v)));
#  else
	return _mm_setr_ps(
		std::log(pm_GetX(v))*PM_INV_LOG_2_F,
		std::log(pm_GetY(v))*PM_INV_LOG_2_F,
		std::log(pm_GetZ(v))*PM_INV_LOG_2_F,
		std::log(pm_GetW(v))*PM_INV_LOG_2_F);
#  endif//PM_WITH_CPP11
# endif//PM_WITH_SVML
#else
	vec r;
# ifdef PM_WITH_CPP11
	r[0] = std::log2(v[0]);
	r[1] = std::log2(v[1]);
	r[2] = std::log2(v[2]);
	r[3] = std::log2(v[3]);
# else
	r[0] = std::log(v[0])*PM_INV_LOG_2_F;
	r[1] = std::log(v[1])*PM_INV_LOG_2_F;
	r[2] = std::log(v[2])*PM_INV_LOG_2_F;
	r[3] = std::log(v[3])*PM_INV_LOG_2_F;
# endif//PM_WITH_CPP11
	return r;
#endif
}

vec PM_MATH_INLINE pm_Log10(const vec& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_log10_ps(v);
# else
	return _mm_setr_ps(
		std::log10(pm_GetX(v)),
		std::log10(pm_GetY(v)),
		std::log10(pm_GetZ(v)),
		std::log10(pm_GetW(v)));
# endif//PM_WITH_SVML
#else
	vec r;
	r[0] = std::log10(v[0]);
	r[1] = std::log10(v[1]);
	r[2] = std::log10(v[2]);
	r[3] = std::log10(v[3]);
	return r;
#endif
}

vec PM_MATH_INLINE pm_Log1P(const vec& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_log1p_ps(v);
# else
#  ifdef PM_WITH_CPP11
	return _mm_setr_ps(
		std::log1p(pm_GetX(v)),
		std::log1p(pm_GetY(v)),
		std::log1p(pm_GetZ(v)),
		std::log1p(pm_GetW(v)));
#  else
	static const vec plus = _mm_set1_ps(1.0f);
	return pm_Add(_mm_setr_ps(
		std::log(pm_GetX(v)),
		std::log(pm_GetY(v)),
		std::log(pm_GetZ(v)),
		std::log(pm_GetW(v))),
	plus);
#  endif//PM_WITH_CPP11
# endif//PM_WITH_SVML
#else
	vec r;
# ifdef PM_WITH_CPP11
	r[0] = std::log1p(v[0]);
	r[1] = std::log1p(v[1]);
	r[2] = std::log1p(v[2]);
	r[3] = std::log1p(v[3]);
# else
	r[0] = std::log(v[0]+1);
	r[1] = std::log(v[1]+1);
	r[2] = std::log(v[2]+1);
	r[3] = std::log(v[3]+1);
# endif//PM_WITH_CPP11
	return r;
#endif
}

/*
 * Sin range [-PI,PI]
 * x^21/51090942171709440000-x^19/121645100408832000+x^17/355687428096000-
 x^15/1307674368000+x^13/6227020800-x^11/39916800+x^9/362880-x^7/5040+x^5/120-x^3/6+x
 */
vec PM_MATH_INLINE pm_Sin(const vec& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_sin_ps(v);
# else
	vec s = _mm_mul_ps(v, v);
	vec p = _mm_mul_ps(v, s);
	vec r = _mm_add_ps(v, _mm_mul_ps(_mm_set1_ps(-0.16666666666667f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(0.0083333333333333f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(-1.9841269841269841e-4f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(2.7557319223985893e-6f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(-2.505210838544172e-8f), p));

#  if PM_MATH_SIN_QUAL >= 1
	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(1.6059043836821613e-10f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(-7.6471637318198164e-13f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(2.8114572543455206e-15f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(-8.2206352466243295e-18f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(1.9572941063391263e-20f), p));
#  endif//PM_MATH_SIN_QUAL >= 1
	return r;
# endif//PM_WITH_SVML
#else
	vec r;
	r[0] = std::sin(v[0]);
	r[1] = std::sin(v[1]);
	r[2] = std::sin(v[2]);
	r[3] = std::sin(v[3]);
	return r;
#endif
}

/*
 * Cos range [-PI, PI]
 * x^20/2432902008176640000-x^18/6402373705728000+x^16/20922789888000-x^14/87178291200
 +x^12/479001600-x^10/3628800+x^8/40320-x^6/720+x^4/24-x^2/2+1
 */
vec PM_MATH_INLINE pm_Cos(const vec& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_cos_ps(v);
# else
	vec s = _mm_mul_ps(v, v);//x^2
	vec r = _mm_add_ps(_mm_set1_ps(1), _mm_mul_ps(_mm_set1_ps(-0.5f), s));//-x^2/2+1

	vec p = _mm_mul_ps(s, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(0.041666666666667f), p));//x^4/24-x^2/2+1

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(-0.0013888888888889f), p));//-x^6/720+x^4/24-x^2/2+1

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(2.4801587301587302e-5f), p));//x^8/40320-x^6/720+x^4/24-x^2/2+1

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(-2.7557319223985888e-7f), p));//-x^10/3628800+x^8/40320-x^6/720+x^4/24-x^2/2+1

#  if PM_MATH_COS_QUAL >= 1
	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(2.08767569878681e-9f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(-1.1470745597729725e-11f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(4.7794773323873853e-14f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(-1.5619206968586225e-16f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(4.1103176233121648e-19f), p));
#  endif//PM_MATH_COS_QUAL >= 1
	return r;
# endif//PM_WITH_SVML
#else
	/*
	 * Use this or the c implementation from above??
	 */
	vec r;
	r[0] = std::cos(v[0]);
	r[1] = std::cos(v[1]);
	r[2] = std::cos(v[2]);
	r[3] = std::cos(v[3]);
	return r;
#endif
}

template<>
void PM_MATH_INLINE pm_SinCos<vec>(const vec& v, vec& s, vec& c)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	s = _mm_sincos_ps(&c, v);
# else
	vec s2 = _mm_mul_ps(v, v);
	vec sinp = _mm_mul_ps(v, s2);
	vec cosp = _mm_mul_ps(s2, s2);

	s = _mm_add_ps(v, _mm_mul_ps(_mm_set1_ps(-0.16666666666667f), sinp));
	c = _mm_add_ps(_mm_set1_ps(1), _mm_mul_ps(_mm_set1_ps(-0.5f), s2));

	sinp = _mm_mul_ps(sinp, s2);
	s = _mm_add_ps(s, _mm_mul_ps(_mm_set1_ps(0.0083333333333333f), sinp));
	c = _mm_add_ps(c, _mm_mul_ps(_mm_set1_ps(0.041666666666667f), cosp));

	sinp = _mm_mul_ps(sinp, s2);
	s = _mm_add_ps(s, _mm_mul_ps(_mm_set1_ps(-1.9841269841269841e-4f), sinp));
	cosp = _mm_mul_ps(cosp, s2);
	c = _mm_add_ps(c, _mm_mul_ps(_mm_set1_ps(-0.0013888888888889f), cosp));

	sinp = _mm_mul_ps(sinp, s2);
	s = _mm_add_ps(s, _mm_mul_ps(_mm_set1_ps(2.7557319223985893e-6f), sinp));
	cosp = _mm_mul_ps(cosp, s2);
	c = _mm_add_ps(c, _mm_mul_ps(_mm_set1_ps(2.4801587301587302e-5f), cosp));

	sinp = _mm_mul_ps(sinp, s2);
	s = _mm_add_ps(s, _mm_mul_ps(_mm_set1_ps(-2.505210838544172e-8f), sinp));
	cosp = _mm_mul_ps(cosp, s2);
	c = _mm_add_ps(c, _mm_mul_ps(_mm_set1_ps(-2.7557319223985888e-7f), cosp));

#  if PM_MATH_SIN_QUAL == 1 || PM_MATH_COS_QUAL == 1
	sinp = _mm_mul_ps(sinp, s2);
	s = _mm_add_ps(s, _mm_mul_ps(_mm_set1_ps(1.6059043836821613e-10f), sinp));
	cosp = _mm_mul_ps(cosp, s2);
	c = _mm_add_ps(c, _mm_mul_ps(_mm_set1_ps(2.08767569878681e-9f), cosp));

	sinp = _mm_mul_ps(sinp, s2);
	s = _mm_add_ps(s, _mm_mul_ps(_mm_set1_ps(-7.6471637318198164e-13f), sinp));
	cosp = _mm_mul_ps(cosp, s2);
	c = _mm_add_ps(c, _mm_mul_ps(_mm_set1_ps(-1.1470745597729725e-11f), cosp));

	sinp = _mm_mul_ps(sinp, s2);
	s = _mm_add_ps(s, _mm_mul_ps(_mm_set1_ps(2.8114572543455206e-15f), sinp));
	cosp = _mm_mul_ps(cosp, s2);
	c = _mm_add_ps(c, _mm_mul_ps(_mm_set1_ps(4.7794773323873853e-14f), cosp));

	sinp = _mm_mul_ps(sinp, s2);
	s = _mm_add_ps(s, _mm_mul_ps(_mm_set1_ps(-8.2206352466243295e-18f), sinp));
	cosp = _mm_mul_ps(cosp, s2);
	c = _mm_add_ps(c, _mm_mul_ps(_mm_set1_ps(-1.5619206968586225e-16f), cosp));

	sinp = _mm_mul_ps(sinp, s2);
	s = _mm_add_ps(s, _mm_mul_ps(_mm_set1_ps(1.9572941063391263e-20f), sinp));
	cosp = _mm_mul_ps(cosp, s2);
	c = _mm_add_ps(c, _mm_mul_ps(_mm_set1_ps(4.1103176233121648e-19f), cosp));
#  endif//PM_MATH_SIN_QUAL == 1 || PM_MATH_COS_QUAL == 1
# endif//PM_WITH_SVML
#else
	pm_SinCos(v[0], s[0], c[0]);
	pm_SinCos(v[1], s[1], c[1]);
	pm_SinCos(v[2], s[2], c[2]);
	pm_SinCos(v[3], s[3], c[3]);
#endif
}

/*
 * Tan range [0, PI/2]
 * (32207686319158956594455462*x^39)/1126482925555250126673224649609375+(56518638202982204522669764*x^37)/801155872830791925447758961328125
 +(418781231495293038913922*x^35)/2405873491984360136479756640625+(1736640792209901647222*x^33)/4043484860477916195764296875+
 (129848163681107301953*x^31)/122529844256906551386796875+(689005380505609448*x^29)/263505041412702261046875+(8374643517010684*x^27)/1298054391195577640625+
 (58870668456604*x^25)/3698160658676859375+(113927491862*x^23)/2900518163668125+(18888466084*x^21)/194896477400625+(443861162*x^19)/1856156927625+
 (6404582*x^17)/10854718875+(929569*x^15)/638512875+(21844*x^13)/6081075+(1382*x^11)/155925+(62*x^9)/2835+(17*x^7)/315+(2*x^5)/15+x^3/3+x
 * @todo I think this is a little slow and has a bad approximation... Change this
 */
vec PM_MATH_INLINE pm_Tan(const vec& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_tan_ps(v);
# else
	return _mm_setr_ps(
		std::tan(pm_GetX(v)),
		std::tan(pm_GetY(v)),
		std::tan(pm_GetZ(v)),
		std::tan(pm_GetW(v)));
# endif//PM_WITH_SVML
#else
	/*
	 * Use this or the c implementation from above??
	 */
	vec r;
	r[0] = std::tan(v[0]);
	r[1] = std::tan(v[1]);
	r[2] = std::tan(v[2]);
	r[3] = std::tan(v[3]);
	return r;
#endif
}

/*
 * @todo Implement own approximation...
 */
vec PM_MATH_INLINE pm_ASin(const vec& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_asin_ps(v);
# else
	return _mm_setr_ps(
		std::asin(pm_GetX(v)),
		std::asin(pm_GetY(v)),
		std::asin(pm_GetZ(v)),
		std::asin(pm_GetW(v)));
# endif
#else
	vec r;
	r[0] = std::asin(v[0]);
	r[1] = std::asin(v[1]);
	r[2] = std::asin(v[2]);
	r[3] = std::asin(v[3]);
	return r;
#endif
}

/*
 * @todo Implement own approximation...
 */
vec PM_MATH_INLINE pm_ACos(const vec& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_acos_ps(v);
# else
	return _mm_setr_ps(
		std::acos(pm_GetX(v)),
		std::acos(pm_GetY(v)),
		std::acos(pm_GetZ(v)),
		std::acos(pm_GetW(v)));
# endif//PM_WITH_SVML
#else
	vec r;
	r[0] = std::acos(v[0]);
	r[1] = std::acos(v[1]);
	r[2] = std::acos(v[2]);
	r[3] = std::acos(v[3]);
	return r;
#endif
}

/*
 * @todo Implement own approximation...
 */
vec PM_MATH_INLINE pm_ATan(const vec& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_atan_ps(v);
# else
	return _mm_setr_ps(
		std::atan(pm_GetX(v)),
		std::atan(pm_GetY(v)),
		std::atan(pm_GetZ(v)),
		std::atan(pm_GetW(v)));
# endif//PM_WITH_SVML
#else
	vec r;
	r[0] = std::atan(v[0]);
	r[1] = std::atan(v[1]);
	r[2] = std::atan(v[2]);
	r[3] = std::atan(v[3]);
	return r;
#endif
}

vec PM_MATH_INLINE pm_Lerp(const vec& v1, const vec& v2, const vec& t)
{
#ifdef PM_WITH_SIMD
	return _mm_add_ps(_mm_mul_ps(_mm_sub_ps(v2, v1), t), v1);
#else
	return pm_Add(v1, pm_Multiply(pm_Subtract(v2, v1), t));
#endif
}

vec PM_MATH_INLINE pm_NLerp(const vec& v1, const vec& v2, const vec& t)
{
#ifdef PM_WITH_SIMD
	return pm_Normalize4D(_mm_add_ps(_mm_mul_ps(_mm_sub_ps(v2, v1), t), v1));
#else
	return pm_Normalize4D(pm_Add(v1, pm_Multiply(pm_Subtract(v2, v1), t)));
#endif
}

vec PM_MATH_INLINE pm_SLerp(const vec& v1, const vec& v2, const vec& t)
{
#ifdef PM_WITH_SIMD
	static const vec lerp_threshold = _mm_set1_ps(PM_VECTOR_SLERP_THRESHOLD);
	static const vec one = _mm_set1_ps(1.0f);

	vec cosAngle = _mm_set1_ps(pm_Dot4D(v1, v2));
	vec mask = _mm_cmpgt_ps(lerp_threshold, cosAngle);
	vec angle = pm_ACos(cosAngle);
	vec decWeight = _mm_sub_ps(one, t);
	vec angles = _mm_unpacklo_ps(one, t);
	angles = _mm_unpacklo_ps(angles, decWeight);
	angles = _mm_mul_ps(angles, angle);
	vec sines = pm_Sin(angles);
	vec scales = _mm_div_ps(sines, _mm_shuffle_ps(sines, sines, _MM_SHUFFLE(0, 0, 0, 0)));
	vec scale0 = _mm_or_ps(_mm_and_ps(mask, _mm_shuffle_ps(scales, scales, _MM_SHUFFLE(1, 1, 1, 1))), _mm_andnot_ps(mask, decWeight));
	vec scale1 = _mm_or_ps(_mm_and_ps(mask, _mm_shuffle_ps(scales, scales, _MM_SHUFFLE(2, 2, 2, 2))), _mm_andnot_ps(mask, t));
	return _mm_add_ps(_mm_mul_ps(v2, scale1), _mm_mul_ps(v1, scale0));

#else
	float angle = pm_Dot4D(v1, v2);

	if (angle > PM_VECTOR_SLERP_THRESHOLD)
	{
		return pm_Lerp(v1, v2, t);
	}

	angle = std::acos(angle);
	vec r;
	r[0] = (v1[0] * std::sin(angle*(1 - t[0])) + v2[0] * std::sin(angle*t[0])) / std::sin(angle);
	r[1] = (v1[1] * std::sin(angle*(1 - t[1])) + v2[1] * std::sin(angle*t[1])) / std::sin(angle);
	r[2] = (v1[2] * std::sin(angle*(1 - t[2])) + v2[2] * std::sin(angle*t[2])) / std::sin(angle);
	r[3] = (v1[3] * std::sin(angle*(1 - t[3])) + v2[3] * std::sin(angle*t[3])) / std::sin(angle);
	return r;
#endif
}

template<>
vec PM_MATH_INLINE pm_Max<vec>(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_max_ps(v1, v2);
#else
	vec r;
	r[0] = (v1[0] < v2[0]) ? v1[0] : v2[0];
	r[1] = (v1[1] < v2[1]) ? v1[1] : v2[1];
	r[2] = (v1[2] < v2[2]) ? v1[2] : v2[2];
	r[3] = (v1[3] < v2[3]) ? v1[3] : v2[3];
	return r;
#endif
}

template<>
vec PM_MATH_INLINE pm_Min<vec>(const vec& v1, const vec& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_min_ps(v1, v2);
#else
	vec r;
	r[0] = (v1[0] > v2[0]) ? v1[0] : v2[0];
	r[1] = (v1[1] > v2[1]) ? v1[1] : v2[1];
	r[2] = (v1[2] > v2[2]) ? v1[2] : v2[2];
	r[3] = (v1[3] > v2[3]) ? v1[3] : v2[3];
	return r;
#endif
}

template<>
vec PM_MATH_INLINE pm_Clamp<vec>(const vec& v, const vec& min, const vec& max)
{
	PM_DEBUG_ASSERT(pm_IsLessOrEqual(min, max));
#ifdef PM_WITH_SIMD
	return _mm_min_ps(_mm_max_ps(min, v), max);
#else
	vec r;
	r[0] = v[0] < min[0] ? min[0] : (v[0] > max[0] ? max[0] : v[0]);
	r[1] = v[1] < min[1] ? min[1] : (v[1] > max[1] ? max[1] : v[1]);
	r[2] = v[2] < min[2] ? min[2] : (v[2] > max[2] ? max[2] : v[2]);
	r[3] = v[3] < min[3] ? min[3] : (v[3] > max[3] ? max[3] : v[3]);
	return r;
#endif
}

vec PM_MATH_INLINE pm_Abs(const vec& v)
{
#ifdef PM_WITH_SIMD
	static const vec sign_mask = _mm_set1_ps(-0.0f);
    return _mm_andnot_ps(sign_mask, v);
#else
	vec r;
	r[0] = std::abs(v[0]);
	r[1] = std::abs(v[1]);
	r[2] = std::abs(v[2]);
	r[3] = std::abs(v[3]);
	return r;
#endif
}

vec PM_MATH_INLINE pm_Saturate(const vec& v)
{
#ifdef PM_WITH_SIMD
	static const vec one = _mm_set1_ps(1.0f);
	static const vec zero = _mm_setzero_ps();
	return _mm_min_ps(_mm_max_ps(v, zero), one);
#else
	return pm_Clamp(v, pm_Zero(), pm_One());
#endif
}

vec PM_MATH_INLINE pm_Ceil(const vec& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_ceil_ps(v);
# else
	return _mm_setr_ps(
		std::ceil(pm_GetX(v)),
		std::ceil(pm_GetY(v)),
		std::ceil(pm_GetZ(v)),
		std::ceil(pm_GetW(v)));
# endif//PM_WITH_SVML
#else
	vec r;
	r[0] = std::ceil(v[0]);
	r[1] = std::ceil(v[1]);
	r[2] = std::ceil(v[2]);
	r[3] = std::ceil(v[3]);
	return r;
#endif
}

vec PM_MATH_INLINE pm_Floor(const vec& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_floor_ps(v);
# else
	return _mm_setr_ps(
		std::floor(pm_GetX(v)),
		std::floor(pm_GetY(v)),
		std::floor(pm_GetZ(v)),
		std::floor(pm_GetW(v)));
# endif//PM_WITH_SVML
#else
	vec r;
	r[0] = std::floor(v[0]);
	r[1] = std::floor(v[1]);
	r[2] = std::floor(v[2]);
	r[3] = std::floor(v[3]);
	return r;
#endif
}

vec PM_MATH_INLINE pm_Load4D(const float src[4])
{
#ifdef PM_WITH_SIMD
	PM_ALIGN(16) float t[4] = { src[0], src[1], src[2], src[3] };
	return _mm_load_ps(t);
#else
	vec r;
	r[0] = src[0];
	r[1] = src[1];
	r[2] = src[2];
	r[3] = src[3];
	return r;
#endif
}

void PM_MATH_INLINE pm_Store4D(const vec& v, float dst[4])
{
#ifdef PM_WITH_SIMD
	PM_ALIGN(16) float t[4];
	_mm_store_ps(t, v);
	dst[0] = t[0];
	dst[1] = t[1];
	dst[2] = t[2];
	dst[3] = t[3];
#else
	dst[0] = v[0];
	dst[1] = v[1];
	dst[2] = v[2];
	dst[3] = v[3];
#endif
}

float PM_MATH_INLINE pm_Dot4D(const vec4& v1, const vec4& v2)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_cvtss_f32(_mm_dp_ps(v1, v2, 0xF1));
# else
	vec s = _mm_mul_ps(v1, v2);
	return _mm_cvtss_f32(_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(3, 3, 3, 3))))));
# endif
#else
	return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]) + (v1[3] * v2[3]);
#endif
}

float PM_MATH_INLINE pm_Magnitude4D(const vec4& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(v, v, 0xF1)));
# else
	vec s = _mm_mul_ps(v, v);
	return _mm_cvtss_f32(_mm_sqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(3, 3, 3, 3)))))));
#endif
#else
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
#endif
}

float PM_MATH_INLINE pm_MagnitudeSqr4D(const vec4& v)
{
	return pm_Dot4D(v, v);
}

vec4 PM_MATH_INLINE pm_Normalize4D(const vec4& v)
{
#if defined(PM_FAST_MATH)
	return pm_FastNormalize4D(v);
#else
	return pm_QualityNormalize4D(v);
#endif
}

vec4 PM_MATH_INLINE pm_QualityNormalize4D(const vec4& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_div_ps(v, _mm_sqrt_ps(_mm_dp_ps(v, v, 0xFF)));
# else
	__m128 s = _mm_mul_ps(v, v);
	s = _mm_sqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
			_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
				_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)),
					_mm_shuffle_ps(s, s, _MM_SHUFFLE(3, 3, 3, 3))))));
	return _mm_div_ps(v, _mm_shuffle_ps(s, s, 0));
# endif
#else
	return pm_FastNormalize4D(v);
#endif
}

vec4 PM_MATH_INLINE pm_FastNormalize4D(const vec4& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_mul_ps(v, _mm_rsqrt_ps(_mm_dp_ps(v, v, 0xFF)));
# else
	__m128 s = _mm_mul_ps(v, v);
	s = _mm_rsqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
			_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
				_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)),
					_mm_shuffle_ps(s, s, _MM_SHUFFLE(3, 3, 3, 3))))));
	return _mm_mul_ps(v, _mm_shuffle_ps(s, s, 0));
# endif
#else
	vec r;
	float mag = pm_Magnitude4D(v);
	r[0] = v[0] / mag;
	r[1] = v[1] / mag;
	r[2] = v[2] / mag;
	r[3] = v[3] / mag;
	return r;
#endif
}

float PM_MATH_INLINE pm_MaxElement4D(const vec4& v)
{
	return PM::pm_Max(PM::pm_GetX(v),
		PM::pm_Max(PM::pm_GetY(v),
		PM::pm_Max(PM::pm_GetZ(v),
		PM::pm_GetW(v))));
}

float PM_MATH_INLINE pm_MinElement4D(const vec4& v)
{
	return PM::pm_Min(PM::pm_GetX(v),
		PM::pm_Min(PM::pm_GetY(v),
		PM::pm_Min(PM::pm_GetZ(v),
		PM::pm_GetW(v))));
}

vec PM_MATH_INLINE pm_Load3D(const float src[3])
{
#ifdef PM_WITH_SIMD
	PM_ALIGN(16) float t[4] = { src[0], src[1], src[2], 0 };
	return _mm_load_ps(t);
#else
	vec r;
	r[0] = src[0];
	r[1] = src[1];
	r[2] = src[2];
	return r;
#endif
}

void PM_MATH_INLINE pm_Store3D(const vec3& v, float dst[3])
{
#ifdef PM_WITH_SIMD
	PM_ALIGN(16) float t[4];
	_mm_store_ps(t, v);
	dst[0] = t[0];
	dst[1] = t[1];
	dst[2] = t[2];
#else
	dst[0] = v[0];
	dst[1] = v[1];
	dst[2] = v[2];
#endif
}

float PM_MATH_INLINE pm_Dot3D(const vec3& v1, const vec3& v2)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_cvtss_f32(_mm_dp_ps(v1, v2, 0x71));
# else
	vec s = _mm_mul_ps(v1, v2);
	return _mm_cvtss_f32(_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)))));
# endif
#else
	return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
#endif
}

vec3 PM_MATH_INLINE pm_Cross3D(const vec3& v1, const vec3& v2)
{
#if defined(PM_WITH_SIMD)
	/*return _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 0, 2, 1)),
		_mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 1, 0, 2))),
		_mm_mul_ps(_mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 1, 0, 2)),
		_mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 0, 2, 1))));*/
	__m128 result = _mm_sub_ps(
		_mm_mul_ps(v1, _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 0, 2, 1))),
		_mm_mul_ps(v2, _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 0, 2, 1))));
	return _mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 0, 2, 1));
#else
	vec3 r;
	r[0] = v1[1] * v2[2] - v1[2] * v2[1];
	r[1] = v1[2] * v2[0] - v1[0] * v2[2];
	r[2] = v1[0] * v2[1] - v1[1] * v2[0];
	r[3] = 0;
	return r;
#endif
}

float PM_MATH_INLINE pm_Magnitude3D(const vec3& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(v, v, 0x71)));
# else
	vec s = _mm_mul_ps(v, v);
	return _mm_cvtss_f32(_mm_sqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2))))));
# endif
#else
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
#endif
}

float PM_MATH_INLINE pm_MagnitudeSqr3D(const vec3& v)
{
	return pm_Dot3D(v, v);
}

vec3 PM_MATH_INLINE pm_Normalize3D(const vec3& v)
{
#if defined(PM_FAST_MATH)
	return pm_FastNormalize3D(v);
#else
	return pm_QualityNormalize3D(v);
#endif
}

vec3 PM_MATH_INLINE pm_QualityNormalize3D(const vec3& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_div_ps(v, _mm_sqrt_ps(_mm_dp_ps(v, v, 0x7F)));
# else
	vec s = _mm_mul_ps(v, v);
	s = _mm_sqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
			_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
				_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)))));
	return _mm_div_ps(v, _mm_shuffle_ps(s, s, 0));
#endif
#else
	return pm_FastNormalize3D(v);
#endif
}

vec3 PM_MATH_INLINE pm_FastNormalize3D(const vec3& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_mul_ps(v, _mm_rsqrt_ps(_mm_dp_ps(v, v, 0x7F)));// Should be 0x77, but x/0 is not good :P
# else
	vec s = _mm_mul_ps(v, v);
	s = _mm_rsqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
			_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
				_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)))));
	return _mm_mul_ps(v, _mm_shuffle_ps(s, s, 0));
#endif
#else
	vec3 r;
	float mag = pm_Magnitude3D(v);
	r[0] = v[0] / mag;
	r[1] = v[1] / mag;
	r[2] = v[2] / mag;
	r[3] = 0;
	return r;
#endif
}

float PM_MATH_INLINE pm_MaxElement3D(const vec3& v)
{
	return PM::pm_Max(PM::pm_GetX(v),
		PM::pm_Max(PM::pm_GetY(v),
		PM::pm_GetZ(v)));
}

float PM_MATH_INLINE pm_MinElement3D(const vec3& v)
{
	return PM::pm_Min(PM::pm_GetX(v),
		PM::pm_Min(PM::pm_GetY(v),
		PM::pm_GetZ(v)));
}

vec PM_MATH_INLINE pm_Load2D(const float src[2])
{
#ifdef PM_WITH_SIMD
	PM_ALIGN(16) float t[4] = { src[0], src[1], 0, 0 };
	return _mm_load_ps(t);
#else
	vec r;
	r[0] = src[0];
	r[1] = src[1];
	return r;
#endif
}

void PM_MATH_INLINE pm_Store2D(const vec& v, float dst[2])
{
#ifdef PM_WITH_SIMD
	PM_ALIGN(16) float t[4];
	_mm_store_ps(t, v);
	dst[0] = t[0];
	dst[1] = t[1];
#else
	dst[0] = v[0];
	dst[1] = v[1];
#endif
}

float PM_MATH_INLINE pm_Dot2D(const vec2& v1, const vec2& v2)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_cvtss_f32(_mm_dp_ps(v1, v2, 0x31));
# else
	vec s = _mm_mul_ps(v1, v2);
	return _mm_cvtss_f32(_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1))));
# endif
#else
	return (v1[0] * v2[0]) + (v1[1] * v2[1]);
#endif
}

float PM_MATH_INLINE pm_Magnitude2D(const vec2& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(v, v, 0x31)));
# else
	vec s = _mm_mul_ps(v, v);
	return _mm_cvtss_f32(_mm_sqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)))));
# endif
#else
	return std::sqrt(v[0] * v[0] + v[1] * v[1]);
#endif
}

float PM_MATH_INLINE pm_MagnitudeSqr2D(const vec2& v)
{
	return pm_Dot2D(v, v);
}

vec2 PM_MATH_INLINE pm_Normalize2D(const vec2& v)
{
#if defined(PM_FAST_MATH)
	return pm_FastNormalize2D(v);
#else
	return pm_QualityNormalize2D(v);
#endif
}

vec2 PM_MATH_INLINE pm_QualityNormalize2D(const vec2& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_div_ps(v, _mm_sqrt_ps(_mm_dp_ps(v, v, 0x3F)));
# else
	vec s = _mm_mul_ps(v, v);
	s = _mm_sqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
			_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1))));
	return _mm_div_ps(v, _mm_shuffle_ps(s, s, 0));
# endif
#else
	return pm_FastNormalize2D(v);
#endif
}

vec2 PM_MATH_INLINE pm_FastNormalize2D(const vec2& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_mul_ps(v, _mm_rsqrt_ps(_mm_dp_ps(v, v, 0x3F)));// Should be 0x33, but x/0 is not good :P
# else
	vec s = _mm_mul_ps(v, v);
	s = _mm_rsqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
			_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1))));
	return _mm_mul_ps(v, _mm_shuffle_ps(s, s, 0));
# endif
#else
	vec2 r;
	float mag = pm_Magnitude2D(v);
	r[0] = v[0] / mag;
	r[1] = v[1] / mag;
	r[2] = 0;
	r[3] = 0;
	return r;
#endif
}

float PM_MATH_INLINE pm_MaxElement2D(const vec2& v)
{
	return PM::pm_Max(PM::pm_GetX(v), PM::pm_GetY(v));
}

float PM_MATH_INLINE pm_MinElement2D(const vec2& v)
{
	return PM::pm_Min(PM::pm_GetX(v), PM::pm_GetY(v));
}