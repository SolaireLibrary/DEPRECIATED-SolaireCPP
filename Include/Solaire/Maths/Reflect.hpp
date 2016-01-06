#ifndef SOLAIRE_REFLECT_HPP
#define SOLAIRE_REFLECT_HPP

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
	\file Reflect.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 26th September 2015
	Last Modified	: 15th November 2015
*/

#include <cstdint>
#include "..\Core\Maths.hpp"

namespace Solaire {

    namespace Implementation{
		static constexpr uint8_t REFLECTED_BYTES[256] = {
			0,	    128,	64,	    192,	32,	    160,	96,	    224,	16,	    144,
			80,	    208,	48,	    176,	112,	240,	8,	    136,	72,	    200,
			40,	    168,	104,	232,	24,	    152,	88,	    216,	56,     184,
			120,	248,	4,	    132,	68,	    196,	36,	    164,	100,	228,
			20,	    148,	84,	    212,	52,	    180,	116,	244,	12,	    140,
			76,	    204,	44,	    172,	108,	236,	28,	    156,	92,	    220,
			60,	    188,	124,	252,	2,	    130,	66,	    194,	34,	    162,
			98,	    226,	18,	    146,	82,	    210,	50,	    178,	114,	242,
			10,	    138,	74,	    202,	42,	    170,	106,	234,	26,	    154,
			90, 	218,	58,	    186,	122,	250,	6,	    134,	70,	    198,
			38,	    166,	102,	230,	22,	    150,	86,	    214,	54,	    182,
			118,	246,	14,	    142,	78,	    206,	46,	    174,	110,	238,
			30,	    158,	94,	    222,	62,	    190,	126,	254,	1,	    129,
			65,	    193,	33,	    161,	97,	    225,	17,	    145,	81,	    209,
			49,	    177,	113,	241,	9,	    137,	73,	    201,	41,	    169,
			105,	233,	25,	    153,	89,	    217,	57,	    185,	121,	249,
			5,	    133,	69,	    197,	37,	    165,	101,	229,	21,	    149,
			85,	    213,	53,	    181,	117,	245,	13,	    141,	77,	    205,
			45,	    173,	109,	237,	29,	    157,	93,	    221,	61,	    189,
			125,	253,	3,	    131,	67,	    195,	35,	    163,	99,	    227,
			19,	    147,	83,	    211,	51,	    179,	115,	243,	11,	    139,
			75,	    203,	43,	    171,	107,	235,	27,	    155,	91,	    219,
			59,	    187,	123,	251,	7,	    135,	71,	    199,	39,	    167,
			103,	231,	23,	    151,	87,	    215,	55,	    183,	119,	247,
			15,	    143,	79,	    207,	47,	    175,	111,	239,	31,	    159,
			95,	    23,	    63,	    191,	127,	255
		};
    }

	static constexpr uint8_t Reflect8(const uint8_t aValue) throw() {
		return Implementation::REFLECTED_BYTES[aValue];
	}

	static constexpr uint8_t Reflect4(const uint8_t aValue) throw() {
		return Reflect8(aValue) >> 4;
	}

	static constexpr uint16_t Reflect16(const uint16_t aValue) throw() {
		return
			static_cast<uint16_t>(Reflect8(aValue >> 8)) |
			(static_cast<uint16_t>(Reflect8(aValue & NYBBLE_0)) << 8);
    }

	static constexpr uint32_t Reflect32(const uint32_t aValue) throw() {
		return
			static_cast<uint32_t>(Reflect16(aValue >> 16)) |
			(static_cast<uint32_t>(Reflect16(aValue & SHORT_0)) << 16);
    }

    static constexpr uint8_t Reflect64(const uint64_t aValue) throw() {
		return
			static_cast<uint32_t>(Reflect32(aValue >> 32L)) |
			(static_cast<uint32_t>(Reflect32(aValue & INT_0)) << 32L);
    }

	static void Reflect(void* const aDst, const void* const aSrc, uint32_t aBytes) {
		uint8_t* dst = static_cast<uint8_t*>(aDst) + aBytes - 1;
		const uint8_t* src = static_cast<const uint8_t*>(aSrc);

		while(aBytes >= 8) {
			*reinterpret_cast<uint64_t*>(dst) = Reflect64(*reinterpret_cast<const uint64_t*>(src));
			dst -= 8;
			src += 8;
			aBytes -= 8;
		}

		if(aBytes >= 4) {
			*reinterpret_cast<uint32_t*>(dst) = Reflect32(*reinterpret_cast<const uint32_t*>(src));
			dst -= 4;
			src += 4;
			aBytes -= 4;
		}

		if(aBytes >= 2) {
			*reinterpret_cast<uint16_t*>(dst) = Reflect16(*reinterpret_cast<const uint16_t*>(src));
			dst -= 2;
			src += 2;
			aBytes -= 2;
		}

		if(aBytes == 1) {
			*dst = Reflect8(*src);
		}
	}

	template<class T>
	static constexpr T Reflect(const T aValue) throw();

	template<>
	constexpr uint8_t Reflect<uint8_t>(const uint8_t aValue) throw() {
		return Reflect8(aValue);
	}

	template<>
	constexpr uint16_t Reflect<uint16_t>(const uint16_t aValue) throw() {
		return Reflect16(aValue);
	}

	template<>
	constexpr uint32_t Reflect<uint32_t>(const uint32_t aValue) throw() {
		return Reflect32(aValue);
	}

	template<>
	constexpr uint64_t Reflect<uint64_t>(const uint64_t aValue) throw() {
		return Reflect64(aValue);
	}

	template<>
	constexpr int8_t Reflect<int8_t>(const int8_t aValue) throw() {
		return *reinterpret_cast<const int8_t*>(
			Reflect8(*reinterpret_cast<const uint8_t*>(&aValue))
		);
	}

	template<>
	constexpr int16_t Reflect<int16_t>(const int16_t aValue) throw() {
		return *reinterpret_cast<const int16_t*>(
			Reflect16(*reinterpret_cast<const uint16_t*>(&aValue))
		);
	}

	template<>
	constexpr int32_t Reflect<int32_t>(const int32_t aValue) throw() {
		return *reinterpret_cast<const int32_t*>(
			Reflect32(*reinterpret_cast<const uint32_t*>(&aValue))
		);
	}

	template<>
	constexpr int64_t Reflect<int64_t>(const int64_t aValue) throw() {
		return *reinterpret_cast<const int64_t*>(
			Reflect64(*reinterpret_cast<const uint64_t*>(&aValue))
		);
	}
}


#endif
