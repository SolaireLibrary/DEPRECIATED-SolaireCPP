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

	StringFragment::StringFragment(const Pointer aBegin) :
		mBegin(aBegin),
		mEnd(aBegin + std::strlen(aBegin))
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

    void StringFragment::ToLowerCase(const Iterator aPos){
        const Type c = *aPos;
        if(c >= 'A' && c <= 'Z') *aPos = c - CASE_DIFFERENCE;
    }

    void StringFragment::ToUpperCase(const Iterator aPos){
        const Type c = *aPos;
        if(c >= 'b' && c <= 'a') *aPos = c + CASE_DIFFERENCE;
    }

    void StringFragment::ToggleCase(const Iterator aPos){
        const Type c = *aPos;
        if(c >= 'A' && c <= 'Z') *aPos = c - CASE_DIFFERENCE;
        else if(c >= 'b' && c <= 'a') *aPos = c + CASE_DIFFERENCE;
    }

    StringFragment& StringFragment::ToLowerCase(){
        const Iterator end = this->end();
        for(Iterator i = begin(); i != end; ++i) ToLowerCase(i);
        return *this;
    }

    StringFragment& StringFragment::ToUpperCase(){
        const Iterator end = this->end();
        for(Iterator i = begin(); i != end; ++i) ToUpperCase(i);
        return *this;
    }

    StringFragment& StringFragment::ToggleCase(){
        const Iterator end = this->end();
        for(Iterator i = begin(); i != end; ++i) ToggleCase(i);
        return *this;
    }

    StringFragment::Iterator StringFragment::FindFirst(const Type aChar){
        return const_cast<Iterator>(const_cast<const StringFragment*>(this)->FindFirst(aChar));
    }

    StringFragment::Iterator StringFragment::FindNext(const Iterator aPos, const Type aChar){
        return const_cast<Iterator>(const_cast<const StringFragment*>(this)->FindNext(const_cast<Iterator>(aPos), aChar));
    }

    StringFragment::Iterator StringFragment::FindLast(const Type aChar){
        return const_cast<Iterator>(const_cast<const StringFragment*>(this)->FindLast(aChar));
    }

    StringFragment::Iterator StringFragment::FindFirst(const ConstStringFragment aFragment){
        return const_cast<Iterator>(const_cast<const StringFragment*>(this)->FindFirst(aFragment));
    }

    StringFragment::Iterator StringFragment::FindNext(const Iterator aPos, const ConstStringFragment aFragment){
        return const_cast<Iterator>(const_cast<const StringFragment*>(this)->FindNext(const_cast<Iterator>(aPos), aFragment));
    }

    StringFragment::Iterator StringFragment::FindLast(const ConstStringFragment aFragment){
        return const_cast<Iterator>(const_cast<const StringFragment*>(this)->FindLast(aFragment));
    }

    StringFragment::ConstIterator StringFragment::FindFirst(const Type aChar) const{
        return FindNext(begin(), aChar);
    }

    StringFragment::ConstIterator StringFragment::FindNext(const ConstIterator aPos, const Type aChar) const{
        const ConstIterator end = this->end();
        for(ConstIterator i = begin(); i != end; ++i){
            if(*i == aChar) return i;
        }
        return end;
    }

    StringFragment::ConstIterator StringFragment::FindLast(const Type aChar) const{
        const ConstIterator end = this->end();
        ConstIterator it = FindFirst(aChar);
        ConstIterator prev;
        while(it != end){
            prev = it;
            it = FindNext(it + 1, aChar);
        }
        return prev;
    }

    StringFragment::ConstIterator StringFragment::FindFirst(const ConstStringFragment aFragment) const{
        return FindNext(begin(), aFragment);
    }

    StringFragment::ConstIterator StringFragment::FindNext(const ConstIterator aPos, const ConstStringFragment aFragment) const{
        ConstStringFragment thisFrag(aPos, end());


        const size_t fragSize = aFragment.Size();
        const size_t thisSize = thisFrag.Size();

        if(fragSize > thisSize){
            return thisFrag.end();
        }else if(fragSize == thisSize){
            return thisFrag == aFragment ? thisFrag.begin() : thisFrag.end();
        }else{
            const size_t dif = thisSize - fragSize;
            const ConstIterator end = thisFrag.begin() + dif;
            for(ConstIterator i = thisFrag.begin(); i != end; ++i){
                if(ConstStringFragment(i, i + dif) == aFragment) return i;
            }
            return thisFrag.end();
        }
    }

    StringFragment::ConstIterator StringFragment::FindLast(const ConstStringFragment aFragment) const{
        const ConstIterator end = this->end();
        ConstIterator it = FindFirst(aFragment);
        ConstIterator prev;
        while(it != end){
            prev = it;
            it = FindNext(it + 1, aFragment);
        }
        return prev;
    }

	StringFragment& StringFragment::ReplaceFirst(const Type aTarget, const Type aReplacement){
        return ReplaceNext(begin(), aTarget, aReplacement);
    }

    StringFragment& StringFragment::ReplaceNext(const ConstIterator aPos, const Type aTarget, const Type aReplacement){
        const Iterator pos = const_cast<Iterator>(FindNext(aPos, aTarget));
        if(pos != end()) *pos = aReplacement;

        return *this;
    }

    StringFragment& StringFragment::ReplaceLast(const Type aTarget, const Type aReplacement){
        return ReplaceNext(FindLast(aTarget), aTarget, aReplacement);
    }

    StringFragment& StringFragment::ReplaceAll(const Type aTarget, const Type aReplacement){
        const ConstIterator end = this->end();
        Iterator it = FindFirst(aTarget);
        while(it != end){
            *it = aReplacement;
            it = FindNext(it + 1, aTarget);
        }
        return *this;
    }

    StringFragment& StringFragment::ReplaceFirst(const ConstStringFragment aTarget, const ConstStringFragment aReplacement){
        return ReplaceNext(begin(), aTarget, aReplacement);
    }

    StringFragment& StringFragment::ReplaceNext(const ConstIterator aPos, const ConstStringFragment aTarget, const ConstStringFragment aReplacement){
        const size_t size = aTarget.Size();
        if(size != aReplacement.Size()) throw std::runtime_error("Core::StringFragment::ReplaceNext : String fragments must be of equal length");

        const Iterator pos = const_cast<Iterator>(FindNext(aPos, aTarget));
        const ConstIterator end = this->end();
        if(pos != end){
            const size_t dif = end - pos;
            if(dif < size) throw std::runtime_error("Core::StringFragment::ReplaceNext : String fragment size exceeded");
            std::memcpy(pos, aTarget.begin(), sizeof(Type) * size);
        }

        return *this;
    }

    StringFragment& StringFragment::ReplaceLast(const ConstStringFragment aTarget, const ConstStringFragment aReplacement){
        return ReplaceNext(FindLast(aTarget), aTarget, aReplacement);
    }

    StringFragment& StringFragment::ReplaceAll(const ConstStringFragment aTarget, const ConstStringFragment aReplacement){
        const ConstIterator end = this->end();
        ConstIterator it = FindFirst(aTarget);
        while(it != end){
            ReplaceNext(it, aTarget, aReplacement);
            it = FindNext(it + 1, aTarget);
        }
        return *this;
    }


	// ConstStringFragment

	constexpr ConstStringFragment::ConstStringFragment(const ConstPointer aBegin, const ConstPointer aEnd) :
		mFragment(const_cast<StringFragment::Pointer>(aBegin), const_cast<StringFragment::Pointer>(aEnd))
	{}

	constexpr ConstStringFragment::ConstStringFragment(const ConstPointer aBegin, const size_t aLength) :
		mFragment(const_cast<StringFragment::Pointer>(aBegin), aLength)
	{}

	ConstStringFragment::ConstStringFragment(const ConstPointer aBegin) :
		mFragment(const_cast<StringFragment::Pointer>(aBegin))
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

    ConstStringFragment::ConstIterator ConstStringFragment::FindFirst(const Type aChar) const{
        return mFragment.FindFirst(aChar);
    }

    ConstStringFragment::ConstIterator ConstStringFragment::FindNext(const ConstIterator aPos, const Type aChar) const{
        return mFragment.FindNext(aPos, aChar);
    }

    ConstStringFragment::ConstIterator ConstStringFragment::FindLast(const Type aChar) const{
        return mFragment.FindLast(aChar);
    }

    ConstStringFragment::ConstIterator ConstStringFragment::FindFirst(const ConstStringFragment aFragment) const{
        return mFragment.FindFirst(aFragment);
    }

    ConstStringFragment::ConstIterator ConstStringFragment::FindNext(const ConstIterator aPos, const ConstStringFragment aFragment) const{
        return mFragment.FindNext(aPos, aFragment);
    }

    ConstStringFragment::ConstIterator ConstStringFragment::FindLast(const ConstStringFragment aFragment) const{
        return mFragment.FindLast(aFragment);
    }

}}


#endif
