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

		std::vector<Component::pointer_t> mComponents;

		static std::vector<Composite*> COMPOSITE_TRACKER;
		static std::mutex COMPOSITE_TRACKER_LOCK;
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

		static size_t CompositeCount(){
			size_t count = 0;
			COMPOSITE_TRACKER_LOCK.lock();
			count = COMPOSITE_TRACKER.size();
			COMPOSITE_TRACKER_LOCK.unlock();
			return count;
		}

		template<class CONDITION, class CALLBACK>
		static size_t ForEachComposite(CONDITION aCondition, CALLBACK aCallback){
			size_t count = 0;
			COMPOSITE_TRACKER_LOCK.lock();
			for(Composite* i : COMPOSITE_TRACKER){
				if(aCondition(*i)){
					++count;
					aCallback(*i);
				}
			}
			COMPOSITE_TRACKER_LOCK.unlock();
			return count;
		}

		template<class COMPOSITE, class CALLBACK>
		static size_t ForEachComposite(CALLBACK aCallback){
			return ForEachComposite(Composite::CheckType<COMPOSITE>, aCallback);
		}

		template<class CONDITION, class CALLBACK>
		static size_t ForEachComponent(CONDITION aCondition, CALLBACK aCallback){
			size_t count = 0;
			COMPOSITE_TRACKER_LOCK.lock();
			for(Composite* i : COMPOSITE_TRACKER){
				count += i->ForEachComponent(aCondition, aCallback);
			}
			COMPOSITE_TRACKER_LOCK.unlock();
			return count;
		}

		template<class COMPONENT, class CALLBACK>
		static size_t ForEachComponent(CALLBACK aCallback){
			return ForEachComposite(Component::CheckType<COMPONENT>, [&](Component& aComponent){
				aCallback(reinterpret_cast<T&>(aComponent));
			});
		}

		Composite(){
			// Register with tracker
			COMPOSITE_TRACKER_LOCK.lock();
			COMPOSITE_TRACKER.push_back(this);
			COMPOSITE_TRACKER_LOCK.unlock();
		}

		virtual ~Composite(){
			// Un-register with tracker
			COMPOSITE_TRACKER_LOCK.lock();
			COMPOSITE_TRACKER.erase(std::find(COMPOSITE_TRACKER.begin(), COMPOSITE_TRACKER.end(), this));
			COMPOSITE_TRACKER_LOCK.unlock();
		}

		// Component management

		bool Attach(Component::pointer_t aComponent){
			// Get a reference to the component
			if(aComponent.get() == nullptr) return false;
			Component& ref = *aComponent;

			// Check if the component is already attached to another component
			Composite* parent = ref.mParent;
			if(parent == this) {
				return true;
			}else if(parent != nullptr){
				if(! parent->Detach(aComponent)) return false;
			}

			// Perform pre-checks
			if(! (PreAttach(ref) && ref.PreAttach(*this))) return false;

			// Attach the component
			mComponents.push_back(aComponent);
			ref.mParent = this;

			// Perform post-notifications
			PostAttach(ref);
			ref.PostAttach();
			return true;
		}
		
		bool Detach(Component::pointer_t aComponent){
			// Get a reference to the component
			if(aComponent.get() == nullptr) return false;
			Component& ref = *aComponent;

			// Check if the component is attached to a component
			if(ref.mParent != this) {
				return false;
			}

			// Perform pre-checks
			if(! (PreDetach(ref) && ref.PreDetach())) return false;

			// Detach the component
			mComponents.erase(std::find(mComponents.begin(), mComponents.end(), aComponent));
			ref.mParent = nullptr;

			// Perform post-notifications
			PostDetach(ref);
			ref.PostDetach(*this);
			return true;
		}

		// Iterators

		//typedef std::vector<Component::pointer_t>::iterator component_iterator;
		//typedef std::vector<Component::pointer_t>::const_iterator const_component_iterator;

		typedef std::vector<Component::pointer_t>::iterator _vector_it;
		typedef std::vector<Component::pointer_t>::const_iterator _const_vector_it;

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