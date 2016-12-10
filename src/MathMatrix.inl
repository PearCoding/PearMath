/*
* Copyright(c) 2014, OEmercan Yazici <pearcoding AT gmail.com>
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
# error MathMatrix.inl should only be included from Math.h
#endif

#pragma once

mat PM_MATH_INLINE pm_LoadMatrix(const float* tr)
{
#ifdef PM_WITH_SIMD
	mat r;
	r.v[0] = _mm_load_ps(&tr[0]);
	r.v[1] = _mm_load_ps(&tr[4]);
	r.v[2] = _mm_load_ps(&tr[8]);
	r.v[3] = _mm_load_ps(&tr[12]);
	return r;
#else
	mat r;
	r.v[0] = pm_Set(tr[0], tr[1], tr[2], tr[3]);
	r.v[1] = pm_Set(tr[4], tr[5], tr[6], tr[7]);
	r.v[2] = pm_Set(tr[8], tr[9], tr[10], tr[11]);
	r.v[3] = pm_Set(tr[12], tr[13], tr[14], tr[15]);
	return r;
#endif
}

void PM_MATH_INLINE pm_StoreMatrix(const mat& tr, float* r)
{
	//Reverse???
#ifdef PM_WITH_SIMD
	_mm_store_ps(&r[0], tr.v[0]);
	_mm_store_ps(&r[4], tr.v[1]);
	_mm_store_ps(&r[8], tr.v[2]);
	_mm_store_ps(&r[12], tr.v[3]);
#else
	r[0] = tr.v[0][0];
	r[1] = tr.v[0][1];
	r[2] = tr.v[0][2];
	r[3] = tr.v[0][3];
	r[4] = tr.v[1][0];
	r[5] = tr.v[1][1];
	r[6] = tr.v[1][2];
	r[7] = tr.v[1][3];
	r[8] = tr.v[2][0];
	r[9] = tr.v[2][1];
	r[10] = tr.v[2][2];
	r[11] = tr.v[2][3];
	r[12] = tr.v[3][0];
	r[13] = tr.v[3][1];
	r[14] = tr.v[3][2];
	r[15] = tr.v[3][3];
#endif
}

mat PM_MATH_INLINE pm_Identity()
{
	mat r;
	r.v[0] = pm_Set(1, 0, 0, 0);
	r.v[1] = pm_Set(0, 1, 0, 0);
	r.v[2] = pm_Set(0, 0, 1, 0);
	r.v[3] = pm_Set(0, 0, 0, 1);
	return r;
}

mat PM_MATH_INLINE pm_ZeroMatrix()
{
#ifdef PM_WITH_SIMD
	mat r;
	r.v[0] = _mm_setzero_ps();
	r.v[1] = _mm_setzero_ps();
	r.v[2] = _mm_setzero_ps();
	r.v[3] = _mm_setzero_ps();
	return r;
#else
	mat r;
	memset(r.v, 0, sizeof(float) * 16);
	return r;
#endif
}

mat PM_MATH_INLINE pm_Translation(const vec& v)
{
	mat r;
	r.v[0] = pm_Set(1, 0, 0, pm_GetX(v));
	r.v[1] = pm_Set(0, 1, 0, pm_GetY(v));
	r.v[2] = pm_Set(0, 0, 1, pm_GetZ(v));
	r.v[3] = pm_Set(0, 0, 0, 1);
	return r;
}

//TODO: Optimize...
mat PM_MATH_INLINE pm_Rotation(const quat& v)
{
#ifdef PM_WITH_SIMD
	float xx = pm_GetX(v) * pm_GetX(v);
	float xy = pm_GetX(v) * pm_GetY(v);
	float xz = pm_GetX(v) * pm_GetZ(v);
	float xw = pm_GetX(v) * pm_GetW(v);

	float yy = pm_GetY(v) * pm_GetY(v);
	float yz = pm_GetY(v) * pm_GetZ(v);
	float yw = pm_GetY(v) * pm_GetW(v);

	float zz = pm_GetZ(v) * pm_GetZ(v);
	float zw = pm_GetZ(v) * pm_GetW(v);

	mat r;
	r.v[0] = _mm_setr_ps(1 - 2 * (yy + zz),
		2 * (xy - zw),
		2 * (xz + yw),
		0);

	r.v[1] = _mm_setr_ps(2 * (xy + zw),
		1 - 2 * (xx + zz),
		2 * (yz - xw),
		0);

	r.v[2] = _mm_setr_ps(2 * (xz - yw),
		2 * (yz + xw),
		1 - 2 * (xx + yy),
		0);

	r.v[3] = _mm_setr_ps(0, 0, 0, 1);

	return r;
#else
	float xx = v[0] * v[0];
	float xy = v[0] * v[1];
	float xz = v[0] * v[2];
	float xw = v[0] * v[3];

	float yy = v[1] * v[1];
	float yz = v[1] * v[2];
	float yw = v[1] * v[3];

	float zz = v[2] * v[2];
	float zw = v[2] * v[3];

	mat r;
	r[0][0] = 1 - 2 * (yy + zz);
	r[0][1] = 2 * (xy - zw);
	r[0][2] = 2 * (xz + yw);
	r[0][3] = 0;

	r[1][0] = 2 * (xy + zw);
	r[1][1] = 1 - 2 * (xx + zz);
	r[1][2] = 2 * (yz - xw);
	r[1][3] = 0;

	r[2][0] = 2 * (xz - yw);
	r[2][1] = 2 * (yz + xw);
	r[2][2] = 1 - 2 * (xx + yy);
	r[2][3] = 0;

	r[3][0] = 0;
	r[3][1] = 0;
	r[3][2] = 0;
	r[3][3] = 1;

	return r;
#endif
}

//TODO: Optimize...
mat PM_MATH_INLINE pm_Rotation(const vec& v, float angle)
{
#ifdef PM_WITH_SIMD
	float c = cosf(angle);
	float s = sinf(angle);
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

	mat r;
	r.v[0] = pm_Set(t*x2 + c, txy - sz, txz + sy, 0);
	r.v[1] = pm_Set(txy + sz, t*y2 + c, tyz - sx, 0);
	r.v[2] = pm_Set(txz - sy, tyz + sx, t*z2 + c, 0);
	r.v[3] = pm_Set(0, 0, 0, 1);
	return r;
#else
	float c = cosf(angle);
	float s = sinf(angle);
	float t = 1 - c;

	float x2 = v[0] * v[0];
	float y2 = v[1] * v[1];
	float z2 = v[2] * v[2];
	float txy = t * v[0] * v[1];
	float txz = t * v[0] * v[2];
	float tyz = t * v[1] * v[2];
	float sx = s * v[0];
	float sy = s * v[1];
	float sz = s * v[2];

	mat r;
	r.v[0] = pm_Set(t*x2 + c, txy - sz, txz + sy, 0);
	r.v[1] = pm_Set(txy + sz, t*y2 + c, tyz - sx, 0);
	r.v[2] = pm_Set(txz - sy, tyz + sx, t*z2 + c, 0);
	r.v[3] = pm_Set(0, 0, 0, 1);
	return r;
#endif
}

mat PM_MATH_INLINE pm_RotationYawPitchRoll(const vec& v)
{
#ifdef PM_WITH_SIMD
	vec si;
	vec co;
	pm_SinCos(v, si, co);

	mat r;
	r.v[0] = pm_Set(pm_GetX(co)*pm_GetY(co), pm_GetX(co)*pm_GetY(si)*pm_GetZ(si) - pm_GetX(si)*pm_GetZ(co), pm_GetX(co)*pm_GetY(si)*pm_GetZ(co) + pm_GetX(si)*pm_GetZ(si), 0);
	r.v[1] = pm_Set(pm_GetX(si)*pm_GetY(co), pm_GetX(si)*pm_GetY(si)*pm_GetZ(si) + pm_GetX(co)*pm_GetZ(co), pm_GetX(si)*pm_GetY(si)*pm_GetZ(co) - pm_GetX(co)*pm_GetZ(si), 0);
	r.v[2] = pm_Set(-pm_GetY(si), pm_GetY(co)*pm_GetZ(si), pm_GetY(co)*pm_GetZ(co), 0);
	r.v[3] = pm_Set(0, 0, 0, 1);
	return r;
#else
	vec si;
	vec co;
	pm_SinCos(v, si, co);

	mat r;
	r.v[0] = pm_Set(co[0] * co[1], co[0] * si[1] * si[2] - si[0] * co[2], co[0] * si[1] * co[2] + si[0] * si[2], 0);
	r.v[1] = pm_Set(si[0] * co[1], si[0] * si[1] * si[2] + co[0] * co[2], si[0] * si[1] * co[2] - co[0] * si[2], 0);
	r.v[2] = pm_Set(-si[1], co[1] * si[2], co[1] * co[2], 0);
	r.v[3] = pm_Set(0, 0, 0, 1);
	return r;
#endif
}

mat PM_MATH_INLINE pm_Scaling(const vec3& v)
{
	mat r;
	r.v[0] = pm_Set(pm_GetX(v), 0, 0, 0);
	r.v[1] = pm_Set(0, pm_GetY(v), 0, 0);
	r.v[2] = pm_Set(0, 0, pm_GetZ(v), 0);
	r.v[3] = pm_Set(0, 0, 0, 1);
	return r;
}

mat PM_MATH_INLINE pm_Set2D(const vec& r1, const vec& r2)
{
	mat r;
	r.v[0] = pm_SetW(pm_SetZ(r1,0),0);
	r.v[1] = pm_SetW(pm_SetZ(r2,0),0);
	r.v[2] = pm_Zero();
	r.v[3] = pm_Zero();
	return r;
}

mat PM_MATH_INLINE pm_Set2D(float m00, float m01, float m10, float m11)
{
	mat r;
	r.v[0] = pm_Set(m00, m01, 0, 0);
	r.v[1] = pm_Set(m10, m11, 0, 0);
	r.v[2] = pm_Zero();
	r.v[3] = pm_Zero();
	return r;
}

mat PM_MATH_INLINE pm_Set3D(const vec& r1, const vec& r2, const vec& r3)
{
	mat r;
	r.v[0] = pm_SetW(r1,0);
	r.v[1] = pm_SetW(r2,0);
	r.v[2] = pm_SetW(r3,0);
	r.v[3] = pm_Zero();
	return r;
}

mat PM_MATH_INLINE pm_Set3D(float m00, float m01, float m02,
	float m10, float m11, float m12,
	float m20, float m21, float m22)
{
	mat r;
	r.v[0] = pm_Set(m00, m01, m02, 0);
	r.v[1] = pm_Set(m10, m11, m12, 0);
	r.v[2] = pm_Set(m20, m21, m22, 0);
	r.v[3] = pm_Zero();
	return r;
}

mat PM_MATH_INLINE pm_Set4D(const vec& r1, const vec& r2, const vec& r3, const vec& r4)
{
	mat r;
	r.v[0] = r1;
	r.v[1] = r2;
	r.v[2] = r3;
	r.v[3] = r4;
	return r;
}

mat PM_MATH_INLINE pm_Set4D(float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	mat r;
	r.v[0] = pm_Set(m00, m01, m02, m03);
	r.v[1] = pm_Set(m10, m11, m12, m13);
	r.v[2] = pm_Set(m20, m21, m22, m23);
	r.v[3] = pm_Set(m30, m31, m32, m33);
	return r;
}

float PM_MATH_INLINE pm_Get(const mat& m, int x, int y)
{
	PM_ASSERT(x >= 0 && x < 4);
	PM_ASSERT(y >= 0 && y < 4);

	return pm_GetIndex(m.v[y],x);
}

vec PM_MATH_INLINE pm_GetRow(const mat& m, int y)
{
	PM_ASSERT(y >= 0 && y < 4);
	return m.v[y];
}

vec PM_MATH_INLINE pm_GetColumn(const mat& m, int x)
{
	PM_ASSERT(x >= 0 && x < 4);
	return pm_Set(pm_Get(m,x,0), pm_Get(m,x,1), pm_Get(m,x,2), pm_Get(m,x,3));
}

vec PM_MATH_INLINE pm_DecomposeTranslation(const mat& m)
{
	return pm_GetColumn(m, 3);
}

vec3 PM_MATH_INLINE pm_DecomposeScale(const mat& m)
{
	return pm_Set(pm_Magnitude3D(pm_GetColumn(m,0)), pm_Magnitude3D(pm_GetColumn(m,1)), pm_Magnitude3D(pm_GetColumn(m,2)), 1);
}

quat PM_MATH_INLINE pm_DecomposeRotation(const mat& m)
{
	vec is = pm_SetW(pm_Reciprocal(pm_DecomposeScale(m)), 0);
	mat rm = pm_Set4D(pm_Multiply(pm_GetRow(m,0), is), pm_Multiply(pm_GetRow(m,1), is), pm_Multiply(pm_GetRow(m,2), is), pm_Set(0,0,0,1));
	return PM::pm_Normalize4D(pm_RotationMatrix(rm));
}

void PM_MATH_INLINE pm_Decompose(const mat& m, vec& t, vec3& s, quat& r)
{
	t = pm_DecomposeTranslation(m);
	s = pm_DecomposeScale(m);

	vec is = pm_SetW(pm_Reciprocal(s), 0);
	mat rm = pm_Set4D(pm_Multiply(pm_GetRow(m,0), is), pm_Multiply(pm_GetRow(m,1), is), pm_Multiply(pm_GetRow(m,2), is), pm_Set(0,0,0,1));
	r = PM::pm_Normalize4D(pm_RotationMatrix(rm));
}

mat PM_MATH_INLINE pm_FillMatrix(float val)
{
	mat r;
	r.v[0] = pm_FillVector(val);
	r.v[1] = pm_FillVector(val);
	r.v[2] = pm_FillVector(val);
	r.v[3] = pm_FillVector(val);
	return r;
}

mat PM_MATH_INLINE pm_IsEqualv(const mat& m1, const mat& m2)
{
	mat r;
	r.v[0] = pm_IsEqualv(m1.v[0], m2.v[0]);
	r.v[1] = pm_IsEqualv(m1.v[1], m2.v[1]);
	r.v[2] = pm_IsEqualv(m1.v[2], m2.v[2]);
	r.v[3] = pm_IsEqualv(m1.v[3], m2.v[3]);
	return r;
}

bool PM_MATH_INLINE pm_IsEqual(const mat& m1, const mat& m2)
{
	return pm_IsEqual(m1.v[0], m2.v[0]) && pm_IsEqual(m1.v[1], m2.v[1]) && pm_IsEqual(m1.v[2], m2.v[2]) && pm_IsEqual(m1.v[3], m2.v[3]);
}

mat PM_MATH_INLINE pm_IsNearlyEqualv(const mat& m1, const mat& m2, const mat& delta)
{
	mat r;
	r.v[0] = pm_IsNearlyEqualv(m1.v[0], m2.v[0], delta.v[0]);
	r.v[1] = pm_IsNearlyEqualv(m1.v[1], m2.v[1], delta.v[1]);
	r.v[2] = pm_IsNearlyEqualv(m1.v[2], m2.v[2], delta.v[2]);
	r.v[3] = pm_IsNearlyEqualv(m1.v[3], m2.v[3], delta.v[3]);
	return r;
}

mat PM_MATH_INLINE pm_IsNearlyEqualv(const mat& m1, const mat& m2, float delta)
{
	mat r;
	r.v[0] = pm_IsNearlyEqualv(m1.v[0], m2.v[0], pm_FillVector(delta));
	r.v[1] = pm_IsNearlyEqualv(m1.v[1], m2.v[1], pm_FillVector(delta));
	r.v[2] = pm_IsNearlyEqualv(m1.v[2], m2.v[2], pm_FillVector(delta));
	r.v[3] = pm_IsNearlyEqualv(m1.v[3], m2.v[3], pm_FillVector(delta));
	return r;
}

bool PM_MATH_INLINE pm_IsNearlyEqual(const mat& m1, const mat& m2, const mat& delta)
{
	return pm_IsNearlyEqual(m1.v[0], m2.v[0], delta.v[0]) && pm_IsNearlyEqual(m1.v[1], m2.v[1], delta.v[1]) && pm_IsNearlyEqual(m1.v[2], m2.v[2], delta.v[2]) && pm_IsNearlyEqual(m1.v[3], m2.v[3], delta.v[3]);
}

bool PM_MATH_INLINE pm_IsNearlyEqual(const mat& m1, const mat& m2, float delta)
{
	return pm_IsNearlyEqual(m1.v[0], m2.v[0], pm_FillVector(delta)) && pm_IsNearlyEqual(m1.v[1], m2.v[1], pm_FillVector(delta)) && pm_IsNearlyEqual(m1.v[2], m2.v[2], pm_FillVector(delta)) && pm_IsNearlyEqual(m1.v[3], m2.v[3], pm_FillVector(delta));
}

mat PM_MATH_INLINE pm_IsNotEqualv(const mat& m1, const mat& m2)
{
	mat r;
	r.v[0] = pm_IsNotEqualv(m1.v[0], m2.v[0]);
	r.v[1] = pm_IsNotEqualv(m1.v[1], m2.v[1]);
	r.v[2] = pm_IsNotEqualv(m1.v[2], m2.v[2]);
	r.v[3] = pm_IsNotEqualv(m1.v[3], m2.v[3]);
	return r;
}

bool PM_MATH_INLINE pm_IsNotEqual(const mat& m1, const mat& m2)
{
	return pm_IsNotEqual(m1.v[0], m2.v[0]) || pm_IsNotEqual(m1.v[1], m2.v[1]) || pm_IsNotEqual(m1.v[2], m2.v[2]) || pm_IsNotEqual(m1.v[3], m2.v[3]);
}

mat PM_MATH_INLINE pm_Negate(const mat& m)
{
#ifdef PM_WITH_SIMD
	mat r;
	vec zero = _mm_setzero_ps();
	r.v[0] = _mm_sub_ps(zero, m.v[0]);
	r.v[1] = _mm_sub_ps(zero, m.v[1]);
	r.v[2] = _mm_sub_ps(zero, m.v[2]);
	r.v[3] = _mm_sub_ps(zero, m.v[3]);
	return r;
#else
	mat r;
	r.v[0] = pm_Negate(m.v[0]);
	r.v[1] = pm_Negate(m.v[1]);
	r.v[2] = pm_Negate(m.v[2]);
	r.v[3] = pm_Negate(m.v[3]);
	return r;
#endif
}

mat PM_MATH_INLINE pm_Add(const mat& m1, const mat& m2)
{
	mat r;
	r.v[0] = pm_Add(m1.v[0], m2.v[0]);
	r.v[1] = pm_Add(m1.v[1], m2.v[1]);
	r.v[2] = pm_Add(m1.v[2], m2.v[2]);
	r.v[3] = pm_Add(m1.v[3], m2.v[3]);
	return r;
}

mat PM_MATH_INLINE pm_Add(const mat& m, float s)
{
	mat r;
	vec add = pm_FillVector(s);
	r.v[0] = pm_Add(m.v[0], add);
	r.v[1] = pm_Add(m.v[1], add);
	r.v[2] = pm_Add(m.v[2], add);
	r.v[3] = pm_Add(m.v[3], add);
	return r;
}

mat PM_MATH_INLINE pm_Subtract(const mat& m1, const mat& m2)
{
	mat r;
	r.v[0] = pm_Subtract(m1.v[0], m2.v[0]);
	r.v[1] = pm_Subtract(m1.v[1], m2.v[1]);
	r.v[2] = pm_Subtract(m1.v[2], m2.v[2]);
	r.v[3] = pm_Subtract(m1.v[3], m2.v[3]);
	return r;
}

mat PM_MATH_INLINE pm_Subtract(const mat& m, float s)
{
	mat r;
	vec sub = pm_FillVector(s);
	r.v[0] = pm_Subtract(m.v[0], sub);
	r.v[1] = pm_Subtract(m.v[1], sub);
	r.v[2] = pm_Subtract(m.v[2], sub);
	r.v[3] = pm_Subtract(m.v[3], sub);
	return r;
}

mat PM_MATH_INLINE pm_Multiply(const mat& m1, const mat& m2)
{
#ifdef PM_WITH_SIMD
	mat r;
	vec row = m1.v[0];

	vec x = _mm_shuffle_ps(row, row, _MM_SHUFFLE(0, 0, 0, 0));
	vec y = _mm_shuffle_ps(row, row, _MM_SHUFFLE(1, 1, 1, 1));
	vec z = _mm_shuffle_ps(row, row, _MM_SHUFFLE(2, 2, 2, 2));
	vec w = _mm_shuffle_ps(row, row, _MM_SHUFFLE(3, 3, 3, 3));
	x = _mm_mul_ps(x, m2.v[0]);
	y = _mm_mul_ps(y, m2.v[1]);
	z = _mm_mul_ps(z, m2.v[2]);
	w = _mm_mul_ps(w, m2.v[3]);
	x = _mm_add_ps(x, z);
	y = _mm_add_ps(y, w);
	x = _mm_add_ps(x, y);
	r.v[0] = x;

	row = m1.v[1];
	x = _mm_shuffle_ps(row, row, _MM_SHUFFLE(0, 0, 0, 0));
	y = _mm_shuffle_ps(row, row, _MM_SHUFFLE(1, 1, 1, 1));
	z = _mm_shuffle_ps(row, row, _MM_SHUFFLE(2, 2, 2, 2));
	w = _mm_shuffle_ps(row, row, _MM_SHUFFLE(3, 3, 3, 3));
	x = _mm_mul_ps(x, m2.v[0]);
	y = _mm_mul_ps(y, m2.v[1]);
	z = _mm_mul_ps(z, m2.v[2]);
	w = _mm_mul_ps(w, m2.v[3]);
	x = _mm_add_ps(x, z);
	y = _mm_add_ps(y, w);
	x = _mm_add_ps(x, y);
	r.v[1] = x;

	row = m1.v[2];
	x = _mm_shuffle_ps(row, row, _MM_SHUFFLE(0, 0, 0, 0));
	y = _mm_shuffle_ps(row, row, _MM_SHUFFLE(1, 1, 1, 1));
	z = _mm_shuffle_ps(row, row, _MM_SHUFFLE(2, 2, 2, 2));
	w = _mm_shuffle_ps(row, row, _MM_SHUFFLE(3, 3, 3, 3));
	x = _mm_mul_ps(x, m2.v[0]);
	y = _mm_mul_ps(y, m2.v[1]);
	z = _mm_mul_ps(z, m2.v[2]);
	w = _mm_mul_ps(w, m2.v[3]);
	x = _mm_add_ps(x, z);
	y = _mm_add_ps(y, w);
	x = _mm_add_ps(x, y);
	r.v[2] = x;

	row = m1.v[3];
	x = _mm_shuffle_ps(row, row, _MM_SHUFFLE(0, 0, 0, 0));
	y = _mm_shuffle_ps(row, row, _MM_SHUFFLE(1, 1, 1, 1));
	z = _mm_shuffle_ps(row, row, _MM_SHUFFLE(2, 2, 2, 2));
	w = _mm_shuffle_ps(row, row, _MM_SHUFFLE(3, 3, 3, 3));
	x = _mm_mul_ps(x, m2.v[0]);
	y = _mm_mul_ps(y, m2.v[1]);
	z = _mm_mul_ps(z, m2.v[2]);
	w = _mm_mul_ps(w, m2.v[3]);
	x = _mm_add_ps(x, z);
	y = _mm_add_ps(y, w);
	x = _mm_add_ps(x, y);
	r.v[3] = x;

	return r;
#else
	mat r;
	float x = m1.m[0][0];
	float y = m1.m[0][1];
	float z = m1.m[0][2];
	float w = m1.m[0][3];
	vec r1 = m2.v[0];
	vec r2 = m2.v[1];
	vec r3 = m2.v[2];
	vec r4 = m2.v[3];
	r.m[0][0] = (r1[0] * x) + (r2[0] * y) + (r3[0] * z) + (r4[0] * w);
	r.m[0][1] = (r1[1] * x) + (r2[1] * y) + (r3[1] * z) + (r4[1] * w);
	r.m[0][2] = (r1[2] * x) + (r2[2] * y) + (r3[2] * z) + (r4[2] * w);
	r.m[0][3] = (r1[3] * x) + (r2[3] * y) + (r3[3] * z) + (r4[3] * w);

	x = m1.m[1][0];
	y = m1.m[1][1];
	z = m1.m[1][2];
	w = m1.m[1][3];
	r.m[1][0] = (r1[0] * x) + (r2[0] * y) + (r3[0] * z) + (r4[0] * w);
	r.m[1][1] = (r1[1] * x) + (r2[1] * y) + (r3[1] * z) + (r4[1] * w);
	r.m[1][2] = (r1[2] * x) + (r2[2] * y) + (r3[2] * z) + (r4[2] * w);
	r.m[1][3] = (r1[3] * x) + (r2[3] * y) + (r3[3] * z) + (r4[3] * w);

	x = m1.m[2][0];
	y = m1.m[2][1];
	z = m1.m[2][2];
	w = m1.m[2][3];
	r.m[2][0] = (r1[0] * x) + (r2[0] * y) + (r3[0] * z) + (r4[0] * w);
	r.m[2][1] = (r1[1] * x) + (r2[1] * y) + (r3[1] * z) + (r4[1] * w);
	r.m[2][2] = (r1[2] * x) + (r2[2] * y) + (r3[2] * z) + (r4[2] * w);
	r.m[2][3] = (r1[3] * x) + (r2[3] * y) + (r3[3] * z) + (r4[3] * w);

	x = m1.m[3][0];
	y = m1.m[3][1];
	z = m1.m[3][2];
	w = m1.m[3][3];
	r.m[3][0] = (r1[0] * x) + (r2[0] * y) + (r3[0] * z) + (r4[0] * w);
	r.m[3][1] = (r1[1] * x) + (r2[1] * y) + (r3[1] * z) + (r4[1] * w);
	r.m[3][2] = (r1[2] * x) + (r2[2] * y) + (r3[2] * z) + (r4[2] * w);
	r.m[3][3] = (r1[3] * x) + (r2[3] * y) + (r3[3] * z) + (r4[3] * w);
	return r;
#endif
}

vec PM_MATH_INLINE pm_Multiply(const mat& m, const vec& v)
{
#ifdef PM_WITH_SIMD
# ifndef PM_WITH_SSE3
	return PM::pm_Set(pm_Dot4D(m.v[0], v),
		pm_Dot4D(m.v[1], v),
		pm_Dot4D(m.v[2], v),
		pm_Dot4D(m.v[3], v));
# else
	vec m0 = _mm_mul_ps(m.v[0], v);
	vec m1 = _mm_mul_ps(m.v[1], v);
	vec m2 = _mm_mul_ps(m.v[2], v);
	vec m3 = _mm_mul_ps(m.v[3], v);

	return _mm_hadd_ps(_mm_hadd_ps(m0, m1), _mm_hadd_ps(m2, m3));
# endif
#else
	vec r;
	r[0] = pm_Dot4D(m.v[0], v);
	r[1] = pm_Dot4D(m.v[1], v);
	r[2] = pm_Dot4D(m.v[2], v);
	r[3] = pm_Dot4D(m.v[3], v);
	return r;
#endif
}

mat PM_MATH_INLINE pm_MultiplyElement(const mat& m1, const mat& m2)
{
	mat r;
	r.v[0] = pm_Multiply(m1.v[0], m2.v[0]);
	r.v[1] = pm_Multiply(m1.v[1], m2.v[1]);
	r.v[2] = pm_Multiply(m1.v[2], m2.v[2]);
	r.v[3] = pm_Multiply(m1.v[3], m2.v[3]);
	return r;
}

mat PM_MATH_INLINE pm_Multiply(const mat& m, float s)
{
	mat r;
	vec mul = pm_FillVector(s);
	r.v[0] = pm_Multiply(m.v[0], mul);
	r.v[1] = pm_Multiply(m.v[1], mul);
	r.v[2] = pm_Multiply(m.v[2], mul);
	r.v[3] = pm_Multiply(m.v[3], mul);
	return r;
}

mat PM_MATH_INLINE pm_Divide(const mat& m1, const mat& m2)
{
	mat r;
	r.v[0] = pm_Divide(m1.v[0], m2.v[0]);
	r.v[1] = pm_Divide(m1.v[1], m2.v[1]);
	r.v[2] = pm_Divide(m1.v[2], m2.v[2]);
	r.v[3] = pm_Divide(m1.v[3], m2.v[3]);
	return r;
}

mat PM_MATH_INLINE pm_Divide(const mat& m, float s)
{
	PM_DEBUG_ASSERT(s);
	mat r;
	vec div = pm_FillVector(s);
	r.v[0] = pm_Divide(m.v[0], div);
	r.v[1] = pm_Divide(m.v[1], div);
	r.v[2] = pm_Divide(m.v[2], div);
	r.v[3] = pm_Divide(m.v[3], div);
	return r;
}

mat PM_MATH_INLINE pm_Transpose(const mat& m)
{
#ifdef PM_WITH_SIMD
	mat r;

	vec tmp = _mm_shuffle_ps(m.v[0], m.v[1], _MM_SHUFFLE(1, 0, 1, 0));
	vec tmp2 = _mm_shuffle_ps(m.v[2], m.v[3], _MM_SHUFFLE(1, 0, 1, 0));
	vec tmp3 = _mm_shuffle_ps(m.v[0], m.v[1], _MM_SHUFFLE(3, 2, 3, 2));
	vec tmp4 = _mm_shuffle_ps(m.v[2], m.v[3], _MM_SHUFFLE(3, 2, 3, 2));

	r.v[0] = _mm_shuffle_ps(tmp, tmp2, _MM_SHUFFLE(2, 0, 2, 0));
	r.v[1] = _mm_shuffle_ps(tmp, tmp2, _MM_SHUFFLE(3, 1, 3, 1));
	r.v[2] = _mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(2, 0, 2, 0));
	r.v[3] = _mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(3, 1, 3, 1));

	return r;
#else
	mat tmp;
	tmp.v[0][0] = m.v[0][0];
	tmp.v[0][1] = m.v[2][0];
	tmp.v[0][2] = m.v[0][1];
	tmp.v[0][3] = m.v[2][1];

	tmp.v[1][0] = m.v[1][0];
	tmp.v[1][1] = m.v[3][0];
	tmp.v[1][2] = m.v[1][1];
	tmp.v[1][3] = m.v[3][1];

	tmp.v[2][0] = m.v[0][2];
	tmp.v[2][1] = m.v[2][2];
	tmp.v[2][2] = m.v[0][3];
	tmp.v[2][3] = m.v[2][3];

	tmp.v[3][0] = m.v[1][2];
	tmp.v[3][1] = m.v[3][2];
	tmp.v[3][2] = m.v[1][3];
	tmp.v[3][3] = m.v[3][3];

	mat r;
	r.v[0][0] = tmp.v[0][0];
	r.v[0][1] = tmp.v[1][0];
	r.v[0][2] = tmp.v[0][1];
	r.v[0][3] = tmp.v[1][1];

	r.v[1][0] = tmp.v[0][2];
	r.v[1][1] = tmp.v[1][2];
	r.v[1][2] = tmp.v[0][3];
	r.v[1][3] = tmp.v[1][3];

	r.v[2][0] = tmp.v[2][0];
	r.v[2][1] = tmp.v[3][0];
	r.v[2][2] = tmp.v[2][1];
	r.v[2][3] = tmp.v[3][1];

	r.v[3][0] = tmp.v[2][2];
	r.v[3][1] = tmp.v[3][2];
	r.v[3][2] = tmp.v[2][3];
	r.v[3][3] = tmp.v[3][3];

	return r;
#endif
}

mat PM_MATH_INLINE pm_Inverse2D(const mat& m, float* determinant)
{
	const float d = pm_Determinant2D(m);
	if(determinant)
		*determinant = d;
	
	return pm_Multiply(pm_Set2D(m[1][1], -m[0][1], -m[1][0], m[0][0]), 1/d);
}

mat PM_MATH_INLINE pm_Inverse3D(const mat& m, float* determinant)
{
	const float d = pm_Determinant3D(m);
	if(determinant)
		*determinant = d;
	
	return pm_Multiply(
		pm_Set3D(m[1][1]*m[2][2] - m[1][2]*m[2][1], m[0][2]*m[2][1] - m[0][1]*m[2][2], m[0][1]*m[1][2] - m[0][2]*m[1][1],
				 m[1][2]*m[2][0] - m[1][0]*m[2][2], m[0][0]*m[2][2] - m[0][2]*m[2][0], m[0][2]*m[1][0] - m[0][0]*m[1][2],
				 m[1][0]*m[2][1] - m[1][1]*m[2][0], m[0][1]*m[2][0] - m[0][0]*m[2][1], m[0][0]*m[1][1] - m[0][1]*m[1][0]),
		1/d);
}

mat PM_MATH_INLINE pm_Inverse4D(const mat& m, float* determinant)
{
#ifdef PM_WITH_SIMD
	mat tr = pm_Transpose(m);

	vec m00 = _mm_shuffle_ps(tr.v[2], tr.v[2], _MM_SHUFFLE(1, 1, 0, 0));
	vec m10 = _mm_shuffle_ps(tr.v[3], tr.v[3], _MM_SHUFFLE(3, 2, 3, 2));
	vec m01 = _mm_shuffle_ps(tr.v[0], tr.v[0], _MM_SHUFFLE(1, 1, 0, 0));
	vec m11 = _mm_shuffle_ps(tr.v[1], tr.v[1], _MM_SHUFFLE(3, 2, 3, 2));
	vec m02 = _mm_shuffle_ps(tr.v[2], tr.v[0], _MM_SHUFFLE(2, 0, 2, 0));
	vec m12 = _mm_shuffle_ps(tr.v[3], tr.v[1], _MM_SHUFFLE(3, 1, 3, 1));

	vec d0 = _mm_mul_ps(m00, m10);
	vec d1 = _mm_mul_ps(m01, m11);
	vec d2 = _mm_mul_ps(m02, m12);

	m00 = _mm_shuffle_ps(tr.v[2], tr.v[2], _MM_SHUFFLE(3, 2, 3, 2));
	m10 = _mm_shuffle_ps(tr.v[3], tr.v[3], _MM_SHUFFLE(1, 1, 0, 0));
	m01 = _mm_shuffle_ps(tr.v[0], tr.v[0], _MM_SHUFFLE(3, 2, 3, 2));
	m11 = _mm_shuffle_ps(tr.v[1], tr.v[1], _MM_SHUFFLE(1, 1, 0, 0));
	m02 = _mm_shuffle_ps(tr.v[2], tr.v[0], _MM_SHUFFLE(3, 1, 3, 1));
	m12 = _mm_shuffle_ps(tr.v[3], tr.v[1], _MM_SHUFFLE(2, 0, 2, 0));

	m00 = _mm_mul_ps(m00, m10);
	m01 = _mm_mul_ps(m01, m11);
	m02 = _mm_mul_ps(m02, m12);
	d0 = _mm_sub_ps(d0, m00);
	d1 = _mm_sub_ps(d1, m01);
	d2 = _mm_sub_ps(d2, m02);

	m11 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(1, 1, 3, 1));
	m00 = _mm_shuffle_ps(tr.v[1], tr.v[1], _MM_SHUFFLE(1, 0, 2, 1));
	m10 = _mm_shuffle_ps(m11, d0, _MM_SHUFFLE(0, 3, 0, 2));
	m01 = _mm_shuffle_ps(tr.v[0], tr.v[0], _MM_SHUFFLE(0, 1, 0, 2));
	m11 = _mm_shuffle_ps(m11, d0, _MM_SHUFFLE(2, 1, 2, 1));

	vec m13 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(3, 3, 3, 1));
	m02 = _mm_shuffle_ps(tr.v[3], tr.v[3], _MM_SHUFFLE(1, 0, 2, 1));
	m12 = _mm_shuffle_ps(m13, d1, _MM_SHUFFLE(0, 3, 0, 2));
	vec m03 = _mm_shuffle_ps(tr.v[2], tr.v[2], _MM_SHUFFLE(0, 1, 0, 2));
	m13 = _mm_shuffle_ps(m13, d1, _MM_SHUFFLE(2, 1, 2, 1));

	vec clm0 = _mm_mul_ps(m00, m10);
	vec clm2 = _mm_mul_ps(m01, m11);
	vec clm4 = _mm_mul_ps(m02, m12);
	vec clm6 = _mm_mul_ps(m03, m13);

	m11 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(0, 0, 1, 0));
	m00 = _mm_shuffle_ps(tr.v[1], tr.v[1], _MM_SHUFFLE(2, 1, 3, 2));
	m10 = _mm_shuffle_ps(d0, m11, _MM_SHUFFLE(2, 1, 0, 3));
	m01 = _mm_shuffle_ps(tr.v[0], tr.v[0], _MM_SHUFFLE(1, 3, 2, 3));
	m11 = _mm_shuffle_ps(d0, m11, _MM_SHUFFLE(0, 2, 1, 2));

	m13 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(2, 2, 1, 0));
	m02 = _mm_shuffle_ps(tr.v[3], tr.v[3], _MM_SHUFFLE(2, 1, 3, 2));
	m12 = _mm_shuffle_ps(d1, m13, _MM_SHUFFLE(2, 1, 0, 3));
	m03 = _mm_shuffle_ps(tr.v[2], tr.v[2], _MM_SHUFFLE(1, 3, 2, 3));
	m13 = _mm_shuffle_ps(d1, m13, _MM_SHUFFLE(0, 2, 1, 2));

	m00 = _mm_mul_ps(m00, m10);
	m01 = _mm_mul_ps(m01, m11);
	m02 = _mm_mul_ps(m02, m12);
	m03 = _mm_mul_ps(m03, m13);
	clm0 = _mm_sub_ps(clm0, m00);
	clm2 = _mm_sub_ps(clm2, m01);
	clm4 = _mm_sub_ps(clm4, m02);
	clm6 = _mm_sub_ps(clm6, m03);

	m00 = _mm_shuffle_ps(tr.v[1], tr.v[1], _MM_SHUFFLE(0, 3, 0, 3));
	m10 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(1, 0, 2, 2));
	m10 = _mm_shuffle_ps(m10, m10, _MM_SHUFFLE(0, 2, 3, 0));
	m01 = _mm_shuffle_ps(tr.v[0], tr.v[0], _MM_SHUFFLE(2, 0, 3, 1));

	m11 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(1, 0, 3, 0));
	m11 = _mm_shuffle_ps(m11, m11, _MM_SHUFFLE(2, 1, 0, 3));
	m02 = _mm_shuffle_ps(tr.v[3], tr.v[3], _MM_SHUFFLE(0, 3, 0, 3));

	m12 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(3, 2, 2, 2));
	m12 = _mm_shuffle_ps(m12, m12, _MM_SHUFFLE(0, 2, 3, 0));
	m03 = _mm_shuffle_ps(tr.v[2], tr.v[2], _MM_SHUFFLE(2, 0, 3, 1));

	m13 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(3, 2, 3, 0));
	m13 = _mm_shuffle_ps(m13, m13, _MM_SHUFFLE(2, 1, 0, 3));

	m00 = _mm_mul_ps(m00, m10);
	m01 = _mm_mul_ps(m01, m11);
	m02 = _mm_mul_ps(m02, m12);
	m03 = _mm_mul_ps(m03, m13);

	vec clm1 = _mm_sub_ps(clm0, m00);
	clm0 = _mm_add_ps(clm0, m00);
	vec clm3 = _mm_add_ps(clm2, m01);
	clm2 = _mm_sub_ps(clm2, m01);
	vec clm5 = _mm_sub_ps(clm4, m02);
	clm4 = _mm_add_ps(clm4, m02);
	vec clm7 = _mm_add_ps(clm6, m03);
	clm6 = _mm_sub_ps(clm6, m03);

	clm0 = _mm_shuffle_ps(clm0, clm1, _MM_SHUFFLE(3, 1, 2, 0));
	clm2 = _mm_shuffle_ps(clm2, clm3, _MM_SHUFFLE(3, 1, 2, 0));
	clm4 = _mm_shuffle_ps(clm4, clm5, _MM_SHUFFLE(3, 1, 2, 0));
	clm6 = _mm_shuffle_ps(clm6, clm7, _MM_SHUFFLE(3, 1, 2, 0));
	clm0 = _mm_shuffle_ps(clm0, clm0, _MM_SHUFFLE(3, 1, 2, 0));
	clm2 = _mm_shuffle_ps(clm2, clm2, _MM_SHUFFLE(3, 1, 2, 0));
	clm4 = _mm_shuffle_ps(clm4, clm4, _MM_SHUFFLE(3, 1, 2, 0));
	clm6 = _mm_shuffle_ps(clm6, clm6, _MM_SHUFFLE(3, 1, 2, 0));

	float detf = pm_Dot4D(clm0, tr.v[0]);
	if (determinant)
		*determinant = detf;

	vec det = _mm_set1_ps(1/detf);

	mat r;
	r.v[0] = _mm_mul_ps(det, clm0);
	r.v[1] = _mm_mul_ps(det, clm2);
	r.v[2] = _mm_mul_ps(det, clm4);
	r.v[3] = _mm_mul_ps(det, clm6);
	return r;
#else
	float a0 = m.v[0][0] * m.v[1][1] - m.v[0][1] * m.v[1][0];
	float a1 = m.v[0][0] * m.v[1][2] - m.v[0][2] * m.v[1][0];
	float a2 = m.v[0][0] * m.v[1][3] - m.v[0][3] * m.v[1][0];
	float a3 = m.v[0][1] * m.v[1][2] - m.v[0][2] * m.v[1][1];
	float a4 = m.v[0][1] * m.v[1][3] - m.v[0][3] * m.v[1][1];
	float a5 = m.v[0][2] * m.v[1][3] - m.v[0][3] * m.v[1][2];
	float b0 = m.v[2][0] * m.v[3][1] - m.v[2][1] * m.v[3][0];
	float b1 = m.v[2][0] * m.v[3][2] - m.v[2][2] * m.v[3][0];
	float b2 = m.v[2][0] * m.v[3][3] - m.v[2][3] * m.v[3][0];
	float b3 = m.v[2][1] * m.v[3][2] - m.v[2][2] * m.v[3][1];
	float b4 = m.v[2][1] * m.v[3][3] - m.v[2][3] * m.v[3][1];
	float b5 = m.v[2][2] * m.v[3][3] - m.v[2][3] * m.v[3][2];

	float det = a0*b5 - a1*b4 + a2*b3 + a3*b2 - a4*b1 + a5*b0;

	if (determinant)
		*determinant = det;

	mat r;

	r.v[0][0] = +m.v[1][1] * b5 - m.v[1][2] * b4 + m.v[1][3] * b3;
	r.v[1][0] = -m.v[1][0] * b5 + m.v[1][2] * b2 - m.v[1][3] * b1;
	r.v[2][0] = +m.v[1][0] * b4 - m.v[1][1] * b2 + m.v[1][3] * b0;
	r.v[3][0] = -m.v[1][0] * b3 + m.v[1][1] * b1 - m.v[1][2] * b0;
	r.v[0][1] = -m.v[0][1] * b5 + m.v[0][2] * b4 - m.v[0][3] * b3;
	r.v[1][1] = +m.v[0][0] * b5 - m.v[0][2] * b2 + m.v[0][3] * b1;
	r.v[2][1] = -m.v[0][0] * b4 + m.v[0][1] * b2 - m.v[0][3] * b0;
	r.v[3][1] = +m.v[0][0] * b3 - m.v[0][1] * b1 + m.v[0][2] * b0;
	r.v[0][2] = +m.v[3][1] * a5 - m.v[3][2] * a4 + m.v[3][3] * a3;
	r.v[1][2] = -m.v[3][0] * a5 + m.v[3][2] * a2 - m.v[3][3] * a1;
	r.v[2][2] = +m.v[3][0] * a4 - m.v[3][1] * a2 + m.v[3][3] * a0;
	r.v[3][2] = -m.v[3][0] * a3 + m.v[3][1] * a1 - m.v[3][2] * a0;
	r.v[0][3] = -m.v[2][1] * a5 + m.v[2][2] * a4 - m.v[2][3] * a3;
	r.v[1][3] = +m.v[2][0] * a5 - m.v[2][2] * a2 + m.v[2][3] * a1;
	r.v[2][3] = -m.v[2][0] * a4 + m.v[2][1] * a2 - m.v[2][3] * a0;
	r.v[3][3] = +m.v[2][0] * a3 - m.v[2][1] * a1 + m.v[2][2] * a0;

	det = 1.0f / det;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			r.v[i][j] *= det;
		}
	}

	return r;
#endif
}

float PM_MATH_INLINE pm_Determinant2D(const mat& m)
{
	return m[0][0]*m[1][1] - m[0][1]*m[1][0];
}

// Use smart cross and dots to improve performance!
float PM_MATH_INLINE pm_Determinant3D(const mat& m)
{
	return m[0][0]*m[1][1]*m[2][2] 
		+ m[0][1]*m[1][2]*m[2][0] 
		+ m[0][2]*m[1][0]*m[2][1] 
		- m[0][2]*m[1][1]*m[2][0] 
		- m[0][1]*m[1][0]*m[2][2] 
		- m[0][0]*m[1][2]*m[2][1];
}

float PM_MATH_INLINE pm_Determinant4D(const mat& m)
{
	const mat A = pm_Set2D(m[0][0],m[0][1], m[1][0], m[1][1]);
	const mat B = pm_Set2D(m[0][2],m[0][3], m[1][2], m[1][3]);
	const mat C = pm_Set2D(m[2][0],m[2][1], m[3][0], m[3][1]);
	const mat D = pm_Set2D(m[2][2],m[2][3], m[3][2], m[3][3]);

	float detD;
	const float t = pm_Determinant2D(pm_Subtract(A, pm_Multiply(B, pm_Multiply(pm_Inverse2D(D, &detD), C)))); 
	return t * detD;
}

//Right handled
mat PM_MATH_INLINE pm_Perspective(float width, float height, float n, float f)
{
	float near2 = n + n;
	float range = f / (n - f);

	mat r;
	r.v[0] = pm_Set(near2 / width, 0.0f, 0.0f, 0.0f);
	r.v[1] = pm_Set(0.0f, near2 / height, 0.0f, 0.0f);
	r.v[2] = pm_Set(0.0f, 0.0f, range, -1.0f);
	r.v[3] = pm_Set(0.0f, 0.0f, range * n, 0.0f);

	return r;
}

mat PM_MATH_INLINE pm_Orthographic(float width, float height, float n, float f)
{
	mat r;
	r.v[0] = pm_Set(-2.0f / width, 0.0f, 0.0f, -1.0f);
	r.v[1] = pm_Set(0.0f, 2.0f / height, 0.0f, 1.0f);
	r.v[2] = pm_Set(0.0f, 0.0f, -2.0f / (f - n), -(f + n) / (f - n));
	r.v[3] = pm_Set(0.0f, 0.0f, 0.0f, 1.0f);

	return r;
}

mat PM_MATH_INLINE pm_Orthographic2D(float width, float height)
{
	mat r;
	r.v[0] = pm_Set(2.0f / width, 0.0f, 0.0f, -1.0f);
	r.v[1] = pm_Set(0.0f, -2.0f / height, 0.0f, 1.0f);
	r.v[2] = pm_Set(0.0f, 0.0f, -1.0f, 0.0f);
	r.v[3] = pm_Set(0.0f, 0.0f, 0.0f, 1.0f);

	return r;
}