#ifndef SOLAIRE_UTILITY_HASH_HPP
#define SOLAIRE_UTILITY_HASH_HPP

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
\file Hash.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 18th September 2015
Last Modified	: 18th September 2015
*/

#include <stdexcept>
#include <type_traits>
#include <cstdint>
#include <string>

namespace Solaire{ namespace Utility {

	typedef uint32_t(*HashFunction)(const void* const, const size_t);

	namespace HashFunctions{

		static uint32_t SDBM(const void* const aData, const size_t aDataLength){
			uint32_t hash = 0;
			const uint8_t* const data = reinterpret_cast<const uint8_t*>(aData);
			for(size_t i = 0; i < aDataLength; ++i){
				hash = data[i] + (hash << 6) + (hash << 16) - hash;
			}
			return hash;
		}

		static uint32_t DJB2(const void* const aData, const size_t aDataLength){
			uint32_t hash = 5381;
			const uint8_t* const data = reinterpret_cast<const uint8_t*>(aData);
			for (size_t i = 0; i < aDataLength; ++i){
				hash = (hash << 5) + hash + data[i];
			}
			return hash;
		}

		static uint32_t Addler32(const void* const aData, const size_t aDataLength){
			const uint8_t* const data = reinterpret_cast<const uint8_t*>(aData);
			uint32_t s1 = 1;
			uint32_t s2 = 0;

			for(size_t i = 0; i < aDataLength; ++i){
				s1 = (s1 + data[i]) % 65521;
				s2 = (s2 + s1) % 65521;
			}
			return (s2 << 16) | s1;
		}

		template<class T>
		static uint32_t Sum(const void* const aData, const size_t aDataLength){
			static_assert(std::is_integral<T>::value, "Solaire::Utility::HashFunctions::Sum<T> T must be an integer type");

			const float lengthCheck = static_cast<float>(aDataLength) / static_cast<float>(sizeof(T));
			if(lengthCheck != std::floor(lengthCheck)) throw std::runtime_error("Solaire::Utility::HashFunctions::Sum<T> Data length is not divisable by word length");

			const T* const data = reinterpret_cast<const T*>(aData);
			uint32_t hash = 0;

			for(size_t i = 0; i < aDataLength; ++i){
				hash += data[i];
			}
			return hash;
		}
	}

	static HashFunction DEFAULT_HASH_FUNCTION = HashFunctions::Addler32;

	template<class T>
	static uint32_t Hash(const T& aData, HashFunction aFunction = DEFAULT_HASH_FUNCTION) = delete;

	template<>
	uint32_t Hash<uint8_t>(const uint8_t& aData, HashFunction aFunction){
		return aFunction(&aData, sizeof(uint8_t));
	}

	template<>
	uint32_t Hash<uint16_t>(const uint16_t& aData, HashFunction aFunction){
		return aFunction(&aData, sizeof(uint16_t));
	}

	template<>
	uint32_t Hash<uint32_t>(const uint32_t& aData, HashFunction aFunction){
		return aFunction(&aData, sizeof(uint32_t));
	}

	template<>
	uint32_t Hash<uint64_t>(const uint64_t& aData, HashFunction aFunction){
		return aFunction(&aData, sizeof(uint64_t));
	}

	template<>
	uint32_t Hash<int8_t>(const int8_t& aData, HashFunction aFunction){
		return aFunction(&aData, sizeof(int8_t));
	}

	template<>
	uint32_t Hash<int16_t>(const int16_t& aData, HashFunction aFunction){
		return aFunction(&aData, sizeof(int16_t));
	}

	template<>
	uint32_t Hash<int32_t>(const int32_t& aData, HashFunction aFunction){
		return aFunction(&aData, sizeof(int32_t));
	}

	template<>
	uint32_t Hash<int64_t>(const int64_t& aData, HashFunction aFunction){
		return aFunction(&aData, sizeof(int64_t));
	}

	template<>
	uint32_t Hash<float>(const float& aData, HashFunction aFunction){
		return aFunction(&aData, sizeof(float));
	}

	template<>
	uint32_t Hash<double>(const double& aData, HashFunction aFunction){
		return aFunction(&aData, sizeof(double));
	}

	template<>
	uint32_t Hash<char>(const char& aData, HashFunction aFunction) {
		return aFunction(&aData, sizeof(char));
	}

	template<>
	uint32_t Hash<std::string>(const std::string& aData, HashFunction aFunction){
		return aFunction(aData.c_str(), aData.size());
	}
}}

#endif
