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

#include "Solaire\Memory\AllocationMapper.hpp"

namespace Solaire{

	// AllocationMappers;
	AllocationMapper::AllocationMapper():
		mAllocatedBytes(0)
	{}

	bool AllocationMapper::Allocate(const void* const aAddress, const uint32_t aBytes) throw() {
		if(aAddress == nullptr) return false;
		auto it = mMapping.find(aAddress);
		if(it != mMapping.end()) return false;
		mMapping.emplace(aAddress, aBytes);
		mAllocatedBytes += aBytes;
		return true;
	}

	bool AllocationMapper::Deallocate(const void* const aAddress) throw() {
		if(aAddress == nullptr) return false;
		auto it = mMapping.find(aAddress);
		if(it == mMapping.end()) return false;
		mAllocatedBytes -= it->second;
		mMapping.erase(it);
		return true;
	}

	bool AllocationMapper::DeallocateAll() throw() {
		mAllocatedBytes = 0;
		mMapping.clear();
		return true;
	}

	uint32_t AllocationMapper::GetAllocationSize(const void* const aAddress) const throw() {
		if(aAddress == nullptr) return 0;
		auto it = mMapping.find(aAddress);
		if(it == mMapping.end()) return 0;
		return it->second;
	}

	uint32_t AllocationMapper::GetAllocatedBytes() const throw() {
		return mAllocatedBytes;
	}

}
