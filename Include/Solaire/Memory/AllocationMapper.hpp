#ifndef SOLAIRE_ALLOCATION_MAPPER_HPP
#define SOLAIRE_ALLOCATION_MAPPER_HPP

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
	\file AllocationMapper.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 16th November 2015
	Last Modified	: 16th November 2015
*/

#include <map>
#include <map>

namespace Solaire{

    class AllocationMapper{
	private:
		std::map<const void*, uint32_t> mMapping;
		uint32_t mAllocatedBytes;
    public:
		AllocationMapper();

		bool Allocate(const void* const, const uint32_t) throw();
		bool Deallocate(const void* const) throw();
		bool DeallocateAll() throw();
		uint32_t GetAllocationSize(const void* const) const throw();
		uint32_t GetAllocatedBytes() const throw();
    };

}

#endif
