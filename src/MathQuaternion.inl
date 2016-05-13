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

// Better and faster approach?
// TODO: Test it!
quat PM_MATH_INLINE pm_SLerpQuat(const quat& q1, const quat& q2, const vec& t)
{
	float theta = PM::pm_Dot4D(q1, q2);

	if (fabsf(theta) > 1)
	{
		return q1;
	}
	
	PM::quat q3 = q2;
	if (theta < 0) {
		q3 = PM::pm_Negate(q2);
		theta = -theta;
	}

	float halfTheta = acosf(theta);
	float sinHalfTheta = sqrtf(1 - theta*theta);
	
	if (fabsf(sinHalfTheta) < 0.001f)
	{
		return PM::pm_Add(PM::pm_Scale(q1, 0.5f), PM::pm_Scale(q3, 0.5f));
	}

	float rsinHalfTheta = 1 / sinHalfTheta;
	PM::vec4 ratioA = PM::pm_Scale(PM::pm_Sin(PM::pm_Scale(PM::pm_Subtract(PM::pm_One(), t), halfTheta)), rsinHalfTheta);
	PM::vec4 ratioB = PM::pm_Scale(PM::pm_Sin(PM::pm_Scale(t, halfTheta)), rsinHalfTheta);

	return PM::pm_MultiplyAdd(q1, ratioA, PM::pm_Multiply(q3, ratioB));
}

quat PM_MATH_INLINE pm_RotationQuatXYZ(float x, float y, float z)
{
	return pm_RotationQuatXYZ(PM::pm_Set(x, y, z));
}

quat PM_MATH_INLINE pm_RotationQuatXYZ(const vec3& angles)
{
	return pm_RotationQuatRollPitchYaw(angles);
}

quat PM_MATH_INLINE pm_RotationQuatRollPitchYaw(float roll, float pitch, float yaw)
{
	return pm_RotationQuatRollPitchYaw(PM::pm_Set(roll, pitch, yaw));
}

quat PM_MATH_INLINE pm_RotationQuatRollPitchYaw(const vec3& angles)
{
	PM::vec3 cosa;
	PM::vec3 sina;

	pm_SinCos(PM::pm_Scale(angles, 0.5f), sina, cosa);

	return PM::pm_Set(PM::pm_GetX(sina)*PM::pm_GetY(cosa)*PM::pm_GetZ(cosa) - PM::pm_GetX(cosa)*PM::pm_GetY(sina)*PM::pm_GetZ(sina),
		PM::pm_GetX(cosa)*PM::pm_GetY(sina)*PM::pm_GetZ(cosa) + PM::pm_GetX(sina)*PM::pm_GetY(cosa)*PM::pm_GetZ(sina),
		PM::pm_GetX(cosa)*PM::pm_GetY(cosa)*PM::pm_GetZ(sina) - PM::pm_GetX(sina)*PM::pm_GetY(sina)*PM::pm_GetZ(cosa),
		PM::pm_GetX(cosa)*PM::pm_GetY(cosa)*PM::pm_GetZ(cosa) + PM::pm_GetX(sina)*PM::pm_GetY(sina)*PM::pm_GetZ(sina));
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

// Really that good?
// TODO: Test it!
quat PM_MATH_INLINE pm_RotationMatrix(const mat& m)
{
	float trace = m.m[0][0] + m.m[1][1] + m.m[2][2];
	if (trace > 0)
	{
		float s = 0.5f / sqrtf(trace + 1.0f);
		return PM::pm_Set((m.m[2][1] - m.m[1][2]) * s,
			(m.m[0][2] - m.m[2][0]) * s,
			(m.m[1][0] - m.m[0][1]) * s,
			0.25f / s);
	}
	else
	{
		if (m.m[0][0] > m.m[1][1] && m.m[0][0] > m.m[2][2])
		{
			float s = 2.0f * sqrtf(1.0f + m.m[0][0] - m.m[1][1] - m.m[2][2]);
			return PM::pm_Set(0.25f * s,
				(m.m[0][1] + m.m[1][0]) / s,
				(m.m[0][2] + m.m[2][0]) / s,
				(m.m[2][1] - m.m[1][2]) / s);
		}
		else if (m.m[1][1] > m.m[2][2])
		{
			float s = 2.0f * sqrtf(1.0f + m.m[1][1] - m.m[0][0] - m.m[2][2]);
			PM::pm_Set((m.m[0][1] + m.m[1][0]) / s,
				0.25f * s,
				(m.m[1][2] + m.m[2][1]) / s,
				(m.m[0][2] - m.m[2][0]) / s);
		}
		else 
		{
			float s = 2.0f * sqrtf(1.0f + m.m[2][2] - m.m[0][0] - m.m[1][1]);
			PM::pm_Set((m.m[0][2] + m.m[2][0]) / s,
				(m.m[1][2] + m.m[2][1]) / s,
				0.25f * s,
				(m.m[1][0] - m.m[0][1]) / s);
		}
	}
}

vec PM_MATH_INLINE pm_RotateWithQuat(const quat& rotation, const vec& vector)
{
	return pm_MultiplyQuat(rotation, pm_MultiplyQuat(pm_SetW(vector,0), pm_ConjugateQuat(rotation)));
}

/*
 Will return the closest rotation from 'from' to 'to'.
 @see Stan Melax's article in Game Programming Gems
*/
quat PM_MATH_INLINE pm_RotateFromTo(const vec3& from, const vec3& to)
{
	float dot = pm_Dot3D(from, to);
	if (dot >= 1)
	{
		return pm_IdentityQuat();
	}
	else
	{
		if (dot + 1 < PM_EPSILON) // Close to -1
		{
			vec axis = pm_Cross3D(from, pm_Set(1, 0, 0));
			if (pm_MagnitudeSqr3D(axis) < PM_EPSILON)
				axis = pm_Cross3D(from, pm_Set(0, 1, 0));

			return PM::pm_SetW(axis, 0);
		}
		else
		{
			float s = std::sqrt((1 + dot) * 2);
			float is = 1 / s;

			vec3 cross = pm_Cross3D(from, to);

			return pm_Normalize4D(pm_SetW(pm_Scale(cross, is), s * 0.5f));
		}

		/*vec3 w = pm_Cross3D(from, to);
		return pm_Normalize4D(pm_SetW(w, 1 + dot));*/
	}
}

/*
Will return the closest rotation from 'from' to 'to'.
Will fallback to rotate with the 'fallback' axis, when both vectors are too close.
*/
quat PM_MATH_INLINE pm_RotateFromTo(const vec3& from, const vec3& to, const vec3& fallback)
{

	float dot = pm_Dot3D(from, to);
	if (dot >= 1)
	{
		return pm_IdentityQuat();
	}
	else
	{
		if (dot + 1 < PM_EPSILON) // Close to -1
		{
			return pm_RotationAxis(fallback, PM_PI_F);
		}
		else
		{
			float s = std::sqrt((1 + dot) * 2);
			float is = 1 / s;

			vec3 cross = pm_Cross3D(from, to);

			return pm_Normalize4D(pm_SetW(pm_Scale(cross, is), s * 0.5f));
		}
	}
}