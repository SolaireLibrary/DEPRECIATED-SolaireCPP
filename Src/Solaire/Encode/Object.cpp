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
#include "Solaire\Memory\DefaultAllocator.hpp"

namespace Solaire{ namespace Encode{

	// Object 
	
	Object::Object() throw() :
		mAllocator(DEFAULT_ALLOCATOR)
	{}

	Object::Object(Allocator& aAllocator) throw() :
		mAllocator(aAllocator)
	{}

	Object& Object::operator=(const Object& aOther) throw(){
		mValues = aOther.mValues;
		return *this;
	}

	uint32_t Object::Size() const {
		return mValues.size();
	}

	const Value& Object::operator[](const uint32_t aIndex) const throw() {
		auto it = mValues.begin();
		for(uint32_t i = 0; i < aIndex; ++i) {
			++it;
		}
		return it->second;
	}

	Value& Object::operator[](const uint32_t aIndex) throw() {
		auto it = mValues.begin();
		for (uint32_t i = 0; i < aIndex; ++i) {
			++it;
		}
		return it->second;
	}

	bool Object::ContainsValue(const String& aName) const throw() {
		return mValues.find(aName) != mValues.end();
	}

	const Value& Object::operator[](const String& aName) const {
		return mValues.find(aName)->second;
	}

	Value& Object::operator[](const String& aName) throw() {
		return mValues.find(aName)->second;
	}

	Value& Object::Add(const String& aName, Value& aValue) throw() {
		return mValues.emplace(aName, aValue).first->second;
	}

	void Object::Erase(const uint32_t aIndex) throw() {
		auto it = mValues.begin();
		for (uint32_t i = 0; i < aIndex; ++i) {
			++it;
		}
		mValues.erase(it);
	}

	void Object::Erase(const String& aName) throw() {
		mValues.erase(mValues.find(aName));
	}

	void Object::Clear() throw() {
		mValues.clear();
	}

	Value& Object::Add(const String& aName) throw() {
		return Add(aName, Value(mAllocator));
	}

	Value& Object::Add(const String& aName, const char aValue) throw() {
		return Add(aName, Value(mAllocator, aValue));
	}

	Value& Object::Add(const String& aName, const bool aValue) throw() {
		return Add(aName, Value(mAllocator, aValue));
	}

	Value& Object::Add(const String& aName, const uint8_t aValue) throw() {
		return Add(aName, Value(mAllocator, aValue));
	}

	Value& Object::Add(const String& aName, const uint16_t aValue) throw() {
		return Add(aName, Value(mAllocator, aValue));
	}

	Value& Object::Add(const String& aName, const uint32_t aValue) throw() {
		return Add(aName, Value(mAllocator, aValue));
	}

	Value& Object::Add(const String& aName, const uint64_t aValue) throw() {
		return Add(aName, Value(mAllocator, aValue));
	}

	Value& Object::Add(const String& aName, const int8_t aValue) throw() {
		return Add(aName, Value(mAllocator, aValue));
	}

	Value& Object::Add(const String& aName, const int16_t aValue) throw() {
		return Add(aName, Value(mAllocator, aValue));
	}

	Value& Object::Add(const String& aName, const int32_t aValue) throw() {
		return Add(aName, Value(mAllocator, aValue));
	}

	Value& Object::Add(const String& aName, const int64_t aValue) throw() {
		return Add(aName, Value(mAllocator, aValue));
	}

	Value& Object::Add(const String& aName, const float aValue) throw() {
		return Add(aName, Value(mAllocator, aValue));
	}

	Value& Object::Add(const String& aName, const double aValue) throw() {
		return Add(aName, Value(mAllocator, aValue));
	}

	Value& Object::Add(const String& aName, const String& aValue) throw() {
		return Add(aName, Value(mAllocator, aValue));
	}

}}
