#ifndef SOLAIRE_IMPLEMENTED_LINK_OBJECT_HPP
#define SOLAIRE_IMPLEMENTED_LINK_OBJECT_HPP

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
\file LinkObjectImplementation.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 9th November 2015
Last Modified	: 15th November 2015
*/

#include "LinkObject.hpp"
#include "LinkAllocator.hpp"

namespace Solaire{
	class LinkObjectImplementation : public LinkObject {
	private:
		LinkAllocator& mAllocator;
	protected:
		LinkObjectImplementation(const LinkObjectImplementation&);
		LinkObjectImplementation(LinkObjectImplementation&&);
		LinkObjectImplementation& operator=(const LinkObjectImplementation&);
		LinkObjectImplementation& operator=(LinkObjectImplementation&&);
		
		virtual uint32_t GetClassSize() const = 0;
	public:
		LinkObjectImplementation(LinkAllocator&);
		~LinkObjectImplementation();

		// Inherited from LinkObject

		LinkAllocator& SOLAIRE_EXPORT_CALL GetAllocator() const override;
		virtual void SOLAIRE_EXPORT_CALL Destructor() override;
	};
}

#endif
