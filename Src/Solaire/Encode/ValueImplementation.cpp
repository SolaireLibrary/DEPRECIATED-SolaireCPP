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
#include "Solaire\Core\Encode\ValueImplementation.hpp"

namespace Solaire{ namespace Encode{

	// ValueImplementation

	ValueImplementation::ValueImplementation():
		mType(TYPE_NULL)
	{}

	ValueImplementation::~ValueImplementation() {
		SetNull();
	}

	bool ValueImplementation::IsNull() const {
		return mType == TYPE_NULL;
	}

	bool ValueImplementation::IsBool() const {
		return mType == TYPE_BOOL;
	}

	bool ValueImplementation::IsChar() const {
		return mType == TYPE_CHAR;
	}

	bool ValueImplementation::IsUint() const {
		return mType == TYPE_UINT;
	}

	bool ValueImplementation::IsInt() const {
		return mType == TYPE_INT;
	}

	bool ValueImplementation::IsDouble() const {
		return mType == TYPE_DOUBLE;
	}

	bool ValueImplementation::IsString() const {
		return mType == TYPE_STRING;
	}

	bool ValueImplementation::IsArray() const {
		return mType == TYPE_ARRAY;
	}

	bool ValueImplementation::IsObject() const {
		return mType == TYPE_OBJECT;
	}

	bool ValueImplementation::GetBool() const {
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
			throw std::runtime_error("Solaire::ValueImplementation : Could not convert value to bool");
		}
	}

	char ValueImplementation::GetChar() const {
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
			throw std::runtime_error("Solaire::ValueImplementation : Could not convert value to char");
		}
	}

	uint64_t ValueImplementation::GetUint() const {
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
			throw std::runtime_error("Solaire::ValueImplementation : Could not convert value to uint64_t");
		}
	}

	int64_t ValueImplementation::GetInt() const {
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
			throw std::runtime_error("Solaire::ValueImplementation : Could not convert value to int64_t");
		}
	}

	double ValueImplementation::GetDouble() const {
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
			throw std::runtime_error("Solaire::ValueImplementation : Could not convert value to double");
		}
	}

	const char* ValueImplementation::GetString() const {
		switch (mType)
		{
		case TYPE_STRING:
			return mString;
		default:
			throw std::runtime_error("Solaire::ValueImplementation : Could not convert value to string");
		}
	}

	uint32_t ValueImplementation::GetStringLength() const {
		switch (mType)
		{
		case TYPE_STRING:
			return std::strlen(mString);
		default:
			return 0;
		}
	}

	Array& ValueImplementation::GetArray() const {
		switch (mType)
		{
		case TYPE_ARRAY:
			return *mArray;
		default:
			throw std::runtime_error("Solaire::ValueImplementation : Could not convert value to Array");
		}
	}

	Object& ValueImplementation::GetObject() const {
		switch (mType)
		{
		case TYPE_OBJECT:
			return *mObject;
		default:
			throw std::runtime_error("Solaire::ValueImplementation : Could not convert value to Object");
		}
	}

	void ValueImplementation::SetNull() {
		switch(mType)
		{
		case TYPE_STRING:
			DestroyString(mString, std::strlen(mString));
			break;
		case TYPE_ARRAY:
			DestroyArray(mArray);
			break;
		case TYPE_OBJECT:
			DestroyObject(mObject);
			break;
		default:
			break;
		}
		mType = TYPE_NULL;
	}

	void ValueImplementation::SetBool(const bool aValue) {
		SetNull();
		mType = TYPE_BOOL;
		mUint = static_cast<uint64_t>(aValue);
	}

	void ValueImplementation::SetChar(const char aValue) {
		SetNull();
		mType = TYPE_CHAR;
		mUint = static_cast<uint64_t>(aValue);
	}

	void ValueImplementation::SetUint(const uint64_t aValue) {
		SetNull();
		mType = TYPE_UINT;
		mUint = aValue;
	}

	void ValueImplementation::SetInt(const int64_t aValue) {
		SetNull();
		mType = TYPE_INT;
		mInt = aValue;
	}

	void ValueImplementation::SetDouble(const double aValue) {
		SetNull();
		mType = TYPE_DOUBLE;
		mDouble = aValue;
	}

	void ValueImplementation::SetString(const char* const aValue) {
		SetNull();
		mType = TYPE_STRING;
		const uint32_t length = std::strlen(aValue);
		mString = CreateString(length);
		std::memcpy(mString, aValue, length);
		mString[length] = '\0';
	}

	void ValueImplementation::SetArray() {
		if(mType != TYPE_ARRAY) {
			SetNull();
			mType = TYPE_ARRAY;
			mArray = CreateArray();
		}
	}

	void ValueImplementation::SetObject() {
		if(mType != TYPE_OBJECT) {
			SetNull();
			mType = TYPE_OBJECT;
			mObject = CreateObject();
		}
	}

}}
