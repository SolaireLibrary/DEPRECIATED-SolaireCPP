#ifndef SOLAIRE_CORE_STRING_FRAGMENT_INL
#define SOLAIRE_CORE_STRING_FRAGMENT_INL

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
Last Modified	: 28th September 2015
*/

namespace Solaire { namespace Core {

	// StringFragment

	constexpr StringFragment::StringFragment(const Pointer aBegin, const Pointer aEnd) :
		mBegin(aBegin),
		mEnd(aEnd)
	{}

	constexpr StringFragment::StringFragment(const Pointer aBegin, const size_t aLength) :
		mBegin(aBegin),
		mEnd(aBegin + aLength)
	{}

	constexpr size_t StringFragment::Size() const{
		return mEnd - mBegin;
	}

	StringFragment::Iterator StringFragment::begin(){
		return mBegin;
	}

	constexpr StringFragment::ConstIterator StringFragment::begin() const{
		return mBegin;
	}

	StringFragment::Iterator StringFragment::end(){
		return mEnd;
	}

	constexpr StringFragment::ConstIterator StringFragment::end() const{
		return mEnd;
	}

	StringFragment::ReverseIterator StringFragment::rbegin(){
		return ReverseIterator(mEnd - 1);
	}

    StringFragment::ConstReverseIterator StringFragment::rbegin() const{
		return ConstReverseIterator(mEnd - 1);
	}

	StringFragment::ReverseIterator StringFragment::rend(){
		return ReverseIterator(mBegin - 1);
	}

	StringFragment::ConstReverseIterator StringFragment::rend() const{
		return ConstReverseIterator(mBegin - 1);
	}

	StringFragment::Reference StringFragment::operator[](const size_t aIndex){
		return mBegin[aIndex];
	}

	constexpr StringFragment::Type StringFragment::operator[](const size_t aIndex) const{
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

    bool StringFragment::operator<(const StringFragment aOther) const{
		return std::memcmp(mBegin, aOther.mBegin, std::min(Size(), aOther.Size())) < 0;
	}

    bool StringFragment::operator>(const StringFragment aOther) const{
		return std::memcmp(mBegin, aOther.mBegin, std::min(Size(), aOther.Size())) > 0;
	}

    bool StringFragment::operator<=(const StringFragment aOther) const{
		return std::memcmp(mBegin, aOther.mBegin, std::min(Size(), aOther.Size())) <= 0;
	}

    bool StringFragment::operator>=(const StringFragment aOther) const{
		return std::memcmp(mBegin, aOther.mBegin, std::min(Size(), aOther.Size())) >= 0;
	}

	// ConstStringFragment

	constexpr ConstStringFragment::ConstStringFragment(const ConstPointer aBegin, const ConstPointer aEnd) :
		mFragment(const_cast<StringFragment::Pointer>(aBegin), const_cast<StringFragment::Pointer>(aEnd))
	{}

	constexpr ConstStringFragment::ConstStringFragment(const ConstPointer aBegin, const size_t aLength) :
		mFragment(const_cast<StringFragment::Pointer>(aBegin), aLength)
	{}

    constexpr ConstStringFragment::ConstStringFragment(const StringFragment aOther) :
		mFragment(aOther)
    {}

	constexpr size_t ConstStringFragment::Size() const{
		return mFragment.Size();
	}

	constexpr ConstStringFragment::ConstIterator ConstStringFragment::begin() const{
		return mFragment.begin();
	}

	constexpr ConstStringFragment::ConstIterator ConstStringFragment::end() const{
		return mFragment.end();
	}

    ConstStringFragment::ConstReverseIterator ConstStringFragment::rbegin() const{
		return mFragment.rbegin();
	}

	ConstStringFragment::ConstReverseIterator ConstStringFragment::rend() const{
		return mFragment.rend();
	}

	constexpr ConstStringFragment::Type ConstStringFragment::operator[](const size_t aIndex) const{
		return mFragment[aIndex];
	}

	bool ConstStringFragment::operator==(const ConstStringFragment aOther) const{
		return mFragment == aOther.mFragment;
	}

	bool ConstStringFragment::operator!=(const ConstStringFragment aOther) const{
		return mFragment == aOther.mFragment;
	}

	bool ConstStringFragment::operator<(const ConstStringFragment aOther) const{
		return mFragment < aOther.mFragment;
	}

	bool ConstStringFragment::operator>(const ConstStringFragment aOther) const{
		return mFragment > aOther.mFragment;
	}

	bool ConstStringFragment::operator<=(const ConstStringFragment aOther) const{
		return mFragment <= aOther.mFragment;
	}

	bool ConstStringFragment::operator>=(const ConstStringFragment aOther) const{
		return mFragment >= aOther.mFragment;
	}
}}


#endif
