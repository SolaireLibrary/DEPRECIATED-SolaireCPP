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

#include <stdexcept>
#include <cstring>
#include "Solaire\Core\Init.hpp"
#include "Solaire\Strings\StringFragment.hpp"

namespace Solaire {

    // StringComparison

    StringComparison::StringComparison(bool aIgnoreCase, uint16_t aLength) :
        ignoreCase(aIgnoreCase),
        length(aLength)
    {}

    int8_t StringComparison::operator()(const ConstStringFragment aFirst, const ConstStringFragment aSecond) const{
        typedef ConstStringFragment::ConstIterator Iterator;

        const Iterator b1 = aFirst.begin();
        const Iterator b2 = aSecond.begin();
        const Iterator e1 = aFirst.end();
        const Iterator e2 = aSecond.end();

        const uint16_t s1 = e1 - b1;
        const uint16_t s2 = e2 - b2;

        if(length == FULL_LENGTH){
            const uint16_t s3 = Min(s1, s2);
            if(ignoreCase){
                Iterator i = b1;
                Iterator j = b2;
                const Iterator e = i + s3;
                while(i != e){
                    char c1 = *i++;
                    char c2 = *j++;
                    if(c1 >= 'A' && c1 <= 'Z') c1 -= 'A' - 'a';
                    if(c2 >= 'A' && c2 <= 'Z') c2 -= 'A' - 'a';
                    if(c1 < c2){
                        return -1;
                    }else if(c1 > c2){
                        return 1;
                    }
                }
                return 0;
            }else{
                return std::memcmp(b1, b2, sizeof(ConstStringFragment::Type) * s3);
            }
        }else{
            if(s1 < length || s2 < length) throw std::runtime_error("StringComparison::operator() : Fragment length is less than comparison length");
            return std::memcmp(b1, b2, sizeof(ConstStringFragment::Type) * length);
        }
    }

	// StringFragment

	StringFragment::StringFragment(const Pointer aBegin, const Pointer aEnd) :
		mBegin(aBegin),
		mEnd(aEnd)
	{}

	StringFragment::StringFragment(const Pointer aBegin, const size_t aLength) :
		mBegin(aBegin),
		mEnd(aBegin + aLength)
	{}

	StringFragment::StringFragment(const Pointer aBegin) :
		mBegin(aBegin),
		mEnd(aBegin + std::strlen(aBegin))
	{}

	size_t StringFragment::Size() const{
		return mEnd - mBegin;
	}

	StringFragment::Iterator StringFragment::begin(){
		return mBegin;
	}

	StringFragment::ConstIterator StringFragment::begin() const{
		return mBegin;
	}

	StringFragment::Iterator StringFragment::end(){
		return mEnd;
	}

	StringFragment::ConstIterator StringFragment::end() const{
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

	StringFragment::Type StringFragment::operator[](const size_t aIndex) const{
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
		return std::memcmp(mBegin, aOther.mBegin, Min(Size(), aOther.Size())) < 0;
	}

    bool StringFragment::operator>(const StringFragment aOther) const{
		return std::memcmp(mBegin, aOther.mBegin, Min(Size(), aOther.Size())) > 0;
	}

    bool StringFragment::operator<=(const StringFragment aOther) const{
		return std::memcmp(mBegin, aOther.mBegin, Min(Size(), aOther.Size())) <= 0;
	}

    bool StringFragment::operator>=(const StringFragment aOther) const{
		return std::memcmp(mBegin, aOther.mBegin, Min(Size(), aOther.Size())) >= 0;
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

    StringFragment StringFragment::FindFirst(const ConstStringFragment aFragment){
        return const_cast<const StringFragment*>(this)->FindFirst(aFragment).mFragment;
    }

    StringFragment StringFragment::FindNext(const Iterator aPos, const ConstStringFragment aFragment){
        return const_cast<const StringFragment*>(this)->FindNext(const_cast<Iterator>(aPos), aFragment).mFragment;;
    }

    StringFragment StringFragment::FindLast(const ConstStringFragment aFragment){
        return const_cast<const StringFragment*>(this)->FindLast(aFragment).mFragment;;
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

    ConstStringFragment StringFragment::FindFirst(const ConstStringFragment aFragment) const{
        return FindNext(begin(), aFragment);
    }

    ConstStringFragment StringFragment::FindNext(const ConstIterator aPos, const ConstStringFragment aFragment) const{
        ConstStringFragment thisFrag(aPos, end());

        const size_t fragSize = aFragment.Size();
        const size_t thisSize = thisFrag.Size();

        if(fragSize > thisSize){
            return ConstStringFragment(thisFrag.end(), thisFrag.end());
        }else if(fragSize == thisSize){
            return ConstStringFragment(thisFrag == aFragment ? thisFrag.begin() :  thisFrag.end(), thisFrag.end());
        }else{
            const size_t dif = thisSize - fragSize;
            const ConstIterator end = thisFrag.begin() + dif + 1;
            for(ConstIterator i = thisFrag.begin(); i != end; ++i){
                if(ConstStringFragment(i, i + fragSize) == aFragment) return i;
            }
            return ConstStringFragment(thisFrag.end(), thisFrag.end());
        }
    }

    ConstStringFragment StringFragment::FindLast(const ConstStringFragment aFragment) const{
        const ConstIterator end = this->end();
        ConstStringFragment it = FindFirst(aFragment);
        ConstStringFragment prev(end, end);
        while(it.begin() != end){
            prev = it;
            it = FindNext(it.begin() + 1, aFragment);
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
        if(size != aReplacement.Size()) throw std::runtime_error("StringFragment::ReplaceNext : String fragments must be of equal length");

        const Iterator pos = const_cast<Iterator>(FindNext(aPos, aTarget).begin());
        const ConstIterator end = this->end();
        if(pos != end){
            const size_t dif = end - pos;
            if(dif < size) throw std::runtime_error("StringFragment::ReplaceNext : String fragment size exceeded");
            std::memcpy(pos, aTarget.begin(), sizeof(Type) * size);
        }

        return *this;
    }

    StringFragment& StringFragment::ReplaceLast(const ConstStringFragment aTarget, const ConstStringFragment aReplacement){
        return ReplaceNext(FindLast(aTarget).begin(), aTarget, aReplacement);
    }

    StringFragment& StringFragment::ReplaceAll(const ConstStringFragment aTarget, const ConstStringFragment aReplacement){
        const ConstIterator end = this->end();
        ConstIterator it = FindFirst(aTarget).begin();
        while(it != end){
            ReplaceNext(it, aTarget, aReplacement);
            it = FindNext(it + 1, aTarget).begin();
        }
        return *this;
    }


	// ConstStringFragment

	ConstStringFragment::ConstStringFragment(const ConstPointer aBegin, const ConstPointer aEnd) :
		mFragment(const_cast<StringFragment::Pointer>(aBegin), const_cast<StringFragment::Pointer>(aEnd))
	{}

	ConstStringFragment::ConstStringFragment(const ConstPointer aBegin, const size_t aLength) :
		mFragment(const_cast<StringFragment::Pointer>(aBegin), aLength)
	{}

	ConstStringFragment::ConstStringFragment(const ConstPointer aBegin) :
		mFragment(const_cast<StringFragment::Pointer>(aBegin))
	{}


    ConstStringFragment::ConstStringFragment(const StringFragment aOther) :
		mFragment(aOther)
    {}

	size_t ConstStringFragment::Size() const{
		return mFragment.Size();
	}

	ConstStringFragment::ConstIterator ConstStringFragment::begin() const{
		return mFragment.begin();
	}

	ConstStringFragment::ConstIterator ConstStringFragment::end() const{
		return mFragment.end();
	}

    ConstStringFragment::ConstReverseIterator ConstStringFragment::rbegin() const{
		return mFragment.rbegin();
	}

	ConstStringFragment::ConstReverseIterator ConstStringFragment::rend() const{
		return mFragment.rend();
	}

	ConstStringFragment::Type ConstStringFragment::operator[](const size_t aIndex) const{
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

    ConstStringFragment ConstStringFragment::FindFirst(const ConstStringFragment aFragment) const{
        return mFragment.FindFirst(aFragment);
    }

    ConstStringFragment ConstStringFragment::FindNext(const ConstIterator aPos, const ConstStringFragment aFragment) const{
        return mFragment.FindNext(aPos, aFragment);
    }

    ConstStringFragment ConstStringFragment::FindLast(const ConstStringFragment aFragment) const{
        return mFragment.FindLast(aFragment);
    }

}
