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

#include <algorithm>
#include <type_traits>
#include "Allocator.hpp"
#include "../DataStructures/DynamicArray.hpp"

namespace Solaire{ namespace Core {

    /*!
        \class MemoryArena
        \detail
        Use different MemoryArenas for different object lifetimes (eg. objects that are destroyed at the end of a frame)
    */
    class MemoryArena : public Allocator{
    public:
        typedef void(*DestructorFn)(void*);
    private:
        //! \bug MemoryArena is not type safe
        typedef std::pair<void*, DestructorFn> DestructorCall;
        typedef std::pair<void*, size_t> Region;

        Allocator& mAllocator;
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

        void DefragmentFreeRegions(){
            const size_t sizeBefore = mFreeRegions.Size();

            auto begin = mFreeRegions.begin();
            auto end = mFreeRegions.end();

            for(auto i = begin; i != end; ++i){
                Region& region = *i;
                void* next = static_cast<uint8_t*>(region.first) + region.second;
                for(auto j = begin; j != end; ++j){
                    if(i == j) continue;
                    if(j->first == next){
                        region.second += j->second;
                        mFreeRegions.Erase(j);
                        begin = mFreeRegions.begin();
                        end = mFreeRegions.end();
                        i = begin;
                        j = begin;
                    }
                }
            }

            for(auto i = begin; i != end; ++i){
                if(static_cast<uint8_t*>(i->first) + i->second == mArenaHead){
                    mArenaHead -= i->second;
                    mFreeRegions.Erase(i);
                    begin = mFreeRegions.begin();
                    end = mFreeRegions.end();
                }
            }

            if(sizeBefore != mFreeRegions.Size()){
                std::sort(mFreeRegions.begin(), mFreeRegions.end(), [](const Region& aFirst, const Region& aSecond)->bool{
                    return aFirst.second < aSecond.second;
                });
                DefragmentFreeRegions();
            }
        }

    public:
        MemoryArena(const size_t aSize, Allocator& aAllocator = GetDefaultAllocator()) :
            mAllocator(aAllocator),
            mDestructorList(128, aAllocator),
            mFreeRegions(128, aAllocator),
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

        size_t AllocatedBytes() const{
            return (mArenaHead - mArenaBegin) + mNext == nullptr ? 0 : mNext->AllocatedBytes();
        }

        size_t CurrentCapacity() const{
            return mArenaSize + (mNext == nullptr ? 0 : mNext->CurrentCapacity());
        }

        size_t MaxCapacity() const{
            return std::numeric_limits<size_t>::max();
        }

        void Defragment(){
            Clear();
            if(mNext != nullptr){
                const size_t newSize = CurrentCapacity();

                mAllocator.Deallocate(mArenaBegin, mArenaSize);
                mArenaBegin = static_cast<uint8_t*>(mAllocator.Allocate(newSize));

                mArenaEnd = mArenaBegin + newSize;
                mArenaHead = mArenaBegin;
                mArenaSize = newSize;

                delete mNext;
                mNext = nullptr;
            }
        }

        // Inherited from Allocator

        void* Allocate(const size_t aBytes) override{
            bool defragmented = false;

            if(mArenaSize < aBytes) goto ALLOCATE_FROM_NEXT;
            goto ALLOCATE_FROM_FREE_REGIONS;

            ALLOCATE_FROM_FREE_REGIONS:
            if(! mFreeRegions.IsEmpty()){
                const Region mainRegion = mFreeRegions.Back();
                if(mainRegion.second >= aBytes){
                    mFreeRegions.PopBack();

                    // Mark any additional memory as a new free region
                    const size_t dif = mainRegion.second - aBytes;
                    if(dif != 0) Deallocate(static_cast<uint8_t*>(mainRegion.first) + dif, dif);

                    return mainRegion.first;
                }
            }
            goto ALLOCATE_FROM_ARENA;

            ALLOCATE_FROM_ARENA:
            {
                const size_t freeArena = mArenaEnd - mArenaHead;
                if(freeArena < aBytes){
                    if(defragmented){
                        goto ALLOCATE_FROM_NEXT;
                    }else{
                        DefragmentFreeRegions();
                        defragmented = true;
                        goto ALLOCATE_FROM_FREE_REGIONS;
                    }
                }

                void* const ptr = mArenaHead;
                mArenaHead += aBytes;
                return ptr;
            }

            ALLOCATE_FROM_NEXT:
            {
                if((mArenaEnd - mArenaHead) < static_cast<ptrdiff_t>(aBytes)){
                    if(mNext == nullptr){
                        mNext = new MemoryArena(mArenaSize > aBytes ? mArenaSize : aBytes, mAllocator);
                    }
                    return mNext->Allocate(aBytes);
                }
            }
            return nullptr;
        }

        void Deallocate(void* const aObject, const size_t aBytes) override{
            const Region region(aObject, aBytes);
            if(mFreeRegions.IsEmpty()){
                mFreeRegions.PushBack(region);
            }else{
                // Largest region at back
                const auto end = mFreeRegions.end() - 1;
                for(auto i = mFreeRegions.begin(); i != end; ++i){
                    if(i->second >= region.second){
                        mFreeRegions.InsertBefore(i, region);
                        return;
                    }
                }
                mFreeRegions.PushBack(region);
            }
        }

        void OnDestroyed(void* const aObject, DestructorFn aCallback) override{
             mDestructorList.PushBack(DestructorCall(aObject, aCallback));
        }
    };


}}

#endif
