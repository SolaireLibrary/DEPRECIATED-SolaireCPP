#ifndef SOLAIRE_GRAPHICS_IMAGE_2D_HPP
#define SOLAIRE_GRAPHICS_IMAGE_2D_HPP

//Copyright 2015 Adam Smith
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.

// Contact :
// Email             : solairelibrary@mail.com
// GitHub repository : https://github.com/SolaireLibrary/SolaireCPP

/*!
	\file Image2D.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 17th September 2015
	Last Modified	: 17th September 2015
*/

#include "..\Core\Init.hpp"
#include "..\Maths\Vector.hpp"
#include "..\Core\BitStream.hpp"
#include "..\Core\Maths.hpp"
#include "..\Memory\Allocator.hpp"
#include "ColourFormat.hpp"

namespace Solaire{

	template<class COLOUR>
	class Image2D {
	public:
		typedef COLOUR Colour;
	private:
		Allocator& mAllocator;
		void* const mData;
		const uint16_t mWidth;
		const uint16_t mHeight;
	private:
		Image2D(const Image2D<COLOUR>&) = delete;
		Image2D(Image2D<COLOUR>&&) = delete;
		Image2D<COLOUR>& operator=(const Image2D<COLOUR>&) = delete;
		Image2D<COLOUR>& operator=(Image2D<COLOUR>&&) = delete;

		static uint32_t CalculateAllocationSize(const uint32_t aWidth, const uint32_t aHeight) {
			return static_cast<uint32_t>(CeilToMultiple<float>(
				static_cast<float>(aWidth * aHeight * Colour::BITS_TOTAL),
				8.f
			) / 8.f);
		}
	public:
		Image2D(Allocator& aAllocator, const uint32_t aWidth, const uint32_t aHeight) :
			mAllocator(aAllocator),
			mData(aAllocator.Allocate(CalculateAllocationSize(aWidth, aHeight))),
			mWidth(aWidth),
			mHeight(aHeight)
		{}

		~Image2D() {
			mAllocator.Deallocate(mData);
		}

		Allocator& GetAllocator() const {
			return mAllocator;
		}

		uint32_t Width() const {
			return mWidth;
		}

		uint32_t Height() const {
			return mHeight;
		}

		uint32_t RawSize() const{
			return CalculateAllocationSize(mWidth, mHeight);
		}

		void* RawPtr() {
			return mData;
		}

		const void* RawPtr() const{
			return mData;
		}

		typename Colour::Vector GetPixel(const uint32_t aX, const uint32_t aY)  const {
			const uint32_t index = RowMajorOrder<uint32_t>::Index(aX, aY, mWidth, mHeight);

			if(Colour::IS_BYTE_ALIGNED) {
				return static_cast<const Colour::Vector*>(mData)[index];
			}else {
				BitStream bitStream(mData);
				bitStream.IncrementBit(index * Colour::BITS_TOTAL);

				Colour::Vector buf;
				if(Colour::BITS_0 > 0) {
					buf[Colour::INDEX_0] = 0;
					bitStream.ReadBits(&buf[Colour::INDEX_0], Colour::BITS_0);
				}

				if(Colour::BITS_1 > 0) {
					buf[Colour::INDEX_1] = 0;
					bitStream.ReadBits(&buf[Colour::INDEX_1], Colour::BITS_1);
				}

				if(Colour::BITS_2 > 0) {
					buf[Colour::INDEX_2] = 0;
					bitStream.ReadBits(&buf[Colour::INDEX_2], Colour::BITS_2);
				}

				if(Colour::BITS_3 > 0) {
					buf[Colour::INDEX_3] = 0;
					bitStream.ReadBits(&buf[Colour::INDEX_3], Colour::BITS_3);
				}

				return buf;
			}
		}

		void SetPixel(const uint32_t aX, const uint32_t aY, typename Colour::Vector aColour) {
			const uint32_t index = RowMajorOrder<uint32_t>::Index(aX, aY, mWidth, mHeight);

			if(Colour::IS_BYTE_ALIGNED) {
				static_cast<Colour::Vector*>(mData)[index] = aColour;
			}else {
				BitStream bitStream(mData);
				bitStream.IncrementBit(index *Colour::BITS_TOTAL);

				if(Colour::BITS_0 > 0)	bitStream.WriteBits(&aColour[Colour::INDEX_0], Colour::BITS_0);
				if(Colour::BITS_1 > 0)	bitStream.WriteBits(&aColour[Colour::INDEX_1], Colour::BITS_1);
				if(Colour::BITS_2 > 0)	bitStream.WriteBits(&aColour[Colour::INDEX_2], Colour::BITS_2);
				if(Colour::BITS_3 > 0)	bitStream.WriteBits(&aColour[Colour::INDEX_3], Colour::BITS_3);
			}
		}
	};
}


#endif
