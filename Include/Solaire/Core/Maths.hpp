#ifndef SOLAIRE_MATHS_HPP
#define SOLAIRE_MATHS_HPP

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
	\file Maths.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 26th September 2015
	Last Modified	: 26th September 2015
*/

#include <cstdint>

namespace Solaire{ namespace Maths{

    namespace MathsInternal{

        static constexpr char HEX_STRING[17]{
            "0123456789ABCDEF"
        };

        static constexpr char BIT_PATTERNS[16][5]{
            {"0000"}, {"0001"}, {"0010"}, {"0011"}, {"0100"}, {"0101"}, {"0110"}, {"0111"},
            {"1000"}, {"1001"}, {"1010"}, {"1011"}, {"1100"}, {"1101"}, {"1110"}, {"1111"},
        };

        static constexpr uint8_t BIT_COUNTS[16]{
            0,	1,	1,	2,	1,	2,	2,	3,
            1,	2,	2,	3,	2,	3,	3,	4
        };

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

    // Cast

    template<class A, class B, typename Enable = typename std::enable_if<sizeof(A) == sizeof(B)>::type>
    static constexpr A IntactCast(const B aValue){
        return *reinterpret_cast<const B*>(&aValue);
    }

    // Word Length

    static constexpr uint8_t Lower4(const uint8_t aValue){
        return aValue & 0xF;
    }

    static constexpr uint8_t Upper4(const uint8_t aValue){
        return aValue >> 4;
    }

    static constexpr uint8_t Lower8(const uint16_t aValue){
        return aValue & 0xFF;
    }

    static constexpr uint8_t Upper8(const uint16_t aValue){
        return aValue >> 8;
    }

    static constexpr uint16_t Lower16(const uint32_t aValue){
        return aValue & 0xFFFF;
    }

    static constexpr uint16_t Upper16(const uint32_t aValue){
        return aValue >> 16;
    }

    static constexpr uint32_t Lower32(const uint64_t aValue){
        return aValue & 0xFFFFFFFFL;
    }

    static constexpr uint32_t Upper32(const uint64_t aValue){
        return aValue >> 32L;
    }

    // Combine

    static constexpr uint16_t Combine16(const uint8_t a0, const uint32_t a1){
        return (static_cast<uint16_t>(a0) << 8) | a1;
    }

    static constexpr uint32_t Combine32(
        const uint8_t a0, const uint32_t a1, const uint8_t a2, const uint32_t a3
    ){
        return
            (static_cast<uint32_t>(a0) << 24) |
            (static_cast<uint32_t>(a1) << 16) |
            (static_cast<uint32_t>(a2) << 8) |
            static_cast<uint32_t>(a3);
    }

    static constexpr uint64_t Combine64(
        const uint8_t a0, const uint32_t a1, const uint8_t a2, const uint32_t a3,
        const uint8_t a4, const uint32_t a5, const uint8_t a6, const uint32_t a7
    ){
        return
            (static_cast<uint64_t>(a0) << 56L) |
            (static_cast<uint64_t>(a1) << 48L) |
            (static_cast<uint64_t>(a2) << 40L) |
            (static_cast<uint64_t>(a3) << 32L) |
            (static_cast<uint64_t>(a4) << 24L) |
            (static_cast<uint64_t>(a5) << 16L) |
            (static_cast<uint64_t>(a6) << 8L) |
            static_cast<uint64_t>(a7);
    }


    // PopCount

    template<class T>
    static constexpr uint8_t PopCount(const T aValue) = delete;

    template<>
    constexpr uint8_t PopCount<uint8_t>(const uint8_t aValue){
        return MathsInternal::BIT_COUNTS[Lower4(aValue)] + MathsInternal::BIT_COUNTS[Upper4(aValue)];
    }

    template<>
    constexpr uint8_t PopCount<uint16_t>(const uint16_t aValue){
      return
            PopCount<uint8_t>(aValue >> 8) +
            PopCount<uint8_t>(aValue & 0xFF);
    }

    template<>
    constexpr uint8_t PopCount<uint32_t>(const uint32_t aValue){
      return
            PopCount<uint8_t>(aValue >> 24) +
            PopCount<uint8_t>(aValue >> 16) +
            PopCount<uint8_t>(aValue >> 8) +
            PopCount<uint8_t>(aValue & 0xFF);
    }

    template<>
    constexpr uint8_t PopCount<uint64_t>(const uint64_t aValue){
        return
            PopCount<uint8_t>(static_cast<uint8_t>(aValue >> 56L)) +
            PopCount<uint8_t>(static_cast<uint8_t>(aValue >> 48L)) +
            PopCount<uint8_t>(static_cast<uint8_t>(aValue >> 40L)) +
            PopCount<uint8_t>(static_cast<uint8_t>(aValue >> 32L)) +
            PopCount<uint8_t>(static_cast<uint8_t>(aValue >> 24L)) +
            PopCount<uint8_t>(static_cast<uint8_t>(aValue >> 16L)) +
            PopCount<uint8_t>(static_cast<uint8_t>(aValue >> 8L)) +
            PopCount<uint8_t>(static_cast<uint8_t>(aValue & 0xFFL));
    }

    template<>
    constexpr uint8_t PopCount<int8_t>(const int8_t aValue){
        return PopCount<uint8_t>(IntactCast<uint8_t>(aValue));
    }

    template<>
    constexpr uint8_t PopCount<int16_t>(const int16_t aValue){
        return PopCount<uint16_t>(IntactCast<uint16_t>(aValue));
    }

    template<>
    constexpr uint8_t PopCount<int32_t>(const int32_t aValue){
        return PopCount<uint32_t>(IntactCast<uint32_t>(aValue));
    }

    template<>
    constexpr uint8_t PopCount<int64_t>(const int64_t aValue){
        return PopCount<uint64_t>(IntactCast<uint64_t>(aValue));
    }

    // Hex

    template<class T>
    static char* ToHex(const T aValue, char* aString) = delete;

    template<>
    char* ToHex<uint8_t>(const uint8_t aValue, char* aString){
        *aString = MathsInternal::HEX_STRING[aValue >> 4];
        ++aString;
        *aString = MathsInternal::HEX_STRING[aValue & 0xF];
        ++aString;
        return aString;
    }

    template<>
    char* ToHex<uint16_t>(const uint16_t aValue, char* aString){
        return ToHex<uint8_t>(aValue >> 8, ToHex<uint8_t>(aValue & 0xFF, aString));
    }

    template<>
    char* ToHex<uint32_t>(const uint32_t aValue, char* aString){
        return ToHex<uint16_t>(aValue >> 16, ToHex<uint16_t>(aValue & 0xFFFF, aString));
    }

    template<>
    char* ToHex<uint64_t>(const uint64_t aValue, char* aString){
        return ToHex<uint32_t>(aValue >> 32L, ToHex<uint32_t>(aValue & 0xFFFFFFFFL, aString));
    }

    template<class T>
    static const char* FromHex(T& aValue, const char* aString) = delete;

    template<>
    const char* FromHex<uint8_t>(uint8_t& aValue, const char* aString){
        char c;

        c = *aString;
        aValue = c >='0' && c <= '9' ? c - '0' : (c + 10) - 'A';
        ++aString;

        c = *aString;
        aValue |= (c >='0' && c <= '9' ? c - '0' : (c + 10) - 'A') << 4;
        ++aString;
        return aString;
    }

    template<>
    const char* FromHex<uint16_t>(uint16_t& aValue, const char* aString){
        return FromHex<uint8_t>(*(reinterpret_cast<uint8_t*>(&aValue) + 1), FromHex<uint8_t>(*reinterpret_cast<uint8_t*>(&aValue), aString));
    }

    template<>
    const char* FromHex<uint32_t>(uint32_t& aValue, const char* aString){
        return FromHex<uint16_t>(*(reinterpret_cast<uint16_t*>(&aValue) + 1), FromHex<uint16_t>(*reinterpret_cast<uint16_t*>(&aValue), aString));
    }

    template<>
    const char* FromHex<uint64_t>(uint64_t& aValue, const char* aString){
        return FromHex<uint32_t>(*(reinterpret_cast<uint32_t*>(&aValue) + 1), FromHex<uint32_t>(*reinterpret_cast<uint32_t*>(&aValue), aString));
    }


    // Binary string

    template<class T>
    static char* ToBinary(const T aValue, char* aString) = delete;

    template<>
    char* ToBinary<uint8_t>(const uint8_t aValue, char* aString){
        uint32_t* string = reinterpret_cast<uint32_t*>(aString);
        *string = *reinterpret_cast<const uint32_t*>(MathsInternal::BIT_PATTERNS[aValue & 0xF]);
        ++string;
        *string = *reinterpret_cast<const uint32_t*>(MathsInternal::BIT_PATTERNS[aValue >> 4]);
        return aString + 8;
    }

    template<>
    char* ToBinary<uint16_t>(const uint16_t aValue, char* aString){
        return ToBinary<uint8_t>(aValue >> 8, ToBinary<uint8_t>(aValue & 0xFF, aString));
    }

    template<>
    char* ToBinary<uint32_t>(const uint32_t aValue, char* aString){
        return ToBinary<uint16_t>(aValue >> 16, ToBinary<uint16_t>(aValue & 0xFFFF, aString));
    }

    template<>
    char* ToBinary<uint64_t>(const uint64_t aValue, char* aString){
        return ToBinary<uint32_t>(aValue >> 32L, ToBinary<uint32_t>(aValue & 0xFFFFFFFFL, aString));
    }

    template<>
    char* ToBinary<int8_t>(const int8_t aValue, char* aString){
        return ToBinary<uint8_t>(IntactCast<uint8_t>(aValue), aString);
    }

    template<>
    char* ToBinary<int16_t>(const int16_t aValue, char* aString){
        return ToBinary<uint16_t>(IntactCast<uint16_t>(aValue), aString);
    }

    template<>
    char* ToBinary<int32_t>(const int32_t aValue, char* aString){
        return ToBinary<uint32_t>(IntactCast<uint32_t>(aValue), aString);
    }

    template<>
    char* ToBinary<int64_t>(const int64_t aValue, char* aString){
        return ToBinary<uint64_t>(IntactCast<uint64_t>(aValue), aString);
    }

    template<class T>
    static const char* FromBinary(T& aValue, const char* aString) = delete;

    template<>
    const char* FromBinary<uint8_t>(uint8_t& aValue, const char* aString){
        aValue = (aString[0] == '1' ? 1 : 0) | (aString[1] == '1' ? 2 : 0) | (aString[2] == '1' ? 4 : 0) | (aString[3] == '1' ? 8 : 0);
        aString += 4;
        aValue |= (aString[0] == '1' ? 1 : 0) | (aString[1] == '1' ? 2 : 0) | (aString[2] == '1' ? 4 : 0) | (aString[3] == '1' ? 8 : 0) << 4;
        return aString + 4;
    }

    template<>
    const char* FromBinary<uint16_t>(uint16_t& aValue, const char* aString){
        return FromBinary<uint8_t>(*(reinterpret_cast<uint8_t*>(&aValue) + 1), FromBinary<uint8_t>(*reinterpret_cast<uint8_t*>(&aValue), aString));
    }

    template<>
    const char* FromBinary<uint32_t>(uint32_t& aValue, const char* aString){
        return FromBinary<uint16_t>(*(reinterpret_cast<uint16_t*>(&aValue) + 1), FromBinary<uint16_t>(*reinterpret_cast<uint16_t*>(&aValue), aString));
    }

    template<>
    const char* FromBinary<uint64_t>(uint64_t& aValue, const char* aString){
        return FromBinary<uint32_t>(*(reinterpret_cast<uint32_t*>(&aValue) + 1), FromBinary<uint32_t>(*reinterpret_cast<uint32_t*>(&aValue), aString));
    }

    template<>
    const char* FromBinary<int8_t>(int8_t& aValue, const char* aString){
        return FromBinary<uint8_t>(reinterpret_cast<uint8_t&>(aValue), aString);
    }

    template<>
    const char* FromBinary<int16_t>(int16_t& aValue, const char* aString){
        return FromBinary<uint16_t>(reinterpret_cast<uint16_t&>(aValue), aString);
    }

    template<>
    const char* FromBinary<int32_t>(int32_t& aValue, const char* aString){
        return FromBinary<uint32_t>(reinterpret_cast<uint32_t&>(aValue), aString);
    }

    template<>
    const char* FromBinary<int64_t>(int64_t& aValue, const char* aString){
        return FromBinary<uint64_t>(reinterpret_cast<uint64_t&>(aValue), aString);
    }

    // Conditions

    template<class T>
    static constexpr bool IsPow2(const T aValue){
        return aValue == 0 ? false : (aValue & (aValue - 1)) == 0;
    }

    // Rounding

    template<class T>
    static constexpr T CeilToClosestMultiple(const T aValue, const T aMultiple){
        return ((aValue + aMultiple - 1) / aMultiple) * aMultiple;
    }

    template<class T>
    static constexpr T CeilToClosestMultiplePow2(const T aValue, const T aMultiple){
        return (aValue + aMultiple - 1) & ~(aMultiple - 1);
    }

    template<class T>
    static constexpr T FloorToClosestMultiple(const T aValue, const T aMultiple){
        return aValue % aMultiple == 0 ? aValue : CeilToClosestMultiple<T>(aValue, aMultiple) - aMultiple;
    }

    template<class T>
    static constexpr T FloorToClosestMultiplePow2(const T aValue, const T aMultiple){
        return aValue & (aMultiple - 1) == 0 ? aValue : CeilToClosestMultiple<T>(aValue, aMultiple) - aMultiple;
    }

    template<class T>
    static constexpr T RoundToClosestMultiple(const T aValue, const T aMultiple){
        return CeilToClosestMultiple<T>(aValue, aMultiple) - (((aValue % aMultiple) < 5) && ((aValue % aMultiple) != 0) ? 5 : 0);
    }

    template<class T>
    static constexpr T RoundToClosestMultiplePow2(const T aValue, const T aMultiple){
        return CeilToClosestMultiple<T>(aValue, aMultiple) - (((aValue & (aMultiple - 1)) < 5) && ((aValue & (aMultiple - 1)) != 0) ? 5 : 0);
    }

    // Reflection

    template<class T>
    static constexpr T Reflect(const T aValue) = delete;

    template<>
    constexpr uint8_t Reflect<uint8_t>(const uint8_t aValue){
        return MathsInternal::REFLECTED_BYTES[aValue];
    }

    template<>
    constexpr uint16_t Reflect<uint16_t>(const uint16_t aValue){
        return
            (static_cast<uint16_t>(Reflect<uint8_t>(aValue >> 8)))|
            (static_cast<uint16_t>(Reflect<uint8_t>(aValue & 0xFF)) << 8);
    }

    template<>
    constexpr uint32_t Reflect<uint32_t>(const uint32_t aValue){
        return
            (static_cast<uint32_t>(Reflect<uint8_t>(aValue >> 24))) |
            (static_cast<uint32_t>(Reflect<uint8_t>(aValue >> 16)) << 8) |
            (static_cast<uint32_t>(Reflect<uint8_t>(aValue >> 8)) << 16) |
            (static_cast<uint32_t>(Reflect<uint8_t>(aValue & 0xFF)) << 24);
    }

    template<>
    constexpr uint64_t Reflect<uint64_t>(const uint64_t aValue){
        return
            (static_cast<uint64_t>(Reflect<uint8_t>(static_cast<uint8_t>(aValue >> 56L)))) |
            (static_cast<uint64_t>(Reflect<uint8_t>(static_cast<uint8_t>(aValue >> 48L)) << 8L)) |
            (static_cast<uint64_t>(Reflect<uint8_t>(static_cast<uint8_t>(aValue >> 40L)) << 16L)) |
            (static_cast<uint64_t>(Reflect<uint8_t>(static_cast<uint8_t>(aValue >> 32L)) << 24L)) |
            (static_cast<uint64_t>(Reflect<uint8_t>(static_cast<uint8_t>(aValue >> 24L)) << 32L)) |
            (static_cast<uint64_t>(Reflect<uint8_t>(static_cast<uint8_t>(aValue >> 16L)) << 40L)) |
            (static_cast<uint64_t>(Reflect<uint8_t>(static_cast<uint8_t>(aValue >> 8L)) << 48L)) |
            (static_cast<uint64_t>(Reflect<uint8_t>(static_cast<uint8_t>(aValue & 0xFFL)) << 56L));
    }

    template<>
    constexpr int8_t Reflect<int8_t>(const int8_t aValue){
        return IntactCast<int8_t>(Reflect<uint8_t>(IntactCast<uint8_t>(aValue)));
    }

    template<>
    constexpr int16_t Reflect<int16_t>(const int16_t aValue){
        return IntactCast<int16_t>(Reflect<uint16_t>(IntactCast<uint16_t>(aValue)));
    }

    template<>
    constexpr int32_t Reflect<int32_t>(const int32_t aValue){
        return IntactCast<int32_t>(Reflect<uint32_t>(IntactCast<uint32_t>(aValue)));
    }

    template<>
    constexpr int64_t Reflect<int64_t>(const int64_t aValue){
        return IntactCast<int64_t>(Reflect<uint64_t>(IntactCast<uint64_t>(aValue)));
    }

    // Misc

	template<class T>
    static constexpr T Lerp(const T aFirst, const T aSecond, const T aWeight){
        return (static_cast<T>(1) - aWeight) * aFirst + aWeight * aSecond;
    }

}}


#endif
