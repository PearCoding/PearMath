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

#pragma once

#include "PearMath.h"

namespace PM
{
	class Region
	{
	public:
		inline Region() :
			mLowerCorner(), mUpperCorner()
		{
		}

		inline Region(const vec3& lowerCorner, const vec3& upperCorner) ://TODO Check if lowerCorner is really lower than upperCorner
			mLowerCorner(lowerCorner), mUpperCorner(upperCorner)
		{
		}

		inline vec3 lowerCorner() const
		{
			return mLowerCorner;
		}

		inline void setLowerCorner(const vec3& corner)
		{
			mLowerCorner = corner;
		}

		inline vec3 upperCorner() const
		{
			return mUpperCorner;
		}

		inline void setUpperCorner(const vec3& corner)
		{
			mUpperCorner = corner;
		}

		inline bool contains(const vec3& point, float boundary = 0) const
		{
			return pm_IsLessOrEqual(point, pm_Subtract(mUpperCorner, pm_FillVector(boundary)))
				&& pm_IsGreaterOrEqual(point, pm_Add(mLowerCorner, pm_FillVector(boundary)));
		}

		inline void cropTo(const Region& region)
		{
			mLowerCorner = pm_Max(mLowerCorner, region.mLowerCorner);
			mUpperCorner = pm_Min(mUpperCorner, region.mUpperCorner);
		}

		inline void shift(const vec3& vec)
		{
			mLowerCorner = pm_Add(mLowerCorner, vec);
			mUpperCorner = pm_Add(mUpperCorner, vec);
		}

		inline void shiftLowerCorner(const vec3& vec)
		{
			mLowerCorner = pm_Add(mLowerCorner, vec);
		}

		inline void shiftUpperCorner(const vec3& vec)
		{
			mUpperCorner = pm_Add(mUpperCorner, vec);
		}

		inline vec3 area() const
		{
			return PM::pm_Abs(PM::pm_Subtract(mUpperCorner, mLowerCorner));
		}

	private:
		vec3 mLowerCorner;
		vec3 mUpperCorner;
	};
}