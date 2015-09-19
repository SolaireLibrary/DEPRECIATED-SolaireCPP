#ifndef SOLAIRE_UTILITY_STRING_FRAGMENT_INL
#define SOLAIRE_UTILITY_STRING_FRAGMENT_INL

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
\file StringFragment.inl
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 18th September 2015
Last Modified	: 18th September 2015
*/

#include "..\Hash.hpp"

namespace Solaire { namespace Utility {

	// StringFragment

	constexpr StringFragment::StringFragment(char* const aBegin, const char* const aEnd) :
		mBegin(aBegin),
		mEnd(aEnd)
	{}

	constexpr StringFragment::StringFragment(char* const aBegin, const size_t aLength) :
		mBegin(aBegin),
		mEnd(aBegin + aLength)
	{}

	StringFragment::StringFragment(std::string& aString) :
		mBegin(&aString[0]),
		mEnd(&aString[0] + aString.size())
	{}

	constexpr size_t StringFragment::Size() const{
		return mEnd - mBegin;
	}

	char* StringFragment::begin(){
		return mBegin;
	}

	constexpr const char* StringFragment::begin() const{
		return mBegin;
	}

	constexpr const char* StringFragment::end() const{
		return mEnd;
	}

	char& StringFragment::operator[](const size_t aIndex){
		return mBegin[aIndex];
	}

	constexpr char StringFragment::operator[](const size_t aIndex) const{
		return mBegin[aIndex];
	}

	bool StringFragment::operator==(const StringFragment aOther) const{
		const size_t size = Size();
		if(aOther.Size() != size) return false;
		return std::memcmp(mBegin, aOther.mBegin, size) == 0;
	}

	bool StringFragment::operator!=(const StringFragment aOther) const{
		const size_t size = Size();
		if(aOther.Size() != size) return true;
		return std::memcmp(mBegin, aOther.mBegin, size) != 0;
	}

	StringFragment::operator std::string() const{
		return std::string(mBegin, Size());
	}

	// ConstStringFragment

	constexpr ConstStringFragment::ConstStringFragment(const char* const aBegin, const char* const aEnd) :
		mFragment(const_cast<char*>(aBegin), aEnd)
	{}

	constexpr ConstStringFragment::ConstStringFragment(const char* const aBegin, const size_t aLength) :
		mFragment(const_cast<char*>(aBegin), aLength)
	{}

	ConstStringFragment::ConstStringFragment(const std::string& aString) :
		mFragment(const_cast<std::string&>(aString))
	{}

	constexpr size_t ConstStringFragment::Size() const{
		return mFragment.Size();
	}

	constexpr const char* ConstStringFragment::begin() const{
		return mFragment.begin();
	}

	constexpr const char* ConstStringFragment::end() const{
		return mFragment.end();
	}

	constexpr char ConstStringFragment::operator[](const size_t aIndex) const{
		return mFragment[aIndex];
	}

	bool ConstStringFragment::operator==(const ConstStringFragment aOther) const{
		return mFragment == aOther.mFragment;
	}

	bool ConstStringFragment::operator!=(const ConstStringFragment aOther) const{
		return mFragment == aOther.mFragment;
	}

	ConstStringFragment::operator std::string() const{
		return mFragment;
	}

	// Other

	template<>
	uint32_t Hash<StringFragment>(const StringFragment& aData, HashFunction aFunction) {
		return aFunction(aData.begin(), aData.Size());
	}
}}


#endif
