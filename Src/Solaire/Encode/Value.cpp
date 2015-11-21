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

namespace Solaire{ namespace Encode{

	static String DEFAULT_STRING(DEFAULT_ALLOCATOR, "ERROR");
	static Array DEFAULT_ARRAY;
	static Object DEFAULT_OBJECT;

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
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_CHAR)
	{}

	Value::Value(const bool aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_BOOL)
	{}

	Value::Value(const uint8_t aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(const uint16_t aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(const uint32_t aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(const uint64_t aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(const int8_t aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mInt(static_cast<int64_t>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(const int16_t aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mInt(static_cast<int64_t>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(const int32_t aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mInt(static_cast<int64_t>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(const int64_t aValue) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mInt(static_cast<int64_t>(aValue)),
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
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_CHAR)
	{}

	Value::Value(Allocator& aAllocator, const bool aValue) throw() :
		mAllocator(aAllocator),
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_BOOL)
	{}

	Value::Value(Allocator& aAllocator, const uint8_t aValue) throw() :
		mAllocator(aAllocator),
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(Allocator& aAllocator, const uint16_t aValue) throw() :
		mAllocator(aAllocator),
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(Allocator& aAllocator, const uint32_t aValue) throw() :
		mAllocator(aAllocator),
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(Allocator& aAllocator, const uint64_t aValue) throw() :
		mAllocator(aAllocator),
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(Allocator& aAllocator, const int8_t aValue) throw() :
		mAllocator(aAllocator),
		mInt(static_cast<int64_t>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(Allocator& aAllocator, const int16_t aValue) throw() :
		mAllocator(aAllocator),
		mInt(static_cast<int64_t>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(Allocator& aAllocator, const int32_t aValue) throw() :
		mAllocator(aAllocator),
		mInt(static_cast<int64_t>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(Allocator& aAllocator, const int64_t aValue) throw() :
		mAllocator(aAllocator),
		mInt(static_cast<int64_t>(aValue)),
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
		switch(aOther.mType)
		{
		case TYPE_BOOL:
		case TYPE_CHAR:
		case TYPE_UINT:
			SetNull();
			mUint = aOther.mUint;
			break;
		case TYPE_INT:
			SetNull();
			mInt = aOther.mInt;
			break;
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
		const Value& ref = aOther;
		return operator=(ref);
	}

	bool Value::IsNull() const throw() {
		return mType == TYPE_NULL;
	}

	bool Value::IsBool() const throw() {
		return mType == TYPE_BOOL;
	}

	bool Value::IsChar() const throw() {
		return mType == TYPE_CHAR;
	}

	bool Value::IsUint() const throw() {
		return mType == TYPE_UINT || (mType == TYPE_INT && mInt >= 0) || (mType == TYPE_DOUBLE && mDouble >= 0.0 && mDouble == std::floor(mDouble));
	}

	bool Value::IsInt() const throw() {
		return mType == TYPE_UINT || mType == TYPE_INT || (mType == TYPE_DOUBLE && mDouble == std::floor(mDouble));
	}

	bool Value::IsDouble() const throw() {
		return mType == TYPE_UINT || mType == TYPE_INT || mType == TYPE_DOUBLE;
	}

	bool Value::IsString() const throw() {
		return mType == TYPE_STRING;
	}

	bool Value::IsArray() const throw() {
		return mType == TYPE_ARRAY;
	}

	bool Value::IsObject() const throw() {
		return mType == TYPE_OBJECT;
	}

	bool Value::GetBool() const throw() {
		switch(mType)
		{
		case TYPE_BOOL:
		case TYPE_CHAR:
		case TYPE_UINT:
			return static_cast<bool>(mUint);
		case TYPE_INT:
			return static_cast<bool>(mInt);
		case TYPE_DOUBLE:
			return static_cast<bool>(mDouble);
		default:
			return false;
		}
	}

	char Value::GetChar() const throw() {
		switch (mType)
		{
		case TYPE_BOOL:
		case TYPE_CHAR:
		case TYPE_UINT:
			return static_cast<char>(mUint);
		case TYPE_INT:
			return static_cast<char>(mInt);
		case TYPE_DOUBLE:
			return static_cast<char>(mDouble);
		default:
			return '\0';
		}
	}

	uint64_t Value::GetUint() const throw() {
		switch (mType)
		{
		case TYPE_BOOL:
		case TYPE_CHAR:
		case TYPE_UINT:
			return static_cast<uint64_t>(mUint);
		case TYPE_INT:
			return static_cast<uint64_t>(mInt);
		case TYPE_DOUBLE:
			return static_cast<uint64_t>(mDouble);
		default:
			return 0;
		}
	}

	int64_t Value::GetInt() const throw() {
		switch (mType)
		{
		case TYPE_BOOL:
		case TYPE_CHAR:
		case TYPE_UINT:
			return static_cast<int64_t>(mUint);
		case TYPE_INT:
			return static_cast<int64_t>(mInt);
		case TYPE_DOUBLE:
			return static_cast<int64_t>(mDouble);
		default:
			return 0;
		}
	}

	double Value::GetDouble() const throw() {
		switch (mType)
		{
		case TYPE_BOOL:
		case TYPE_CHAR:
		case TYPE_UINT:
			return static_cast<double>(mUint);
		case TYPE_INT:
			return static_cast<double>(mInt);
		case TYPE_DOUBLE:
			return static_cast<double>(mDouble);
		default:
			return 0.0;
		}
	}

	String& Value::GetString() throw() {
		switch (mType)
		{
		case TYPE_STRING:
			return *mString;
		default:
			return DEFAULT_STRING;
		}
	}

	const String& Value::GetString() const throw() {
		switch (mType)
		{
		case TYPE_STRING:
			return *mString;
		default:
			return DEFAULT_STRING;
		}
	}

	Array& Value::GetArray() throw() {
		switch (mType)
		{
		case TYPE_ARRAY:
			return *mArray;
		default:
			return DEFAULT_ARRAY;
		}
	}

	const Array& Value::GetArray() const throw() {
		switch (mType)
		{
		case TYPE_ARRAY:
			return *mArray;
		default:
			return DEFAULT_ARRAY;
		}
	}

	Object& Value::GetObject() throw() {
		switch (mType)
		{
		case TYPE_OBJECT:
			return *mObject;
		default:
			return DEFAULT_OBJECT;
		}
	}

	const Object& Value::GetObject() const throw() {
		switch (mType)
		{
		case TYPE_OBJECT:
			return *mObject;
		default:
			return DEFAULT_OBJECT;
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
		mUint = static_cast<uint64_t>(aValue);
	}

	void Value::SetChar(const char aValue) throw() {
		SetNull();
		mType = TYPE_CHAR;
		mUint = static_cast<uint64_t>(aValue);
	}

	void Value::SetUint(const uint64_t aValue) throw() {
		SetNull();
		mType = TYPE_UINT;
		mUint = aValue;
	}

	void Value::SetInt(const int64_t aValue) throw() {
		SetNull();
		mType = TYPE_INT;
		mInt = aValue;
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
