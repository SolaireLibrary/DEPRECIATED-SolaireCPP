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

#include "Solaire\Memory\DefaultAllocator.hpp"

namespace Solaire{

	// DefaultAllocator

	DefaultAllocator::DefaultAllocator(){

	}

	uint32_t DefaultAllocator::GetAllocatedBytes() const throw() {
		return mAllocations.GetAllocatedBytes();
	}

	uint32_t DefaultAllocator::GetFreeBytes() const throw() {
		return UINT32_MAX - GetAllocatedBytes();
	}

	void* DefaultAllocator::Allocate(const size_t aBytes) throw() {
		void* const tmp = operator new(aBytes);
		mAllocations.Allocate(tmp, aBytes);
		return tmp;
	}

	bool DefaultAllocator::Deallocate(void* const aObject) throw() {
		operator delete(aObject);
		mAllocations.Deallocate(aObject);
		return true;
	}

}
