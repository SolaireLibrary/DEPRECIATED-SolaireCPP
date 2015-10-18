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
Last Modified	: 25th September 2015
*/

#include <algorithm>
#include <type_traits>
#include "Allocator.hpp"
#include "../DataStructures/DynamicArray.hpp"

namespace Solaire{

    /*!
        \class MemoryArena
        \detail
        Use different MemoryArenas for different object lifetimes (eg. objects that are destroyed at the end of a frame)
    */

    class MemoryArena : public Allocator{
    private:
        typedef std::pair<void*, uint32_t> Block;
    private:
        Allocator& mAllocator;
        DynamicArray<Block> mMainBlocks;
        DynamicArray<Block> mBlocks;
        uint32_t mAllocatedBytes;
    private:
        void SortBlocks(){
            std::sort(mBlocks.begin(), mBlocks.end(), [](const Block aFirst, const Block aSecond)->bool{
                return aFirst.second < aSecond.second;
            });
        }

        void MergeBlocks(){
            auto begin = mBlocks.begin();
            auto end = mBlocks.end();

            for(auto i = begin; i != end; ++i){
                for(auto j = begin; j != end; ++j){
                    if(i == j) continue;

                    uint8_t* const ptrA = static_cast<uint8_t*>(i->first);
                    uint8_t* const ptrB = static_cast<uint8_t*>(j->first);
                    const uint32_t sizeA = i->second;
                    const uint32_t sizeB = j->second;

                    if(ptrA + sizeA == ptrB){
                        mBlocks.Erase(j);
                        i->second += sizeB;
                        i = begin;
                        j = begin;
                    }
                }
            }
        }

        void* AllocateFromBlocks(const uint32_t aSize){
            const auto end = mBlocks.end();
            for(auto i = mBlocks.begin(); i != mBlocks.end(); ++i){
                if(i->second >= aSize){
                    const Block block = *i;
                    mBlocks.Erase(i);

                    if(block.second > aSize){
                        mBlocks.PushBack(Block(
                            static_cast<uint8_t*>(block.first) + aSize,
                            block.second - aSize
                        ));
                        SortBlocks();
                    }

                    return block.first;
                }
            }

            return nullptr;
        }
    public:
        MemoryArena(Allocator& aAllocator, const uint32_t aInitialSize):
            mAllocator(aAllocator),
            mMainBlocks(aAllocator),
            mBlocks(aAllocator),
            mAllocatedBytes(0)
        {
            const Block block(mAllocator.Allocate(aInitialSize), aInitialSize);
            mMainBlocks.PushBack(block);
            mBlocks.PushBack(block);
        }

        ~MemoryArena(){
            for(Block i : mBlocks){
                mAllocator.Deallocate(i.first, i.second);
            }
        }

        void Clear(){
            mBlocks.Clear();
            for(Block i : mMainBlocks){
                mBlocks.PushBack(i);
            }
        }

        // Inherited from Allocator

        uint32_t GetAllocatedBytes() const override{
            return mAllocatedBytes;
        }

        uint32_t GetFreeBytes() const override{
            uint32_t count = 0;
            for(const Block i : mBlocks) count += i.second;

            return count - mAllocatedBytes;
        }

        void* Allocate(const size_t aBytes) override{
            void* address = AllocateFromBlocks(aBytes);

            if(! address){
                MergeBlocks();
                SortBlocks();
                address = AllocateFromBlocks(aBytes);
            }

            if(! address){
                const Block block(mAllocator.Allocate(aBytes), aBytes);
                if(! block.first) return nullptr;
                mMainBlocks.PushBack(block);
                address = block.first;
            }

            return address;
        }

        void Deallocate(void* const aObject, const size_t aBytes) override{
            mBlocks.PushBack(Block(aObject, aBytes));
            SortBlocks();
            mAllocatedBytes -= aBytes;
        }
    };


}

#endif
