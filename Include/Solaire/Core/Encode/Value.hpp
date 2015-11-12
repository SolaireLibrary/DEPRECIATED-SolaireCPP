#ifndef SOLAIRE_ENCODE_VALUE_HPP
#define SOLAIRE_ENCODE_VALUE_HPP

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
	\file Value.hpp
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

namespace Solaire{ namespace Encode{

	class Array;
	class Object;

	class Value {
	public:
		virtual ~Value(){}

		virtual bool IsNull() const = 0;
		virtual bool IsBool() const = 0;
		virtual bool IsChar() const = 0;
		virtual bool IsUint() const = 0;
		virtual bool IsInt() const = 0;
		virtual bool IsDouble() const = 0;
		virtual bool IsString() const = 0;
		virtual bool IsArray() const = 0;
		virtual bool IsObject() const = 0;

		virtual bool GetBool() const = 0;
		virtual char GetChar() const = 0;
		virtual uint64_t GetUint() const = 0;
		virtual int64_t GetInt() const = 0;
		virtual double GetDouble() const = 0;
		virtual const char* GetString() const = 0;
		virtual uint32_t GetStringLength() const = 0;
		virtual Array& GetArray() const = 0;
		virtual Object& GetObject() const = 0;

		virtual void SetNull() = 0;
		virtual void SetBool(const bool) = 0;
		virtual void SetChar(const char) = 0;
		virtual void SetUint(const uint64_t) = 0;
		virtual void SetInt(const int64_t) = 0;
		virtual void SetDouble(const double) = 0;
		virtual const char* SetString(const char* const) = 0;
		virtual Array& SetArray() = 0;
		virtual Object& SetObject() = 0;
	};
}}

#endif
