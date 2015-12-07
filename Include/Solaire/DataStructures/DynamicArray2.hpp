#ifndef SOLAIRE_DYNAMIC_ARRAY2_HPP
#define SOLAIRE_DYNAMIC_ARRAY2_HPP

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
	\file DynamicArray2.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 25th September 2015
	Last Modified	: 7th December 2015
*/

#include "..\Core\Container.hpp"
#include "..\Memory\Allocator.hpp"

namespace Solaire {

    template<class T>
	class DynamicArray2 : public DoubleEndedStack<T> {
	private:
		IteratorPtr<T> mEnd;
		IteratorPtr<T> mBegin;
		Allocator* mAllocator;
		Type* mData;
		uint32_t mSize;
		uint32_t mHead;
	public:
		DynamicArray2(Allocator& aAllocator) :
			mAllocator(&aAllocator),
			mData(nullptr),
			mSize(0),
			mHead(0)
		{}

		DynamicArray2(Allocator& aAllocator, const uint32_t aSize) :
			mAllocator(&aAllocator),
			mData(nullptr),
			mSize(0),
			mHead(0)
		{
			Reserve(aSize);
		}

		DynamicArray2(Allocator& aAllocator, const uint32_t aSize, const Type& aValue) :
			mAllocator(&aAllocator),
			mData(nullptr),
			mSize(0),
			mHead(0)
		{
			Reserve(aSize);
			for(uint32_t i = 0; i < aSize; ++i) {
				new(mData + i) Type(aValue);
			}
			mHead = aSize;
		}

		DynamicArray2(DynamicArray2<Type>&& aOther) :
			mAllocator(aOther.mAllocator),
			mData(aOther.mData),
			mSize(aOther.mSize),
			mHead(aOther.mHead)
		{
			aOther.mData = nullptr;
			aOther.mSize = 0;
			aOther.mHead = 0;
		}

		DynamicArray2(const FixedContainer<T>& aOther) :
			mAllocator(& aOther.GetAllocator()),
			mData(nullptr),
			mSize(0),
			mHead(0)
		{
			mHead = aOther.Size();
			Reserve(mHead);
			for(uint32_t i = 0; i < mHead; ++i) {
				new(mData + i) Type(aOther[i]);
			}
		}

		SOLAIRE_EXPORT_CALL ~DynamicArray2() {
			if(mData != nullptr) {
				Clear();
				mAllocator.Deallocate(mData);
			}
		}

		DynamicArray2<Type>& operator=(DynamicArray2<Type>&& aOther) {
			std::swap(mAllocator, aOther.mAllocator);
			std::swap(mData, aOther.mAllocator);
			std::swap(mSize, aOther.mSize);
			std::swap(mHead, aOther.mHead);
			return *this;
		}

		// Inherited from FixedContainer

		uint32_t SOLAIRE_EXPORT_CALL Size() const override {
			return mSize;
		}

		Type& SOLAIRE_EXPORT_CALL operator[](const uint32_t aIndex) override {
			return mData[aIndex];
		}

		bool SOLAIRE_EXPORT_CALL IsContiguous() const override {
			return true;
		}

		Iterator<T>& SOLAIRE_EXPORT_CALL begin() override {
			return mBegin = IteratorPtr<T>(mData);
		}

		Iterator<T>& SOLAIRE_EXPORT_CALL end() override {
			return mEnd = IteratorPtr<T>(mData + 1);
		}

		Allocator& SOLAIRE_EXPORT_CALL GetAllocator() const override {
			return mAllocator;
		}

		bool SOLAIRE_EXPORT_CALL Reserve(const uint32_t aSize) override {
			if(aSize <= mSize) return true;
			Type* const data = mAllocator.Allocate(sizeof(T) * aSize);
			if(data == nullptr) return false;

			if(mData != nullptr) {
				for(uint32_t i = 0; i < mHead; ++i) {
					Type* const _new = data + i;
					Type* const old = mData + i;
					new(_new) Type(std::move(*old));
					old->~Type();
				}

				mAllocator.Deallocate(mData);
			}

			mData = data;
			mSize = aSize;
			return true;
		}
		
		// Inherted from Stack
		
		Stack<T>& SOLAIRE_EXPORT_CALL operator=(const FixedContainer<T>& aOther) override {
			const uint32_t size = aOther.Size();
			Clear();
			Reserve(size);
			for(uint32_t i = 0; i < size; ++i) {
				new(mData + i) Type(aOther[i]);
			}
			mHead = size;
			return *this;
		}

		void SOLAIRE_EXPORT_CALL Clear() override {
			for(uint32_t i = 0; i < mHead; ++i) {
				mData[i].~Type();
			}
			mHead = 0;
		}

		Type& SOLAIRE_EXPORT_CALL PushBack(const Type& aValue) override {
			Reserve(mHead + 1);
			return mData[mHead++] = aValue;
		}

		Type SOLAIRE_EXPORT_CALL PopBack() override {
			Type* const ptr = mData + (--mHead);
			const Type tmp = std::move(*ptr);
			ptr->~Type();
			return tmp;
		}

		// Inherited from DoubleEndedStack

		Type& SOLAIRE_EXPORT_CALL PushFront(const Type& aValue) {
			Reserve(mHead + 1);

			for(int32_t i = mHead; i > 0; --i) {
				new(mData + i) Type(std::move(mData[i - 1]));
				mData[i - 1].~Type();
			}

			++mHead;
			return mData[0] = aValue;
		}

		Type SOLAIRE_EXPORT_CALL PopFront() {
			Type tmp = std::move(mData[0]);
			mData[0].~Type();

			--mHead;
			for(uint32_t i = 0; i < mHead; ++i) {
				new(mData + i) Type(std::move(mData[i + 1]));
				mData[i + 1].~Type();
			}

			return tmp;
		}
	};

}

#endif
