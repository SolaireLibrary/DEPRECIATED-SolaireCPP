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
	\version 4.0
	\date
	Created			: 7th September 2015
	Last Modified	: 19th October 2015
*/

#include "Component.hpp"

namespace Solaire{ namespace Components{

    class Composite;

    class Composite{
    private:
		DynamicArray<Component*> mComponents;
	private:
		Composite(Composite&&) = delete;
		Composite(const Composite&) = delete;
		Composite& operator=(Composite&&) = delete;
		Composite& operator=(const Composite&) = delete;
	protected:
		virtual void OnAttach(Component& aComponent) = 0;
	public:
		Composite(Allocator& aAllocator):
		    mComponents(aAllocator, 16)
        {}

		virtual ~Composite(){
		    Allocator& allocator = GetAllocator();
            for(Component* component : mComponents){
                component->~Component();
                //! \bug Size of original component class is not deallocated
                allocator.Deallocate(component, sizeof(Component));
            }
		}

		Allocator& GetAllocator() const{
		    return mComponents.GetAllocator();
		}

		// Iterators

		typedef typename DynamicArray<Component*>::Iterator Iterator;
		typedef typename DynamicArray<Component*>::ConstIterator ConstIterator;
		typedef typename DynamicArray<Component*>::ReverseIterator ReverseIterator;
		typedef typename DynamicArray<Component*>::ConstReverseIterator ConstReverseIterator;

		Iterator begin(){return mComponents.begin();}
		ConstIterator begin() const{return mComponents.begin();}
		Iterator end(){return mComponents.end();}
		ConstIterator end() const{return mComponents.end();}
		ReverseIterator rbegin(){return mComponents.rbegin();}
		ConstReverseIterator rbegin() const{return mComponents.rbegin();}
		ReverseIterator rend(){return mComponents.rend();}
		ConstReverseIterator rend() const{return mComponents.rend();}

		// Value Find

		ConstIterator FindFirst(Component* aValue) const{return mComponents.FindFirst(aValue);}
		ConstIterator FindNext(ConstIterator aPos, Component* aValue) const{return mComponents.FindNext(aPos, aValue);}
		ConstIterator FindLast(Component* aValue) const{return mComponents.FindLast(aValue);}
		Iterator FindFirst(Component* aValue){return mComponents.FindFirst(aValue);}
		Iterator FindNext(ConstIterator aPos, Component* aValue){return mComponents.FindNext(aPos, aValue);}
		Iterator FindLast(Component* aValue){return mComponents.FindFirst(aValue);}

        // Condition find

		template<class F>
		ConstIterator FindFirstIf(const F aCondition) const{return mComponents.FindFirstIf(aCondition);}

		template<class F>
		ConstIterator FindNextIf(ConstIterator aPos, const F aCondition) const{return mComponents.FindNext(aPos, aCondition);}

		template<class F>
		ConstIterator FindLastIf(const F aCondition) const{return mComponents.FindLastIf(aCondition);}

		template<class F>
		Iterator FindFirstIf(const F aCondition){return mComponents.FindFirstIf(aCondition);}

		template<class F>
		Iterator FindNextIf(ConstIterator aPos, const F aCondition){return mComponents.FindNextIf(aPos, aCondition);}

		template<class F>
		Iterator FindLastIf(const F aCondition){return mComponents.FindLastIf(aCondition);}

		// Component-typed find

		template<class COMPONENT>
		const COMPONENT* Get() const{
		    ConstIterator it = mComponents.FindFirstIf(CheckComponentType<COMPONENT>);
		    return it == end() ? nullptr : reinterpret_cast<const COMPONENT*>(*it);
        }

		template<class COMPONENT>
		COMPONENT* Get(){
            Iterator it = mComponents.FindFirstIf(CheckComponentType<COMPONENT>);
		    return it == end() ? nullptr : reinterpret_cast<COMPONENT*>(*it);
        }

		// Component Attach

		template<class COMPONENT, class ...PARAMS>
		std::shared_ptr<COMPONENT> Attach(PARAMS... aParams){
		    COMPONENT* component = Get<COMPONENT>();

		    if(! component){
                component = new(GetAllocator().Allocate(sizeof(COMPONENT))) COMPONENT(*this, aParams...);
                mComponents.PushBack(component);
                component->OnAttach(*this);
                return component;
		    }

			return std::shared_ptr<COMPONENT>();
		}
	};

    template<class T>
    static constexpr bool CheckCompositeType(const Composite* const aComposite){
        static_assert(std::is_base_of<Composite, T>::value, "CheckCompositeType() template must derive from Composite");
        return dynamic_cast<const T*>(aComposite) != nullptr;
    }
}}


#endif
