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

#include "Solaire\Encode\Object.hpp"

namespace Solaire{ namespace Encode{

	// Object 
	
	Object::Object() throw() :
		mNames(GetDefaultAllocator()),
		mValues(GetDefaultAllocator())
	{}

	Object::Object(Allocator& aAllocator) throw() :
		mNames(aAllocator),
		mValues(aAllocator)
	{}

	uint32_t Object::Size() const {
		return mNames.Size();
	}

	const ConstString<char>& Object::GetMemberName(const uint32_t aIndex) const throw() {
		return mNames[aIndex];
	}

	const Value& Object::operator[](const uint32_t aIndex) const throw() {
		return mValues[aIndex];
	}

	Value& Object::operator[](const uint32_t aIndex) throw() {
		return mValues[aIndex];
	}

	bool Object::ContainsValue(const ConstString<char>& aName) const throw() {
		const uint32_t size = mNames.Size();
		for(uint32_t i = 0; i < size; ++i){
			if(mNames[i] == aName) return true;
		}
		return false;
	}

	const Value& Object::operator[](const ConstString<char>& aName) const {
		const uint32_t size = mNames.Size();
		for(uint32_t i = 0; i < size; ++i){
			if(mNames[i] == aName) return mValues[i];
		}
		return mValues[-1];
	}

	Value& Object::operator[](const ConstString<char>& aName) throw() {
		const uint32_t size = mNames.Size();
		for(uint32_t i = 0; i < size; ++i) {
			if(mNames[i] == aName) return mValues[i];
		}
		return mValues[-1];
	}

	Value& Object::Add(const ConstString<char>& aName, const Value& aValue) throw() {
		const uint32_t size = mNames.Size();
		for(uint32_t i = 0; i < size; ++i) {
			if(mNames[i] == aName) {
				return mValues[i] = aValue;
			}
		}
		mNames.PushBack(CString(mNames.GetAllocator(), aName));
		return mValues.PushBack(aValue);
	}

	void Object::Erase(const uint32_t aIndex) throw() {
		mNames.Erase(mNames.begin() + aIndex);
		mNames.Erase(mNames.end() + aIndex);
	}

	void Object::Erase(const ConstString<char>& aName) throw() {
		const uint32_t size = mNames.Size();
		for(uint32_t i = 0; i < size; ++i) {
			if(mNames[i] == aName) {
				Erase(i);
				return;
			}
		}
	}

	void Object::Clear() throw() {
		mNames.Clear();
		mValues.Clear();
	}

	Value& Object::Add(const ConstString<char>& aName) throw() {
		return Add(aName, Value(mNames.GetAllocator()));
	}

	Value& Object::Add(const ConstString<char>& aName, const char aValue) throw() {
		return Add(aName, Value(mNames.GetAllocator(), aValue));
	}

	Value& Object::Add(const ConstString<char>& aName, const bool aValue) throw() {
		return Add(aName, Value(mNames.GetAllocator(), aValue));
	}

	Value& Object::Add(const ConstString<char>& aName, const uint8_t aValue) throw() {
		return Add(aName, Value(mNames.GetAllocator(), aValue));
	}

	Value& Object::Add(const ConstString<char>& aName, const uint16_t aValue) throw() {
		return Add(aName, Value(mNames.GetAllocator(), aValue));
	}

	Value& Object::Add(const ConstString<char>& aName, const uint32_t aValue) throw() {
		return Add(aName, Value(mNames.GetAllocator(), aValue));
	}

	Value& Object::Add(const ConstString<char>& aName, const uint64_t aValue) throw() {
		return Add(aName, Value(mNames.GetAllocator(), aValue));
	}

	Value& Object::Add(const ConstString<char>& aName, const int8_t aValue) throw() {
		return Add(aName, Value(mNames.GetAllocator(), aValue));
	}

	Value& Object::Add(const ConstString<char>& aName, const int16_t aValue) throw() {
		return Add(aName, Value(mNames.GetAllocator(), aValue));
	}

	Value& Object::Add(const ConstString<char>& aName, const int32_t aValue) throw() {
		return Add(aName, Value(mNames.GetAllocator(), aValue));
	}

	Value& Object::Add(const ConstString<char>& aName, const int64_t aValue) throw() {
		return Add(aName, Value(mNames.GetAllocator(), aValue));
	}

	Value& Object::Add(const ConstString<char>& aName, const float aValue) throw() {
		return Add(aName, Value(mNames.GetAllocator(), aValue));
	}

	Value& Object::Add(const ConstString<char>& aName, const double aValue) throw() {
		return Add(aName, Value(mNames.GetAllocator(), aValue));
	}

	Value& Object::Add(const ConstString<char>& aName, const ConstString<char>& aValue) throw() {
		return Add(aName, Value(mNames.GetAllocator(), aValue));
	}

}}
