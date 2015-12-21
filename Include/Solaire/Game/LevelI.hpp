#ifndef SOLAIRE_LEVELI_HPP
#define SOLAIRE_LEVELI_HPP

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
	\file LevelI.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 21st December 2015
	Last Modified	: 21st December 2015
*/

#include <cstdint>
#include "..\Core\ModuleHeader.hpp"
#include "..\Core\System.hpp"

namespace Solaire{

	class GameI;
	class LevelListenerI;

	SOLAIRE_EXPORT_INTERFACE LevelI {
	public:
		friend GameI;
	protected:
		virtual bool SOLAIRE_EXPORT_CALL OnLoad(GameI&) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnUnload() throw() = 0;
	public:
		virtual SOLAIRE_EXPORT_CALL ~LevelI() {}

		// Helpers
		virtual GameI& SOLAIRE_EXPORT_CALL GetGame() const throw() = 0;

		// Listeners
		virtual bool SOLAIRE_EXPORT_CALL AddListener(LevelListenerI&) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL RemoveListener(LevelListenerI&) throw() = 0;
	};

	SOLAIRE_EXPORT_INTERFACE LevelListenerI{
	public:
		friend LevelI;
	protected:
		virtual bool SOLAIRE_EXPORT_CALL OnLoad(GameI&) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnUnload(GameI&) throw() = 0;
	public:
		virtual SOLAIRE_EXPORT_CALL ~LevelListenerI() {}
	};
}

#endif
