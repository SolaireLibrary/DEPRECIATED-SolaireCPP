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

    class Allocator{
    public:
        virtual ~Allocator(){}

        virtual void* AllocateSingle() = 0;
        virtual void* AllocateMany(const size_t aCount) = 0;
        virtual void DeallocateSingle(void* const aAddress) = 0;
        virtual void DeallocateMany(void* const aAddress, const size_t aCount) = 0;
        virtual void CallDestructor(void* const aAddress) = 0;
    };

    template<class T>
    class TypedAllocator{
    public:
        virtual ~TypedAllocator(){}

        void CallDestructor(void* const aAddress) override{
            static_cast<T*>(aAddress)->~T();
        };
    };

    template<class T>
    class DefaultAllocator : public TypedAllocator<T>{
    public:
        void* AllocateSingle() override{
            return operator new(sizeof(T));
        }

        void* AllocateMany(const size_t aCount) override{
            return operator new(sizeof(T) * aCount);
        }

        void DeallocateSingle(void* const aAddress) override{
            operator delete(aAddress);
        }

        void DeallocateMany(void* const aAddress, const size_t aCount) override{
            operator delete(aAddress);
        }
    };

    template<class T>
    Allocator& GetDefaultAllocator(){
        static DefaultAllocator<T> ALLOCATOR;
        return ALLOCATOR;
    }
}}

#endif
