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
	Last Modified	: 19th November 2015
*/

#include "..\Core\Init.hpp"

namespace Solaire{

	class Listener;

	SOLAIRE_EXPORT_INTERFACE Source {
	protected:
		virtual bool SOLAIRE_EXPORT_CALL CanAcceptListener(const Listener&) throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL OnListen(Listener&) throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL OnUnlisten(const Listener&) throw() = 0;
	public:
		virtual bool SOLAIRE_EXPORT_CALL AddListener(Listener&) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL RemoveListener(Listener&) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL HasListener(const Listener&) const throw() = 0;

		virtual uint32_t SOLAIRE_EXPORT_CALL GetListenerCount() const throw() = 0;
		virtual Listener& SOLAIRE_EXPORT_CALL GetListener(const uint32_t) const throw() = 0;
	};

}


#endif
