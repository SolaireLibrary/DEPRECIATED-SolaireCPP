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

//#include "Frame.hpp"
//#include "Level.hpp"

#include "..\Core\Init.hpp"
#include "..\Core\DataStructures\DynamicArray.hpp"
#include "..\Core\Memory\Allocator.hpp"
#include "..\Core\Memory\MemoryArena.hpp"
#include "..\Core\Time.hpp"
#include "..\Core\Threading\Task.hpp"

namespace Solaire{ namespace Game{

    class GameListener;

    class Game{
    public:
        typedef uint8_t TaskPriority;
    private:
        struct PriorityTask{
            Task* task;
            TaskPriority priority;

            constexpr PriorityTask(const TaskPriority aPriority, Task* const aTask):
                task(aTask),
                priority(aPriority)
            {}
        };

        struct RepeatedTaskList{
            DynamicArray<PriorityTask> taskList;
            uint64_t repeatInterval;
            uint64_t timeLastRun;

            RepeatedTaskList(Allocator& aAllocator, uint64_t aRepeatInterval):
                taskList(aAllocator),
                repeatInterval(aRepeatInterval),
                timeLastRun(0)
            {}
        };
    private:
        MemoryArena mFrameArena;
        Allocator& mGameAllocator;
        Allocator& mLevelAllocator;
        Allocator& mFrameAllocator;

        DynamicArray<RepeatedTaskList> mTasks;
    private:
        void Frame(){
            const uint64_t frameBegin = Timer::GetSystemTimeMilli();
            OnBeginFrame(frameBegin);

            // Pull tasks to run this frame
            DynamicArray<PriorityTask> tasks(GetFrameAllocator());

            for(RepeatedTaskList& list : mTasks){
                const uint64_t timeDifference = frameBegin - list.timeLastRun;
                if(timeDifference > list.repeatInterval){
                    list.timeLastRun = frameBegin;
                    for(PriorityTask task : list.taskList){
                        tasks.PushBack(task);
                    }
                }
            }

            // Sort tasks by priority
            std::sort(tasks.begin(), tasks.end(), [](const PriorityTask aFirst, const PriorityTask aSecond)->bool{
                return aFirst.priority < aSecond.priority;
            });

            // Execute tasks
            //! \todo Execute tasks

            // Wait for tasks to complete
            //! \todo Wait for tasks to complete

            const uint64_t frameEnd = Timer::GetSystemTimeMilli();
            OnEndFrame(frameEnd);

            mFrameArena.Clear();
        }
    protected:
        virtual void OnBeginFrame(const uint64_t aBeginTime) = 0;
        virtual void OnEndFrame(const uint64_t aEndTime) = 0;
    public:
        Game():
            mFrameArena(1000000),
            mGameAllocator(GetDefaultAllocator()),
            mLevelAllocator(GetDefaultAllocator()),
            mFrameAllocator(mFrameArena),
            mTasks(mGameAllocator)
        {}

        virtual ~Game(){

        }

        Allocator& GetGameAllocator() const{
            return mGameAllocator;
        }

        Allocator& GetLevelAllocator() const{
            return mLevelAllocator;
        }

        Allocator& GetFrameAllocator() const{
            return mFrameAllocator;
        }

        /*virtual uint64_t GetElapsedTime() const = 0;

        virtual Allocator& GetGameAllocator() const = 0;
        virtual Allocator& GetLevelAllocator() const = 0;
        virtual Allocator& GetFrameAllocator() const = 0;

        virtual void Pause() = 0;
        virtual uint64_t Resume() = 0;
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

        virtual void ScheduleTask(Task* aTask) = 0;
        virtual void ScheduleTaskRepeat(Task* aTask, const size_t aMilliseconds) = 0;
        virtual void CancelTask(Task* aTask) = 0;*/
    };

    /*class GameListener{
        virtual ~GameListener() = 0;

        virtual void OnPause(Game& aGame) = 0;
        virtual void OnResume(Game& aGame, const uint64_t aTimePaused) = 0;

        virtual void OnFrameBegin(Game& aGame) = 0;
        virtual void OnFrameEnd(Game& aGame, const uint64_t aTimeInFrame) = 0;

        virtual void OnLevelLoaded(Game& aGame) = 0;
        virtual void OnLevelUnloaded(Game& aGame, const uint64_t aTimeInLevel) = 0;
    };*/

}}

#endif
