#ifndef SOLAIRE_COMPONENT_COMPOSITE_HPP
#define SOLAIRE_COMPONENT_COMPOSITE_HPP

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
	\file Source.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 19th November 2015
	Last Modified	: 19th November 2015
*/

#include "..\Core\Init.hpp"
#include "..\Link\Object.hpp"

namespace Solaire{

	class Component;
	typedef uint64_t ComponentID;

	class SOLAIRE_EXPORT_API Composite : public Link::Object {
	public:
		virtual bool SOLAIRE_EXPORT_CALL Attach(Component&) = 0;
		virtual bool SOLAIRE_EXPORT_CALL Detach(Component&) = 0;

		virtual uint32_t SOLAIRE_EXPORT_CALL GetComponentCount() const = 0;
		virtual Component& SOLAIRE_EXPORT_CALL GetComponent(const uint32_t) const = 0;

		virtual bool SOLAIRE_EXPORT_CALL HasComponentWithID(const ComponentID) const = 0;
		virtual Component& SOLAIRE_EXPORT_CALL GetComponentWithID(const ComponentID) const = 0;
	};

}


#endif
