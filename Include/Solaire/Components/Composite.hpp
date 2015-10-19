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

    typedef SharedPointer<Composite> CompositePtr;
    typedef SharedPointer<const Composite> ConstCompositePtr;

    class Composite : public std::enable_shared_from_this<Composite>
	{
    private:
		DynamicArray<ComponentPtr> mComponents;
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

		}

		Allocator& GetAllocator() const{
		    return mComponents.GetAllocator();
		}

		// Iterators

		typedef typename DynamicArray<ComponentPtr>::Iterator Iterator;
		typedef typename DynamicArray<ComponentPtr>::ConstIterator ConstIterator;
		typedef typename DynamicArray<ComponentPtr>::ReverseIterator ReverseIterator;
		typedef typename DynamicArray<ComponentPtr>::ConstReverseIterator ConstReverseIterator;

		Iterator begin(){return mComponents.begin();}
		ConstIterator begin() const{return mComponents.begin();}
		Iterator end(){return mComponents.end();}
		ConstIterator end() const{return mComponents.end();}
		ReverseIterator rbegin(){return mComponents.rbegin();}
		ConstReverseIterator rbegin() const{return mComponents.rbegin();}
		ReverseIterator rend(){return mComponents.rend();}
		ConstReverseIterator rend() const{return mComponents.rend();}

		// Value Find

		ConstIterator FindFirst(ComponentPtr aValue) const{return mComponents.FindFirst(aValue);}
		ConstIterator FindNext(ConstIterator aPos, ComponentPtr aValue) const{return mComponents.FindNext(aPos, aValue);}
		ConstIterator FindLast(ComponentPtr aValue) const{return mComponents.FindLast(aValue);}
		Iterator FindFirst(ComponentPtr aValue){return mComponents.FindFirst(aValue);}
		Iterator FindNext(ConstIterator aPos, ComponentPtr aValue){return mComponents.FindNext(aPos, aValue);}
		Iterator FindLast(ComponentPtr aValue){return mComponents.FindFirst(aValue);}

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
		std::shared_ptr<const COMPONENT> Get() const{
		    ConstIterator it = mComponents.FindFirst(CheckComponentType<COMPONENT>);
		    return it == end() ? std::shared_ptr<const COMPONENT>() : *it;
        }

		template<class COMPONENT>
		std::shared_ptr<COMPONENT> Get(){
            Iterator it = mComponents.FindFirst(CheckComponentType<COMPONENT>);
		    return it == end() ? std::shared_ptr<COMPONENT>() : *it;
        }

		// Component Attach

		template<class COMPONENT, class ...PARAMS>
		std::shared_ptr<COMPONENT> Attach(PARAMS... aParams){
		    std::shared_ptr<COMPONENT> component = Get<COMPONENT>();

		    if(! component){
                component = SharedAllocate<COMPONENT>(*this, aParams...);
                mComponents.PushBack(component);
                component->OnAttach(*this);
		    }

			return component;
		}
	};

    template<class T>
    static constexpr bool CheckCompositeType(const ConstCompositePtr aComposite){
        static_assert(std::is_base_of<Composite, T>::value, "CheckCompositeType() template must derive from Composite");
        return dynamic_cast<const T*>(aComposite.get()) != nullptr;
    }
}}


#endif
