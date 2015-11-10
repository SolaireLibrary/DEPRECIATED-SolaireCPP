#ifndef SOLAIRE_SOURCE_HPP
#define SOLAIRE_SOURCE_HPP

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
	\file Source.hpp
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
    class Source : private SourceBase
    {
    protected:
        virtual void OnListenerAdded(ListenerType& aListener) = 0;
        virtual void OnListenerRemoved(ListenerType& aListener) = 0;

        typedef typename DynamicArray<ListenerType*>::Iterator ListenerIterator;
        typedef typename DynamicArray<ListenerType*>::ConstIterator ConstListenerIterator;

        ListenerIterator ListenerBegin(){
            return reinterpret_cast<ListenerIterator>(SourceBase::ListenerBegin());
        }

        ConstListenerIterator ListenerBegin() const{
            return reinterpret_cast<ConstListenerIterator>(SourceBase::ListenerBegin());
        }

        ListenerIterator ListenerEnd(){
            return reinterpret_cast<ListenerIterator>(SourceBase::ListenerEnd());
        }

        ConstListenerIterator ListenerEnd() const{
            return reinterpret_cast<ConstListenerIterator>(SourceBase::ListenerEnd());
        }

        // Inherited from SourceBase

        void OnListenerAdded(ListenerBase& aListener){
            OnListenerAdded(reinterpret_cast<ListenerType&>(aListener));
        }

        void OnListenerRemoved(ListenerBase& aListener){
            OnListenerRemoved(reinterpret_cast<ListenerType&>(aListener));
        }
    public:
        friend ListenerType;

		Source() :
			SourceBase(DEFAULT_ALLOCATOR)
		{
			static_assert(std::is_base_of<SourceBase, SourceType>::value, "Ui::Source : SourceType must inherit SourceBase");
			static_assert(std::is_base_of<ListenerBase, ListenerType>::value, "Ui::Source : ListenerType must inherit ListenerBase");
		}

        Source(Allocator& aAllocator = GetDefaultAllocator()) :
            SourceBase(aAllocator)
        {
            static_assert(std::is_base_of<SourceBase, SourceType>::value, "Ui::Source : SourceType must inherit SourceBase");
            static_assert(std::is_base_of<ListenerBase, ListenerType>::value, "Ui::Source : ListenerType must inherit ListenerBase");
        }

        bool AddListener(ListenerType& aListener){
            return SourceBase::AddListener(aListener);
        }

        bool RemoveListener(ListenerType& aListener){
            return SourceBase::RemoveListener(aListener);
        }

        virtual ~Source(){

        }
    };

}


#endif
