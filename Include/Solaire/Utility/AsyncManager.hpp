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

		std::vector<Task*> mActiveTasks;
		std::vector<Task*> mScheduledTasks;
        std::deque<Task*> mPreTasks;
        std::vector<Task*> mPostTasks;
        std::vector<ProgressBundle> mProgressList;

        #ifndef SOLAIRE_DISABLE_MULTITHREADING
            std::condition_variable mConditionVariable;
            mutable std::mutex mLock;
            std::atomic_bool mExit;
            std::vector<std::thread> mThreads;
        #else
            bool mExit;
        #endif

        AsyncManager(const AsyncManager&) = delete;
        AsyncManager(AsyncManager&&) = delete;
        AsyncManager& operator=(const AsyncManager&) = delete;
        AsyncManager& operator=(AsyncManager&&) = delete;

        class Worker{
        private:
            #ifndef SOLAIRE_DISABLE_MULTITHREADING
                std::mutex mSleepLock;
                std::mutex& mManagerLock;
            #endif
            Task* mTask;
            AsyncManager& mManager;
            const size_t mIndex;
        public:
            Worker(AsyncManager& aManager, const size_t aIndex) :
                #ifndef SOLAIRE_DISABLE_MULTITHREADING
                    mManagerLock(aManager.mLock),
                #endif
                mTask(nullptr),
                mManager(aManager),
                mIndex(aIndex)
            {}

            Task* PopTask(){
                Task* task = nullptr;
                solaire_synchronized(mManagerLock,
                    if(! mManager.mPreTasks.empty()){
                        task = mManager.mPreTasks.front();
                        mManager.mPreTasks.pop_front();
                    }
                )
                return task;
            }

            void Execute(Task& aTask){
                if(! aTask.mCanceled){
                    aTask.mState = Task::EXECUTING;
                    aTask.OnExecute();
                }

                aTask.mState = Task::POST_EXECUTION;

                solaire_synchronized(mManagerLock,
                    mManager.mPostTasks.push_back(&aTask);
                )
            }

            void SetTask(){
                mTask = nullptr;
                solaire_synchronized(mManagerLock,
                    mManager.mActiveTasks[mIndex] = nullptr;
                )
            }

            void SetTask(Task& aTask){
                mTask = &aTask;
                solaire_synchronized(mManagerLock,
                    mManager.mActiveTasks[mIndex] = &aTask;
                )
            }

            Task& GetTask(){
                return *mTask;
            }

            bool HasTask() const{
                return mTask != nullptr;
            }

            bool ExecuteCycle(){
                Task* task = PopTask();
                if(task == nullptr){
                    SetTask();
                }else{
                    SetTask(*task);
                }

                if(HasTask()){
                    Execute(GetTask());
                    SetTask();
                    return true;
                }else{
                    return false;
                }
            }

            void ExecuteCycleUntilExit(){
                while(! mManager.mExit){
                    if(! ExecuteCycle()){
                        #ifndef SOLAIRE_DISABLE_MULTITHREADING
                            std::unique_lock<std::mutex> lock(mSleepLock);
                            mManager.mConditionVariable.wait(lock);
                        #endif
                    }
                }
            }
        };

        static void ThreadFunction(Worker aWorker){
            aWorker.ExecuteCycleUntilExit();
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

            #ifndef SOLAIRE_DISABLE_MULTITHREADING
                mConditionVariable.notify_one();
			#endif
        }
    public:
        AsyncManager(size_t aThreads = 1) :
            mExit(false)
        {
            #ifndef SOLAIRE_DISABLE_MULTITHREADING
                if(aThreads < 1) throw std::runtime_error("AsyncManager must have at least one thread");
                for(size_t i = 0; i < aThreads; ++i){
                    mActiveTasks.push_back(nullptr);
                    mThreads.push_back(std::thread(ThreadFunction, this, i));
                }
            #else
                mActiveTasks.push_back(nullptr);
            #endif
        }

        ~AsyncManager(){
            mExit = true;
            #ifndef SOLAIRE_DISABLE_MULTITHREADING
                mConditionVariable.notify_all();
                for(std::thread& i : mThreads){
                    i.join();
                }
            #endif
        }

		size_t GetNumberOfWorkers() const{
		    #ifndef SOLAIRE_DISABLE_MULTITHREADING
                return mThreads.size();
            #else
                return 1;
            #endif
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

            #ifdef SOLAIRE_DISABLE_MULTITHREADING
                {
                    Worker worker(*this, 0);
                    do{
                    }while(worker.ExecuteCycle());
                }
            #endif

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
