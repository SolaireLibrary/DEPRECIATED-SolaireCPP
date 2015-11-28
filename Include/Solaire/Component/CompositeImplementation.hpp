#ifndef SOLAIRE_COMPONENT_COMPOSITE_IMPLEMENTATION_HPP
#define SOLAIRE_COMPONENT_COMPOSITE_IMPLEMENTATION_HPP

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

#include "Composite.hpp"
#include "..\DataStructures\DynamicArray.hpp"

namespace Solaire{

	class CompositeImplementation : public Composite {
	private:
		DynamicArray<Component*> mComponents;
	public:
		CompositeImplementation();
		CompositeImplementation(Allocator&);

		// Inherited from Composite

		bool SOLAIRE_EXPORT_CALL Attach(Component&) throw() override;
		bool SOLAIRE_EXPORT_CALL Detach(Component&) throw() override;
		bool SOLAIRE_EXPORT_CALL IsAttached(const Component&) const throw() override;
		uint32_t SOLAIRE_EXPORT_CALL GetComponentCount() const throw() override;
		Component* SOLAIRE_EXPORT_CALL GetComponent(const uint32_t) const throw() override;
		Component* SOLAIRE_EXPORT_CALL GetComponentWithID(const ComponentID) const throw() override;
		Allocator& SOLAIRE_EXPORT_CALL GetAllocator() const throw() override;

		// Inherited from LinkObject

		void SOLAIRE_EXPORT_CALL Destructor() throw() override;
	};

}


#endif
