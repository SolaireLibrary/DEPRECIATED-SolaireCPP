#ifndef SOLAIRE_ENCODE_ARRAY_HPP
#define SOLAIRE_ENCODE_ARRAY_HPP

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
	\file Array.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 21st November 2015
	Last Modified	: 21st November 2015
*/

#include "..\DataStructures\DynamicArray.hpp"
#include "Value.hpp"

namespace Solaire{ namespace Encode{

	class Array{
	private:
		DynamicArray<Value> mValues;
	public:
		Array() throw();
		Array(Allocator&) throw();

		uint32_t Size() const throw();
		const Value& operator[](const uint32_t) const throw();
		Value& operator[](const uint32_t) throw();

		Value& Add(const Value&) throw();
		void Erase(const uint32_t) throw();

		void Clear() throw();

		Value& Add();
		Value& Add(const char) throw();
		Value& Add(const bool) throw();
		Value& Add(const uint8_t) throw();
		Value& Add(const uint16_t) throw();
		Value& Add(const uint32_t) throw();
		Value& Add(const uint64_t) throw();
		Value& Add(const int8_t) throw();
		Value& Add(const int16_t) throw();
		Value& Add(const int32_t) throw();
		Value& Add(const int64_t) throw();
		Value& Add(const float) throw();
		Value& Add(const double) throw();
		Value& Add(const ConstString<char>&) throw();
	};
}}

#endif
