#ifndef SOLAIRE_MEMORY_ARENA_ADVANCED_HPP
#define SOLAIRE_MEMORY_ARENA_ADVANCED_HPP

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
\file MemoryArenaAdvanced.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.1
\date
Created			: 21st September 2015
Last Modified	: 9th November 2015
*/

#include <vector>
#include "Allocator.hpp"
#include "MemoryArena.hpp"

namespace Solaire{

    /*!
        \detail
    */
	class AdvancedMemoryArena : public MemoryArena {
	private:
		typedef std::pair<void*, uint32_t> Block;
	private:
		Allocator& mAllocator;
		std::vector<Block> mMainBlocks;
		std::vector<Block> mBlocks;
		uint32_t mAllocatedBytes;
	private:
		AdvancedMemoryArena(const AdvancedMemoryArena&) = delete;
		AdvancedMemoryArena(AdvancedMemoryArena&&) = delete;
		AdvancedMemoryArena& operator=(const AdvancedMemoryArena&) = delete;
		AdvancedMemoryArena& operator=(AdvancedMemoryArena&&) = delete;
	public:
		AdvancedMemoryArena(const uint32_t);
		AdvancedMemoryArena(Allocator&, const uint32_t);
		~AdvancedMemoryArena();

		// Inherited from Allocator

		void Clear() override;

		// Inherited from Allocator

		uint32_t GetAllocatedBytes() const override;
		uint32_t GetFreeBytes() const override;

		void* Allocate(const size_t aBytes) override;
		void Deallocate(void* const aObject, const size_t aBytes) override;
	};

}

#endif
