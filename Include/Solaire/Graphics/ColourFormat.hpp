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
	Created			: 17th November 2015
	Last Modified	: 18th November 2015
*/

#include "..\Core\Init.hpp"
#include "..\Core\Maths.hpp"
#include "..\Maths\Vector.hpp"

namespace Solaire{

	enum {
		COLOUR_NONE		= 0,
		COLOUR_RED		= BIT_0,
		COLOUR_GREEN	= BIT_1,
		COLOUR_BLUE		= BIT_2,
		COLOUR_ALPHA	= BIT_3,
	};

	template<
		const uint8_t COLOUR_0, const uint8_t COLOUR_1, const uint8_t COLOUR_2, const uint8_t COLOUR_3,
		const uint8_t COLOUR_0_BITS, const uint8_t COLOUR_1_BITS, const uint8_t COLOUR_2_BITS, const uint8_t COLOUR_3_BITS
	>
	class ColourFormat {
	private:
		static_assert(COLOUR_0 != COLOUR_NONE, "SolaireCPP : channel 0 cannot be COLOUR_NONE");
		static_assert(COLOUR_0_BITS > 0, "SolaireCPP : channel 0 must have bits");
		static_assert(! (COLOUR_1_BITS == 0 && COLOUR_2_BITS > 0), "SolaireCPP : ColourFormat channel 1 cannot be empty when channel 2 is not empty");
		static_assert(! (COLOUR_1_BITS == 0 && COLOUR_3_BITS > 0), "SolaireCPP : ColourFormat channel 1 cannot be empty when channel 3 is not empty");
		static_assert(! (COLOUR_2_BITS == 0 && COLOUR_3_BITS > 0), "SolaireCPP : ColourFormat channel 2 cannot be empty when channel 3 is not empty");

		static_assert(! (COLOUR_1 == COLOUR_NONE && COLOUR_1_BITS > 0), "SolaireCPP : ColourFormat channel 1 cannot be COLOUR_NONE when it has bits");
		static_assert(! (COLOUR_2 == COLOUR_NONE && COLOUR_2_BITS > 0), "SolaireCPP : ColourFormat channel 2 cannot be COLOUR_NONE when it has bits");
		static_assert(! (COLOUR_3 == COLOUR_NONE && COLOUR_3_BITS > 0), "SolaireCPP : ColourFormat channel 3 cannot be COLOUR_NONE when it has bits");

		static_assert(! (COLOUR_1 != COLOUR_NONE && COLOUR_1_BITS == 0), "SolaireCPP : ColourFormat channel 1 must be COLOUR_NONE when it has no bits");
		static_assert(! (COLOUR_2 != COLOUR_NONE && COLOUR_2_BITS == 0), "SolaireCPP : ColourFormat channel 2 must be COLOUR_NONE when it has no bits");
		static_assert(! (COLOUR_3 != COLOUR_NONE && COLOUR_3_BITS == 0), "SolaireCPP : ColourFormat channel 3 must be COLOUR_NONE when it has no bits");

		static_assert(((COLOUR_0 == COLOUR_RED) + (COLOUR_1 == COLOUR_RED) + (COLOUR_2 == COLOUR_RED) + (COLOUR_3 == COLOUR_RED)) <= 1, "SolaireCPP : ColourFormat only one channel can be COLOUR_RED");
		static_assert(((COLOUR_0 == COLOUR_GREEN) + (COLOUR_1 == COLOUR_GREEN) + (COLOUR_2 == COLOUR_GREEN) + (COLOUR_3 == COLOUR_GREEN)) <= 1, "SolaireCPP : ColourFormat only one channel can be COLOUR_GREEN");
		static_assert(((COLOUR_0 == COLOUR_BLUE) + (COLOUR_1 == COLOUR_BLUE) + (COLOUR_2 == COLOUR_BLUE) + (COLOUR_3 == COLOUR_BLUE)) <= 1, "SolaireCPP : ColourFormat only one channel can be COLOUR_BLUE");
		static_assert(((COLOUR_0 == COLOUR_ALPHA) + (COLOUR_1 == COLOUR_ALPHA) + (COLOUR_2 == COLOUR_ALPHA) + (COLOUR_3 == COLOUR_ALPHA)) <= 1, "SolaireCPP : ColourFormat only one channel can be COLOUR_RED");
	public:
		typedef uint8_t ChannelColour;
		typedef int32_t ChannelIndex;
	public:
		enum  : int32_t{
			INDEX_0		=	COLOUR_0_BITS > 0 ? 0 : -1,
			INDEX_1		=	COLOUR_1_BITS > 0 ?
							COLOUR_0_BITS > 0 ? 1 : 0 :
							-1,
			INDEX_2		=	COLOUR_2_BITS > 0 ?
							(COLOUR_0_BITS > 0 ? 1 : 0) + (COLOUR_1_BITS> 0 ? 1 : 0) :
							-1,
			INDEX_3		=	COLOUR_3_BITS > 0 ?
							(COLOUR_0_BITS > 0 ? 1 : 0) + (COLOUR_1_BITS > 0 ? 1 : 0) + (COLOUR_2_BITS> 0 ? 1 : 0) :
							-1
		};

		static constexpr ChannelColour GetColour(const ChannelIndex aChannel) {
			return
				aChannel == 0 ? COLOUR_0 :
				aChannel == 1 ? COLOUR_1 :
				aChannel == 2 ? COLOUR_2 :
				aChannel == 3 ? COLOUR_3 :
				-1;
		}

		static constexpr ChannelIndex GetChannel(const ChannelColour aColour) {
			return
				COLOUR_0 == aColour ? 0 :
				COLOUR_1 == aColour ? 1 :
				COLOUR_2 == aColour ? 2 :
				COLOUR_3 == aColour ? 3 :
				-1;
		}

		static constexpr int32_t GetChannelBits(const ChannelIndex aChannel) {
			return
				aChannel == 0	? COLOUR_0_BITS :
				aChannel == 1	? COLOUR_1_BITS :
				aChannel == 2	? COLOUR_2_BITS :
				aChannel == 3	? COLOUR_3_BITS :
				0;
		}

		static constexpr int32_t GetColourBits(const ChannelColour aColour) {
			return GetChannelBits(GetChannel(aColour));
		}

		static constexpr int32_t GetChannelIndex(const ChannelIndex aChannel) {
			return
				aChannel == 0 ? INDEX_0 :
				aChannel == 1 ? INDEX_1 :
				aChannel == 2 ? INDEX_2 :
				aChannel == 3 ? INDEX_3 :
				-1;
		}

		static constexpr int32_t GetColourIndex(const ChannelColour aColour) {
			return GetChannelIndex(GetChannel(aColour));
		}

		static constexpr int32_t GetChannelOffset(const ChannelIndex aChannel) {
			return
				aChannel == 0 ? 0 :
				aChannel == 1 ? GetChannelBits(0) + GetChannelOffset(0) :
				aChannel == 2 ? GetChannelBits(1) + GetChannelOffset(1) :
				aChannel == 3 ? GetChannelBits(2) + GetChannelOffset(2) :
				-1;
		}

		static constexpr int32_t GetColourOffset(const ChannelColour aColour) {
			return GetChannelOffset(GetChannel(aColour));
		}

		enum : int32_t {
			BITS_RED		=	GetColourBits(COLOUR_RED),
			BITS_GREEN		=	GetColourBits(COLOUR_GREEN),
			BITS_BLUE		=	GetColourBits(COLOUR_BLUE),
			BITS_ALPHA		=	GetColourBits(COLOUR_ALPHA),

			BITS_0			=	GetChannelBits(0),
			BITS_1			=	GetChannelBits(1),
			BITS_2			=	GetChannelBits(2),
			BITS_3			=	GetChannelBits(3),

			BITS_TOTAL		=	BITS_0 + BITS_1 + BITS_2 + BITS_3,

			MIN_RED			=	0,
			MIN_GREEN		=	0,
			MIN_BLUE		=	0,
			MIN_ALPHA		=	0,

			MAX_RED			=	MaxValue(BITS_RED),
			MAX_GREEN		=	MaxValue(BITS_GREEN),
			MAX_BLUE		=	MaxValue(BITS_BLUE),
			MAX_ALPHA		=	MaxValue(BITS_ALPHA),

			MIN_0			=	0,
			MIN_1			=	0,
			MIN_2			=	0,
			MIN_3			=	0,

			MAX_0			=	MaxValue(BITS_0),
			MAX_1			=	MaxValue(BITS_1),
			MAX_2			=	MaxValue(BITS_2),
			MAX_3			=	MaxValue(BITS_3),

			IS_BYTE_ALIGNED	=	((BITS_0 & 7) | (BITS_1 & 7) | (BITS_2 & 7) | (BITS_3 & 7)) == 0 ? 1 : 0,
			BYTES_TOTAL		=	IS_BYTE_ALIGNED ? BITS_TOTAL / 8 : -1,

			INDEX_RED		=	GetColourIndex(COLOUR_RED),
			INDEX_GREEN		=	GetColourIndex(COLOUR_GREEN),
			INDEX_BLUE		=	GetColourIndex(COLOUR_BLUE),
			INDEX_ALPHA		=	GetColourIndex(COLOUR_ALPHA),

			OFFSET_RED		=	GetColourOffset(COLOUR_RED),
			OFFSET_GREEN	=	GetColourOffset(COLOUR_GREEN),
			OFFSET_BLUE		=	GetColourOffset(COLOUR_BLUE),
			OFFSET_ALPHA	=	GetColourOffset(COLOUR_ALPHA),

			OFFSET_0		=	GetChannelOffset(0),
			OFFSET_1		=	GetChannelOffset(1),
			OFFSET_2		=	GetChannelOffset(2),
			OFFSET_3		=	GetChannelOffset(3),

			MASK_RED		=	Set1(BITS_RED) << OFFSET_RED,
			MASK_GREEN		=	Set1(BITS_GREEN) << OFFSET_GREEN,
			MASK_BLUE		=	Set1(BITS_BLUE) << OFFSET_BLUE,
			MASK_ALPHA		=	Set1(BITS_ALPHA) << OFFSET_ALPHA,

			MASK_0			=	Set1(BITS_0) << OFFSET_0,
			MASK_1			=	Set1(BITS_1) << OFFSET_1,
			MASK_2			=	Set1(BITS_2) << OFFSET_2,
			MASK_3			=	Set1(BITS_3) << OFFSET_3,

			CHANNELS_USED	=	(BITS_0 > 0 ? 1 : 0) +
								(BITS_1 > 0 ? 1 : 0) +
								(BITS_2 > 0 ? 1 : 0)+
								(BITS_3 > 0 ? 1 : 0)
		};

		typedef BinaryContainer<BITS_RED>	ContainerRed;
		typedef BinaryContainer<BITS_GREEN> ContainerGreen;
		typedef BinaryContainer<BITS_BLUE>	ContainerBlue;
		typedef BinaryContainer<BITS_ALPHA> ContainerAlpha;

		typedef BinaryContainer<BITS_0>	Container0;
		typedef BinaryContainer<BITS_1> Container1;
		typedef BinaryContainer<BITS_2>	Container2;
		typedef BinaryContainer<BITS_3> Container3;

		typedef BinaryContainer<Max<int32_t>(Max<int32_t>(BITS_0, BITS_1), Max<int32_t>(BITS_2, BITS_3))> MaxChannel;
		typedef Vector<MaxChannel, CHANNELS_USED> Vector;
	};

	template<const uint8_t BITS>
	using ColourFormatRGB = ColourFormat<COLOUR_RED, COLOUR_GREEN, COLOUR_BLUE, COLOUR_NONE, BITS, BITS, BITS, 0>;

	template<const uint8_t BITS>
	using ColourFormatRGBA = ColourFormat<COLOUR_RED, COLOUR_GREEN, COLOUR_BLUE, COLOUR_ALPHA, BITS, BITS, BITS, BITS>;

	template<const uint8_t BITS>
	using ColourFormatRBG = ColourFormat<COLOUR_RED, COLOUR_BLUE, COLOUR_GREEN, COLOUR_NONE, BITS, BITS, BITS, 0>;

	template<const uint8_t BITS>
	using ColourFormatRBGA = ColourFormat<COLOUR_RED, COLOUR_BLUE, COLOUR_GREEN, COLOUR_ALPHA, BITS, BITS, BITS, BITS>;

	typedef ColourFormatRGB<8>		RGB_8;
	typedef ColourFormatRGBA<8>		RGBA_8;
	typedef ColourFormatRBG<8>		RBG_8;
	typedef ColourFormatRBGA<8>		RBGA_8;

	template<class A, class B>
	static typename A::Vector ColourCast(const typename B::Vector aColour) {
		typename A::Vector tmp;

		if(A::BITS_RED > 0) tmp[A::INDEX_RED] = B::BITS_RED > 0 ? 
			static_cast<typename A::MaxChannel>((static_cast<float>(aColour[B::INDEX_RED]) / static_cast<float>(B::MAX_RED)) * static_cast<float>(A::MAX_RED)) :
			A::MIN_RED;

		if(A::BITS_GREEN > 0) tmp[A::INDEX_GREEN] = B::BITS_GREEN > 0 ? 
			static_cast<typename A::MaxChannel>((static_cast<float>(aColour[B::INDEX_GREEN]) / static_cast<float>(B::MAX_GREEN)) * static_cast<float>(A::MAX_GREEN)) :
			A::MIN_GREEN;

		if(A::BITS_BLUE > 0) tmp[A::INDEX_BLUE] = B::BITS_BLUE > 0 ? 
			static_cast<typename A::MaxChannel>((static_cast<float>(aColour[B::INDEX_BLUE]) / static_cast<float>(B::MAX_BLUE)) * static_cast<float>(A::MAX_BLUE)) :
			A::MIN_BLUE;

		if(A::BITS_ALPHA > 0) tmp[A::INDEX_ALPHA] = B::BITS_ALPHA > 0 ? 
			static_cast<typename A::MaxChannel>((static_cast<float>(aColour[B::INDEX_ALPHA]) / static_cast<float>(B::MAX_ALPHA)) * static_cast<float>(A::MAX_ALPHA)) :
			A::MIN_ALPHA;

		return tmp;
	}

	template<class A>
	static Vector<float, A::CHANNELS_USED> NormaliseColour(const typename A::Vector aColour) {
		Vector<float, A::CHANNELS_USED> tmp;

		if(A::BITS_RED > 0) tmp[A::INDEX_RED]		= static_cast<float>(aColour[A::INDEX_RED]) / static_cast<float>(aColour[A::MAX_RED]);
		if(A::BITS_GREEN > 0) tmp[A::INDEX_GREEN]	= static_cast<float>(aColour[A::INDEX_GREEN]) / static_cast<float>(aColour[A::MAX_GREEN]);
		if(A::BITS_BLUE > 0) tmp[A::INDEX_BLUE]		= static_cast<float>(aColour[A::INDEX_BLUE]) / static_cast<float>(aColour[A::MAX_BLUE]);
		if(A::BITS_ALPHA > 0) tmp[A::INDEX_ALPHA]	= static_cast<float>(aColour[A::INDEX_ALPHA]) / static_cast<float>(aColour[A::MAX_ALPHA]);
	
		return tmp;
	}
}


#endif
