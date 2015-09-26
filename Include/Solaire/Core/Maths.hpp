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

    static void NybbleToBinary(const uint8_t aValue, char* const aString){
        *reinterpret_cast<uint32_t*>(aString) = *reinterpret_cast<const uint32_t*>(MathsInternal::BIT_PATTERNS[aValue]);
    }

    static void ToBinary(const uint8_t aValue, char* const aString){
        NybbleToBinary(Lower4(aValue), aString);
        NybbleToBinary(Upper4(aValue), aString + 4);
    }

    static void ToBinary(const uint16_t aValue, char* const aString){
        NybbleToBinary(Lower8(aValue), aString);
        NybbleToBinary(Upper8(aValue), aString + 8);
    }

    static void ToBinary(const uint32_t aValue, char* const aString){
        NybbleToBinary(Lower16(aValue), aString);
        NybbleToBinary(Upper16(aValue), aString + 16);
    }

    static void ToBinary(const uint64_t aValue, char* const aString){
        NybbleToBinary(Lower32(aValue), aString);
        NybbleToBinary(Upper32(aValue), aString + 32);
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

}}}


#endif
