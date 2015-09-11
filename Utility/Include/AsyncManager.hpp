#ifndef SOLAIRE_UTILITY_ASYNC_MANAGER_HPP
#define SOLAIRE_UTILITY_ASYNC_MANAGER_HPP

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
	\version 1.0
	\date
	Created			: 11th September 2015
	Last Modified	: 11th September 2015
*/

#include <thread>
#include <mutex>
#include <vector>
#include "Task.hpp"

namespace Solaire{ namespace Utility{

    class AsyncManager: public TaskManager{
    private:
        typedef std::pair<Task*, void*> ProgressBundle;

        Task* mActiveTask;
        std::vector<Task*> mPreTasks;
        std::vector<Task*> mPostTasks;
        std::vector<ProgressBundle> mProgressList;
        std::mutex mLock;
        std::atomic_bool mExit;
        std::thread mTread;

        AsyncManager(const AsyncManager&) = delete;
        AsyncManager(AsyncManager&&) = delete;
        AsyncManager& operator=(const AsyncManager&) = delete;
        AsyncManager& operator=(AsyncManager&&) = delete;

        static void ThreadFunction(AsyncManager* aManager){
            Task* task = nullptr;
            while(! aManager->mExit){
                aManager->mLock.lock();
                    if(! aManager->mPreTasks.empty()){
                        task = aManager->mPreTasks.back();
                        aManager->mPreTasks.pop_back();
                    }
                aManager->mLock.unlock();

                if(task != nullptr){
                    task->Execute();

                    aManager->mLock.lock();
                        aManager->mPreTasks.push_back(task);
                    aManager->mLock.unlock();

                    task = nullptr;
                }else{
                    //! \TODO Sleep until a new task is added or mExit is set to true
                }
            }
        }
    protected:
        // Inherited from TaskManager

        void SendProgress(Task* aTask, void* aProgress) override{
            mLock.lock();
                mProgressList.push_back(ProgressBundle(aTask, aProgress));
            mLock.unlock();
        }

        void Schedule(Task* aTask) override{
            aTask->PreExecute();
            mLock.lock();
                mPreTasks.push_back(aTask);
            mLock.unlock();
        }
    public:
        AsyncManager() :
            mExit(false)
        {
            //! \TODO Launch thread
            //mThread = std::thread(ThreadFunction, this);
        }

        ~AsyncManager(){
            mExit = true;
            //! \TODO Join with thread
            //mThread.join();
        }

        // Inherited from TaskManager

        void Update() override{
            mLock.lock();
                for(Task* i : mPostTasks){
                    i->PostExecute();
                }

                for(const ProgressBundle& i : mProgressList){
                    i.first->OnRecieveProgress(i.second);
                }

                mPostTasks.clear();
                mProgressList.clear();
            mLock.unlock();
        }
    };

}}


#endif