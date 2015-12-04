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
	Last Modified	: 3rd December 2015
*/

#include <type_traits>
#include <sstream>
#include "../DataStructures/DynamicArray.hpp"
#include "ConstString.hpp"      
#include "ConstStringTerminated.hpp"

namespace Solaire{

	template<class T>
	class String : public ConstString<T>{
	public:
		virtual bool SOLAIRE_EXPORT_CALL AppendChar(const T) throw() = 0;

		virtual bool SOLAIRE_EXPORT_CALL InsertCharBefore(const T, const uint32_t) throw() = 0;

		virtual bool SOLAIRE_EXPORT_CALL Erase(const uint32_t) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Clear() throw() = 0;

		virtual SOLAIRE_EXPORT_CALL ~String() throw() {}

		inline bool SOLAIRE_EXPORT_CALL Append(const ConstString<T>& aString) throw() {
			const uint32_t size = aString.Size();
			for (uint32_t i = 0; i < size; ++i) if (!AppendChar(aString[i])) return false;
			return true;
		}

		inline String<T>& operator=(const ConstString<T>& aString) throw() {
			Clear();
			Append(aString);
			return *this;
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

		inline T* SOLAIRE_EXPORT_CALL GetContiguousPtr() throw() {
			//! \todo Check if non-virtual overload is abi compatible
			return const_cast<char*>(static_cast<const ConstString<T>*>(this)->GetContiguousPtr());
		}
	};

	template<class T, const T TERMINATOR>
	class TerminatedString : public String<T> {
	private:
		DynamicArray<T> mString;
	public:
		TerminatedString() :
			mString(GetDefaultAllocator())
		{
			mString.PushBack(TERMINATOR);
		}

		TerminatedString(Allocator& aAllocator) :
			mString(aAllocator)
		{
			mString.PushBack(TERMINATOR);
		}

		TerminatedString(Allocator& aAllocator, const T* const aValue) :
			mString(aAllocator)
		{
			const T* tmp = aValue;
			while(*tmp != TERMINATOR) {
				mString.PushBack(*tmp);
				++tmp;
			}
			mString.PushBack(TERMINATOR);
		}

		TerminatedString(const T* const aValue) :
			mString(GetDefaultAllocator())
		{
			const T* tmp = aValue;
			while(*tmp != TERMINATOR) {
				mString.PushBack(*tmp);
				++tmp;
			}
			mString.PushBack(TERMINATOR);
		}

		TerminatedString(const ConstString<T>& aValue) :
			mString(aValue.GetAllocator())
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

		Allocator& SOLAIRE_EXPORT_CALL GetAllocator() const throw() override {
			return mString.GetAllocator();
		}

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

	};

	typedef TerminatedString<char, '\0'> CString;
}

template<class T>
Solaire::String<T>& SOLAIRE_EXPORT_CALL operator+=(Solaire::String<T>& aString, const uint8_t aValue) throw() {
	//! \todo implement String += uint8_t
	return aString;
}

template<class T>
Solaire::String<T>& SOLAIRE_EXPORT_CALL operator+=(Solaire::String<T>& aString, const uint16_t aValue) throw() {
	//! \todo implement String += uint16_t
	return aString;
}

template<class T>
Solaire::String<T>& SOLAIRE_EXPORT_CALL operator+=(Solaire::String<T>& aString, const uint32_t aValue) throw() {
	//! \todo implement String += uint32_t
	return aString;
}

template<class T>
Solaire::String<T>& SOLAIRE_EXPORT_CALL operator+=(Solaire::String<T>& aString, const uint64_t aValue) throw() {
	//! \todo implement String += uint64_t
	return aString;
}

template<class T>
Solaire::String<T>& SOLAIRE_EXPORT_CALL operator+=(Solaire::String<T>& aString, const int8_t aValue) throw() {
	//! \todo implement String += int8_t
	return aString;
}

template<class T>
Solaire::String<T>& SOLAIRE_EXPORT_CALL operator+=(Solaire::String<T>& aString, const int16_t aValue) throw() {
	//! \todo implement String += int16_t
	return aString;
}

template<class T>
Solaire::String<T>& SOLAIRE_EXPORT_CALL operator+=(Solaire::String<T>& aString, const int32_t aValue) throw() {
	//! \todo implement String += int32_t
	return aString;
}

template<class T>
Solaire::String<T>& SOLAIRE_EXPORT_CALL operator+=(Solaire::String<T>& aString, const int64_t aValue) throw() {
	//! \todo implement String += int64_t
	return aString;
}

template<class T>
Solaire::String<T>& SOLAIRE_EXPORT_CALL operator+=(Solaire::String<T>& aString, const float aValue) throw() {
	//! \todo implement String += float
	return aString;
}

template<class T>
Solaire::String<T>& SOLAIRE_EXPORT_CALL operator+=(Solaire::String<T>& aString, const double aValue) throw() {
	//! \todo implement String += double
	return aString;
}

template<class T>
Solaire::String<T>& SOLAIRE_EXPORT_CALL operator+=(Solaire::String<T>& aString, const T aValue) throw() {
	aString.AppendChar(aValue);
	return aString;
}

template<class T>
Solaire::String<T>& SOLAIRE_EXPORT_CALL operator+=(Solaire::String<T>& aString, const Solaire::ConstString<T>& aValue) throw() {
	aString.Append(aValue);
	return aString;
}

template<class T>
Solaire::String<T>& SOLAIRE_EXPORT_CALL operator+=(Solaire::String<T>& aString, const T* const aValue) throw() {
	aString.Append(TerminatedString<T, '\0'>(aString.GetAllocator(), aValue));
	return aString;
}

template<class T, const T TERMINATOR = '\0'>
Solaire::TerminatedString<T, TERMINATOR> SOLAIRE_EXPORT_CALL operator+(const Solaire::ConstString<T>& aFirst, const T aSecond) {
	return Solaire::TerminatedString<T, TERMINATOR>(aFirst) += aSecond;
}

template<class T, const T TERMINATOR = '\0'>
Solaire::TerminatedString<T, TERMINATOR> SOLAIRE_EXPORT_CALL operator+(const Solaire::ConstString<T>& aFirst, const Solaire::ConstString<T>& aSecond) {
	return Solaire::TerminatedString<T, TERMINATOR>(aFirst) += aSecond;
}

template<class T, const T TERMINATOR = '\0'>
Solaire::TerminatedString<T, TERMINATOR> SOLAIRE_EXPORT_CALL operator+(const Solaire::ConstString<T>& aFirst, const T* const aSecond) {
	return Solaire::TerminatedString<T, TERMINATOR>(aFirst) += aSecond;
}

template<class T, const T TERMINATOR = '\0'>
Solaire::TerminatedString<T, TERMINATOR> SOLAIRE_EXPORT_CALL operator+(const char aFirst, const Solaire::ConstString<T>& aSecond) {
	Solaire::TerminatedString<T, TERMINATOR> tmp(aSecond.GetAllocator());
	tmp += aFirst;
	tmp += aSecond;
	return tmp;
}

template<class T, const T TERMINATOR = '\0'>
Solaire::TerminatedString<T, TERMINATOR> SOLAIRE_EXPORT_CALL operator+(const T* const aFirst, const Solaire::ConstString<T>& aSecond) {
	Solaire::TerminatedString<T, TERMINATOR> tmp(aSecond.GetAllocator());
	tmp += aFirst;
	tmp += aSecond;
	return tmp;
}


#endif
