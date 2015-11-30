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
	Last Modified	: 30th November 2015
*/

#include "ConstString.hpp"
	
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


#endif
