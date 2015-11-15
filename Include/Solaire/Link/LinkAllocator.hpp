#ifndef SOLAIRE_LINK_ALLOCATOR_HPP
#define SOLAIRE_LINK_ALLOCATOR_HPP

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
\file LinkAllocator.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 15th November 2015
Last Modified	: 15th November 2015
*/

#include <cstdint>
#include "Link.inl"

namespace Solaire{
	class SOLAIRE_EXPORT_API LinkAllocator{
	public:
		virtual uint32_t SOLAIRE_EXPORT_CALL GetAllocatedBytes() const = 0;
		virtual uint32_t SOLAIRE_EXPORT_CALL GetFreeBytes() const = 0;

		virtual void* SOLAIRE_EXPORT_CALL Allocate(const size_t) = 0;
		virtual void SOLAIRE_EXPORT_CALL Deallocate(void* const, const size_t) = 0;
	};
}

#endif
