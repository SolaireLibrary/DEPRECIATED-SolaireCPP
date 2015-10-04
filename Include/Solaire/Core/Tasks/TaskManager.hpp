#ifndef SOLAIRE_CORE_TASK_MANAGER_HPP
#define SOLAIRE_CORE_TASK_MANAGER_HPP

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
	\file TaskManager.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 2.0
	\date
	Created			: 30th September 2015
	Last Modified	: 30th September 2015
*/

#include "Task.hpp"

namespace Solaire{

    class TaskManager{
    private:
        TaskManager(const TaskManager&) = delete;
        TaskManager(TaskManager&&) = delete;
        TaskManager& operator=(const TaskManager&) = delete;
        TaskManager& operator=(TaskManager&&) = delete;

        #ifdef SOLAIRE_DISABLE_MULTITHREADING
            typedef uint32_t ThreadID;
            typedef DummyLock Mutex;
        #else
            typedef std::thread::id ThreadID;
            typedef std::mutex Mutex;
        #endif

        DynamicArray<Task*> mInitialiseTasks;
        DynamicArray<Task*> mPreExecuteTasks;
        DynamicArray<Task*> mPostExecuteTasks;
        DynamicArray<Task*> mCancelTasks;
        std::map<ThreadID, Task*> mExecutionTasks;

        ThreadID GetThreadID() const;
    protected:
        Mutex mLock;

        bool Execute();
    public:
        friend Task;

        TaskManager(Allocator& aAllocator);

        virtual ~TaskManager();

        virtual bool Schedule(Task& aTask);
        virtual void Update();
        virtual void Wait(const size_t aSleepTime = 33);
        size_t TaskCount() const;

        virtual size_t ThreadCount() const = 0;
    };
}

#include "TaskManager.inl"


#endif
