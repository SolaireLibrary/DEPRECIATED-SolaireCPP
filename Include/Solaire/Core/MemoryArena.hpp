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
\version 1.0
\date
Created			: 21st September 2015
Last Modified	: 21st September 2015
*/

#include <vector>
#include <type_traits>

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

        std::vector<DestructorCall> mDestructorList;
        uint8_t* const mArenaBegin;
        uint8_t* const mArenaEnd;
        uint8_t* mArenaHead;
        const size_t mArenaSize;
        MemoryArena* mNext;

        MemoryArena(const MemoryArena& aOther) = delete;
        MemoryArena(MemoryArena&& aOther) = delete;
        MemoryArena& operator=(const MemoryArena& aOther) = delete;
        MemoryArena& operator=(MemoryArena&& aOther) = delete;

    public:
        MemoryArena(const size_t aSize) :
            mArenaBegin(static_cast<uint8_t*>(operator new(aSize))),
            mArenaEnd(mArenaBegin + aSize),
            mArenaHead(mArenaBegin),
            mArenaSize(aSize),
            mNext(nullptr)
        {}

        ~MemoryArena(){
            Clear();
            operator delete(mArenaBegin);
            if(mNext != nullptr){
                delete mNext;
            }
        }

        void Clear(){
            // Call destructors
            for(const DestructorCall& i : mDestructorList){
                i.second(i.first);
            }
            mDestructorList.clear();

            // Reset allocation
            mArenaHead = mArenaBegin;

            // Propagate the call to the next arena if it exists
            if(mNext != nullptr){
                mNext->Clear();
            }
        }

        void* Allocate(const size_t aSize){
            if((mArenaEnd - mArenaHead) < static_cast<ptrdiff_t>(aSize)){
                if(mNext == nullptr){
                    mNext = new MemoryArena(mArenaSize > aSize ? mArenaSize : aSize);
                }
                return mNext->Allocate(aSize);
            }
            void* const ptr = mArenaHead;
            mArenaHead += aSize;
            return ptr;
        }

        bool Deallocate(void* const aAddress, const size_t aSize){
            #ifdef SOLAIRE_DISABLED_MEMORY_ARENA_DEALLOCATION
            if(aAddress >= mArenaBegin && aAddress < mArenaEnd){
                if(mArenaHead - aSize == aAddress){
                    if(! mDestructorList.empty()){
                        // Find destructor callbacks
                        std::vector<DestructorCall> calls;
                        for(const DestructorCall& call : mDestructorList){
                            if(call.first >= aAddress) calls.push_back(call);
                        }

                        // Call destructor callbacks
                        for(const DestructorCall& call : calls){
                            call.second(call.first);
                            mDestructorList.erase(std::find(mDestructorList.begin(), mDestructorList.end(), call));
                        }
                    }

                    // Move the allocator head back
                    mArenaHead -= aSize;
                    return true;
                }
            }else if(mNext != nullptr){
                return mNext->Deallocate(aAddress, aSize);
            }
            #endif
            return false;
        }

        void OnDestroyed(void* aObject, DestructorFn aCallback){
             mDestructorList.push_back(DestructorCall(aObject, aCallback));
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
    };

    #define SolaireArenaAllocator(aType, aArenaPtr) Core::ArenaAllocator<aType, &aArenaPtr>

    template <class T, MemoryArena** ARENA>
    class ArenaAllocator {
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
            typedef ArenaAllocator<U, ARENA> other;
        };

        pointer address (reference aValue) const {
            return &aValue;
        }

        const_pointer address (const_reference aValue) const {
            return &aValue;
        }

        ArenaAllocator() throw(){

        }

        ArenaAllocator(const ArenaAllocator&) throw(){

        }

        template <class U>
        ArenaAllocator(const ArenaAllocator<U, ARENA>&) throw(){

        }

        ~ArenaAllocator() throw(){

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
    bool operator==(const ArenaAllocator<T1, ARENA>&, const ArenaAllocator<T2, ARENA>&) throw(){
        return true;
    }

    template <class T1, class T2, const MemoryArena** ARENA>
    bool operator!=(const ArenaAllocator<T1, ARENA>&, const ArenaAllocator<T2, ARENA>&) throw(){
        return false;
    }
}}

#endif
