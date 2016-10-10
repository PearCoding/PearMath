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
	return pm_Set(0, 0, 0, 1);
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
	float theta = pm_Dot4D(q1, q2);

	if (std::abs(theta) > 1)
		return q1;
	
	quat q3 = q2;
	if (theta < 0) {
		q3 = pm_Negate(q2);
		theta = -theta;
	}

	float halfTheta = std::acos(theta);
	float sinHalfTheta = std::sqrt(1 - theta*theta);
	
	if (std::abs(sinHalfTheta) < 0.001f)
		return pm_Add(pm_Scale(q1, 0.5f), pm_Scale(q3, 0.5f));

	float rsinHalfTheta = 1 / sinHalfTheta;
	vec4 ratioA = pm_Scale(pm_Sin(pm_Scale(pm_Subtract(pm_One(), t), halfTheta)), rsinHalfTheta);
	vec4 ratioB = pm_Scale(pm_Sin(pm_Scale(t, halfTheta)), rsinHalfTheta);

	return pm_MultiplyAdd(q1, ratioA, pm_Multiply(q3, ratioB));
}

quat PM_MATH_INLINE pm_RotationQuatFromXYZ(float angle_x, float angle_y, float angle_z)
{
	return pm_RotationQuatFromXYZ(pm_Set(angle_x, angle_y, angle_z));
}

quat PM_MATH_INLINE pm_RotationQuatFromXYZ(const vec3& angles)
{
	vec3 cosa;
	vec3 sina;

	pm_SinCos(PM::pm_Scale(angles, 0.5f), sina, cosa);

	return pm_Set(
		pm_GetX(sina)*pm_GetY(cosa)*pm_GetZ(cosa) - pm_GetX(cosa)*pm_GetY(sina)*pm_GetZ(sina),
		pm_GetX(cosa)*pm_GetY(sina)*pm_GetZ(cosa) + pm_GetX(sina)*pm_GetY(cosa)*pm_GetZ(sina),
		pm_GetX(cosa)*pm_GetY(cosa)*pm_GetZ(sina) - pm_GetX(sina)*pm_GetY(sina)*pm_GetZ(cosa),
		pm_GetX(cosa)*pm_GetY(cosa)*pm_GetZ(cosa) + pm_GetX(sina)*pm_GetY(sina)*pm_GetZ(sina));
}

vec3 PM_MATH_INLINE pm_RotationQuatToXYZ(const quat& rot)
{
	vec p = pm_Multiply(rot, rot);
	return pm_Set(
			std::atan2(2*(pm_GetW(rot)*pm_GetX(rot) + pm_GetY(rot)*pm_GetZ(rot)), 1 - 2*(pm_GetX(p) + pm_GetY(p))),
			std::asin(2*(pm_GetW(rot)*pm_GetY(rot) - pm_GetX(rot)*pm_GetZ(rot))),
			std::atan2(2*(pm_GetW(rot)*pm_GetZ(rot) + pm_GetX(rot)*pm_GetY(rot)), 1 - 2*(pm_GetY(p) + pm_GetZ(p))),
			0);
}

quat PM_MATH_INLINE pm_RotationAxis(const vec& axis, float angle)
{
	vec3 cosa;
	vec3 sina;
	pm_SinCos(pm_FillVector(0.5f*angle), sina, cosa);
	return pm_Normalize4D(pm_SetW(pm_Multiply(axis, sina), pm_GetW(cosa)));
}

quat PM_MATH_INLINE pm_RotationMatrixNormalized(const mat& m)
{
	const float w = pm_SafeSqrt<float>(1 + m[0][0] + m[1][1] + m[2][2] ) / 2;
	const float x = pm_SafeSqrt<float>(1 + m[0][0] - m[1][1] - m[2][2] ) / 2;
	const float y = pm_SafeSqrt<float>(1 - m[0][0] + m[1][1] - m[2][2] ) / 2;
	const float z = pm_SafeSqrt<float>(1 - m[0][0] - m[1][1] + m[2][2] ) / 2;

	return pm_Set(copysignf(x, m[2][1] - m[1][2] ),
		copysignf(y, m[0][2] - m[2][0] ),
		copysignf(z, m[1][0] - m[0][1] ),
		w);
}

quat PM_MATH_INLINE pm_RotationMatrix(const mat& m)
{
	const float absDet = std::pow(pm_Determinant4D(m), 1/3.0f);
	const float w = pm_SafeSqrt<float>(absDet + m[0][0] + m[1][1] + m[2][2] ) / 2;
	const float x = pm_SafeSqrt<float>(absDet + m[0][0] - m[1][1] - m[2][2] ) / 2;
	const float y = pm_SafeSqrt<float>(absDet - m[0][0] + m[1][1] - m[2][2] ) / 2;
	const float z = pm_SafeSqrt<float>(absDet - m[0][0] - m[1][1] + m[2][2] ) / 2;

	return pm_Set(copysignf(x, m[2][1] - m[1][2] ),
		copysignf(y, m[0][2] - m[2][0] ),
		copysignf(z, m[1][0] - m[0][1] ),
		w);
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