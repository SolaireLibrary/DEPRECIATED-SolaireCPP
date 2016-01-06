#ifndef SOLAIRE_VECTOR_ENCODER_HPP
#define SOLAIRE_VECTOR_ENCODER_HPP

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
	\file VectorEncoder.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 11th September 2015
	Last Modified	: 11th November 2015
*/

#include "..\Core\Encode\Encoder.hpp"
#include "..\Core\Encode\Value.hpp"
#include "..\Core\Memory\Allocator.hpp"
#include "Vector.hpp"

namespace Solaire{

	template<class T, const uint32_t LENGTH>
	struct Encode::Encoder<Vector<T, LENGTH>> {
		typedef Vector<T, LENGTH> Type;
		typedef Type DecodeType;
		typedef const Type EncodeType;

		static DecodeType Decode(const Value& aValue, Allocator& aAllocator) {
			SolaireRuntimeAssert(aValue.IsArray(), "Solaire::Vector-Encode : Value is not an array");
			const Encode::Array& _array = aValue.GetArray();
			SolaireRuntimeAssert(_array.Size() >= LENGTH, "Solaire::Vector-Encode : Array is too short");

			DecodeType tmp;
			for(uint32_t i = 0; i < LENGTH; ++i) {
				tmp[i] = Encode::Decode<T>(_array[i], aAllocator);
			}
			return tmp;
		}

		static Value Encode(EncodeType aVector, Allocator& aAllocator) {
			Value value(aAllocator);
			Encode::Array& _array = value.SetArray();

			for(uint32_t i = 0; i < LENGTH; ++i) {
				_array.PushBack(Encode::Encode<T>(aVector[i], aAllocator));
			}

			return value;
		}
	};

}

#endif
