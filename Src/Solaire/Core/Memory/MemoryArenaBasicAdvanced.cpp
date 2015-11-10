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

#include <algorithm>
#include "Solaire\Core\Memory\MemoryArenaAdvanced.hpp"
#include "Solaire\Core\Memory\DefaultAllocator.hpp"

namespace Solaire{

	typedef std::pair<void*, uint32_t> Block;

	static void SortBlocks(std::vector<Block>& aBlocks) {
		std::sort(aBlocks.begin(), aBlocks.end(), [](const Block aFirst, const Block aSecond)->bool{
			return aFirst.second < aSecond.second;
		});
	}

	static void MergeBlocks(std::vector<Block>& aBlocks) {
		auto begin = aBlocks.begin();
		auto end = aBlocks.end();

		for(auto i = begin; i != end; ++i){
			for(auto j = begin; j != end; ++j){
				if(i == j) continue;

				uint8_t* const ptrA = static_cast<uint8_t*>(i->first);
				uint8_t* const ptrB = static_cast<uint8_t*>(j->first);
				const uint32_t sizeA = i->second;
				const uint32_t sizeB = j->second;

				if(ptrA + sizeA == ptrB){
					aBlocks.erase(j);
					i->second += sizeB;
					i = begin;
					j = begin;
				}else if(ptrB + sizeB == ptrA){
					aBlocks.erase(j);
					i->first = ptrB;
					i->second += sizeB;
					i = begin;
					j = begin;
				}
			}
		}
	}

	static void* AllocateFromBlocks(std::vector<Block>& aBlocks, const uint32_t aSize) {
		const auto end = aBlocks.end();
		for(auto i = aBlocks.begin(); i != aBlocks.end(); ++i){
			if(i->second >= aSize){
				const Block block = *i;
				aBlocks.erase(i);

				if(block.second > aSize){
					aBlocks.push_back(Block(
						static_cast<uint8_t*>(block.first) + aSize,
						block.second - aSize
						));
					SortBlocks(aBlocks);
				}

				return block.first;
			}
		}

		return nullptr;
	}

	// AdvancedMemoryArena

	AdvancedMemoryArena::AdvancedMemoryArena(const uint32_t aInitialSize) :
		mAllocator(DEFAULT_ALLOCATOR),
		mMainBlocks(),
		mBlocks(),
		mAllocatedBytes(0)
	{
		const Block block(mAllocator.Allocate(aInitialSize), aInitialSize);
		mMainBlocks.push_back(block);
		mBlocks.push_back(block);
	}

	AdvancedMemoryArena::AdvancedMemoryArena(Allocator& aAllocator, const uint32_t aInitialSize) :
		mAllocator(aAllocator),
		mMainBlocks(),
		mBlocks(),
		mAllocatedBytes(0)
	{
		const Block block(mAllocator.Allocate(aInitialSize), aInitialSize);
		mMainBlocks.push_back(block);
		mBlocks.push_back(block);
	}

	AdvancedMemoryArena::~AdvancedMemoryArena() {
		for(Block i : mBlocks){
			mAllocator.Deallocate(i.first, i.second);
		}
	}

	void AdvancedMemoryArena::Clear() {
		mBlocks.clear();
		MergeBlocks(mMainBlocks);
		SortBlocks(mMainBlocks);
		for(Block i : mMainBlocks){
			mBlocks.push_back(i);
		}
	}

	uint32_t AdvancedMemoryArena::GetAllocatedBytes() const {
		return mAllocatedBytes;
	}

	uint32_t AdvancedMemoryArena::GetFreeBytes() const {
		uint32_t count = 0;
		for (const Block i : mBlocks) count += i.second;

		return count - mAllocatedBytes;
	}

	void* AdvancedMemoryArena::Allocate(const size_t aBytes) {
		void* address = AllocateFromBlocks(mBlocks, aBytes);

		if(! address){
			MergeBlocks(mBlocks);
			SortBlocks(mBlocks);
			address = AllocateFromBlocks(mBlocks, aBytes);
		}

		if(! address){
			const Block block(mAllocator.Allocate(aBytes), aBytes);
			if (!block.first) return nullptr;
			mMainBlocks.push_back(block);
			address = block.first;
		}

		return address;
	}

	void AdvancedMemoryArena::Deallocate(void* const aObject, const size_t aBytes) {
		mBlocks.push_back(Block(aObject, aBytes));
		SortBlocks(mBlocks);
		mAllocatedBytes -= aBytes;
	}

}
