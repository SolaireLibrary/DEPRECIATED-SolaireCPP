#ifndef SOLAIRE_CORE_CONDITIONAL_ITERATOR_HPP
#define SOLAIRE_CORE_CONDITIONAL_ITERATOR_HPP

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
	\version 2.0
	\date
	Created			: 17th September 2015
	Last Modified	: 27th September 2015
*/

namespace Solaire{

	template<class T, class ITERATOR, class FUNCTION>
	class ConditionalIteratorWrapper{
	private:
		ITERATOR mIterator;
		ITERATOR mBegin;
		ITERATOR mEnd;
		FUNCTION* mCondition;
	public:
		typedef T Type;
		typedef Type& Reference;
		typedef Type* Pointer;
		typedef ConditionalIteratorWrapper<Type, ITERATOR> Self;

		ConditionalIteratorWrapper() :
			mIterator(),
			mBegin(),
			mEnd(),
			mCondition(nullptr)
		{}

		ConditionalIteratorWrapper(const ITERATOR aIterator, const ITERATOR aBegin, const ITERATOR aEnd, FUNCTION& aCondition) :
			mIterator(aIterator),
			mBegin(aBegin),
			mEnd(aEnd),
			mCondition(&aCondition)
		{
		    --mBegin;
		    if(! mCondition(*mIterator)) operator++();
        }

		//inline size_t operator-(const Self aOther) const{
		//	return mIterator - aOther.mIterator;
		//}

		Self& operator++(){
		    if(mIterator != mEnd){
                ++mIterator;
                while(mIterator != mEnd && ! mCondition(*mIterator)){
                    ++mIterator;
                }
		    }
			return *this;
		}

		inline Self operator++(int){
			return Self(operator++());
		}

		Self& operator--(){
		    if(mIterator != mBegin){
                --mIterator;
                while(mIterator != mBegin && ! mCondition(*mIterator)){
                    --mIterator;
                }
			}
			return *this;
		}

		inline Self operator--(int){
			return Self(operator--());
		}

		Self& operator+=(const size_t aNumber){
			while(aNumber > 0){
                operator++();
                --aNumber;
			}
			return *this;
		}

		Self operator+(const size_t aNumber) const{
			Self tmp(*this);
		    tmp += aNumber;
		    return tmp;
		}

		Self& operator-=(size_t aNumber){
			while(aNumber > 0){
                operator--();
                --aNumber;
			}
			return *this;
		}

		inline Self operator-(const size_t aNumber) const{
		    Self tmp(*this);
		    tmp -= aNumber;
		    return tmp;
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
