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
	\file Base64.hpp
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
	struct Base64 {
		static char* Encode(char*, const uint32_t, const void* const, const uint32_t, const char* const, const char* const);
		static char* Decode(char*, const uint32_t, const void* const, const uint32_t, const char* const, const char* const);

		static constexpr uint32_t UnpaddedPaddingBytes(const uint32_t);
		static uint32_t UnpaddedEncodeLength(const uint32_t);
		static uint32_t UnpaddedDecodeLength(const uint32_t);

		static uint32_t PaddedPaddingBytes(const uint32_t);
		static uint32_t PaddedEncodeLength(const uint32_t);
		static uint32_t PaddedDecodeLength(const uint32_t);
	};

	static constexpr const char* BASE_64_NO_PADDING = nullptr;

	static constexpr char BASE_64_STANDARD[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	static constexpr const char* BASE_64_STANDARD_PADDING = BASE_64_STANDARD + 64;

	static constexpr char BASE_64_URL[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
	static constexpr const char* BASE_64_URL_PADDING = BASE_64_NO_PADDING;

	static constexpr char BASE_64_XML_NAME[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.-";
	static constexpr const char* BASE_64_XML_NAME_PADDING = BASE_64_NO_PADDING;

	static constexpr char BASE_64_XML_IDENTIFIER[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_:";
	static constexpr const char* BASE_64_XML_IDENTIFIER_PADDING = BASE_64_NO_PADDING;
}


#endif
