#ifndef SOLAIRE_LISTENER_HPP
#define SOLAIRE_LISTENER_HPP

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
	\file Listener.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 26th September 2015
	Last Modified	: 26th September 2015
*/

#include "SourceBase.hpp"
#include "ListenerBase.hpp"

namespace Solaire{

    template<class SourceType, class ListenerType>
    class Listener : private ListenerBase
    {
    protected:
        virtual void OnSourceAdded(SourceType& aSource) = 0;
        virtual void OnSourceRemoved(SourceType& aSource) = 0;

        typedef typename DynamicArray<SourceType*>::Iterator SourceIterator;
        typedef typename DynamicArray<SourceType*>::ConstIterator ConstSourceIterator;

        SourceIterator SourceBegin(){
            return reinterpret_cast<SourceIterator>(ListenerBase::SourceBegin());
        }

        ConstSourceIterator SourceBegin() const{
            return reinterpret_cast<ConstSourceIterator>(ListenerBase::SourceBegin());
        }

        SourceIterator SourceEnd(){
            return reinterpret_cast<SourceIterator>(ListenerBase::SourceEnd());
        }

        ConstSourceIterator SourceEnd() const{
            return reinterpret_cast<ConstSourceIterator>(ListenerBase::SourceEnd());
        }

        // Inherited from ListenerBase

        void OnSourceAdded(SourceBase& aSource){
            OnSourceAdded(reinterpret_cast<SourceType&>(aSource));
        }

        void OnSourceRemoved(SourceBase& aSource){
            OnSourceRemoved(reinterpret_cast<SourceType&>(aSource));
        }
    public:
        friend SourceType;

		Listener() :
			ListenerBase(DEFAULT_ALLOCATOR)
		{
			static_assert(std::is_base_of<SourceBase, SourceType>::value, "Ui::Source : SourceType must inherit SourceBase");
			static_assert(std::is_base_of<ListenerBase, ListenerType>::value, "Ui::Source : ListenerType must inherit ListenerBase");
		}

        Listener(Allocator& aAllocator = GetDefaultAllocator()) :
            ListenerBase(aAllocator)
        {
            static_assert(std::is_base_of<SourceBase, SourceType>::value, "Ui::Source : SourceType must inherit SourceBase");
            static_assert(std::is_base_of<ListenerBase, ListenerType>::value, "Ui::Source : ListenerType must inherit ListenerBase");
        }

        bool AddSource(SourceType& aSource){
            return ListenerBase::AddSource(aSource);
        }

        bool RemoveSource(SourceType& aSource){
            return ListenerBase::RemoveSource(aSource);
        }

        virtual ~Listener(){

        }
    };

}


#endif
