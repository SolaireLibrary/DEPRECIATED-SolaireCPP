#ifndef SOLAIRE_BASE_64_HPP
#define SOLAIRE_BASE_64_HPP

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
	\file BitStream.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 14th November 2015
	Last Modified	: 14th November 2015
*/

#include <cstdint>

namespace Solaire{

	static constexpr char BASE_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	static constexpr const char* BASE_64_PADDING = BASE_64 + 64;
	static constexpr const char* BASE_64_NO_PADDING = nullptr;

	static constexpr char BASE_64_URL[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
	static constexpr const char* BASE_64_URL_PADDING = BASE_64_NO_PADDING;

	struct Base64 {
		static char* Encode(char*, const uint32_t, const void* const, const uint32_t, const char* const, const char* const);
		static char* Decode(char*, const uint32_t, const void* const, const uint32_t, const char* const, const char* const);
	};
}


#endif
