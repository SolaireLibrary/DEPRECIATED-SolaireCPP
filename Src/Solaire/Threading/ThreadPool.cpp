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

#include "TaskImplementation.hpp"
#include "Solaire\Threading\TaskExecutorI.hpp"
#include "Solaire\Memory\Allocator.hpp"
#include "Solaire\Memory\SmartAllocation.hpp"
#include "Solaire\Core\System.hpp"

#include <mutex>
#include <thread>
#include <condition_variable>
#include <deque>
#include <map>
#include <memory>

namespace Solaire {

	class ThreadPool : public TaskExecutorI {
	public:
		friend TaskImplementation;
	private:
		typedef std::deque<SharedAllocation<TaskImplementation>> TaskQueue;
	private:
		Allocator& mAllocator;
		std::mutex mLock;
		std::condition_variable mPreCondition;
		std::deque<std::thread> mWorkers;
		TaskQueue mPreQueue;
		TaskQueue mExeQueue;
		std::map<std::thread::id, SharedAllocation<TaskImplementation>> mWorkerMap;
		TaskQueue mMainExeQueue;
		TaskQueue mPauseQueue;
		TaskQueue mPostQueue;
		TaskQueue mPrimaryBuffer;
		TaskQueue mSecondaryBuffer;
		TaskQueue mTertiaryBuffer;
		bool mExitFlag;
	private:
		void NotifyWorkers(const uint32_t aCount) throw() {
			if(aCount >= mWorkers.size()) {
				mPreCondition.notify_all();
			}else {
				for(uint32_t i = 0; i < aCount; ++i) mPreCondition.notify_one();
			}
		}

		bool UpdatePreExecute() throw() {
			// Move the tasks currently in the initialise queue to a thread safe location
			{
				std::lock_guard<std::mutex> lock(mLock);
				std::swap(mPrimaryBuffer, mPreQueue);
			}

			// Pre-execute the tasks
			for (SharedAllocation<TaskImplementation> i : mPrimaryBuffer) if (!i->PreExecute()) i->Cancel();

			// Move the tasks to their correct execution queues
			uint32_t taskAddedForWorkers = 0;
			{
				std::lock_guard<std::mutex> lock(mLock);
				for(SharedAllocation<TaskImplementation> i : mPrimaryBuffer) {
					const TaskI::Configuration config = i->GetConfiguration();
					if(config.State == TaskI::STATE_PRE_EXECUTE) {
						if(config.Execution == TaskI::EXECUTE_ON_MAIN) {
							mMainExeQueue.push_back(i);
						}else {
							mExeQueue.push_back(i);
							++taskAddedForWorkers;
						}
					}
				}
				mPrimaryBuffer.clear();
			}

			// Notify workers of how many Tasks were added to the execution queue
			NotifyWorkers(taskAddedForWorkers);
			return true;
		}

		bool UpdateResume() throw() {
			// mPrimaryBuffer		= List of paused tasks
			// mSecondaryBuffer		= List of tasks with expired pause
			// mTertiaryBuffer		= List of tasks that are still paused

			// Move the tasks currently in the pause queue to a thread safe location
			{
				std::lock_guard<std::mutex> lock(mLock);
				std::swap(mPauseQueue, mPrimaryBuffer);
			}

			// Check if the pause timer has expired for each task and place them in the corresponding list
			const uint64_t time = GetTimeMilliseconds();
			for(SharedAllocation<TaskImplementation> i : mPrimaryBuffer) {
				const TaskI::Configuration config = i->GetConfiguration();
				if(config.PauseTime + config.PauseDuration <= time) {
					mSecondaryBuffer.push_back(i);
				}else {
					mTertiaryBuffer.push_back(i);
				}
			}

			// All tasks have been examined, so clear the list
			mPrimaryBuffer.clear();

			// Protect the main queues from thread access and syncronise the queues
			{
				std::lock_guard<std::mutex> lock(mLock);
				for (SharedAllocation<TaskImplementation> i : mSecondaryBuffer) mExeQueue.push_front(i);
				mTertiaryBuffer.swap(mPauseQueue);
			}

			// Notify workers of how many Tasks were added to the execution queue
			NotifyWorkers(mSecondaryBuffer.size());

			mSecondaryBuffer.clear();
			mTertiaryBuffer.clear();
			return true;
		}

		bool UpdatePostExecute() throw() {
			// Move the tasks currently in the post-execution queue to a thread safe location
			{
				std::lock_guard<std::mutex> lock(mLock);
				std::swap(mPostQueue, mPrimaryBuffer);
			}

			// Post-execute the tasks
			for(SharedAllocation<TaskImplementation> i : mPrimaryBuffer) if (!i->PostExecute()) i->Cancel();
			mPrimaryBuffer.clear();
			return true;
		}

		bool UpdateExecute() throw() {
			// mPrimaryBuffer		= List of tasks that were paused during execution
			// mSecondaryBuffer		= List of tasks that executed sucessfully

			// Attempt to execute each task in the main thread execution queue and place them into the corresponding list
			for(SharedAllocation<TaskImplementation> i : mMainExeQueue) {
				bool result = true;
				switch (i->GetConfiguration().State) {
				case TaskI::STATE_PAUSED:
					result = i->Resume();
					break;
				case TaskI::STATE_PRE_EXECUTE:
					result = i->Execute();
					break;
				default:
					i.Swap(SharedAllocation<TaskImplementation>());
					continue;
				}

				if(! result) {
					i->Cancel();
					i.Swap(SharedAllocation<TaskImplementation>());
					continue;
				}

				if(i) {
					switch (i->GetConfiguration().State) {
					case TaskI::STATE_POST_EXECUTE :
						mSecondaryBuffer.push_back(i);
						break;
					case TaskI::STATE_PAUSED :
						mPrimaryBuffer.push_back(i);
						break;
					}
				
					continue;
				}
			}

			// Return the paused tasks to the main thread execution queue
			mMainExeQueue.clear();
			std::swap(mMainExeQueue, mPrimaryBuffer);

			// Add the executed tasks to the post-execute queue
			{
				std::lock_guard<std::mutex> lock(mLock);
				for(SharedAllocation<TaskImplementation> i : mSecondaryBuffer) mPostQueue.push_back(i);
			}
			mSecondaryBuffer.clear();

			return true;
		}
	public:
		ThreadPool(Allocator& aAllocator, const uint32_t aThreads) throw() :
			mAllocator(aAllocator),
			mExitFlag(false)
		{
			for (uint32_t i = 0; i < aThreads; ++i) {
				mWorkers.push_back(std::thread(&ThreadPool::WorkerFunction, this));
				mWorkerMap.emplace(mWorkers.back().get_id(), SharedAllocation<TaskImplementation>());
			}
		}

		SOLAIRE_EXPORT_CALL ~ThreadPool() throw() {
			mExitFlag = true;
			{
				std::lock_guard<std::mutex> lock(mLock);
				for (SharedAllocation<TaskImplementation> i : mExeQueue) i->Cancel();
				for (SharedAllocation<TaskImplementation> i : mPauseQueue) i->Cancel();
				for (SharedAllocation<TaskImplementation> i : mPostQueue) i->Cancel();
				mPreQueue.clear();
				mExeQueue.clear();
				mPauseQueue.clear();
				mPauseQueue.clear();
			}
			mPreCondition.notify_all();
			for (std::thread& i : mWorkers) i.join();
		}

		// Inherited from TaskExecutorI

		void SOLAIRE_EXPORT_CALL WaitAll() throw() override {
			bool success = false;
			do{
				success = WaitAllFor(UINT32_MAX);
			}while(! success);
		}

		bool SOLAIRE_EXPORT_CALL WaitAllFor(const uint32_t aMilliseconds) throw() override {
			enum {
				WAIT_TIME = 1
			};

			for(uint32_t i = 0; i < aMilliseconds; i += WAIT_TIME) {
				Update();
				if(GetTaskCount() == 0) return true;
				std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));
			}

			return false;
		}

		uint32_t SOLAIRE_EXPORT_CALL GetThreadCount() throw() override {
			return mWorkers.size();
		}

		uint32_t SOLAIRE_EXPORT_CALL GetTaskCount() throw() override {
			uint32_t count = 0;
			std::lock_guard<std::mutex> lock(mLock);
			count += mPreQueue.size();
			count += mExeQueue.size();
			count += mPostQueue.size();
			count += mPauseQueue.size();
			count += mPrimaryBuffer.size();
			count += mSecondaryBuffer.size();
			count += mTertiaryBuffer.size();
			count += mMainExeQueue.size();
			for(const auto& i : mWorkerMap) if(i.second) ++count;
			return count;
		}
		
		bool SOLAIRE_EXPORT_CALL Schedule(TaskI& aTask) throw() override {
			{
				const SharedAllocation<TaskImplementation> task = mAllocator.SharedAllocate<TaskImplementation>(aTask);
				if (!task->Initialise()) return false;

				if (task->GetConfiguration().State != TaskI::STATE_INITIALISED) return false;

				std::lock_guard<std::mutex> lock(mLock);
				mPreQueue.push_back(task);
			}
			return true;
		}

		bool SOLAIRE_EXPORT_CALL Update() throw() override {
			return
				UpdatePreExecute() &&
				UpdateResume() &&
				UpdateExecute() &&
				UpdatePostExecute();
		}
	private:
		void WorkerFunction() throw() {
			while(! mExitFlag) {
				SharedAllocation<TaskImplementation>& task = mWorkerMap[std::this_thread::get_id()];
				{
					std::lock_guard<std::mutex> lock(mLock);
					if(! mExeQueue.empty()) {
						task.Swap(mExeQueue.front());
						mExeQueue.pop_front();
					}                
				}  

				if(task) {
					bool result = false;
					switch (task->GetConfiguration().State) {
					case TaskI::STATE_PAUSED:
						result = task->Resume();
						break;
					case TaskI::STATE_PRE_EXECUTE:
						result = task->Execute();
						break;
					default:
						task.Swap(SharedAllocation<TaskImplementation>());
						continue;
					}

					if(! result) {
						task->Cancel();
						task.Swap(SharedAllocation<TaskImplementation>());
						continue;
					}

					if(task) {
						switch (task->GetConfiguration().State) {
						case TaskI::STATE_POST_EXECUTE :
							{
								std::lock_guard<std::mutex> lock(mLock);
								mPostQueue.push_back(task);
							}
							break;
						case TaskI::STATE_PAUSED :
							{
								std::lock_guard<std::mutex> lock(mLock);
								mPauseQueue.push_back(task);
							}
							break;
						}
				
						continue;
					}
				}else {
					std::unique_lock<std::mutex> lock(mLock);
					mPreCondition.wait(lock);
				}
			}
		}
	};
	                 
	extern "C" {
		SOLAIRE_EXPORT_API TaskExecutorI* SOLAIRE_EXPORT_CALL _CreateThreadPool(Allocator& aAllocator, const uint32_t aThreads) throw() {
			return aAllocator.RawAllocate<ThreadPool>(aAllocator, aThreads);
		}
	}

}
    