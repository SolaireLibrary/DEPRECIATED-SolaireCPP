#ifndef SOLAIRE_CORE_MEMORY_ARENA_HPP
#define SOLAIRE_CORE_MEMORY_ARENA_HPP

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
\file MemoryArena.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.1
\date
Created			: 21st September 2015
Last Modified	: 25th September 2015
*/

#include <type_traits>
#include "Allocator.hpp"
#include "DataStructures/DynamicArray.hpp"

namespace Solaire{ namespace Core {

    /*!
        \class MemoryArena
        \detail
        Use different MemoryArenas for different object lifetimes (eg. objects that are destroyed at the end of a frame)
    */
    class MemoryArena{
    public:
        typedef void(*DestructorFn)(void*);
    private:
        typedef std::pair<void*, DestructorFn> DestructorCall;
        typedef std::pair<void*, size_t> Region;

        Allocator<void>& mAllocator;
        WrapperAllocator<DestructorCall> mDestructorListAllocator;
        WrapperAllocator<Region> mFreeRegionsAllocator;
        DynamicArray<DestructorCall> mDestructorList;
        DynamicArray<Region> mFreeRegions;
        uint8_t* mArenaBegin;
        uint8_t* mArenaEnd;
        uint8_t* mArenaHead;
        size_t mArenaSize;
        MemoryArena* mNext;

        MemoryArena(const MemoryArena& aOther) = delete;
        MemoryArena(MemoryArena&& aOther) = delete;
        MemoryArena& operator=(const MemoryArena& aOther) = delete;
        MemoryArena& operator=(MemoryArena&& aOther) = delete;

        void Deallocate(const Region aRegion){
            if(mFreeRegions.IsEmpty()){
                mFreeRegions.PushBack(aRegion);
            }else{
                // Largest region at back
                const auto end = mFreeRegions.end() - 1;
                for(auto i = mFreeRegions.begin(); i != end; ++i){
                    if(i->second >= aRegion.second){
                        mFreeRegions.InsertBefore(i, aRegion);
                        return;
                    }
                }
                mFreeRegions.PushBack(aRegion);
            }
        }

    public:
        MemoryArena(const size_t aSize, Allocator<void>& aAllocator = GetDefaultAllocator<void>()) :
            mAllocator(aAllocator),
            mDestructorListAllocator(mAllocator),
            mFreeRegionsAllocator(mAllocator),
            mDestructorList(128, mDestructorListAllocator),
            mFreeRegions(128, mFreeRegionsAllocator),
            mArenaBegin(static_cast<uint8_t*>(mAllocator.Allocate(aSize))),
            mArenaEnd(mArenaBegin + aSize),
            mArenaHead(mArenaBegin),
            mArenaSize(aSize),
            mNext(nullptr)
        {}

        ~MemoryArena(){
            Clear();
            mAllocator.Deallocate(mArenaBegin, mArenaSize);
            if(mNext != nullptr){
                delete mNext;
            }
        }

        void Clear(){
            // Call destructors
            for(const DestructorCall& i : mDestructorList){
                i.second(i.first);
            }
            mDestructorList.Clear();

            // Reset allocation
            mArenaHead = mArenaBegin;
            mFreeRegions.Clear();

            // Propagate the call to the next arena if it exists
            if(mNext != nullptr){
                mNext->Clear();
            }
        }

        void* Allocate(const size_t aSize){
            if(mArenaSize < aSize) goto ALLOCATE_FROM_NEXT;
            if(! mFreeRegions.IsEmpty()) goto ALLOCATE_FROM_FREE_REGIONS;
            goto ALLOCATE_FROM_ARENA;

            ALLOCATE_FROM_FREE_REGIONS:
            {
                const Region mainRegion = mFreeRegions.Back();
                if(mainRegion.second >= aSize){
                    mFreeRegions.PopBack();

                    Region subRegion;
                    subRegion.second = mainRegion.second - aSize;
                    subRegion.first = mainRegion.first + subRegion.second;

                    // Mark any additional memory as a new free region
                    if(subRegion.second != 0) Deallocate(subRegion);

                    return mainRegion.first;
                }
            }
            goto ALLOCATE_FROM_ARENA;

            ALLOCATE_FROM_ARENA:
            {
                const size_t freeArena = mArenaEnd - mArenaHead;
                if(freeArena < aSize) goto ALLOCATE_FROM_NEXT;

                void* const ptr = mArenaHead;
                mArenaHead += aSize;
                return ptr;
            }

            ALLOCATE_FROM_NEXT:
            {
                if((mArenaEnd - mArenaHead) < static_cast<ptrdiff_t>(aSize)){
                    if(mNext == nullptr){
                        mNext = new MemoryArena(mArenaSize > aSize ? mArenaSize : aSize, mAllocator);
                    }
                    return mNext->Allocate(aSize);
                }
            }
        }

        void Deallocate(void* const aAddress, const size_t aSize){
            Deallocate(Region(aAddress, aSize));
        }

        void OnDestroyed(void* aObject, DestructorFn aCallback){
             mDestructorList.PushBack(DestructorCall(aObject, aCallback));
        }

        template<class T>
        void* Allocate(const size_t aCount = 1){
            static_assert(! std::is_abstract<T>::value, "Cannot allocate abstract class on MemoryArena");

            void* const ptr = Allocate(sizeof(T) * aCount);

            if(! std::is_trivially_destructible<T>::value){
                const DestructorFn destructor = [](void* aObject){
                    static_cast<T*>(aObject)->~T();
                };

                for(size_t i = 0; i < aCount; ++i){
                    OnDestroyed(static_cast<T*>(ptr) + i, destructor);
                }
            }

            return ptr;
        }

        size_t AllocatedBytes() const{
            return (mArenaHead - mArenaBegin) + mNext == nullptr ? 0 : mNext->AllocatedBytes();
        }

        size_t CurrentCapacity() const{
            return mArenaSize + mNext == nullptr ? 0 : mNext->CurrentCapacity();
        }

        size_t MaxCapacity() const{
            return std::numeric_limits<size_t>::max();
        }

        void Defragment(){
            Clear();
            if(mNext != nullptr){
                const size_t newSize = mArenaSize + mNext->CurrentCapacity();

                mAllocator.Deallocate(mArenaBegin, mArenaSize);
                mArenaBegin = static_cast<uint8_t*>(mAllocator.Allocate(newSize));

                mArenaEnd = mArenaBegin + newSize;
                mArenaHead = mArenaBegin;
                mArenaSize = newSize;

                delete mNext;
            }
        }
    };

    #define SolaireArenaAllocator(aType, aArenaPtr) Core::ArenaAllocator<aType, &aArenaPtr>

    template <class T, MemoryArena** ARENA>
    class STLArenaAllocator {
    public:
        typedef T        value_type;
        typedef T*       pointer;
        typedef const T* const_pointer;
        typedef T&       reference;
        typedef const T& const_reference;
        typedef size_t    size_type;
        typedef ptrdiff_t difference_type;

        template <class U>
        struct rebind {
            typedef STLArenaAllocator<U, ARENA> other;
        };

        pointer address (reference aValue) const {
            return &aValue;
        }

        const_pointer address (const_reference aValue) const {
            return &aValue;
        }

        STLArenaAllocator() throw(){

        }

        STLArenaAllocator(const STLArenaAllocator&) throw(){

        }

        template <class U>
        STLArenaAllocator(const STLArenaAllocator<U, ARENA>&) throw(){

        }

        ~STLArenaAllocator() throw(){

        }

        size_type max_size() const throw(){
            MemoryArena* const arena = *ARENA;
            return arena == nullptr ? 0 : std::numeric_limits<std::size_t>::max() / sizeof(T);;
        }

        pointer allocate(size_type aCount, const void* = 0){
            MemoryArena* const arena = *ARENA;
            return arena == nullptr ? nullptr : static_cast<T*>(arena->Allocate(aCount * sizeof(T)));
        }

        void construct(pointer aAddress, const T& aValue){
            new(aAddress)T(aValue);
        }

        void destroy(pointer aAddress){
            aAddress->~T();
        }

        void deallocate(pointer aAddress, size_type aCount){
            MemoryArena* const arena = *ARENA;
            if(arena != nullptr) arena->Deallocate(aAddress, aCount * sizeof(T));
        }
    };

    template <class T1, class T2, const MemoryArena** ARENA>
    bool operator==(const STLArenaAllocator<T1, ARENA>&, const STLArenaAllocator<T2, ARENA>&) throw(){
        return true;
    }

    template <class T1, class T2, const MemoryArena** ARENA>
    bool operator!=(const STLArenaAllocator<T1, ARENA>&, const STLArenaAllocator<T2, ARENA>&) throw(){
        return false;
    }

    template<class T>
    class ArenaAllocator : public Allocator<T>{
    private:
        MemoryArena& mArena;
    public:
        ArenaAllocator(MemoryArena& aArena) :
            mArena(aArena)
        {}

        T* AllocateSingle() override{
            return static_cast<T*>(mArena.Allocate(sizeof(T)));
        }

        T* AllocateMany(const size_t aCount) override{
            return static_cast<T*>(mArena.Allocate(sizeof(T) * aCount));
        }

        void DeallocateSingle(T* const aAddress) override{
            mArena.Deallocate(aAddress, sizeof(T));
        }

        void DeallocateMany(T* const aAddress, const size_t aCount) override{
            mArena.Deallocate(aAddress, sizeof(T) * aCount);
        }
    };

    template<>
    class ArenaAllocator<void> : public Allocator<void>{
    private:
        MemoryArena& mArena;
    public:
        ArenaAllocator(MemoryArena& aArena) :
            mArena(aArena)
        {}

        void* Allocate(const size_t aBytes) override{
            return mArena.Allocate(aBytes);
        }

        void Deallocate(void* const aAddress, const size_t aBytes) override{
            mArena.Deallocate(aAddress, aBytes);
        }
    };
}}

#endif
