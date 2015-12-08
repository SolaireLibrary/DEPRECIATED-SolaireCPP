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

#include "Solaire\Memory\Allocator.hpp"
#include <algorithm>
#include <vector>

namespace Solaire{

	class BasicMemoryArena : public Allocator {
	private:
		Allocator& mAllocator;
		void* const mBlock;
		const uint32_t mSize;
		bool mAllocated;
	public:
		BasicMemoryArena(Allocator& aAllocator, const uint32_t aInitialSize) :
			mAllocator(aAllocator),
			mBlock(aAllocator.Allocate(aInitialSize)),
			mSize(aInitialSize),
			mAllocated(false)
		{}

		SOLAIRE_EXPORT_CALL ~BasicMemoryArena() throw() {
			mAllocator.Deallocate(mBlock);
		}

		// Inherited from Allocator

		uint32_t SOLAIRE_EXPORT_CALL GetAllocatedBytes() const throw()  override {
			return mAllocated ? mSize : 0;
		}

		uint32_t SOLAIRE_EXPORT_CALL GetFreeBytes() const throw() override {
			return mAllocated ? 0 : mSize;
		}

		uint32_t SOLAIRE_EXPORT_CALL SizeOf(const void* const aObject) throw() override {
			return aObject == mBlock ? mSize : 0;
		}

		void* SOLAIRE_EXPORT_CALL Allocate(const size_t aBytes) throw() override {
			if(mAllocated || aBytes > mSize) {
				return nullptr;
			}else {
				mAllocated = true;
				return mBlock;
			}
		}

		bool SOLAIRE_EXPORT_CALL Deallocate(const void* const aObject) throw() override {
			if(aObject != mBlock) return false;
			if(! mAllocated) return false;
			mAllocated = false;
			return true;
		}

		bool SOLAIRE_EXPORT_CALL DeallocateAll() throw() {
			return Deallocate(mBlock);
		}
	};

	////

	class AdvancedMemoryArena : public Allocator {
	public:
		typedef std::pair<void*, uint32_t> Block;
	private:
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

		void* AllocateFromBlocks(const uint32_t aSize) throw() {
			const auto end = mFreeBlocks.end();
			for(auto i = mFreeBlocks.begin(); i != mFreeBlocks.end(); ++i){
				if(i->second >= aSize){
					const Block block = *i;
					mFreeBlocks.erase(i);

					if(block.second > aSize){
						mFreeBlocks.push_back(Block(
							static_cast<uint8_t*>(block.first) + aSize,
							block.second - aSize
							));
						SortBlocks(mFreeBlocks);
					}

					mUsedBlocks.push_back(block);
					return block.first;
				}
			}

			return nullptr;
		}

		void* AllocateFromAllocator(const uint32_t aSize) throw() {
			const Block block(mAllocator.Allocate(aSize), aSize);
			if(! block.first) return nullptr;
			mMainBlocks.push_back(block);
			mUsedBlocks.push_back(block);
			return block.first;
		}
	private:
		Allocator& mAllocator;
		std::vector<Block> mMainBlocks;
		std::vector<Block> mFreeBlocks;
		std::vector<Block> mUsedBlocks;
	public:
		AdvancedMemoryArena(Allocator& aAllocator, const uint32_t aInitialSize) :
			mAllocator(aAllocator)
		{
			const Block block(mAllocator.Allocate(aInitialSize), aInitialSize);
			mMainBlocks.push_back(block);
			mUsedBlocks.push_back(block);
		}

		SOLAIRE_EXPORT_CALL ~AdvancedMemoryArena() throw() {
			for (const Block i : mMainBlocks) mAllocator.Deallocate(i.first);
		}

		// Inherited from Allocator

		uint32_t SOLAIRE_EXPORT_CALL GetAllocatedBytes() const throw()  override {
			uint32_t bytes = 0;
			for(const Block i : mUsedBlocks) bytes += i.second;
			return bytes;
		}

		uint32_t SOLAIRE_EXPORT_CALL GetFreeBytes() const throw() override {
			uint32_t bytes = 0;
			for(const Block i : mFreeBlocks) bytes += i.second;
			return bytes;
		}

		uint32_t SOLAIRE_EXPORT_CALL SizeOf(const void* const aObject) throw() override {
			for(const Block i : mUsedBlocks) if(i.first == aObject) return i.second;
			return 0;
		}

		void* SOLAIRE_EXPORT_CALL Allocate(const size_t aBytes) throw() override {
			void* address = AllocateFromBlocks(aBytes);

			if(! address){
				MergeBlocks(mFreeBlocks);
				SortBlocks(mFreeBlocks);
				address = AllocateFromBlocks(aBytes);
			}

			if(! address) address = AllocateFromAllocator(aBytes);

			return address;
		}

		bool SOLAIRE_EXPORT_CALL Deallocate(const void* const aObject) throw() override {
			auto it = std::find_if(mUsedBlocks.begin(), mUsedBlocks.end(), [aObject](const Block& aBlock) {
				return aBlock.first == aObject;
			});

			if(it == mUsedBlocks.end()) return false;
			const Block block = *it;
			mUsedBlocks.erase(it);
			mFreeBlocks.push_back(block);

			return true;
		}

		bool SOLAIRE_EXPORT_CALL DeallocateAll() throw() {
			mUsedBlocks.clear();
			mFreeBlocks = mMainBlocks;
			return true;
		}
	};

	////

	extern "C" {
		SOLAIRE_EXPORT_API Allocator* SOLAIRE_EXPORT_CALL _CreateMemoryArena(Allocator& aAllocator, Allocator& aArenaAllocator, const uint32_t aInitialSize, const bool aRecycle) throw() {
			return aRecycle ?
				static_cast<Allocator*>(new(aAllocator.Allocate(sizeof(AdvancedMemoryArena))) AdvancedMemoryArena(aArenaAllocator, aInitialSize)) :
				static_cast<Allocator*>(new(aAllocator.Allocate(sizeof(BasicMemoryArena))) BasicMemoryArena(aArenaAllocator, aInitialSize));
		}
	}

}
