#ifndef SOLAIRE_UTILITY_WORD_ITERATOR_HPP
#define SOLAIRE_UTILITY_WORD_ITERATOR_HPP

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
	\file WordIterator.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 17th September 2015
	Last Modified	: 17th September 2015
*/

#include "StringFragment.hpp"
#include "..\StringProcessor.hpp"

namespace Solaire{ namespace Utility{

	class WordIterator{
	private:
		const char* mBegin;
		const char* mEnd;
		char* mCurrent;
		size_t mCurrentLength;
	public:
		WordIterator() :
			mBegin(nullptr),
			mEnd(nullptr),
			mCurrent(nullptr),
			mCurrentLength(0)
		{}

		WordIterator(char* const aBegin, const char* const aEnd) :
			mBegin(aBegin),
			mEnd(aEnd),
			mCurrent(aBegin),
			mCurrentLength(0)
		{
			++*this;
		}

		WordIterator(char* const aBegin) :
			mBegin(aBegin),
			mEnd(aBegin + std::strlen(aBegin)),
			mCurrent(aBegin),
			mCurrentLength(0)
		{
			++*this;
		}

		WordIterator(std::string& aString) :
			mBegin(&aString[0]),
			mEnd(&aString[0] + aString.size()),
			mCurrent(&aString[0]),
			mCurrentLength(0)
		{
			++*this;
		}

		WordIterator End() const{
			return WordIterator(const_cast<char*>(mEnd), mEnd);
		}

		size_t operator-(const WordIterator aOther) const{
			if(mBegin != aOther.mBegin) throw std::runtime_error("Iterators are not matched");

			size_t count = 0;
			WordIterator tmp(*this);
			while(tmp != aOther && tmp.mCurrent > mBegin) {
				--tmp;
				++count;
			}
			return count;
		}

		WordIterator& operator++(){
			if(mCurrent == mEnd) return *this;

			// Move to end of word
			mCurrent += mCurrentLength + 1;

			mCurrentLength = 0;

			// Find beginning of word
			do{
				if(Strings::IsWhitespace(*mCurrent)){
					++mCurrent;
				}else{
					break;
				}
			}while(mCurrent < mEnd);

			// Find end of word
			const char* tmp = mCurrent;
			do{
				if(! Strings::IsWhitespace(*tmp)){
					++tmp;
					++mCurrentLength;
				}else{
					break;
				}
			}while(tmp < mEnd);

			return *this;
		}

		WordIterator operator++(int){
			WordIterator tmp(*this);
			++*this;
			return tmp;
		}

		WordIterator& operator--(){
			if (mCurrent == mBegin) return *this;

			// Move to (before) beginning of word
			--mCurrent;

			mCurrentLength = 0;

			// Find end of word
			do{
				if(Strings::IsWhitespace(*mCurrent)){
					--mCurrent;
				}else{
					break;
				}
			}while(mCurrent > mBegin);

			// Find begin of word
			const char* tmp = mCurrent;
			do{
				if(! Strings::IsWhitespace(*tmp)){
					--tmp;
					++mCurrentLength;
				}else{
					break;
				}
			}while(tmp > mBegin);

			return *this;
		}

		WordIterator operator--(int){
			WordIterator tmp(*this);
			--*this;
			return tmp;
		}

		WordIterator& operator+=(const size_t aNumber){
			for (size_t i = 0; i < aNumber; ++i) ++*this;
			return *this;
		}

		WordIterator operator+(const size_t aNumber) const{
			return WordIterator(*this) += aNumber;
		}

		WordIterator& operator-=(const size_t aNumber) {
			for (size_t i = 0; i < aNumber; ++i)--*this;
			return *this;
		}

		WordIterator operator-(const size_t aNumber) const {
			return WordIterator(*this) -= aNumber;
		}

		StringFragment operator*(){
			return StringFragment(mCurrent, mCurrent + mCurrentLength);
		}

		ConstStringFragment operator*() const{
			return ConstStringFragment(mCurrent, mCurrent + mCurrentLength);
		}

		StringFragment operator->(){
			return StringFragment(mCurrent, mCurrent + mCurrentLength);
		}

		ConstStringFragment operator->() const{
			return ConstStringFragment(mCurrent, mCurrent + mCurrentLength);
		}

		bool operator==(const WordIterator aOther) const {
			return mCurrent == aOther.mCurrent;
		}

		bool operator!=(const WordIterator aOther) const {
			return mCurrent == aOther.mCurrent;
		}

		bool operator<(const WordIterator aOther) const {
			return mCurrent < aOther.mCurrent;
		}

		bool operator>(const WordIterator aOther) const {
			return mCurrent > aOther.mCurrent;
		}

		bool operator<=(const WordIterator aOther) const {
			return mCurrent <= aOther.mCurrent;
		}

		bool operator>=(const WordIterator aOther) const {
			return mCurrent >= aOther.mCurrent;
		}
	};

	class ConstWordIterator {
	private:
		WordIterator mIterator;
	public:
		ConstWordIterator() :
			mIterator()
		{}

		ConstWordIterator(const WordIterator aOther) :
			mIterator(aOther)
		{}

		ConstWordIterator(const char* const aBegin, const char* const aEnd) :
			mIterator(const_cast<char*>(aBegin), aEnd)
		{}

		ConstWordIterator(const char* const aBegin) :
			mIterator(const_cast<char*>(aBegin))
		{}

		ConstWordIterator(const std::string& aString) :
			mIterator(const_cast<std::string&>(aString))
		{}

		ConstWordIterator End() const {
			return mIterator.End();
		}

		ConstWordIterator& operator++() {
			++mIterator;
			return *this;
		}

		ConstWordIterator operator++(int) {
			ConstWordIterator tmp(*this);
			++*this;
			return tmp;
		}

		ConstWordIterator& operator--() {
			--mIterator;
			return *this;
		}

		ConstWordIterator operator--(int) {
			ConstWordIterator tmp(*this);
			--*this;
			return tmp;
		}

		ConstWordIterator& operator+=(const size_t aNumber) {
			mIterator += aNumber;
			return *this;
		}

		ConstWordIterator operator+(const size_t aNumber) const {
			return mIterator + aNumber;
		}

		ConstWordIterator& operator-=(const size_t aNumber) {
			mIterator -= aNumber;
			return *this;
		}

		ConstWordIterator operator-(const size_t aNumber) const {
			return mIterator - aNumber;
		}

		ConstStringFragment operator*() const {
			return *mIterator;
		}

		ConstStringFragment operator->() const {
			return *mIterator;
		}

		bool operator==(const ConstWordIterator aOther) const {
			return mIterator == aOther.mIterator;
		}

		bool operator!=(const ConstWordIterator aOther) const {
			return mIterator == aOther.mIterator;
		}

		bool operator<(const ConstWordIterator aOther) const {
			return mIterator < aOther.mIterator;
		}

		bool operator>(const ConstWordIterator aOther) const {
			return mIterator > aOther.mIterator;
		}

		bool operator<=(const ConstWordIterator aOther) const {
			return mIterator <= aOther.mIterator;
		}

		bool operator>=(const ConstWordIterator aOther) const {
			return mIterator >= aOther.mIterator;
		}
	};
}}


#endif
