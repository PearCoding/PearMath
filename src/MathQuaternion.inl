/*
* Copyright(c) 2014, �mercan Yazici <pearcoding AT gmail.com>
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
# error MathQuaternion.h should only be included from Math.h
#endif

quat PM_MATH_INLINE pm_IdentityQuat()
{
#ifdef PM_USE_SIMD
	return _mm_set_ps(1, 0, 0, 0);
#else
	quat r;
	r.v[0] = 0;
	r.v[1] = 0;
	r.v[2] = 0;
	r.v[3] = 1;
	return r;
#endif
}

quat PM_MATH_INLINE pm_MultiplyQuat(const quat& q1, const quat& q2)
{
#ifdef PM_USE_SIMD
	const vec mask = _mm_setr_ps(1.0, 1.0, 1.0, -1.0);
	const vec flip1 = _mm_mul_ps(
		_mm_mul_ps(
		_mm_shuffle_ps(q1, q1, _MM_SHUFFLE(2, 0, 2, 1)),
		_mm_shuffle_ps(q2, q2, _MM_SHUFFLE(2, 1, 0, 2))),
		mask);
	const vec flip2 = _mm_mul_ps(
		_mm_mul_ps(
		_mm_shuffle_ps(q1, q1, _MM_SHUFFLE(1, 3, 3, 3)),
		_mm_shuffle_ps(q2, q2, _MM_SHUFFLE(1, 2, 1, 0))),
		mask);
	return _mm_add_ps(
		_mm_sub_ps(
		_mm_mul_ps(q1,
		_mm_shuffle_ps(q2, q2, _MM_SHUFFLE(3, 3, 3, 3))),
		_mm_mul_ps(
		_mm_shuffle_ps(q1, q1, _MM_SHUFFLE(0, 1, 0, 2)),
		_mm_shuffle_ps(q2, q2, _MM_SHUFFLE(0, 0, 2, 1)))),
		_mm_add_ps(flip1, flip2));
#else
	quat r;
	r.v[0] = q1.v[3] * q2.v[0] + q1.v[0] * q2.v[3] + q1.v[1] * q2.v[2] - q1.v[2] * q2.v[1];
	r.v[1] = q1.v[3] * q2.v[1] + q1.v[1] * q2.v[3] + q1.v[2] * q2.v[0] - q1.v[0] * q2.v[2];
	r.v[2] = q1.v[3] * q2.v[2] + q1.v[2] * q2.v[3] + q1.v[0] * q2.v[1] - q1.v[1] * q2.v[0];
	r.v[3] = q1.v[3] * q2.v[3] - q1.v[0] * q2.v[0] - q1.v[1] * q2.v[1] - q1.v[2] * q2.v[2];

	return r;
#endif
}

quat PM_MATH_INLINE pm_ConjugateQuat(const quat& q)
{
#ifdef PM_USE_SIMD
	return _mm_mul_ps(q, _mm_setr_ps(-1, -1, -1, 1));
#else
	quat r;
	r.v[0] = -q.v[0];
	r.v[1] = -q.v[1];
	r.v[2] = -q.v[2];
	r.v[3] = q.v[3];
	return r;
#endif
}

quat PM_MATH_INLINE pm_InverseQuat(const quat& q)
{
#ifdef PM_USE_SIMD
	quat r = _mm_mul_ps(q, _mm_setr_ps(-1, -1, -1, 1));
	vec s = _mm_mul_ps(q, q);
	s = _mm_rcp_ps(_mm_add_ps(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_add_ps(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
		_mm_add_ps(_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(3, 3, 3, 3))))));
	return _mm_mul_ps(r, s);
#else
	quat r = pm_ConjugateQuat(q);
	return pm_Divide(r, pm_FillVector(pm_MagnitudeSqr4D(r)));
#endif
}

quat PM_MATH_INLINE pm_SLerpQuat(const quat& q1, const quat& q2, const vec& t);

quat PM_MATH_INLINE pm_RotationQuatRollPitchYaw(float roll, float pitch, float yaw)
{
	
}

quat PM_MATH_INLINE pm_RotationQuatRollPitchYaw(const vec3& angles)
{
	return pm_RotationQuatRollPitchYaw(PM::pm_GetX(angles), PM::pm_GetY(angles), PM::pm_GetZ(angles));
}

quat PM_MATH_INLINE pm_RotationAxis(const quat& axis, float angle)
{
	//quat nAxis = pm_Normalize3D(axis);
#ifdef PM_USE_SIMD
	vec s = _mm_set1_ps(0.5f*angle);
	vec vS;
	vec vC;
	pm_SinCos(s, vS, vC);
	s = vS;
	((float *)&(s))[3] = ((float *)&(vC))[3];
	return _mm_mul_ps(axis, s);
#else
	vec s;
	s.v[0] = s.v[1] = s.v[2] = sinf(0.5f*angle);
	s.v[3] = cosf(0.5f*angle);

	vec t;
	t[0] = axis[0];
	t[1] = axis[1];
	t[2] = axis[2];
	t[3] = 0;
	return pm_Multiply(t, s);
#endif
}

quat PM_MATH_INLINE pm_RotationMatrix(const mat& m);