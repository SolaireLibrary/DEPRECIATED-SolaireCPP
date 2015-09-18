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
	\version 2.0
	\date 
	Created			: 7th September 2015
	Last Modified	: 7th September 2015
*/

#include <memory>

namespace Solaire{ namespace Components{

	class Composite;

	class Component : public std::enable_shared_from_this<Component>
	{
	private:
		Component(Component&&);
		Component(const Component&);
		Component& operator=(Component&&);
		Component& operator=(const Component&);

		Composite* mParent;
	protected:
		virtual bool PreAttach(Composite& aNewParent) const = 0;
		virtual void PostAttach() = 0;

		virtual bool PreDetach() const = 0;
		virtual void PostDetach(Composite& aOldParent) = 0;
	public:
		friend Composite;

		typedef std::shared_ptr<Component> pointer_t;
		typedef std::shared_ptr<const Component> const_pointer_t;

		template<class T>
		static bool CheckType(const Component& aComponent){
			static_assert(std::is_base_of<Component, T>::value, "Component::CheckType() template must derive from Component");
			return dynamic_cast<const T*>(&aComponent) != nullptr;
		}

		Component() :
			mParent(nullptr)
		{

		}

		virtual ~Component(){

		}

		bool HasParent() const{
			return mParent != nullptr;
		}

		Composite& GetParent(){
			return *mParent;
		}

		const Composite& GetParent() const{
			return *mParent;
		}
	};
}}


#endif