#ifndef SOLAIRE_UTILITY_TASK_HPP
#define SOLAIRE_UTILITY_TASK_HPP

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
	\file Task.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 11th September 2015
	Last Modified	: 11th September 2015
*/

#include <type_traits>
#include <atomic>
#include <functional>

namespace Solaire{ namespace Utility{

    class Task;

    class TaskManager{
    protected:
        friend Task;

        virtual void SendProgress(Task* aTask, void* aProgress) = 0;
        virtual void Schedule(Task* aTask) = 0;
    public:
        virtual ~TaskManager(){

        }

		virtual size_t GetNumberOfTasksQueued() const = 0;
		virtual bool CanUpdate() const = 0;
        virtual void Update() = 0;
		virtual size_t ForEachTask(std::function<void(Task&)> aFunction) = 0;
		virtual size_t ForEachTask(std::function<void(const Task&)> aFunction) const = 0;

		void WaitUntilEmpty(const size_t aSleepTimeMilli = 30){
			while(GetNumberOfTasksQueued() > 0){
				if(CanUpdate()){
					Update();
				}else{
				    #ifndef SOLAIRE_DISABLE_MULTITHREADING
                        std::this_thread::sleep_for(std::chrono::milliseconds(aSleepTimeMilli));
					#endif
				}
			}
		}
    };

    class AsyncManager;

    class Task{
	public:
		enum State : uint8_t {
			NOT_SCHEDULED,
			SCHEDULED,
			PRE_EXECUTION,
			EXECUTING,
			POST_EXECUTION,
			CANCELED,
			EXECUTED
		};
    private:
        friend AsyncManager;

        TaskManager* mManager;
        #ifndef SOLAIRE_DISABLE_MULTITHREADING
            std::atomic_uint8_t mState;
            std::atomic_bool mCanceled;
		#else
            uint8_t mState;
            bool mCanceled;
		#endif

        Task(const Task&) = delete;
        Task(Task&&) = delete;
        Task& operator=(const Task&) = delete;
        Task& operator=(Task&&) = delete;
    protected:
        virtual void OnRecieveProgress(void* aProgress) = 0;

        void SendProgress(void* aProgress){
            mManager->SendProgress(this, aProgress);
        }

		virtual void OnScheduled(const State aPreviousState) = 0;
		virtual void OnPreExecute() = 0;
		virtual void OnExecute() = 0;
		virtual void OnPostExecute(const bool aWasCanceled) = 0;
		virtual void OnCanceled() = 0;
    public:
		Task() :
			mCanceled(false),
            mManager(nullptr),
			mState(NOT_SCHEDULED)
        {

        }

        virtual ~Task(){

        }

		State GetState() const {
			return static_cast<State>(mState);
		}

		void Schedule(TaskManager& aManager){
			mManager = &aManager;
			mManager->Schedule(this);
		}

        void Cancel(){
            mCanceled = true;
			OnCanceled();
        }

		void Wait() {
			//! \TODO Implement better Task.Wait

			auto IsDone = [&](){
				const State state = GetState();
				return state == CANCELED || state == EXECUTED;
			};
            #ifndef SOLAIRE_DISABLE_MULTITHREADING
                while (!IsDone()) std::this_thread::sleep_for(std::chrono::milliseconds(30));
            #endif
		}

		bool WaitFor(const size_t aMilliseconds) {
			//! \TODO Implement better Task.WaitFor

			auto IsDone = [&]() {
				const State state = GetState();
				return state == CANCELED || state == EXECUTED;
			};

            #ifndef SOLAIRE_DISABLE_MULTITHREADING
                if(IsDone()) return true;
                std::this_thread::sleep_for(std::chrono::milliseconds(aMilliseconds));
            #endif
			return IsDone();
		}

		bool WaitUntil(const size_t aTimeMilliseconds) {
			//! \TODO Implement Task.WaitUntil
			throw std::runtime_error("Not implemented");
			// Return WaitFor(aTimeMilliseconds - currentTimeMilliseconds);
		}
    };

	template<typename PROGRESS>
	class TypedProgressTask : public Task {
	public:
		static_assert(std::is_move_constructible<PROGRESS>::value, "TypedProgressTask::progress_t must have a move constructor");

		typedef PROGRESS progress_t;
	protected:
		virtual void OnRecieveProgress(progress_t& aProgress) = 0;

		void SendProgress(progress_t&& aProgress) {
			progress_t* tmp = new progress_t(std::move(aProgress));
			Task::SendProgress(tmp);
		}

		// Inherited from Task

		void OnRecieveProgress(void* aProgress) override {
			progress_t* ptr = static_cast<progress_t*>(aProgress);
			OnRecieveProgress(*ptr);
			delete ptr;
		}
	public:
		virtual ~TypedProgressTask() {

		}
	};

    template<typename RESULT, typename PROGRESS>
    class ResultTask : public TypedProgressTask<PROGRESS>{
    public:
        static_assert(std::is_default_constructible<RESULT>::value, "ResultTask::result_t must have a default constructor");
        static_assert(std::is_copy_assignable<RESULT>::value, "ResultTask::result_t must have a copy assignment");

        typedef RESULT result_t;
    private:
        result_t mResult;
    protected:
		void SetResult(result_t aResult){
			mResult = aResult;
		}

    public:
        virtual ~ResultTask(){

        }

		result_t Get(){
			TypedProgressTask<PROGRESS>::Wait();
			return mResult;
		}
    };

}}


#endif
