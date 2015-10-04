#ifndef SOLAIRE_CORE_REVERSE_ITERATOR_HPP
#define SOLAIRE_CORE_REVERSE_ITERATOR_HPP

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
	\file DereferenceIterator.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 2.0
	\date
	Created			: 27th September 2015
	Last Modified	: 27th September 2015
*/

namespace Solaire{

	template<class T, class ITERATOR>
	class ReverseIteratorWrapper{
	private:
		ITERATOR mIterator;
	public:
		typedef T Type;
		typedef Type& Reference;
		typedef Type* Pointer;
		typedef ReverseIteratorWrapper<Type, ITERATOR> Self;

		ReverseIteratorWrapper() :
			mIterator()
		{}

		ReverseIteratorWrapper(const ITERATOR aOther) :
			mIterator(aOther)
		{}

		inline size_t operator-(const Self aOther) const{
			return mIterator - aOther.mIterator;
		}

		inline Self& operator++(){
			--mIterator;
			return *this;
		}

		inline Self operator++(int){
			return Self(mIterator--);
		}

		inline Self& operator--(){
			++mIterator;
			return *this;
		}

		inline Self operator--(int){
			return Self(mIterator++);
		}

		inline Self& operator+=(const size_t aNumber){
			mIterator -= aNumber;
			return *this;
		}

		Self operator+(const size_t aNumber) const{
			return Self(mIterator - aNumber); //! \TODO Check if this is correct
		}

		inline Self& operator-=(const size_t aNumber){
			mIterator += aNumber;
			return *this;
		}

		inline Self operator-(const size_t aNumber) const{
			return Self(mIterator + aNumber);
		}

		inline Reference operator*() const{
			return *mIterator;
		}

		inline Pointer operator->(){
			return &operator*();
		}

		inline bool operator==(const Self aOther) const{
			return mIterator == aOther.mIterator;
		}

		inline bool operator!=(const Self aOther) const{
			return mIterator != aOther.mIterator;
		}

		inline bool operator<(const Self aOther) const{
			return mIterator < aOther.mIterator;
		}

		inline bool operator>(const Self aOther) const{
			return mIterator > aOther.mIterator;
		}

		inline bool operator<=(const Self aOther) const{
			return mIterator <= aOther.mIterator;
		}

		inline bool operator>=(const Self aOther) const{
			return mIterator >= aOther.mIterator;
		}
	};
}


#endif
