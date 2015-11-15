#ifndef SOLAIRE_MEMORY_ARENA_BASIC_HPP
#define SOLAIRE_MEMORY_ARENA_BASIC_HPP

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
\file MemoryArenaBasic.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.1
\date
Created			: 21st September 2015
Last Modified	: 10th November 2015
*/

#include <algorithm>
#include "MemoryArena.hpp"
#include "Allocator.hpp"

namespace Solaire{

    /*!
        \detail
    */
    class BasicMemoryArena : public MemoryArena {
    private:
        typedef std::pair<void*, uint32_t> Block;
    private:
        Allocator& mAllocator;
		void* const mBlock;
		const uint32_t mSize;
		uint32_t mAllocated;
	private:
		BasicMemoryArena(const BasicMemoryArena&) = delete;
		BasicMemoryArena(BasicMemoryArena&&) = delete;
		BasicMemoryArena& operator=(const BasicMemoryArena&) = delete;
		BasicMemoryArena& operator=(BasicMemoryArena&&) = delete;
    public:
		BasicMemoryArena(const uint32_t) throw();
		BasicMemoryArena(Allocator&, const uint32_t) throw();
		~BasicMemoryArena() throw();

		// Inherited from MemoryArena

		bool SOLAIRE_EXPORT_CALL Clear() throw() override;

        // Inherited from Allocator

		uint32_t SOLAIRE_EXPORT_CALL GetAllocatedBytes() const throw() override;
		uint32_t SOLAIRE_EXPORT_CALL GetFreeBytes() const throw() override;

		void* SOLAIRE_EXPORT_CALL Allocate(const size_t aBytes) throw() override;
		bool SOLAIRE_EXPORT_CALL Deallocate(void* const aObject, const size_t aBytes) throw() override;
    };

}

#endif
