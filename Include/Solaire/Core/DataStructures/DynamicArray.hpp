#ifndef SOLAIRE_CORE_DYNAMIC_ARRAY_HPP
#define SOLAIRE_CORE_DYNAMIC_ARRAY_HPP

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
	\file DynamicArray.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 25th September 2015
	Last Modified	: 25th September 2015
*/

#include <cstdint>
#include <stdexcept>
#include <functional>
#include "..\Memory\Allocator.hpp"
#include "..\Iterators\ReverseIterator.hpp"
#include "..\Iterators\ConstIterator.hpp"

namespace Solaire{ namespace Core{

    template<class TYPE, class CONST_TYPE = const TYPE, class INDEX = uint16_t>
	class DynamicArray{
	public:
		typedef TYPE Type;
		typedef CONST_TYPE ConstType;
		typedef Type& Reference;
		typedef ConstType& ConstReference;
		typedef Type&& Move;
		typedef Type* Pointer;
		typedef ConstType* ConstPointer;
		typedef Pointer Iterator;
		typedef ConstPointer ConstIterator;
		typedef Core::ReverseIterator<Type, Iterator> ReverseIterator;
		typedef Core::ConstIterator<Type, ReverseIterator> ConstReverseIterator;
		typedef INDEX Index;
		typedef DynamicArray<TYPE, CONST_TYPE, INDEX> Self;
	private:
        Allocator* mAllocator;
		Index mHead;
		Index mSize;
		Type* mData;
    private:
        void IncreaseSize(){
            const Index newSize = mSize * 2;
            Type* newData = mAllocator->Allocate<Type>(newSize);

            for(Index i = 0; i < mHead; ++i){
                Type* const address = mData + i;
                new(newData + i) Type(std::move(*address));
                address->~Type();
		    }

            mAllocator->Deallocate<Type>(mData, mSize);
            mSize = newSize;
            mData = newData;
        }

        void ShiftDown(const ConstPointer aPosition){
            if(aPosition < mData || aPosition >= mData + mHead) throw std::runtime_error("Core::DynamicArray : Position is out of bounds");
            --mHead;
            for(Index i = aPosition - mData; i < mHead; ++i){
                mData[i] = std::move(mData[i + 1]);
            }
            (mData + mHead)->~Type();
        }

        void ShiftUp(const ConstPointer aPosition){
            if(aPosition < mData || aPosition >= mData + mHead) throw std::runtime_error("Core::DynamicArray : Position is out of bounds");
            if(mHead >= mSize) IncreaseSize();
            ++mHead;

            const Index end = aPosition - mData;

            new(mData + mHead - 1) Type(std::move(mData[mHead - 2]));
            for(Index i = mHead - 2; i != end; --i){
                mData[i] = std::move(mData[i - 1]);
            }
        }
	public:
		static Index MaxCapacity(){
		    return std::numeric_limits<Index>::max();
		}

	    DynamicArray(const Self& aOther) :
		    mAllocator(aOther.mAllocator),
			mHead(0),
			mSize(aOther.mSize),
			mData(mAllocator->Allocate<Type>(mSize))
        {
            for(ConstReference i : aOther) PushBack(i);
        }

	    DynamicArray(Self&& aOther) :
		    mAllocator(aOther.mAllocator),
			mHead(aOther.mHead),
			mSize(aOther.mSize),
			mData(aOther.mData)
        {
            aOther.mHead = 0;
            aOther.mSize = 0;
            aOther.mData = nullptr;
        }

		DynamicArray(const Index aCount = 32, Allocator& aAllocator = GetDefaultAllocator()) :
		    mAllocator(&aAllocator),
			mHead(0),
			mSize(aCount),
			mData(mAllocator->Allocate<Type>(mSize))
		{}

		DynamicArray(ConstReference aValue, const Index aCount, Allocator& aAllocator = GetDefaultAllocator()) :
		    mAllocator(&aAllocator),
			mHead(0),
			mSize(aCount),
			mData(mAllocator->Allocate<Type>(mSize))
		{
			for(Index i = 0; i < aCount; ++i){
				PushBack(aValue);
			}
		}

		DynamicArray(const std::initializer_list<Type> aList, Allocator& aAllocator = GetDefaultAllocator()) :
		    mAllocator(&aAllocator),
			mHead(0),
			mSize(aList.size()),
			mData(mAllocator->Allocate<Type>(mSize))
		{
			for(ConstReference i : aList) PushBack(i);
		}

        template<class ExternalIterator>
		DynamicArray(ExternalIterator aBegin, const ExternalIterator aEnd, Allocator& aAllocator = GetDefaultAllocator()) :
		    mAllocator(&aAllocator),
			mHead(0),
			mSize(aEnd - aBegin),
			mData(mAllocator->Allocate<Type>(mSize))
		{
			while(aBegin != aEnd){
                PushBack(*aBegin);
                ++aBegin;
			}
		}

		~DynamicArray(){
		    if(mData != nullptr){
                Clear();
                mAllocator->Deallocate<Type>(mData, mSize);
		    }
		}

	    Self& operator=(const Self& aOther){
            Clear();

            if(mSize < aOther.mSize){
                mAllocator->Deallocate<Type>(mData, mSize);
                mSize= aOther.mSize;
                mData = mAllocator->Allocate<Type>(mSize);
            }

            for(ConstReference i : aOther) PushBack(i);
            return *this;
	    }

	    Self& operator=(Self&& aOther){
	        std::swap(mAllocator, aOther.mAllocator);
	        std::swap(mHead, aOther.mHead);
	        std::swap(mSize, aOther.mSize);
	        std::swap(mData, aOther.mData);
	        return *this;
	    }

        Allocator& GetAllocator() const{
            return *mAllocator;
        }

		void Clear(){
		    for(Index i = 0; i < mHead; ++i){
                (mData + i)->~Type();
		    }
		    mHead = 0;
		}

		Reference PushBack(Move aValue){
		    if(mHead == mSize) IncreaseSize();
		    return *new(mData + mHead++) Type(std::move(aValue));
		}

		Reference PushBack(ConstReference aValue){
		    if(mHead == mSize) IncreaseSize();
		    return *new(mData + mHead++) Type(aValue);
		}

		Reference PushFront(Move aValue){
		    return InsertBefore(begin(), std::move(aValue));
		}

		Reference PushFront(ConstReference aValue){
		    return InsertBefore(begin(), aValue);
		}

		Type PopBack(){
		    const Iterator pos = end() - 1;
		    const Type value = *pos;
		    Erase(pos);
		    return value;
		}

		Type PopFront(){
		    const Iterator pos = begin();
		    const Type value = *pos;
		    Erase(pos);
		    return value;
		}

		void Erase(const ConstIterator aPosition){
		    ShiftDown(aPosition);
		}

		Reference InsertBefore(const ConstIterator aPosition, Move aValue){
		    ShiftUp(aPosition);
		    const Iterator& pos = const_cast<Iterator&>(aPosition);
		    *pos = std::move(aValue);
		    return *pos;
		}

		Reference InsertBefore(const ConstIterator aPosition, ConstReference aValue){
		    ShiftUp(aPosition);
		    const Iterator& pos = const_cast<Iterator&>(aPosition);
		    *pos = aValue;
		    return *pos;
		}

		Reference InsertAfter(const ConstIterator aPosition, Move aValue){
		    return aPosition == end() - 1 ? PushBack(std::move(aValue)) : InsertBefore(aPosition + 1, std::move(aValue));
		}

		Reference InsertAfter(const ConstIterator aPosition, ConstReference aValue){
		   return aPosition == end() - 1 ? PushBack(std::move(aValue)) : InsertBefore(aPosition + 1, aValue);
		}

		bool IsEmpty() const{
		    return mHead == 0;
		}

		Index Size() const{
		    return mHead;
		}

		Index Capacity() const{
		    return mSize;
		}

		Reference operator[](const Index aIndex){
		    if(aIndex < 0 || aIndex >= mHead) throw std::runtime_error("Core::DynamicArray : Index is out of bounds");
		    return mData[aIndex];
		}

		ConstReference operator[](const Index aIndex) const{
		    if(aIndex < 0 || aIndex >= mHead) throw std::runtime_error("Core::DynamicArray : Index is out of bounds");
		    return mData[aIndex];
		}

		ConstIterator FindFirst(ConstReference aValue) const{
            return FindNext(begin(), aValue);
		}

		ConstIterator FindNext(ConstIterator aPos, ConstReference aValue) const{
            const ConstIterator end = this->end();
            while(aPos != end){
                if(*aPos == aValue) return aPos;
                ++aPos;
            }
            return end;
		}

		ConstIterator FindLast(ConstReference aValue) const{
            const ConstIterator end = this->end();
            ConstIterator it = FindFirst(aValue);
            ConstIterator prev = end;

            while(it != end){
                prev = it;
                it = FindNext(it + 1, aValue);
            }

            return prev;
		}

		template<class F>
		static constexpr bool CheckIfCondition(){
		    return (
                std::is_same<Type, F>::value ||
                std::is_same<ConstType, F>::value ||
                std::is_same<Reference, F>::value ||
                std::is_same<ConstReference, F>::value
            );
		}

		template<class F>
		typename std::enable_if<CheckIfCondition<F>(), ConstIterator>::type
        FindFirst(const F aCondition) const{
            return FindNext<F>(begin(), aCondition);
		}

		template<class F>
		typename std::enable_if<CheckIfCondition<F>(), ConstIterator>::type
		FindNext(ConstIterator aPos, const F aCondition) const{
            const ConstIterator end = this->end();
            while(aPos != end){
                if(aCondition(*aPos)) return aPos;
                ++aPos;
            }
            return end;
		}

		template<class F>
		typename std::enable_if<CheckIfCondition<F>(), ConstIterator>::type
		FindLast(const F aCondition) const{
            const ConstIterator end = this->end();
            ConstIterator it = FindFirst<F>(aCondition);
            ConstIterator prev = end;

            while(it != end){
                prev = it;
                it = FindNext<F>(it + 1, aCondition);
            }

            return prev;
		}

		Iterator FindFirst(ConstReference aValue){
            return const_cast<Iterator>(const_cast<const Self*>(this)->FindFirst(aValue));
		}

		Iterator FindNext(ConstIterator aPos, ConstReference aValue){
            return const_cast<Iterator>(const_cast<const Self*>(this)->FindNext(aPos, aValue));
		}

		Iterator FindLast(ConstReference aValue){
            return const_cast<Iterator>(const_cast<const Self*>(this)->FindLast(aValue));
		}

		template<class F>
		typename std::enable_if<CheckIfCondition<F>(), Iterator>::type
		FindFirst(const F aCondition){
            return const_cast<Iterator>(const_cast<const Self*>(this)->FindFirst(aCondition));
		}

		template<class F>
		typename std::enable_if<CheckIfCondition<F>(), Iterator>::type
		FindNext(ConstIterator aPos, const F aCondition){
            return const_cast<Iterator>(const_cast<const Self*>(this)->FindNext(aPos, aCondition));
		}

		template<class F>
		typename std::enable_if<CheckIfCondition<F>(), Iterator>::type
		FindLast(const F aCondition){
            return const_cast<Iterator>(const_cast<const Self*>(this)->FindLast(aCondition));
		}

		Reference Back(){
		    return *(end() - 1);
		}

		Reference Front(){
		    return *begin();
		}

		ConstReference Back() const{
		    return *(end() - 1);
		}

		ConstReference Front() const{
		    return *begin();
		}

		Iterator begin(){
		    return mData;
		}

		ConstIterator begin() const{
		    return mData;
		}

		Iterator end(){
		    return mData + mHead;
		}

		ConstIterator end() const{
		    return mData + mHead;
		}

		ReverseIterator rbegin(){
		    return ReverseIterator(end() - 1);
		}

		ConstReverseIterator rbegin() const{
		    return ConstReverseIterator(ReverseIterator(Iterator(end() - 1)));
		}

		ReverseIterator rend(){
		    return ReverseIterator(begin() - 1);
		}

		ConstReverseIterator rend() const{
		    return ConstReverseIterator(ReverseIterator(Iterator(begin() - 1)));
		}
	};
}}

#endif
