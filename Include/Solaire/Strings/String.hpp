#ifndef SOLAIRE_STRING_HPP
#define SOLAIRE_STRING_HPP

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
	\file String.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 27th September 2015
	Last Modified	: 27th September 2015
*/

#include <type_traits>
#include <sstream>
#include "../DataStructures/DynamicArray.hpp"
#include "ConstString.hpp"      
#include "ConstStringTerminated.hpp"

namespace Solaire{

	template<class T>
	class StringCase {
	public:
		virtual T SOLAIRE_EXPORT_CALL ToLowerCase(const T) const throw() = 0;
		virtual T SOLAIRE_EXPORT_CALL ToUpperCase(const T) const throw() = 0;
		virtual T SOLAIRE_EXPORT_CALL ToggleCase(const T) const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL IsLowerCase(const T) const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL IsUpperCase(const T) const throw() = 0;
	};

	template<class T>
	class NullStringCase : public StringCase<T>{
	public:
		T SOLAIRE_EXPORT_CALL ToLowerCase(const T aValue) const throw() override {
			return aValue;
		}

		T SOLAIRE_EXPORT_CALL ToUpperCase(const T aValue) const throw() {
			return aValue;
		}

		T SOLAIRE_EXPORT_CALL ToggleCase(const T aValue) const throw() {
			return aValue;
		}

		bool SOLAIRE_EXPORT_CALL IsLowerCase(const T aValue) const throw() {
			return false;
		}

		bool SOLAIRE_EXPORT_CALL IsUpperCase(const T aValue) const throw() {
			return false;
		}
	};

	class AsciiCase : public StringCase<char> {
	private:
		enum {
			CASE_DIF = 'A' - 'a'
		};
	public:
		char SOLAIRE_EXPORT_CALL ToLowerCase(const char aValue) const throw() override {
			return IsUpperCase(aValue) ? aValue - CASE_DIF : aValue;
		}

		char SOLAIRE_EXPORT_CALL ToUpperCase(const char aValue) const throw() {
			return IsLowerCase(aValue) ? aValue + CASE_DIF : aValue;
		}

		char SOLAIRE_EXPORT_CALL ToggleCase(const char aValue) const throw() {
			return IsLowerCase(aValue) ? ToUpperCase(aValue) : ToLowerCase(aValue);
		}

		bool SOLAIRE_EXPORT_CALL IsLowerCase(const char aValue) const throw() {
			return aValue >= 'a' && aValue <= 'z';
		}

		bool SOLAIRE_EXPORT_CALL IsUpperCase(const char aValue) const throw() {
			return aValue >= 'A' && aValue <= 'Z';
		}
	};

	template<class T>
	struct DefaultCase {
		typedef NullStringCase<T> Type;
	};

	template<>
	struct DefaultCase<char> {
		typedef AsciiCase Type;
	};

	template<class T>
	class String : public ConstString<T>{
	public:
		virtual bool SOLAIRE_EXPORT_CALL AppendChar(const T) throw() = 0;

		virtual bool SOLAIRE_EXPORT_CALL InsertCharBefore(const T, const uint32_t) throw() = 0;

		virtual bool SOLAIRE_EXPORT_CALL Erase(const uint32_t) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Clear() throw() = 0;

		virtual const StringCase<T>& SOLAIRE_EXPORT_CALL GetCase() const throw() = 0;

		virtual SOLAIRE_EXPORT_CALL ~String() throw() {}
		 
		inline String<T>& operator=(const ConstString<T>& aString) throw() {
			Clear();
			return operator+=(aString);
		}

		inline bool SOLAIRE_EXPORT_CALL Append(const ConstString<T>& aString) throw() {
			const uint32_t size = aString.Size();
			for (uint32_t i = 0; i < size; ++i) if (!AppendChar(aString[i])) return false;
			return true;
		}

		inline String<T>& SOLAIRE_EXPORT_CALL ToLowerCase() throw() {
			const uint32_t size = Size();
			for(uint32_t i = 0; i < size; ++i) {
				char& ref = operator[](i);
				ref = GetCase().ToLowerCase(ref);
			}
			return *this;
		}

		inline String<T>& SOLAIRE_EXPORT_CALL ToUpperCase() throw() {
			const uint32_t size = Size();
			for(uint32_t i = 0; i < size; ++i) {
				char& ref = operator[](i);
				ref = GetCase().ToUpperCase(ref);
			}
			return *this;
		}
		 
		inline String<T>& SOLAIRE_EXPORT_CALL ToggleCase() throw() {
			const uint32_t size = Size();
			for(uint32_t i = 0; i < size; ++i) {
				char& ref = operator[](i);
				ref = GetCase().ToggleCase(ref);
			}
			return *this;
		}

		inline bool SOLAIRE_EXPORT_CALL IsLowerCase() const throw() {
			//! \todo Move to ConstString
			const uint32_t size = Size();
			const StringCase<T>& _case = GetCase();
			for(uint32_t i = 0; i < size; ++i) {
				if(! _case.IsLowerCase(operator[](i))) return false;
			}
			return true;
		}

		inline bool SOLAIRE_EXPORT_CALL IsUpperCase() const throw() {
			//! \todo Move to ConstString
			const uint32_t size = Size();
			const StringCase<T>& _case = GetCase();
			for(uint32_t i = 0; i < size; ++i) {
				if(! _case.IsUpperCase(operator[](i))) return false;
			}
			return true;
		}

		inline bool SOLAIRE_EXPORT_CALL InsertBefore(const ConstString<T>& aString, const uint32_t aIndex) throw() {
			const uint32_t size = aString.Size();
			for(uint32_t i = 0; i < size; ++i) if(! InsertCharBefore(aString[i], aIndex + i)) return false;
			return true;
		}

		inline bool SOLAIRE_EXPORT_CALL InsertCharAfter(const T aValue, const uint32_t aIndex) throw() {
			return InsertCharBefore(aValue, aIndex + 1);
		}

		inline bool SOLAIRE_EXPORT_CALL InsertAfter(const ConstString<T>& aValue, const uint32_t aIndex) throw() {
			return InsertBefore(aValue, aIndex + 1);
		}

		inline uint32_t SOLAIRE_EXPORT_CALL ReplaceFirstChar(const T aTarget, const T aReplacement) throw() {
			return ReplaceNextChar(aTarget, aReplacement, 0);
		}

		inline uint32_t SOLAIRE_EXPORT_CALL ReplaceNextChar(const T aTarget, const T aReplacement, const uint32_t aIndex) throw() {
			const uint32_t size = Size();
			const uint32_t pos = FindNextChar(aTarget);
			if(pos < size) {
				operator[](pos) = aReplacement;
			}
			return pos;
		}

		inline uint32_t SOLAIRE_EXPORT_CALL ReplaceLastChar(const T aTarget, const T aReplacement) throw() {
			return ReplaceNextChar(aTarget, aReplacement, FindLastChar(aTarget));
		}

		inline uint32_t SOLAIRE_EXPORT_CALL ReplaceFirst(const ConstString<T>& aTarget, const ConstString<T>& aReplacement) throw() {
			return ReplaceNext(aTarget, aReplacement, 0);
		}

		inline uint32_t SOLAIRE_EXPORT_CALL ReplaceNext(const ConstString<T>& aTarget, const ConstString<T>& aReplacement, const uint32_t aIndex) throw() {
			const uint32_t size = Size();
			const uint32_t targetSize = aTarget.Size();
			const uint32_t replacementSize = aReplacement.Size();
			const uint32_t minSize = targetSize < replacementSize ? targetSize : replacementSize;

			uint32_t pos = FindNext(aTarget, aIndex);
			if(pos == size) return size;

			if(IsContiguous() && aValue.IsContiguous()) {
				std::memcpy(GetContiguousPtr() + pos, aValue.GetContiguousPtr(), minSize * sizeof(T));
			}else {
				for(uint32_t i = 0; i < minSize; ++i) operator[](pos + i) = aValue[i];
			}
			for(uint32_t i = minSize; i < targetSize; ++i) if(! Erase(pos + minSize)) return size;
			for(uint32_t i = minSize; i < replacementSize; ++i) if(! InsertAfter(aReplacement[i], pos + minsize + i)) return size;

			return pos;
		}

		inline uint32_t SOLAIRE_EXPORT_CALL ReplaceLast(const ConstString<T>& aTarget, const ConstString<T>& aReplacement) throw() {
			return ReplaceNext(aTarget, aReplacement, FindLast(aTarget));
		}

		inline uint32_t SOLAIRE_EXPORT_CALL ReplaceAllChars(const T aTarget, const T aReplacement) throw() {
			const uint32_t size = Size();
			const uint32_t count = 0;
			uint32_t pos = FindFirst(aTarget);
			while(pos != size) {
				operator[](pos) = aReplacement;
				pos = FindFirst(aTarget);
				++count;
			}

			return count;
		}

		inline uint32_t SOLAIRE_EXPORT_CALL ReplaceAll(const ConstString<T>& aTarget, const ConstString<T>& aReplacement) throw() {
			const uint32_t count = 0;
			uint32_t pos = FindFirst(aTarget);
			while(pos != Size()) {
				ReplaceNext(aTarget, aReplacement, pos);
				pos = FindFirst(aTarget);
				++count;
			}

			return count;
		}

		inline T& SOLAIRE_EXPORT_CALL operator[](const uint32_t aIndex) throw() {
			//! \todo Check if non-virtual overload is abi compatible
			return const_cast<T&>(static_cast<const ConstString<T>*>(this)->operator[](aIndex));
		}

		inline String<T>& SOLAIRE_EXPORT_CALL operator+=(const char aValue) throw() {
			//! \todo Check if non-virtual overload is abi compatible
			AppendChar(aValue);
			return *this;
		}

		inline String<T>& SOLAIRE_EXPORT_CALL operator+=(const ConstString<T>& aValue) throw() {
			//! \todo Check if non-virtual overload is abi compatible
			Append(aValue);
			return *this;
		}

		inline T* SOLAIRE_EXPORT_CALL GetContiguousPtr() throw() {
			//! \todo Check if non-virtual overload is abi compatible
			return const_cast<char*>(static_cast<const ConstString<T>*>(this)->GetContiguousPtr());
		}
	};

	template<class T, const T TERMINATOR>
	class TerminatedString : public String<T> {
	private:
		static const typename DefaultCase<T>::Type DEFAULT_CASE;
	private:
		DynamicArray<T> mString;
		const StringCase<T>* mCase;
	public:
		TerminatedString() :
			mString(GetDefaultAllocator()),
			mCase(& DEFAULT_CASE)
		{
			mString.PushBack(TERMINATOR);
		}

		TerminatedString(Allocator& aAllocator) :
			mString(aAllocator),
			mCase(&DEFAULT_CASE)
		{
			mString.PushBack(TERMINATOR);
		}

		TerminatedString(const StringCase<T>& aCase) :
			mString(GetDefaultAllocator()),
			mCase(&aCase)
		{
			mString.PushBack(TERMINATOR);
		}

		TerminatedString(Allocator& aAllocator, const StringCase<T>& aCase) :
			mString(aAllocator),
			mCase(&aCase)
		{
			mString.PushBack(TERMINATOR);
		}

		TerminatedString(Allocator& aAllocator, const T* const aValue) :
			mString(aAllocator),
			mCase(&DEFAULT_CASE)
		{
			const T* tmp = aValue;
			while(*tmp != TERMINATOR) {
				mString.PushBack(*tmp);
				++tmp;
			}
			mString.PushBack(TERMINATOR);
		}

		

		TerminatedString(Allocator& aAllocator, const ConstString<T>& aValue) :
			mString(aAllocator),
			mCase(&DEFAULT_CASE)
		{
			const uint32_t size = aValue.Size();
			for(uint32_t i = 0; i < size; ++i) {
				mString.PushBack(aValue[i]);
			}
			mString.PushBack(TERMINATOR);
		}

		SOLAIRE_EXPORT_CALL ~TerminatedString() throw() {

		}

		// Inherited from ConstString

		const T& SOLAIRE_EXPORT_CALL operator[](const uint32_t aIndex) const throw() override {
			return mString.GetPtr()[aIndex];
		}

		uint32_t SOLAIRE_EXPORT_CALL Size() const throw() override {
			return mString.Size() - 1;
		}

		bool SOLAIRE_EXPORT_CALL IsContiguous() const throw() override {
			return true;
		}

		// Inherited from String

		bool SOLAIRE_EXPORT_CALL AppendChar(const T aValue) throw() override {
			mString.Back() = aValue;
			mString.PushBack(TERMINATOR);
			return true;
		}

		bool SOLAIRE_EXPORT_CALL InsertCharBefore(const T aValue, const uint32_t aIndex) throw() override {
			mString.InsertBefore(mString.begin() + aIndex, aValue);
			return true;
		}

		bool SOLAIRE_EXPORT_CALL Erase(const uint32_t aIndex) throw() override {
			mString.Erase(mString.begin() + aIndex);
			return true;
		}

		bool SOLAIRE_EXPORT_CALL Clear() throw() override {
			mString.Clear();
			return true;
		}

		const StringCase<T>& SOLAIRE_EXPORT_CALL GetCase() const throw() override {
			return *mCase;
		}

	};

	template<class T, const T TERMINATOR>
	const typename DefaultCase<T>::Type TerminatedString<T, TERMINATOR>::DEFAULT_CASE;

	typedef TerminatedString<char, '\0'> CString;
}


#endif
