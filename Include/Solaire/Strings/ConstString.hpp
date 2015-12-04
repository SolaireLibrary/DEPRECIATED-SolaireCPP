#ifndef SOLAIRE_CONST_STRING_HPP
#define SOLAIRE_CONST_STRING_HPP

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
	\file ConstString.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 30th November 2015
	Last Modified	: 30th November 2015
*/

#include <cstdint>
#include "..\Core\Init.hpp"

namespace Solaire{

	template<class T>
	class ConstString{
	public:
		virtual const T& SOLAIRE_EXPORT_CALL operator[](const uint32_t) const throw() = 0;
		virtual uint32_t SOLAIRE_EXPORT_CALL Size() const throw() = 0;

		virtual bool SOLAIRE_EXPORT_CALL IsContiguous() const throw() = 0;

		virtual SOLAIRE_EXPORT_CALL ~ConstString() throw() {}

		inline uint32_t SOLAIRE_EXPORT_CALL FindFirstChar(const T aValue) const throw() {
			return FindNextChar(aValue, 0);
		}

		inline uint32_t SOLAIRE_EXPORT_CALL FindNextChar(const T aValue, const uint32_t aIndex) const throw() {
			const uint32_t size = Size();
			for(uint32_t i = aIndex; i < size; ++i) if(operator[](i) == aValue) return i;
			return size;
		}

		inline uint32_t SOLAIRE_EXPORT_CALL FindLastChar(const T aValue) const throw() {
			const uint32_t size = Size();
			uint32_t i = FindFirstChar(aValue);
			uint32_t j = i;
			while(i != size) {
				j = i;
				i = FindNextChar(aValue);
			}
			return j;
		}

		inline uint32_t SOLAIRE_EXPORT_CALL FindFirst(const ConstString<T>& aValue) const throw() {
			return FindNext(aValue, 0);
		}

		inline uint32_t SOLAIRE_EXPORT_CALL FindNext(const ConstString<T>& aValue, const uint32_t aIndex) const throw() {
			const uint32_t size = Size();
			const uint32_t valueSize = aValue.Size();
			if (valueSize > size) return size;
			const uint32_t end = size - valueSize;

			if(IsContiguous() && aValue.IsContiguous()) {
				const T* const ptrA = GetContiguousPtr();
				const T* const ptrB = aValue.GetContiguousPtr();

				for(uint32_t i = 0; i < end; ++i) {
					if(std::memcmp(ptrA + i, ptrB, valueSize * sizeof(T)) == 0) return i;
				}
			}else {
				for(uint32_t i = 0; i < end; ++i) {
					bool matched = true;
					for(uint32_t j = 0; j < valueSize; ++j) {
						if(operator[](i + j) != aValue[j]) {
							matched = false;
							break;
						}
					}
					if(matched) return i;
				}
			}
			
			return size;
		}

		inline uint32_t SOLAIRE_EXPORT_CALL FindLast(const ConstString<T>& aValue) const throw() {
			const uint32_t size = Size();
			uint32_t i = FindFirst(aValue);
			uint32_t j = i;
			while(i != size) {
				j = i;
				i = FindNext(aValue);
			}
			return j;
		}
	 
		inline bool SOLAIRE_EXPORT_CALL operator==(const ConstString<T>& aOther) const throw() {
			const uint32_t size = Size();
			const uint32_t otherSize = aOther.Size();
			if(size != otherSize) return false;

			if(IsContiguous() && aOther.IsContiguous()) {
				return std::memcmp(GetContiguousPtr(), aOther.GetContiguousPtr(), size * sizeof(T)) == 0;
			}else {
				for(uint32_t i = 0; i < size; ++i) {
					if(operator[](i) != aOther[i]) return false;
				}
				return true;
			}
		}

		inline bool SOLAIRE_EXPORT_CALL operator!=(const ConstString<T>& aOther) const throw() {
			const uint32_t size = Size();
			const uint32_t otherSize = aOther.Size();
			if(size != otherSize) return false;

			if(IsContiguous() && aOther.IsContiguous()) {
				return std::memcmp(GetContiguousPtr(), aOther.GetContiguousPtr(), size * sizeof(T)) != 0;
			}else {
				for(uint32_t i = 0; i < size; ++i) {
					if(operator[](i) != aOther[i]) return true;
				}
				return false;
			}
		}

		inline bool SOLAIRE_EXPORT_CALL operator<(const ConstString<T>& aOther) const throw() {
			const uint32_t size = Size();
			const uint32_t otherSize = aOther.Size();
			const uint32_t minSize = size < otherSize ? size : otherSize;

			if(IsContiguous() && aOther.IsContiguous()) {
				return std::memcmp(GetContiguousPtr(), aOther.GetContiguousPtr(), minSize * sizeof(T)) < 0;
			}else {
				for(uint32_t i = 0; i < minSize; ++i) {
					if(operator[](i) >= aOther[i]) return false;
				}
				return true;
			}
		}

		inline bool SOLAIRE_EXPORT_CALL operator>(const ConstString<T>& aOther) const throw() {
			const uint32_t size = Size();
			const uint32_t otherSize = aOther.Size();
			const uint32_t minSize = size < otherSize ? size : otherSize;

			if(IsContiguous() && aOther.IsContiguous()) {
				return std::memcmp(GetContiguousPtr(), aOther.GetContiguousPtr(), minSize * sizeof(T)) > 0;
			}else {
				for(uint32_t i = 0; i < minSize; ++i) {
					if(operator[](i) <= aOther[i]) return false;
				}
				return true;
			}
		}

		inline bool SOLAIRE_EXPORT_CALL operator<=(const ConstString<T>& aOther) const throw(){
			const uint32_t size = Size();
			const uint32_t otherSize = aOther.Size();
			const uint32_t minSize = size < otherSize ? size : otherSize;

			if(IsContiguous() && aOther.IsContiguous()) {
				return std::memcmp(GetContiguousPtr(), aOther.GetContiguousPtr(), minSize * sizeof(T)) <= 0;
			}else {
				for(uint32_t i = 0; i < minSize; ++i) {
					if(operator[](i) > aOther[i]) return false;
				}
				return true;
			}
		}

		inline bool SOLAIRE_EXPORT_CALL operator>=(const ConstString<T>& aOther) const throw() {
			const uint32_t size = Size();
			const uint32_t otherSize = aOther.Size();
			const uint32_t minSize = size < otherSize ? size : otherSize;

			if(IsContiguous() && aOther.IsContiguous()) {
				return std::memcmp(GetContiguousPtr(), aOther.GetContiguousPtr(), minSize * sizeof(T)) >= 0;
			}else {
				for(uint32_t i = 0; i < minSize; ++i) {
					if(operator[](i) > aOther[i]) return false;
				}
				return true;
			}
		}

		inline const T* SOLAIRE_EXPORT_CALL GetContiguousPtr() const throw() {
			return IsContiguous() ? &operator[](0) : nullptr;
		}
	};
    
}

#endif
