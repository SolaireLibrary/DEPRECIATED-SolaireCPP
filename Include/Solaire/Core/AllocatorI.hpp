#ifndef SOLAIRE_ALLOCATORI_HPP
#define SOLAIRE_ALLOCATORI_HPP

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
	\file AllocatorI.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 25th September 2015
	Last Modified	: 4th January 2015
*/

#include <cstdint>
#include "ModuleHeader.hpp"

namespace Solaire {

	/*!
		\class AllocatorI
		\brief An interface that abstracts memory allocation behavior.
		\detail Memory allocated from an Allocator can only be deallocated from the same Allocator object.
		\author Adam Smith
		\date Created : 25th September 2015
		\date Modified : 3rd December 2015
		\version 1.0
	*/
	SOLAIRE_EXPORT_INTERFACE AllocatorI {
    public:
		/*!
			\brief Return the total number of bytes that are currently allocated by this Allocator.
			\return The number of bytes allocated.
		*/
        virtual SOLAIRE_DEFAULT_API uint32_t SOLAIRE_EXPORT_CALL GetAllocatedBytes() const throw() = 0;

		/*!
			\brief Return the total number of bytes that this Allocator has avalible for allocation.
			\detail If the Allocator does not have an allocation limit, the returned value will be UINT32_MAX.
			\return The number of unallocated bytes.
		*/
        virtual SOLAIRE_DEFAULT_API uint32_t SOLAIRE_EXPORT_CALL GetFreeBytes() const throw() = 0;

		/*!
			\brief Return the size of an allocated memory block.
			\detail \a aObject must point to the start of the allocted block, else the function will return 0.
			\param aObject The address of the allocation block to check.
			\return The size of \a aObject 's block in bytes.
		*/
		virtual SOLAIRE_DEFAULT_API uint32_t SOLAIRE_EXPORT_CALL SizeOf(const void* const) throw() = 0;

		/*!
			\brief Allocate a block of memory.
			\param aBytes The number of bytes to allocate.
			\return The starting address of the allocated block, or nullptr if the allocation failed.
			\see Deallocate
		*/
        virtual SOLAIRE_DEFAULT_API void* SOLAIRE_EXPORT_CALL Allocate(const size_t) throw() = 0;

		/*!
			\brief Deallocate a block of memory.
			\param aObject The starting address of the block to deallocate.
			\return True if the block was deallocated successfully.
			\see Allocate
		*/
        virtual SOLAIRE_DEFAULT_API bool SOLAIRE_EXPORT_CALL Deallocate(const void* const) throw() = 0;

		/*!
			\brief Deallocated all blocks currently allocated by this Allocator.
			\return True if all blocks were deallocated.
			\see Deallocate
		*/
		virtual SOLAIRE_DEFAULT_API bool SOLAIRE_EXPORT_CALL DeallocateAll() throw() = 0;

		/*!
			\brief Destroy the Allocator.
			\detail DeallocateAll will be called before the Allocator is destroyed.
			\see DeallocateAll
		*/
		virtual SOLAIRE_EXPORT_CALL ~AllocatorI(){}
    };

}

#endif
