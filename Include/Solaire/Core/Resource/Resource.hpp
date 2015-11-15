#ifndef SOLAIRE_RESOURCE_HPP
#define SOLAIRE_RESOURCE_HPP

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
	\file Resource.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 13th November 2015
	Last Modified	: 13th November 2015
*/

#include <cstdint>
#include <memory>
#include "..\Link\Object.hpp"
#include "..\Link\ObjectUtility.hpp"

namespace Solaire{

	class SOLAIRE_EXPORT_API Resource : public Link::Object{
	public:
		typedef uint64_t ID;
	};

	typedef Link::SharedPtr<Resource> ResourcePtr;
	typedef Link::SharedPtr<const Resource> ConstResourcePtr;
    
}


#endif
