#ifndef SOLAIRE_CORE_MATHS_HPP
#define SOLAIRE_CORE_MATHS_HPP

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

namespace Solaire{ namespace Core{ namespace Maths{

    namespace MathsInternal{
        static constexpr char BIT_PATTERNS[16][5]{
            {"0000"}, {"0001"}, {"0010"}, {"0011"},
            {"0100"}, {"0101"}, {"0110"}, {"0111"},
            {"1000"}, {"1001"}, {"1010"}, {"1011"},
            {"1100"}, {"1101"}, {"1110"}, {"1111"},
        };

        static constexpr uint8_t PopCount(const char* const aPattern){
            return (aPattern[0] == '1' ? 1 : 0) + (aPattern[1] == '1' ? 1 : 0) + (aPattern[2] == '1' ? 1 : 0) + (aPattern[3] == '1' ? 1 : 0);
        }

        static constexpr uint8_t BIT_COUNTS[16]{
            PopCount(BIT_PATTERNS[0]), PopCount(BIT_PATTERNS[1]), PopCount(BIT_PATTERNS[2]), PopCount(BIT_PATTERNS[3]),
            PopCount(BIT_PATTERNS[4]), PopCount(BIT_PATTERNS[5]), PopCount(BIT_PATTERNS[6]), PopCount(BIT_PATTERNS[7]),
            PopCount(BIT_PATTERNS[8]), PopCount(BIT_PATTERNS[9]), PopCount(BIT_PATTERNS[10]), PopCount(BIT_PATTERNS[11]),
            PopCount(BIT_PATTERNS[12]), PopCount(BIT_PATTERNS[13]), PopCount(BIT_PATTERNS[14]), PopCount(BIT_PATTERNS[15])
        };
    }

    // Word Length

    static constexpr uint8_t Lower4(const uint8_t aValue){
        return aValue & 15;
    }

    static constexpr uint8_t Upper4(const uint8_t aValue){
        return aValue >> 4;
    }

    static constexpr uint8_t Lower8(const uint16_t aValue){
        return aValue & UINT8_MAX;
    }

    static constexpr uint8_t Upper8(const uint16_t aValue){
        return aValue >> 8;
    }

    static constexpr uint16_t Lower16(const uint32_t aValue){
        return aValue & UINT16_MAX;
    }

    static constexpr uint16_t Upper16(const uint32_t aValue){
        return aValue >> 16;
    }

    static constexpr uint32_t Lower32(const uint64_t aValue){
        return aValue & UINT32_MAX;
    }

    static constexpr uint32_t Upper32(const uint64_t aValue){
        return aValue >> 32L;
    }

    // PopCount

    static constexpr uint32_t PopCountNybble(const uint8_t aValue){
        return MathsInternal::BIT_COUNTS[aValue];
    }

    static constexpr uint32_t PopCount(const uint8_t aValue){
        return PopCountNybble(Lower4(aValue)) + PopCountNybble(Upper4(aValue));
    }

    static constexpr uint32_t PopCount(const uint16_t aValue){
        return PopCountNybble(Lower8(aValue)) + PopCountNybble(Upper8(aValue));
    }

    static constexpr uint32_t PopCount(const uint32_t aValue){
        return PopCountNybble(Lower16(aValue)) + PopCountNybble(Upper16(aValue));
    }

    static constexpr uint32_t PopCount(const uint64_t aValue){
        return PopCountNybble(Lower32(aValue)) + PopCountNybble(Upper32(aValue));
    }

    // Hex

    static constexpr char NybbleToHex(const uint8_t aNybble){
        return aNybble < 10 ? '0' + aNybble : 'A' + (aNybble - 10);
    }

    static constexpr uint8_t HexToNybble(const char aHex){
        return aHex >='0' && aHex <= '9' ? aHex - '0' : (aHex + 10) - 'A';
    }

    // Binary string

    static void ToBinary4(const uint8_t aValue, char* const aString){
        *reinterpret_cast<uint32_t*>(aString) = *reinterpret_cast<const uint32_t*>(MathsInternal::BIT_PATTERNS[aValue]);
    }

    static void ToBinary8(const uint8_t aValue, char* const aString){
        ToBinary4(Lower4(aValue), aString);
        ToBinary4(Upper4(aValue), aString + 4);
    }

    static void ToBinary16(const uint16_t aValue, char* const aString){
        ToBinary8(Lower8(aValue), aString);
        ToBinary8(Upper8(aValue), aString + 8);
    }

    static void ToBinary32(const uint32_t aValue, char* const aString){
        ToBinary16(Lower16(aValue), aString);
        ToBinary16(Upper16(aValue), aString + 16);
    }

    static void ToBinary64(const uint64_t aValue, char* const aString){
        ToBinary32(Lower32(aValue), aString);
        ToBinary32(Upper32(aValue), aString + 32);
    }

    static constexpr uint8_t FromBinary4(const char* const aString){
        return (aString[0] == '1' ? 1 : 0) | (aString[1] == '1' ? 2 : 0) | (aString[2] == '1' ? 4 : 0) | (aString[3] == '1' ? 8 : 0);
    }

    static constexpr uint8_t FromBinary8(const char* const aString){
        return FromBinary4(aString) | FromBinary4(aString + 4);
    }

    static constexpr uint16_t FromBinary16(const char* const aString){
        return FromBinary8(aString) | FromBinary8(aString + 8);
    }

    static constexpr uint32_t FromBinary32(const char* const aString){
        return FromBinary16(aString) | FromBinary16(aString + 16);
    }

    static constexpr uint64_t FromBinary64(const char* const aString){
        return FromBinary32(aString) | FromBinary32(aString + 32);
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

    //

    template<class A, class B, typename Enable = typename std::enable_if<sizeof(A) == sizeof(B)>::type>
    static constexpr A IntactCast(const B aValue){
        return *reinterpret_cast<const B*>(&aValue);
    }

    // Reflection

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

    template<class T>
    static constexpr T Reflect(const T aValue) = delete;

    template<>
    constexpr uint8_t Reflect<uint8_t>(const uint8_t aValue){
        return REFLECTED_BYTES[aValue];
    }

    template<>
    constexpr uint16_t Reflect<uint16_t>(const uint16_t aValue){
        return (static_cast<uint16_t>(REFLECTED_BYTES[aValue & 0xFF]) << 8) | REFLECTED_BYTES[aValue >> 8];
    }

    template<>
    constexpr uint32_t Reflect<uint32_t>(const uint32_t aValue){
        return (static_cast<uint32_t>(Reflect<uint16_t>(aValue & 0xFFFF)) << 16) | Reflect<uint16_t>(aValue >> 16);
    }

    template<>
    constexpr uint64_t Reflect<uint64_t>(const uint64_t aValue){
        return (static_cast<uint64_t>(Reflect<uint32_t>(aValue & 0xFFFFFFFFL)) << 32L) | Reflect<uint32_t>(aValue >> 32L);
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

    static constexpr double Lerp(const double aFirst, const double aSecond, const double aWeight){
        return (1.0 - aWeight) * aFirst + aWeight * aSecond;
    }

}}}


#endif
