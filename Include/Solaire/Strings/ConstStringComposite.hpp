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
	
		const T& SOLAIRE_EXPORT_CALL operator[](const uint32_t aIndex) const throw() override {
			uint32_t index = aIndex;
		
			for(const ConstString<T>* string : StringList) {
				const uint32_t size = string->Size();
				if(index < size) return string->operator[](index);
				index -= size;
			}
		
			return mStrings->Begin()->operator[0];
		}
	
		uint32_t SOLAIRE_EXPORT_CALL Size() const throw() override {
			uint32_t size = 0;
			for(const ConstString<T>* const string : StringList) size += string->Size();
			return size;
		}
	
		uint32_t SOLAIRE_EXPORT_CALL FindFirstChar(const T aValue) const throw() override {
			return FindNextChar(aValue, 0);
		}
	
		uint32_t SOLAIRE_EXPORT_CALL FindNextChar(const T aValue, const uint32_t aIndex) const throw() override {
			for(uint32_t i = aIndex; i < mSize; ++i) {
				if(operator[](i) == aValue) return i;
			}
			return mSize;
		}
	
		uint32_t SOLAIRE_EXPORT_CALL FindLastChar(const T aValue) const throw() override {
			uint32_t pos = FindFirst(aValue);
			uint32_t posPrev = pos;
			while(pos != mSize) { 
				posPrev = pos;
				pos = FindNextChar(aValue, pos + 1);
			}
			return posPrev;
		}
	
		uint32_t SOLAIRE_EXPORT_CALL FindFirst(const ConstString<T>& aValue) const throw() override {
			return FindNext(aValue, 0);
		}
	
		uint32_t SOLAIRE_EXPORT_CALL FindNext(const ConstString<T>& aValue, const uint32_t aIndex) const throw() override {
			//! \todo Implement FindNext
			return mSize;
		}
	
		uint32_t SOLAIRE_EXPORT_CALL FindLast(const ConstString<T>& aValue) const throw() override {
			uint32_t pos = FindFirst(aValue);
			uint32_t posPrev = pos;
			while(pos != mSize) { 
				posPrev = pos;
				pos = FindNext(aValue, pos + 1);
			}
			return posPrev;
		}
	
		bool SOLAIRE_EXPORT_CALL IsContiguous() const throw() override {
			return false;
		}
	
		void SOLAIRE_EXPORT_CALL Destructor() throw() override {
	
		}
	};

}


#endif
