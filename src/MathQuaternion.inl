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
# error MathQuaternion.h should only be included from Math.h
#endif

quat PM_MATH_INLINE pm_IdentityQuat()
{
	return pm_Set(0, 0, 0, 1);
}

quat PM_MATH_INLINE pm_MultiplyQuat(const quat& q1, const quat& q2)
{
#ifdef PM_WITH_SIMD
	static const __m128 mask = _mm_setr_ps(1.0, 1.0, 1.0, -1.0);
	const __m128 flip1 = _mm_mul_ps(
		_mm_mul_ps(
		_mm_shuffle_ps(q1._vec, q1._vec, _MM_SHUFFLE(2, 0, 2, 1)),
		_mm_shuffle_ps(q2._vec, q2._vec, _MM_SHUFFLE(2, 1, 0, 2))),
		mask);
	const __m128 flip2 = _mm_mul_ps(
		_mm_mul_ps(
		_mm_shuffle_ps(q1._vec, q1._vec, _MM_SHUFFLE(1, 3, 3, 3)),
		_mm_shuffle_ps(q2._vec, q2._vec, _MM_SHUFFLE(1, 2, 1, 0))),
		mask);
	return _mm_add_ps(
		_mm_sub_ps(
		_mm_mul_ps(q1._vec,
		_mm_shuffle_ps(q2._vec, q2._vec, _MM_SHUFFLE(3, 3, 3, 3))),
		_mm_mul_ps(
		_mm_shuffle_ps(q1._vec, q1._vec, _MM_SHUFFLE(0, 1, 0, 2)),
		_mm_shuffle_ps(q2._vec, q2._vec, _MM_SHUFFLE(0, 0, 2, 1)))),
		_mm_add_ps(flip1, flip2));
#else
	quat r;
	r._vec[0] = q1._vec[3] * q2._vec[0] + q1._vec[0] * q2._vec[3] + q1._vec[1] * q2._vec[2] - q1._vec[2] * q2._vec[1];
	r._vec[1] = q1._vec[3] * q2._vec[1] + q1._vec[1] * q2._vec[3] + q1._vec[2] * q2._vec[0] - q1._vec[0] * q2._vec[2];
	r._vec[2] = q1._vec[3] * q2._vec[2] + q1._vec[2] * q2._vec[3] + q1._vec[0] * q2._vec[1] - q1._vec[1] * q2._vec[0];
	r._vec[3] = q1._vec[3] * q2._vec[3] - q1._vec[0] * q2._vec[0] - q1._vec[1] * q2._vec[1] - q1._vec[2] * q2._vec[2];

	return r;
#endif
}

quat PM_MATH_INLINE pm_ConjugateQuat(const quat& q)
{
#ifdef PM_WITH_SIMD
	return _mm_mul_ps(q._vec, _mm_setr_ps(-1, -1, -1, 1));
#else
	quat r;
	r._vec[0] = -q._vec[0];
	r._vec[1] = -q._vec[1];
	r._vec[2] = -q._vec[2];
	r._vec[3] = q._vec[3];
	return r;
#endif
}

quat PM_MATH_INLINE pm_InverseQuat(const quat& q)
{
#ifdef PM_WITH_SIMD
	const __m128 r = _mm_mul_ps(q._vec, _mm_setr_ps(-1, -1, -1, 1));
	__m128 s = _mm_mul_ps(q._vec, q._vec);
	s = _mm_rcp_ps(_mm_add_ps(_mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 0, 0)),
		_mm_add_ps(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1)),
		_mm_add_ps(_mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 2, 2, 2)),
		_mm_shuffle_ps(s, s, _MM_SHUFFLE(3, 3, 3, 3))))));
	return _mm_mul_ps(r, s);
#else
	quat r = pm_ConjugateQuat(q);
	return pm_Divide(r, pm_FillVector4D(pm_MagnitudeSqr(r)));
#endif
}

// Better and faster approach?
// TODO: Test it!
quat PM_MATH_INLINE pm_SLerpQuat(const quat& q1, const quat& q2, const vec4& t)
{
	float theta = pm_Dot(q1, q2);

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
	vec4 ratioA = pm_Scale(pm_Sin(pm_Scale(pm_Subtract(pm_One4D(), t), halfTheta)), rsinHalfTheta);
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
	vec4 p = pm_Multiply(rot, rot);
	return pm_Set(
			std::atan2(2*(pm_GetW(rot)*pm_GetX(rot) + pm_GetY(rot)*pm_GetZ(rot)), 1 - 2*(pm_GetX(p) + pm_GetY(p))),
			std::asin(2*(pm_GetW(rot)*pm_GetY(rot) - pm_GetX(rot)*pm_GetZ(rot))),
			std::atan2(2*(pm_GetW(rot)*pm_GetZ(rot) + pm_GetX(rot)*pm_GetY(rot)), 1 - 2*(pm_GetY(p) + pm_GetZ(p))));
}

quat PM_MATH_INLINE pm_RotationAxis(const vec3& axis, float angle)
{
	vec3 cosa;
	vec3 sina;
	pm_SinCos(pm_FillVector3D(0.5f*angle), sina, cosa);
	vec4 r = pm_ExtendTo4D(pm_Multiply(axis, sina));
	return pm_Normalize(pm_SetW(r, pm_GetW(cosa)));
}

quat PM_MATH_INLINE pm_RotationMatrixNormalized(const mat4& m)
{
	const float w = pm_SafeSqrt<float>(1 + pm_GetIndex(m,0,0) + pm_GetIndex(m,1,1) + pm_GetIndex(m,2,2) ) / 2;
	const float x = pm_SafeSqrt<float>(1 + pm_GetIndex(m,0,0) - pm_GetIndex(m,1,1) - pm_GetIndex(m,2,2) ) / 2;
	const float y = pm_SafeSqrt<float>(1 - pm_GetIndex(m,0,0) + pm_GetIndex(m,1,1) - pm_GetIndex(m,2,2) ) / 2;
	const float z = pm_SafeSqrt<float>(1 - pm_GetIndex(m,0,0) - pm_GetIndex(m,1,1) + pm_GetIndex(m,2,2) ) / 2;

	return pm_Set(std::copysignf(x, pm_GetIndex(m,2,1) - pm_GetIndex(m,1,2) ),
		std::copysignf(y, pm_GetIndex(m,0,2) - pm_GetIndex(m,2,0) ),
		std::copysignf(z, pm_GetIndex(m,1,0) - pm_GetIndex(m,0,1) ),
		w);
}

quat PM_MATH_INLINE pm_RotationMatrix(const mat4& m)
{
	const float absDet = std::pow(pm_Determinant(m), 1/3.0f);
	const float w = pm_SafeSqrt<float>(absDet + pm_GetIndex(m,0,0) + pm_GetIndex(m,1,1) + pm_GetIndex(m,2,2) ) / 2;
	const float x = pm_SafeSqrt<float>(absDet + pm_GetIndex(m,0,0) - pm_GetIndex(m,1,1) - pm_GetIndex(m,2,2) ) / 2;
	const float y = pm_SafeSqrt<float>(absDet - pm_GetIndex(m,0,0) + pm_GetIndex(m,1,1) - pm_GetIndex(m,2,2) ) / 2;
	const float z = pm_SafeSqrt<float>(absDet - pm_GetIndex(m,0,0) - pm_GetIndex(m,1,1) + pm_GetIndex(m,2,2) ) / 2;

	return pm_Set(std::copysignf(x, pm_GetIndex(m,2,1) - pm_GetIndex(m,1,2) ),
		std::copysignf(y, pm_GetIndex(m,0,2) - pm_GetIndex(m,2,0) ),
		std::copysignf(z, pm_GetIndex(m,1,0) - pm_GetIndex(m,0,1) ),
		w);
}

vec3 PM_MATH_INLINE pm_RotateWithQuat(const quat& rotation, const vec3& vector)
{
	return pm_ShrinkTo3D(pm_MultiplyQuat(rotation, pm_MultiplyQuat(pm_ExtendTo4D(vector), pm_ConjugateQuat(rotation))));
}

/*
 Will return the closest rotation from 'from' to 'to'.
 @see Stan Melax's article in Game Programming Gems
*/
quat PM_MATH_INLINE pm_RotateFromTo(const vec3& from, const vec3& to)
{
	const float dot = pm_Dot(from, to);
	if (dot >= 1)
	{
		return pm_IdentityQuat();
	}
	else
	{
		if (dot + 1 < PM_EPSILON) // Close to -1
		{
			vec3 axis = pm_Cross(from, pm_Set(1, 0, 0));
			if (pm_MagnitudeSqr(axis) < PM_EPSILON)
				axis = pm_Cross(from, pm_Set(0, 1, 0));

			return pm_ExtendTo4D(axis);
		}
		else
		{
			const float s = std::sqrt((1 + dot) * 2);
			const float is = 1 / s;

			const vec3 cross = pm_Cross(from, to);

			const vec4 r = pm_ExtendTo4D(pm_Scale(cross, is));
			return pm_Normalize(pm_SetW(r, s * 0.5f));
		}
	}
}

/*
Will return the closest rotation from 'from' to 'to'.
Will fallback to rotate with the 'fallback' axis, when both vectors are too close.
*/
quat PM_MATH_INLINE pm_RotateFromTo(const vec3& from, const vec3& to, const vec3& fallback)
{
	const float dot = pm_Dot(from, to);
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
			const float s = std::sqrt((1 + dot) * 2);
			const float is = 1 / s;

			const vec3 cross = pm_Cross(from, to);

			const vec4 r = pm_ExtendTo4D(pm_Scale(cross, is));
			return pm_Normalize(pm_SetW(r, s * 0.5f));
		}
	}
}