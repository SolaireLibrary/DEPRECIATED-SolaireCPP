#ifndef SOLAIRE_COMPONENT_COMPONENT_HPP
#define SOLAIRE_COMPONENT_COMPONENT_HPP

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
	\file Component.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 19th November 2015
	Last Modified	: 1st December 2015
*/

#include "Composite.hpp"

namespace Solaire{

	class CompositeImplementation;

	class Component {
	public:
		friend CompositeImplementation;
	protected:
		virtual void SOLAIRE_EXPORT_CALL OnAttachment(Composite&) throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL OnDetachment() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL CanAttachTo(const Composite&) const throw() = 0;
	public:
		virtual Allocator& SOLAIRE_EXPORT_CALL GetAllocator() const throw() = 0;

		virtual Composite& SOLAIRE_EXPORT_CALL GetComposite() const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL IsAttached() const throw() = 0;

		virtual ComponentID SOLAIRE_EXPORT_CALL GetClassID() const throw() = 0;

		virtual bool SOLAIRE_EXPORT_CALL CanDetach() const throw() = 0;

		virtual void SOLAIRE_EXPORT_CALL Destructor() throw() = 0;

		inline bool SOLAIRE_EXPORT_CALL AttachTo(Composite& aComposite) throw() {
			return aComposite.Attach(*this);
		}

		inline bool SOLAIRE_EXPORT_CALL Detach() throw() {
			return GetComposite().Detach(*this);
		}

		inline bool SOLAIRE_EXPORT_CALL IsAttachedTo(const Composite& aComposite) const throw() {
			return IsAttached() ? &GetComposite() == &aComposite : false;
		}
	};

}


#endif
