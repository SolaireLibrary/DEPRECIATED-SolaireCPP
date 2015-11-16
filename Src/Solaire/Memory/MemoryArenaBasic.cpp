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

#include "Solaire\Memory\MemoryArenaBasic.hpp"
#include "Solaire\Memory\DefaultAllocator.hpp"

namespace Solaire{

	// BasicMemoryArena 
	
	BasicMemoryArena::BasicMemoryArena(const uint32_t aBytes) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mBlock(mAllocator.Allocate(aBytes)),
		mSize(aBytes)
	{}

	BasicMemoryArena::BasicMemoryArena(Allocator& aAllocator, const uint32_t aBytes) throw() :
		mAllocator(aAllocator),
		mBlock(mAllocator.Allocate(aBytes)),
		mSize(aBytes)
	{}

	BasicMemoryArena::~BasicMemoryArena() throw() {
		mAllocator.Deallocate(mBlock);
	}

	bool BasicMemoryArena::Clear() throw() {
		mAllocations.DeallocateAll();
		return true;
	}

	uint32_t BasicMemoryArena::GetAllocatedBytes() const throw() {
		return mAllocations.GetAllocatedBytes();
	}

	uint32_t BasicMemoryArena::GetFreeBytes() const throw() {
		return mSize - GetAllocatedBytes();
	}

	void* BasicMemoryArena::Allocate(const size_t aBytes) throw() {
		const uint32_t allocated = GetAllocatedBytes();
		if(allocated + aBytes > mSize) return nullptr;
		void* const tmp = static_cast<uint8_t*>(mBlock) + allocated;
		mAllocations.Allocate(tmp, aBytes);
		return tmp;
	}

	bool BasicMemoryArena::Deallocate(void* const aObject) throw() {
		return mAllocations.Deallocate(aObject);
	}

}
