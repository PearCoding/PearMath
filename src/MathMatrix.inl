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

#ifndef _PM_MATH_INCLUDED_
# error MathMatrix.inl should only be included from PearMath.h
#endif

#pragma once

// ----------------------------------- 4x4
mat4 PM_MATH_INLINE pm_LoadMatrix4(const float* tr)
{
	mat4 r;
	r.v[0] = pm_Load4D(&tr[0]);
	r.v[1] = pm_Load4D(&tr[4]);
	r.v[2] = pm_Load4D(&tr[8]);
	r.v[3] = pm_Load4D(&tr[12]);
	return r;
}

void PM_MATH_INLINE pm_StoreMatrix4(const mat4& tr, float* r)
{
	pm_Store4D(tr.v[0], &r[0]);
	pm_Store4D(tr.v[1], &r[4]);
	pm_Store4D(tr.v[2], &r[8]);
	pm_Store4D(tr.v[3], &r[12]);
}

mat4 PM_MATH_INLINE pm_Identity4()
{
	mat4 r;
	r.v[0] = pm_Set(1, 0, 0, 0);
	r.v[1] = pm_Set(0, 1, 0, 0);
	r.v[2] = pm_Set(0, 0, 1, 0);
	r.v[3] = pm_Set(0, 0, 0, 1);
	return r;
}

mat4 PM_MATH_INLINE pm_ZeroMatrix4()
{
	mat4 r;
	r.v[0] = pm_Zero4D();
	r.v[1] = pm_Zero4D();
	r.v[2] = pm_Zero4D();
	r.v[3] = pm_Zero4D();
	return r;
}

mat4 PM_MATH_INLINE pm_FillMatrix4(float val)
{
	mat4 r;
	r.v[0] = pm_FillVector4D(val);
	r.v[1] = pm_FillVector4D(val);
	r.v[2] = pm_FillVector4D(val);
	r.v[3] = pm_FillVector4D(val);
	return r;
}

mat4 PM_MATH_INLINE pm_Create(const vec4& r1, const vec4& r2, const vec4& r3, const vec4& r4)
{
	mat4 r;
	r.v[0] = r1;
	r.v[1] = r2;
	r.v[2] = r3;
	r.v[3] = r4;
	return r;
}

mat4 PM_MATH_INLINE pm_Create(float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	mat4 r;
	r.v[0] = pm_Set(m00, m01, m02, m03);
	r.v[1] = pm_Set(m10, m11, m12, m13);
	r.v[2] = pm_Set(m20, m21, m22, m23);
	r.v[3] = pm_Set(m30, m31, m32, m33);
	return r;
}

vec4 PM_MATH_INLINE pm_GetColumn(const mat4& m, int x)
{
	PM_ASSERT(x >= 0 && x < 4);
	return pm_Set(pm_GetIndex(m,0,x), pm_GetIndex(m,1,x), pm_GetIndex(m,2,x), pm_GetIndex(m,3,x));
}

mat4 PM_MATH_INLINE pm_Product(const mat4& m1, const mat4& m2)
{
#ifdef PM_WITH_SIMD
	mat4 r;
	__m128 row = m1.v[0]._vec;

	__m128 x = _mm_shuffle_ps(row, row, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 y = _mm_shuffle_ps(row, row, _MM_SHUFFLE(1, 1, 1, 1));
	__m128 z = _mm_shuffle_ps(row, row, _MM_SHUFFLE(2, 2, 2, 2));
	__m128 w = _mm_shuffle_ps(row, row, _MM_SHUFFLE(3, 3, 3, 3));
	x = _mm_mul_ps(x, m2.v[0]._vec);
	y = _mm_mul_ps(y, m2.v[1]._vec);
	z = _mm_mul_ps(z, m2.v[2]._vec);
	w = _mm_mul_ps(w, m2.v[3]._vec);
	x = _mm_add_ps(x, z);
	y = _mm_add_ps(y, w);
	x = _mm_add_ps(x, y);
	r.v[0] = x;

	row = m1.v[1]._vec;
	x = _mm_shuffle_ps(row, row, _MM_SHUFFLE(0, 0, 0, 0));
	y = _mm_shuffle_ps(row, row, _MM_SHUFFLE(1, 1, 1, 1));
	z = _mm_shuffle_ps(row, row, _MM_SHUFFLE(2, 2, 2, 2));
	w = _mm_shuffle_ps(row, row, _MM_SHUFFLE(3, 3, 3, 3));
	x = _mm_mul_ps(x, m2.v[0]._vec);
	y = _mm_mul_ps(y, m2.v[1]._vec);
	z = _mm_mul_ps(z, m2.v[2]._vec);
	w = _mm_mul_ps(w, m2.v[3]._vec);
	x = _mm_add_ps(x, z);
	y = _mm_add_ps(y, w);
	x = _mm_add_ps(x, y);
	r.v[1] = x;

	row = m1.v[2]._vec;
	x = _mm_shuffle_ps(row, row, _MM_SHUFFLE(0, 0, 0, 0));
	y = _mm_shuffle_ps(row, row, _MM_SHUFFLE(1, 1, 1, 1));
	z = _mm_shuffle_ps(row, row, _MM_SHUFFLE(2, 2, 2, 2));
	w = _mm_shuffle_ps(row, row, _MM_SHUFFLE(3, 3, 3, 3));
	x = _mm_mul_ps(x, m2.v[0]._vec);
	y = _mm_mul_ps(y, m2.v[1]._vec);
	z = _mm_mul_ps(z, m2.v[2]._vec);
	w = _mm_mul_ps(w, m2.v[3]._vec);
	x = _mm_add_ps(x, z);
	y = _mm_add_ps(y, w);
	x = _mm_add_ps(x, y);
	r.v[2] = x;

	row = m1.v[3]._vec;
	x = _mm_shuffle_ps(row, row, _MM_SHUFFLE(0, 0, 0, 0));
	y = _mm_shuffle_ps(row, row, _MM_SHUFFLE(1, 1, 1, 1));
	z = _mm_shuffle_ps(row, row, _MM_SHUFFLE(2, 2, 2, 2));
	w = _mm_shuffle_ps(row, row, _MM_SHUFFLE(3, 3, 3, 3));
	x = _mm_mul_ps(x, m2.v[0]._vec);
	y = _mm_mul_ps(y, m2.v[1]._vec);
	z = _mm_mul_ps(z, m2.v[2]._vec);
	w = _mm_mul_ps(w, m2.v[3]._vec);
	x = _mm_add_ps(x, z);
	y = _mm_add_ps(y, w);
	x = _mm_add_ps(x, y);
	r.v[3] = x;

	return r;
#else
	mat4 r;
	float x = m1.v[0]._vec[0];
	float y = m1.v[0]._vec[1];
	float z = m1.v[0]._vec[2];
	float w = m1.v[0]._vec[3];
	vec4 r1 = m2.v[0];
	vec4 r2 = m2.v[1];
	vec4 r3 = m2.v[2];
	vec4 r4 = m2.v[3];
	r.v[0]._vec[0] = (r1._vec[0] * x) + (r2._vec[0] * y) + (r3._vec[0] * z) + (r4._vec[0] * w);
	r.v[0]._vec[1] = (r1._vec[1] * x) + (r2._vec[1] * y) + (r3._vec[1] * z) + (r4._vec[1] * w);
	r.v[0]._vec[2] = (r1._vec[2] * x) + (r2._vec[2] * y) + (r3._vec[2] * z) + (r4._vec[2] * w);
	r.v[0]._vec[3] = (r1._vec[3] * x) + (r2._vec[3] * y) + (r3._vec[3] * z) + (r4._vec[3] * w);

	x = m1.v[1]._vec[0];
	y = m1.v[1]._vec[1];
	z = m1.v[1]._vec[2];
	w = m1.v[1]._vec[3];
	r.v[1]._vec[0] = (r1._vec[0] * x) + (r2._vec[0] * y) + (r3._vec[0] * z) + (r4._vec[0] * w);
	r.v[1]._vec[1] = (r1._vec[1] * x) + (r2._vec[1] * y) + (r3._vec[1] * z) + (r4._vec[1] * w);
	r.v[1]._vec[2] = (r1._vec[2] * x) + (r2._vec[2] * y) + (r3._vec[2] * z) + (r4._vec[2] * w);
	r.v[1]._vec[3] = (r1._vec[3] * x) + (r2._vec[3] * y) + (r3._vec[3] * z) + (r4._vec[3] * w);

	x = m1.v[2]._vec[0];
	y = m1.v[2]._vec[1];
	z = m1.v[2]._vec[2];
	w = m1.v[2]._vec[3];
	r.v[2]._vec[0] = (r1._vec[0] * x) + (r2._vec[0] * y) + (r3._vec[0] * z) + (r4._vec[0] * w);
	r.v[2]._vec[1] = (r1._vec[1] * x) + (r2._vec[1] * y) + (r3._vec[1] * z) + (r4._vec[1] * w);
	r.v[2]._vec[2] = (r1._vec[2] * x) + (r2._vec[2] * y) + (r3._vec[2] * z) + (r4._vec[2] * w);
	r.v[2]._vec[3] = (r1._vec[3] * x) + (r2._vec[3] * y) + (r3._vec[3] * z) + (r4._vec[3] * w);

	x = m1.v[3]._vec[0];
	y = m1.v[3]._vec[1];
	z = m1.v[3]._vec[2];
	w = m1.v[3]._vec[3];
	r.v[3]._vec[0] = (r1._vec[0] * x) + (r2._vec[0] * y) + (r3._vec[0] * z) + (r4._vec[0] * w);
	r.v[3]._vec[1] = (r1._vec[1] * x) + (r2._vec[1] * y) + (r3._vec[1] * z) + (r4._vec[1] * w);
	r.v[3]._vec[2] = (r1._vec[2] * x) + (r2._vec[2] * y) + (r3._vec[2] * z) + (r4._vec[2] * w);
	r.v[3]._vec[3] = (r1._vec[3] * x) + (r2._vec[3] * y) + (r3._vec[3] * z) + (r4._vec[3] * w);
	return r;
#endif
}

float PM_MATH_INLINE pm_Determinant(const mat4& m)
{
	const mat2 A = pm_Create(pm_GetIndex(m,0,0),pm_GetIndex(m,0,1), pm_GetIndex(m,1,0), pm_GetIndex(m,1,1));
	const mat2 B = pm_Create(pm_GetIndex(m,0,2),pm_GetIndex(m,0,3), pm_GetIndex(m,1,2), pm_GetIndex(m,1,3));
	const mat2 C = pm_Create(pm_GetIndex(m,2,0),pm_GetIndex(m,2,1), pm_GetIndex(m,3,0), pm_GetIndex(m,3,1));
	const mat2 D = pm_Create(pm_GetIndex(m,2,2),pm_GetIndex(m,2,3), pm_GetIndex(m,3,2), pm_GetIndex(m,3,3));

	float detD;
	const float t = pm_Determinant(pm_Subtract(A, pm_Product(B, pm_Product(pm_Inverse(D, &detD), C)))); 
	return t * detD;
}

mat4 PM_MATH_INLINE pm_Inverse(const mat4& m, float* determinant)
{
#ifdef PM_WITH_SIMD
	const mat4 tr = pm_Transpose(m);

	__m128 m00 = _mm_shuffle_ps(tr.v[2]._vec, tr.v[2]._vec, _MM_SHUFFLE(1, 1, 0, 0));
	__m128 m10 = _mm_shuffle_ps(tr.v[3]._vec, tr.v[3]._vec, _MM_SHUFFLE(3, 2, 3, 2));
	__m128 m01 = _mm_shuffle_ps(tr.v[0]._vec, tr.v[0]._vec, _MM_SHUFFLE(1, 1, 0, 0));
	__m128 m11 = _mm_shuffle_ps(tr.v[1]._vec, tr.v[1]._vec, _MM_SHUFFLE(3, 2, 3, 2));
	__m128 m02 = _mm_shuffle_ps(tr.v[2]._vec, tr.v[0]._vec, _MM_SHUFFLE(2, 0, 2, 0));
	__m128 m12 = _mm_shuffle_ps(tr.v[3]._vec, tr.v[1]._vec, _MM_SHUFFLE(3, 1, 3, 1));

	__m128 d0 = _mm_mul_ps(m00, m10);
	__m128 d1 = _mm_mul_ps(m01, m11);
	__m128 d2 = _mm_mul_ps(m02, m12);

	m00 = _mm_shuffle_ps(tr.v[2]._vec, tr.v[2]._vec, _MM_SHUFFLE(3, 2, 3, 2));
	m10 = _mm_shuffle_ps(tr.v[3]._vec, tr.v[3]._vec, _MM_SHUFFLE(1, 1, 0, 0));
	m01 = _mm_shuffle_ps(tr.v[0]._vec, tr.v[0]._vec, _MM_SHUFFLE(3, 2, 3, 2));
	m11 = _mm_shuffle_ps(tr.v[1]._vec, tr.v[1]._vec, _MM_SHUFFLE(1, 1, 0, 0));
	m02 = _mm_shuffle_ps(tr.v[2]._vec, tr.v[0]._vec, _MM_SHUFFLE(3, 1, 3, 1));
	m12 = _mm_shuffle_ps(tr.v[3]._vec, tr.v[1]._vec, _MM_SHUFFLE(2, 0, 2, 0));

	m00 = _mm_mul_ps(m00, m10);
	m01 = _mm_mul_ps(m01, m11);
	m02 = _mm_mul_ps(m02, m12);
	d0 = _mm_sub_ps(d0, m00);
	d1 = _mm_sub_ps(d1, m01);
	d2 = _mm_sub_ps(d2, m02);

	m11 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(1, 1, 3, 1));
	m00 = _mm_shuffle_ps(tr.v[1]._vec, tr.v[1]._vec, _MM_SHUFFLE(1, 0, 2, 1));
	m10 = _mm_shuffle_ps(m11, d0, _MM_SHUFFLE(0, 3, 0, 2));
	m01 = _mm_shuffle_ps(tr.v[0]._vec, tr.v[0]._vec, _MM_SHUFFLE(0, 1, 0, 2));
	m11 = _mm_shuffle_ps(m11, d0, _MM_SHUFFLE(2, 1, 2, 1));

	__m128 m13 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(3, 3, 3, 1));
	m02 = _mm_shuffle_ps(tr.v[3]._vec, tr.v[3]._vec, _MM_SHUFFLE(1, 0, 2, 1));
	m12 = _mm_shuffle_ps(m13, d1, _MM_SHUFFLE(0, 3, 0, 2));
	__m128 m03 = _mm_shuffle_ps(tr.v[2]._vec, tr.v[2]._vec, _MM_SHUFFLE(0, 1, 0, 2));
	m13 = _mm_shuffle_ps(m13, d1, _MM_SHUFFLE(2, 1, 2, 1));

	__m128 clm0 = _mm_mul_ps(m00, m10);
	__m128 clm2 = _mm_mul_ps(m01, m11);
	__m128 clm4 = _mm_mul_ps(m02, m12);
	__m128 clm6 = _mm_mul_ps(m03, m13);

	m11 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(0, 0, 1, 0));
	m00 = _mm_shuffle_ps(tr.v[1]._vec, tr.v[1]._vec, _MM_SHUFFLE(2, 1, 3, 2));
	m10 = _mm_shuffle_ps(d0, m11, _MM_SHUFFLE(2, 1, 0, 3));
	m01 = _mm_shuffle_ps(tr.v[0]._vec, tr.v[0]._vec, _MM_SHUFFLE(1, 3, 2, 3));
	m11 = _mm_shuffle_ps(d0, m11, _MM_SHUFFLE(0, 2, 1, 2));

	m13 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(2, 2, 1, 0));
	m02 = _mm_shuffle_ps(tr.v[3]._vec, tr.v[3]._vec, _MM_SHUFFLE(2, 1, 3, 2));
	m12 = _mm_shuffle_ps(d1, m13, _MM_SHUFFLE(2, 1, 0, 3));
	m03 = _mm_shuffle_ps(tr.v[2]._vec, tr.v[2]._vec, _MM_SHUFFLE(1, 3, 2, 3));
	m13 = _mm_shuffle_ps(d1, m13, _MM_SHUFFLE(0, 2, 1, 2));

	m00 = _mm_mul_ps(m00, m10);
	m01 = _mm_mul_ps(m01, m11);
	m02 = _mm_mul_ps(m02, m12);
	m03 = _mm_mul_ps(m03, m13);
	clm0 = _mm_sub_ps(clm0, m00);
	clm2 = _mm_sub_ps(clm2, m01);
	clm4 = _mm_sub_ps(clm4, m02);
	clm6 = _mm_sub_ps(clm6, m03);

	m00 = _mm_shuffle_ps(tr.v[1]._vec, tr.v[1]._vec, _MM_SHUFFLE(0, 3, 0, 3));
	m10 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(1, 0, 2, 2));
	m10 = _mm_shuffle_ps(m10, m10, _MM_SHUFFLE(0, 2, 3, 0));
	m01 = _mm_shuffle_ps(tr.v[0]._vec, tr.v[0]._vec, _MM_SHUFFLE(2, 0, 3, 1));

	m11 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(1, 0, 3, 0));
	m11 = _mm_shuffle_ps(m11, m11, _MM_SHUFFLE(2, 1, 0, 3));
	m02 = _mm_shuffle_ps(tr.v[3]._vec, tr.v[3]._vec, _MM_SHUFFLE(0, 3, 0, 3));

	m12 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(3, 2, 2, 2));
	m12 = _mm_shuffle_ps(m12, m12, _MM_SHUFFLE(0, 2, 3, 0));
	m03 = _mm_shuffle_ps(tr.v[2]._vec, tr.v[2]._vec, _MM_SHUFFLE(2, 0, 3, 1));

	m13 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(3, 2, 3, 0));
	m13 = _mm_shuffle_ps(m13, m13, _MM_SHUFFLE(2, 1, 0, 3));

	m00 = _mm_mul_ps(m00, m10);
	m01 = _mm_mul_ps(m01, m11);
	m02 = _mm_mul_ps(m02, m12);
	m03 = _mm_mul_ps(m03, m13);

	__m128 clm1 = _mm_sub_ps(clm0, m00);
	clm0 = _mm_add_ps(clm0, m00);
	__m128 clm3 = _mm_add_ps(clm2, m01);
	clm2 = _mm_sub_ps(clm2, m01);
	__m128 clm5 = _mm_sub_ps(clm4, m02);
	clm4 = _mm_add_ps(clm4, m02);
	__m128 clm7 = _mm_add_ps(clm6, m03);
	clm6 = _mm_sub_ps(clm6, m03);

	clm0 = _mm_shuffle_ps(clm0, clm1, _MM_SHUFFLE(3, 1, 2, 0));
	clm2 = _mm_shuffle_ps(clm2, clm3, _MM_SHUFFLE(3, 1, 2, 0));
	clm4 = _mm_shuffle_ps(clm4, clm5, _MM_SHUFFLE(3, 1, 2, 0));
	clm6 = _mm_shuffle_ps(clm6, clm7, _MM_SHUFFLE(3, 1, 2, 0));
	clm0 = _mm_shuffle_ps(clm0, clm0, _MM_SHUFFLE(3, 1, 2, 0));
	clm2 = _mm_shuffle_ps(clm2, clm2, _MM_SHUFFLE(3, 1, 2, 0));
	clm4 = _mm_shuffle_ps(clm4, clm4, _MM_SHUFFLE(3, 1, 2, 0));
	clm6 = _mm_shuffle_ps(clm6, clm6, _MM_SHUFFLE(3, 1, 2, 0));

	const float detf = pm_Dot(clm0, tr.v[0]);
	if (determinant)
		*determinant = detf;

	__m128 det = _mm_set1_ps(1/detf);

	mat4 r;
	r.v[0] = _mm_mul_ps(det, clm0);
	r.v[1] = _mm_mul_ps(det, clm2);
	r.v[2] = _mm_mul_ps(det, clm4);
	r.v[3] = _mm_mul_ps(det, clm6);
	return r;
#else
	float a0 = m.v[0]._vec[0] * m.v[1]._vec[1] - m.v[0]._vec[1] * m.v[1]._vec[0];
	float a1 = m.v[0]._vec[0] * m.v[1]._vec[2] - m.v[0]._vec[2] * m.v[1]._vec[0];
	float a2 = m.v[0]._vec[0] * m.v[1]._vec[3] - m.v[0]._vec[3] * m.v[1]._vec[0];
	float a3 = m.v[0]._vec[1] * m.v[1]._vec[2] - m.v[0]._vec[2] * m.v[1]._vec[1];
	float a4 = m.v[0]._vec[1] * m.v[1]._vec[3] - m.v[0]._vec[3] * m.v[1]._vec[1];
	float a5 = m.v[0]._vec[2] * m.v[1]._vec[3] - m.v[0]._vec[3] * m.v[1]._vec[2];
	float b0 = m.v[2]._vec[0] * m.v[3]._vec[1] - m.v[2]._vec[1] * m.v[3]._vec[0];
	float b1 = m.v[2]._vec[0] * m.v[3]._vec[2] - m.v[2]._vec[2] * m.v[3]._vec[0];
	float b2 = m.v[2]._vec[0] * m.v[3]._vec[3] - m.v[2]._vec[3] * m.v[3]._vec[0];
	float b3 = m.v[2]._vec[1] * m.v[3]._vec[2] - m.v[2]._vec[2] * m.v[3]._vec[1];
	float b4 = m.v[2]._vec[1] * m.v[3]._vec[3] - m.v[2]._vec[3] * m.v[3]._vec[1];
	float b5 = m.v[2]._vec[2] * m.v[3]._vec[3] - m.v[2]._vec[3] * m.v[3]._vec[2];

	float det = a0*b5 - a1*b4 + a2*b3 + a3*b2 - a4*b1 + a5*b0;

	if (determinant)
		*determinant = det;

	mat4 r;

	r.v[0]._vec[0] = +m.v[1]._vec[1] * b5 - m.v[1]._vec[2] * b4 + m.v[1]._vec[3] * b3;
	r.v[1]._vec[0] = -m.v[1]._vec[0] * b5 + m.v[1]._vec[2] * b2 - m.v[1]._vec[3] * b1;
	r.v[2]._vec[0] = +m.v[1]._vec[0] * b4 - m.v[1]._vec[1] * b2 + m.v[1]._vec[3] * b0;
	r.v[3]._vec[0] = -m.v[1]._vec[0] * b3 + m.v[1]._vec[1] * b1 - m.v[1]._vec[2] * b0;
	r.v[0]._vec[1] = -m.v[0]._vec[1] * b5 + m.v[0]._vec[2] * b4 - m.v[0]._vec[3] * b3;
	r.v[1]._vec[1] = +m.v[0]._vec[0] * b5 - m.v[0]._vec[2] * b2 + m.v[0]._vec[3] * b1;
	r.v[2]._vec[1] = -m.v[0]._vec[0] * b4 + m.v[0]._vec[1] * b2 - m.v[0]._vec[3] * b0;
	r.v[3]._vec[1] = +m.v[0]._vec[0] * b3 - m.v[0]._vec[1] * b1 + m.v[0]._vec[2] * b0;
	r.v[0]._vec[2] = +m.v[3]._vec[1] * a5 - m.v[3]._vec[2] * a4 + m.v[3]._vec[3] * a3;
	r.v[1]._vec[2] = -m.v[3]._vec[0] * a5 + m.v[3]._vec[2] * a2 - m.v[3]._vec[3] * a1;
	r.v[2]._vec[2] = +m.v[3]._vec[0] * a4 - m.v[3]._vec[1] * a2 + m.v[3]._vec[3] * a0;
	r.v[3]._vec[2] = -m.v[3]._vec[0] * a3 + m.v[3]._vec[1] * a1 - m.v[3]._vec[2] * a0;
	r.v[0]._vec[3] = -m.v[2]._vec[1] * a5 + m.v[2]._vec[2] * a4 - m.v[2]._vec[3] * a3;
	r.v[1]._vec[3] = +m.v[2]._vec[0] * a5 - m.v[2]._vec[2] * a2 + m.v[2]._vec[3] * a1;
	r.v[2]._vec[3] = -m.v[2]._vec[0] * a4 + m.v[2]._vec[1] * a2 - m.v[2]._vec[3] * a0;
	r.v[3]._vec[3] = +m.v[2]._vec[0] * a3 - m.v[2]._vec[1] * a1 + m.v[2]._vec[2] * a0;

	det = 1.0f / det;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			r.v[i]._vec[j] *= det;
		}
	}

	return r;
#endif
}

mat4 PM_MATH_INLINE pm_Transpose(const mat4& m)
{
#ifdef PM_WITH_SIMD
	mat4 r;
	const __m128 tmp = _mm_shuffle_ps(m.v[0]._vec, m.v[1]._vec, _MM_SHUFFLE(1, 0, 1, 0));
	const __m128 tmp2 = _mm_shuffle_ps(m.v[2]._vec, m.v[3]._vec, _MM_SHUFFLE(1, 0, 1, 0));
	const __m128 tmp3 = _mm_shuffle_ps(m.v[0]._vec, m.v[1]._vec, _MM_SHUFFLE(3, 2, 3, 2));
	const __m128 tmp4 = _mm_shuffle_ps(m.v[2]._vec, m.v[3]._vec, _MM_SHUFFLE(3, 2, 3, 2));
	r.v[0]._vec = _mm_shuffle_ps(tmp, tmp2, _MM_SHUFFLE(2, 0, 2, 0));
	r.v[1]._vec = _mm_shuffle_ps(tmp, tmp2, _MM_SHUFFLE(3, 1, 3, 1));
	r.v[2]._vec = _mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(2, 0, 2, 0));
	r.v[3]._vec = _mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(3, 1, 3, 1));
	return r;
#else
	mat4 r = m;
	std::swap(r.v[0]._vec[1], r.v[1]._vec[0]);
	std::swap(r.v[0]._vec[2], r.v[2]._vec[0]);
	std::swap(r.v[0]._vec[3], r.v[3]._vec[0]);
	std::swap(r.v[1]._vec[2], r.v[2]._vec[1]);
	std::swap(r.v[1]._vec[3], r.v[3]._vec[1]);
	std::swap(r.v[2]._vec[3], r.v[3]._vec[2]);
	return r;
#endif
}

// ----------------------------------- 3x3
mat3 PM_MATH_INLINE pm_LoadMatrix3(const float* tr)
{
	mat3 r;
	r.v[0] = pm_Load3D(&tr[0]);
	r.v[1] = pm_Load3D(&tr[3]);
	r.v[2] = pm_Load3D(&tr[6]);
	return r;
}

void PM_MATH_INLINE pm_StoreMatrix3(const mat3& tr, float* r)
{
	pm_Store3D(tr.v[0], &r[0]);
	pm_Store3D(tr.v[1], &r[3]);
	pm_Store3D(tr.v[2], &r[6]);
}

mat3 PM_MATH_INLINE pm_Identity3()
{
	mat3 r;
	r.v[0] = pm_Set(1, 0, 0);
	r.v[1] = pm_Set(0, 1, 0);
	r.v[2] = pm_Set(0, 0, 1);
	return r;
}

mat3 PM_MATH_INLINE pm_ZeroMatrix3()
{
	mat3 r;
	r.v[0] = pm_Zero3D();
	r.v[1] = pm_Zero3D();
	r.v[2] = pm_Zero3D();
	return r;
}

mat3 PM_MATH_INLINE pm_FillMatrix3(float val)
{
	mat3 r;
	r.v[0] = pm_FillVector3D(val);
	r.v[1] = pm_FillVector3D(val);
	r.v[2] = pm_FillVector3D(val);
	return r;
}

mat3 PM_MATH_INLINE pm_Create(const vec3& r1, const vec3& r2, const vec3& r3)
{
	mat3 r;
	r.v[0] = r1;
	r.v[1] = r2;
	r.v[2] = r3;
	return r;
}

mat3 PM_MATH_INLINE pm_Create(float m00, float m01, float m02,
	float m10, float m11, float m12,
	float m20, float m21, float m22)
{
	mat3 r;
	r.v[0] = pm_Set(m00, m01, m02);
	r.v[1] = pm_Set(m10, m11, m12);
	r.v[2] = pm_Set(m20, m21, m22);
	return r;
}

vec3 PM_MATH_INLINE pm_GetColumn(const mat3& m, int x)
{
	PM_ASSERT(x >= 0 && x < 3);
	return pm_Set(pm_GetIndex(m,0,x), pm_GetIndex(m,1,x), pm_GetIndex(m,2,x));
}

mat3 PM_MATH_INLINE pm_Product(const mat3& m1, const mat3& m2)
{
#ifdef PM_WITH_SIMD
	mat3 r;
	__m128 row = m1.v[0]._vec;

	__m128 x = _mm_shuffle_ps(row, row, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 y = _mm_shuffle_ps(row, row, _MM_SHUFFLE(1, 1, 1, 1));
	__m128 z = _mm_shuffle_ps(row, row, _MM_SHUFFLE(2, 2, 2, 2));
	x = _mm_mul_ps(x, m2.v[0]._vec);
	y = _mm_mul_ps(y, m2.v[1]._vec);
	z = _mm_mul_ps(z, m2.v[2]._vec);
	x = _mm_add_ps(x, z);
	x = _mm_add_ps(x, y);
	r.v[0] = x;

	row = m1.v[1]._vec;
	x = _mm_shuffle_ps(row, row, _MM_SHUFFLE(0, 0, 0, 0));
	y = _mm_shuffle_ps(row, row, _MM_SHUFFLE(1, 1, 1, 1));
	z = _mm_shuffle_ps(row, row, _MM_SHUFFLE(2, 2, 2, 2));
	x = _mm_mul_ps(x, m2.v[0]._vec);
	y = _mm_mul_ps(y, m2.v[1]._vec);
	z = _mm_mul_ps(z, m2.v[2]._vec);
	x = _mm_add_ps(x, z);
	x = _mm_add_ps(x, y);
	r.v[1] = x;

	row = m1.v[2]._vec;
	x = _mm_shuffle_ps(row, row, _MM_SHUFFLE(0, 0, 0, 0));
	y = _mm_shuffle_ps(row, row, _MM_SHUFFLE(1, 1, 1, 1));
	z = _mm_shuffle_ps(row, row, _MM_SHUFFLE(2, 2, 2, 2));
	x = _mm_mul_ps(x, m2.v[0]._vec);
	y = _mm_mul_ps(y, m2.v[1]._vec);
	z = _mm_mul_ps(z, m2.v[2]._vec);
	x = _mm_add_ps(x, z);
	x = _mm_add_ps(x, y);
	r.v[2] = x;

	return r;
#else
	mat3 r;
	float x = m1.v[0]._vec[0];
	float y = m1.v[0]._vec[1];
	float z = m1.v[0]._vec[2];
	vec3 r1 = m2.v[0];
	vec3 r2 = m2.v[1];
	vec3 r3 = m2.v[2];
	r.v[0]._vec[0] = (r1._vec[0] * x) + (r2._vec[0] * y) + (r3._vec[0] * z);
	r.v[0]._vec[1] = (r1._vec[1] * x) + (r2._vec[1] * y) + (r3._vec[1] * z);
	r.v[0]._vec[2] = (r1._vec[2] * x) + (r2._vec[2] * y) + (r3._vec[2] * z);

	x = m1.v[1]._vec[0];
	y = m1.v[1]._vec[1];
	z = m1.v[1]._vec[2];
	r.v[1]._vec[0] = (r1._vec[0] * x) + (r2._vec[0] * y) + (r3._vec[0] * z);
	r.v[1]._vec[1] = (r1._vec[1] * x) + (r2._vec[1] * y) + (r3._vec[1] * z);
	r.v[1]._vec[2] = (r1._vec[2] * x) + (r2._vec[2] * y) + (r3._vec[2] * z);

	x = m1.v[2]._vec[0];
	y = m1.v[2]._vec[1];
	z = m1.v[2]._vec[2];
	r.v[2]._vec[0] = (r1._vec[0] * x) + (r2._vec[0] * y) + (r3._vec[0] * z);
	r.v[2]._vec[1] = (r1._vec[1] * x) + (r2._vec[1] * y) + (r3._vec[1] * z);
	r.v[2]._vec[2] = (r1._vec[2] * x) + (r2._vec[2] * y) + (r3._vec[2] * z);

	return r;
#endif
}

float PM_MATH_INLINE pm_Determinant(const mat3& m)
{
	return pm_GetIndex(m,0,0)*pm_GetIndex(m,1,1)*pm_GetIndex(m,2,2)
		+ pm_GetIndex(m,0,1)*pm_GetIndex(m,1,2)*pm_GetIndex(m,2,0)
		+ pm_GetIndex(m,0,2)*pm_GetIndex(m,1,0)*pm_GetIndex(m,2,1)
		- pm_GetIndex(m,0,2)*pm_GetIndex(m,1,1)*pm_GetIndex(m,2,0)
		- pm_GetIndex(m,0,1)*pm_GetIndex(m,1,0)*pm_GetIndex(m,2,2)
		- pm_GetIndex(m,0,0)*pm_GetIndex(m,1,2)*pm_GetIndex(m,2,1);
}

mat3 PM_MATH_INLINE pm_Inverse(const mat3& m, float* determinant)
{
	const float d = pm_Determinant(m);
	if(determinant)
		*determinant = d;
	
	return pm_Scale(
		pm_Create(pm_GetIndex(m,1,1)*pm_GetIndex(m,2,2) - pm_GetIndex(m,1,2)*pm_GetIndex(m,2,1),
					pm_GetIndex(m,0,2)*pm_GetIndex(m,2,1) - pm_GetIndex(m,0,1)*pm_GetIndex(m,2,2),
					pm_GetIndex(m,0,1)*pm_GetIndex(m,1,2) - pm_GetIndex(m,0,2)*pm_GetIndex(m,1,1),
				 pm_GetIndex(m,1,2)*pm_GetIndex(m,2,0) - pm_GetIndex(m,1,0)*pm_GetIndex(m,2,2),
				 	pm_GetIndex(m,0,0)*pm_GetIndex(m,2,2) - pm_GetIndex(m,0,2)*pm_GetIndex(m,2,0),
					pm_GetIndex(m,0,2)*pm_GetIndex(m,1,0) - pm_GetIndex(m,0,0)*pm_GetIndex(m,1,2),
				 pm_GetIndex(m,1,0)*pm_GetIndex(m,2,1) - pm_GetIndex(m,1,1)*pm_GetIndex(m,2,0),
				 	pm_GetIndex(m,0,1)*pm_GetIndex(m,2,0) - pm_GetIndex(m,0,0)*pm_GetIndex(m,2,1),
					pm_GetIndex(m,0,0)*pm_GetIndex(m,1,1) - pm_GetIndex(m,0,1)*pm_GetIndex(m,1,0)),
		1/d);
}

mat3 PM_MATH_INLINE pm_Transpose(const mat3& m)
{
#ifdef PM_WITH_SIMD
	static const __m128 zero = _mm_setzero_ps();
	mat3 r;
	const __m128 tmp = _mm_shuffle_ps(m.v[0]._vec, m.v[1]._vec, _MM_SHUFFLE(1, 0, 1, 0));
	const __m128 tmp2 = _mm_shuffle_ps(m.v[2]._vec, zero, _MM_SHUFFLE(1, 0, 1, 0));
	const __m128 tmp3 = _mm_shuffle_ps(m.v[0]._vec, m.v[1]._vec, _MM_SHUFFLE(3, 2, 3, 2));
	const __m128 tmp4 = _mm_shuffle_ps(m.v[2]._vec, zero, _MM_SHUFFLE(3, 2, 3, 2));
	r.v[0]._vec = _mm_shuffle_ps(tmp, tmp2, _MM_SHUFFLE(2, 0, 2, 0));
	r.v[1]._vec = _mm_shuffle_ps(tmp, tmp2, _MM_SHUFFLE(3, 1, 3, 1));
	r.v[2]._vec = _mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(2, 0, 2, 0));
	return r;
#else
	mat3 r = m;
	std::swap(r.v[0]._vec[1], r.v[1]._vec[0]);
	std::swap(r.v[0]._vec[2], r.v[2]._vec[0]);
	std::swap(r.v[1]._vec[2], r.v[2]._vec[1]);
	return r;
#endif
}

// ----------------------------------- 2x2
mat2 PM_MATH_INLINE pm_LoadMatrix2(const float* tr)
{
	mat2 r;
	r.v[0] = pm_Load2D(&tr[0]);
	r.v[1] = pm_Load2D(&tr[2]);
	return r;
}

void PM_MATH_INLINE pm_StoreMatrix2(const mat2& tr, float* r)
{
	pm_Store2D(tr.v[0], &r[0]);
	pm_Store2D(tr.v[1], &r[2]);
}

mat2 PM_MATH_INLINE pm_Identity2()
{
	mat2 r;
	r.v[0] = pm_Set(1, 0);
	r.v[1] = pm_Set(0, 1);
	return r;
}

mat2 PM_MATH_INLINE pm_ZeroMatrix2()
{
	mat2 r;
	r.v[0] = pm_Zero2D();
	r.v[1] = pm_Zero2D();
	return r;
}

mat2 PM_MATH_INLINE pm_FillMatrix2(float val)
{
	mat2 r;
	r.v[0] = pm_FillVector2D(val);
	r.v[1] = pm_FillVector2D(val);
	return r;
}

mat2 PM_MATH_INLINE pm_Create(const vec2& r1, const vec2& r2)
{
	mat2 r;
	r.v[0] = r1;
	r.v[1] = r2;
	return r;
}

mat2 PM_MATH_INLINE pm_Create(float m00, float m01, float m10, float m11)
{
	mat2 r;
	r.v[0] = pm_Set(m00, m01);
	r.v[1] = pm_Set(m10, m11);
	return r;
}

vec2 PM_MATH_INLINE pm_GetColumn(const mat2& m, int x)
{
	PM_ASSERT(x >= 0 && x < 2);
	return pm_Set(pm_GetIndex(m,0,x), pm_GetIndex(m,1,x));
}

mat2 PM_MATH_INLINE pm_Product(const mat2& m1, const mat2& m2)
{
#ifdef PM_WITH_SIMD
	mat2 r;
	__m128 row = m1.v[0]._vec;

	__m128 x = _mm_shuffle_ps(row, row, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 y = _mm_shuffle_ps(row, row, _MM_SHUFFLE(1, 1, 1, 1));
	x = _mm_mul_ps(x, m2.v[0]._vec);
	y = _mm_mul_ps(y, m2.v[1]._vec);
	x = _mm_add_ps(x, y);
	r.v[0] = x;

	row = m1.v[1]._vec;
	x = _mm_shuffle_ps(row, row, _MM_SHUFFLE(0, 0, 0, 0));
	y = _mm_shuffle_ps(row, row, _MM_SHUFFLE(1, 1, 1, 1));
	x = _mm_mul_ps(x, m2.v[0]._vec);
	y = _mm_mul_ps(y, m2.v[1]._vec);
	x = _mm_add_ps(x, y);
	r.v[1] = x;

	return r;
#else
	mat2 r;
	float x = m1.v[0]._vec[0];
	float y = m1.v[0]._vec[1];
	vec2 r1 = m2.v[0];
	vec2 r2 = m2.v[1];
	r.v[0]._vec[0] = (r1._vec[0] * x) + (r2._vec[0] * y);
	r.v[0]._vec[1] = (r1._vec[1] * x) + (r2._vec[1] * y);

	x = m1.v[1]._vec[0];
	y = m1.v[1]._vec[1];
	r.v[1]._vec[0] = (r1._vec[0] * x) + (r2._vec[0] * y);
	r.v[1]._vec[1] = (r1._vec[1] * x) + (r2._vec[1] * y);

	return r;
#endif
}

float PM_MATH_INLINE pm_Determinant(const mat2& m)
{
	return pm_GetIndex(m,0,0)*pm_GetIndex(m,1,1) - pm_GetIndex(m,0,1)*pm_GetIndex(m,1,0);
}

mat2 PM_MATH_INLINE pm_Inverse(const mat2& m, float* determinant)
{
	const float d = pm_Determinant(m);
	if(determinant)
		*determinant = d;
	
	return pm_Scale(pm_Create(pm_GetIndex(m, 1,1), -pm_GetIndex(m,0,1),
				-pm_GetIndex(m,1,0), pm_GetIndex(m,0,0)), 1/d);
}

mat2 PM_MATH_INLINE pm_Transpose(const mat2& m)
{
#ifdef PM_WITH_SIMD
	mat2 r;
	const __m128 tmp = _mm_shuffle_ps(m.v[0]._vec, m.v[1]._vec, _MM_SHUFFLE(1, 0, 1, 0));
	r.v[0]._vec = _mm_shuffle_ps(tmp, m.v[0]._vec, _MM_SHUFFLE(3, 2, 2, 0));
	r.v[1]._vec = _mm_shuffle_ps(tmp, m.v[0]._vec, _MM_SHUFFLE(3, 2, 3, 1));
	return r;
#else
	mat2 r;
	r.v[0]._vec[0] = m.v[0]._vec[0];
	r.v[0]._vec[1] = m.v[1]._vec[0];
	r.v[1]._vec[0] = m.v[0]._vec[1];
	r.v[1]._vec[1] = m.v[1]._vec[1];
	return r;
#endif
}

// ------------------------------ Other
template<typename MatrixType>
MatrixType PM_MATH_INLINE pm_SetIndex(const MatrixType& m, int x, int y, float f)
{
	MatrixType r = m;
	PM_ASSERT(x >= 0 && x < MatrixType::Rows);
	PM_ASSERT(y >= 0 && y < MatrixType::Columns);
	r.v[x] = pm_SetIndex(m.v[x],y,f);
	return r;
}

template<typename MatrixType>
float PM_MATH_INLINE pm_GetIndex(const MatrixType& m, int x, int y)
{
	PM_ASSERT(x >= 0 && x < MatrixType::Rows);
	PM_ASSERT(y >= 0 && y < MatrixType::Columns);
	return pm_GetIndex(m.v[x],y);
}

template<typename MatrixType>
typename MatrixType::row_t PM_MATH_INLINE pm_GetRow(const MatrixType& m, int x)
{
	PM_ASSERT(x >= 0 && x < MatrixType::Rows);
	return m.v[x];
}

mat4 PM_MATH_INLINE pm_Translation(const vec3& v)
{
	mat4 r;
	r.v[0] = pm_Set(1, 0, 0, pm_GetX(v));
	r.v[1] = pm_Set(0, 1, 0, pm_GetY(v));
	r.v[2] = pm_Set(0, 0, 1, pm_GetZ(v));
	r.v[3] = pm_Set(0, 0, 0, 1);
	return r;
}

//TODO: Optimize...
mat4 PM_MATH_INLINE pm_Rotation(const quat& v)
{
	float xx = pm_GetX(v) * pm_GetX(v);
	float xy = pm_GetX(v) * pm_GetY(v);
	float xz = pm_GetX(v) * pm_GetZ(v);
	float xw = pm_GetX(v) * pm_GetW(v);

	float yy = pm_GetY(v) * pm_GetY(v);
	float yz = pm_GetY(v) * pm_GetZ(v);
	float yw = pm_GetY(v) * pm_GetW(v);

	float zz = pm_GetZ(v) * pm_GetZ(v);
	float zw = pm_GetZ(v) * pm_GetW(v);

	mat4 r;
	r.v[0] = pm_Set(1 - 2 * (yy + zz),
		2 * (xy - zw),
		2 * (xz + yw),
		0);

	r.v[1] = pm_Set(2 * (xy + zw),
		1 - 2 * (xx + zz),
		2 * (yz - xw),
		0);

	r.v[2] = pm_Set(2 * (xz - yw),
		2 * (yz + xw),
		1 - 2 * (xx + yy),
		0);

	r.v[3] = pm_Set(0, 0, 0, 1);

	return r;
}

//TODO: Optimize...
mat4 PM_MATH_INLINE pm_Rotation(const vec3& v, float angle)
{
	float s, c;
	pm_SinCos(angle, s, c);
	float t = 1 - c;

	float x2 = pm_GetX(v)*pm_GetX(v);
	float y2 = pm_GetY(v)*pm_GetY(v);
	float z2 = pm_GetZ(v)*pm_GetZ(v);
	float txy = t * pm_GetX(v) * pm_GetY(v);
	float txz = t * pm_GetX(v) * pm_GetZ(v);
	float tyz = t * pm_GetY(v) * pm_GetZ(v);
	float sx = s * pm_GetX(v);
	float sy = s * pm_GetY(v);
	float sz = s * pm_GetZ(v);

	mat4 r;
	r.v[0] = pm_Set(t*x2 + c, txy - sz, txz + sy, 0);
	r.v[1] = pm_Set(txy + sz, t*y2 + c, tyz - sx, 0);
	r.v[2] = pm_Set(txz - sy, tyz + sx, t*z2 + c, 0);
	r.v[3] = pm_Set(0, 0, 0, 1);
	return r;
}

mat4 PM_MATH_INLINE pm_RotationYawPitchRoll(const vec3& v)
{
	vec3 si;
	vec3 co;
	pm_SinCos(v, si, co);

	mat4 r;
	r.v[0] = pm_Set(pm_GetX(co)*pm_GetY(co), pm_GetX(co)*pm_GetY(si)*pm_GetZ(si) - pm_GetX(si)*pm_GetZ(co), pm_GetX(co)*pm_GetY(si)*pm_GetZ(co) + pm_GetX(si)*pm_GetZ(si), 0);
	r.v[1] = pm_Set(pm_GetX(si)*pm_GetY(co), pm_GetX(si)*pm_GetY(si)*pm_GetZ(si) + pm_GetX(co)*pm_GetZ(co), pm_GetX(si)*pm_GetY(si)*pm_GetZ(co) - pm_GetX(co)*pm_GetZ(si), 0);
	r.v[2] = pm_Set(-pm_GetY(si), pm_GetY(co)*pm_GetZ(si), pm_GetY(co)*pm_GetZ(co), 0);
	r.v[3] = pm_Set(0, 0, 0, 1);
	return r;
}

mat4 PM_MATH_INLINE pm_Scaling(const vec3& v)
{
	mat4 r;
	r.v[0] = pm_Set(pm_GetX(v), 0, 0, 0);
	r.v[1] = pm_Set(0, pm_GetY(v), 0, 0);
	r.v[2] = pm_Set(0, 0, pm_GetZ(v), 0);
	r.v[3] = pm_Set(0, 0, 0, 1);
	return r;
}

vec3 PM_MATH_INLINE pm_DecomposeTranslation(const mat4& m)
{
	return pm_ShrinkTo3D(pm_GetColumn(m, 3));
}

vec3 PM_MATH_INLINE pm_DecomposeScale(const mat4& m)
{
	return pm_Set(pm_Magnitude(pm_GetColumn(m,0)), pm_Magnitude(pm_GetColumn(m,1)), pm_Magnitude(pm_GetColumn(m,2)));
}

quat PM_MATH_INLINE pm_DecomposeRotation(const mat4& m)
{
	vec4 is = pm_ExtendTo4D(pm_Reciprocal(pm_DecomposeScale(m)));
	mat4 rm = pm_Create(pm_Multiply(pm_GetRow(m,0), is), pm_Multiply(pm_GetRow(m,1), is), pm_Multiply(pm_GetRow(m,2), is), pm_Set(0,0,0,1));
	return PM::pm_Normalize(pm_RotationMatrix(rm));
}

void PM_MATH_INLINE pm_Decompose(const mat4& m, vec3& t, vec3& s, quat& r)
{
	t = pm_DecomposeTranslation(m);
	s = pm_DecomposeScale(m);

	vec4 is = pm_ExtendTo4D(pm_Reciprocal(s));
	mat4 rm = pm_Create(pm_Multiply(pm_GetRow(m,0), is), pm_Multiply(pm_GetRow(m,1), is), pm_Multiply(pm_GetRow(m,2), is), pm_Set(0,0,0,1));
	r = PM::pm_Normalize(pm_RotationMatrix(rm));
}

vec3 PM_MATH_INLINE pm_Transform(const mat4& m, const vec3& v)
{
	vec4 t = pm_ExtendTo4D(v);
	return pm_ShrinkTo3D(pm_Product(m, pm_SetW(t,1)));
}

// --------------------------------- Relations
template<typename MatrixType>
typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
	PM_MATH_INLINE pm_IsEqualv(const MatrixType& m1, const MatrixType& m2)
{
	MatrixType r;
	for(int i = 0; i < MatrixType::Rows; ++i)
		r.v[i] = pm_IsEqualv(m1.v[i], m2.v[i]);
	return r;
}

template<typename MatrixType>
typename std::enable_if<is_matrix<MatrixType>::value, bool>::type
	PM_MATH_INLINE pm_IsEqual(const MatrixType& m1, const MatrixType& m2)
{
	for(int i = 0; i < MatrixType::Rows; ++i)
	{
		if(!pm_IsEqual(m1.v[i], m2.v[i]))
			return false;
	}
	return true;
}

template<typename MatrixType>
typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
	PM_MATH_INLINE pm_IsNearlyEqualv(const MatrixType& m1, const MatrixType& m2, const MatrixType& delta)
{
	MatrixType r;
	for(int i = 0; i < MatrixType::Rows; ++i)
		r.v[i] = pm_IsNearlyEqualv(m1.v[i], m2.v[i], delta.v[i]);
	return r;
}

template<typename MatrixType>
typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
	PM_MATH_INLINE pm_IsNearlyEqualv(const MatrixType& m1, const MatrixType& m2, float delta)
{
	MatrixType r;
	for(int i = 0; i < MatrixType::Rows; ++i)
		r.v[i] = pm_IsNearlyEqualv(m1.v[i], m2.v[i], delta);
	return r;
}

template<typename MatrixType>
typename std::enable_if<is_matrix<MatrixType>::value, bool>::type
	PM_MATH_INLINE pm_IsNearlyEqual(const MatrixType& m1, const MatrixType& m2, const MatrixType& delta)
{
	for(int i = 0; i < MatrixType::Rows; ++i)
	{
		if(!pm_IsNearlyEqual(m1.v[i], m2.v[i], delta.v[i]))
			return false;
	}
	return true;
}

template<typename MatrixType>
typename std::enable_if<is_matrix<MatrixType>::value, bool>::type
	PM_MATH_INLINE pm_IsNearlyEqual(const MatrixType& m1, const MatrixType& m2, float delta)
{
	for(int i = 0; i < MatrixType::Rows; ++i)
	{
		if(!pm_IsNearlyEqual(m1.v[i], m2.v[i], delta))
			return false;
	}
	return true;
}

template<typename MatrixType>
typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
	PM_MATH_INLINE pm_IsNotEqualv(const MatrixType& m1, const MatrixType& m2)
{
	MatrixType r;
	for(int i = 0; i < MatrixType::Rows; ++i)
		r.v[i] = pm_IsNotEqualv(m1.v[i], m2.v[i]);
	return r;
}

template<typename MatrixType>
typename std::enable_if<is_matrix<MatrixType>::value, bool>::type
	PM_MATH_INLINE pm_IsNotEqual(const MatrixType& m1, const MatrixType& m2)
{
	for(int i = 0; i < MatrixType::Rows; ++i)
	{
		if(!pm_IsNotEqual(m1.v[i], m2.v[i]))
			return false;
	}
	return true;
}

template<typename MatrixType>
typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
	PM_MATH_INLINE pm_Negate(const MatrixType& m)
{
	MatrixType r;
	for(int i = 0; i < MatrixType::Rows; ++i)
		r.v[i] = pm_Negate(m.v[i]);
	return r;
}

template<typename MatrixType>
typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
	PM_MATH_INLINE pm_Add(const MatrixType& m1, const MatrixType& m2)
{
	MatrixType r;
	for(int i = 0; i < MatrixType::Rows; ++i)
		r.v[i] = pm_Add(m1.v[i], m2.v[i]);
	return r;
}

template<typename MatrixType>
typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
	PM_MATH_INLINE pm_Subtract(const MatrixType& m1, const MatrixType& m2)
{
	MatrixType r;
	for(int i = 0; i < MatrixType::Rows; ++i)
		r.v[i] = pm_Subtract(m1.v[i], m2.v[i]);
	return r;
}

template<typename MatrixType>
typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
	PM_MATH_INLINE pm_Multiply(const MatrixType& m1, const MatrixType& m2)
{
	MatrixType r;
	for(int i = 0; i < MatrixType::Rows; ++i)
		r.v[i] = pm_Multiply(m1.v[i], m2.v[i]);
	return r;
}

template<typename MatrixType>
typename MatrixType::row_t PM_MATH_INLINE pm_Product(const MatrixType& m, const typename MatrixType::row_t& v)
{
	typename MatrixType::row_t r;
	for(int i = 0; i < MatrixType::Rows; ++i)
		r = PM::pm_SetIndex(r, i, pm_Dot(m.v[i], v));
	return r;
}

template<typename MatrixType>
typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
	PM_MATH_INLINE pm_Scale(const MatrixType& m, float s)
{
	MatrixType r;
	for(int i = 0; i < MatrixType::Rows; ++i)
		r.v[i] = pm_Scale(m.v[i], s);
	return r;
}

template<typename MatrixType>
typename std::enable_if<is_matrix<MatrixType>::value, MatrixType>::type
	PM_MATH_INLINE pm_Divide(const MatrixType& m1, const MatrixType& m2)
{
	MatrixType r;
	for(int i = 0; i < MatrixType::Rows; ++i)
		r.v[i] = pm_Divide(m1.v[i], m2.v[i]);
	return r;
}

//Right handled
mat4 PM_MATH_INLINE pm_Perspective(float width, float height, float n, float f)
{
	float near2 = n + n;
	float range = f / (n - f);

	mat4 r;
	r.v[0] = pm_Set(near2 / width, 0.0f, 0.0f, 0.0f);
	r.v[1] = pm_Set(0.0f, near2 / height, 0.0f, 0.0f);
	r.v[2] = pm_Set(0.0f, 0.0f, range, -1.0f);
	r.v[3] = pm_Set(0.0f, 0.0f, range * n, 0.0f);

	return r;
}

mat4 PM_MATH_INLINE pm_Orthographic(float width, float height, float n, float f)
{
	mat4 r;
	r.v[0] = pm_Set(-2.0f / width, 0.0f, 0.0f, -1.0f);
	r.v[1] = pm_Set(0.0f, 2.0f / height, 0.0f, 1.0f);
	r.v[2] = pm_Set(0.0f, 0.0f, -2.0f / (f - n), -(f + n) / (f - n));
	r.v[3] = pm_Set(0.0f, 0.0f, 0.0f, 1.0f);

	return r;
}

mat4 PM_MATH_INLINE pm_Orthographic2D(float width, float height)
{
	mat4 r;
	r.v[0] = pm_Set(2.0f / width, 0.0f, 0.0f, -1.0f);
	r.v[1] = pm_Set(0.0f, -2.0f / height, 0.0f, 1.0f);
	r.v[2] = pm_Set(0.0f, 0.0f, -1.0f, 0.0f);
	r.v[3] = pm_Set(0.0f, 0.0f, 0.0f, 1.0f);

	return r;
}