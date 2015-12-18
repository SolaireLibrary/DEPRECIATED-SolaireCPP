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
		Allocator& mAllocator;
		std::mutex mLock;
		std::condition_variable mPreCondition;
		std::deque<std::thread> mWorkers;
		std::deque<SharedAllocation<TaskImplementation>> mInitialiseList;
		std::deque<SharedAllocation<TaskImplementation>> mPreList;
		std::map<std::thread::id, SharedAllocation<TaskImplementation>> mExecuteList;
		std::deque<SharedAllocation<TaskImplementation>> mMainExecuteList;
		std::deque<SharedAllocation<TaskImplementation>> mPauseList;
		std::deque<SharedAllocation<TaskImplementation>> mPostList;
		std::deque<SharedAllocation<TaskImplementation>> mPrimaryBufferList;
		std::deque<SharedAllocation<TaskImplementation>> mSecondaryBufferList;
		std::deque<SharedAllocation<TaskImplementation>> mTertiaryBufferList;
		bool mExitFlag;
	private:
		void NotifyWorkers(const uint32_t aCount) {
			if(aCount >= mWorkers.size()) {
				mPreCondition.notify_all();
			}else {
				for(uint32_t i = 0; i < aCount; ++i) mPreCondition.notify_one();
			}
		}
	public:
		ThreadPool(Allocator& aAllocator, const uint32_t aThreads) throw() :
			mAllocator(aAllocator),
			mExitFlag(false)
		{
			for (uint32_t i = 0; i < aThreads; ++i) {
				mWorkers.push_back(std::thread(&ThreadPool::WorkerFunction, this));
				mExecuteList.emplace(mWorkers.back().get_id(), SharedAllocation<TaskImplementation>());
			}
		}

		SOLAIRE_EXPORT_CALL ~ThreadPool() throw() {
			mExitFlag = true;
			{
				std::lock_guard<std::mutex> lock(mLock);
				for (SharedAllocation<TaskImplementation> i : mPreList) i->Cancel();
				for (SharedAllocation<TaskImplementation> i : mPostList) i->Cancel();
				for (SharedAllocation<TaskImplementation> i : mPauseList) i->Cancel();
				mInitialiseList.clear();
				mPreList.clear();
				mPostList.clear();
				mPauseList.clear();
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
			count += mInitialiseList.size();
			count += mPreList.size();
			count += mPauseList.size();
			count += mPostList.size();
			count += mPrimaryBufferList.size();
			count += mSecondaryBufferList.size();
			count += mTertiaryBufferList.size();
			count += mMainExecuteList.size();
			for(const auto& i : mExecuteList) if(i.second) ++count;
			return count;
		}
		
		bool SOLAIRE_EXPORT_CALL Schedule(TaskI& aTask) throw() override {
			{
				const SharedAllocation<TaskImplementation> task = mAllocator.SharedAllocate<TaskImplementation>(aTask);
				if (!task->Initialise()) return false;

				if (task->GetState() != TaskI::STATE_INITIALISED) return false;

				std::lock_guard<std::mutex> lock(mLock);
				mInitialiseList.push_back(task);
			}
			return true;
		}

		bool SOLAIRE_EXPORT_CALL Update() throw() override {

			// Pre-execution
			{
				std::lock_guard<std::mutex> lock(mLock);
				std::swap(mPrimaryBufferList, mInitialiseList);
			}

			for(SharedAllocation<TaskImplementation> i : mPrimaryBufferList) if(! i->PreExecute()) i->Cancel();

			uint32_t preCount = 0;
			{
				std::lock_guard<std::mutex> lock(mLock);
				preCount = mPreList.size();
				for(SharedAllocation<TaskImplementation> i : mPrimaryBufferList) if(i->GetState() == TaskI::STATE_PRE_EXECUTE) {
					if(i->ExecuteOnMain()) {
						mMainExecuteList.push_back(i);
					}else {
						mPreList.push_back(i);
					}
				}
				preCount = mPreList.size() - preCount;
				mPrimaryBufferList.clear();
				mPrimaryBufferList.swap(mPauseList);
			}

			NotifyWorkers(preCount);

			// Resume
			const uint64_t time = GetTimeMilliseconds();
			for(SharedAllocation<TaskImplementation> i : mPrimaryBufferList) if (i->GetPauseDuration() + i->GetPauseTime() <= time) {
				mSecondaryBufferList.push_back(i);
			}else {
				mTertiaryBufferList.push_back(i);
			}

			mPrimaryBufferList.clear();

			{
				std::lock_guard<std::mutex> lock(mLock);
				mPrimaryBufferList.swap(mPostList);
				for(SharedAllocation<TaskImplementation> i : mSecondaryBufferList) mPreList.push_front(i);
				mTertiaryBufferList.swap(mPauseList);
			}

			NotifyWorkers(mSecondaryBufferList.size());

			mSecondaryBufferList.clear();
			mTertiaryBufferList.clear();

			// Post-execution

			for(SharedAllocation<TaskImplementation> i : mPrimaryBufferList) if(! i->PostExecute()) i->Cancel();
			mPrimaryBufferList.clear();

			// Execution

			for(SharedAllocation<TaskImplementation> i : mMainExecuteList) {
				bool result = true;
				switch (i->GetState()) {
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
					switch (i->GetState()) {
					case TaskI::STATE_POST_EXECUTE :
						mSecondaryBufferList.push_back(i);
						break;
					case TaskI::STATE_PAUSED :
						mPrimaryBufferList.push_back(i);
						break;
					}
				
					continue;
				}
			}
			mMainExecuteList.clear();
			std::swap(mMainExecuteList, mPrimaryBufferList);

			{
				std::lock_guard<std::mutex> lock(mLock);
				for(SharedAllocation<TaskImplementation> i : mSecondaryBufferList) mPostList.push_back(i);
			}
			mSecondaryBufferList.clear();

			return true;
		}
	private:
		void WorkerFunction() throw() {
			while(! mExitFlag) {
				SharedAllocation<TaskImplementation>& task = mExecuteList[std::this_thread::get_id()];
				{
					std::lock_guard<std::mutex> lock(mLock);
					if(! mPreList.empty()) {
						task.Swap(mPreList.front());
						mPreList.pop_front();
					}                
				}  

				if(task) {
					bool result = false;
					switch (task->GetState()) {
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
						switch (task->GetState()) {
						case TaskI::STATE_POST_EXECUTE :
							{
								std::lock_guard<std::mutex> lock(mLock);
								mPostList.push_back(task);
							}
							break;
						case TaskI::STATE_PAUSED :
							{
								std::lock_guard<std::mutex> lock(mLock);
								mPauseList.push_back(task);
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
    