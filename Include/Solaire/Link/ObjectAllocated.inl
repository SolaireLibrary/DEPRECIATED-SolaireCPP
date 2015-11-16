#ifndef SOLAIRE_LINK_ALLOCATED_OBJECT_INL
#define SOLAIRE_LINK_ALLOCATED_OBJECT_INL

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
\file ObjectAllocated.inl
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 16th November 2015
Last Modified	: 16th November 2015
*/

namespace Solaire{ namespace Link{

	template<class T>
	AllocatedObject<T>::AllocatedObject(Allocator& aAllocator) throw() :
		mAllocator(aAllocator)
	{}

	template<class T>
	Allocator& AllocatedObject<T>::GetAllocator() const throw() {
		return mAllocator;
	}

	template<class T>
	AllocatedObject<T>::void SOLAIRE_EXPORT_CALL Free() throw() {
		Destructor();
		mAllocator.Deallocate(this, sizeof(T));
	}

}}

#endif
