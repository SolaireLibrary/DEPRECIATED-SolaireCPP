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

#include "Value.hpp"

namespace Solaire{ namespace Encode{

	template<class T>
	struct Encoder<
		T,
		std::enable_if_t<
			std::is_same<T, bool>::value
		>
	>{
		typedef T Type;
		typedef T DecodeType;
		typedef const T EncodeType;

		static DecodeType Decode(const Value& aValue, Allocator&) {
			return aValue.GetBool();
		}

		static Value Encode(EncodeType aValue, Allocator& aAllocator) {
			return Value(aAllocator, aValue);
		}

	};

	template<class T>
	struct Encoder<
		T,
		std::enable_if_t<
			std::is_same<T, char>::value
		>
	>{
		typedef T Type;
		typedef T DecodeType;
		typedef const T EncodeType;

		static DecodeType Decode(const Value& aValue, Allocator&) {
			return aValue.GetChar();
		}

		static Value Encode(EncodeType aValue, Allocator& aAllocator) {
			return Value(aAllocator, aValue);
		}

	};

	template<class T>
	struct Encoder<
		T,
		std::enable_if_t<
			std::is_same<T, uint8_t>::value ||
			std::is_same<T, uint16_t>::value ||
			std::is_same<T, uint32_t>::value ||
			std::is_same<T, uint64_t>::value
		>
	>{
		typedef T Type;
		typedef T DecodeType;
		typedef const T EncodeType;

		static DecodeType Decode(const Value& aValue, Allocator&) {
			return static_cast<T>(aValue.GetUint());
		}

		static Value Encode(EncodeType aValue, Allocator& aAllocator) {
			return Value(aAllocator, aValue);
		}

	};

	template<class T>
	struct Encoder<
		T,
		std::enable_if_t<
			std::is_same<T, int8_t>::value ||
			std::is_same<T, int16_t>::value ||
			std::is_same<T, int32_t>::value ||
			std::is_same<T, int64_t>::value
		>
	> {
		typedef T Type;
		typedef T DecodeType;
		typedef const T EncodeType;

		static DecodeType Decode(const Value& aValue, Allocator&) {
			return static_cast<T>(aValue.GetInt());
		}

		static Value Encode(EncodeType aValue, Allocator& aAllocator) {
			return Value(aAllocator, aValue);
		}

	};

	template<class T>
	struct Encoder<
		T,
		std::enable_if_t<
			std::is_same<T, float>::value ||
			std::is_same<T, double>::value
		>
	> {
		typedef T Type;
		typedef T DecodeType;
		typedef const T EncodeType;

		static DecodeType Decode(const Value& aValue, Allocator&) {
			return static_cast<T>(aValue.GetDouble());
		}

		static Value Encode(EncodeType aValue, Allocator& aAllocator) {
			return Value(aAllocator, aValue);
		}

	};

}}
