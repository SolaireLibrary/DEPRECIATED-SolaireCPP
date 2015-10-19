#ifndef SOLAIRE_COMPONENTS_COMPONENT_HPP
#define SOLAIRE_COMPONENTS_COMPONENT_HPP

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
	\brief Contains the code for the Component class.
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 4.0
	\date
	Created			: 7th September 2015
	Last Modified	: 19th October 2015
*/

#include "..\Core\Memory\Allocator.hpp"

namespace Solaire{ namespace Components{

    class Composite;
    class Component;

    typedef SharedPointer<Component> ComponentPtr;
    typedef SharedPointer<const Component> ConstComponentPtr;

    class Component : public std::enable_shared_from_this<Component>
	{
    public:
        friend Composite;
	private:
		Composite& mParent;
    private:
		Component(Component&&) = delete;
		Component(const Component&) = delete;
		Component& operator=(Component&&) = delete;
		Component& operator=(const Component&) = delete;
	public:
		Component(Composite& aParent);
		virtual ~Component();

		Composite& GetParent() const;
		Allocator& GetAllocator() const;
	};

    template<class T>
    static constexpr bool CheckComponentType(const ConstComponentPtr aComponent){
        static_assert(std::is_base_of<Component, T>::value, "CheckComponentType() template must derive from Component");
        return dynamic_cast<const T*>(aComponent.get()) != nullptr;
    }
}}


#endif
