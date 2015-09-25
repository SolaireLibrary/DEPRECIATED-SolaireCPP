#ifndef SOLAIRE_CORE_ALLOCATOR_HPP
#define SOLAIRE_CORE_ALLOCATOR_HPP

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
	\file Allocator.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 25th September 2015
	Last Modified	: 25th September 2015
*/

#include <type_traits>

namespace Solaire{ namespace Core{

    template<class T>
    class Allocator{
    public:
        virtual ~Allocator(){}

        virtual T* AllocateSingle() = 0;
        virtual T* AllocateMany(const size_t aCount) = 0;
        virtual void DeallocateSingle(T* const aAddress) = 0;
        virtual void DeallocateMany(T* const aAddress, const size_t aCount) = 0;

        void CallDestructor(T* const aAddress){
            aAddress->~T();
        };
    };

    template<>
    class Allocator<void>{
    public:
        virtual ~Allocator(){}

        virtual void* Allocate(const size_t aBytes) = 0;
        virtual void Deallocate(void* const aAddress, const size_t aBytes) = 0;
    };

    template<class T>
    class DefaultAllocator : public Allocator<T>{
    public:
        T* AllocateSingle() override{
            return static_cast<T*>(operator new(sizeof(T)));
        }

        T* AllocateMany(const size_t aCount) override{
            return static_cast<T*>(operator new(sizeof(T) * aCount));
        }

        void DeallocateSingle(T* const aAddress) override{
            operator delete(aAddress);
        }

        void DeallocateMany(T* const aAddress, const size_t aCount) override{
            operator delete(aAddress);
        }
    };

    template<>
    class DefaultAllocator<void> : public Allocator<void>{
    public:
        void* Allocate(const size_t aBytes) override{
            return operator new(aBytes);
        }

        void Deallocate(void* const aAddress, const size_t aBytes) override{
            operator delete(aAddress);
        }
    };

    template<class T>
    class WrapperAllocator : public Allocator<T>{
    private:
        Allocator<void>* mAllocator;
    public:
        WrapperAllocator(Allocator<void>& aAllocator) :
            mAllocator(&aAllocator)
        {}

        T* AllocateSingle() override{
            return static_cast<T*>(mAllocator->Allocate(sizeof(T)));
        }

        T* AllocateMany(const size_t aCount) override{
            return static_cast<T*>(mAllocator->Allocate(sizeof(T) * aCount));
        }

        void DeallocateSingle(T* const aAddress) override{
            mAllocator->Deallocate(aAddress, sizeof(T));
        }

        void DeallocateMany(T* const aAddress, const size_t aCount) override{
            mAllocator->Deallocate(aAddress, sizeof(T) * aCount);
        }
    };

    template<class T>
    Allocator<T>& GetDefaultAllocator(){
        static DefaultAllocator<T> ALLOCATOR;
        return ALLOCATOR;
    }
}}

#endif
