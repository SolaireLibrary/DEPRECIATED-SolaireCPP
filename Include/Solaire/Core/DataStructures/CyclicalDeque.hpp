#ifndef SOLAIRE_CORE_CYCLICAL_DEQUE_HPP
#define SOLAIRE_CORE_CYCLICAL_DEQUE_HPP

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
	\file CyclicalDeque.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 27th September 2015
	Last Modified	: 27th September 2015
*/

#include "DynamicArray.hpp"

namespace Solaire{

    template<class TYPE, class CONST_TYPE = const TYPE, class INDEX = uint16_t>
	class CyclicalDeque{
	private:
		typedef DynamicArray<TYPE, CONST_TYPE, INDEX> Array;
	public:
		typedef typename Array::Type Type;
		typedef typename Array::ConstType ConstType;
		typedef typename Array::Reference Reference;
		typedef typename Array::ConstReference ConstReference;
		typedef typename Array::Move Move;
		typedef typename Array::Pointer Pointer;
		typedef typename Array::ConstPointer ConstPointer;
		typedef typename Array::Iterator Iterator;
		typedef typename Array::ConstIterator ConstIterator;
		typedef typename Array::ReverseIterator ReverseIterator;
		typedef typename Array::ConstReverseIterator ConstReverseIterator;
		typedef typename Array::Index Index;

		typedef CyclicalDeque<TYPE, CONST_TYPE, INDEX> Self;
    private:
        Array mArray;
	public:
		static Index MaxCapacity(){
		    return Array::MaxCapacity();
		}

	    CyclicalDeque(const Self& aOther) :
	        mArray(aOther.mArray)
        {}

	    CyclicalDeque(Self&& aOther) :
	        mArray(std::move(aOther.mArray))
        {}

		CyclicalDeque(const Index aCount, Allocator& aAllocator = GetDefaultAllocator()) :
		    mArray(aCount, aAllocator)
		{}

		CyclicalDeque(ConstReference aValue, const Index aCount, Allocator& aAllocator = GetDefaultAllocator()) :
		    mArray(aValue, aCount, aAllocator)
		{}

		CyclicalDeque(const std::initializer_list<Type> aList, Allocator& aAllocator = GetDefaultAllocator()) :
		    mArray(aList.begin(), aList.end(), aAllocator)
		{}

        template<class ExternalIterator>
		CyclicalDeque(ExternalIterator aBegin, const ExternalIterator aEnd, Allocator& aAllocator = GetDefaultAllocator()) :
		    mArray(aBegin, aEnd, aAllocator)
		{}

	    Self& operator=(const Self& aOther){
            mArray = aOther.mArray;
            return *this;
	    }

	    Self& operator=(Self&& aOther){
	        mArray = std::move(aOther.mArray);
	        return *this;
	    }

        Allocator& GetAllocator() const{
            return mArray.GetAllocator();
        }

		void Clear(){
		    mArray.Clear();
		}

		Reference PushBack(Move aValue){
		    if(mArray.Size() == mArray.Capacity()) mArray.PopFront();
		    return mArray.PushBack(std::move(aValue));
		}

		Reference PushBack(ConstReference aValue){
		    if(mArray.Size() == mArray.Capacity()) mArray.PopFront();
		    return mArray.PushBack(aValue);
		}

		Reference PushFront(Move aValue){
		    if(mArray.Size() == mArray.Capacity()) mArray.PopBack();
		    return mArray.PushFront(std::move(aValue));
		}

		Reference PushFront(ConstReference aValue){
		    if(mArray.Size() == mArray.Capacity()) mArray.PopBack();
		    return mArray.PushFront(aValue);
		}

		Type PopBack(){
		    return mArray.PopBack();
		}

		Type PopFront(){
		    return mArray.PopFront();
		}

		void Erase(const ConstIterator aPosition){
		    mArray.Erase(aPosition);
		}

		bool IsEmpty() const{
		    return mArray.IsEmpty();
		}

		Index Size() const{
		    return mArray.Size();
		}

		Index Capacity() const{
		    return mArray.Capacity();
		}

		Reference operator[](const Index aIndex){
		    return mArray[aIndex];
		}

		ConstReference operator[](const Index aIndex) const{
		    return mArray[aIndex];
		}

		Iterator Find(ConstReference aValue){
		     return mArray.Find(aValue);
		}

		ConstIterator Find(ConstReference aValue) const{
		    return mArray.Find(aValue);
		}

		Iterator FindIf(const std::function<bool(ConstReference)>& aCondition){
		    return mArray.FindIf(aCondition);
		}

		ConstIterator FindIf(const std::function<bool(ConstReference)>& aCondition) const{
		    return mArray.FindIf(aCondition);
		}

		Reference Back(){
		    return mArray.Back();
		}

		Reference Front(){
		    return mArray.Front();
		}

		ConstReference Back() const{
		    return mArray.Back();
		}

		ConstReference Front() const{
		    return mArray.Front();
		}

		Iterator begin(){
		    return mArray.begin();
		}

		ConstIterator begin() const{
		    return mArray.begin();
		}

		Iterator end(){
		    return mArray.end();
		}

		ConstIterator end() const{
		    return mArray.end();
		}

		ReverseIterator rbegin(){
		    return mArray.rbegin();
		}

		ConstReverseIterator rbegin() const{
		    return mArray.rbegin();
		}

		ReverseIterator rend(){
		    return mArray.rend();
		}

		ConstReverseIterator rend() const{
		    return mArray.rend();
		}
	};
}

#endif
