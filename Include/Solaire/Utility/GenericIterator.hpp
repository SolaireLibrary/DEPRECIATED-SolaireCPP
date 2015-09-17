#ifndef SOLAIRE_UTILITY_GENERIC_ITERATOR_HPP
#define SOLAIRE_UTILITY_GENERIC_ITERATOR_HPP

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
	\file GenericIterator.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 17th September 2015
	Last Modified	: 17th September 2015
*/

namespace Solaire{ namespace Utility{

	template<class T, class ITERATOR>
	class ConstGenericIterator{
	private:
		ITERATOR mIterator;
	public:
		typedef ConstGenericIterator<T, ITERATOR> this_t;

		ConstGenericIterator() :
			mIterator()
		{}

		ConstGenericIterator(const ITERATOR aOther) :
			mIterator(aOther)
		{}

		size_t operator-(const this_t aOther) const{
			return mIterator - aOther.mIterator;
		}

		this_t& operator++(){
			++mIterator;
			return *this;
		}

		this_t operator++(int){
			this_t tmp(*this);
			++*this;
			return tmp;
		}

		this_t& operator--(){
			--mIterator;
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
			return mIterator + aNumber;
		}

		this_t& operator-=(const size_t aNumber){
			mIterator -= aNumber;
			return *this;
		}

		this_t operator-(const size_t aNumber) const{
			return mIterator - aNumber;
		}

		T operator*() const{
			return *mIterator;
		}

		T operator->() const{
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
