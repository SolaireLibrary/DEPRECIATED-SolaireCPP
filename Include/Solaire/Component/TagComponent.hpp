#ifndef SOLAIRE_COMPONENT_TAG_COMPONENT_HPP
#define SOLAIRE_COMPONENT_TAG_COMPONENT_HPP

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
	\file TagComponent.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 1st December 2015
	Last Modified	: 3rd December 2015
*/

#include "Component.hpp"
#include "..\Strings\ConstString.hpp"

namespace Solaire{

	class TagComponent : public Component {
	public:
		enum : ComponentID {
			CLASS_ID = SolaireMakeComponentID("TAG_COMP")
		};
	public:
		virtual bool SOLAIRE_EXPORT_CALL AddTag(const ConstString<char>&) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL HasTag(const ConstString<char>&) const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL RemoveTag(const ConstString<char>&) throw() = 0;
		virtual ~TagComponent() {}
	};

	extern "C" {
		SOLAIRE_EXPORT_API TagComponent* SOLAIRE_EXPORT_CALL CreateTagComponent(Allocator&);
	}

}


#endif
