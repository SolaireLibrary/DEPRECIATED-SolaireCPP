#ifndef SOLAIRE_CONST_STRING_SUB_HPP
#define SOLAIRE_CONST_STRING_SUB_HPP

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
	\file ConstStringSub.hpp
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
	
	template<class T>
	class ConstSubString : public ConstString<T>{
	private:
		const ConstString<T>& mString;
		const uint32_t mBegin;
		const uint32_t mEnd;
	public:
		ConstSubString(const ConstString<T>& aOther) throw() :
			mString(aOther),
			mBegin(0),
			mEnd(aOther.Size())
		{}
	
		ConstSubString(const ConstString<T>& aOther, const uint32_t aBegin, const uint32_t aEnd) throw() :
			mString(aOther),
			mBegin(aBegin),
			mEnd(aEnd)
		{}

		// Inherited from ConstString
	
		T operator[](const uint32_t aIndex) const throw() override {
			return mString(aIndex + mBegin);
		}
	
		uint32_t Size() const throw() override {
			return mEnd - mBegin;
		}
	
		uint32_t FindFirstChar(const T aValue) const throw() override {
			return FindNextChar(aValue, 0);
		}
	
		uint32_t FindNextChar(const T aValue, const uint32_t aIndex) const throw() override {
			const uint32_t pos = mString.FindFirstChar(aValue, aIndex + mBegin);
			if(pos >= mEnd) return mEnd;
			return pos;
		}
	
		uint32_t FindLastChar(const T aValue) const throw() override {
			uint32_t pos = FindFirst(aValue);
			uint32_t posPrev = pos;
			while(pos != mSize) { 
				posPrev = pos;
				pos = FindNextChar(aValue, pos + 1);
			}
			return posPrev;
		}
	
		uint32_t FindFirst(const ConstString<T>& aValue) const throw() override {
			return FindNext(aValue, 0);
		}
	
		uint32_t FindNext(const ConstString<T>& aValue, const uint32_t aIndex) const throw() override {
			const uint32_t pos = mString.FindFirs(aValue, aIndex + mBegin);
			if(pos >= mEnd) return mEnd;
			return pos;
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
			const uint32_t size = mString.GetSize();
			const uint32_t otherSize = aOther.GetSize();
		
			if(size != otherSize) return false;
			if(mString.IsContiguous() && aOther.IsContiguous()) {
				return std::memcmp(mString.GetContiguousPtr(), aOther.GetContiguousPtr(), size * sizeof(T)) == 0;
			}else {
				for(uint32_t i = 0; i < size; ++i) {
					if(mString[i] != aOther[i] return false;
				}
				return true;
			}
		}
	
		bool operator!=(const ConstString<T>& aOther) const throw() override {		
			const uint32_t size = mString.GetSize();
			const uint32_t otherSize = aOther.GetSize();
		
			if(size != otherSize) return false;
			if(mString.IsContiguous() && aOther.IsContiguous()) {
				return std::memcmp(mString.GetContiguousPtr(), aOther.GetContiguousPtr(), size * sizeof(T)) == 0;
			}else {
				for(uint32_t i = 0; i < size; ++i) {
					if(mString[i] != aOther[i] return true;
				}
				return false;
			}
		}
	
		bool operator<(const ConstString<T>& aOther) const throw() override {
			const uint32_t size = mString.GetSize();
			const uint32_t otherSize = aOther.GetSize();
			const uint32_t minSize = Min(size, otherSize);
		
			if(mString.IsContiguous() && aOther.IsContiguous()) {
				return std::memcmp(mString.GetContiguousPtr(), aOther.GetContiguousPtr(), minSize * sizeof(T)) < 0;
			}else {
				for(uint32_t i = 0; i < minSize; ++i) {
					if(mString[i] >= aOther[i] return false;
				}
				return true;
			}
		}
	
		bool operator>(const ConstString<T>& aOther) const throw() override {
			const uint32_t size = mString.GetSize();
			const uint32_t otherSize = aOther.GetSize();
			const uint32_t minSize = Min(size, otherSize);
		
			if(mString.IsContiguous() && aOther.IsContiguous()) {
				return std::memcmp(mString.GetContiguousPtr(), aOther.GetContiguousPtr(), minSize * sizeof(T)) > 0;
			}else {
				for(uint32_t i = 0; i < minSize; ++i) {
					if(mString[i] <= aOther[i] return false;
				}
				return true;
			}
		}
	
		bool operator<=(const ConstString<T>& aOther) const throw() override {
			const uint32_t size = mString.GetSize();
			const uint32_t otherSize = aOther.GetSize();
			const uint32_t minSize = Min(size, otherSize);
		
			if(mString.IsContiguous() && aOther.IsContiguous()) {
				return std::memcmp(mString.GetContiguousPtr(), aOther.GetContiguousPtr(), minSize * sizeof(T)) <= 0;
			}else {
				for(uint32_t i = 0; i < minSize; ++i) {
					if(mString[i] > aOther[i] return false;
				}
				return true;
			}
		}
	
		bool operator>=(const ConstString<T>& aOther) const throw() override {
			const uint32_t size = mString.GetSize();
			const uint32_t otherSize = aOther.GetSize();
			const uint32_t minSize = Min(size, otherSize);
		
			if(mString.IsContiguous() && aOther.IsContiguous()) {
				return std::memcmp(mString.GetContiguousPtr(), aOther.GetContiguousPtr(), minSize * sizeof(T)) >= 0;
			}else {
				for(uint32_t i = 0; i < minSize; ++i) {
					if(mString[i] < aOther[i] return false;
				}
				return true;
			}
		}
	
		bool IsContiguous() const throw() override {
			return mString.IsContiguous();
		}
	
		const T* GetContiguousPtr() const throw() override {
			return mString.IsContiguous() ? mString.GetContiguousPtr() + mBegin : nullptr;
		}
	
		void Destructor() throw() override {
	
		}
	};

}


#endif
