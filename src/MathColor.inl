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
# error MathColor.h should only be included from Math.h
#endif

color PM_MATH_INLINE pm_SetR(const color& v, float x)
{
	return pm_SetX(v, x);
}

color PM_MATH_INLINE pm_SetG(const color& v, float y)
{
	return pm_SetY(v, y);
}

color PM_MATH_INLINE pm_SetB(const color& v, float z)
{
	return pm_SetZ(v, z);
}

color PM_MATH_INLINE pm_SetA(const color& v, float w)
{
	return pm_SetW(v, w);
}

float PM_MATH_INLINE pm_GetR(const color& v)
{
	return pm_GetX(v);
}

float PM_MATH_INLINE pm_GetG(const color& v)
{
	return pm_GetY(v);
}

float PM_MATH_INLINE pm_GetB(const color& v)
{
	return pm_GetZ(v);
}

float PM_MATH_INLINE pm_GetA(const color& v)
{
	return pm_GetW(v);
}