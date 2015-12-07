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
	class FixedContainer {
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
		virtual Solaire::Iterator<T>& SOLAIRE_EXPORT_CALL GetBeginIterator() const = 0;
		virtual SOLAIRE_EXPORT_CALL ~FixedContainer(){}

		inline const Type& operator[](const uint32_t aIndex) const {
			return const_cast<FixedContainer<T>*>(this)->operator[](aIndex);
		}

		inline T* GetPtr() {
			return IsContiguous() ? &operator[](0) : nullptr;
		}

		inline const T* GetPtr() const {
			return IsContiguous() ? &operator[](0) : nullptr;
		}

		inline Iterator begin() {
			return Iterator(GetBeginIterator(), 0);
		}

		inline Iterator end() {
			return Iterator(GetBeginIterator(), Size());
		}

		inline ConstIterator begin() const {
			return ContainerConstIterator<T>(GetBeginIterator(), 0);
		}

		inline ConstIterator end() const {
			return ContainerConstIterator<T>(GetBeginIterator(), Size());
		}

		inline ReverseIterator rbegin() {
			return ReverseIterator(GetBeginIterator(), Size() - 1);
		}

		inline ReverseIterator rend() {
			return ReverseIterator(GetBeginIterator(), -1);
		}

		inline ConstReverseIterator rbegin() const {
			return ReverseIterator(GetBeginIterator(), Size() - 1);
		}

		inline ConstReverseIterator rend() const {
			return ReverseIterator(GetBeginIterator(), -1);
		}
	};

	template<class T>
	class Stack : public FixedContainer<T> {
	public:
		virtual Type& SOLAIRE_EXPORT_CALL PushBack(const Type&) = 0;
		virtual Type SOLAIRE_EXPORT_CALL PopBack() = 0;
		virtual Stack<T>& SOLAIRE_EXPORT_CALL operator=(const FixedContainer<T>&) = 0;
		virtual void SOLAIRE_EXPORT_CALL Clear() = 0;
		virtual  SOLAIRE_EXPORT_CALL ~Stack(){}

		inline T& Back() {
			return operator[](Size() - 1);
		}

		inline const T& Back() const {
			return operator[](Size() - 1);
		}
	};

	template<class T>
	class DoubleEndedStack : public Stack<T> {
	public:
		virtual Type& SOLAIRE_EXPORT_CALL PushFront(const Type&) = 0;
		virtual Type SOLAIRE_EXPORT_CALL PopFront() = 0;
		virtual SOLAIRE_EXPORT_CALL ~DoubleEndedStack(){}

		inline T& Front() {
			return operator[](0);
		}

		inline const T& Front() const {
			return operator[](0);
		}
	};

	template<class T>
	class List : public DoubleEndedStack<T> {
	public:
		virtual Type& SOLAIRE_EXPORT_CALL InsertBefore(const ConstIterator, const Type&) = 0;
		virtual Type& SOLAIRE_EXPORT_CALL InsertAfter(const ConstIterator, const Type&) = 0;
		virtual bool SOLAIRE_EXPORT_CALL Erase(const ConstIterator) = 0;
		virtual SOLAIRE_EXPORT_CALL ~List(){}
	};

}


#endif
