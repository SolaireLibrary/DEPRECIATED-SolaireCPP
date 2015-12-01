#ifndef SOLAIRE_COMPONENT_TAG_COMPONENT_IMPLEMENTATION_HPP
#define SOLAIRE_COMPONENT_TAG_COMPONENT_IMPLEMENTATION_HPP

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
	\file TagComponentImplementation.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 1st December 2015
	Last Modified	: 1st December 2015
*/

#include "TagComponent.hpp"
#include "..\DataStructures\DynamicArray.hpp"
#include "..\Strings\String.hpp"

namespace Solaire{

	class TagComponentImplementation : public TagComponent {
	private:
		DynamicArray<CString>* mTags;
		Composite* mParent;
	protected:
		// Inherited from Component
		void SOLAIRE_EXPORT_CALL OnAttachment(Composite&) throw() override;
		void SOLAIRE_EXPORT_CALL OnDetachment() throw() override;
		bool SOLAIRE_EXPORT_CALL CanAttachTo(const Composite&) const throw() override;
	public:
		TagComponentImplementation();

		// Inherited from Component
		Allocator& SOLAIRE_EXPORT_CALL GetAllocator() const throw() override;
		Composite& SOLAIRE_EXPORT_CALL GetComposite() const throw() override;
		bool SOLAIRE_EXPORT_CALL IsAttached() const throw() override;
		ComponentID SOLAIRE_EXPORT_CALL GetClassID() const throw() override;
		bool SOLAIRE_EXPORT_CALL CanDetach() const throw() override;
		void SOLAIRE_EXPORT_CALL Destructor() throw() override;

		// Inherited from TagComponent
		bool SOLAIRE_EXPORT_CALL AddTag(const ConstString<char>&) throw() override;
		bool SOLAIRE_EXPORT_CALL HasTag(const ConstString<char>&) const throw() override;
		bool SOLAIRE_EXPORT_CALL RemoveTag(const ConstString<char>&) throw() override;
	};

}


#endif
