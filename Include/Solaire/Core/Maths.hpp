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
    }

    // Cast

    template<class A, class B, typename Enable = typename std::enable_if<sizeof(A) == sizeof(B)>::type>
    static constexpr A BitwiseCast(const B aValue) throw() {
        return *reinterpret_cast<const B*>(&aValue);
    }

    // Word Length

    static constexpr uint8_t Lower4(const uint8_t aValue) throw() {
        return aValue & 0xF;
    }

    static constexpr uint8_t Upper4(const uint8_t aValue) throw() {
        return aValue >> 4;
    }

    static constexpr uint8_t Lower8(const uint16_t aValue) throw() {
        return aValue & 0xFF;
    }

    static constexpr uint8_t Upper8(const uint16_t aValue) throw() {
        return aValue >> 8;
    }

    static constexpr uint16_t Lower16(const uint32_t aValue) throw() {
        return aValue & 0xFFFF;
    }

    static constexpr uint16_t Upper16(const uint32_t aValue) throw() {
        return aValue >> 16;
    }

    static constexpr uint32_t Lower32(const uint64_t aValue) throw() {
        return aValue & 0xFFFFFFFFL;
    }

    static constexpr uint32_t Upper32(const uint64_t aValue) throw() {
        return aValue >> 32L;
    }

    // Combine

    static constexpr uint16_t Combine16(const uint8_t a0, const uint32_t a1) throw(){
        return (static_cast<uint16_t>(a0) << 8) | a1;
    }

    static constexpr uint32_t Combine32(
        const uint8_t a0, const uint32_t a1, const uint8_t a2, const uint32_t a3
    ) throw() {
        return
            (static_cast<uint32_t>(a0) << 24) |
            (static_cast<uint32_t>(a1) << 16) |
            (static_cast<uint32_t>(a2) << 8) |
            static_cast<uint32_t>(a3);
    }

    static constexpr uint64_t Combine64(
        const uint8_t a0, const uint32_t a1, const uint8_t a2, const uint32_t a3,
        const uint8_t a4, const uint32_t a5, const uint8_t a6, const uint32_t a7
    ) throw() {
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

    // Hex

    template<class T>
    static char* ToHex(const T aValue, char* aString) throw() = delete;

    template<>
    char* ToHex<uint8_t>(const uint8_t aValue, char* aString) throw() {
        *aString = MathsInternal::HEX_STRING[aValue >> 4];
        ++aString;
        *aString = MathsInternal::HEX_STRING[aValue & 0xF];
        ++aString;
        return aString;
    }

    template<>
    char* ToHex<uint16_t>(const uint16_t aValue, char* aString) throw() {
        return ToHex<uint8_t>(aValue >> 8, ToHex<uint8_t>(aValue & 0xFF, aString));
    }

    template<>
    char* ToHex<uint32_t>(const uint32_t aValue, char* aString) throw() {
        return ToHex<uint16_t>(aValue >> 16, ToHex<uint16_t>(aValue & 0xFFFF, aString));
    }

    template<>
    char* ToHex<uint64_t>(const uint64_t aValue, char* aString) throw() {
        return ToHex<uint32_t>(aValue >> 32L, ToHex<uint32_t>(aValue & 0xFFFFFFFFL, aString));
    }

    template<class T>
    static const char* FromHex(T& aValue, const char* aString) throw() = delete;

    template<>
    const char* FromHex<uint8_t>(uint8_t& aValue, const char* aString) throw() {
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
    const char* FromHex<uint16_t>(uint16_t& aValue, const char* aString) throw() {
        return FromHex<uint8_t>(*(reinterpret_cast<uint8_t*>(&aValue) + 1), FromHex<uint8_t>(*reinterpret_cast<uint8_t*>(&aValue), aString));
    }

    template<>
    const char* FromHex<uint32_t>(uint32_t& aValue, const char* aString) throw() {
        return FromHex<uint16_t>(*(reinterpret_cast<uint16_t*>(&aValue) + 1), FromHex<uint16_t>(*reinterpret_cast<uint16_t*>(&aValue), aString));
    }

    template<>
    const char* FromHex<uint64_t>(uint64_t& aValue, const char* aString) throw() {
        return FromHex<uint32_t>(*(reinterpret_cast<uint32_t*>(&aValue) + 1), FromHex<uint32_t>(*reinterpret_cast<uint32_t*>(&aValue), aString));
    }


    // Binary string

    template<class T>
    static char* ToBinary(const T aValue, char* aString) throw() = delete;

    template<>
    char* ToBinary<uint8_t>(const uint8_t aValue, char* aString) throw() {
        uint32_t* string = reinterpret_cast<uint32_t*>(aString);
        *string = *reinterpret_cast<const uint32_t*>(MathsInternal::BIT_PATTERNS[aValue & 0xF]);
        ++string;
        *string = *reinterpret_cast<const uint32_t*>(MathsInternal::BIT_PATTERNS[aValue >> 4]);
        return aString + 8;
    }

    template<>
    char* ToBinary<uint16_t>(const uint16_t aValue, char* aString) throw() {
        return ToBinary<uint8_t>(aValue >> 8, ToBinary<uint8_t>(aValue & 0xFF, aString));
    }

    template<>
    char* ToBinary<uint32_t>(const uint32_t aValue, char* aString){
        return ToBinary<uint16_t>(aValue >> 16, ToBinary<uint16_t>(aValue & 0xFFFF, aString));
    }

    template<>
    char* ToBinary<uint64_t>(const uint64_t aValue, char* aString) throw() {
        return ToBinary<uint32_t>(aValue >> 32L, ToBinary<uint32_t>(aValue & 0xFFFFFFFFL, aString));
    }

    template<>
    char* ToBinary<int8_t>(const int8_t aValue, char* aString) throw() {
        return ToBinary<uint8_t>(BitwiseCast<uint8_t>(aValue), aString);
    }

    template<>
    char* ToBinary<int16_t>(const int16_t aValue, char* aString) throw() {
        return ToBinary<uint16_t>(BitwiseCast<uint16_t>(aValue), aString);
    }

    template<>
    char* ToBinary<int32_t>(const int32_t aValue, char* aString) throw() {
        return ToBinary<uint32_t>(BitwiseCast<uint32_t>(aValue), aString);
    }

    template<>
    char* ToBinary<int64_t>(const int64_t aValue, char* aString) throw() {
        return ToBinary<uint64_t>(BitwiseCast<uint64_t>(aValue), aString);
    }

    template<class T>
    static const char* FromBinary(T& aValue, const char* aString) throw() = delete;

    template<>
    const char* FromBinary<uint8_t>(uint8_t& aValue, const char* aString) throw() {
        aValue = (aString[0] == '1' ? 1 : 0) | (aString[1] == '1' ? 2 : 0) | (aString[2] == '1' ? 4 : 0) | (aString[3] == '1' ? 8 : 0);
        aString += 4;
        aValue |= (aString[0] == '1' ? 1 : 0) | (aString[1] == '1' ? 2 : 0) | (aString[2] == '1' ? 4 : 0) | (aString[3] == '1' ? 8 : 0) << 4;
        return aString + 4;
    }

    template<>
    const char* FromBinary<uint16_t>(uint16_t& aValue, const char* aString) throw() {
        return FromBinary<uint8_t>(*(reinterpret_cast<uint8_t*>(&aValue) + 1), FromBinary<uint8_t>(*reinterpret_cast<uint8_t*>(&aValue), aString));
    }

    template<>
    const char* FromBinary<uint32_t>(uint32_t& aValue, const char* aString) throw() {
        return FromBinary<uint16_t>(*(reinterpret_cast<uint16_t*>(&aValue) + 1), FromBinary<uint16_t>(*reinterpret_cast<uint16_t*>(&aValue), aString));
    }

    template<>
    const char* FromBinary<uint64_t>(uint64_t& aValue, const char* aString) throw() {
        return FromBinary<uint32_t>(*(reinterpret_cast<uint32_t*>(&aValue) + 1), FromBinary<uint32_t>(*reinterpret_cast<uint32_t*>(&aValue), aString));
    }

    template<>
    const char* FromBinary<int8_t>(int8_t& aValue, const char* aString) throw() {
        return FromBinary<uint8_t>(reinterpret_cast<uint8_t&>(aValue), aString);
    }

    template<>
    const char* FromBinary<int16_t>(int16_t& aValue, const char* aString) throw() {
        return FromBinary<uint16_t>(reinterpret_cast<uint16_t&>(aValue), aString);
    }

    template<>
    const char* FromBinary<int32_t>(int32_t& aValue, const char* aString) throw() {
        return FromBinary<uint32_t>(reinterpret_cast<uint32_t&>(aValue), aString);
    }

    template<>
    const char* FromBinary<int64_t>(int64_t& aValue, const char* aString) throw() {
        return FromBinary<uint64_t>(reinterpret_cast<uint64_t&>(aValue), aString);
    }

    // Conditions

    template<class T>
    static constexpr bool IsPow2(const T aValue) throw() {
        return aValue == 0 ? false : (aValue & (aValue - 1)) == 0;
    }

    // Rounding

    template<class T>
    static constexpr T CeilToClosestMultiple(const T aValue, const T aMultiple) throw() {
        return ((aValue + aMultiple - 1) / aMultiple) * aMultiple;
    }

    template<class T>
    static constexpr T CeilToClosestMultiplePow2(const T aValue, const T aMultiple) throw() {
        return (aValue + aMultiple - 1) & ~(aMultiple - 1);
    }

    template<class T>
    static constexpr T FloorToClosestMultiple(const T aValue, const T aMultiple) throw() {
        return aValue % aMultiple == 0 ? aValue : CeilToClosestMultiple<T>(aValue, aMultiple) - aMultiple;
    }

    template<class T>
    static constexpr T FloorToClosestMultiplePow2(const T aValue, const T aMultiple) throw() {
        return aValue & (aMultiple - 1) == 0 ? aValue : CeilToClosestMultiple<T>(aValue, aMultiple) - aMultiple;
    }

    template<class T>
    static constexpr T RoundToClosestMultiple(const T aValue, const T aMultiple) throw() {
        return CeilToClosestMultiple<T>(aValue, aMultiple) - (((aValue % aMultiple) < 5) && ((aValue % aMultiple) != 0) ? 5 : 0);
    }

    template<class T>
    static constexpr T RoundToClosestMultiplePow2(const T aValue, const T aMultiple) throw() {
        return CeilToClosestMultiple<T>(aValue, aMultiple) - (((aValue & (aMultiple - 1)) < 5) && ((aValue & (aMultiple - 1)) != 0) ? 5 : 0);
    }
    // Misc

	template<class T>
    static constexpr T Lerp(const T aFirst, const T aSecond, const T aWeight) throw() {
        return (static_cast<T>(1) - aWeight) * aFirst + aWeight * aSecond;
    }

}}


#endif
