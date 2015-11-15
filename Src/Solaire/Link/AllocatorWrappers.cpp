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

#include "Solaire\Link\AllocatorWrappers.hpp"

namespace Solaire{
	
	// LinkAllocatorWrapper

	CoreToLinkAllocator::CoreToLinkAllocator(Allocator& aAllocator)  :
		mAllocator(aAllocator)
	{}

	uint32_t CoreToLinkAllocator::GetAllocatedBytes() const {
		return mAllocator.GetAllocatedBytes();
	}

	uint32_t CoreToLinkAllocator::GetFreeBytes() const {
		return mAllocator.GetFreeBytes();
	}

	void* CoreToLinkAllocator::Allocate(const size_t aBytes) {
		return mAllocator.Allocate(aBytes);
	}

	void CoreToLinkAllocator::Deallocate(void* const aObject, const size_t aBytes) {
		return mAllocator.Deallocate(aObject, aBytes);
	}

	// LinkToCoreAllocator

	LinkToCoreAllocator::LinkToCoreAllocator(LinkAllocator& aAllocator) :
		mAllocator(aAllocator)
	{}

	uint32_t LinkToCoreAllocator::GetAllocatedBytes() const {
		return mAllocator.GetAllocatedBytes();
	}

	uint32_t LinkToCoreAllocator::GetFreeBytes() const {
		return mAllocator.GetFreeBytes();
	}

	void* LinkToCoreAllocator::Allocate(const size_t aBytes) {
		return mAllocator.Allocate(aBytes);
	}

	void LinkToCoreAllocator::Deallocate(void* const aObject, const size_t aBytes) {
		return mAllocator.Deallocate(aObject, aBytes);
	}
	
}