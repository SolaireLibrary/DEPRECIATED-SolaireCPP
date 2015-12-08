#ifndef SOLAIRE_SMART_ALLOCATOR_HPP
#define SOLAIRE_SMART_ALLOCATOR_HPP

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
	\file SmartAllocator.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 8th December 2015
	Last Modified	: 8th December 2015
*/

#include <memory>
#include <functional>
#include "Allocator.hpp"

namespace Solaire{

	template<class T>
	using SharedAllocation = std::shared_ptr<T>;

	template<class T>
	using UniqueAllocation = std::unique_ptr<T, std::function<void(void*)>>;

	template<class T>
	SOLAIRE_DEFAULT_API SharedAllocation<T> SOLAIRE_DEFAULT_CALL CreateSharedAllocation(Allocator& aAllocator, T* const aObject) {
		return SharedAllocation<T>(
			aObject,
			[&](T* aPtr) {
				aPtr->~T();
				aAllocator.Deallocate(aPtr);
			}
		);
	}

	template<class T>
	SOLAIRE_DEFAULT_API UniqueAllocation<T> SOLAIRE_DEFAULT_CALL CreateUniqueAllocation(Allocator& aAllocator, T* const aObject) {
		return UniqueAllocation<T>(
			aObject,
			[&](void* aPtr) {
				static_cast<T*>(aPtr)->~T();
				aAllocator.Deallocate(aPtr);
			}
		);
	}

	template<class T, typename... PARAMS>
	SOLAIRE_DEFAULT_API SharedAllocation<T> SOLAIRE_DEFAULT_CALL SharedAllocate(Allocator& aAllocator, PARAMS&& ...aParams) {
		return CreateSharedAllocation<T>(aAllocator, aAllocator.AllocateObject<T>(aParams...));
	}

	template<class T, typename... PARAMS>
	SOLAIRE_DEFAULT_API UniqueAllocation<T> SOLAIRE_DEFAULT_CALL UniqueAllocate(Allocator& aAllocator, PARAMS&& ...aParams) {
		return CreateUniqueAllocation<T>(aAllocator, aAllocator.AllocateObject<T>(aParams...));
	}

}

#endif
