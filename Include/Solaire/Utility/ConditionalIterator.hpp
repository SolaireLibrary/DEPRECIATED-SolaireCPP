#ifndef SOLAIRE_UTILITY_CONDITIONAL_ITERATOR_HPP
#define SOLAIRE_UTILITY_CONDITIONAL_ITERATOR_HPP

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
	\file ConditionalIterator.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 17th September 2015
	Last Modified	: 17th September 2015
*/

#include <functional>
#include <stdexcept>

namespace Solaire{ namespace Utility{

	template<class T, class CONST_T, class ITERATOR>
	class ConditionalIterator{
	public:
		typedef ConditionalIterator<T, CONST_T, ITERATOR> this_t;
		typedef std::function<bool(ITERATOR)> condition_t;
	private:
		ITERATOR mBegin;
		ITERATOR mEnd;
		ITERATOR mIterator;
		condition_t mCondition;
	public:
		ConditionalIterator() :
			mBegin(),
			mEnd(),
			mIterator(),
			mCondition([](ITERATOR aIt)->bool{throw std::runtime_error("Condition not set");})
		{}

		ConditionalIterator(const ITERATOR aIterator, const ITERATOR aBegin, const ITERATOR aEnd, const condition_t aCondition) :
			mBegin(aBegin),
			mEnd(aEnd),
			mIterator(aIterator),
			mCondition(aCondition)
		{
			if(mIterator < mEnd){
				while(! mCondition(mIterator) && mIterator < mEnd) ++mIterator;
			}
		}

		this_t& operator++(){
			if(mIterator < mEnd){
				++mIterator;
				while(mIterator < mEnd && ! mCondition(mIterator)) ++mIterator;
			}
			return *this;
		}

		this_t operator++(int){
			this_t tmp(*this);
			++*this;
			return tmp;
		}

		this_t& operator--(){
			if(mIterator > mBegin){
				--mIterator;
				while(mIterator > mBegin && ! mCondition(mIterator)) --mIterator;
			}
			return *this;
		}

		this_t operator--(int){
			this_t tmp(*this);
			--*this;
			return tmp;
		}

		this_t& operator+=(const size_t aNumber){
			mIterator += aNumber;
			return *this;
		}

		this_t operator+(const size_t aNumber) const{
			return this_t(mIterator, mCondition) += aNumber;
		}

		this_t& operator-=(const size_t aNumber){
			mIterator -= aNumber;
			return *this;
		}

		this_t operator-(const size_t aNumber) const{
			return this_t(mIterator, mCondition) -= aNumber;
		}

		T operator*(){
			return *mIterator;
		}

		CONST_T operator*() const {
			return *mIterator;
		}

		T operator->(){
			return *mIterator;
		}

		CONST_T operator->() const {
			return *mIterator;
		}

		bool operator==(const this_t aOther) const{
			return mIterator == aOther.mIterator;
		}

		bool operator!=(const this_t aOther) const{
			return mIterator == aOther.mIterator;
		}

		bool operator<(const this_t aOther) const{
			return mIterator < aOther.mIterator;
		}

		bool operator>(const this_t aOther) const{
			return mIterator > aOther.mIterator;
		}

		bool operator<=(const this_t aOther) const{
			return mIterator <= aOther.mIterator;
		}

		bool operator>=(const this_t aOther) const{
			return mIterator >= aOther.mIterator;
		}
	};
}}


#endif
