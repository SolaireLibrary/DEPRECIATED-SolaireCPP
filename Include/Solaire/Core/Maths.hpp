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

namespace Solaire{

	enum : uint64_t {
		BIT_0		= 1,
		BIT_1		= BIT_0 << 1L,
		BIT_2		= BIT_1 << 1L,
		BIT_3		= BIT_2 << 1L,
		BIT_4		= BIT_3 << 1L,
		BIT_5		= BIT_4 << 1L,
		BIT_6		= BIT_5 << 1L,
		BIT_7		= BIT_6 << 1L,
		BIT_8		= BIT_7 << 1L,
		BIT_9		= BIT_8 << 1L,
		BIT_10		= BIT_9 << 1L,
		BIT_11		= BIT_10 << 1L,
		BIT_12		= BIT_11 << 1L,
		BIT_13		= BIT_12 << 1L,
		BIT_14		= BIT_13 << 1L,
		BIT_15		= BIT_14 << 1L,
		BIT_16		= BIT_15 << 1L,
		BIT_17		= BIT_16 << 1L,
		BIT_18		= BIT_17 << 1L,
		BIT_19		= BIT_18 << 1L,
		BIT_20		= BIT_19 << 1L,
		BIT_21		= BIT_20 << 1L,
		BIT_22		= BIT_21 << 1L,
		BIT_23		= BIT_22 << 1L,
		BIT_24		= BIT_23 << 1L,
		BIT_25		= BIT_24 << 1L,
		BIT_26		= BIT_25 << 1L,
		BIT_27		= BIT_26 << 1L,
		BIT_28		= BIT_27 << 1L,
		BIT_29		= BIT_28 << 1L,
		BIT_30		= BIT_29 << 1L,
		/*BIT_31		= BIT_30 << 1L,
		BIT_32		= BIT_31 << 1L,
		BIT_33		= BIT_32 << 1L,
		BIT_34		= BIT_33 << 1L,
		BIT_35		= BIT_34 << 1L,
		BIT_36		= BIT_35 << 1L,
		BIT_37		= BIT_36 << 1L,
		BIT_38		= BIT_37 << 1L,
		BIT_39		= BIT_38 << 1L,
		BIT_40		= BIT_39 << 1L,
		BIT_41		= BIT_40 << 1L,
		BIT_42		= BIT_41 << 1L,
		BIT_43		= BIT_42 << 1L,
		BIT_44		= BIT_43 << 1L,
		BIT_45		= BIT_44 << 1L,
		BIT_46		= BIT_45 << 1L,
		BIT_47		= BIT_46 << 1L,
		BIT_48		= BIT_47 << 1L,
		BIT_49		= BIT_48 << 1L,
		BIT_50		= BIT_49 << 1L,
		BIT_51		= BIT_50 << 1L,
		BIT_52		= BIT_51 << 1L,
		BIT_53		= BIT_52 << 1L,
		BIT_54		= BIT_53 << 1L,
		BIT_55		= BIT_54 << 1L,
		BIT_56		= BIT_55 << 1L,
		BIT_57		= BIT_56 << 1L,
		BIT_58		= BIT_57 << 1L,
		BIT_59		= BIT_58 << 1L,
		BIT_60		= BIT_59 << 1L,
		BIT_61		= BIT_60 << 1L,
		BIT_62		= BIT_61 << 1L,
		BIT_63		= BIT_62 << 1L,*/

		NYBBLE_0	= BIT_0 | BIT_1 | BIT_2 | BIT_3,
		NYBBLE_1	= NYBBLE_0 << 4L,
		NYBBLE_2	= NYBBLE_1 << 4L,
		NYBBLE_3	= NYBBLE_2 << 4L,
		NYBBLE_4	= NYBBLE_3 << 4L,
		NYBBLE_5	= NYBBLE_4 << 4L,
		NYBBLE_6	= NYBBLE_5 << 4L,
		/*NYBBLE_7	= NYBBLE_6 << 4L,
		NYBBLE_8	= NYBBLE_7 << 4L,
		NYBBLE_9	= NYBBLE_8 << 4L,
		NYBBLE_10	= NYBBLE_9 << 4L,
		NYBBLE_11	= NYBBLE_10 << 4L,
		NYBBLE_12	= NYBBLE_11 << 4L,
		NYBBLE_13	= NYBBLE_12 << 4L,
		NYBBLE_14	= NYBBLE_13 << 4L,
		NYBBLE_15	= NYBBLE_14 << 4L,*/

		BYTE_0		= NYBBLE_0 | NYBBLE_1,
		BYTE_1		= BYTE_0 << 8L,
		BYTE_2		= BYTE_1 << 8L,
		/*BYTE_3		= BYTE_2 << 8L,
		BYTE_4		= BYTE_3 << 8L,
		BYTE_5		= BYTE_4 << 8L,
		BYTE_6		= BYTE_5 << 8L,
		BYTE_7		= BYTE_6 << 8L,*/

		SHORT_0		= BYTE_0 | BYTE_1,
		/*SHORT_1		= SHORT_0 << 16L,
		SHORT_2		= SHORT_1 << 16L,
		SHORT_3		= SHORT_2 << 16L,*/

		INT_0		= UINT32_MAX, //SHORT_0 | SHORT_1,
		//INT_1		= SHORT_1 << 32L,

		//LONG_0		= INT_0 | INT_1
	};

	////

	static constexpr bool IsPowerOfTwo(const uint32_t aValue) throw() {
		return aValue == 0 ? false : (aValue & (aValue - 1)) == 0;
	}

	////

	template<class T>
	static constexpr T LinearInterpolationAccurate(const T aFirst, const T aSecond, const T aWeight) throw() {
		return (static_cast<T>(1) - aWeight) * aFirst + aWeight * aSecond;
	}

	template<class T>
	static constexpr T LinearInterpolationFast(const T aFirst, const T aSecond, const T aWeight) throw() {
		return aFirst + aWeight *(aSecond - aFirst);
	}

	////

	template<class T>
	static constexpr T CeilToMultiple(const T aValue, const T aMultiple) throw() {
		return ((aValue + aMultiple - 1) / aMultiple) * aMultiple;
	}

	template<class T>
	static constexpr T FloorToMultiple(const T aValue, const T aMultiple) throw() {
		return aValue % aMultiple == 0 ? aValue : CeilToMultiple<T>(aValue, aMultiple) - aMultiple;
	}

	template<class T>
	static constexpr T RoundToClosestMultiple(const T aValue, const T aMultiple) throw() {
		return CeilToMultiple<T>(aValue, aMultiple) - (((aValue % aMultiple) < 5) && ((aValue % aMultiple) != 0) ? 5 : 0);
	}

	////

	static constexpr uint64_t Set1(const uint32_t aBits) {
		return 1 | (Set1(aBits - 1) << 1);
	}

	/*



	namespace Maths{

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

    // Rounding
    // Misc

	*/

}


#endif
