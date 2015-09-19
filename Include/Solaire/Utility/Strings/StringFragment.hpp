#ifndef SOLAIRE_UTILITY_STRING_FRAGMENT_HPP
#define SOLAIRE_UTILITY_STRING_FRAGMENT_HPP

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
\file StringFragment.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 17th September 2015
Last Modified	: 18th September 2015
*/

#include <string>

namespace Solaire { namespace Utility {
	class StringFragment{
	private:
		char* mBegin;
		const char* mEnd;
	public:
		constexpr StringFragment(char* const aBegin, const char* const aEnd);
		constexpr StringFragment(char* const aBegin, const size_t aLength);
		StringFragment(std::string& aString);

		char* begin();
		constexpr const char* begin() const;
		constexpr const char* end() const;

		constexpr char operator[](const size_t aIndex) const;
		char& operator[](const size_t aIndex);
		constexpr size_t Size() const;

		bool operator==(const StringFragment aOther) const;
		bool operator!=(const StringFragment aOther) const;

		operator std::string() const;
	};

	class ConstStringFragment{
	private:
		StringFragment mFragment;
	public:
		constexpr ConstStringFragment(const char* const aBegin, const char* const aEnd);
		constexpr ConstStringFragment(const char* const aBegin, const size_t aLength);
		ConstStringFragment(const std::string& aString);

		constexpr const char* begin() const;
		constexpr const char* end() const;

		constexpr char operator[](const size_t aIndex) const;
		constexpr size_t Size() const;

		bool operator==(const ConstStringFragment aOther) const;
		bool operator!=(const ConstStringFragment aOther) const;

		operator std::string() const;
	};
}}

#include "StringFragment.inl"

#endif
