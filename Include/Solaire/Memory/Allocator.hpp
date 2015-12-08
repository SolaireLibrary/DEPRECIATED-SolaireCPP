#ifndef SOLAIRE_ALLOCATOR_HPP
#define SOLAIRE_ALLOCATOR_HPP

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
	\file Allocator.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 25th September 2015
	Last Modified	: 3rd December 2015
*/

#include <cstdint>
#include "..\Core\Init.hpp"

namespace Solaire{

    class Allocator{
    public:
        virtual uint32_t SOLAIRE_EXPORT_CALL GetAllocatedBytes() const throw() = 0;
        virtual uint32_t SOLAIRE_EXPORT_CALL GetFreeBytes() const throw() = 0;
		virtual uint32_t SOLAIRE_EXPORT_CALL SizeOf(const void* const) throw() = 0;

        virtual void* SOLAIRE_EXPORT_CALL Allocate(const size_t) throw() = 0;
        virtual bool SOLAIRE_EXPORT_CALL Deallocate(const void* const) throw() = 0;

		virtual bool SOLAIRE_EXPORT_CALL DeallocateAll() throw() = 0;

		virtual SOLAIRE_EXPORT_CALL ~Allocator(){}
    };



#ifdef SOLAIRE_EXPORT_IMPORT_LIBRARY
	extern "C" SOLAIRE_EXPORT_API Allocator& SOLAIRE_EXPORT_CALL _GetDefaultAllocator() throw();
	extern "C" SOLAIRE_EXPORT_API Allocator* SOLAIRE_EXPORT_CALL _CreateMemoryArena(Allocator&, Allocator&, const uint32_t, const bool) throw();

	inline Allocator& GetDefaultAllocator() { return _GetDefaultAllocator(); }
	inline Allocator* CreateMemoryArena(Allocator& aAllocator, Allocator& aArenaAllocator, const uint32_t aBytes, const bool aRecycle){ return _CreateMemoryArena(aAllocator, aArenaAllocator, aBytes, aRecycle); }
#else 
	static Allocator& (SOLAIRE_EXPORT_CALL *GetDefaultAllocator)();
	static Allocator* (SOLAIRE_EXPORT_CALL *CreateMemoryArena)(Allocator&, Allocator&, const uint32_t, const bool);
#endif

}

#endif
