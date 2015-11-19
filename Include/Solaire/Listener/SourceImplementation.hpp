#ifndef SOLAIRE_SOURCE_IMPLEMENTATION_HPP
#define SOLAIRE_SOURCE_IMPLEMENTATION_HPP

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
	\file SourceImplementation.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 26th September 2015
	Last Modified	: 19th November 2015
*/

#include "Source.hpp"
#include "Listener.hpp"
#include "..\DataStructures\DynamicArray.hpp"

namespace Solaire{

	class SourceImplementation : public Source {
	private:
		DynamicArray<Listener*> mListeners;
	public:
		SourceImplementation();
		SourceImplementation(Allocator&);
		virtual ~SourceImplementation();

		// Inherited from Source

		bool SOLAIRE_EXPORT_API AddListener(Listener&) throw() override;
		bool SOLAIRE_EXPORT_API RemoveListener(Listener&) throw() override;
		bool SOLAIRE_EXPORT_API HasListener(const Listener&)const throw() override;
		uint32_t SOLAIRE_EXPORT_API GetListenerCount() const throw() override;
		Listener& SOLAIRE_EXPORT_API GetListener(const uint32_t) const throw() override;
	};

}


#endif
