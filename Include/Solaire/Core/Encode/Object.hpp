#ifndef SOLAIRE_ENCODE_OBJECT_HPP
#define SOLAIRE_ENCODE_OBJECT_HPP

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
	\file Object.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 12th November 2015
	Last Modified	: 12th November 2015
*/

#include <cstdint>
#include <algorithm>

namespace Solaire{ namespace Encode{

	class Value;

	class Object {
	public:
		virtual ~Object(){}

		virtual uint32_t Size() const = 0;

		virtual std::pair<const char*, const Value*> operator[](const uint32_t) const = 0;
		virtual std::pair<const char*, Value*> operator[](const uint32_t) = 0;

		virtual const Value& operator[](const char* const) const = 0;
		virtual Value& operator[](const char* const) = 0;

		virtual bool HasMember(const char* const) = 0;
		virtual Value& AddMember(const char* const) = 0;
	};
}}

#endif
