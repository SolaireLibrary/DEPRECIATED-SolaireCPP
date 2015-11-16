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
#include "Solaire\Memory\MemoryArenaAdvanced.hpp"
#include "Solaire\Memory\DefaultAllocator.hpp"

namespace Solaire{

	typedef std::pair<void*, uint32_t> Block;

	static void SortBlocks(std::vector<Block>& aBlocks) throw() {
		std::sort(aBlocks.begin(), aBlocks.end(), [](const Block aFirst, const Block aSecond)->bool{
			return aFirst.second < aSecond.second;
		});
	}

	static void MergeBlocks(std::vector<Block>& aBlocks) throw() {
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

	static void* AllocateFromBlocks(std::vector<Block>& aBlocks, const uint32_t aSize) throw() {
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

	AdvancedMemoryArena::AdvancedMemoryArena(const uint32_t aInitialSize) throw() :
		mAllocator(DEFAULT_ALLOCATOR),
		mMainBlocks(),
		mBlocks()
	{
		const Block block(mAllocator.Allocate(aInitialSize), aInitialSize);
		mMainBlocks.push_back(block);
		mBlocks.push_back(block);
	}

	AdvancedMemoryArena::AdvancedMemoryArena(Allocator& aAllocator, const uint32_t aInitialSize) throw() :
		mAllocator(aAllocator),
		mMainBlocks(),
		mBlocks()
	{
		const Block block(mAllocator.Allocate(aInitialSize), aInitialSize);
		mMainBlocks.push_back(block);
		mBlocks.push_back(block);
	}

	AdvancedMemoryArena::~AdvancedMemoryArena() throw() {
		Clear();
		for(Block i : mBlocks){
			mAllocator.Deallocate(i.first);
		}
	}

	bool AdvancedMemoryArena::Clear() throw() {
		mBlocks.clear();
		MergeBlocks(mMainBlocks);
		SortBlocks(mMainBlocks);
		for(Block i : mMainBlocks){
			mBlocks.push_back(i);
		}
		mAllocations.DeallocateAll();
		return true;
	}

	uint32_t AdvancedMemoryArena::GetAllocatedBytes() const throw() {
		return mAllocations.GetAllocatedBytes();
	}

	uint32_t AdvancedMemoryArena::GetFreeBytes() const throw() {
		uint32_t count = 0;
		for (const Block i : mBlocks) count += i.second;

		return count - GetAllocatedBytes();
	}

	void* AdvancedMemoryArena::Allocate(const size_t aBytes) throw() {
		void* address = AllocateFromBlocks(mBlocks, aBytes);

		if(! address){
			MergeBlocks(mBlocks);
			SortBlocks(mBlocks);
			address = AllocateFromBlocks(mBlocks, aBytes);
		}

		if(! address){
			const Block block(mAllocator.Allocate(aBytes), aBytes);
			if(! block.first) return nullptr;
			mMainBlocks.push_back(block);
			address = block.first;
		}

		mAllocations.Allocate(address, aBytes);

		return address;
	}

	bool AdvancedMemoryArena::Deallocate(void* const aObject) throw() {
		mBlocks.push_back(Block(aObject, mAllocations.GetAllocationSize(aObject)));
		SortBlocks(mBlocks);
		return mAllocations.Deallocate(aObject);
	}

}
