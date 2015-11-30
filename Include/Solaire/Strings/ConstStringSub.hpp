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
	
		const T& SOLAIRE_EXPORT_CALL operator[](const uint32_t aIndex) const throw() override {
			return mString[aIndex + mBegin];
		}
	
		uint32_t SOLAIRE_EXPORT_CALL Size() const throw() override {
			return mEnd - mBegin;
		}
	
		bool SOLAIRE_EXPORT_CALL IsContiguous() const throw() override {
			return mString.IsContiguous();
		}
	
		void SOLAIRE_EXPORT_CALL Destructor() throw() override {
	
		}
	};

}


#endif
