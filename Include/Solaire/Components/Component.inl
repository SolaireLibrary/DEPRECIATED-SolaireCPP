#ifndef SOLAIRE_COMPONENTS_COMPONENT_INL
#define SOLAIRE_COMPONENTS_COMPONENT_INL

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
	\file Component.inl
	\brief Contains the code for the Component class.
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 4.0
	\date
	Created			: 19th September 2015
	Last Modified	: 19th October 2015
*/

#include "Composite.hpp"

namespace Solaire{ namespace Components{

    // Component

    Component::Component(Composite& aParent) :
        mParent(aParent)
    {}

    Component::~Component(){

    }

    Composite& Component::GetParent() const{
        return mParent;
    }

    Allocator& Component::GetAllocator() const{
        return mParent->GetAllocator();
    }
}}


#endif
