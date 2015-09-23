#ifndef SOLAIRE_GAME_GAME_HPP
#define SOLAIRE_GAME_GAME_HPP

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
\file Game.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 22nd September 2015
Last Modified	: 22nd September 2015
*/

#include "Frame.hpp"
#include "Level.hpp"

namespace Solaire{ namespace Game{

    class GameListener;

    class Game{
    public:
        virtual ~Game(){}

        virtual uint64_t GetElapsedTime() const = 0;
        virtual Core::MemoryArena& GetArena();

        virtual void Pause() = 0;
        virtual void Resume() = 0;
        virtual bool IsPaused() const = 0;

        virtual void AddListener(GameListener& aListener) = 0;
        virtual void RemoveListener(GameListener& aListener) = 0;

        virtual Frame& CurrentFrame() = 0;
        virtual const Frame& CurrentFrame() const = 0;

        virtual void UnloadLevel() = 0;
        virtual void LoadLevel(LevelFactory& aLevel) = 0;

        virtual bool IsLevelLoaded() const = 0;
        virtual Level& CurrentLevel() = 0;
        virtual const Level& CurrentLevel() const = 0;
    };

    class GameListener{
        virtual ~GameListener() = 0;

        virtual void OnPause(Game& aGame) = 0;
        virtual void OnResume(Game& aGame) = 0;

        virtual void OnFrameBegin(Game& aGame) = 0;
        virtual void OnFrameEnd(Game& aGame) = 0;

        virtual void OnLevelLoaded(Game& aGame) = 0;
        virtual void OnLevelUnloaded(Game& aGame) = 0;
    };

}}

#endif
