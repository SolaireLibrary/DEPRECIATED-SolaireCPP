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
	
		virtual uint32_t SOLAIRE_EXPORT_CALL FindFirstChar(const T) const throw() = 0;
		virtual uint32_t SOLAIRE_EXPORT_CALL FindNextChar(const T, const uint32_t) const throw() = 0;
		virtual uint32_t SOLAIRE_EXPORT_CALL FindLastChar(const T) const throw() = 0;
	
		virtual uint32_t SOLAIRE_EXPORT_CALL FindFirst(const ConstString<T>&) const throw() = 0;
		virtual uint32_t SOLAIRE_EXPORT_CALL FindNext(const ConstString<T>&, const uint32_t) const throw() = 0;
		virtual uint32_t SOLAIRE_EXPORT_CALL FindLast(const ConstString<T>&) const throw() = 0;
	 
		virtual bool SOLAIRE_EXPORT_CALL operator==(const ConstString<T>&) const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL operator!=(const ConstString<T>&) const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL operator<(const ConstString<T>&) const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL operator>(const ConstString<T>&) const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL operator<=(const ConstString<T>&) const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL operator>=(const ConstString<T>&) const throw() = 0;
	
		virtual bool SOLAIRE_EXPORT_CALL IsContiguous() const throw() = 0;
	
		virtual void SOLAIRE_EXPORT_CALL Destructor() throw() = 0;

		inline const T* SOLAIRE_EXPORT_CALL GetContiguousPtr() const throw() {
			return IsContiguous() ? &operator[](0) : nullptr;
		}
	};
    
}

#endif
