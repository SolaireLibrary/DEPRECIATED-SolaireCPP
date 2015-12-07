#ifndef SOLAIRE_MEMORY_ALLOCATOR_STACK_HPP
#define SOLAIRE_MEMORY_ALLOCATOR_STACK_HPP

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
\file AllocatorStack.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.1
\date
Created			: 21st November 2015
Last Modified	: 21st November 2015
*/

#include "Allocator.hpp"

namespace Solaire{

    template<const uint32_t BYTES>
    class AllocatorStack : public Allocator {
    private:
		uint8_t mBlock[BYTES];
		bool mAllocated;
	private:
		AllocatorStack(const AllocatorStack&) = delete;
		AllocatorStack(AllocatorStack&&) = delete;
		AllocatorStack& operator=(const AllocatorStack&) = delete;
		AllocatorStack& operator=(AllocatorStack&&) = delete;
    public:
		AllocatorStack() throw() :
			mAllocated(false)
		{}

		SOLAIRE_EXPORT_CALL ~AllocatorStack() throw() {
			DeallocateAll();
		}

        // Inherited from Allocator

		uint32_t SOLAIRE_EXPORT_CALL GetAllocatedBytes() const throw() override {
			return mAllocated ? BYTES : 0;
		}

		uint32_t SOLAIRE_EXPORT_CALL GetFreeBytes() const throw() override {
			return mAllocated ? 0 : BYTES;
		}
		
		uint32_t SOLAIRE_EXPORT_CALL SizeOf(const void* const aObject) throw() {
			return mBlock == aObject && mAllocated ? BYTES : 0;
		}

		void* SOLAIRE_EXPORT_CALL Allocate(const size_t aBytes) throw() override {
			if(mAllocated || aBytes > BYTES) {
				return nullptr;
			}else {
				mAllocated = true;
				return mBlock;
			}
		}

		bool SOLAIRE_EXPORT_CALL Deallocate(const void* const aObject) throw() override {
			if(mAllocated && aObject == mBlock) {
				mAllocated = false;
				return true;
			}else {
				return false;
			}
		}

		bool SOLAIRE_EXPORT_CALL DeallocateAll() throw() override {
			return Deallocate(mBlock);
		}
    };

	template<const uint32_t BYTES, const bool RECYCLE>
    class MemoryArenaStack : public Allocator {
    private:
		AllocatorStack<BYTES> mStack;
		Allocator& mArena;
	private:
		MemoryArenaStack(const MemoryArenaStack&) = delete;
		MemoryArenaStack(MemoryArenaStack&&) = delete;
		MemoryArenaStack& operator=(const MemoryArenaStack&) = delete;
		MemoryArenaStack& operator=(MemoryArenaStack&&) = delete;
    public:
		MemoryArenaStack() throw() :
			mStack(),
			//mArena(*CreateMemoryArena(GetDefaultAllocator(), mArena, BYTES, RECYCLE))
			mArena(*CreateMemoryArena(GetDefaultAllocator(), GetDefaultAllocator(), BYTES, RECYCLE))
		{}

		SOLAIRE_EXPORT_CALL ~MemoryArenaStack() throw() {
			mArena.~Allocator();
			GetDefaultAllocator().Deallocate(&mArena);
		}

        // Inherited from Allocator

		uint32_t SOLAIRE_EXPORT_CALL GetAllocatedBytes() const throw() override {
			return mArena.GetAllocatedBytes();
		}

		uint32_t SOLAIRE_EXPORT_CALL GetFreeBytes() const throw() override {
			return mArena.GetFreeBytes();
		}
		
		uint32_t SOLAIRE_EXPORT_CALL SizeOf(const void* const aObject) throw() {
			return mArena.SizeOf(aObject);
		}

		void* SOLAIRE_EXPORT_CALL Allocate(const size_t aBytes) throw() override {
			return mArena.Allocate(aBytes);
		}

		bool SOLAIRE_EXPORT_CALL Deallocate(const void* const aObject) throw() override {
			return mArena.Deallocate(aObject);
		}

		bool SOLAIRE_EXPORT_CALL DeallocateAll() throw() override {
			return mArena.DeallocateAll();
		}
    };

}

#endif
