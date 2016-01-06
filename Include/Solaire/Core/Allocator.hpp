#ifndef SOLAIRE_ALLOCATOR_HPP
#define SOLAIRE_ALLOCATOR_HPP

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
	Last Modified	: 4th January 2015
*/

#include "AllocatorI.hpp"
#include "UniqueAllocation.hpp"
#include "SharedAllocation.hpp"

namespace Solaire {

	/*!
		\class Allocator
		\brief An interface that abstracts memory allocation behavior.
		\detail Memory allocated from an Allocator can only be deallocated from the same Allocator object.
		\author Adam Smith
		\date Created : 25th September 2015
		\date Modified : 3rd December 2015
		\version 1.0
	*/
	SOLAIRE_EXPORT_INTERFACE Allocator : public AllocatorI {
    public:
		/*!
			\brief Destroy the Allocator.
			\detail DeallocateAll will be called before the Allocator is destroyed.
			\see DeallocateAll
		*/
		virtual SOLAIRE_EXPORT_CALL ~Allocator(){}

		/*!
			\brief Allocated a block of memory that will fit type \a T
			\detail Allocation size is determined uisng sizeof
			\tparam T The type to allocate.
			\tparam PARAMS The parameter types to pass to the object's constructor.
			\param aParams The parameters to pass to the object's constructor.
			\return The address of the object, or nullptr if the allocation failed.
			\see Allocate
		*/
		template<class T, typename ...PARAMS>
		SOLAIRE_FORCE_INLINE T* SOLAIRE_DEFAULT_CALL RawAllocate(PARAMS&&... aParams) {
			return new(Allocate(sizeof(T))) T(aParams...);
		}

		/*!
			\brief Allocated a block of memory that will fit type \a T
			\detail Allocation size is determined uisng sizeof
			\tparam T The type to allocate.
			\tparam PARAMS The parameter types to pass to the object's constructor.
			\param aParams The parameters to pass to the object's constructor.
			\return The address of the object, or nullptr if the allocation failed.
			\see Allocate
		*/
		template<class T, typename ...PARAMS>
		SOLAIRE_FORCE_INLINE UniqueAllocation<T> SOLAIRE_DEFAULT_CALL UniqueAllocate(PARAMS&&... aParams) {
			return UniqueAllocation<T>(
				*this,
				new(Allocate(sizeof(T))) T(aParams...)
			);
		}

		/*!
			\brief Allocated a block of memory that will fit type \a T
			\detail Allocation size is determined uisng sizeof
			\tparam T The type to allocate.
			\tparam PARAMS The parameter types to pass to the object's constructor.
			\param aParams The parameters to pass to the object's constructor.
			\return The address of the object, or nullptr if the allocation failed.
			\see Allocate
		*/
		template<class T, typename ...PARAMS>
		SOLAIRE_FORCE_INLINE SharedAllocation<T> SOLAIRE_DEFAULT_CALL SharedAllocate(PARAMS&&... aParams) {
			return SharedAllocation<T>(
				*this,
				new(Allocate(sizeof(T))) T(aParams...)
			);
		}
    };

}

#endif
