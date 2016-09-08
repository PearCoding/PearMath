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
# error MathFrame.h should only be included from Math.h
#endif

//TODO: Transpose the used matrix in this frame class
vec3 PM_MATH_INLINE pm_GetXAxis(const frame& f)
{
	return pm_Cross3D(f.Up, f.Forward);
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
	r.Origin = pm_Zero();
	r.Up = pm_Set(0, 1, 0);
	r.Forward = pm_Set(0, 0, 1);
	return r;
}

frame PM_MATH_INLINE pm_Set(const vec& origin, const vec& up, const vec& forward)
{
	frame r;
	r.Origin = origin;
	r.Up = up;
	r.Forward = forward;
	return r;
}

frame PM_MATH_INLINE pm_IsEqualv(const frame& f1, const frame& f2)
{
	frame r;
	r.Origin = pm_IsEqualv(f1.Origin, f2.Origin);
	r.Up = pm_IsEqualv(f1.Up, f2.Up);
	r.Forward = pm_IsEqualv(f1.Forward, f2.Forward);

	return r;
}

bool PM_MATH_INLINE pm_IsEqual(const frame& f1, const frame& f2)
{
	return pm_IsEqual(f1.Origin, f2.Origin) && pm_IsEqual(f1.Up, f2.Up) && pm_IsEqual(f1.Forward, f2.Forward);
}

frame PM_MATH_INLINE pm_IsNearlyEqualv(const frame& f1, const frame& f2, const frame& delta)
{
	frame r;
	r.Origin = pm_IsNearlyEqualv(f1.Origin, f2.Origin, delta.Origin);
	r.Up = pm_IsNearlyEqualv(f1.Up, f2.Up, delta.Up);
	r.Forward = pm_IsNearlyEqualv(f1.Forward, f2.Forward, delta.Forward);

	return r;
}

frame PM_MATH_INLINE pm_IsNearlyEqualv(const frame& f1, const frame& f2, float delta)
{
	frame r;
	r.Origin = pm_IsNearlyEqualv(f1.Origin, f2.Origin, pm_FillVector(delta));
	r.Up = pm_IsNearlyEqualv(f1.Up, f2.Up, pm_FillVector(delta));
	r.Forward = pm_IsNearlyEqualv(f1.Forward, f2.Forward, pm_FillVector(delta));

	return r;
}

bool PM_MATH_INLINE pm_IsNearlyEqual(const frame& f1, const frame& f2, const frame& delta)
{
	return pm_IsNearlyEqual(f1.Origin, f2.Origin, delta.Origin) && pm_IsNearlyEqual(f1.Up, f2.Up, delta.Up) && pm_IsNearlyEqual(f1.Forward, f2.Forward, delta.Forward);
}

bool PM_MATH_INLINE pm_IsNearlyEqual(const frame& f1, const frame& f2, float delta)
{
	return pm_IsNearlyEqual(f1.Origin, f2.Origin, pm_FillVector(delta)) && pm_IsNearlyEqual(f1.Up, f2.Up, pm_FillVector(delta)) && pm_IsNearlyEqual(f1.Forward, f2.Forward, pm_FillVector(delta));
}

frame PM_MATH_INLINE pm_IsNotEqualv(const frame& f1, const frame& f2)
{
	frame r;
	r.Origin = pm_IsNotEqualv(f1.Origin, f2.Origin);
	r.Up = pm_IsNotEqualv(f1.Up, f2.Up);
	r.Forward = pm_IsNotEqualv(f1.Forward, f2.Forward);

	return r;
}

bool PM_MATH_INLINE pm_IsNotEqual(const frame& f1, const frame& f2)
{
	return pm_IsNotEqual(f1.Origin, f2.Origin) || pm_IsNotEqual(f1.Up, f2.Up) || pm_IsNotEqual(f1.Forward, f2.Forward);
}

frame PM_MATH_INLINE pm_Normalize(const frame& f)
{
	vec lX = pm_Cross3D(f.Up, f.Forward);

	frame r;
	r.Forward = pm_Normalize3D(pm_Cross3D(lX, f.Up));
	r.Up = pm_Normalize3D(f.Up);
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
	r.Origin = pm_Add(f.Origin, pm_Scale(pm_Cross3D(f.Up, f.Forward), s));
	return r;
}

frame PM_MATH_INLINE pm_MoveLeft(const frame& f, float s)
{
	frame r = f;
	r.Origin = pm_Subtract(f.Origin, pm_Scale(pm_Cross3D(f.Up, f.Forward), s));
	return r;
}

mat PM_MATH_INLINE pm_ToMatrix(const frame& f, bool rotationOnly)
{
	return pm_Set4D(pm_SetW(pm_Cross3D(f.Up, f.Forward), rotationOnly ? 0.0f : pm_GetX(f.Origin)),
		pm_SetW(f.Up, rotationOnly ? 0.0f : pm_GetY(f.Origin)),
		pm_SetW(f.Forward, rotationOnly ? 0.0f : pm_GetZ(f.Origin)),
		pm_Set(0.0f, 0.0f, 0.0f, 1.0));
}

mat PM_MATH_INLINE pm_ToCameraMatrix(const frame& f, bool rotationOnly)
{
	return pm_Set4D(pm_SetW(pm_Cross3D(f.Up, pm_Negate(f.Forward)), rotationOnly ? 0.0f : -pm_GetX(f.Origin)),
		pm_SetW(f.Up, rotationOnly ? 0.0f : -pm_GetY(f.Origin)),
		pm_SetW(pm_Negate(f.Forward), rotationOnly ? 0.0f : -pm_GetZ(f.Origin)),
		pm_Set(0.0f, 0.0f, 0.0f, 1.0));
}

frame PM_MATH_INLINE pm_RotateLocalX(const frame& f, float angle)
{
	vec lX = pm_Cross3D(f.Up, f.Forward);
	mat t = pm_Rotation(lX, angle);

	frame r = f;
	r.Up = pm_Set(t[0][0] * pm_GetX(f.Up) + t[1][0] * pm_GetY(f.Up) + t[2][0] * pm_GetZ(f.Up),
		t[0][1] * pm_GetX(f.Up) + t[1][1] * pm_GetY(f.Up) + t[2][1] * pm_GetZ(f.Up),
		t[0][2] * pm_GetX(f.Up) + t[1][2] * pm_GetY(f.Up) + t[2][2] * pm_GetZ(f.Up), pm_GetW(f.Up));

	r.Forward = pm_Set(t[0][0] * pm_GetX(f.Forward) + t[1][0] * pm_GetY(f.Forward) + t[2][0] * pm_GetZ(f.Forward),
		t[0][1] * pm_GetX(f.Forward) + t[1][1] * pm_GetY(f.Forward) + t[2][1] * pm_GetZ(f.Forward),
		t[0][2] * pm_GetX(f.Forward) + t[1][2] * pm_GetY(f.Forward) + t[2][2] * pm_GetZ(f.Forward), pm_GetW(f.Forward));
	return r;
}

frame PM_MATH_INLINE pm_RotateLocalY(const frame& f, float angle)
{
	mat t = pm_Rotation(f.Up, angle);

	frame r = f;
	r.Forward = pm_Set(t[0][0] * pm_GetX(f.Forward) + t[1][0] * pm_GetY(f.Forward) + t[2][0] * pm_GetZ(f.Forward),
		t[0][0] * pm_GetX(f.Forward) + t[1][1] * pm_GetY(f.Forward) + t[2][1] * pm_GetZ(f.Forward),
		t[0][0] * pm_GetX(f.Forward) + t[1][2] * pm_GetY(f.Forward) + t[2][2] * pm_GetZ(f.Forward), pm_GetW(f.Forward));
	return r;
}

frame PM_MATH_INLINE pm_RotateLocalZ(const frame& f, float angle)
{
	mat t = pm_Rotation(f.Forward, angle);

	frame r = f;
	r.Up = pm_Set(t[0][0] * pm_GetX(f.Up) + t[1][0] * pm_GetY(f.Up) + t[2][0] * pm_GetZ(f.Up),
		t[0][1] * pm_GetX(f.Up) + t[1][1] * pm_GetY(f.Up) + t[2][1] * pm_GetZ(f.Up),
		t[0][2] * pm_GetX(f.Up) + t[1][2] * pm_GetY(f.Up) + t[2][2] * pm_GetZ(f.Up), pm_GetW(f.Up));
	return r;
}

frame PM_MATH_INLINE pm_RotateWorld(const frame& f, float angle, const vec3& axis)
{
	mat t = pm_Rotation(axis, angle);

	frame r = f;
	r.Up = pm_Set(t[0][0] * pm_GetX(f.Up) + t[1][0] * pm_GetY(f.Up) + t[2][0] * pm_GetZ(f.Up),
		t[0][1] * pm_GetX(f.Up) + t[1][1] * pm_GetY(f.Up) + t[2][1] * pm_GetZ(f.Up),
		t[0][2] * pm_GetX(f.Up) + t[1][2] * pm_GetY(f.Up) + t[2][2] * pm_GetZ(f.Up), pm_GetW(f.Up));

	r.Forward = pm_Set(t[0][0] * pm_GetX(f.Forward) + t[1][0] * pm_GetY(f.Forward) + t[2][0] * pm_GetZ(f.Forward),
		t[0][1] * pm_GetX(f.Forward) + t[1][1] * pm_GetY(f.Forward) + t[2][1] * pm_GetZ(f.Forward),
		t[0][2] * pm_GetX(f.Forward) + t[1][2] * pm_GetY(f.Forward) + t[2][2] * pm_GetZ(f.Forward), pm_GetW(f.Forward));

	return r;
}

frame PM_MATH_INLINE pm_RotateLocal(const frame& f, float angle, const vec3& axis)
{
	return pm_RotateWorld(f, angle, pm_LocalToWorld(f, axis, true));
}

vec3 PM_MATH_INLINE pm_LocalToWorld(const frame& f, const vec3& v, bool rotationOnly)
{
	mat t = pm_ToMatrix(f, true);

	vec3 r;
	r = pm_Set(t[0][0] * pm_GetX(v) + t[1][0] * pm_GetY(v) + t[2][0] * pm_GetZ(v),
		t[0][1] * pm_GetX(v) + t[1][1] * pm_GetY(v) + t[2][1] * pm_GetZ(v),
		t[0][2] * pm_GetX(v) + t[1][2] * pm_GetY(v) + t[2][2] * pm_GetZ(v), pm_GetW(v));

	if (!rotationOnly)
		r = pm_Add(r, f.Origin);

	return r;
}

vec3 PM_MATH_INLINE pm_WorldToLocal(const frame& f, const vec3& v)
{
	mat t = pm_ToMatrix(f, true);
	t = pm_Inverse4D(t);

	vec3 r = pm_Subtract(v, f.Origin);
	r = pm_Set(t[0][0] * pm_GetX(r) + t[1][0] * pm_GetY(r) + t[2][0] * pm_GetZ(r),
		t[0][1] * pm_GetX(r) + t[1][1] * pm_GetY(r) + t[2][1] * pm_GetZ(r),
		t[0][2] * pm_GetX(r) + t[1][2] * pm_GetY(r) + t[2][2] * pm_GetZ(r), pm_GetW(r));

	return r;
}

vec3 PM_MATH_INLINE pm_TransformPoint(const frame& f, const vec3& off)
{
	mat t = pm_ToMatrix(f);

	return pm_Set(t[0][0] * pm_GetX(off) + t[1][0] * pm_GetY(off) + t[2][0] * pm_GetZ(off),
		t[0][1] * pm_GetX(off) + t[1][1] * pm_GetY(off) + t[2][1] * pm_GetZ(off),
		t[0][2] * pm_GetX(off) + t[1][2] * pm_GetY(off) + t[2][2] * pm_GetZ(off), pm_GetW(off));
}

vec3 PM_MATH_INLINE pm_Rotate(const frame& f, const vec3& v)
{
	mat t = pm_ToMatrix(f, true);

	return pm_Set(t[0][0] * pm_GetX(v) + t[1][0] * pm_GetY(v) + t[2][0] * pm_GetZ(v),
		t[0][1] * pm_GetX(v) + t[1][1] * pm_GetY(v) + t[2][1] * pm_GetZ(v),
		t[0][2] * pm_GetX(v) + t[1][2] * pm_GetY(v) + t[2][2] * pm_GetZ(v), pm_GetW(v));
}