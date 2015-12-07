#ifndef SOLAIRE_STL_ALLOCATOR_HPP
#define SOLAIRE_STL_ALLOCATOR_HPP

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
	\file STLAllocator.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 25th September 2015
	Last Modified	: 9th November 2015
*/

#include "Allocator.hpp"

namespace Solaire {

	static Allocator* STL_ALLOCATOR_PTR = nullptr;

    template <class T>
    class STLAllocator {
    public:
        typedef T           value_type;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;

        template <class U>
        struct rebind {
            typedef STLAllocator<U> other;
        };

        pointer address (reference aValue) const {
            return &aValue;
        }

        const_pointer address (const_reference aValue) const {
            return &aValue;
        }

        STLAllocator() throw(){

        }

        STLAllocator(const STLAllocator&) throw(){

        }

        template <class U>
        STLAllocator(const STLAllocator<U>&) throw(){

        }

        ~STLAllocator() throw(){

        }

        size_type max_size() const throw(){
            return STL_ALLOCATOR_PTR->GetFreeBytes() / sizeof(T);
        }

        pointer allocate(size_type aCount, const void* = 0){
            return STL_ALLOCATOR_PTR->Allocate(sizeof(T) * aCount);
        }

        void construct(pointer aAddress, const T& aValue){
            new(aAddress)T(aValue);
        }

        void destroy(pointer aAddress){
            aAddress->~T();
        }

        void deallocate(pointer aAddress, size_type aCount){
			STL_ALLOCATOR_PTR->Deallocate(aAddress);
        }
    };

    template <class T1, class T2>
    bool operator==(const STLAllocator<T1>&, const STLAllocator<T2>&) throw(){
        return true;
    }

    template <class T1, class T2>
    bool operator!=(const STLAllocator<T1>&, const STLAllocator<T2>&) throw(){
        return false;
    }

}

#endif
