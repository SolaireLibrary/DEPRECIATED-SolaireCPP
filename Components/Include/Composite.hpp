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
	Last Modified	: 7th September 2015
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

		typedef std::vector<Component::pointer_t>::iterator component_iterator;
		typedef std::vector<Component::pointer_t>::const_iterator const_component_iterator;

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

		// Foreach loops

		typedef std::function<bool(const Component&)> component_condition;
		typedef std::function<void(Component&)> component_action;
		typedef std::function<void(const Component&)> const_component_action;

		size_t ForEachComponent(const_component_action aCallback) const{
			for(Component::const_pointer_t i : mComponents){
				aCallback(*i);
			}
			return mComponents.size();
		}

		size_t ForEachComponent(component_action aCallback){
			for(Component::pointer_t i : mComponents){
				aCallback(*i);
			}
			return mComponents.size();
		}

		size_t ForEachComponent(component_condition aCondition, const_component_action aCallback) const{
			size_t count = 0;
			for(Component::const_pointer_t i : mComponents){
				const Component& ref = *i;
				if(aCondition(ref)){
					++count;
					aCallback(ref);
				}
			}
			return count;
		}

		size_t ForEachComponent(component_condition aCondition, component_action aCallback){
			size_t count = 0;
			for(Component::pointer_t i : mComponents){
				Component& ref = *i;
				if(aCondition(ref)){
					++count;
					aCallback(ref);
				}
			}
			return count;
		}

		template<class T>
		size_t ForEachComponent(const_component_action aCallback) const{
			return ForEachComponent(Component::CheckType<T>, aCallback);
		}

		template<class T>
		size_t ForEachComponent(component_action aCallback){
			return ForEachComponent(Component::CheckType<T>, aCallback);
		}

		// Component counting

		size_t GetComponentCount() const{
			return mComponents.size();
		}

		size_t GetComponentCount(component_condition aCondition) const{
			size_t count = 0;
			for(Component::const_pointer_t i : mComponents){
				if(aCondition(*i)) ++count;
			}
			return count;
		}

		template<class T>
		size_t GetComponentCount() const{
			return GetComponentCount(Component::CheckType<T>);
		}

		// Component find

		std::vector<const_component_iterator> GetComponents(component_condition aCondition) const{
			const const_component_iterator end = mComponents.end();
			std::vector<const_component_iterator> tmp(mComponents.size());
			for(const_component_iterator i = mComponents.begin(); i != end; ++i){
				if(aCondition(**i)) tmp.push_back(i);
			}
			return tmp;
		}

		std::vector<component_iterator> GetComponents(component_condition aCondition){
			const component_iterator end = mComponents.end();
			std::vector<component_iterator> tmp(mComponents.size());
			for(component_iterator i = mComponents.begin(); i != end; ++i){
				if(aCondition(**i)) tmp.push_back(i);
			}
			return tmp;
		}

		const_component_iterator GetComponent(component_condition aCondition, size_t aIndex = 0) const{
			std::vector<const_component_iterator> list = GetComponents(aCondition);
			return aIndex >= list.size() ? mComponents.end() : list[aIndex];
		}

		component_iterator GetComponent(component_condition aCondition, size_t aIndex = 0){
			std::vector<component_iterator> list = GetComponents(aCondition);
			return aIndex >= list.size() ? mComponents.end() : list[aIndex];
		}

		template<class T>
		std::vector<const_component_iterator> GetComponents() const{
			return GetComponents(Component::CheckType<T>);
		}

		template<class T>
		std::vector<component_iterator> GetComponents(){
			return GetComponents(Component::CheckType<T>);
		}

		template<class T>
		const_component_iterator GetComponent(size_t aIndex = 0) const{
			return GetComponent(Component::CheckType<T>, aIndex);
		}

		template<class T>
		component_iterator GetComponent(size_t aIndex = 0){
			return GetComponent(Component::CheckType<T>, aIndex);
		}
	};
}}


#endif