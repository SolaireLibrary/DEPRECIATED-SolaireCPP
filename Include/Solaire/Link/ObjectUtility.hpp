#ifndef SOLAIRE_LINK_OBJECT_UTILITY_HPP
#define SOLAIRE_LINK_OBJECT_UTILITY_HPP

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
\file ObjectUtility.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 9th November 2015
Last Modified	: 15th November 2015
*/

#include <memory>
#include "Object.hpp"
#include "Allocator.hpp"

namespace Solaire{ namespace Link{

	template<class T, typename ENABLE = std::enable_if_t<std::is_base_of<Object, T>::value>>
	using SharedPtr = std::shared_ptr<T>;

	template<class T, typename ENABLE = std::enable_if_t<std::is_base_of<Object, T>::value>>
	using UniquePtr = std::unique_ptr<T, void(*)(Object*)>;

	static void CallDestructorAndDeallocate(Object* const aObject, const uint32_t aSize){
		if(aObject) {
			const uint32_t size = aObject->GetObjectSize();
			LinkAllocator& allocator = aObject->GetAllocator();
			aObject->Destructor();
			allocator.Deallocate(aObject, size);
		}
	}

	template<class T, std::enable_if_t<std::is_base_of<Object, T>::value>>
	SharedPtr<T> MakeShared(T* const aObject) {
		return SharedPtr<T>(
			aObject,
			&CallDestructorAndDeallocate
		);
	}

	template<class T, std::enable_if_t<std::is_base_of<Object, T>::value>>
	UniquePtr<T> MakeUnique(T* const aObject) {
		return UniquePtr<T>(
			aObject,
			&CallDestructorAndDeallocate
		);
	}
}}

#endif
