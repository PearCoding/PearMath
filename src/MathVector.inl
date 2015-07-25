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
#ifdef PM_USE_SIMD
	vdst = vsrc;
#else
	//vdst[0] = vsrc[0];
	//vdst[1] = vsrc[1];
	//vdst[2] = vsrc[2];
	//vdst[3] = vsrc[3];
	memcpy(vdst.v, vsrc.v, sizeof(float) * 4);
#endif
}

vec PM_MATH_INLINE pm_Zero()
{
#ifdef PM_USE_SIMD
	return _mm_setzero_ps();
#else
	vec v;
	memset(v.v, 0, sizeof(v));
	return v;
#endif
}

vec PM_MATH_INLINE pm_One()
{
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
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

vec PM_MATH_INLINE pm_SetX(const vec& v, float x)
{
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
	vec r = v;
	((float *)&(r))[3] = w;
	return r;
#else
	vec r = v;
	r[3] = w;
	return r;
#endif
}

float PM_MATH_INLINE pm_GetX(const vec& v)
{
#ifdef PM_USE_SIMD
	return ((float *)&(v))[0];
#else
	return v[0];
#endif
}

float PM_MATH_INLINE pm_GetY(const vec& v)
{
#ifdef PM_USE_SIMD
	return ((float *)&(v))[1];
#else
	return v[1];
#endif
}

float PM_MATH_INLINE pm_GetZ(const vec& v)
{
#ifdef PM_USE_SIMD
	return ((float *)&(v))[2];
#else
	return v[2];
#endif
}

float PM_MATH_INLINE pm_GetW(const vec& v)
{
#ifdef PM_USE_SIMD
	return ((float *)&(v))[3];
#else
	return v[3];
#endif
}

vec PM_MATH_INLINE pm_FillVector(float val)
{
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
	_mm_store_ps(&dst[0], v);
#else
	memcpy(const_cast<float*>(v.v), dst, sizeof(float) * 4);
#endif
}

vec PM_MATH_INLINE pm_IsEqualv(const vec& v1, const vec& v2)
{
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
	return (_mm_movemask_ps(_mm_cmpeq_ps(v1, v2)) & 0xF) == 0xF;
#else
	return (v1[0] == v2[0] && v1[1] == v2[1] && v1[2] == v2[2] && v1[3] == v2[3]);
#endif
}

vec PM_MATH_INLINE pm_IsNearlyEqualv(const vec& v1, const vec& v2, const vec& delta)
{
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
	return (_mm_movemask_ps(_mm_cmpneq_ps(v1, v2)) & 0xF) == 0xF;
#else
	return (v1[0] != v2[0] && v1[1] != v2[1] && v1[2] != v2[2] && v1[3] != v2[3]);
#endif
}

vec PM_MATH_INLINE pm_IsLessv(const vec& v1, const vec& v2)
{
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
	return (_mm_movemask_ps(_mm_cmplt_ps(v1, v2)) & 0xF) == 0xF;
#else
	return (v1[0] < v2[0] && v1[1] < v2[1] && v1[2] < v2[2] && v1[3] < v2[3]);
#endif
}

vec PM_MATH_INLINE pm_IsLessOrEqualv(const vec& v1, const vec& v2)
{
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
	return (_mm_movemask_ps(_mm_cmple_ps(v1, v2)) & 0xF) == 0xF;
#else
	return (v1[0] <= v2[0] && v1[1] <= v2[1] && v1[2] <= v2[2] && v1[3] <= v2[3]);
#endif
}

vec PM_MATH_INLINE pm_IsGreaterv(const vec& v1, const vec& v2)
{
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
	return (_mm_movemask_ps(_mm_cmpgt_ps(v1, v2)) & 0xF) == 0xF;
#else
	return (v1[0] > v2[0] && v1[1] > v2[1] && v1[2] > v2[2] && v1[3] > v2[3]);
#endif
}

vec PM_MATH_INLINE pm_IsGreaterOrEqualv(const vec& v1, const vec& v2)
{
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
	return (_mm_movemask_ps(_mm_cmpge_ps(v1, v2)) & 0xF) == 0xF;
#else
	return (v1[0] >= v2[0] && v1[1] >= v2[1] && v1[2] >= v2[2] && v1[3] >= v2[3]);
#endif
}

vec PM_MATH_INLINE pm_IsInBounds(const vec& v, const vec& bounds)
{
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
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

/*
 * @todo This is not good enough
 */
vec PM_MATH_INLINE pm_Sqrt(const vec& v)
{
#ifdef PM_USE_SIMD
	return _mm_sqrt_ps(v);
#else
	return pm_Multiply(v, pm_ReciprocalSqrt(v));
#endif
}

vec PM_MATH_INLINE pm_ReciprocalSqrt(const vec& v)
{
#ifdef PM_USE_SIMD
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
#ifdef PM_USE_SIMD
	//I'm not happy with this :(
	return _mm_setr_ps(
		powf(pm_GetX(v1), pm_GetX(v2)),
		powf(pm_GetY(v1), pm_GetY(v2)),
		powf(pm_GetZ(v1), pm_GetZ(v2)),
		powf(pm_GetW(v1), pm_GetW(v2)));
	//return pm_Exp(_mm_mul_ps(pm_Log(v1), v2));
#else
	vec r;
	r[0] = powf(v1[0], v2[0]);
	r[1] = powf(v1[1], v2[1]);
	r[2] = powf(v1[2], v2[2]);
	r[3] = powf(v1[3], v2[3]);
	return r;
	//return pm_Exp(pm_Multiply(pm_Log(v1), v2));
#endif
}

/*
 * @todo Implement own approximation...
 */
vec PM_MATH_INLINE pm_Exp(const vec& v)
{
#ifdef PM_USE_SIMD
	return _mm_setr_ps(
		expf(pm_GetX(v)),
		expf(pm_GetY(v)),
		expf(pm_GetZ(v)),
		expf(pm_GetW(v)));
#else
	vec r;
	r[0] = expf(v[0]);
	r[1] = expf(v[1]);
	r[2] = expf(v[2]);
	r[3] = expf(v[3]);
	return r;
#endif
}

/*
 * @todo Implement own approximation...
 */
vec PM_MATH_INLINE pm_Log(const vec& v)
{
#ifdef PM_USE_SIMD
	return _mm_setr_ps(
		logf(pm_GetX(v)),
		logf(pm_GetY(v)),
		logf(pm_GetZ(v)),
		logf(pm_GetW(v)));
#else
	vec r;
	r[0] = logf(v[0]);
	r[1] = logf(v[1]);
	r[2] = logf(v[2]);
	r[3] = logf(v[3]);
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
#ifdef PM_USE_SIMD
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

#if PM_MATH_SIN_QUAL >= 1
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
#endif

	return r;
#else
	/*
	 * Use this or the c implementation from above??
	 */
	vec r;
	r[0] = sinf(v[0]);
	r[1] = sinf(v[1]);
	r[2] = sinf(v[2]);
	r[3] = sinf(v[3]);
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
#ifdef PM_USE_SIMD
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

#if PM_MATH_COS_QUAL >= 1
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
#endif

	return r;
#else
	/*
	 * Use this or the c implementation from above??
	 */
	vec r;
	r[0] = cosf(v[0]);
	r[1] = cosf(v[1]);
	r[2] = cosf(v[2]);
	r[3] = cosf(v[3]);
	return r;
#endif
}

void PM_MATH_INLINE pm_SinCos(const vec& v, vec& sin, vec& cos)
{
#ifdef PM_USE_SIMD
	vec s = _mm_mul_ps(v, v);
	vec sinp = _mm_mul_ps(v, s);
	vec cosp = _mm_mul_ps(s, s);

	sin = _mm_add_ps(v, _mm_mul_ps(_mm_set1_ps(-0.16666666666667f), sinp));
	cos = _mm_add_ps(_mm_set1_ps(1), _mm_mul_ps(_mm_set1_ps(-0.5f), s));

	sinp = _mm_mul_ps(sinp, s);
	sin = _mm_add_ps(sin, _mm_mul_ps(_mm_set1_ps(0.0083333333333333f), sinp));
	cos = _mm_add_ps(cos, _mm_mul_ps(_mm_set1_ps(0.041666666666667f), cosp));

	sinp = _mm_mul_ps(sinp, s);
	sin = _mm_add_ps(sin, _mm_mul_ps(_mm_set1_ps(-1.9841269841269841e-4f), sinp));
	cosp = _mm_mul_ps(cosp, s);
	cos = _mm_add_ps(cos, _mm_mul_ps(_mm_set1_ps(-0.0013888888888889f), cosp));

	sinp = _mm_mul_ps(sinp, s);
	sin = _mm_add_ps(sin, _mm_mul_ps(_mm_set1_ps(2.7557319223985893e-6f), sinp));
	cosp = _mm_mul_ps(cosp, s);
	cos = _mm_add_ps(cos, _mm_mul_ps(_mm_set1_ps(2.4801587301587302e-5f), cosp));

	sinp = _mm_mul_ps(sinp, s);
	sin = _mm_add_ps(sin, _mm_mul_ps(_mm_set1_ps(-2.505210838544172e-8f), sinp));
	cosp = _mm_mul_ps(cosp, s);
	cos = _mm_add_ps(cos, _mm_mul_ps(_mm_set1_ps(-2.7557319223985888e-7f), cosp));

#if PM_MATH_SIN_QUAL == 1 || PM_MATH_COS_QUAL == 1
	sinp = _mm_mul_ps(sinp, s);
	sin = _mm_add_ps(sin, _mm_mul_ps(_mm_set1_ps(1.6059043836821613e-10f), sinp));
	cosp = _mm_mul_ps(cosp, s);
	cos = _mm_add_ps(cos, _mm_mul_ps(_mm_set1_ps(2.08767569878681e-9f), cosp));

	sinp = _mm_mul_ps(sinp, s);
	sin = _mm_add_ps(sin, _mm_mul_ps(_mm_set1_ps(-7.6471637318198164e-13f), sinp));
	cosp = _mm_mul_ps(cosp, s);
	cos = _mm_add_ps(cos, _mm_mul_ps(_mm_set1_ps(-1.1470745597729725e-11f), cosp));

	sinp = _mm_mul_ps(sinp, s);
	sin = _mm_add_ps(sin, _mm_mul_ps(_mm_set1_ps(2.8114572543455206e-15f), sinp));
	cosp = _mm_mul_ps(cosp, s);
	cos = _mm_add_ps(cos, _mm_mul_ps(_mm_set1_ps(4.7794773323873853e-14f), cosp));

	sinp = _mm_mul_ps(sinp, s);
	sin = _mm_add_ps(sin, _mm_mul_ps(_mm_set1_ps(-8.2206352466243295e-18f), sinp));
	cosp = _mm_mul_ps(cosp, s);
	cos = _mm_add_ps(cos, _mm_mul_ps(_mm_set1_ps(-1.5619206968586225e-16f), cosp));

	sinp = _mm_mul_ps(sinp, s);
	sin = _mm_add_ps(sin, _mm_mul_ps(_mm_set1_ps(1.9572941063391263e-20f), sinp));
	cosp = _mm_mul_ps(cosp, s);
	cos = _mm_add_ps(cos, _mm_mul_ps(_mm_set1_ps(4.1103176233121648e-19f), cosp));
#endif
#else
	/*
	 * Use this or the c implementation from above??
	 */
	sin[0] = sinf(v[0]);
	sin[1] = sinf(v[1]);
	sin[2] = sinf(v[2]);
	sin[3] = sinf(v[3]);

	cos[0] = cosf(v[0]);
	cos[1] = cosf(v[1]);
	cos[2] = cosf(v[2]);
	cos[3] = cosf(v[3]);
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
#ifdef PM_USE_SIMD
	/*vec s = _mm_mul_ps(v, v);
	vec p = _mm_mul_ps(v, s);
	vec r = v;

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(0.33333333333333f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(0.13333333333333f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(0.053968253968254f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(0.021869488536155f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(0.0088632355299022f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(0.0035921280365725f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(0.0014558343870513f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(5.9002744094558595e-4f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(2.3912911424355248e-4f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(9.6915379569294509e-5f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(3.9278323883316833e-5f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(1.5918905069328964e-5f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(6.4516892156554306e-6f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(2.6147711512907546e-6f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(1.0597268320104654e-6f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(4.2949110782738063e-7f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(1.7406618963571648e-7f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(7.0546369464009681e-8f), p));

	p = _mm_mul_ps(p, s);
	r = _mm_add_ps(r, _mm_mul_ps(_mm_set1_ps(2.859136662305254e-8f), p));

	return r;*/
	return _mm_setr_ps(
		tanf(pm_GetX(v)),
		tanf(pm_GetY(v)),
		tanf(pm_GetZ(v)),
		tanf(pm_GetW(v)));
#else
	/*
	 * Use this or the c implementation from above??
	 */
	vec r;
	r[0] = tanf(v[0]);
	r[1] = tanf(v[1]);
	r[2] = tanf(v[2]);
	r[3] = tanf(v[3]);
	return r;
#endif
}

/*
 * @todo Implement own approximation...
 */
vec PM_MATH_INLINE pm_ASin(const vec& v)
{
#ifdef PM_USE_SIMD
	return _mm_setr_ps(
		asinf(pm_GetX(v)),
		asinf(pm_GetY(v)),
		asinf(pm_GetZ(v)),
		asinf(pm_GetW(v)));
#else
	vec r;
	r[0] = asinf(v[0]);
	r[1] = asinf(v[1]);
	r[2] = asinf(v[2]);
	r[3] = asinf(v[3]);
	return r;
#endif
}

/*
 * @todo Implement own approximation...
 */
vec PM_MATH_INLINE pm_ACos(const vec& v)
{
#ifdef PM_USE_SIMD
	return _mm_setr_ps(
		acosf(pm_GetX(v)),
		acosf(pm_GetY(v)),
		acosf(pm_GetZ(v)),
		acosf(pm_GetW(v)));
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
#ifdef PM_USE_SIMD
	return _mm_setr_ps(
		atanf(pm_GetX(v)),
		atanf(pm_GetY(v)),
		atanf(pm_GetZ(v)),
		atanf(pm_GetW(v)));
#else
	vec r;
	r[0] = atan(v[0]);
	r[1] = atan(v[1]);
	r[2] = atan(v[2]);
	r[3] = atan(v[3]);
	return r;
#endif
}

vec PM_MATH_INLINE pm_Lerp(const vec& v1, const vec& v2, const vec& t)
{
#ifdef PM_USE_SIMD
	return _mm_add_ps(_mm_mul_ps(_mm_sub_ps(v2, v1), t), v1);
#else
	return pm_Add(v1, pm_Multiply(pm_Subtract(v2, v1), t));
#endif
}

vec PM_MATH_INLINE pm_NLerp(const vec& v1, const vec& v2, const vec& t)
{
#ifdef PM_USE_SIMD
	return pm_Normalize4D(_mm_add_ps(_mm_mul_ps(_mm_sub_ps(v2, v1), t), v1));
#else
	return pm_Normalize4D(pm_Add(v1, pm_Multiply(pm_Subtract(v2, v1), t)));
#endif
}

vec PM_MATH_INLINE pm_SLerp(const vec& v1, const vec& v2, const vec& t)
{
#ifdef PM_USE_SIMD
	vec cosAngle = _mm_set1_ps(pm_Dot4D(v1, v2));
	vec mask = _mm_cmpgt_ps(_mm_set1_ps(PM_VECTOR_SLERP_THRESHOLD), cosAngle);
	vec angle = pm_ACos(cosAngle);
	vec decWeight = _mm_sub_ps(_mm_set1_ps(1.0f), t);
	vec angles = _mm_unpacklo_ps(_mm_set1_ps(1.0f), t);
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

	angle = acosf(angle);
	vec r;
	r[0] = (v1[0] * sinf(angle*(1 - t[0])) + v2[0] * sinf(angle*t[0])) / sinf(angle);
	r[1] = (v1[1] * sinf(angle*(1 - t[1])) + v2[1] * sinf(angle*t[1])) / sinf(angle);
	r[2] = (v1[2] * sinf(angle*(1 - t[2])) + v2[2] * sinf(angle*t[2])) / sinf(angle);
	r[3] = (v1[3] * sinf(angle*(1 - t[3])) + v2[3] * sinf(angle*t[3])) / sinf(angle);
	return r;
#endif
}

vec PM_MATH_INLINE pm_Max(const vec& v1, const vec& v2)
{
#ifdef PM_USE_SIMD
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

vec PM_MATH_INLINE pm_Min(const vec& v1, const vec& v2)
{
#ifdef PM_USE_SIMD
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

vec PM_MATH_INLINE pm_Clamp(const vec& v, const vec& min, const vec& max)
{
	PM_DEBUG_ASSERT(pm_IsLessOrEqual(min, max));
#ifdef PM_USE_SIMD
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

vec PM_MATH_INLINE pm_Saturate(const vec& v)
{
#ifdef PM_USE_SIMD
	return _mm_min_ps(_mm_max_ps(v, _mm_setzero_ps()), _mm_set1_ps(1.0f));
#else
	return pm_Clamp(v, pm_Zero(), pm_One());
#endif
}

vec PM_MATH_INLINE pm_Load4D(const float src[4])
{
#ifdef PM_USE_SIMD
	return _mm_load_ps(src);
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
#ifdef PM_USE_SIMD
	_mm_store_ps(dst, v);
#else
	dst[0] = v[0];
	dst[1] = v[1];
	dst[2] = v[2];
	dst[3] = v[3];
#endif
}

float PM_MATH_INLINE pm_Dot4D(const vec4& v1, const vec4& v2)
{
	//TODO: Use SSE3 _mm_dp_ps!
#if defined(PM_USE_SIMD)
	vec s = _mm_mul_ps(v1, v2);
	s = _mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(3, 3, 3, 3)))));
	return ((float *)&(s))[0];
#else
	return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]) + (v1[3] * v2[3]);
#endif
}

float PM_MATH_INLINE pm_Magnitude4D(const vec4& v)
{
#if defined(PM_USE_SIMD)
	vec s = _mm_mul_ps(v, v);
	s = _mm_sqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(3, 3, 3, 3))))));
	return ((float *)&(s))[0];
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
#if defined(PM_USE_SIMD)
	__m128 s = _mm_mul_ps(v, v);
	s = _mm_rsqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(3, 3, 3, 3))))));
	return _mm_mul_ps(v, _mm_shuffle_ps(s, s, 0));
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

vec PM_MATH_INLINE pm_Load3D(const float src[3])
{
#ifdef PM_USE_SIMD
	float t[4] = { src[0], src[1], src[2], 0 };
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
#ifdef PM_USE_SIMD
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
#if defined(PM_USE_SIMD)
	vec s = _mm_mul_ps(v1, v2);
	s = _mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2))));
	return ((float *)&(s))[0];
#else
	return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
#endif
}

vec3 PM_MATH_INLINE pm_Cross3D(const vec3& v1, const vec3& v2)
{
#if defined(PM_USE_SIMD)
	return _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 0, 2, 1)),
		_mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 1, 0, 2))),
		_mm_mul_ps(_mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 1, 0, 2)),
		_mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 0, 2, 1))));
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
#if defined(PM_USE_SIMD)
	vec s = _mm_mul_ps(v, v);
	s = _mm_sqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)))));
	return ((float *)&(s))[0];
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
#if defined(PM_USE_SIMD)
	vec s = _mm_mul_ps(v, v);
	s = _mm_rsqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)))));
	return _mm_mul_ps(v, _mm_shuffle_ps(s, s, 0));
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

vec PM_MATH_INLINE pm_Load2D(const float src[2])
{
#ifdef PM_USE_SIMD
	float t[4] = { src[0], src[1], 0, 0 };
	return _mm_load_ps(t);
#else
	vec r;
	r[0] = src[0];
	r[1] = src[1];
	r[2] = src[2];
	return r;
#endif
}

void PM_MATH_INLINE pm_Store2D(const vec& v, float dst[2])
{
#ifdef PM_USE_SIMD
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
#if defined(PM_USE_SIMD)
	vec s = _mm_mul_ps(v1, v2);
	s = _mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)));
	return ((float *)&(s))[0];
#else
	return (v1[0] * v2[0]) + (v1[1] * v2[1]);
#endif
}

float PM_MATH_INLINE pm_Magnitude2D(const vec2& v)
{
#if defined(PM_USE_SIMD)
	vec s = _mm_mul_ps(v, v);
	s = _mm_sqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1))));
	return ((float *)&(s))[0];
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
#if defined(PM_USE_SIMD)
	vec s = _mm_mul_ps(v, v);
	s = _mm_rsqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1))));
	return _mm_mul_ps(v, _mm_shuffle_ps(s, s, 0));
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