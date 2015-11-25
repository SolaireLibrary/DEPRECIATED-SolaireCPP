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

#include "Solaire\Encode\Value.hpp"
#include "Solaire\Encode\Array.hpp"
#include "Solaire\Encode\Object.hpp"
#include "Solaire\Memory\DefaultAllocator.hpp"
#include "Solaire\Strings\NumberParser.hpp"

namespace Solaire{ namespace Encode{

	// Value

	Value::Value(const Value& aOther) throw() :
		mAllocator(aOther.mAllocator),
		mType(TYPE_NULL)
	{
		operator=(aOther);
	}

	Value::Value(Value&& aOther) throw() :
		mAllocator(aOther.mAllocator),
		mType(TYPE_NULL)
	{
		operator=(std::move(aOther));
	}

	Value::Value() throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mType(TYPE_NULL)
	{}

	Value::Value(const char aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_CHAR)
	{}

	Value::Value(const bool aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_BOOL)
	{}

	Value::Value(const uint8_t aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(const uint16_t aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(const uint32_t aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(const uint64_t aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(const int8_t aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(const int16_t aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(const int32_t aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(const int64_t aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(const float aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_DOUBLE)
	{}

	Value::Value(const double aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_DOUBLE)
	{}

	Value::Value(const String& aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mType(TYPE_NULL)
	{
		SetString() = aValue;
	}

	Value::Value(Allocator& aAllocator) throw() :
		mAllocator(aAllocator),
		mType(TYPE_NULL)
	{}

	Value::Value(Allocator& aAllocator, const char aValue) throw() :
		mAllocator(aAllocator),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_CHAR)
	{}

	Value::Value(Allocator& aAllocator, const bool aValue) throw() :
		mAllocator(aAllocator),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_BOOL)
	{}

	Value::Value(Allocator& aAllocator, const uint8_t aValue) throw() :
		mAllocator(aAllocator),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(Allocator& aAllocator, const uint16_t aValue) throw() :
		mAllocator(aAllocator),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(Allocator& aAllocator, const uint32_t aValue) throw() :
		mAllocator(aAllocator),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(Allocator& aAllocator, const uint64_t aValue) throw() :
		mAllocator(aAllocator),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(Allocator& aAllocator, const int8_t aValue) throw() :
		mAllocator(aAllocator),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(Allocator& aAllocator, const int16_t aValue) throw() :
		mAllocator(aAllocator),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(Allocator& aAllocator, const int32_t aValue) throw() :
		mAllocator(aAllocator),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(Allocator& aAllocator, const int64_t aValue) throw() :
		mAllocator(aAllocator),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(Allocator& aAllocator, const float aValue) throw() :
		mAllocator(aAllocator),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_DOUBLE)
	{}

	Value::Value(Allocator& aAllocator, const double aValue) throw() :
		mAllocator(aAllocator),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_DOUBLE)
	{}

	Value::Value(Allocator& aAllocator, const String& aValue) throw() :
		mAllocator(aAllocator),
		mType(TYPE_NULL)
	{
		SetString() = aValue;
	}

	Value::~Value() throw() {
		SetNull();
	}

	Value& Value::operator=(const Value& aOther) throw() {
		switch(aOther.mType) {
		case TYPE_BOOL:
		case TYPE_CHAR:
		case TYPE_UINT:
		case TYPE_DOUBLE:
			SetNull();
			mDouble = aOther.mDouble;
			break;
		case TYPE_STRING:
			SetString() = *aOther.mString;
			break;
		case TYPE_ARRAY:
			SetArray() = *aOther.mArray;
			break;
		case TYPE_OBJECT:
			SetObject() = *aOther.mObject;
			break;
		default:
			SetNull();
		}

		mType = aOther.mType;
		return *this;
	}

	Value& Value::operator=(Value&& aOther) throw() {
		
		if(&mAllocator != &aOther.mAllocator) {
			return operator=(static_cast<const Value&>(aOther));
		}

		SetNull();
		
		switch(aOther.mType) {
		case TYPE_BOOL:
		case TYPE_CHAR:
		case TYPE_UINT:
		case TYPE_INT:
		case TYPE_DOUBLE:
			mDouble = aOther.mDouble;
			break;
		case TYPE_STRING:
		case TYPE_ARRAY:
		case TYPE_OBJECT:
			mString = aOther.mString;
			break;
		default:
			return *this;
		}

		mType = aOther.mType;
		aOther.mType = TYPE_NULL;
		return *this;
	}

	Value::Type Value::GetType() const throw() {
		return mType;
	}

	bool Value::IsNull() const throw() {
		switch(mType) {
		case TYPE_NULL:
			return true;
		case TYPE_STRING:
			return mString->Size() == 0 || *mString == "null";
		default:
			return false;
		}
	}

	bool Value::IsBool() const throw() {
		switch(mType) {
		case TYPE_BOOL:
			return true;
		case TYPE_UINT:
		case TYPE_INT:
		case TYPE_DOUBLE:
			return mDouble >= 0.0 && mDouble == std::floor(mDouble);
		case TYPE_STRING:
			return *mString == "true" || *mString == "false";
		default:
			return false;
		}
	}

	bool Value::IsChar() const throw() {
		switch (mType) {
		case TYPE_BOOL:
		case TYPE_CHAR:
			return true;
		case TYPE_UINT:
		case TYPE_INT:
		case TYPE_DOUBLE:
			return mDouble >= 0.0 && mDouble <= 9.0 && mDouble == std::floor(mDouble);
		case TYPE_STRING:
			return mString->Size() == 1;
		default:
			return false;
		}
	}

	bool Value::IsUint() const throw() {
		switch(mType) {
		case TYPE_BOOL:
			return true;
		case TYPE_CHAR:
			{
				const char c = static_cast<char>(mDouble);
				return c >= '0' && c <= '9';
			}
		case TYPE_UINT:
			return true;
		case TYPE_INT:
		case TYPE_DOUBLE:
			return mDouble >= 0.0 && mDouble == std::floor(mDouble);
		case TYPE_STRING:
			{
				const char* const begin = mString->CString();
				const char* const end = begin + mString->Size();
				uint32_t value;
				const char* const tmp = ReadNumber<uint32_t>(begin, end, value);
				if(begin != end && tmp == end) {
					const_cast<Value*>(this)->SetUint(value);
					return true;
				}else {
					return false;
				}
			}
		default:
			return false;
		}
	}

	bool Value::IsInt() const throw() {
		switch(mType) {
		case TYPE_BOOL:
			return true;
		case TYPE_CHAR:
			{
				const char c = static_cast<char>(mDouble);
				return c >= '0' && c <= '9';
			}
		case TYPE_UINT:
		case TYPE_INT:
			return true;
		case TYPE_DOUBLE:
			return mDouble == std::floor(mDouble);
		case TYPE_STRING:
			{
				const char* const begin = mString->CString();
				const char* const end = begin + mString->Size();
				int32_t value;
				const char* const tmp = ReadNumber<int32_t>(begin, end, value);
				if(begin != end && tmp == end) {
					const_cast<Value*>(this)->SetInt(value);
					return true;
				}else {
					return false;
				}
			}
		default:
			return false;
		}
	}

	bool Value::IsDouble() const throw() {
		switch(mType) {
		case TYPE_BOOL:
			return true;
		case TYPE_CHAR:
			{
				const char c = static_cast<char>(mDouble);
				return c >= '0' && c <= '9';
			}
		case TYPE_UINT:
		case TYPE_INT:
		case TYPE_DOUBLE:
			return true;
		case TYPE_STRING:
			{
				const char* const begin = mString->CString();
				const char* const end = begin + mString->Size();
				double value;
				const char* const tmp = ReadNumber<double>(begin, end, value);
				if(begin != end && tmp == end) {
					const_cast<Value*>(this)->SetDouble(value);
					return true;
				}else {
					return false;
				}
			}
		default:
			return false;
		}
	}

	bool Value::IsString() const throw() {
		switch(mType) {
		case TYPE_BOOL:
		case TYPE_CHAR:
		case TYPE_UINT:
		case TYPE_INT:
		case TYPE_DOUBLE:
		case TYPE_STRING:
			return true;
		default:
			return false;
		}
	}

	bool Value::IsArray() const throw() {
		return mType == TYPE_ARRAY;
	}

	bool Value::IsObject() const throw() {
		return mType == TYPE_OBJECT;
	}

	bool Value::GetBool() const throw() {
		switch(mType) {
		case TYPE_BOOL:
		case TYPE_UINT:
		case TYPE_INT:
		case TYPE_DOUBLE:
			return mDouble > 0.0;
		case TYPE_STRING:
			return *mString == "true";
		default:
			return false;
		}
	}

	char Value::GetChar() const throw() {
		switch(mType) {
		case TYPE_BOOL:
			return mDouble > 0.0 ? '1' : '0';
		case TYPE_CHAR:
			return static_cast<char>(mDouble);
		case TYPE_UINT:
		case TYPE_INT:
		case TYPE_DOUBLE:
			return mDouble >= 0.0 && mDouble <= 9.0 && mDouble == std::floor(mDouble) ? '0' + static_cast<char>(mDouble) : '\0';
		case TYPE_STRING:
			return mString->Size() == 1 ? mString->operator[](0) : '\0';
		default:
			return '\0';
		}
	}

	uint64_t Value::GetUint() const throw() {
		switch(mType) {
		case TYPE_CHAR:
			{
				const char c = static_cast<char>(mDouble);
				return c >= '0' && c <= '9' ? static_cast<uint64_t>(c - '0') : 0;
			}
		case TYPE_BOOL:
		case TYPE_UINT:
		case TYPE_INT:
		case TYPE_DOUBLE:
			return static_cast<uint32_t>(mDouble);
		case TYPE_STRING:
			{
				const char* const begin = mString->CString();
				const char* const end = begin + mString->Size();
				uint32_t value;
				const char* const tmp = ReadNumber<uint32_t>(begin, end, value);
				if(begin != end && tmp == end) {
					const_cast<Value*>(this)->SetUint(value);
					return value;
				}else {
					return 0;
				}
			}
		default:
			return 0;
		}
	}

	int64_t Value::GetInt() const throw() {
		switch(mType) {
		case TYPE_CHAR:
			{
				const char c = static_cast<char>(mDouble);
				return c >= '0' && c <= '9' ? static_cast<int64_t>(c - '0') : 0;
			}
		case TYPE_BOOL:
		case TYPE_UINT:
		case TYPE_INT:
		case TYPE_DOUBLE:
			return static_cast<int32_t>(mDouble);
		case TYPE_STRING:
			{
				const char* const begin = mString->CString();
				const char* const end = begin + mString->Size();
				int32_t value;
				const char* const tmp = ReadNumber<int32_t>(begin, end, value);
				if(begin != end && tmp == end) {
					const_cast<Value*>(this)->SetInt(value);
					return value;
				}else {
					return 0;
				}
			}
		default:
			return 0;
		}
	}

	double Value::GetDouble() const throw() {
		switch(mType) {
		case TYPE_CHAR:
			{
				const char c = static_cast<char>(mDouble);
				return c >= '0' && c <= '9' ? static_cast<double>(c - '0') : 0;
			}
		case TYPE_BOOL:
		case TYPE_UINT:
		case TYPE_INT:
		case TYPE_DOUBLE:
			return static_cast<uint32_t>(mDouble);
		case TYPE_STRING:
			{
				const char* const begin = mString->CString();
				const char* const end = begin + mString->Size();
				double value;
				const char* const tmp = ReadNumber<double>(begin, end, value);
				if(begin != end && tmp == end) {
					const_cast<Value*>(this)->SetDouble(value);
					return value;
				}else {
					return 0;
				}
			}
		default:
			return 0;
		}
	}

	String& Value::GetString() throw() {
		switch(mType) {
		case TYPE_CHAR:
			{
				const char c = static_cast<char>(mDouble);
				SetString() += c;
				return *mString;
			}
		case TYPE_BOOL:
		case TYPE_UINT:
		case TYPE_INT:
		case TYPE_DOUBLE:
			{
				const double num = mDouble;
				SetString() += num;
				return *mString;
			}
		case TYPE_STRING:
			return *mString;
		default:
			return *static_cast<String*>(nullptr);
		}
	}

	const String& Value::GetString() const throw() {
		return const_cast<Value*>(this)->GetString();
	}

	Array& Value::GetArray() throw() {
		switch (mType)
		{
		case TYPE_ARRAY:
			return *mArray;
		default:
			return *static_cast<Array*>(nullptr);;
		}
	}

	const Array& Value::GetArray() const throw() {
		switch (mType)
		{
		case TYPE_ARRAY:
			return *mArray;
		default:
			return *static_cast<Array*>(nullptr);
		}
	}

	Object& Value::GetObject() throw() {
		switch (mType)
		{
		case TYPE_OBJECT:
			return *mObject;
		default:
			return *static_cast<Object*>(nullptr);
		}
	}

	const Object& Value::GetObject() const throw() {
		switch (mType)
		{
		case TYPE_OBJECT:
			return *mObject;
		default:
			return *static_cast<Object*>(nullptr);
		}
	}

	void Value::SetNull() throw() {
		switch(mType)
		{
		case TYPE_STRING:
			mString->~String();
			mAllocator.Deallocate(mString);
			break;
		case TYPE_ARRAY:
			mArray->~Array();
			mAllocator.Deallocate(mString);
			break;
		case TYPE_OBJECT:
			mObject->~Object();
			mAllocator.Deallocate(mString);
			break;
		default:
			break;
		}
		mType = TYPE_NULL;
	}

	void Value::SetBool(const bool aValue) throw() {
		SetNull();
		mType = TYPE_BOOL;
		mDouble = static_cast<double>(aValue);
	}

	void Value::SetChar(const char aValue) throw() {
		SetNull();
		mType = TYPE_CHAR;
		mDouble = static_cast<double>(aValue);
	}

	void Value::SetUint(const uint64_t aValue) throw() {
		SetNull();
		mType = TYPE_UINT;
		mDouble = static_cast<double>(aValue);
	}

	void Value::SetInt(const int64_t aValue) throw() {
		SetNull();
		mType = TYPE_INT;
		mDouble = static_cast<double>(aValue);
	}

	void Value::SetDouble(const double aValue) throw() {
		SetNull();
		mType = TYPE_DOUBLE;
		mDouble = aValue;
	}

	String& Value::SetString() throw() {
		if(mType != TYPE_STRING) {
			SetNull();
			mType = TYPE_STRING;
			mString = new(mAllocator.Allocate(sizeof(std::string))) String(mAllocator);
		}else {
			mString->Clear();
		}
		return *mString;
	}

	Array& Value::SetArray() throw() {
		if(mType != TYPE_ARRAY) {
			SetNull();
			mType = TYPE_ARRAY;
			mArray = new(mAllocator.Allocate(sizeof(Array))) Array(mAllocator);
		}else {
			mArray->Clear();
		}
		return *mArray;
	}

	Object& Value::SetObject() throw() {
		if(mType != TYPE_OBJECT) {
			SetNull();
			mType = TYPE_OBJECT;
			mObject = new(mAllocator.Allocate(sizeof(Object))) Object(mAllocator);
		}else {
			mObject->Clear();
		}
		return *mObject;
	}

}}
