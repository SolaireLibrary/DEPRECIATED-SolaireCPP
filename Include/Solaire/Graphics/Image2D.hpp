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

namespace Solaire{

	template<const uint8_t RED_BITS, const uint8_t GREEN_BITS, const uint8_t BLUE_BITS, const uint8_t ALPHA_BITS>
	struct ColourFormat {
		enum : uint32_t{
			BITS_RED		= RED_BITS,
			BITS_GREEN		= GREEN_BITS,
			BITS_BLUE		= BLUE_BITS,
			BITS_ALPHA		= ALPHA_BITS,
			BITS_TOTAL		= BITS_RED + BITS_BLUE + BITS_GREEN, BITS_ALPHA,

			OFFSET_RED		= 0,
			OFFSET_GREEN	= OFFSET_RED + BITS_RED,
			OFFSET_BLUE		= OFFSET_GREEN + BITS_GREEN,
			OFFSET_ALPHA	= OFFSET_BLUE + BITS_BLUE,

			MASK_RED		= Set1(RED_BITS) << OFFSET_RED,
			MASK_GREEN		= Set1(OFFSET_GREEN) << OFFSET_GREEN,
			MASK_BLUE		= Set1(OFFSET_BLUE) << OFFSET_BLUE,
			MASK_ALPHA		= Set1(OFFSET_ALPHA) << OFFSET_ALPHA
		}

		typedef BinaryContainer<BITS_RED> RedType;
		typedef BinaryContainer<BITS_GREEN> GreenType;
		typedef BinaryContainer<BITS_BLUE> BlueType;
		typedef BinaryContainer<BITS_ALPHA> AlphaType;
		typedef Vector4<MaxClass<MaxClass<RedType, GreenType>, MaxClass<BlueType, AlphaType>>> ColourType;
	};

	template<const uint8_t RED_BITS, const uint8_t GREEN_BITS, const uint8_t BLUE_BITS, const uint8_t ALPHA_BITS>
	class Image2D {
	public:
		typedef ColourFormat<RED_BITS, GREEN_BITS, BLUE_BITS, ALPHA_BITS> _ColourFormat;
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
			float size = aWidth * aHeight;
			size *= _ColourFormat::BITS_TOTAL;
			size /= 8;
			return static_cast<uint32_t>(CeilToMultiple<float>(size, 8.f));
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

		_ColourFormat::ColourType SOLAIRE_EXPORT_CALL GetPixel(const uint32_t aX, const uint32_t aY)  const {
			const uint32_t index = 0; //! \todo Calculate index

			if(_ColourFormat::BITS_TOTAL == 32 || _ColourFormat::BITS_TOTAL == 64 || _ColourFormat::BITS_TOTAL == 128 || _ColourFormat::BITS_TOTAL == 256) {
				return static_cast<const _ColourFormat::ColourType*>(mData)[index];
			}else {
				BitStream bitStream(mData);
				bitStream.IncrementBit(index * _ColourFormat::BITS_TOTAL);

				_ColourFormat::ColourType buf;
				bitStream.ReadBits(&buf[0], _ColourFormat::BITS_RED);
				bitStream.ReadBits(&buf[1], _ColourFormat::BITS_GREEN);
				bitStream.ReadBits(&buf[2], _ColourFormat::BITS_BLUE);
				bitStream.ReadBits(&buf[3], _ColourFormat::BITS_ALPHA);

				return buf;
			}
		}

		void SOLAIRE_EXPORT_CALL SetPixel(const uint32_t aX, const uint32_t aY, const _ColourFormat::ColourType aColour) {
			const uint32_t index = 0; //! \todo Calculate index

			if(_ColourFormat::BITS_TOTAL == 32 || _ColourFormat::BITS_TOTAL == 64 || _ColourFormat::BITS_TOTAL == 128 || _ColourFormat::BITS_TOTAL == 256) {
				return static_cast<_ColourFormat::ColourType*>(mData)[index] = aColour;
			}else {
				BitStream bitStream(mData);
				bitStream.IncrementBit(index *_ColourFormat::BITS_TOTAL);

				bitStream.WriteBits(&aColour[0], _ColourFormat::BITS_RED);
				bitStream.WriteBits(&aColour[1], _ColourFormat::BITS_GREEN);
				bitStream.WriteBits(&aColour[2], _ColourFormat::BITS_BLUE);
				bitStream.WriteBits(&aColour[3], _ColourFormat::BITS_ALPHA);
			}
		}
	};

	typedef Image2D<8, 8, 8, 0> Image2D_8880;
	typedef Image2D<8, 8, 8, 8> Image2D_8888;
}


#endif
