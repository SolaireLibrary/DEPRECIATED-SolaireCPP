#ifndef SOLAIRE_TASK_MANAGER_INL
#define SOLAIRE_TASK_MANAGER_INL

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
	\file TaskManager.inl
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 2.0
	\date
	Created			: 30th September 2015
	Last Modified	: 30th September 2015
*/

#include "TaskManager.hpp"

namespace Solaire{

    // TaskManager

    ThreadID TaskManager::GetThreadID() const{
        #ifdef SOLAIRE_DISABLE_MULTITHREADING
            return 0;
        #else
            return std::this_thread::get_id();
        #endif
    }

    bool TaskManager::Execute(){
        // Acquire task from manager
        Task* task = nullptr;
        solaire_synchronized(mLock,
            if(! mPreExecuteTasks.IsEmpty()){
                task = mPreExecuteTasks.PopBack();
                mExecutionTasks.emplace(GetThreadID(), task);
            }
        )

        // If a task could not be acquired
        if(task == nullptr) goto TASK_NULL;

        // Check if the task is canceled
        if(task->GetState() == Task::STATE_CANCELED) goto TASK_CANCELED;


        // If the task is paused, resume it
        if(task->GetState() == Task::STATE_PAUSED){
            if(! task->Resume()) goto TASK_FAIL;
        }

        // Execute the task
        if(! task->Execute()) goto TASK_FAIL;

        // Check which state the task is in after execution
        switch(task->GetState()){
        case Task::STATE_PAUSED:
            goto TASK_PAUSED;
        case Task::STATE_CANCELED:
            goto TASK_CANCELED;
        default:
            goto TASK_SUCCESS;
        }


        TASK_NULL:
        return false;

        TASK_CANCELED:
        solaire_synchronized(mLock,
            mCancelTasks.PushBack(task);
            mExecutionTasks.emplace(GetThreadID(), nullptr);
        )

        TASK_PAUSED:
        solaire_synchronized(mLock,
            mPreExecuteTasks.PushBack(task);
            mExecutionTasks.emplace(GetThreadID(), nullptr);
        )
        return true;

        TASK_SUCCESS:
        solaire_synchronized(mLock,
            mPostExecuteTasks.PushBack(task);
            mExecutionTasks.emplace(GetThreadID(), nullptr);
        )
        return true;

        TASK_FAIL:
        solaire_synchronized(mLock,
            mExecutionTasks.emplace(GetThreadID(), nullptr);
        )
        return true;

    }

    TaskManager::TaskManager(Allocator& aAllocator) :
        mInitialiseTasks(128, aAllocator),
        mPreExecuteTasks(128, aAllocator),
        mPostExecuteTasks(128, aAllocator),
        mCancelTasks(128, aAllocator)
    {}

    TaskManager::~TaskManager(){
        solaire_synchronized(mLock,
            for(Task* task : mInitialiseTasks){
                task->Cancel();
                mCancelTasks.PushBack(task);
            }
            mInitialiseTasks.Clear();

            for(Task* task : mPreExecuteTasks){
                task->Cancel();
                mCancelTasks.PushBack(task);
            }
            mPreExecuteTasks.Clear();

            for(Task* task : mPostExecuteTasks){
                task->Cancel();
                mCancelTasks.PushBack(task);
            }
            mPostExecuteTasks.Clear();

            for(Task* task : mCancelTasks){
                task->OnEndState();
            }
            mCancelTasks.Clear();
        )
    }

    bool TaskManager::Schedule(Task& aTask){
        if(aTask.GetState() != Task::STATE_INITIALISED) return false;
        aTask.mState = Task::STATE_SCHEDULED;

        solaire_synchronized(mLock,
            mInitialiseTasks.PushBack(&aTask);
        )
        return true;
    }

    void TaskManager::Update(){
        solaire_synchronized(mLock,
            // OnPreExecute
            for(Task* task : mInitialiseTasks){
                task->Schedule(*this);
                if(task->GetState() == Task::STATE_CANCELED){
                    mCancelTasks.PushBack(task);
                }else if(task->PreExecute()){
                    mPreExecuteTasks.PushBack(task);
                }
            }
            mInitialiseTasks.Clear();

            // OnProgress
            for(std::pair<ThreadID, Task*> task : mExecutionTasks){
                task.second->HandleProgress();
            }

            // OnPostExecute
            for(Task* task : mPostExecuteTasks){
                if(task->GetState() == Task::STATE_CANCELED){
                    mCancelTasks.PushBack(task);
                }else if(task->GetState() != Task::STATE_PAUSED){
                    task->PostExecute();
                }
            }
            mPostExecuteTasks.Clear();

            // OnCancel
            for(Task* task : mCancelTasks){
                task->OnEndState();
            }
            mCancelTasks.Clear();
        )
    }

    size_t TaskManager::TaskCount() const{
        size_t count = 0;
        solaire_synchronized(mLock,
            for(const std::pair<ThreadID, Task*>& i : mExecutionTasks){
                if(i.second != nullptr) ++count;
            }
            count += mInitialiseTasks.Size();
            count += mPreExecuteTasks.Size();
            count += mPostExecuteTasks.Size();
            count += mCancelTasks.Size();
        )
        return count;
    }

    void TaskManager::Wait(const size_t aSleepTime){
        while(TaskCount() > 0){
            Update();
            #ifndef SOLAIRE_DISABLE_MULTITHREADING
                std::this_thread::sleep_for(std::chrono::milliseconds(aSleepTime));
            #endif
        }
    }
}


#endif
