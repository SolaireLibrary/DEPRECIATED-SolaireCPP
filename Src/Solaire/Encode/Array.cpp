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

#include "Solaire\Encode\Array.hpp"

namespace Solaire{ namespace Encode{

	// Array

	Array::Array() throw() :
		mValues(GetDefaultAllocator())
	{}

	Array::Array(Allocator& aAllocator) throw() :
		mValues(aAllocator)
	{}

	uint32_t Array::Size() const throw() {
		return mValues.Size();
	}

	const Value& Array::operator[](const uint32_t aIndex) const throw() {
		return mValues[aIndex];
	}

	Value& Array::operator[](const uint32_t aIndex) throw() {
		return mValues[aIndex];
	}

	void Array::Clear() throw() {
		mValues.Clear();
	}

	Value& Array::Add(const Value& aValue) throw() {
		return mValues.PushBack(aValue);
	}

	void Array::Erase(const uint32_t aIndex) throw() {
		mValues.Erase(mValues.begin() + aIndex);
	}

	Value& Array::Add() throw() {
		return Add(Value(mValues.GetAllocator()));
	}

	Value& Array::Add(const char aValue) throw() {
		return Add(Value(mValues.GetAllocator(), aValue));
	}

	Value& Array::Add(const bool aValue) throw() {
		return Add(Value(mValues.GetAllocator(), aValue));
	}

	Value& Array::Add(const uint8_t aValue) throw() {
		return Add(Value(mValues.GetAllocator(), aValue));
	}
	
	Value& Array::Add(const uint16_t aValue) throw() {
		return Add(Value(mValues.GetAllocator(), aValue));
	}

	Value& Array::Add(const uint32_t aValue) throw() {
		return Add(Value(mValues.GetAllocator(), aValue));
	}

	Value& Array::Add(const uint64_t aValue) throw() {
		return Add(Value(mValues.GetAllocator(), aValue));
	}

	Value& Array::Add(const int8_t aValue) throw() {
		return Add(Value(mValues.GetAllocator(), aValue));
	}

	Value& Array::Add(const int16_t aValue) throw() {
		return Add(Value(mValues.GetAllocator(), aValue));
	}

	Value& Array::Add(const int32_t aValue) throw() {
		return Add(Value(mValues.GetAllocator(), aValue));
	}

	Value& Array::Add(const int64_t aValue) throw() {
		return Add(Value(mValues.GetAllocator(), aValue));
	}

	Value& Array::Add(const float aValue) throw() {
		return Add(Value(mValues.GetAllocator(), aValue));
	}

	Value& Array::Add(const double aValue) throw() {
		return Add(Value(mValues.GetAllocator(), aValue));
	}

	Value& Array::Add(const ConstString<char>& aValue) throw() {
		return Add(Value(mValues.GetAllocator(), aValue));
	}

}}
