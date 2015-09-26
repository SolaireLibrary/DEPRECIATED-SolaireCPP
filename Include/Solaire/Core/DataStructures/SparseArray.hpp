#ifndef SOLAIRE_CORE_SPARSE_ARRAY_HPP
#define SOLAIRE_CORE_SPARSE_ARRAY_HPP

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
	\file SparseArray.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 26th September 2015
	Last Modified	: 26th September 2015
*/

#include <type_traits>
#include <array>
#include "../Maths.hpp"
#include "DynamicArray.hpp"

namespace Solaire{ namespace Core{

    template<class K, class T>
    class SparseArray{
    public:
        static_assert(std::is_integral<K>::value, "Core::SparseArray : Key type must be an integer value");

        typedef K Key;
        typedef T Type;
        typedef Type* Pointer;
        typedef const Type* ConstPointer;
        typedef Type&& Move;
        typedef Type& Reference;
        typedef const Type& ConstReference;
        typedef void* Iterator;
        typedef const void* ConstIterator;
    private:
        enum : Key{
            RANGE_LENGTH = 16
        };

        typedef std::pair<Key, DynamicArray<Type>> RangedArray;

        Allocator<void>& mAllocator;
        WrapperAllocator<Type> mTypeAllocator;
        WrapperAllocator<RangedArray> mRangedArrayAllocator;

        DynamicArray<RangedArray> mRanges;

        RangedArray* GetArrayPtr(const Key aKey){
            for(RangedArray& range : mRanges){
                if(aKey >= range.first && aKey < range.first + RANGE_LENGTH){
                    return &range;
                }
            }

            return nullptr;
        }

        const RangedArray* GetArrayPtr(const Key aKey) const{
            return const_cast<SparseArray<Key, Type>*>(this)->GetArrayPtr();
        }

        RangedArray& GetArrayRef(const Key aKey){
            RangedArray* const ptr = GetArrayPtr(aKey);
            if(ptr != nullptr) return *ptr;

            const Key rangeBase = Maths::FloorToClosestMultiple<Key>(aKey, RANGE_LENGTH);
            return mRanges.PushBack(RangedArray(
                rangeBase,
                DynamicArray<Type>(
                    RANGE_LENGTH,
                    static_cast<Type>(0),
                    mTypeAllocator
                )
            ));
        }
    public:
        SparseArray(Allocator<void>& = GetDefaultAllocator<void>()) :
            mTypeAllocator(mAllocator),
            mRangedArrayAllocator(mAllocator),
            mRanges(32, mRangedArrayAllocator)
        {}

        Iterator Insert(const Key aKey, Move aValue){
            RangedArray& array_ = GetArrayRef(aKey);
            const Key aIndex = array_.first / RANGE_LENGTH;

            ConstReference ref = array_.second[aIndex] = std::move(aValue);

            //! \TODO Implement Iterator
            return nullptr;
        }

        Iterator Insert(const Key aKey, ConstReference aValue){
            RangedArray& array_ = GetArrayRef(aKey);
            const Key aIndex = array_.first / RANGE_LENGTH;

            ConstReference ref = array_.second[aIndex] = aValue;

            //! \TODO Implement Iterator
            return nullptr;
        }

        Iterator Find(const Key aKey){
            RangedArray* const array_ = GetArrayPtr(aKey);
            if(array_ == nullptr) return end();
            const Key aIndex = array_->first / RANGE_LENGTH;

            //! \TODO Implement Iterator
            return &array_->second[aIndex];
        }

        ConstIterator Find(const Key aKey) const{
            return const_cast<SparseArray<Key, Type>*>(this)->Find(aKey);
        }

        Iterator begin(){
            //! \TODO Implement Iterator
            return nullptr;
        }

        ConstIterator begin() const{
            //! \TODO Implement ConstIterator
            return nullptr;
        }

        Iterator end(){
            //! \TODO Implement Iterator
            return nullptr;
        }

        ConstIterator end() const{
            //! \TODO Implement ConstIterator
            return nullptr;
        }
    };

}}


#endif
