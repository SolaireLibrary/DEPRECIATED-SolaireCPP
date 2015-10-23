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
	\version 4.1
	\date
	Created			: 7th September 2015
	Last Modified	: 23rd October 2015
*/

#include "..\Core\Iterators\DereferenceIterator.hpp"
#include "..\Core\Iterators\ConstIterator.hpp"
#include "Component.hpp"

namespace Solaire{ namespace Components{

    class Composite;

    class Composite{
    private:
		DynamicArray<Component*> mComponents;
    private:
		Composite& operator=(const Composite&) = delete;
	protected:
		virtual void OnAttach(Component& aComponent) = 0;
		Composite& operator=(Composite&& aOther);
	public:
		Composite(Allocator& aAllocator);
		Composite(const Composite& aOther);
		Composite(Composite&& aOther);
		virtual ~Composite();

		Allocator& GetAllocator() const;

		// Iterators

		typedef DereferenceIteratorWrapper<Component, typename DynamicArray<Component*>::Iterator> Iterator;
		typedef ConstIteratorWrapper<Component, Iterator> ConstIterator;
		typedef DereferenceIteratorWrapper<Component, typename DynamicArray<Component*>::ReverseIterator> ReverseIterator;
		typedef ConstIteratorWrapper<Component, ReverseIterator> ConstReverseIterator;

		Iterator begin();
		ConstIterator begin() const;
		Iterator end();
		ConstIterator end() const;
		ReverseIterator rbegin();
		ConstReverseIterator rbegin() const;
		ReverseIterator rend();
		ConstReverseIterator rend() const;

		// Value Find

		ConstIterator FindFirst(Component& aValue) const;
		ConstIterator FindNext(ConstIterator aPos, Component& aValue) const;
		ConstIterator FindLast(Component& aValue) const;
		Iterator FindFirst(Component& aValue);
		Iterator FindNext(ConstIterator aPos, Component& aValue);
		Iterator FindLast(Component& aValue);

        // Condition find

		template<class F>
		ConstIterator FindFirstIf(const F aCondition) const{
		    return Iterator(const_cast<DynamicArray<Component*>&>(mComponents).FindFirstIf(aCondition));
        }

		template<class F>
		ConstIterator FindNextIf(ConstIterator aPos, const F aCondition) const{
		    return Iterator(const_cast<DynamicArray<Component*>&>(mComponents).FindNextIf(mComponents.begin() + (ConstIterator(end()) - aPos), aCondition));
        }

		template<class F>
		ConstIterator FindLastIf(const F aCondition) const{
		    return Iterator(const_cast<DynamicArray<Component*>&>(mComponents).FindLastIf(aCondition));
        }

		template<class F>
		Iterator FindFirstIf(const F aCondition){
		    return mComponents.FindFirstIf(aCondition);
        }

		template<class F>
		Iterator FindNextIf(ConstIterator aPos, const F aCondition){
		    return mComponents.FindNextIf(aPos, aCondition);
        }

		template<class F>
		Iterator FindLastIf(const F aCondition){
		    return mComponents.FindLastIf(aCondition);
        }

		// Component-typed find

		template<class COMPONENT>
		const COMPONENT* Get() const{
		    auto it = mComponents.FindFirstIf(CheckComponentType<COMPONENT>);
		    return it == mComponents.end() ? nullptr : reinterpret_cast<const COMPONENT*>(*it);
        }

		template<class COMPONENT>
		COMPONENT* Get(){
            auto it = mComponents.FindFirstIf(CheckComponentType<COMPONENT>);
		    return it == mComponents.end() ? nullptr : reinterpret_cast<COMPONENT*>(*it);
        }

		// Component Attach

		template<class COMPONENT, class ...PARAMS>
		COMPONENT* Attach(PARAMS... aParams){
		    COMPONENT* component = Get<COMPONENT>();

		    if(! component){
                component = new(GetAllocator().Allocate(sizeof(COMPONENT))) COMPONENT(*this, aParams...);
                mComponents.PushBack(component);
                OnAttach(*component);
                return component;
		    }

			return nullptr;
		}
	};

    template<class T>
    static constexpr bool CheckCompositeType(const Composite* const aComposite){
        static_assert(std::is_base_of<Composite, T>::value, "CheckCompositeType() template must derive from Composite");
        return dynamic_cast<const T*>(aComposite) != nullptr;
    }
}}

#include "Composite.inl"


#endif
