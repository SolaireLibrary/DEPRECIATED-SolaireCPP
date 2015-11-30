#ifndef SOLAIRE_CONST_STRING_COMPOSITE_HPP
#define SOLAIRE_CONST_STRING_COMPOSITE_HPP

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
	\file ConstStringComposite.hpp
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
#include "..\DataStructures\DynamicArray.hpp"

namespace Solaire{
	
	template<class T>
	class ConstCompositeString : public ConstString<T>{
	public:
		DynamicArray<ConstString<T>*> StringList;
	public:
		ConstCompositeString() throw() :
			StringList(GetDefaultAllocator())
		{}
	
		ConstCompositeString(Allocator& aAllocator) throw() :
			StringList(aAllocator)
		{}

		// Inherited from ConstString
	
		T operator[](const uint32_t aIndex) const throw() override {
			uint32_t index = aIndex;
		
			for(const ConstString<T>* string : StringList) {
				const uint32_t size = string->Size();
				if(index < size) return string->operator[](index);
				index -= size;
			}
		
			return static_cast<T>(0);
		}
	
		uint32_t Size() const throw() override {
			uint32_t size = 0;
			for(const ConstString<T>* const string : StringList) size += string->Size();
			return size;
		}
	
		uint32_t FindFirstChar(const T aValue) const throw() override {
			return FindNextChar(aValue, 0);
		}
	
		uint32_t FindNextChar(const T aValue, const uint32_t aIndex) const throw() override {
			for(uint32_t i = aIndex; i < mSize; ++i) {
				if(operator[](i) == aValue) return i;
			}
			return mSize;
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
			uint32_t offset = 0;
			const uint32_t otherSize = aOther.Size();
			for(const ConstString<T>* const string : StringList) {
				const uint32_t strSize = string->Size();
				if(offset + strSize >= otherSize) return false;
				if(*string != SubString<T>(aOther, offset, offset + strSize)) return false;
				offset += strSize;
			}
			return true;
		}
	
		bool operator!=(const ConstString<T>& aOther) const throw() override {
			uint32_t offset = 0;
			const uint32_t otherSize = aOther.Size();
			for(const ConstString<T>* const string : StringList) {
				const uint32_t strSize = string->Size();
				if(offset + strSize >= otherSize) return true;
				if(*string != SubString<T>(aOther, offset, offset + strSize)) return true;
				offset += strSize;
			}
			return false;
		}
	
		bool operator<(const ConstString<T>& aOther) const throw() override {
			uint32_t offset = 0;
			const uint32_t otherSize = aOther.Size();
			for(const ConstString<T>* const string : StringList) {
				const uint32_t strSize = string->Size();
				if(*string >= SubString<T>(aOther, offset, offset + strSize)) return false;
				offset += strSize;
			}
			return true;
		}
	
		bool operator>(const ConstString<T>& aOther) const throw() override {
			uint32_t offset = 0;
			const uint32_t otherSize = aOther.Size();
			for(const ConstString<T>* const string : StringList) {
				const uint32_t strSize = string->Size();
				if(*string <= SubString<T>(aOther, offset, offset + strSize)) return false;
				offset += strSize;
			}
			return true;
		}
	
		bool operator<=(const ConstString<T>& aOther) const throw() override {
			uint32_t offset = 0;
			const uint32_t otherSize = aOther.Size();
			for(const ConstString<T>* const string : StringList) {
				const uint32_t strSize = string->Size();
				if(*string > SubString<T>(aOther, offset, offset + strSize)) return false;
				offset += strSize;
			}
			return true;
		}
	
		bool operator>=(const ConstString<T>& aOther) const throw() override {
			uint32_t offset = 0;
			const uint32_t otherSize = aOther.Size();
			for(const ConstString<T>* const string : StringList) {
				const uint32_t strSize = string->Size();
				if(*string < SubString<T>(aOther, offset, offset + strSize)) return false;
				offset += strSize;
			}
			return true;
		}
	
		bool IsContiguous() const throw() override {
			return false;
		}
	
		const T* GetContiguousPtr() const throw() override {
			return nullptr;
		}
	
		void Destructor() throw() override {
	
		}
	};

}


#endif
