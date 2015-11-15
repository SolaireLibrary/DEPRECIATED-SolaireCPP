#ifndef SOLAIRE_ALLOCATOR_WRAPPERS_HPP
#define SOLAIRE_ALLOCATOR_WRAPPERS_HPP

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
\file AllocatorWrappers.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 15th November 2015
Last Modified	: 15th November 2015
*/

#include "LinkAllocator.hpp"
#include "Solaire\Core\Memory\Allocator.hpp"

namespace Solaire{

	class CoreToLinkAllocator : public LinkAllocator{
	private:
		Allocator& mAllocator;
	public:
		CoreToLinkAllocator(Allocator&);

		uint32_t SOLAIRE_EXPORT_CALL GetAllocatedBytes() const override;
		uint32_t SOLAIRE_EXPORT_CALL GetFreeBytes() const override;

		void* SOLAIRE_EXPORT_CALL Allocate(const size_t) override;
		void SOLAIRE_EXPORT_CALL Deallocate(void* const, const size_t) override;
	};

	class LinkToCoreAllocator : public Allocator {
	private:
		LinkAllocator& mAllocator;
	public:
		LinkToCoreAllocator(LinkAllocator&);

		uint32_t GetAllocatedBytes() const override;
		uint32_t GetFreeBytes() const override;

		void* Allocate(const size_t) override;
		void Deallocate(void* const, const size_t) override;
	};
}

#endif
