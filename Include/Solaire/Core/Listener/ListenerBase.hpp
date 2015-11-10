#ifndef SOLAIRE_LISTENER_BASE_HPP
#define SOLAIRE_LISTENER_BASE_HPP

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
	\file ListenerBase.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 26th September 2015
	Last Modified	: 26th September 2015
*/

#include "..\DataStructures\DynamicArray.hpp"

namespace Solaire{

	class SourceBase;

    class ListenerBase{
    private:
        DynamicArray<SourceBase*> mSources;
    protected:
        virtual void OnSourceAdded(SourceBase& aSource) = 0;
        virtual void OnSourceRemoved(SourceBase& aSource) = 0;

        typedef DynamicArray<SourceBase*>::Iterator SourceIterator;
        typedef DynamicArray<SourceBase*>::ConstIterator ConstSourceIterator;

		SourceIterator SourceBegin();
		ConstSourceIterator SourceBegin() const;
		SourceIterator SourceEnd();
		ConstSourceIterator SourceEnd() const;

		bool AddSource(SourceBase& aSource);
		bool RemoveSource(SourceBase& aSource);
    public:
        friend SourceBase;

		ListenerBase();
		ListenerBase(Allocator& aAllocator);
		virtual ~ListenerBase();
    };

}


#endif
