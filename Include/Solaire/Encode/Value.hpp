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
	Last Modified	: 21st November 2015
*/

#include "..\Core\Init.hpp"
#include "..\Strings\String.hpp"
#undef GetObject

namespace Solaire{ namespace Encode{
	
	class Array;
	class Object;

	class Value{
	public:
		enum Type : uint8_t {
			TYPE_NULL,
			TYPE_BOOL,
			TYPE_CHAR,
			TYPE_UINT,
			TYPE_INT,
			TYPE_DOUBLE,
			TYPE_STRING,
			TYPE_ARRAY,
			TYPE_OBJECT
		};
	private:
		Allocator& mAllocator;
		union{
			double mDouble;
			Object* mObject;
			Array* mArray;
			String<char>* mString;
		};
		Type mType;
	public:
		Value(const Value&) throw();
		Value(Value&&) throw();

		Value() throw();
		Value(const char) throw();
		Value(const bool) throw();
		Value(const uint8_t) throw();
		Value(const uint16_t) throw();
		Value(const uint32_t) throw();
		Value(const uint64_t) throw();
		Value(const int8_t) throw();
		Value(const int16_t) throw();
		Value(const int32_t) throw();
		Value(const int64_t) throw();
		Value(const float) throw();
		Value(const double) throw();
		Value(const ConstString<char>&) throw();

		Value(Allocator&) throw();
		Value(Allocator&, const char) throw();
		Value(Allocator&, const bool) throw();
		Value(Allocator&, const uint8_t) throw();
		Value(Allocator&, const uint16_t) throw();
		Value(Allocator&, const uint32_t) throw();
		Value(Allocator&, const uint64_t) throw();
		Value(Allocator&, const int8_t) throw();
		Value(Allocator&, const int16_t) throw();
		Value(Allocator&, const int32_t) throw();
		Value(Allocator&, const int64_t) throw();
		Value(Allocator&, const float) throw();
		Value(Allocator&, const double) throw();
		Value(Allocator&, const ConstString<char>&) throw();

		~Value() throw();

		Value& operator=(const Value&) throw();
		Value& operator=(Value&&) throw();

		Type GetType() const throw();

		bool IsNull() const throw();
		bool IsBool() const throw();
		bool IsChar() const throw();
		bool IsUint() const throw();
		bool IsInt() const throw();
		bool IsDouble() const throw();
		bool IsString() const throw();
		bool IsArray() const throw();
		bool IsObject() const throw();

		bool GetBool() const throw();
		char GetChar() const throw();
		uint64_t GetUint() const throw();
		int64_t GetInt() const throw();
		double GetDouble() const throw();
		String<char>& GetString() throw();
		const ConstString<char>& GetString() const throw();
		const Array& GetArray() const throw();
		const Object& GetObject() const throw();
		Array& GetArray() throw();
		Object& GetObject() throw();

		void SetNull() throw();
		void SetBool(const bool) throw();
		void SetChar(const char) throw();
		void SetUint(const uint64_t) throw();
		void SetInt(const int64_t) throw();
		void SetDouble(const double) throw();
		String<char>& SetString() throw();
		Array& SetArray() throw();
		Object& SetObject() throw();
	};
}}

#endif
