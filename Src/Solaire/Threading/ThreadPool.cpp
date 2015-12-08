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

#include "Solaire\Threading\TaskI.hpp"
#include "Solaire\Threading\TaskCallbacks.hpp"
#include "Solaire\Threading\TaskExecutorI.hpp"
#include "Solaire\Memory\Allocator.hpp"

#include <mutex>
#include <thread>
#include <condition_variable>
#include <deque>
#include <map>
#include <memory>

namespace Solaire {

	class ThreadPool;

	class TaskImplementation : public TaskCallbacks {
	public:
		std::mutex Lock;
		std::condition_variable WaitCondition;
		ThreadPool& Executor;
		TaskI& Task;

		TaskImplementation(ThreadPool&, TaskI&) throw();
		SOLAIRE_EXPORT_CALL ~TaskImplementation() throw();

		bool Initialise() throw();
		bool PreExecute() throw();
		bool Execute() throw();
		bool Resume() throw();
		bool PostExecute() throw();

		// Inherited from TaskCallbacks
		
		bool SOLAIRE_EXPORT_CALL Cancel() throw() override;
		bool SOLAIRE_EXPORT_CALL Wait() throw() override;
		bool SOLAIRE_EXPORT_CALL WaitFor(const uint32_t) throw() override;
	};

	typedef std::shared_ptr<TaskImplementation> TaskImplementationPtr;

	class ThreadPool : public TaskExecutorI {
	public:
		friend TaskImplementation;
	private:
		Allocator& mAllocator;
		std::mutex mLock;
		std::condition_variable mPreCondition;
		std::deque<std::thread> mWorkers;
		std::deque<TaskImplementationPtr> mInitialiseList;
		std::deque<TaskImplementationPtr> mPreList;
		std::map<std::thread::id, TaskImplementationPtr> mExecuteList;
		std::deque<TaskImplementationPtr> mPauseList;
		std::deque<TaskImplementationPtr> mPostList;
		std::deque<TaskImplementationPtr> mBufferList;
		bool mExitFlag;
	private:
		void WorkerFunction() throw();
	public:
		ThreadPool(Allocator&, const uint32_t) throw();
		SOLAIRE_EXPORT_CALL ~ThreadPool() throw();

		// Inherited from TaskExecutorI
		
		bool SOLAIRE_EXPORT_CALL Schedule(TaskI&) throw() override;
		bool SOLAIRE_EXPORT_CALL Update() throw() override;
	};

	extern "C" {
		SOLAIRE_EXPORT_API TaskExecutorI* SOLAIRE_EXPORT_CALL CreateThreadPool(Allocator& aAllocator, const uint32_t aThreads) throw() {
			return new(aAllocator.Allocate(sizeof(ThreadPool))) ThreadPool(aAllocator, aThreads);
		}
	}

	// TaskImplementation

	TaskImplementation::TaskImplementation(ThreadPool& aPool, TaskI& aTask) throw() :
		Executor(aPool),
		Task(aTask)
	{}

	SOLAIRE_EXPORT_CALL TaskImplementation::~TaskImplementation() throw() {

	}

	bool TaskImplementation::Initialise() throw() {
		return Task.InitialiseI(*this);
	}

	bool TaskImplementation::PreExecute() throw() {
		return Task.OnPreExecuteI();
	}

	bool TaskImplementation::Execute() throw() {
		return Task.OnExecuteI();
	}

	bool TaskImplementation::Resume() throw() {
		return Task.OnResumeI();
	}

	bool TaskImplementation::PostExecute() throw() {
		const bool result = Task.OnPostExecuteI();
		WaitCondition.notify_all();
		return result;
	}

	bool SOLAIRE_EXPORT_CALL TaskImplementation::Cancel() throw() {
		const bool result = Task.OnCancelI();
		WaitCondition.notify_all();
		return result;
	}

	bool SOLAIRE_EXPORT_CALL TaskImplementation::Wait() throw() {
		std::unique_lock<std::mutex> lock(Lock);
		WaitCondition.wait(lock);
		return true;
	}         

	bool SOLAIRE_EXPORT_CALL TaskImplementation::WaitFor(const uint32_t aMilliseconds) throw() {
		std::unique_lock<std::mutex> lock(Lock);
		return WaitCondition.wait_for(lock, std::chrono::milliseconds(aMilliseconds)) == std::cv_status::no_timeout;
	}

	// ThreadPool

	ThreadPool::ThreadPool(Allocator& aAllocator, const uint32_t aThreads) throw() :
		mAllocator(aAllocator),
		mExitFlag(false)
	{
		for(uint32_t i = 0; i < aThreads; ++i) {
			mWorkers.push_back(std::thread(&ThreadPool::WorkerFunction, this));
			mExecuteList.emplace(mWorkers.back().get_id(), TaskImplementationPtr());
		}
	}

	SOLAIRE_EXPORT_CALL ThreadPool::~ThreadPool() throw() {
		mExitFlag = true;
		{
			std::lock_guard<std::mutex> lock(mLock);
			for(TaskImplementationPtr i : mPreList) i->Cancel();
			for(TaskImplementationPtr i : mPostList) i->Cancel();
			for(TaskImplementationPtr i : mPauseList) i->Cancel();
			mInitialiseList.clear();
			mPreList.clear();
			mPostList.clear();
			mPauseList.clear();
		}
		mPreCondition.notify_all();
		for(std::thread& i : mWorkers) i.join();
	}

	bool SOLAIRE_EXPORT_CALL ThreadPool::Schedule(TaskI& aTask) throw() {
		const TaskImplementationPtr task(
			new(mAllocator.Allocate(sizeof(TaskImplementation))) TaskImplementation(*this, aTask),
			[&](TaskImplementation* aTask) {
				aTask->~TaskImplementation();
				mAllocator.Deallocate(aTask);
			}
		);

		const TaskI::State state = task->Task.GetState();
		if(state == TaskI::STATE_CANCELED || state == TaskI::STATE_COMPLETE) {
			if (!task->Initialise()) return false;
		}

		if(task->Task.GetState() != TaskI::STATE_INITIALISED) return false;

		std::lock_guard<std::mutex> lock(mLock);
		mInitialiseList.push_back(task);
		return true;
	}

	bool SOLAIRE_EXPORT_CALL ThreadPool::Update() throw() {
		{
			std::lock_guard<std::mutex> lock(mLock);
			std::swap(mBufferList, mInitialiseList);
		}

		for(TaskImplementationPtr i : mBufferList) i->PreExecute();

		uint32_t preCount = 0;
		{
			std::lock_guard<std::mutex> lock(mLock);
			preCount = mPreList.size();
			for(TaskImplementationPtr i : mBufferList) if(i->Task.GetState() == TaskI::STATE_PRE_EXECUTE) mPreList.push_back(i);
			preCount = mPreList.size() - preCount;
			mBufferList.clear();
			mBufferList.swap(mPostList);
		}

		if(preCount >= mWorkers.size()) {
			mPreCondition.notify_all();
		}else {
			for(uint32_t i = 0; i < preCount; ++i) mPreCondition.notify_one();
		}

		for(TaskImplementationPtr i : mBufferList) i->PostExecute();
		mBufferList.clear();

		return true;
	}

	void ThreadPool::WorkerFunction() throw() {
		while(! mExitFlag) {
			TaskImplementationPtr& task = mExecuteList[std::this_thread::get_id()];
			{
				std::lock_guard<std::mutex> lock(mLock);
				if(! mPauseList.empty()) {
					task.swap(mPauseList.front());
					mPauseList.pop_front();
				}else if(! mPreList.empty()) {
					task.swap(mPreList.front());
					mPreList.pop_front();
				}                
			}  

			if(task) {
				bool result = false;
				switch (task->Task.GetState()) {
				case TaskI::STATE_PAUSED:
					result = task->Resume();
					break;
				case TaskI::STATE_EXECUTE:
					result = task->Execute();
					break;
				default:
					task.swap(TaskImplementationPtr());
					continue;
				}

				if(! result) {
					task.swap(TaskImplementationPtr());
					continue;
				}

				if(task) {
					switch (task->Task.GetState()) {
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
				
					task.swap(TaskImplementationPtr());
					continue;
				}
			}else {
				std::unique_lock<std::mutex> lock(mLock);
				mPreCondition.wait(lock);
			}
		}
	}

}
    