#ifndef SOLAIRE_ASYNC_MANAGER_HPP
#define SOLAIRE_ASYNC_MANAGER_HPP

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
	\file AsyncManager.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 2.0
	\date
	Created			: 11th September 2015
	Last Modified	: 27th September 2015
*/

#include "Task.hpp"

#ifndef SOLAIRE_DISABLE_MULTITHREADING

namespace Solaire{

    Class AsyncManager : public TaskManager{
    private:
        static void WorkerThread(AsyncManager* aManager){
            while(! aManager->mExit){
                if(! aManager->Execute()){
                    std::unique_lock<Mutex> lock(aManager->aLock);
                    aManager->mConditionVariable.wait(lock);
                }
            }
        }

        std::vector<std::thread> mThreads;
        std::condition_variable mConditionVariable;
        bool mExit;
    public:
        AsyncManager(const size_t aThreads, Allocator<Task*> aAllocator = GetDefaultAllocator<void*>()):
            TaskManager(aAllocator),
            mExit(false)
        {
            for(size_t i = 0; i < aThreads; ++i){
                mThreads.push_back(std::thread(WorkerThread, this));
            }
        }

        ~AsyncManager(){
            mExit = true;
            mConditionVariable.notify_all();
            for(std::thread& thread : mThreads){
                mThreads.join();
            }
        }

        // Inherited from TaskManager

        size_t ThreadCount() const override{
            return mThreads.size();
        }

        bool Schedule(Task& aTask) override{
            if(TaskManager::Schedule(aTask)){
                mConditionVariable.notify_one();
                return true;
            }else{
                return false;
            }
        }
    };
}

#else
    #if SOLAIRE_WARNINGS
        #warning SolaireCPP : Utility::AsyncManager cannot be compiled when SOLAIRE_DISABLE_MULTITHREADING is defined
    #else
        #error SolaireCPP : Utility::AsyncManager cannot be compiled when SOLAIRE_DISABLE_MULTITHREADING is defined
    #endif
#endif


#endif
