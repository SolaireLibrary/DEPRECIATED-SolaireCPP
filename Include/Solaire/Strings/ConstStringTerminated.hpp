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
	Last Modified	: 3rd December 2015
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

		SOLAIRE_EXPORT_CALL ~ConstTerminatedString() throw() {

		}

		// Inherited from ConstString
	
		const T& SOLAIRE_EXPORT_CALL operator[](const uint32_t aIndex) const throw() override {
			return mString[aIndex];
		}
	
		uint32_t SOLAIRE_EXPORT_CALL Size() const throw() override {
			return mSize;
		}
	
		bool SOLAIRE_EXPORT_CALL IsContiguous() const throw() override {
			return true;
		}
	};

	typedef ConstTerminatedString<char, '\0'> ConstCString;

}


#endif
