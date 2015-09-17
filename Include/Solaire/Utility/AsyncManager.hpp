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
#include <deque>
#include "Task.hpp"
#include "..\Core\Macros.hpp"

namespace Solaire{ namespace Utility{

    class AsyncManager: public TaskManager{
    private:
        typedef std::pair<Task*, void*> ProgressBundle;

		std::condition_variable mConditionVariable;
		std::vector<Task*> mActiveTasks;
		std::vector<Task*> mScheduledTasks;
        std::deque<Task*> mPreTasks;
        std::vector<Task*> mPostTasks;
        std::vector<ProgressBundle> mProgressList;
        mutable std::mutex mLock;
        std::atomic_bool mExit;
        std::vector<std::thread> mThreads;

        AsyncManager(const AsyncManager&) = delete;
        AsyncManager(AsyncManager&&) = delete;
        AsyncManager& operator=(const AsyncManager&) = delete;
        AsyncManager& operator=(AsyncManager&&) = delete;

        static void ThreadFunction(AsyncManager* const aManager, const size_t aIndex){
			std::mutex sleepLock;
			std::mutex& managerLock = aManager->mLock;
            Task* currentTask = nullptr;

			const auto GetNextTask = [&]()->Task*{
				Task* tmp = nullptr;

				solaire_synchronized(managerLock,
					if (! aManager->mPreTasks.empty()) {
						tmp = aManager->mPreTasks.front();
						aManager->mPreTasks.pop_front();
					} 
				)

				return tmp;
			};

			const auto SetCurrentTask = [&](Task* const aTask)->void{
				currentTask = aTask;

				solaire_synchronized(managerLock,
					aManager->mActiveTasks[aIndex] = aTask;
				)
			};

			const auto ExecuteTask = [&](Task* const aTask) {
				if (!currentTask->mCanceled) {
					currentTask->mState = Task::EXECUTING;
					currentTask->OnExecute();
				}

				currentTask->mState = Task::POST_EXECUTION;

				solaire_synchronized(managerLock,
					aManager->mPostTasks.push_back(aTask);
				)
			};

			const auto GetCurrentTask = [&]()->Task*{
				return currentTask;
			};

			const auto HasCurrentTask = [&]()->bool{
				return currentTask != nullptr;
			};

            while(! aManager->mExit){
				SetCurrentTask(GetNextTask());

                if(HasCurrentTask()){
					ExecuteTask(GetCurrentTask());
					SetCurrentTask(nullptr);
                }else{
					std::unique_lock<std::mutex> lock(sleepLock);
					aManager->mConditionVariable.wait(lock);
                }
            }
        }

		size_t CountActiveTasks() const{
			size_t count = 0;
			for (const Task* i : mActiveTasks) {
				if (i != nullptr){
					++count;
				}
			}
			return count;
		}

		template<class F>
		size_t ForEachTaskInternal(F aFunction) const{
			size_t count = 0;
			solaire_synchronized(mLock,
				for(Task* i : mScheduledTasks){
					aFunction(*i);
					++count;
				}

				for(Task* i : mPreTasks){
					aFunction(*i);
					++count;
				}

				for(Task* i : mActiveTasks){
					if(i != nullptr){
						aFunction(*i);
						++count;
					}
				}

				for(Task* i : mPostTasks){
					aFunction(*i);
					++count;
				}
			)

			return count;
		}
    protected:
        // Inherited from TaskManager

        void SendProgress(Task* aTask, void* aProgress) override{
			solaire_synchronized(mLock,
                mProgressList.push_back(ProgressBundle(aTask, aProgress));
			)
        }

        void Schedule(Task* aTask) override{
			if(aTask == nullptr) throw std::runtime_error("Cannot schedule null task");

			const Task::State prev = aTask->GetState();
			aTask->mState = Task::SCHEDULED;
			aTask->mCanceled = false;
			aTask->OnScheduled(prev); 
			solaire_synchronized(mLock ,
				mScheduledTasks.push_back(aTask);
			)

			mConditionVariable.notify_one();
        }
    public:
        AsyncManager(const size_t aThreads = 1) :
            mExit(false)
        {
            if(aThreads < 1) throw std::runtime_error("AsyncManager must have at least one thread");
            for(size_t i = 0; i < aThreads; ++i){
				mActiveTasks.push_back(nullptr);
                mThreads.push_back(std::thread(ThreadFunction, this, i));
            }
        }
		 
        ~AsyncManager(){
            mExit = true;
		 	mConditionVariable.notify_all();
            for(std::thread& i : mThreads){
                i.join();
            }
        }

		size_t GetNumberOfWorkers() const{
			return mThreads.size();
		}

        // Inherited from TaskManager

		size_t GetNumberOfTasksQueued() const{
			solaire_synchronized(mLock,
				return
					mScheduledTasks.size() +
		 			mPreTasks.size() +
		 			mPostTasks.size() +
		 			CountActiveTasks();
		 	)
		} 

		bool CanUpdate() const override{
			solaire_synchronized(mLock,
				return (mScheduledTasks.size() + mPostTasks.size() + mProgressList.size()) > 0;
			)
		}

        void Update() override{
			solaire_synchronized(mLock,
				for(Task* i : mScheduledTasks){
					i->mState = Task::PRE_EXECUTION;
					i->OnPreExecute();
					mPreTasks.push_back(i);
				}

				for(Task* i : mPostTasks){
					i->OnPostExecute(i->mCanceled);
					i->mState = i->mCanceled ? Task::CANCELED : Task::EXECUTED;
				}

				for(const ProgressBundle& i : mProgressList){
					i.first->OnRecieveProgress(i.second);
				}

				mScheduledTasks.clear();
				mPostTasks.clear();
				mProgressList.clear();
			)
        }

		size_t ForEachTask(std::function<void(Task&)> aFunction) override{
			return ForEachTaskInternal(aFunction);
		}

		size_t ForEachTask(std::function<void(const Task&)> aFunction) const override{
			return ForEachTaskInternal(aFunction);
		}
    };

}}


#endif
