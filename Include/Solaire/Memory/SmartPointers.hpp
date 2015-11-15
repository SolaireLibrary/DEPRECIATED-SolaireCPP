#ifndef SOLAIRE_SMART_POINTER_HPP
#define SOLAIRE_SMART_POINTER_HPP

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
	\file SmartPointers.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 25th September 2015
	Last Modified	: 9th November 2015
*/

#include <limits>
#include <type_traits>
#include <memory>
#include "Allocator.hpp"

namespace Solaire{
    template<class T>
    using SharedPointer = std::shared_ptr<T>;

    template<class T>
    using UniquePointer = std::unique_ptr<T, std::function<void(void*)>>;

    template<class T, class ...PARAMS>
    static SharedPointer<T> SharedAllocate(Allocator& aAllocator, PARAMS&&... aParams) throw() {
		try{
			return SharedPointer<T>(
				new(aAllocator.Allocate(sizeof(T))) T(aParams...),
				[&](T* const aObject) {
					aAllocator.Deallocate(aObject, sizeof(T));
					aObject->~T();
				}
			);
		}catch (...) {
			return SharedPointer<T>();
		}
    }

    template<class T, class ...PARAMS>
    static UniquePointer<T> UniqueAllocate(Allocator& aAllocator, PARAMS&&... aParams) throw() {
		try{
			return UniquePointer<T>(
				new(aAllocator.Allocate(sizeof(T))) T(aParams...),
				[&](void* aObject) {
					aAllocator.Deallocate(aObject, sizeof(T));
					static_cast<T*>(aObject)->~T();
				}
			);
		}catch (...) {
			return UniquePointer<T>();
		}
        
    }
}

#endif
