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

#include <map>
#include <memory>
#include "..\DataStructures\DynamicArray.hpp"

#undef GetObject

namespace Solaire{ namespace Encode{

	class Value;

	typedef DynamicArray<Value> Array;
	typedef std::map<std::string, Value> Object;

	class Value{
	private:
		enum : uint8_t {
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
		Allocator* mAllocator;
		union{
			uint64_t mUint;
			int64_t mInt;
			double mDouble;
			Object* mObject;
			Array* mArray;
			std::string* mString;
		};
		uint8_t mType;
	public:
		Value(Allocator&);
		Value(const Value&);
		Value(Value&&);
		Value(Allocator&, const bool);
		Value(Allocator&, const uint8_t);
		Value(Allocator&, const uint16_t);
		Value(Allocator&, const uint32_t);
		Value(Allocator&, const uint64_t);
		Value(Allocator&, const int8_t);
		Value(Allocator&, const int16_t);
		Value(Allocator&, const int32_t);
		Value(Allocator&, const int64_t);
		Value(Allocator&, const float);
		Value(Allocator&, const double);
		Value(Allocator&, const std::string&);
		~Value();

		Value& operator=(const Value&);
		Value& operator=(Value&&);

		bool IsNull() const;
		bool IsBool() const;
		bool IsChar() const;
		bool IsUint() const;
		bool IsInt() const;
		bool IsDouble() const;
		bool IsString() const;
		bool IsArray() const;
		bool IsObject() const;

		bool GetBool() const;
		char GetChar() const;
		uint64_t GetUint() const;
		int64_t GetInt() const;
		double GetDouble() const;
		const std::string& GetString() const;
		Array& GetArray() const;
		Object& GetObject() const;

		void SetNull();
		void SetBool(const bool);
		void SetChar(const char);
		void SetUint(const uint64_t);
		void SetInt(const int64_t);
		void SetDouble(const double);
		std::string& SetString();
		Array& SetArray();
		Object& SetObject();
	};
}}

#endif
