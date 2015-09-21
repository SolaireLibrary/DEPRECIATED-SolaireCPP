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
            if(mArenaEnd - mArenaHead < aSize){
                if(mNext == nullptr){
                    mNext = new MemoryArena(mArenaSize);
                }
                return mNext->Allocate(aSize);
            }
            void* const ptr = mArenaHead;
            mArenaHead += aSize;
            return ptr;
        }

        void OnDestroyed(void* aObject, DestructorFn aCallback){
             mDestructorList.push_back(DestructorCall(aObject, aCallback));
        }

        template<class T>
        void* Allocate(){
            static_assert(! std::is_abstract<T>::value, "Cannot allocate abstract class on MemoryArena");

            void* const ptr = Allocate(sizeof(T));

            if(! std::is_trivially_destructible<T>::value){
                OnDestroyed(ptr, [](void* aObject){
                    static_cast<T*>(aObject)->~T();
                });
            }

            return ptr;
        }
    };

    template<class IMPLEMENTOR>
    class ArenaAllocatable{
    public:
        static void* operator new(size_t aCount){
            return malloc(aCount);
        }

        static void operator delete(void* aPtr){
            free(aPtr);
        }

        static void* operator new(std::size_t aCount, MemoryArena& aArena){
            return aArena.Allocate<IMPLEMENTOR>();
        }

        //static void* operator new[](std::size_t aCount, MemoryArena& aArena) = delete; //!< \TODO Implement

        virtual ~ArenaAllocatable(){

        }
    };

    template<class IMPLEMENTOR>
    class ArenaAllocatableNoDestructor{
    public:
        static void* operator new(size_t aCount){
            return malloc(aCount);
        }

        static void operator delete(void* aPtr){
            free(aPtr);
        }

        static void* operator new(std::size_t aCount, MemoryArena& aArena){
            static_assert(std::is_trivially_destructible<IMPLEMENTOR>::value, "Implementing class does not have a trivial destructor");
            return aArena.Allocate<IMPLEMENTOR>();
        }

        //static void* operator new[](std::size_t aCount, MemoryArena& aArena) = delete; //!< \TODO Implement
    };
}}

void* operator new(size_t aCount, Solaire::Core::MemoryArena& aArena){
    return aArena.Allocate(aCount);
}

#endif
