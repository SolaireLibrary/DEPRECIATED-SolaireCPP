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
	Last Modified	: 9th November 2015
*/

#include <limits>
#include <type_traits>
#include <cstdint>
#include "..\Core\Init.hpp"

namespace Solaire{

    class SOLAIRE_EXPORT_API Allocator{
    public:
        virtual uint32_t SOLAIRE_EXPORT_CALL GetAllocatedBytes() const throw() = 0;
        virtual uint32_t SOLAIRE_EXPORT_CALL GetFreeBytes() const throw() = 0;

        virtual void* SOLAIRE_EXPORT_CALL Allocate(const size_t aBytes) throw() = 0;
        virtual bool SOLAIRE_EXPORT_CALL Deallocate(void* const aObject, const size_t aBytes) throw() = 0;
    };

}

#endif
