#ifndef SOLAIRE_COMPONENTS_COMPOSITE_HPP
#define SOLAIRE_COMPONENTS_COMPOSITE_HPP

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
	\file Composite.hpp
	\brief Contains the code for the Composite class.
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 2.0
	\date
	Created			: 7th September 2015
	Last Modified	: 18th September 2015
*/

#include <mutex>
#include <vector>
#include <functional>
#include "Component.hpp"

namespace Solaire{ namespace Components{
	class Composite
	{
	private:
		Composite(Composite&&);
		Composite(const Composite&);
		Composite& operator=(Composite&&);
		Composite& operator=(const Composite&);

		std::vector<Component*> mComponents;
		bool mInDestructor;
	protected:
		virtual bool PreAttach(const Component& aComponent) const = 0;
		virtual void PostAttach(Component& aComponent) = 0;

		virtual bool PreDetach(const Component& aComponent) const = 0;
		virtual void PostDetach(Component& aComponent) = 0;
	public:
		template<class T>
		static bool CheckType(const Composite& aComposite){
			static_assert(std::is_base_of<Composite, T>::value, "Composite::CheckType() template must derive from Composite");
			return dynamic_cast<const T*>(&aComposite) != nullptr;
		}

		Composite():
            mInDestructor(false)
        {}

		virtual ~Composite(){
		    mInDestructor = true;
            for(Component* component : mComponents){
                Detach(*component);
            }
		}

		bool IsBeingDestroyed() const{
		    return mInDestructor;
		}

		// Component management

		bool Attach(Component& aComponent){
			// Check if the component is already attached to another component
			Composite* parent = aComponent.mParent;
			if(parent == this) {
				return true;
			}else if(parent != nullptr){
				if(! parent->Detach(aComponent)) return false;
			}

			// Perform pre-checks
			if(! (PreAttach(aComponent) && aComponent.PreAttach(*this))) return false;

			// Attach the component
			mComponents.push_back(&aComponent);
			aComponent.mParent = this;

			// Perform post-notifications
			PostAttach(aComponent);
			aComponent.PostAttach();
			return true;
		}

		bool Detach(Component& aComponent){
			// Check if the component is attached to a component
			if(aComponent.mParent != this) {
				return false;
			}

			// Perform pre-checks
			if(! (PreDetach(aComponent) && aComponent.PreDetach())) return false;

			// Detach the component
			mComponents.erase(std::find(mComponents.begin(), mComponents.end(), &aComponent));
			aComponent.mParent = nullptr;

			// Perform post-notifications
			PostDetach(aComponent);
			aComponent.PostDetach(*this);
			return true;
		}

		// Iterators

		typedef std::vector<Component*>::iterator _vector_it;
		typedef std::vector<Component*>::const_iterator _const_vector_it;

		typedef _vector_it component_iterator;
		typedef _const_vector_it const_component_iterator;

#include "TemplatedComponentIterator.inl"

		component_iterator ComponentBegin(){
			return mComponents.begin();
		}

		const_component_iterator ComponentBegin() const{
			return mComponents.begin();
		}

		component_iterator ComponentEnd(){
			return mComponents.end();
		}

		const_component_iterator ComponentEnd() const{
			return mComponents.end();
		}

		template<class T>
		TemplatedComponentIterator<T> ComponentBegin(){
			return TemplatedComponentIterator<T>(mComponents.begin(), mComponents.begin(), mComponents.end());
		}

		template<class T>
		ConstTemplatedComponentIterator<T> ComponentBegin() const{
			return ConstTemplatedComponentIterator<T>(mComponents.begin(), mComponents.begin(), mComponents.end());
		}

		template<class T>
		TemplatedComponentIterator<T> ComponentEnd() {
			return TemplatedComponentIterator<T>(mComponents.begin(), mComponents.begin(), mComponents.end());
		}

		template<class T>
		ConstTemplatedComponentIterator<T> ComponentEnd() const{
			return ConstTemplatedComponentIterator<T>(mComponents.begin(), mComponents.begin(), mComponents.end());
		}

		// Component counting

		size_t GetComponentCount() const{
			return mComponents.size();
		}
	};
}}


#endif
