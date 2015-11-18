#ifndef SOLAIRE_GRAPHICS_COLOUR_FORMAT_HPP
#define SOLAIRE_GRAPHICS_COLOUR_FORMAT_HPP

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
	\file ColourFormat.hpp
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
#include "..\Core\Maths.hpp"
#include "..\Maths\Vector.hpp"

namespace Solaire{

	enum {
		COLOUR_RED = BIT_0,
		COLOUR_GREEN = BIT_1,
		COLOUR_BLUE = BIT_2,
		COLOUR_ALPHA = BIT_3,
	};

	namespace Implementation {
		static constexpr int32_t ColourIndexRed(const uint8_t aRedBits, const uint8_t aGreenBits, const uint8_t aBlueBits, const uint8_t aAlphaBits) {
			return aRedBits > 0 ? 0 : -1;
		}

		static constexpr int32_t ColourIndexGreen(const uint8_t aRedBits, const uint8_t aGreenBits, const uint8_t aBlueBits, const uint8_t aAlphaBits) {
			return aGreenBits > 0 ? 
				aRedBits > 0 ? 1 : 0 :
				-1;
		}

		static constexpr int32_t ColourIndexBlue(const uint8_t aRedBits, const uint8_t aGreenBits, const uint8_t aBlueBits, const uint8_t aAlphaBits) {
			return aBlueBits > 0 ?
				(aRedBits > 0 ? 1 : 0) + (aGreenBits > 0 ? 1 : 0):
				-1;
		}

		static constexpr int32_t ColourIndexAlpha(const uint8_t aRedBits, const uint8_t aGreenBits, const uint8_t aBlueBits, const uint8_t aAlphaBits) {
			return aAlphaBits > 0 ?
				(aRedBits > 0 ? 1 : 0) + (aGreenBits > 0 ? 1 : 0) + (aBlueBits > 0 ? 1 : 0) :
				-1;
		}
	}

	template<const uint8_t RED_BITS, const uint8_t GREEN_BITS, const uint8_t BLUE_BITS, const uint8_t ALPHA_BITS>
	struct ColourFormat {
		enum : int32_t {
			BITS_RED		=	RED_BITS,
			BITS_GREEN		=	GREEN_BITS,
			BITS_BLUE		=	BLUE_BITS,
			BITS_ALPHA		=	ALPHA_BITS,
			BITS_TOTAL		=	BITS_RED + BITS_BLUE + BITS_GREEN + BITS_ALPHA,

			MIN_RED			=	0,
			MIN_GREEN		=	0,
			MIN_BLUE		=	0,
			MIN_ALPHA		=	0,

			MAX_RED			=	MaxValue(BITS_RED),
			MAX_GREEN		=	MaxValue(BITS_GREEN),
			MAX_BLUE		=	MaxValue(BITS_BLUE),
			MAX_ALPHA		=	MaxValue(BITS_ALPHA),

			IS_BYTE_ALIGNED	= ((RED_BITS & 7) | (GREEN_BITS & 7) | (BLUE_BITS & 7) | (ALPHA_BITS & 7)) == 0 ? 1 : 0,
			BYTES_TOTAL		= IS_BYTE_ALIGNED ? BITS_TOTAL / 8 : -1,

			OFFSET_RED		=	0,
			OFFSET_GREEN	=	OFFSET_RED + BITS_RED,
			OFFSET_BLUE		=	OFFSET_GREEN + BITS_GREEN,
			OFFSET_ALPHA	=	OFFSET_BLUE + BITS_BLUE,

			MASK_RED		=	Set1(RED_BITS) << OFFSET_RED,
			MASK_GREEN		=	Set1(BITS_GREEN) << OFFSET_GREEN,
			MASK_BLUE		=	Set1(BITS_BLUE) << OFFSET_BLUE,
			MASK_ALPHA		=	Set1(BITS_ALPHA) << OFFSET_ALPHA,

			CHANNELS_USED	=	(BITS_RED > 0 ? 1 : 0) +
								(GREEN_BITS > 0 ? 1 : 0) +
								(BLUE_BITS > 0 ? 1 : 0)+
								(ALPHA_BITS > 0 ? 1 : 0),

			INDEX_RED		=	Implementation::ColourIndexRed(BITS_RED, BITS_GREEN, BITS_BLUE, BITS_ALPHA),
			INDEX_GREEN		=	Implementation::ColourIndexGreen(BITS_RED, BITS_GREEN, BITS_BLUE, BITS_ALPHA),
			INDEX_BLUE		=	Implementation::ColourIndexBlue(BITS_RED, BITS_GREEN, BITS_BLUE, BITS_ALPHA),
			INDEX_ALPHA		=	Implementation::ColourIndexAlpha(BITS_RED, BITS_GREEN, BITS_BLUE, BITS_ALPHA),

			CHANNEL_FLAGS	=	(BITS_RED > 0 ? COLOUR_RED : 0) |
								(GREEN_BITS > 0 ? COLOUR_GREEN : 0) |
								(BLUE_BITS > 0 ? COLOUR_BLUE : 0) |
								(ALPHA_BITS > 0 ? COLOUR_ALPHA : 0)
		};

		typedef BinaryContainer<BITS_RED> RedChannel;
		typedef BinaryContainer<BITS_GREEN> GreenChannel;
		typedef BinaryContainer<BITS_BLUE> BlueChannel;
		typedef BinaryContainer<BITS_ALPHA> AlphaChannel;
		typedef BinaryContainer<Max<int32_t>(Max<int32_t>(BITS_RED, GREEN_BITS), Max<int32_t>(BLUE_BITS, ALPHA_BITS))> MaxChannel;
		typedef Vector<MaxChannel, CHANNELS_USED> Vector;
	};

	typedef ColourFormat<1, 0, 0, 0> R_1;
	typedef ColourFormat<8, 0, 0, 0> R_8;
	typedef ColourFormat<8, 8, 0, 0> RG_8;
	typedef ColourFormat<8, 8, 8, 0> RGB_8;
	typedef ColourFormat<8, 8, 8, 8> RGBA_8;
}


#endif
