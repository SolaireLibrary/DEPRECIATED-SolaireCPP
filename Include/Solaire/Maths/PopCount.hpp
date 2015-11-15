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
	\file PopCount.hpp
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

namespace Solaire{

    namespace Implementation{
        static constexpr uint8_t NYBBLE_POP_COUNTS[16]{
            0,	1,	1,	2,	1,	2,	2,	3,
            1,	2,	2,	3,	2,	3,	3,	4
        };
    }

	static constexpr uint8_t PopCount4(const uint8_t aValue) throw() {
		return Implementation::NYBBLE_POP_COUNTS[aValue];
	}

	static constexpr uint8_t PopCount8(const uint8_t aValue) throw() {
		return
			PopCount4(aValue >> 4) +
			PopCount4(aValue & NYBBLE_0);
    }

	static constexpr uint8_t PopCount16(const uint16_t aValue) throw() {
		return
            PopCount8(aValue >> 8) +
            PopCount8(aValue & BYTE_0);
    }

	static constexpr uint8_t PopCount32(const uint32_t aValue) throw() {
		return
			PopCount16(aValue >> 16) +
            PopCount16(aValue & SHORT_0);
    }

    static constexpr uint8_t PopCount64(const uint64_t aValue) throw() {
		return
			PopCount32(aValue >> 32) +
			PopCount32(aValue & INT_0);
    }
	
	template<class T>
	static constexpr T PopCount(const T aValue) throw();

	template<>
	static constexpr uint8_t PopCount<uint8_t>(const uint8_t aValue) throw() {
		return PopCount8(aValue);
	}

	template<>
	static constexpr uint16_t PopCount<uint16_t>(const uint16_t aValue) throw() {
		return PopCount16(aValue);
	}

	template<>
	static constexpr uint32_t PopCount<uint32_t>(const uint32_t aValue) throw() {
		return PopCount32(aValue);
	}

	template<>
	static constexpr uint64_t PopCount<uint64_t>(const uint64_t aValue) throw() {
		return PopCount64(aValue);
	}

	template<>
	static constexpr int8_t PopCount<int8_t>(const int8_t aValue) throw() {
		return *reinterpret_cast<const int8_t*>(
			PopCount8(*reinterpret_cast<const uint8_t*>(&aValue))
			);
	}

	template<>
	static constexpr int16_t PopCount<int16_t>(const int16_t aValue) throw() {
		return *reinterpret_cast<const int16_t*>(
			PopCount16(*reinterpret_cast<const uint16_t*>(&aValue))
			);
	}

	template<>
	static constexpr int32_t PopCount<int32_t>(const int32_t aValue) throw() {
		return *reinterpret_cast<const int32_t*>(
			PopCount32(*reinterpret_cast<const uint32_t*>(&aValue))
			);
	}

	template<>
	static constexpr int64_t PopCount<int64_t>(const int64_t aValue) throw() {
		return *reinterpret_cast<const int64_t*>(
			PopCount64(*reinterpret_cast<const uint64_t*>(&aValue))
			);
	}
}


#endif
