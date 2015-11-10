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

#include "Solaire\Core\Memory\MemoryArenaBasic.hpp"
#include "Solaire\Core\Memory\DefaultAllocator.hpp"

namespace Solaire{

	// BasicMemoryArena 
	
	BasicMemoryArena::BasicMemoryArena(const uint32_t aBytes):
		mAllocator(DEFAULT_ALLOCATOR),
		mBlock(mAllocator.Allocate(aBytes)),
		mSize(aBytes),
		mAllocated(0)
	{}

	BasicMemoryArena::BasicMemoryArena(Allocator& aAllocator, const uint32_t aBytes) :
		mAllocator(aAllocator),
		mBlock(mAllocator.Allocate(aBytes)),
		mSize(aBytes),
		mAllocated(0) 
	{}

	BasicMemoryArena::~BasicMemoryArena(){
		mAllocator.Deallocate(mBlock, mSize);
	}

	void BasicMemoryArena::Clear() {
		mAllocated = 0;
	}

	uint32_t BasicMemoryArena::GetAllocatedBytes() const {
		return mAllocated;
	}

	uint32_t BasicMemoryArena::GetFreeBytes() const {
		return mSize - mAllocated;
	}

	void* BasicMemoryArena::Allocate(const size_t aBytes) {
		if(mAllocated + aBytes > mSize) return nullptr;
		void* const tmp = static_cast<uint8_t*>(mBlock) + mAllocated;
		mAllocated += aBytes;
		return tmp;
	}

	void BasicMemoryArena::Deallocate(void* const aObject, const size_t aBytes){
		if(static_cast<uint8_t*>(mBlock) + (mAllocated - aBytes) == aObject){
			mAllocated -= aBytes;
		}
	}

}
