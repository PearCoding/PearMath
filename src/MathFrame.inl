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
# error MathFrame.h should only be included from Math.h
#endif

vec3 PM_MATH_INLINE pm_GetXAxis(const frame& f)
{
	return pm_Cross(f.Up, f.Forward);
}

vec3 PM_MATH_INLINE pm_GetYAxis(const frame& f)
{
	return f.Up;
}

vec3 PM_MATH_INLINE pm_GetZAxis(const frame& f)
{
	return f.Forward;
}

frame PM_MATH_INLINE pm_IdentityFrame()
{
	frame r;
	r.Origin = pm_Zero3D();
	r.Up = pm_Set(0, 1, 0);
	r.Forward = pm_Set(0, 0, 1);
	return r;
}

frame PM_MATH_INLINE pm_Set(const vec3& origin, const vec3& up, const vec3& forward)
{
	frame r;
	r.Origin = origin;
	r.Up = up;
	r.Forward = forward;
	return r;
}

bool PM_MATH_INLINE pm_IsAllTrue(const frame& f)
{
	return pm_IsAllTrue(f.Origin) && pm_IsAllTrue(f.Up) && pm_IsAllTrue(f.Forward);
}

bool PM_MATH_INLINE pm_IsSomeTrue(const frame& f)
{
	return pm_IsSomeTrue(f.Origin) || pm_IsSomeTrue(f.Up) || pm_IsSomeTrue(f.Forward);
}

bool PM_MATH_INLINE pm_IsNoneTrue(const frame& f)
{
	return pm_IsNoneTrue(f.Origin) && pm_IsNoneTrue(f.Up) && pm_IsNoneTrue(f.Forward);
}

frame PM_MATH_INLINE pm_IsEqual(const frame& f1, const frame& f2)
{
	frame r;
	r.Origin = pm_IsEqual(f1.Origin, f2.Origin);
	r.Up = pm_IsEqual(f1.Up, f2.Up);
	r.Forward = pm_IsEqual(f1.Forward, f2.Forward);

	return r;
}

frame PM_MATH_INLINE pm_IsNearlyEqual(const frame& f1, const frame& f2, const frame& delta)
{
	frame r;
	r.Origin = pm_IsNearlyEqual(f1.Origin, f2.Origin, delta.Origin);
	r.Up = pm_IsNearlyEqual(f1.Up, f2.Up, delta.Up);
	r.Forward = pm_IsNearlyEqual(f1.Forward, f2.Forward, delta.Forward);

	return r;
}

frame PM_MATH_INLINE pm_IsNearlyEqual(const frame& f1, const frame& f2, float delta)
{
	frame r;
	r.Origin = pm_IsNearlyEqual(f1.Origin, f2.Origin, delta);
	r.Up = pm_IsNearlyEqual(f1.Up, f2.Up, delta);
	r.Forward = pm_IsNearlyEqual(f1.Forward, f2.Forward, delta);

	return r;
}

frame PM_MATH_INLINE pm_IsNotEqual(const frame& f1, const frame& f2)
{
	frame r;
	r.Origin = pm_IsNotEqual(f1.Origin, f2.Origin);
	r.Up = pm_IsNotEqual(f1.Up, f2.Up);
	r.Forward = pm_IsNotEqual(f1.Forward, f2.Forward);

	return r;
}

frame PM_MATH_INLINE pm_Normalize(const frame& f)
{
	vec3 lX = pm_Cross(f.Up, f.Forward);

	frame r;
	r.Forward = pm_Normalize(pm_Cross(lX, f.Up));
	r.Up = pm_Normalize(f.Up);
	r.Origin = f.Origin;

	return r;
}

frame PM_MATH_INLINE pm_TranslateWorld(const frame& f, const vec3& v)
{
	frame r = f;
	r.Origin = pm_Add(f.Origin, v);
	return r;
}

frame PM_MATH_INLINE pm_TranslateLocal(const frame& f, const vec3& v)
{
	return pm_MoveRight(pm_MoveUp(pm_MoveForward(f, pm_GetZ(v)), pm_GetY(v)), pm_GetX(v));
}

frame PM_MATH_INLINE pm_MoveForward(const frame& f, float s)
{
	frame r = f;
	r.Origin = pm_Add(f.Origin, pm_Scale(f.Forward, s));
	return r;
}

frame PM_MATH_INLINE pm_MoveBackward(const frame& f, float s)
{
	frame r = f;
	r.Origin = pm_Subtract(f.Origin, pm_Scale(f.Forward, s));
	return r;
}

frame PM_MATH_INLINE pm_MoveUp(const frame& f, float s)
{
	frame r = f;
	r.Origin = pm_Add(f.Origin, pm_Scale(f.Up, s));
	return r;
}

frame PM_MATH_INLINE pm_MoveDown(const frame& f, float s)
{
	frame r = f;
	r.Origin = pm_Subtract(f.Origin, pm_Scale(f.Up, s));
	return r;
}

frame PM_MATH_INLINE pm_MoveRight(const frame& f, float s)
{
	frame r = f;
	r.Origin = pm_Add(f.Origin, pm_Scale(pm_Cross(f.Up, f.Forward), s));
	return r;
}

frame PM_MATH_INLINE pm_MoveLeft(const frame& f, float s)
{
	frame r = f;
	r.Origin = pm_Subtract(f.Origin, pm_Scale(pm_Cross(f.Up, f.Forward), s));
	return r;
}

mat4 PM_MATH_INLINE pm_ToMatrix(const frame& f, bool rotationOnly)
{
	const vec3 n = pm_Cross(f.Up, f.Forward);
	return pm_Create(pm_GetX(n), pm_GetY(n), pm_GetZ(n), rotationOnly ? 0.0f : pm_GetX(f.Origin),
		pm_GetX(f.Up), pm_GetY(f.Up), pm_GetZ(f.Up), rotationOnly ? 0.0f : pm_GetY(f.Origin),
		pm_GetX(f.Forward), pm_GetY(f.Forward), pm_GetZ(f.Forward), rotationOnly ? 0.0f : pm_GetZ(f.Origin),
		0.0f, 0.0f, 0.0f, 1.0f);
}

mat4 PM_MATH_INLINE pm_ToCameraMatrix(const frame& f, bool rotationOnly)
{
	const vec3 n = pm_Cross(f.Up, pm_Negate(f.Forward));
	return pm_Create(pm_GetX(n), pm_GetY(n), pm_GetZ(n), rotationOnly ? 0.0f : -pm_GetX(f.Origin),
		pm_GetX(f.Up), pm_GetY(f.Up), pm_GetZ(f.Up), rotationOnly ? 0.0f : -pm_GetY(f.Origin),
		pm_GetX(f.Forward), pm_GetY(f.Forward), pm_GetZ(f.Forward), rotationOnly ? 0.0f : -pm_GetZ(f.Origin),
		0.0f, 0.0f, 0.0f, 1.0f);
}

frame PM_MATH_INLINE pm_RotateLocalX(const frame& f, float angle)
{
	const vec3 lX = pm_Cross(f.Up, f.Forward);
	const mat4 t = pm_Rotation(lX, angle);

	frame r = f;
	r.Up = pm_Set(pm_GetIndex(t,0,0) * pm_GetX(f.Up) + pm_GetIndex(t,1,0) * pm_GetY(f.Up) + pm_GetIndex(t,2,0) * pm_GetZ(f.Up),
		pm_GetIndex(t,0,1) * pm_GetX(f.Up) + pm_GetIndex(t,1,1) * pm_GetY(f.Up) + pm_GetIndex(t,2,1) * pm_GetZ(f.Up),
		pm_GetIndex(t,0,2) * pm_GetX(f.Up) + pm_GetIndex(t,1,2) * pm_GetY(f.Up) + pm_GetIndex(t,2,2) * pm_GetZ(f.Up));

	r.Forward = pm_Set(pm_GetIndex(t,0,0) * pm_GetX(f.Forward) + pm_GetIndex(t,1,0) * pm_GetY(f.Forward) + pm_GetIndex(t,2,0) * pm_GetZ(f.Forward),
		pm_GetIndex(t,0,1) * pm_GetX(f.Forward) + pm_GetIndex(t,1,1) * pm_GetY(f.Forward) + pm_GetIndex(t,2,1) * pm_GetZ(f.Forward),
		pm_GetIndex(t,0,2) * pm_GetX(f.Forward) + pm_GetIndex(t,1,2) * pm_GetY(f.Forward) + pm_GetIndex(t,2,2) * pm_GetZ(f.Forward));
	return r;
}

frame PM_MATH_INLINE pm_RotateLocalY(const frame& f, float angle)
{
	const mat4 t = pm_Rotation(f.Up, angle);

	frame r = f;
	r.Forward = pm_Set(pm_GetIndex(t,0,0) * pm_GetX(f.Forward) + pm_GetIndex(t,1,0) * pm_GetY(f.Forward) + pm_GetIndex(t,2,0) * pm_GetZ(f.Forward),
		pm_GetIndex(t,0,0) * pm_GetX(f.Forward) + pm_GetIndex(t,1,1) * pm_GetY(f.Forward) + pm_GetIndex(t,2,1) * pm_GetZ(f.Forward),
		pm_GetIndex(t,0,0) * pm_GetX(f.Forward) + pm_GetIndex(t,1,2) * pm_GetY(f.Forward) + pm_GetIndex(t,2,2) * pm_GetZ(f.Forward));
	return r;
}

frame PM_MATH_INLINE pm_RotateLocalZ(const frame& f, float angle)
{
	const mat4 t = pm_Rotation(f.Forward, angle);

	frame r = f;
	r.Up = pm_Set(pm_GetIndex(t,0,0) * pm_GetX(f.Up) + pm_GetIndex(t,1,0) * pm_GetY(f.Up) + pm_GetIndex(t,2,0) * pm_GetZ(f.Up),
		pm_GetIndex(t,0,1) * pm_GetX(f.Up) + pm_GetIndex(t,1,1) * pm_GetY(f.Up) + pm_GetIndex(t,2,1) * pm_GetZ(f.Up),
		pm_GetIndex(t,0,2) * pm_GetX(f.Up) + pm_GetIndex(t,1,2) * pm_GetY(f.Up) + pm_GetIndex(t,2,2) * pm_GetZ(f.Up));
	return r;
}

frame PM_MATH_INLINE pm_RotateWorld(const frame& f, float angle, const vec3& axis)
{
	mat4 t = pm_Rotation(axis, angle);

	frame r = f;
	r.Up = pm_Set(pm_GetIndex(t,0,0) * pm_GetX(f.Up) + pm_GetIndex(t,1,0) * pm_GetY(f.Up) + pm_GetIndex(t,2,0) * pm_GetZ(f.Up),
		pm_GetIndex(t,0,1) * pm_GetX(f.Up) + pm_GetIndex(t,1,1) * pm_GetY(f.Up) + pm_GetIndex(t,2,1) * pm_GetZ(f.Up),
		pm_GetIndex(t,0,2) * pm_GetX(f.Up) + pm_GetIndex(t,1,2) * pm_GetY(f.Up) + pm_GetIndex(t,2,2) * pm_GetZ(f.Up));

	r.Forward = pm_Set(pm_GetIndex(t,0,0) * pm_GetX(f.Forward) + pm_GetIndex(t,1,0) * pm_GetY(f.Forward) + pm_GetIndex(t,2,0) * pm_GetZ(f.Forward),
		pm_GetIndex(t,0,1) * pm_GetX(f.Forward) + pm_GetIndex(t,1,1) * pm_GetY(f.Forward) + pm_GetIndex(t,2,1) * pm_GetZ(f.Forward),
		pm_GetIndex(t,0,2) * pm_GetX(f.Forward) + pm_GetIndex(t,1,2) * pm_GetY(f.Forward) + pm_GetIndex(t,2,2) * pm_GetZ(f.Forward));

	return r;
}

frame PM_MATH_INLINE pm_RotateLocal(const frame& f, float angle, const vec3& axis)
{
	return pm_RotateWorld(f, angle, pm_LocalToWorld(f, axis, true));
}

vec3 PM_MATH_INLINE pm_LocalToWorld(const frame& f, const vec3& v, bool rotationOnly)
{
	const mat4 t = pm_ToMatrix(f, true);

	vec3 r;
	r = pm_Set(pm_GetIndex(t,0,0) * pm_GetX(v) + pm_GetIndex(t,1,0) * pm_GetY(v) + pm_GetIndex(t,2,0) * pm_GetZ(v),
		pm_GetIndex(t,0,1) * pm_GetX(v) + pm_GetIndex(t,1,1) * pm_GetY(v) + pm_GetIndex(t,2,1) * pm_GetZ(v),
		pm_GetIndex(t,0,2) * pm_GetX(v) + pm_GetIndex(t,1,2) * pm_GetY(v) + pm_GetIndex(t,2,2) * pm_GetZ(v));

	if (!rotationOnly)
		r = pm_Add(r, f.Origin);

	return r;
}

vec3 PM_MATH_INLINE pm_WorldToLocal(const frame& f, const vec3& v)
{
	mat4 t = pm_ToMatrix(f, true);
	t = pm_Inverse(t);

	vec3 r = pm_Subtract(v, f.Origin);
	r = pm_Set(pm_GetIndex(t,0,0) * pm_GetX(r) + pm_GetIndex(t,1,0) * pm_GetY(r) + pm_GetIndex(t,2,0) * pm_GetZ(r),
		pm_GetIndex(t,0,1) * pm_GetX(r) + pm_GetIndex(t,1,1) * pm_GetY(r) + pm_GetIndex(t,2,1) * pm_GetZ(r),
		pm_GetIndex(t,0,2) * pm_GetX(r) + pm_GetIndex(t,1,2) * pm_GetY(r) + pm_GetIndex(t,2,2) * pm_GetZ(r));

	return r;
}

vec3 PM_MATH_INLINE pm_TransformPoint(const frame& f, const vec3& off)
{
	const mat4 t = pm_ToMatrix(f);

	return pm_Set(pm_GetIndex(t,0,0) * pm_GetX(off) + pm_GetIndex(t,1,0) * pm_GetY(off) + pm_GetIndex(t,2,0) * pm_GetZ(off),
		pm_GetIndex(t,0,1) * pm_GetX(off) + pm_GetIndex(t,1,1) * pm_GetY(off) + pm_GetIndex(t,2,1) * pm_GetZ(off),
		pm_GetIndex(t,0,2) * pm_GetX(off) + pm_GetIndex(t,1,2) * pm_GetY(off) + pm_GetIndex(t,2,2) * pm_GetZ(off));
}

vec3 PM_MATH_INLINE pm_Rotate(const frame& f, const vec3& v)
{
	const mat4 t = pm_ToMatrix(f, true);

	return pm_Set(pm_GetIndex(t,0,0) * pm_GetX(v) + pm_GetIndex(t,1,0) * pm_GetY(v) + pm_GetIndex(t,2,0) * pm_GetZ(v),
		pm_GetIndex(t,0,1) * pm_GetX(v) + pm_GetIndex(t,1,1) * pm_GetY(v) + pm_GetIndex(t,2,1) * pm_GetZ(v),
		pm_GetIndex(t,0,2) * pm_GetX(v) + pm_GetIndex(t,1,2) * pm_GetY(v) + pm_GetIndex(t,2,2) * pm_GetZ(v));
}