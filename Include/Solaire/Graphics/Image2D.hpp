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
#include "Colour.hpp"

namespace Solaire{

	template<class COLOUR>
	class Image2D {
	public:
		typedef COLOUR Colour;
		typedef Image2D<RED_BITS, GREEN_BITS, BLUE_BITS, ALPHA_BITS> _Image2D;
	private:
		Allocator& mAllocator;
		void* const mData;
		const uint16_t mWidth;
		const uint16_t mHeight;
	private:
		Image2D(const Image2D&) = delete;
		Image2D(Image2D&&) = delete;
		Image2D& operator=(const Image2D&) = delete;
		Image2D& operator=(Image2D&&) = delete;

		static uint32_t CalculateAllocationSize(const uint32_t aWidth, const uint32_t aHeight) {
			return static_cast<uint32_t>(CeilToMultiple<float>(
				static_cast<float>(aWidth * aHeight * Colour::BITS_TOTAL),
				8.f
			) / 8.f;
		}
	public:
		Image2D(Allocator& aAllocator, const uint32_t aWidth, const uint32_t aHeight) :
			mAllocator(aAllocator),
			mData(aAllocator.Allocate(CalculateAllocationSize(aWidth, aHeight)),
			mWidth(aWidth),
			mHeight(aHeight)
		{}

		~Image2D() {
			mAllocator.Deallocate(mData);
		}

		Allocator& SOLAIRE_EXPORT_CALL GetAllocator() const {
			return mAllocator;
		}

		uint32_t SOLAIRE_EXPORT_CALL Width() const {
			return mWidth;
		}

		uint32_t SOLAIRE_EXPORT_CALL Height() const {
			return mHeight;
		}

		static SOLAIRE_EXPORT_CALL uint32_t RawSize() const{
			return CalculateAllocationSize(mWidth, mHeight);
		}

		void* SOLAIRE_EXPORT_CALL RawPtr() {
			return mData;
		}

		const void* SOLAIRE_EXPORT_CALL ConstRawPtr() const{
			return mData;
		}

		Colour::Vector SOLAIRE_EXPORT_CALL GetPixel(const uint32_t aX, const uint32_t aY)  const {
			const uint32_t index = RowMajorOrder::Index<uint32_t>(aX, aY, mWidth, mHeight);

			enum {
				ELEMENT_BITS = sizeof(Colour::MaxChannel) * 8
				COLOUR_BYTES = Colour::BITS_TOTAL / 8
			};

			if((Colour::BITS_TOTAL & 7) == 0) {
				uint8_t buf[COLOUR_BYTES];
				std::memcpy(buf, static_cast<const uint8_t*>(mData) + (index * COLOUR_BYTES), COLOUR_BYTES);
				return *static_cast<const Colour::Vector*>(buf);
			}else {
				BitStream bitStream(mData);
				bitStream.IncrementBit(index * Colour::BITS_TOTAL);

				Colour::Vector buf;
				if(Colour::BITS_RED > 0)	bitStream.ReadBits(&buf[Colour::INDEX_RED], Colour::BITS_RED);
				if(Colour::BITS_GREEN > 0)	bitStream.ReadBits(&buf[Colour::INDEX_GREEN], Colour::BITS_GREEN);
				if(Colour::BITS_BLUE > 0)	bitStream.ReadBits(&buf[Colour::INDEX_BLUE], Colour::BITS_BLUE);
				if(Colour::BITS_ALPHA > 0)	bitStream.ReadBits(&buf[Colour::INDEX_ALPHA], Colour::BITS_ALPHA);

				if(Colour::BITS_RED > 0)	buf[Colour::INDEX_RED] >>= ELEMENT_BITS - Colour::BITS_RED;
				if(Colour::BITS_GREEN > 0)	buf[Colour::INDEX_GREEN] >>= ELEMENT_BITS - Colour::BITS_GREEN;
				if(Colour::BITS_BLUE > 0)	buf[Colour::INDEX_BLUE] >>= ELEMENT_BITS - Colour::BITS_BLUE;
				if(Colour::BITS_ALPHA > 0)	buf[Colour::INDEX_ALPHA] >>= ELEMENT_BITS - Colour::BITS_ALPHA;

				return buf;
			}
		}

		void SOLAIRE_EXPORT_CALL SetPixel(const uint32_t aX, const uint32_t aY, Colour::Vector aColour) {
			const uint32_t index = RowMajorOrder::Index<uint32_t>(aX, aY, mWidth, mHeight);

			enum {
				ELEMENT_BITS = sizeof(Colour::MaxChannel) * 8
				COLOUR_BYTES = Colour::BITS_TOTAL / 8
			};

			if((Colour::BITS_TOTAL & 7) == 0) {
				std::memcpy(static_cast<uint8_t*>(mData) + (index * COLOUR_BYTES), aColour.AsPointer(), COLOUR_BYTES);
			}else {
				BitStream bitStream(mData);
				bitStream.IncrementBit(index *Colour::BITS_TOTAL);

				if(Colour::BITS_RED > 0)	aColour[Colour::INDEX_RED] <<= ELEMENT_BITS - Colour::BITS_RED;
				if(Colour::BITS_GREEN > 0)	aColour[Colour::INDEX_GREEN] <<= ELEMENT_BITS - Colour::BITS_GREEN;
				if(Colour::BITS_BLUE > 0)	aColour[Colour::INDEX_BLUE] <<= ELEMENT_BITS - Colour::BITS_BLUE;
				if(Colour::BITS_ALPHA > 0)	aColour[Colour::INDEX_ALPHA] <<= ELEMENT_BITS - Colour::BITS_ALPHA;

				if(Colour::BITS_RED > 0)	bitStream.WriteBits(&aColour[Colour::INDEX_RED], Colour::BITS_RED);
				if(Colour::BITS_GREEN > 0)	bitStream.WriteBits(&aColour[Colour::INDEX_GREEN], Colour::BITS_GREEN);
				if(Colour::BITS_BLUE > 0)	bitStream.WriteBits(&aColour[Colour::INDEX_BLUE], Colour::BITS_BLUE);
				if(Colour::BITS_ALPHA > 0)	bitStream.WriteBits(&aColour[Colour::INDEX_ALPHA], Colour::BITS_ALPHA);
			}
		}
	};
}


#endif
