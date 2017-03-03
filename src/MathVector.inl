/*
* Copyright(c) 2014-2017, Ömercan Yazici <pearcoding AT gmail.com>
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

template<typename VectorType>
VectorType PM_MATH_INLINE pm_SetIndex(const VectorType& v, int index, float val)
{
	PM_DEBUG_ASSERT(index < VectorType::Dimension);
#ifdef PM_WITH_SIMD
	VectorType r = v;
	((float *)&(r._vec))[index] = val;
	return r;
#else
	VectorType r = v;
	r._vec[index] = val;
	return r;
#endif
}

template<typename VectorType>
float PM_MATH_INLINE pm_GetIndex(const VectorType& v, int index)
{
	PM_DEBUG_ASSERT(index < VectorType::Dimension);
#ifdef PM_WITH_SIMD
	return ((float *)&(v._vec))[index];
#else
	return v._vec[index];
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_SetX(const VectorType& v, float x)
{
	return pm_SetIndex(v, 0, x);
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_SetY(const VectorType& v, float y)
{
	return pm_SetIndex(v, 1, y);
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_SetZ(const VectorType& v, float z)
{
	return pm_SetIndex(v, 2, z);
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_SetW(const VectorType& v, float w)
{
	return pm_SetIndex(v, 3, w);
}

template<typename VectorType>
float PM_MATH_INLINE pm_GetX(const VectorType& v)
{
	return pm_GetIndex(v, 0);
}

template<typename VectorType>
float PM_MATH_INLINE pm_GetY(const VectorType& v)
{
	return pm_GetIndex(v, 1);
}

template<typename VectorType>
float PM_MATH_INLINE pm_GetZ(const VectorType& v)
{
	return pm_GetIndex(v, 2);
}

template<typename VectorType>
float PM_MATH_INLINE pm_GetW(const VectorType& v)
{
	return pm_GetIndex(v, 3);
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
	PM_MATH_INLINE pm_IsEqualv(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_cmpeq_ps(v1._vec, v2._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = (float)((v1._vec[i] == v2._vec[i]) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, bool>::type
	PM_MATH_INLINE pm_IsEqual(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return (_mm_movemask_ps(_mm_cmpeq_ps(v1._vec, v2._vec)) 
		& _PM_SSE_DIM_MASK(VectorType::Dimension)) == _PM_SSE_DIM_MASK(VectorType::Dimension);
#else
	for(int i = 0; i < VectorType::Dimension; ++i)
	{
		if(v1._vec[i] != v2._vec[i])
			return false;
	}
	return true;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
	PM_MATH_INLINE pm_IsNearlyEqualv(const VectorType& v1, const VectorType& v2, const VectorType& delta)
{
#ifdef PM_WITH_SIMD
	__m128 del = _mm_sub_ps(v1._vec, v2._vec);
	__m128 abs = _mm_max_ps(_mm_sub_ps(_mm_setzero_ps(), del), del);
	return _mm_cmple_ps(abs, delta._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = (float)((std::abs(v1._vec[i] - v2._vec[i]) <= delta._vec[i]) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
	PM_MATH_INLINE pm_IsNearlyEqualv(const VectorType& v1, const VectorType& v2, float delta)
{
#ifdef PM_WITH_SIMD
	__m128 del = _mm_sub_ps(v1._vec, v2._vec);
	__m128 abs = _mm_max_ps(_mm_sub_ps(_mm_setzero_ps(), del), del);
	return _mm_cmple_ps(abs, _mm_set1_ps(delta));
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = (float)((std::abs(v1._vec[i] - v2._vec[i]) <= delta) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, bool>::type
	PM_MATH_INLINE pm_IsNearlyEqual(const VectorType& v1, const VectorType& v2, const VectorType& delta)
{
#ifdef PM_WITH_SIMD
	__m128 del = _mm_sub_ps(v1._vec, v2._vec);
	__m128 abs = _mm_max_ps(_mm_sub_ps(_mm_setzero_ps(), del), del);
	return (_mm_movemask_ps(_mm_cmple_ps(abs, delta._vec)) 
		& _PM_SSE_DIM_MASK(VectorType::Dimension)) == _PM_SSE_DIM_MASK(VectorType::Dimension);
#else
	for(int i = 0; i < VectorType::Dimension; ++i)
	{
		if(std::abs(v1._vec[i] - v2._vec[i]) > delta._vec[i])
			return false;
	}
	return true;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, bool>::type
	PM_MATH_INLINE pm_IsNearlyEqual(const VectorType& v1, const VectorType& v2, float delta)
{
#ifdef PM_WITH_SIMD
	__m128 del = _mm_sub_ps(v1._vec, v2._vec);
	__m128 abs = _mm_max_ps(_mm_sub_ps(_mm_setzero_ps(), del), del);
	return (_mm_movemask_ps(_mm_cmple_ps(abs, _mm_set1_ps(delta))) 
		& _PM_SSE_DIM_MASK(VectorType::Dimension)) == _PM_SSE_DIM_MASK(VectorType::Dimension);
#else
	for(int i = 0; i < VectorType::Dimension; ++i)
	{
		if(std::abs(v1._vec[i] - v2._vec[i]) > delta)
			return false;
	}
	return true;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
	PM_MATH_INLINE pm_IsNotEqualv(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_cmpneq_ps(v1._vec, v2._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = (float)((v1._vec[i] != v2._vec[i]) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, bool>::type
	PM_MATH_INLINE pm_IsNotEqual(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return (_mm_movemask_ps(_mm_cmpneq_ps(v1._vec, v2._vec)) 
		& _PM_SSE_DIM_MASK(VectorType::Dimension)) == _PM_SSE_DIM_MASK(VectorType::Dimension);
#else
	for(int i = 0; i < VectorType::Dimension; ++i)
	{
		if(v1._vec[i] == v2._vec[i])
			return false;
	}
	return true;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
	PM_MATH_INLINE pm_IsLessv(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_cmplt_ps(v1._vec, v2._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = (float)((v1._vec[i] < v2._vec[i]) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, bool>::type
	PM_MATH_INLINE pm_IsLess(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return (_mm_movemask_ps(_mm_cmplt_ps(v1._vec, v2._vec)) 
		& _PM_SSE_DIM_MASK(VectorType::Dimension)) == _PM_SSE_DIM_MASK(VectorType::Dimension);
#else
	for(int i = 0; i < VectorType::Dimension; ++i)
	{
		if(v1._vec[i] >= v2._vec[i])
			return false;
	}
	return true;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
	PM_MATH_INLINE pm_IsLessOrEqualv(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_cmple_ps(v1._vec, v2._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = (float)((v1._vec[i] <= v2._vec[i]) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, bool>::type
	PM_MATH_INLINE pm_IsLessOrEqual(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return (_mm_movemask_ps(_mm_cmple_ps(v1._vec, v2._vec)) 
		& _PM_SSE_DIM_MASK(VectorType::Dimension)) == _PM_SSE_DIM_MASK(VectorType::Dimension);
#else
	for(int i = 0; i < VectorType::Dimension; ++i)
	{
		if(v1._vec[i] > v2._vec[i])
			return false;
	}
	return true;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
	PM_MATH_INLINE pm_IsGreaterv(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_cmpgt_ps(v1, v2);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = (float)((v1._vec[i] > v2._vec[i]) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, bool>::type
	PM_MATH_INLINE pm_IsGreater(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return (_mm_movemask_ps(_mm_cmpgt_ps(v1._vec, v2._vec)) 
		& _PM_SSE_DIM_MASK(VectorType::Dimension)) == _PM_SSE_DIM_MASK(VectorType::Dimension);
#else
	for(int i = 0; i < VectorType::Dimension; ++i)
	{
		if(v1._vec[i] <= v2._vec[i])
			return false;
	}
	return true;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
	PM_MATH_INLINE pm_IsGreaterOrEqualv(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_cmpge_ps(v1._vec, v2._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = (float)((v1._vec[i] >= v2._vec[i]) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, bool>::type
	PM_MATH_INLINE pm_IsGreaterOrEqual(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return (_mm_movemask_ps(_mm_cmpge_ps(v1._vec, v2._vec)) 
		& _PM_SSE_DIM_MASK(VectorType::Dimension)) == _PM_SSE_DIM_MASK(VectorType::Dimension);
#else
	for(int i = 0; i < VectorType::Dimension; ++i)
	{
		if(v1._vec[i] < v2._vec[i])
			return false;
	}
	return true;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_IsInBounds(const VectorType& v, const VectorType& bounds)
{
#ifdef PM_WITH_SIMD
	//Check if less or equal than negate and check again if less or equal. Next "and" both results.
	__m128 tmp1 = _mm_cmple_ps(v._vec, bounds._vec);
	__m128 tmp2 = _mm_mul_ps(bounds._vec, _mm_set1_ps(-1.0f));
	tmp2 = _mm_cmple_ps(tmp2, v._vec);
	return _mm_and_ps(tmp1, tmp2);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = (float)((v._vec[i] <= bounds._vec[i] && v._vec[i] >= -bounds._vec[i]) ? 0xFFFFFFFF : 0);
	return r;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
	PM_MATH_INLINE pm_Negate(const VectorType& v)
{
#ifdef PM_WITH_SIMD
	return _mm_sub_ps(_mm_set1_ps(0.0f), v._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = -v._vec[i];
	return r;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
	PM_MATH_INLINE pm_Add(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_add_ps(v1._vec, v2._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = v1._vec[i] + v2._vec[i];
	return r;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
	PM_MATH_INLINE pm_Subtract(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_sub_ps(v1._vec, v2._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = v1._vec[i] - v2._vec[i];
	return r;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
	PM_MATH_INLINE pm_Multiply(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_mul_ps(v1._vec, v2._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = v1._vec[i] * v2._vec[i];
	return r;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
	PM_MATH_INLINE pm_Divide(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_div_ps(v1._vec, v2._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = v1._vec[i] / v2._vec[i];
	return r;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
	PM_MATH_INLINE pm_Scale(const VectorType& v, float factor)
{
#ifdef PM_WITH_SIMD
	return _mm_mul_ps(v._vec, _mm_set1_ps(factor));
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = v._vec[i] * factor;
	return r;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
	PM_MATH_INLINE pm_MultiplyAdd(const VectorType& v1, const VectorType& v2, const VectorType& v3)
{
#ifdef PM_WITH_SIMD
	return _mm_add_ps(_mm_mul_ps(v1._vec, v2._vec), v3._vec);//? Faster version?
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = (v1._vec[i] * v2._vec[i]) + v3._vec[i];
	return r;
#endif
}

template<typename VectorType>
typename std::enable_if<is_vector<VectorType>::value, VectorType>::type
	PM_MATH_INLINE pm_Reciprocal(const VectorType& v)
{
#ifdef PM_WITH_SIMD
	return _mm_rcp_ps(v._vec);
#else
	//Faster?
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = 1/v._vec[i];
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_Sqrt(const VectorType& v)
{
#ifdef PM_WITH_SIMD
	return _mm_sqrt_ps(v._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = std::sqrt(v._vec[i]);
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_ReciprocalSqrt(const VectorType& v)
{
#ifdef PM_WITH_SIMD
	return _mm_rsqrt_ps(v._vec);
#else
	union
	{
		float V[VectorType::Dimension];
		long I[VectorType::Dimension];
	} val;

	const VectorType half = pm_Scale(v, 0.5f);

	for(int i = 0; i < VectorType::Dimension; ++i)
		val.V[i] = v._vec[i];

	for(int i = 0; i < VectorType::Dimension; ++i)
		val.I[i] = 0x5F3759DF - (val.I[i] >> 1);

	for(int j = 0; j < PM_MATH_RECIPROCALSQRT_QUAL; ++j)
	{
		for(int i = 0; i < VectorType::Dimension; ++i)
			val.V[i] *= (1.5f - half._vec[i] * val.V[i] * val.V[i]);
	}

	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = val.V[i];
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_Pow(const VectorType& v1, const VectorType& v2)
{
#if defined(PM_WITH_SIMD) && defined(PM_WITH_SVML)
	return _mm_pow_ps(v1._vec, v2._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r = pm_SetIndex(r, i, std::pow(pm_GetIndex(v1, i), pm_GetIndex(v2, i)));
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_Exp(const VectorType& v)
{
#if defined(PM_WITH_SIMD) && defined(PM_WITH_SVML)
	return _mm_exp_ps(v._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r = pm_SetIndex(r, i, std::exp(pm_GetIndex(v, i)));
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_Exp2(const VectorType& v)
{
#if defined(PM_WITH_SIMD) && defined(PM_WITH_SVML)
	return _mm_exp2_ps(v._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r = pm_SetIndex(r, i, std::exp2(pm_GetIndex(v, i)));
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_Exp10(const VectorType& v)
{
#if defined(PM_WITH_SIMD) && defined(PM_WITH_SVML)
	return _mm_exp10_ps(v._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r = pm_SetIndex(r, i, std::exp(pm_GetIndex(v, i))*PM_LOG_10_F);
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_ExpM1(const VectorType& v)
{
#if defined(PM_WITH_SIMD) && defined(PM_WITH_SVML)
	return _mm_expm1_ps(v._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r = pm_SetIndex(r, i, std::expm1(pm_GetIndex(v, i)));
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_Log(const VectorType& v)
{
#if defined(PM_WITH_SIMD) && defined(PM_WITH_SVML)
	return _mm_log_ps(v._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r = pm_SetIndex(r, i, std::log(pm_GetIndex(v, i)));
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_Log2(const VectorType& v)
{
#if defined(PM_WITH_SIMD) && defined(PM_WITH_SVML)
	return _mm_log2_ps(v);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r = pm_SetIndex(r, i, std::log2(pm_GetIndex(v, i)));
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_Log10(const VectorType& v)
{
#if defined(PM_WITH_SIMD) && defined(PM_WITH_SVML)
	return _mm_log10_ps(v);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r = pm_SetIndex(r, i, std::log10(pm_GetIndex(v, i)));
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_Log1P(const VectorType& v)
{
#if defined(PM_WITH_SIMD) && defined(PM_WITH_SVML)
	return _mm_log1p_ps(v);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r = pm_SetIndex(r, i, std::log1p(pm_GetIndex(v, i)));
	return r;
#endif
}

/*
 * Sin range [-PI,PI]
 * x^21/51090942171709440000-x^19/121645100408832000+x^17/355687428096000-
 x^15/1307674368000+x^13/6227020800-x^11/39916800+x^9/362880-x^7/5040+x^5/120-x^3/6+x
 */
template<typename VectorType>
VectorType PM_MATH_INLINE pm_Sin(const VectorType& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_sin_ps(v._vec);
# else
	const __m128 s = _mm_mul_ps(v._vec, v._vec);
	__m128 p = _mm_mul_ps(v._vec, s);
	__m128 r = _mm_add_ps(v._vec, _mm_mul_ps(_mm_set1_ps(-0.16666666666667f), p));

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
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = std::sin(v._vec[i]);
	return r;
#endif
}

/*
 * Cos range [-PI, PI]
 * x^20/2432902008176640000-x^18/6402373705728000+x^16/20922789888000-x^14/87178291200
 +x^12/479001600-x^10/3628800+x^8/40320-x^6/720+x^4/24-x^2/2+1
 */
template<typename VectorType>
VectorType PM_MATH_INLINE pm_Cos(const VectorType& v)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	return _mm_cos_ps(v._vec);
# else
	const __m128 s = _mm_mul_ps(v._vec, v._vec);//x^2
	__m128 r = _mm_add_ps(_mm_set1_ps(1), _mm_mul_ps(_mm_set1_ps(-0.5f), s));//-x^2/2+1

	__m128 p = _mm_mul_ps(s, s);
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
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = std::cos(v._vec[i]);
	return r;
#endif
}

template<typename VectorType>
void PM_MATH_INLINE _pm_SinCos(const VectorType& v, VectorType& s, VectorType& c)
{
#ifdef PM_WITH_SIMD
# ifdef PM_WITH_SVML
	s._vec = _mm_sincos_ps(&c._vec, v._vec);
# else
	const __m128 s2 = _mm_mul_ps(v._vec, v._vec);
	__m128 sinp = _mm_mul_ps(v._vec, s2);
	__m128 cosp = _mm_mul_ps(s2, s2);

	s._vec = _mm_add_ps(v._vec, _mm_mul_ps(_mm_set1_ps(-0.16666666666667f), sinp));
	c._vec = _mm_add_ps(_mm_set1_ps(1), _mm_mul_ps(_mm_set1_ps(-0.5f), s2));

	sinp = _mm_mul_ps(sinp, s2);
	s._vec = _mm_add_ps(s._vec, _mm_mul_ps(_mm_set1_ps(0.0083333333333333f), sinp));
	c._vec = _mm_add_ps(c._vec, _mm_mul_ps(_mm_set1_ps(0.041666666666667f), cosp));

	sinp = _mm_mul_ps(sinp, s2);
	s._vec = _mm_add_ps(s._vec, _mm_mul_ps(_mm_set1_ps(-1.9841269841269841e-4f), sinp));
	cosp = _mm_mul_ps(cosp, s2);
	c._vec = _mm_add_ps(c._vec, _mm_mul_ps(_mm_set1_ps(-0.0013888888888889f), cosp));

	sinp = _mm_mul_ps(sinp, s2);
	s._vec = _mm_add_ps(s._vec, _mm_mul_ps(_mm_set1_ps(2.7557319223985893e-6f), sinp));
	cosp = _mm_mul_ps(cosp, s2);
	c._vec = _mm_add_ps(c._vec, _mm_mul_ps(_mm_set1_ps(2.4801587301587302e-5f), cosp));

	sinp = _mm_mul_ps(sinp, s2);
	s._vec = _mm_add_ps(s._vec, _mm_mul_ps(_mm_set1_ps(-2.505210838544172e-8f), sinp));
	cosp = _mm_mul_ps(cosp, s2);
	c._vec = _mm_add_ps(c._vec, _mm_mul_ps(_mm_set1_ps(-2.7557319223985888e-7f), cosp));

#  if PM_MATH_SIN_QUAL == 1 || PM_MATH_COS_QUAL == 1
	sinp = _mm_mul_ps(sinp, s2);
	s._vec = _mm_add_ps(s._vec, _mm_mul_ps(_mm_set1_ps(1.6059043836821613e-10f), sinp));
	cosp = _mm_mul_ps(cosp, s2);
	c._vec = _mm_add_ps(c._vec, _mm_mul_ps(_mm_set1_ps(2.08767569878681e-9f), cosp));

	sinp = _mm_mul_ps(sinp, s2);
	s._vec = _mm_add_ps(s._vec, _mm_mul_ps(_mm_set1_ps(-7.6471637318198164e-13f), sinp));
	cosp = _mm_mul_ps(cosp, s2);
	c._vec = _mm_add_ps(c._vec, _mm_mul_ps(_mm_set1_ps(-1.1470745597729725e-11f), cosp));

	sinp = _mm_mul_ps(sinp, s2);
	s._vec = _mm_add_ps(s._vec, _mm_mul_ps(_mm_set1_ps(2.8114572543455206e-15f), sinp));
	cosp = _mm_mul_ps(cosp, s2);
	c._vec = _mm_add_ps(c._vec, _mm_mul_ps(_mm_set1_ps(4.7794773323873853e-14f), cosp));

	sinp = _mm_mul_ps(sinp, s2);
	s._vec = _mm_add_ps(s._vec, _mm_mul_ps(_mm_set1_ps(-8.2206352466243295e-18f), sinp));
	cosp = _mm_mul_ps(cosp, s2);
	c._vec = _mm_add_ps(c._vec, _mm_mul_ps(_mm_set1_ps(-1.5619206968586225e-16f), cosp));

	sinp = _mm_mul_ps(sinp, s2);
	s._vec = _mm_add_ps(s._vec, _mm_mul_ps(_mm_set1_ps(1.9572941063391263e-20f), sinp));
	cosp = _mm_mul_ps(cosp, s2);
	c._vec = _mm_add_ps(c._vec, _mm_mul_ps(_mm_set1_ps(4.1103176233121648e-19f), cosp));
#  endif//PM_MATH_SIN_QUAL == 1 || PM_MATH_COS_QUAL == 1
# endif//PM_WITH_SVML
#else
	for(int i = 0; i < VectorType::Dimension; ++i)
		pm_SinCos(v._vec[i], s._vec[i], c._vec[i]);
#endif
}

template<>
void PM_MATH_INLINE pm_SinCos(const vec4& v, vec4& s, vec4& c) { _pm_SinCos(v,s,c); }
template<>
void PM_MATH_INLINE pm_SinCos(const vec3& v, vec3& s, vec3& c) { _pm_SinCos(v,s,c); }
template<>
void PM_MATH_INLINE pm_SinCos(const vec2& v, vec2& s, vec2& c) { _pm_SinCos(v,s,c); }

template<typename VectorType>
VectorType PM_MATH_INLINE pm_Tan(const VectorType& v)
{
#if defined(PM_WITH_SIMD) && defined(PM_WITH_SVML)
	return _mm_tan_ps(v._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r = pm_SetIndex(r, i, std::tan(pm_GetIndex(v, i)));
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_ASin(const VectorType& v)
{
#if defined(PM_WITH_SIMD) && defined(PM_WITH_SVML)
	return _mm_asin_ps(v._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r = pm_SetIndex(r, i, std::asin(pm_GetIndex(v, i)));
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_ACos(const VectorType& v)
{
#if defined(PM_WITH_SIMD) && defined(PM_WITH_SVML)
	return _mm_acos_ps(v._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r = pm_SetIndex(r, i, std::acos(pm_GetIndex(v, i)));
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_ATan(const VectorType& v)
{
#if defined(PM_WITH_SIMD) && defined(PM_WITH_SVML)
	return _mm_atan_ps(v._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r = pm_SetIndex(r, i, std::atan(pm_GetIndex(v, i)));
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_Lerp(const VectorType& v1, const VectorType& v2, const VectorType& t)
{
#ifdef PM_WITH_SIMD
	return _mm_add_ps(_mm_mul_ps(_mm_sub_ps(v2._vec, v1._vec), t._vec), v1._vec);
#else
	return pm_Add(v1, pm_Multiply(pm_Subtract(v2, v1), t));
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_NLerp(const VectorType& v1, const VectorType& v2, const VectorType& t)
{
	return pm_Normalize(pm_Add(v1, pm_Multiply(pm_Subtract(v2, v1), t)));
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_SLerp(const VectorType& v1, const VectorType& v2, const VectorType& t)
{
#ifdef PM_WITH_SIMD
	static const __m128 lerp_threshold = _mm_set1_ps(PM_VECTOR_SLERP_THRESHOLD);
	static const __m128 one = _mm_set1_ps(1.0f);

	__m128 cosAngle = _mm_set1_ps(pm_Dot(v1, v2));
	__m128 mask = _mm_cmpgt_ps(lerp_threshold, cosAngle);
	__m128 angle = pm_ACos((VectorType)cosAngle)._vec;
	__m128 decWeight = _mm_sub_ps(one, t._vec);
	__m128 angles = _mm_unpacklo_ps(one, t._vec);
	angles = _mm_unpacklo_ps(angles, decWeight);
	angles = _mm_mul_ps(angles, angle);
	__m128 sines = pm_Sin((VectorType)angles)._vec;
	__m128 scales = _mm_div_ps(sines, _mm_shuffle_ps(sines, sines, _MM_SHUFFLE(0, 0, 0, 0)));
	__m128 scale0 = _mm_or_ps(_mm_and_ps(mask, _mm_shuffle_ps(scales, scales, _MM_SHUFFLE(1, 1, 1, 1))), _mm_andnot_ps(mask, decWeight));
	__m128 scale1 = _mm_or_ps(_mm_and_ps(mask, _mm_shuffle_ps(scales, scales, _MM_SHUFFLE(2, 2, 2, 2))), _mm_andnot_ps(mask, t._vec));
	return _mm_add_ps(_mm_mul_ps(v2._vec, scale1), _mm_mul_ps(v1._vec, scale0));

#else
	float angle = pm_Dot(v1, v2);

	if (angle > PM_VECTOR_SLERP_THRESHOLD)
		return pm_Lerp(v1, v2, t);

	angle = std::acos(angle);
	float sangle = std::sin(angle);
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = (v1._vec[i] * std::sin(angle*(1 - t._vec[i])) + v2._vec[i] * std::sin(angle*t._vec[i])) / sangle;
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE _pm_Max(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_max_ps(v1._vec, v2._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = (v1._vec[i] < v2._vec[i]) ? v1._vec[i] : v2._vec[i];
	return r;
#endif
}

template<>
vec4 PM_MATH_INLINE pm_Max<vec4>(const vec4& v1, const vec4& v2) { return _pm_Max(v1, v2); }
template<>
vec3 PM_MATH_INLINE pm_Max<vec3>(const vec3& v1, const vec3& v2) { return _pm_Max(v1, v2); }
template<>
vec2 PM_MATH_INLINE pm_Max<vec2>(const vec2& v1, const vec2& v2) { return _pm_Max(v1, v2); }

template<typename VectorType>
VectorType PM_MATH_INLINE _pm_Min(const VectorType& v1, const VectorType& v2)
{
#ifdef PM_WITH_SIMD
	return _mm_min_ps(v1._vec, v2._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = (v1._vec[i] > v2._vec[i]) ? v1._vec[i] : v2._vec[i];
	return r;
#endif
}

template<>
vec4 PM_MATH_INLINE pm_Min<vec4>(const vec4& v1, const vec4& v2) { return _pm_Min(v1, v2); }
template<>
vec3 PM_MATH_INLINE pm_Min<vec3>(const vec3& v1, const vec3& v2) { return _pm_Min(v1, v2); }
template<>
vec2 PM_MATH_INLINE pm_Min<vec2>(const vec2& v1, const vec2& v2) { return _pm_Min(v1, v2); }

template<typename VectorType>
VectorType PM_MATH_INLINE _pm_Clamp(const VectorType& v, const VectorType& min, const VectorType& max)
{
#ifdef PM_WITH_SIMD
	return _mm_min_ps(_mm_max_ps(min._vec, v._vec), max._vec);
#else
	return _pm_Min(_pm_Max(min, v), max);
#endif
}

template<>
vec4 PM_MATH_INLINE pm_Clamp<vec4>(const vec4& v, const vec4& min, const vec4& max) { return _pm_Clamp(v, min, max); }
template<>
vec3 PM_MATH_INLINE pm_Clamp<vec3>(const vec3& v, const vec3& min, const vec3& max) { return _pm_Clamp(v, min, max); }
template<>
vec2 PM_MATH_INLINE pm_Clamp<vec2>(const vec2& v, const vec2& min, const vec2& max) { return _pm_Clamp(v, min, max); }

template<typename VectorType>
VectorType PM_MATH_INLINE pm_Abs(const VectorType& v)
{
#ifdef PM_WITH_SIMD
	static const __m128 sign_mask = _mm_set1_ps(-0.0f);
    return _mm_andnot_ps(sign_mask, v._vec);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = std::abs(v._vec[i]);
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_Saturate(const VectorType& v)
{
#ifdef PM_WITH_SIMD
	static const __m128 one = _mm_set1_ps(1.0f);
	static const __m128 zero = _mm_setzero_ps();
	return _mm_min_ps(_mm_max_ps(v._vec, zero), one);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r._vec[i] = (v._vec[i] < 0 ? 0 : (v._vec[i] > 1 ? 1 : v._vec[i]));
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_Ceil(const VectorType& v)
{
#if defined(PM_WITH_SIMD) && defined(PM_WITH_SVML)
	return _mm_ceil_ps(v);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r = pm_SetIndex(r, i, std::ceil(pm_GetIndex(v, i)));
	return r;
#endif
}

template<typename VectorType>
VectorType PM_MATH_INLINE pm_Floor(const VectorType& v)
{
#if defined(PM_WITH_SIMD) && defined(PM_WITH_SVML)
	return _mm_floor_ps(v);
#else
	VectorType r;
	for(int i = 0; i < VectorType::Dimension; ++i)
		r = pm_SetIndex(r, i, std::round(pm_GetIndex(v, i)));
	return r;
#endif
}

//-------------------------------- 4D
vec4 PM_MATH_INLINE pm_Zero4D()
{
#ifdef PM_WITH_SIMD
	return _mm_setzero_ps();
#else
	vec4 v;
	v._vec[0] = 0; v._vec[1] = 0; v._vec[2] = 0; v._vec[3] = 0;
	return v;
#endif
}

vec4 PM_MATH_INLINE pm_One4D()
{
#ifdef PM_WITH_SIMD
	return _mm_set1_ps(1.0f);
#else
	vec4 v;
	v._vec[0] = 1; v._vec[1] = 1; v._vec[2] = 1; v._vec[3] = 1;
	return v;
#endif
}

vec4 PM_MATH_INLINE pm_Set(float x, float y, float z, float w)
{
#ifdef PM_WITH_SIMD
	return _mm_set_ps(w, z, y, x);
#else
	vec4 v;
	v._vec[0] = x;
	v._vec[1] = y;
	v._vec[2] = z;
	v._vec[3] = w;
	return v;
#endif
}

vec4 PM_MATH_INLINE pm_FillVector4D(float val)
{
#ifdef PM_WITH_SIMD
	return _mm_set1_ps(val);
#else
	vec4 r;
	r._vec[0] = val;
	r._vec[1] = val;
	r._vec[2] = val;
	r._vec[3] = val;
	return r;
#endif
}

vec4 PM_MATH_INLINE pm_Load4D(const float src[4])
{
#ifdef PM_WITH_SIMD
	alignas(16) float t[4] = { src[0], src[1], src[2], src[3] };
	return _mm_load_ps(t);
#else
	vec4 r;
	r._vec[0] = src[0];
	r._vec[1] = src[1];
	r._vec[2] = src[2];
	r._vec[3] = src[3];
	return r;
#endif
}

void PM_MATH_INLINE pm_Store4D(const vec4& v, float dst[4])
{
#ifdef PM_WITH_SIMD
	alignas(16) float t[4];
	_mm_store_ps(t, v._vec);
	dst[0] = t[0];
	dst[1] = t[1];
	dst[2] = t[2];
	dst[3] = t[3];
#else
	dst[0] = v._vec[0];
	dst[1] = v._vec[1];
	dst[2] = v._vec[2];
	dst[3] = v._vec[3];
#endif
}

vec4 PM_MATH_INLINE pm_ExtendTo4D(const vec2& v)
{
#ifdef PM_WITH_SIMD
	vec4 r;
	r._vec = v._vec;
	return r;
#else
	return pm_Set(pm_GetX(v), pm_GetY(v), 0, 0);
#endif
}

vec4 PM_MATH_INLINE pm_ExtendTo4D(const vec3& v)
{
#ifdef PM_WITH_SIMD
	vec4 r;
	r._vec = v._vec;
	return r;
#else
	return pm_Set(pm_GetX(v), pm_GetY(v), pm_GetZ(v), 0);
#endif
}

float PM_MATH_INLINE pm_Dot(const vec4& v1, const vec4& v2)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_cvtss_f32(_mm_dp_ps(v1._vec, v2._vec, 0xF1));
# else
	const __m128 s = _mm_mul_ps(v1._vec, v2._vec);
	return _mm_cvtss_f32(_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(3, 3, 3, 3))))));
# endif
#else
	return (v1._vec[0] * v2._vec[0]) + (v1._vec[1] * v2._vec[1]) + (v1._vec[2] * v2._vec[2]) + (v1._vec[3] * v2._vec[3]);
#endif
}

float PM_MATH_INLINE pm_Magnitude(const vec4& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(v._vec, v._vec, 0xF1)));
# else
	const __m128 s = _mm_mul_ps(v._vec, v._vec);
	return _mm_cvtss_f32(_mm_sqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(3, 3, 3, 3)))))));
#endif
#else
	return sqrt(v._vec[0] * v._vec[0] + v._vec[1] * v._vec[1] + v._vec[2] * v._vec[2] + v._vec[3] * v._vec[3]);
#endif
}

float PM_MATH_INLINE pm_MagnitudeSqr(const vec4& v)
{
	return pm_Dot(v, v);
}

vec4 PM_MATH_INLINE pm_Normalize(const vec4& v)
{
#if defined(PM_FAST_MATH)
	return pm_FastNormalize(v);
#else
	return pm_QualityNormalize(v);
#endif
}

vec4 PM_MATH_INLINE pm_QualityNormalize(const vec4& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_div_ps(v._vec, _mm_sqrt_ps(_mm_dp_ps(v._vec, v._vec, 0xFF)));
# else
	__m128 s = _mm_mul_ps(v._vec, v._vec);
	s = _mm_sqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
			_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
				_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)),
					_mm_shuffle_ps(s, s, _MM_SHUFFLE(3, 3, 3, 3))))));
	return _mm_div_ps(v._vec, _mm_shuffle_ps(s, s, 0));
# endif
#else
	return pm_FastNormalize(v);
#endif
}

vec4 PM_MATH_INLINE pm_FastNormalize(const vec4& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_mul_ps(v._vec, _mm_rsqrt_ps(_mm_dp_ps(v._vec, v._vec, 0xFF)));
# else
	__m128 s = _mm_mul_ps(v._vec, v._vec);
	s = _mm_rsqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
			_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
				_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)),
					_mm_shuffle_ps(s, s, _MM_SHUFFLE(3, 3, 3, 3))))));
	return _mm_mul_ps(v._vec, _mm_shuffle_ps(s, s, 0));
# endif
#else
	return pm_Scale(v, 1/pm_Magnitude(v));
#endif
}

float PM_MATH_INLINE pm_MaxElement(const vec4& v)
{
	return PM::pm_Max(PM::pm_GetX(v),
		PM::pm_Max(PM::pm_GetY(v),
		PM::pm_Max(PM::pm_GetZ(v),
		PM::pm_GetW(v))));
}

float PM_MATH_INLINE pm_MinElement(const vec4& v)
{
	return PM::pm_Min(PM::pm_GetX(v),
		PM::pm_Min(PM::pm_GetY(v),
		PM::pm_Min(PM::pm_GetZ(v),
		PM::pm_GetW(v))));
}

//-------------------------------- 3D
vec3 PM_MATH_INLINE pm_Zero3D()
{
#ifdef PM_WITH_SIMD
	return _mm_setzero_ps();
#else
	vec3 v;
	v._vec[0] = 0; v._vec[1] = 0; v._vec[2] = 0; 
	return v;
#endif
}

vec3 PM_MATH_INLINE pm_One3D()
{
#ifdef PM_WITH_SIMD
	return _mm_set1_ps(1.0f);
#else
	vec3 v;
	v._vec[0] = 1; v._vec[1] = 1; v._vec[2] = 1; v._vec[3] = 1;
	return v;
#endif
}

vec3 PM_MATH_INLINE pm_FillVector3D(float val)
{
#ifdef PM_WITH_SIMD
	return _mm_set1_ps(val);
#else
	vec3 r;
	r._vec[0] = val;
	r._vec[1] = val;
	r._vec[2] = val;
	return r;
#endif
}

vec3 PM_MATH_INLINE pm_Set(float x, float y, float z)
{
#ifdef PM_WITH_SIMD
	return _mm_set_ps(0, z, y, x);
#else
	vec3 v;
	v._vec[0] = x;
	v._vec[1] = y;
	v._vec[2] = z;
	return v;
#endif
}

vec3 PM_MATH_INLINE pm_Load3D(const float src[3])
{
#ifdef PM_WITH_SIMD
	alignas(16) float t[4] = { src[0], src[1], src[2], 0 };
	return _mm_load_ps(t);
#else
	vec3 r;
	r._vec[0] = src[0];
	r._vec[1] = src[1];
	r._vec[2] = src[2];
	return r;
#endif
}

void PM_MATH_INLINE pm_Store3D(const vec3& v, float dst[3])
{
#ifdef PM_WITH_SIMD
	alignas(16) float t[4];
	_mm_store_ps(t, v._vec);
	dst[0] = t[0];
	dst[1] = t[1];
	dst[2] = t[2];
#else
	dst[0] = v._vec[0];
	dst[1] = v._vec[1];
	dst[2] = v._vec[2];
#endif
}

vec3 PM_MATH_INLINE pm_ExtendTo3D(const vec2& v)
{
#ifdef PM_WITH_SIMD
	vec3 r;
	r._vec = v._vec;
	return r;
#else
	return pm_Set(pm_GetX(v), pm_GetY(v), 0);
#endif
}

vec3 PM_MATH_INLINE pm_ShrinkTo3D(const vec4& v)
{
#ifdef PM_WITH_SIMD
	vec3 r;
	r._vec = v._vec;
	return r;
#else
	return pm_Set(pm_GetX(v), pm_GetY(v), pm_GetZ(v));
#endif
}

float PM_MATH_INLINE pm_Dot(const vec3& v1, const vec3& v2)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_cvtss_f32(_mm_dp_ps(v1._vec, v2._vec, 0x71));
# else
	const __m128 s = _mm_mul_ps(v1._vec, v2._vec);
	return _mm_cvtss_f32(_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)))));
# endif
#else
	return (v1._vec[0] * v2._vec[0]) + (v1._vec[1] * v2._vec[1]) + (v1._vec[2] * v2._vec[2]);
#endif
}

vec3 PM_MATH_INLINE pm_Cross(const vec3& v1, const vec3& v2)
{
#if defined(PM_WITH_SIMD)
	__m128 result = _mm_sub_ps(
		_mm_mul_ps(v1._vec, _mm_shuffle_ps(v2._vec, v2._vec, _MM_SHUFFLE(3, 0, 2, 1))),
		_mm_mul_ps(v2._vec, _mm_shuffle_ps(v1._vec, v1._vec, _MM_SHUFFLE(3, 0, 2, 1))));
	return _mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 0, 2, 1));
#else
	vec3 r;
	r._vec[0] = v1._vec[1] * v2._vec[2] - v1._vec[2] * v2._vec[1];
	r._vec[1] = v1._vec[2] * v2._vec[0] - v1._vec[0] * v2._vec[2];
	r._vec[2] = v1._vec[0] * v2._vec[1] - v1._vec[1] * v2._vec[0];
	return r;
#endif
}

float PM_MATH_INLINE pm_Magnitude(const vec3& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(v._vec, v._vec, 0x71)));
# else
	const __m128 s = _mm_mul_ps(v._vec, v._vec);
	return _mm_cvtss_f32(_mm_sqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2))))));
# endif
#else
	return std::sqrt(pm_MagnitudeSqr(v));
#endif
}

float PM_MATH_INLINE pm_MagnitudeSqr(const vec3& v)
{
	return pm_Dot(v, v);
}

vec3 PM_MATH_INLINE pm_Normalize(const vec3& v)
{
#if defined(PM_FAST_MATH)
	return pm_FastNormalize(v);
#else
	return pm_QualityNormalize(v);
#endif
}

vec3 PM_MATH_INLINE pm_QualityNormalize(const vec3& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_div_ps(v._vec, _mm_sqrt_ps(_mm_dp_ps(v._vec, v._vec, 0x7F)));
# else
	__m128 s = _mm_mul_ps(v._vec, v._vec);
	s = _mm_sqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
			_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
				_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)))));
	return _mm_div_ps(v._vec, _mm_shuffle_ps(s, s, 0));
#endif
#else
	return pm_FastNormalize(v);
#endif
}

vec3 PM_MATH_INLINE pm_FastNormalize(const vec3& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_mul_ps(v._vec, _mm_rsqrt_ps(_mm_dp_ps(v._vec, v._vec, 0x7F)));// Should be 0x77, but x/0 is not good :P
# else
	__m128 s = _mm_mul_ps(v._vec, v._vec);
	s = _mm_rsqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
			_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
				_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)))));
	return _mm_mul_ps(v._vec, _mm_shuffle_ps(s, s, 0));
#endif
#else
	return pm_Scale(v, 1/pm_Magnitude(v));
#endif
}

float PM_MATH_INLINE pm_MaxElement(const vec3& v)
{
	return PM::pm_Max(PM::pm_GetX(v),
		PM::pm_Max(PM::pm_GetY(v),
		PM::pm_GetZ(v)));
}

float PM_MATH_INLINE pm_MinElement(const vec3& v)
{
	return PM::pm_Min(PM::pm_GetX(v),
		PM::pm_Min(PM::pm_GetY(v),
		PM::pm_GetZ(v)));
}

//-------------------------------- 2D
vec2 PM_MATH_INLINE pm_Zero2D()
{
#ifdef PM_WITH_SIMD
	return _mm_setzero_ps();
#else
	vec2 v;
	v._vec[0] = 0; v._vec[1] = 0;
	return v;
#endif
}

vec2 PM_MATH_INLINE pm_One2D()
{
#ifdef PM_WITH_SIMD
	return _mm_set1_ps(1.0f);
#else
	vec2 v;
	v._vec[0] = 1; v._vec[1] = 1;
	return v;
#endif
}

vec2 PM_MATH_INLINE pm_FillVector2D(float val)
{
#ifdef PM_WITH_SIMD
	return _mm_set1_ps(val);
#else
	vec2 r;
	r._vec[0] = val;
	r._vec[1] = val;
	return r;
#endif
}

vec2 PM_MATH_INLINE pm_Set(float x, float y)
{
#ifdef PM_WITH_SIMD
	return _mm_set_ps(0, 0, y, x);
#else
	vec2 v;
	v._vec[0] = x;
	v._vec[1] = y;
	return v;
#endif
}

vec2 PM_MATH_INLINE pm_Load2D(const float src[2])
{
#ifdef PM_WITH_SIMD
	alignas(16) float t[4] = { src[0], src[1], 0, 0 };
	return _mm_load_ps(t);
#else
	vec2 r;
	r._vec[0] = src[0];
	r._vec[1] = src[1];
	return r;
#endif
}

void PM_MATH_INLINE pm_Store2D(const vec2& v, float dst[2])
{
#ifdef PM_WITH_SIMD
	alignas(16) float t[4];
	_mm_store_ps(t, v._vec);
	dst[0] = t[0];
	dst[1] = t[1];
#else
	dst[0] = v._vec[0];
	dst[1] = v._vec[1];
#endif
}

vec2 PM_MATH_INLINE pm_ShrinkTo2D(const vec4& v)
{
#ifdef PM_WITH_SIMD
	vec2 r;
	r._vec = v._vec;
	return r;
#else
	return pm_Set(pm_GetX(v), pm_GetY(v));
#endif
}

vec2 PM_MATH_INLINE pm_ShrinkTo2D(const vec3& v)
{
#ifdef PM_WITH_SIMD
	vec2 r;
	r._vec = v._vec;
	return r;
#else
	return pm_Set(pm_GetX(v), pm_GetY(v));
#endif
}

float PM_MATH_INLINE pm_Dot(const vec2& v1, const vec2& v2)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_cvtss_f32(_mm_dp_ps(v1._vec, v2._vec, 0x31));
# else
	const __m128 s = _mm_mul_ps(v1._vec, v2._vec);
	return _mm_cvtss_f32(_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1))));
# endif
#else
	return (v1._vec[0] * v2._vec[0]) + (v1._vec[1] * v2._vec[1]);
#endif
}

float PM_MATH_INLINE pm_Magnitude(const vec2& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(v._vec, v._vec, 0x31)));
# else
	const __m128 s = _mm_mul_ps(v._vec, v._vec);
	return _mm_cvtss_f32(_mm_sqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)))));
# endif
#else
	return std::sqrt(pm_MagnitudeSqr(v));
#endif
}

float PM_MATH_INLINE pm_MagnitudeSqr(const vec2& v)
{
	return pm_Dot(v, v);
}

vec2 PM_MATH_INLINE pm_Normalize(const vec2& v)
{
#if defined(PM_FAST_MATH)
	return pm_FastNormalize(v);
#else
	return pm_QualityNormalize(v);
#endif
}

vec2 PM_MATH_INLINE pm_QualityNormalize(const vec2& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_div_ps(v._vec, _mm_sqrt_ps(_mm_dp_ps(v._vec, v._vec, 0x3F)));
# else
	__m128 s = _mm_mul_ps(v._vec, v._vec);
	s = _mm_sqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
			_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1))));
	return _mm_div_ps(v._vec, _mm_shuffle_ps(s, s, 0));
# endif
#else
	return pm_FastNormalize(v);
#endif
}

vec2 PM_MATH_INLINE pm_FastNormalize(const vec2& v)
{
#if defined(PM_WITH_SIMD)
# if defined(PM_WITH_SSE4_1)
	return _mm_mul_ps(v._vec, _mm_rsqrt_ps(_mm_dp_ps(v._vec, v._vec, 0x3F)));// Should be 0x33, but x/0 is not good :P
# else
	__m128 s = _mm_mul_ps(v._vec, v._vec);
	s = _mm_rsqrt_ss(
		_mm_add_ss(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
			_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1))));
	return _mm_mul_ps(v._vec, _mm_shuffle_ps(s, s, 0));
# endif
#else
	return pm_Scale(v, 1/pm_Magnitude(v));
#endif
}

float PM_MATH_INLINE pm_MaxElement(const vec2& v)
{
	return PM::pm_Max(PM::pm_GetX(v), PM::pm_GetY(v));
}

float PM_MATH_INLINE pm_MinElement(const vec2& v)
{
	return PM::pm_Min(PM::pm_GetX(v), PM::pm_GetY(v));
}