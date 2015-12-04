#ifndef SOLAIRE_STRING_UTILS_HPP
#define SOLAIRE_STRING_UTILS_HPP

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
	\file StringUtils.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 30th November 2015
	Last Modified	: 3rd December 2015
*/

#include "String.hpp"
	
template<class T>
std::ostream& operator<<(std::ostream& aStream, const Solaire::ConstString<T>& aString) {
	if(aString.IsContiguous()) {
		aStream.write(aString.GetContiguousPtr(), aString.Size() * sizeof(T));
	}else {
		const uint32_t size = aString.Size();
		for(uint32_t i = 0; i < size; ++i) {
			std::cout << aString[i];
		}
	}

	return aStream;
}

namespace Solaire {

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
	class NullStringCase : public StringCase<T> {
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

	template<class T, typename CASE = typename DefaultCase<T>::Type, const T TEMINATOR = '\0'>
	TerminatedString<T, TEMINATOR> SOLAIRE_EXPORT_CALL ToLowerCase(const ConstString<T>& aString) throw() {
		const CASE _case;
		TerminatedString<T, TEMINATOR> string;
		const uint32_t size = aString.Size();
		for(uint32_t i = 0; i < size; ++i) {
			string += _case.ToLowerCase(aString[i]);
		}
		return string;
	}

	template<class T, const class CASE = typename DefaultCase<T>::Type, const T TEMINATOR = '\0'>
	TerminatedString<T, TEMINATOR> SOLAIRE_EXPORT_CALL ToUpperCase(const ConstString<T>& aString) throw() {
		const CASE _case;
		TerminatedString<T, TEMINATOR> string;
		const uint32_t size = aString.Size();
		for(uint32_t i = 0; i < size; ++i) {
			string += _case.ToUpperCase(aString[i]);
		}
		return string;
	}


	template<class T, const class CASE = typename DefaultCase<T>::Type, const T TEMINATOR = '\0'>
	TerminatedString<T, TEMINATOR> SOLAIRE_EXPORT_CALL ToggleCase(const ConstString<T>& aString) throw() {
		const CASE _case;
		TerminatedString<T, TEMINATOR> string;
		const uint32_t size = aString.Size();
		for(uint32_t i = 0; i < size; ++i) {
			string += _case.ToggleCase(aString[i]);
		}
		return string;
	}

	template<class T, const class CASE = typename DefaultCase<T>::Type>
	bool SOLAIRE_EXPORT_CALL IsLowerCase(const ConstString<T>& aString) throw() {
		const CASE _case;
		const uint32_t size = aString.Size();
		for(uint32_t i = 0; i < size; ++i) {
			const T c = aString[i];
			if(_case.IsUpperCase(c)) return false;
		}
		return true;
	}

	template<class T, const class CASE = typename DefaultCase<T>::Type>
	bool SOLAIRE_EXPORT_CALL IsUpperCase(const ConstString<T>& aString) throw() {
		const CASE _case;
		const uint32_t size = aString.Size();
		for (uint32_t i = 0; i < size; ++i) {
			const T c = aString[i];
			if (_case.IsLowerCase(c)) return false;
		}
		return true;
	}
}

#endif
