#ifndef SOLAIRE_ENCODE_VALUE_IMPLEMENTATION_HPP
#define SOLAIRE_ENCODE_VALUE_IMPLEMENTATION_HPP

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
	\file ValueImplementation.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 12th November 2015
	Last Modified	: 12th November 2015
*/

#include "Value.hpp"

namespace Solaire{ namespace Encode{

	class ValueImplementation : public Value {
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
		union{
			uint64_t mUint;
			int64_t mInt;
			double mDouble;
			Object* mObject;
			Array* mArray;
			char* mString;
		};
		uint8_t mType;
	protected:
		virtual Object* CreateObject() const = 0;
		virtual void DestroyObject(const Object*) const = 0;

		virtual Array* CreateArray() const = 0;
		virtual void DestroyArray(const Array*) const = 0;

		virtual char* CreateString(const uint32_t) const = 0;
		virtual void DestroyString(const char* const, const uint32_t) const = 0;
	public:
		ValueImplementation();
		virtual ~ValueImplementation();

		// Inherited from Value

		bool IsNull() const override;
		bool IsBool() const override;
		bool IsChar() const override;
		bool IsUint() const override;
		bool IsInt() const override;
		bool IsDouble() const override;
		bool IsString() const override;
		bool IsArray() const override;
		bool IsObject() const override;

		bool GetBool() const override;
		char GetChar() const override;
		uint64_t GetUint() const override;
		int64_t GetInt() const override;
		double GetDouble() const override;
		const char* GetString() const override;
		uint32_t GetStringLength() const override;
		Array& GetArray() const override;
		Object& GetObject() const override;

		void SetNull() override;
		void SetBool(const bool) override;
		void SetChar(const char) override;
		void SetUint(const uint64_t) override;
		void SetInt(const int64_t) override;
		void SetDouble(const double) override;
		void SetString(const char* const) override;
		void SetArray() override;
		void SetObject() override;
	};
}}

#endif
