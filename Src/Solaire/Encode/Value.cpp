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

#include <stdexcept>
#include "Solaire\Encode\Value.hpp"

namespace Solaire{ namespace Encode{

	// Value

	Value::Value(Allocator& aAllocator):
		mAllocator(&aAllocator),
		mType(TYPE_NULL)
	{}

	Value::Value(const Value& aOther) :
		mAllocator(aOther.mAllocator),
		mType(TYPE_NULL)
	{
		operator=(aOther);
	}

	Value::Value(Value&& aOther) :
		mAllocator(aOther.mAllocator),
		mType(TYPE_NULL)
	{
		operator=(std::move(aOther));
	}

	Value::Value(Allocator& aAllocator, const bool aValue) :
		mAllocator(&aAllocator),
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_BOOL)
	{}

	Value::Value(Allocator& aAllocator, const uint8_t aValue) :
		mAllocator(&aAllocator),
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(Allocator& aAllocator, const uint16_t aValue) :
		mAllocator(&aAllocator),
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(Allocator& aAllocator, const uint32_t aValue) :
		mAllocator(&aAllocator),
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(Allocator& aAllocator, const uint64_t aValue) :
		mAllocator(&aAllocator),
		mUint(static_cast<uint64_t>(aValue)),
		mType(TYPE_UINT)
	{}

	Value::Value(Allocator& aAllocator, const int8_t aValue) :
		mAllocator(&aAllocator),
		mInt(static_cast<int64_t>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(Allocator& aAllocator, const int16_t aValue) :
		mAllocator(&aAllocator),
		mInt(static_cast<int64_t>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(Allocator& aAllocator, const int32_t aValue) :
		mAllocator(&aAllocator),
		mInt(static_cast<int64_t>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(Allocator& aAllocator, const int64_t aValue) :
		mAllocator(&aAllocator),
		mInt(static_cast<int64_t>(aValue)),
		mType(TYPE_INT)
	{}

	Value::Value(Allocator& aAllocator, const float aValue) :
		mAllocator(&aAllocator),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_DOUBLE)
	{}

	Value::Value(Allocator& aAllocator, const double aValue) :
		mAllocator(&aAllocator),
		mDouble(static_cast<double>(aValue)),
		mType(TYPE_DOUBLE)
	{}

	Value::Value(Allocator& aAllocator, const std::string& aValue) :
		mAllocator(&aAllocator),
		mType(TYPE_NULL)
	{
		SetString() = aValue;
	}

	Value::~Value() {
		SetNull();
	}

	Value& Value::operator=(const Value& aOther) {
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
			throw std::runtime_error("Solaire::Value : Assignment type unrecognised");
		}

		mType = aOther.mType;
		return *this;
	}

	Value& Value::operator=(Value&& aOther) {
		const Value& ref = aOther;
		return operator=(ref);
	}

	bool Value::IsNull() const {
		return mType == TYPE_NULL;
	}

	bool Value::IsBool() const {
		return mType == TYPE_BOOL;
	}

	bool Value::IsChar() const {
		return mType == TYPE_CHAR;
	}

	bool Value::IsUint() const {
		return mType == TYPE_UINT;
	}

	bool Value::IsInt() const {
		return mType == TYPE_INT;
	}

	bool Value::IsDouble() const {
		return mType == TYPE_DOUBLE;
	}

	bool Value::IsString() const {
		return mType == TYPE_STRING;
	}

	bool Value::IsArray() const {
		return mType == TYPE_ARRAY;
	}

	bool Value::IsObject() const {
		return mType == TYPE_OBJECT;
	}

	bool Value::GetBool() const {
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
			throw std::runtime_error("Solaire::Value : Could not convert value to bool");
		}
	}

	char Value::GetChar() const {
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
			throw std::runtime_error("Solaire::Value : Could not convert value to char");
		}
	}

	uint64_t Value::GetUint() const {
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
			throw std::runtime_error("Solaire::Value : Could not convert value to uint64_t");
		}
	}

	int64_t Value::GetInt() const {
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
			throw std::runtime_error("Solaire::Value : Could not convert value to int64_t");
		}
	}

	double Value::GetDouble() const {
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
			throw std::runtime_error("Solaire::Value : Could not convert value to double");
		}
	}

	ConstStringFragment Value::GetString() const {
		switch (mType)
		{
		case TYPE_STRING:
			return *mString;
		default:
			throw std::runtime_error("Solaire::Value : Could not convert value to string");
		}
	}

	const Array& Value::GetArray() const {
		switch (mType)
		{
		case TYPE_ARRAY:
			return *mArray;
		default:
			throw std::runtime_error("Solaire::Value : Could not convert value to Array");
		}
	}

	const Object& Value::GetObject() const {
		switch (mType)
		{
		case TYPE_OBJECT:
			return *mObject;
		default:
			throw std::runtime_error("Solaire::Value : Could not convert value to Object");
		}
	}

	void Value::SetNull() {
		switch(mType)
		{
		case TYPE_STRING:
			mString->~String();
			mAllocator->Deallocate(mString, sizeof(std::string));
			break;
		case TYPE_ARRAY:
			mArray->~DynamicArray();
			mAllocator->Deallocate(mString, sizeof(Array));
			break;
		case TYPE_OBJECT:
			mObject->~map();
			mAllocator->Deallocate(mString, sizeof(Object));
			break;
		default:
			break;
		}
		mType = TYPE_NULL;
	}

	void Value::SetBool(const bool aValue) {
		SetNull();
		mType = TYPE_BOOL;
		mUint = static_cast<uint64_t>(aValue);
	}

	void Value::SetChar(const char aValue) {
		SetNull();
		mType = TYPE_CHAR;
		mUint = static_cast<uint64_t>(aValue);
	}

	void Value::SetUint(const uint64_t aValue) {
		SetNull();
		mType = TYPE_UINT;
		mUint = aValue;
	}

	void Value::SetInt(const int64_t aValue) {
		SetNull();
		mType = TYPE_INT;
		mInt = aValue;
	}

	void Value::SetDouble(const double aValue) {
		SetNull();
		mType = TYPE_DOUBLE;
		mDouble = aValue;
	}

	String& Value::SetString() {
		if(mType != TYPE_STRING) {
			SetNull();
			mType = TYPE_STRING;
			mString = new(mAllocator->Allocate(sizeof(std::string))) String(*mAllocator);
		}else {
			mString->Clear();
		}
		return *mString;
	}

	Array& Value::SetArray() {
		if(mType != TYPE_ARRAY) {
			SetNull();
			mType = TYPE_ARRAY;
			mArray = new(mAllocator->Allocate(sizeof(Array))) Array(*mAllocator);
		}else {
			mArray->Clear();
		}
		return *mArray;
	}

	Object& Value::SetObject() {
		if(mType != TYPE_OBJECT) {
			SetNull();
			mType = TYPE_OBJECT;
			mObject = new(mAllocator->Allocate(sizeof(Object))) Object();
		}else {
			mObject->clear();
		}
		return *mObject;
	}

}}
