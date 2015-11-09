#ifndef SOLAIRE_MEMORY_ARENA_HPP
#define SOLAIRE_MEMORY_ARENA_HPP

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
\file MemoryArena.hpp
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
#include <algorithm>
#include <type_traits>
#include "Allocator.hpp"

namespace Solaire{

    /*!
        \class MemoryArena
        \detail
        Use different MemoryArenas for different object lifetimes (eg. objects that are destroyed at the end of a frame)
    */

	class MemoryArena : public Allocator{
	public:
		virtual ~MemoryArena();
		virtual void Clear() = 0;
	};

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
		BasicMemoryArena(const uint32_t);
		BasicMemoryArena(Allocator&, const uint32_t);
		~BasicMemoryArena();

		// Inherited from Allocator

		void Clear() override;

        // Inherited from Allocator

		uint32_t GetAllocatedBytes() const override;
		uint32_t GetFreeBytes() const override;

		void* Allocate(const size_t aBytes) override;
		void Deallocate(void* const aObject, const size_t aBytes) override;
    };

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
