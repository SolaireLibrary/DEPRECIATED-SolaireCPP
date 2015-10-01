#ifndef SOLAIRE_CORE_TASK_HPP
#define SOLAIRE_CORE_TASK_HPP

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
	\version 2.0
	\date
	Created			: 11th September 2015
	Last Modified	: 27th September 2015
*/

#include <map>
#ifndef SOLAIRE_DISABLE_MULTITHREADING
    #include <thread>
    #include <mutex>
#else
    #include "DummyLock.hpp"
#endif
#include "../Macros.hpp"
#include "../DataStructures/DynamicArray.hpp"

namespace Solaire{ namespace Core{

    class TaskManager;

    class Task{
    public:
        enum State : uint8_t {
			STATE_INITIALISED,
			STATE_SCHEDULED,
			STATE_PRE_EXECUTION,
			STATE_PAUSED,
			STATE_EXECUTION,
			STATE_POST_EXECUTION,
			STATE_CANCELED,
			STATE_COMPLETE
		};
        friend TaskManager;
    private:
        #ifdef SOLAIRE_DISABLE_MULTITHREADING
            typedef DummyLock Mutex;
        #else
            typedef std::mutex Mutex;
        #endif

        DynamicArray<void*> mProgressList;
        TaskManager* mManager;
        std::exception_ptr mException;
        mutable Mutex mLock;
        uint16_t mPauseLocation;
        State mState;

        typedef bool(Task:: *StateCondition)() const;
        typedef void(Task:: *StateAction)();

        bool StateTransition(const StateCondition aCondition, const StateAction aAction, const State aTransition, const std::string& aName);

        bool PreExecuteCondition() const;
        bool ExecuteCondition() const;
        bool PostExecuteCondition() const;
        bool PauseCondition() const;
        bool ResumeCondition() const;
        bool CancelCondition() const;
        bool ResetCondition() const;

        void Schedule(TaskManager& aManager);
        void HandleProgress();

        void OnPreExecuteInternal();
        void OnExecuteInternal();
        void OnPostExecuteInternal();
        void OnPauseInternal();
        void OnResumeInternal();
        void OnResetInternal();
        void OnCancelInternal();

        void OnEndState();
        bool PreExecute();
        bool Execute();
        bool PostExecute();
        bool Resume();

    protected:
        virtual void OnPreExecute() = 0;
        virtual void OnExecute(const uint16_t aLocation = 0) = 0;
        virtual void OnPostExecute() = 0;
        virtual void OnPause() = 0;
        virtual void OnResume(const uint16_t aLocation) = 0;
        virtual void OnCanceled() = 0;
        virtual void OnReset() = 0;

        virtual void DestroyProgress(void* const aProgress) = 0;
        virtual void OnProgress(void* const aProgress) = 0;

        void SendProgress(void* const aProgress);

        void lock();
        void unlock();
        bool try_lock();

        bool Pause(const uint16_t aLocation);
    public:
        Task();

        virtual ~Task();

        State GetState() const;
        TaskManager& GetManager();
        const TaskManager& GetManager() const;

        bool CaughtException() const;
        void RethrowException();

        bool Cancel();
        bool Reset();

        void Wait() const;
        bool TryWait() const;
    };

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

    template<class T>
    class ResultTask : public Task{
    private:
        T mResult;
    protected:
        virtual void OnPreExecute(T& aResult) = 0;
        virtual void OnExecute(T& aResult) = 0;
        virtual void OnPostExecute(T& aResult) = 0;
        virtual void OnCanceled(T& aResult) = 0;
        virtual void OnReset(T& aResult) = 0;
        virtual void OnPause(T& aResult) = 0;
        virtual void OnResume(const uint16_t aLocation, T& aResult) = 0;

        // Inherited from Task

        void OnReset() override{
            OnReset(mResult);
        }

        void OnPreExecute() override{
            OnPreExecute(mResult);
        }

        void OnExecute() override{
            OnExecute(mResult);
        }

        void OnPostExecute() override{
            OnPostExecute(mResult);
        }

        void OnCanceled() override{
            OnCanceled(mResult);
        }

        void OnPause() override{
            OnPause(mResult);
        }

        void OnResume(const uint16_t aLocation) override{
            OnResume(aLocation, mResult);
        }
    public:

        virtual ~ResultTask(){

        }

        const T& Get() const{
            if(CaughtException()) RethrowException();

            switch(State()){
            case STATE_INITIALISED:
                throw std::runtime_error("Cannot get result while in STATE_INITIALISED");
            case STATE_CANCELED:
                throw std::runtime_error("Cannot get result while in STATE_CANCELED");
            case STATE_COMPLETE:
                return mResult;
            default:
                Wait();
                return Get();
            }
        }
    };
}}

#include "Task.inl"


#endif
