#ifndef SOLAIRE_GAMEI_HPP
#define SOLAIRE_GAMEI_HPP

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
	\file GameI.hpp
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
#include "LevelI.hpp"

namespace Solaire{

	class GameListenerI;
	class FrameI;
	class Allocator;

	//! \todo Frame management

	SOLAIRE_EXPORT_INTERFACE GameI{
	protected:
		static SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL CallOnLoad(GameI& aGame, LevelI& aLevel) throw() {
			return aLevel.OnLoad(aGame);
		}

		static SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL CallOnUnload(LevelI& aLevel) throw() {
			return aLevel.OnUnload();
		}
	protected:
		virtual bool SOLAIRE_EXPORT_CALL OnLoadLevel(LevelI&) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnUnloadLevel() throw() = 0;

		virtual bool SOLAIRE_EXPORT_CALL LoadLevel(LevelI&) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL UnloadLevel() throw() = 0;
	public:
		virtual SOLAIRE_EXPORT_CALL ~GameI() {}

		// Helpers
		virtual FrameI& SOLAIRE_EXPORT_CALL GetFrame() const throw() = 0;
		virtual Allocator& SOLAIRE_EXPORT_CALL GetAllocator() const throw() = 0;

		// Level Management
		virtual LevelI& SOLAIRE_EXPORT_CALL GetLevel() const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL HasLoadedLevel() const throw() = 0;

		// Listeners
		virtual bool SOLAIRE_EXPORT_CALL AddListener(GameListenerI&) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL RemoveListener(GameListenerI&) throw() = 0;
	};

	SOLAIRE_EXPORT_INTERFACE GameListenerI{
	public:
		friend GameI;
	protected:
	public:
		virtual SOLAIRE_EXPORT_CALL ~GameListenerI() {}
	};
}

#endif
