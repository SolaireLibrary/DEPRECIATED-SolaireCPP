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
Last Modified	: 9th November 2015
*/

#include "Allocator.hpp"

namespace Solaire{

    /*!
        \detail Use different MemoryArenas for different object lifetimes (eg. objects that are destroyed at the end of a frame)
    */
	class SOLAIRE_EXPORT_API MemoryArena : public Allocator{
	public:
		virtual bool SOLAIRE_EXPORT_CALL Clear() throw() = 0;
	};
}

#endif
