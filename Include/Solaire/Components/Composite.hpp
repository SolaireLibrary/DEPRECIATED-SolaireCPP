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
	\version 3.0
	\date
	Created			: 7th September 2015
	Last Modified	: 1st October 2015
*/

#include <vector>
#include "Component.hpp"

namespace Solaire{ namespace Components{

    class Composite : public std::enable_shared_from_this<Composite>
	{
	private:
		Composite(Composite&&) = delete;
		Composite(const Composite&) = delete;
		Composite& operator=(Composite&&) = delete;
		Composite& operator=(const Composite&) = delete;

		Core::DynamicArray<ComponentPointer> mComponents;
		bool mInDestructor;
	protected:
		virtual bool PreAttach(const ConstComponentPointer aComponent) const = 0;
		virtual void PostAttach(const ComponentPointer aComponent) = 0;

		virtual bool PreDetach(const ComponentPointer aComponent) const = 0;
		virtual void PostDetach(const ConstComponentPointer aComponent) = 0;
	public:
		template<class T>
		static bool CheckType(const ConstCompositePointer aComposite){
			static_assert(std::is_base_of<Composite, T>::value, "Composite::CheckType() template must derive from Composite");
			return dynamic_cast<const T*>(&aComposite) != nullptr;
		}

		Composite(Core::Allocator& aAllocator):
		    mComponents(32, aAllocator),
            mInDestructor(false)
        {}

		virtual ~Composite(){
		    mInDestructor = true;
            for(ComponentPointer component : mComponents){
                Detach(component);
            }
		}

		Core::Allocator& GetAllocator() const{
		    return mComponents.GetAllocator();
		}

		// Component management

		bool Attach(const ComponentPointer aComponent){
			// Check if the component is already attached to another component
			const CompositePointer parent = aComponent->mParent;
			if(parent.get() == this) {
				return true;
			}else if(parent){
				if(! parent->Detach(aComponent)) return false;
			}

			const CompositePointer thisPtr = shared_from_this();

			// Perform pre-checks
			if(! (PreAttach(aComponent) && aComponent->PreAttach(thisPtr))) return false;

			// Attach the component
			mComponents.PushBack(aComponent);
			aComponent->mParent = thisPtr;

			// Perform post-notifications
			PostAttach(aComponent);
			aComponent->PostAttach();
			return true;
		}

		bool Detach(const ComponentPointer aComponent){
			// Check if the component is attached to a component
			if(aComponent->mParent.get() != this) {
				return false;
			}

			const CompositePointer thisPtr = shared_from_this();

			// Perform pre-checks
			if(! mInDestructor){
                if(! (PreDetach(aComponent) && aComponent->PreDetach())) return false;

                // Detach the component
                mComponents.Erase(mComponents.FindFirst(aComponent));
			}

			// Remove composite's parent
			aComponent->mParent.reset();

			// Perform post-notifications
			PostDetach(aComponent);
			aComponent->PostDetach(thisPtr);
			return true;
		}

		// Iterators

		typedef typename Core::DynamicArray<ComponentPointer>::Iterator Iterator;
		typedef typename Core::DynamicArray<ComponentPointer>::ConstIterator ConstIterator;
		typedef typename Core::DynamicArray<ComponentPointer>::ReverseIterator ReverseIterator;
		typedef typename Core::DynamicArray<ComponentPointer>::ConstReverseIterator ConstReverseIterator;

		Iterator begin(){return mComponents.begin();}
		ConstIterator begin() const{return mComponents.begin();}
		Iterator end(){return mComponents.end();}
		ConstIterator end() const{return mComponents.end();}
		ReverseIterator rbegin(){return mComponents.rbegin();}
		ConstReverseIterator rbegin() const{return mComponents.rbegin();}
		ReverseIterator rend(){return mComponents.rend();}
		ConstReverseIterator rend() const{return mComponents.rend();}

		// Value Find

		ConstIterator FindFirst(ComponentPointer aValue) const{return mComponents.FindFirst(aValue);}
		ConstIterator FindNext(ConstIterator aPos, ComponentPointer aValue) const{return mComponents.FindNext(aPos, aValue);}
		ConstIterator FindLast(ComponentPointer aValue) const{return mComponents.FindLast(aValue);}
		Iterator FindFirst(ComponentPointer aValue){return mComponents.FindFirst(aValue);}
		Iterator FindNext(ConstIterator aPos, ComponentPointer aValue){return mComponents.FindNext(aPos, aValue);}
		Iterator FindLast(ComponentPointer aValue){return mComponents.FindFirst(aValue);}

        // Condition find

		template<class F>
		typename std::enable_if<decltype(mComponents)::CheckIfCondition<F>(), ConstIterator>::type
		FindFirst(const F aCondition) const{return mComponents.FindFirst(aCondition);}

		template<class F>
		typename std::enable_if<decltype(mComponents)::CheckIfCondition<F>(), ConstIterator>::type
		FindNext(ConstIterator aPos, const F aCondition) const{return mComponents.FindNext(aPos, aCondition);}

		template<class F>
		typename std::enable_if<decltype(mComponents)::CheckIfCondition<F>(), ConstIterator>::type
		FindLast(const F aCondition) const{return mComponents.FindLast(aCondition);}

		template<class F>
		typename std::enable_if<decltype(mComponents)::CheckIfCondition<F>(), Iterator>::type
		FindFirst(const F aCondition){return mComponents.FindFirst(aCondition);}

		template<class F>
		typename std::enable_if<decltype(mComponents)::CheckIfCondition<F>(), Iterator>::type
		FindNext(ConstIterator aPos, const F aCondition){return mComponents.FindNext(aPos, aCondition);}

		template<class F>
		typename std::enable_if<decltype(mComponents)::CheckIfCondition<F>(), Iterator>::type
		FindLast(const F aCondition){return mComponents.FindLast(aCondition);}

		// Component-typed find

		template<class T>
		typename std::enable_if<std::is_base_of<Component, T>::type, ConstIterator>::type
		FindFirst() const{
            return mComponents.FindFirst<decltype(Component::CheckType<T>)>(Component::CheckType<T>);
		}

		template<class T>
		typename std::enable_if<std::is_base_of<Component, T>::type, ConstIterator>::type
		FindNext(const ConstIterator aPos) const{
            return mComponents.FindNext<decltype(Component::CheckType<T>)>(aPos, Component::CheckType<T>);
		}

		template<class T>
		typename std::enable_if<std::is_base_of<Component, T>::type, ConstIterator>::type
		FindLast() const{
            return mComponents.FindLast<decltype(Component::CheckType<T>)>(Component::CheckType<T>);
		}
	};
}}


#endif
