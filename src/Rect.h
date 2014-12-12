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
	template<typename T>
	class Rect
	{
	private:
		T mLeft;
		T mRight;
		T mTop;
		T mBottom;

	public:
		Rect() :
			mLeft(T()), mRight(T()), mTop(T()), mBottom(T())
		{
		}

		Rect(const T& left, const T& right, const T& top, const T& bottom) :
			mLeft(left), mRight(right), mTop(top), mBottom(bottom)
		{
		}

		Rect(const Rect<T>& rect) :
			mLeft(rect.mLeft), mRight(rect.mRight), mTop(rect.mTop), mBottom(rect.mBottom)
		{
		}

		inline T left() const
		{
			return mLeft;
		}

		inline void setLeft(const T& val)
		{
			mLeft = val;
		}

		inline T right() const
		{
			return mRight;
		}

		inline void setRight(const T& val)
		{
			mRight = val;
		}

		inline T top() const
		{
			return mTop;
		}

		inline void setTop(const T& val)
		{
			mTop = val;
		}

		inline T bottom() const
		{
			return mBottom;
		}

		inline void setBottom(const T& val)
		{
			mBottom = val;
		}

		inline T width() const
		{
			return std::abs(mRight - mLeft);//Abs?
		}

		inline T height() const
		{
			return std::abs(mBottom - mTop);//Abs?
		}

		inline T area() const
		{
			return width() * height();
		}

		inline PM::vec2 size() const
		{
			return PM::pm_Set(width(), height());
		}
	};
}