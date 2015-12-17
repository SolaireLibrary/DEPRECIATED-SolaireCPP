#ifndef SOLAIRE_CONTAINER_HPP
#define SOLAIRE_CONTAINER_HPP

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
	\file Container.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 3rd December 2015
	Last Modified	: 3rd December 2015
*/

#include "Iterator.hpp"

namespace Solaire {

	class Allocator;

	template<class T>
	class ContainerIterator : public Iterator<T>{
	private:
		Iterator<T>* mIterator;
		uint32_t mOffset;
	protected:
		//Inherited from Iterator
		Offset SOLAIRE_EXPORT_CALL GetOffset() const throw() override{
			return mOffset;
		}
	public:
		ContainerIterator(Iterator<T>& aIterator, uint32_t aOffset = 0) :
			mIterator(&aIterator),
			mOffset(aOffset)
		{}

		SOLAIRE_EXPORT_CALL ~ContainerIterator() {

		}

		// Inherited from Iterator

		Type* SOLAIRE_EXPORT_CALL operator->() throw() override{
			mIterator->operator+=(mOffset);
			Type* const tmp = mIterator->operator->();
			mIterator->operator-=(mOffset);
			return tmp;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator++() throw() override {
			++mOffset;
			return *this;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator--() throw() override {
			--mOffset;
			return *this;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator+=(const Offset aOffset) throw() override {
			mOffset += aOffset;
			return *this;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator-=(const Offset aOffset) throw() override {
			mOffset -= aOffset;
			return *this;
		}
	};

	template<class T>
	class ContainerReverseIterator : public Iterator<T>{
	private:
		Iterator<T>* mIterator;
		uint32_t mOffset;
	protected:
		//Inherited from Iterator
		Offset SOLAIRE_EXPORT_CALL GetOffset() const throw() override{
			return mOffset;
		}
	public:
		ContainerReverseIterator(Iterator<T>& aIterator, uint32_t aOffset = 0) :
			mIterator(&aIterator),
			mOffset(aOffset)
		{}

		SOLAIRE_EXPORT_CALL ~ContainerReverseIterator() {

		}

		// Inherited from Iterator

		Type* SOLAIRE_EXPORT_CALL operator->() throw() override{
			mIterator->operator+=(mOffset);
			Type* const tmp = mIterator->operator->();
			mIterator->operator-=(mOffset);
			return tmp;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator++() throw() override {
			--mOffset;
			return *this;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator--() throw() override {
			++mOffset;
			return *this;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator+=(const Offset aOffset) throw() override {
			mOffset -= aOffset;
			return *this;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator-=(const Offset aOffset) throw() override {
			mOffset += aOffset;
			return *this;
		}
	};

	template<class T, class ITERATOR>
	class ContainerConstIterator : public Iterator<const T>{
	private:
		ITERATOR mIterator;
	protected:
		//Inherited from Iterator
		Offset SOLAIRE_EXPORT_CALL GetOffset() const throw() override{
			return mIterator.GetOffset();
		}
	public:
		ContainerConstIterator(Iterator<T>& aIterator, const Offset aOffset = 0) :
			ITERATOR(aIterator, aOffset)
		{}

		SOLAIRE_EXPORT_CALL ~ContainerConstIterator() {

		}

		// Inherited from Iterator

		Type* SOLAIRE_EXPORT_CALL operator->() throw() override {
			return mIterator.operator->();
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator++() throw() override {
			++mIterator;
			return *this;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator--() throw() override {
			--mIterator;
			return *this;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator+=(const Offset aOffset) throw() override {
			mIterator += aOffset;
			return *this;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator-=(const Offset aOffset) throw() override {
			mIterator -= aOffset;
			return *this;
		}
	};

	template<class T>
	SOLAIRE_EXPORT_INTERFACE FixedContainer {
	public:
		typedef T Type;
		typedef ContainerIterator<Type> Iterator;
		typedef ContainerConstIterator<Type, Iterator> ConstIterator;
		typedef ContainerReverseIterator<Type> ReverseIterator;
		typedef ContainerConstIterator<Type, ReverseIterator> ConstReverseIterator;
	public:
		virtual uint32_t SOLAIRE_EXPORT_CALL Size() const = 0;
		virtual Type& SOLAIRE_EXPORT_CALL operator[](const uint32_t) = 0;
		virtual bool SOLAIRE_EXPORT_CALL IsContiguous() const = 0;
		virtual Allocator& SOLAIRE_EXPORT_CALL GetAllocator() const = 0;
		virtual bool SOLAIRE_EXPORT_CALL Reserve(const uint32_t) = 0;
		virtual Solaire::Iterator<T>& SOLAIRE_EXPORT_CALL GetBeginIterator() = 0;
		virtual SOLAIRE_EXPORT_CALL ~FixedContainer(){}

		SOLAIRE_FORCE_INLINE const Type& operator[](const uint32_t aIndex) const {
			return const_cast<FixedContainer<T>*>(this)->operator[](aIndex);
		}

		SOLAIRE_FORCE_INLINE T* GetPtr() {
			return IsContiguous() ? &operator[](0) : nullptr;
		}

		SOLAIRE_FORCE_INLINE const T* GetPtr() const {
			return IsContiguous() ? &operator[](0) : nullptr;
		}

		SOLAIRE_FORCE_INLINE Iterator begin() {
			return Iterator(GetBeginIterator(), 0);
		}

		SOLAIRE_FORCE_INLINE Iterator end() {
			return Iterator(GetBeginIterator(), Size());
		}

		SOLAIRE_FORCE_INLINE ConstIterator begin() const {
			return ContainerConstIterator<T>(const_cast<FixedContainer<T>*>(this)->GetBeginIterator(), 0);
		}

		SOLAIRE_FORCE_INLINE ConstIterator end() const {
			return ContainerConstIterator<T>(const_cast<FixedContainer<T>*>(this)->GetBeginIterator(), Size());
		}

		SOLAIRE_FORCE_INLINE ReverseIterator rbegin() {
			return ReverseIterator(GetBeginIterator(), Size() - 1);
		}

		SOLAIRE_FORCE_INLINE ReverseIterator rend() {
			return ReverseIterator(GetBeginIterator(), -1);
		}

		SOLAIRE_FORCE_INLINE ConstReverseIterator rbegin() const {
			return ReverseIterator(const_cast<FixedContainer<T>*>(this)->GetBeginIterator(), Size() - 1);
		}

		SOLAIRE_FORCE_INLINE ConstReverseIterator rend() const {
			return ReverseIterator(const_cast<FixedContainer<T>*>(this)->GetBeginIterator(), -1);
		}
	};

	template<class T>
	SOLAIRE_EXPORT_INTERFACE Stack : public FixedContainer<T> {
	public:
		virtual Type& SOLAIRE_EXPORT_CALL PushBack(const Type&) = 0;
		virtual Type SOLAIRE_EXPORT_CALL PopBack() = 0;
		virtual Stack<T>& SOLAIRE_EXPORT_CALL operator=(const FixedContainer<T>&) = 0;
		virtual void SOLAIRE_EXPORT_CALL Clear() = 0;
		virtual  SOLAIRE_EXPORT_CALL ~Stack(){}

		SOLAIRE_FORCE_INLINE T& Back() {
			return operator[](Size() - 1);
		}

		SOLAIRE_FORCE_INLINE const T& Back() const {
			return operator[](Size() - 1);
		}
	};

	template<class T>
	SOLAIRE_EXPORT_INTERFACE DoubleEndedStack : public Stack<T> {
	public:
		virtual Type& SOLAIRE_EXPORT_CALL PushFront(const Type&) = 0;
		virtual Type SOLAIRE_EXPORT_CALL PopFront() = 0;
		virtual SOLAIRE_EXPORT_CALL ~DoubleEndedStack(){}

		SOLAIRE_FORCE_INLINE T& Front() {
			return operator[](0);
		}

		SOLAIRE_FORCE_INLINE const T& Front() const {
			return operator[](0);
		}
	};

	template<class T>
	SOLAIRE_EXPORT_INTERFACE List : public DoubleEndedStack<T> {
	public:
		virtual Type& SOLAIRE_EXPORT_CALL InsertBefore(const ConstIterator, const Type&) = 0;
		virtual Type& SOLAIRE_EXPORT_CALL InsertAfter(const ConstIterator, const Type&) = 0;
		virtual bool SOLAIRE_EXPORT_CALL Erase(const ConstIterator) = 0;
		virtual SOLAIRE_EXPORT_CALL ~List(){}
	};

}


#endif
