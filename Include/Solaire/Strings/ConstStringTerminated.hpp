#ifndef SOLAIRE_CONST_STRING_TERMINATED_HPP
#define SOLAIRE_CONST_STRING_TERMINATED_HPP

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
	\file ConstStringTerminated.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 30th November 2015
	Last Modified	: 30th November 2015
*/

#include "ConstString.hpp"

namespace Solaire{
	template<class T, const T TERMINATOR>
	class ConstTerminatedString : public ConstString<T>{
	public:
		static uint32_t LengthOf(const T* const aString)throw() {
			uint32_t size = 0;
			while(aString[size] != TERMINATOR) ++size;
			return size;
		}
	private:
		const T* const mString;
		const uint32_t mSize;
	public:
		ConstTerminatedString(const T* const aString) throw() :
			mString(aString),
			mSize(LengthOf(aString))
		{}
	
		ConstTerminatedString(const T* const aString, const uint32_t aSize) throw() :
			mString(aString),
			mSize(aSize)
		{}

		// Inherited from ConstString
	
		T operator[](const uint32_t aIndex) const throw() override {
			return mString[aIndex];
		}
	
		uint32_t Size() const throw() override {
			return mSize;
		}
	
		uint32_t FindFirstChar(const T aValue) const throw() override {
			return FindNextChar(aValue, 0);
		}
	
		uint32_t FindNextChar(const T aValue, const uint32_t aIndex) const throw() override {
			for(uint32_t i = aIndex; i < mSize; ++i) {
				if(mString[i] == aValue) return i;
			}
			return mSize;
		}
	
		uint32_t FindLastChar(const T aValue) const throw() override {
			uint32_t pos = FindFirstChar(aValue);
			uint32_t posPrev = pos;
			while(pos != mSize){ 
				posPrev = pos;
				pos = FindNextChar(aValue, pos + 1);
			}
			return posPrev;
		}
	
		uint32_t FindFirst(const ConstString<T>& aValue) const throw() override {
			return FindNext(aValue, 0);
		}
	
		uint32_t FindNext(const ConstString<T>& aValue, const uint32_t aIndex) const throw() override {
			//! \todo Implement FindNext
			return mSize;
		}
	
		uint32_t FindLast(const ConstString<T>& aValue) const throw() override {
			uint32_t pos = FindFirst(aValue);
			uint32_t posPrev = pos;
			while(pos != mSize) { 
				posPrev = pos;
				pos = FindNext(aValue, pos + 1);
			}
			return posPrev;
		}
	
		bool operator==(const ConstString<T>& aOther) const throw() override {
			if(mSize != aOther.Size()) return false;
			if(aOther.IsContiguous()) {
				return std::memcmp(mString, aOther.GetContiguousPtr(), mSize * sizeof(T)) == 0;
			} else{
				for(uint32_t i = 0; i < mSize; ++i) {
					if(mString[i] != aOther[i]) return false;
				}
				return true;
			}
		}
	
		bool operator!=(const ConstString<T>& aOther) const throw() override {
			if(mSize != aOther.Size()) return true;
			if(aOther.IsContiguous()) {
				return std::memcmp(mString, aOther.GetContiguousPtr(), mSize * sizeof(T)) != 0;
			}else {
				for(uint32_t i = 0; i < mSize; ++i) {
					if(mString[i] != aOther[i]) return true;
				}
				return false;
			}
		}
	
		bool operator<(const ConstString<T>& aOther) const throw() override {
			if(aOther.IsContiguous()) {
				return std::memcmp(mString, aOther.GetContiguousPtr(), Min(mSize, aOther.Size()) * sizeof(T)) < 0;
			}else {
				const uint32_t size = Min(mSize, aOther.Size());
				for(uint32_t i = 0; i < size; ++i) {
					if(mString[i] >= aOther[i]) return false;
				}
				return true;
			}
		}
	
		bool operator>(const ConstString<T>& aOther) const throw() override {
			if(aOther.IsContiguous()) {
				return std::memcmp(mString, aOther.GetContiguousPtr(), Min(mSize, aOther.Size()) * sizeof(T)) > 0;
			}else {
				const uint32_t size = Min(mSize, aOther.Size());
				for(uint32_t i = 0; i < size; ++i) {
					if(mString[i] <= aOther[i]) return false;
				}
				return true;
			}
		}
	
		bool operator<=(const ConstString<T>& aOther) const throw() override {
			if(aOther.IsContiguous()) {
				return std::memcmp(mString, aOther.GetContiguousPtr(), Min(mSize, aOther.Size()) * sizeof(T)) <= 0;
			}else {
				const uint32_t size = Min(mSize, aOther.Size());
				for(uint32_t i = 0; i < size; ++i) {
					if(mString[i] > aOther[i]) return false;
				}
				return true;
			}
		}
	
		bool operator>=(const ConstString<T>& aOther) const throw() override {
			if(aOther.IsContiguous()) {
				return std::memcmp(mString, aOther.GetContiguousPtr(), Min(mSize, aOther.Size()) * sizeof(T)) >= 0;
			}else {
				const uint32_t size = Min(mSize, aOther.Size());
				for(uint32_t i = 0; i < size; ++i) {
					if(mString[i] < aOther[i]) return false;
				}
				return true;
			}
		}
	
		bool IsContiguous() const throw() override {
			return true;
		}
	
		const T* GetContiguousPtr() const throw() override {
			return mString;
		}
	
		void Destructor() throw() override {
	
		}
	};

	typedef ConstTerminatedString<char, '\0'> ConstCString;

}


#endif
