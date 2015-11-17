#ifndef SOLAIRE_INIT_INL
#define SOLAIRE_INIT_INL

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
	\file Init.inl
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 10th November 2015
	Last Modified	: 10th November 2015
*/

#include <type_traits>

namespace Solaire{

	template<class T>
	struct TypeTraits<T, typename std::enable_if<
		std::is_arithmetic<T>::value ||
		std::is_same<T, bool>::value ||
		std::is_same<T, char>::value
	>::type> {
		typedef T Type;
		typedef const T ConstType;
		typedef T PassType;
		typedef const T ConstPassType;

		static constexpr TypeTraitData GetData(){
			return TypeTraitData::GetData<T>(false);
		}
	};

	template<class T>
	struct TypeTraits<T, typename std::enable_if<
		std::is_pointer<T>::value
	>::type> {
		typedef T Type;
		typedef const T ConstType;
		typedef T PassType;
		typedef const T ConstPassType;

		static constexpr TypeTraitData GetData() {
			return TypeTraitData::GetData<T>(false);
		}
	};

	template<class T>
	static constexpr bool LessThan(const T aFirst, const T aSecond) {
		return aFirst < aSecond;
	}

	template<class T>
	static constexpr bool LessThanEquals(const T aFirst, const T aSecond) {
		return aFirst <= aSecond;
	}

	template<class T>
	static constexpr bool GreaterThan(const T aFirst, const T aSecond) {
		return aFirst > aSecond;
	}

	template<class T>
	static constexpr bool GreaterThanEquals(const T aFirst, const T aSecond) {
		return aFirst >= aSecond;
	}

	template<const uint32_t BITS>
	struct BinaryContainerStruct<BITS, std::enable_if_t<BITS == 0>> {
		typedef uint8_t Type;
	};

	template<const uint32_t BITS>
	struct BinaryContainerStruct<BITS, std::enable_if_t<GreaterThan<uint32_t>(BITS, 0) && LessThanEquals<uint32_t>(BITS, 8)>> {
		typedef uint8_t Type;
	};

	template<const uint32_t BITS>
	struct BinaryContainerStruct<BITS, std::enable_if_t<GreaterThan<uint32_t>(BITS, 8) && LessThanEquals<uint32_t>(BITS, 16)>> {
		typedef uint16_t Type;
	};

	template<const uint32_t BITS>
	struct BinaryContainerStruct<BITS, std::enable_if_t<GreaterThan<uint32_t>(BITS, 16) && LessThanEquals<uint32_t>(BITS, 32)>> {
		typedef uint32_t Type;
	};

	template<const uint32_t BITS>
	struct BinaryContainerStruct<BITS, std::enable_if_t<GreaterThan<uint32_t>(BITS, 32) && LessThanEquals<uint32_t>(BITS, 64)>> {
		typedef uint64_t Type;
	};

	template<class A, class B>
	struct MaxClassContainer<A, B, std::enable_if_t<GreaterThan<uint32_t>(sizeof(A), sizeof(B))>> {
		typedef A Type;
	};

	template<class A, class B>
	struct MaxClassContainer<A, B, std::enable_if_t<LessThan<uint32_t>(sizeof(A), sizeof(B))>> {
		typedef B Type;
	};

	template<class A, class B>
	struct MinClassContainer<A, B, std::enable_if_t<GreaterThan<uint32_t>(sizeof(A), sizeof(B))>> {
		typedef B Type;
	};

	template<class A, class B>
	struct MinClassContainer<A, B, std::enable_if_t<LessThan<uint32_t>(sizeof(A), sizeof(B))>> {
		typedef A Type;
	};

}

#endif
