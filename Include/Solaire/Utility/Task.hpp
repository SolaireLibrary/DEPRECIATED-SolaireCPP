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
	\version 2.0
	\date
	Created			: 11th September 2015
	Last Modified	: 24th September 2015
*/

#include <map>
#include <vector>
#ifndef SOLAIRE_DISABLE_MULTITHREADING
    #include <thread>
    #include <mutex>
#else
    #include "DummyLock.hpp"
#endif
#include "..\Core\Macros.hpp"

namespace Solaire{ namespace Utility{

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

        std::exception_ptr mException;
        mutable Mutex mLock;
        State mState;

        typedef bool(Task:: *StateCondition)() const;
        typedef void(Task:: *StateAction)();

        bool StateTransition(const StateCondition aCondition, const StateAction aAction, const State aTransition, const std::string& aName){
            if((this->*aCondition)()){
                try{
                    (this->*aAction)();
                    mState = aTransition;
                    return true;
                }catch(...){
                    mException = std::current_exception();
                    Cancel();
                }
            }else{
                mException = std::make_exception_ptr(std::runtime_error("Utility::Task : Can only enter " + aName + " while in STATE_SCHEDULED"));
            }
            return false;
        }

        bool PreExecuteCondition() const{
            return mState == STATE_SCHEDULED && ! CaughtException();
        }

        bool ExecuteCondition() const{
            return mState == STATE_PRE_EXECUTION && ! CaughtException();
        }

        bool PostExecuteCondition() const{
            return mState == STATE_EXECUTION && ! CaughtException();
        }

        bool PauseCondition() const{
            return mState == STATE_EXECUTION && ! CaughtException();
        }

        bool ResumeCondition() const{
            return mState == STATE_PAUSED && ! CaughtException();
        }

        bool CancelCondition() const{
            return mState != STATE_COMPLETE && mState != STATE_CANCELED && ! CaughtException();
        }

        bool ResetCondition() const{
            return mState == STATE_CANCELED || mState == STATE_COMPLETE;
        }

        void Schedule(){
            mLock.lock();
        }

        void OnPreExecuteInternal(){
            OnPreExecute();
        }

        void OnExecuteInternal(){
            OnExecute();
        }

        void OnPostExecuteInternal(){
            OnPostExecute();
            mLock.unlock();
        }

        void OnPauseInternal(){
            OnPause();
        }

        void OnResumeInternal(){
            OnResume();
        }

        void OnResetInternal(){
            mException = nullptr;
            mLock.unlock();
            OnReset();
        }

        void OnCancelInternal(){
            OnCanceled();
        }

        bool PreExecute(){
            return StateTransition(
                &Task::PreExecuteCondition,
                &Task::OnPreExecuteInternal,
                STATE_PRE_EXECUTION,
                "pre-execution"
            );
        }

        bool Execute(){
            return StateTransition(
                &Task::ExecuteCondition,
                &Task::OnExecuteInternal,
                STATE_EXECUTION,
                "execution"
            );
        }

        bool PostExecute(){
            return StateTransition(
                &Task::PostExecuteCondition,
                &Task::OnPostExecuteInternal,
                STATE_POST_EXECUTION,
                "post-execution"
            );
        }

    protected:
        virtual void OnPreExecute() = 0;
        virtual void OnExecute() = 0;
        virtual void OnPostExecute() = 0;
        virtual void OnPause() = 0;
        virtual void OnResume() = 0;
        virtual void OnCanceled() = 0;
        virtual void OnReset() = 0;

        bool Pause(){
            return StateTransition(
                &Task::PauseCondition,
                &Task::OnPauseInternal,
                STATE_PAUSED,
                "paused"
            );
        }

        bool Resume(){
            return StateTransition(
                &Task::ResumeCondition,
                &Task::OnResumeInternal,
                STATE_PRE_EXECUTION,
                "resumed"
            );
        }
    public:
        virtual ~Task(){

        }

        State State() const{
            return mState;
        }

        bool Cancel(){
            return StateTransition(
                &Task::CancelCondition,
                &Task::OnCancelInternal,
                STATE_CANCELED,
                "canceled"
            );
        }

        bool Reset(){
            return StateTransition(
                &Task::ResetCondition,
                &Task::OnResetInternal,
                STATE_INITIALISED,
                "reset"
            );
        }

        bool CaughtException() const{
            return mException != nullptr;
        }

        void RethrowException(){
            std::rethrow_exception(mException);
        }

        void Wait() const{
            switch(mState){
            case STATE_CANCELED:
            case STATE_COMPLETE:
            case STATE_INITIALISED:
                break;
            default:
                mLock.lock();
                mLock.unlock();
                break;
            }
        }

        bool TryWait() const{
            switch(mState){
            case STATE_CANCELED:
            case STATE_COMPLETE:
            case STATE_INITIALISED:
                break;
            default:
                if(mLock.try_lock()){
                    mLock.unlock();
                    return true;
                }else{
                    return false;
                }
            }
        }

        //! \TODO post progress
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

        std::vector<Task*> mInitialiseTasks;
        std::vector<Task*> mPreExecuteTasks;
        std::vector<Task*> mPostExecuteTasks;
        std::map<ThreadID, Task*> mExecutionTasks;

        ThreadID GetThreadID() const{
            #ifdef SOLAIRE_DISABLE_MULTITHREADING
                return 0;
            #else
                return std::this_thread::get_id();
            #endif
        }
    protected:
        Mutex mLock;

        bool Execute(){
            Task* task = nullptr;

            solaire_synchronized(mLock,
                if(! mPreExecuteTasks.empty()){
                    task = mPreExecuteTasks.back();
                    mPreExecuteTasks.pop_back();
                    mExecutionTasks.emplace(GetThreadID(), task);
                }
            )

            bool success = task != nullptr;

            if(success){
                if(task->State() == Task::STATE_PAUSED){
                    success = success && task->Resume();
                }

                if(success){
                    success = success && task->Execute();
                }

                solaire_synchronized(mLock,
                    if(success){
                        if(task->State() == Task::STATE_PAUSED){
                            mPreExecuteTasks.push_back(task);
                        }else{
                            mPostExecuteTasks.push_back(task);
                        }
                    }
                    mExecutionTasks.emplace(GetThreadID(), nullptr);
                )
            }
            return success;
        }
    public:

        virtual ~TaskManager(){
            solaire_synchronized(mLock,
                for(Task* task : mInitialiseTasks){
                    task->Cancel();
                }
                mInitialiseTasks.clear();

                for(Task* task : mPreExecuteTasks){
                    task->Cancel();
                }
                mPreExecuteTasks.clear();

                for(Task* task : mPostExecuteTasks){
                    task->Cancel();
                }
                mPostExecuteTasks.clear();
            )
        }

        virtual bool Schedule(Task& aTask){
            if(aTask.State() != Task::STATE_INITIALISED) return false;
            aTask.mState = Task::STATE_SCHEDULED;

            solaire_synchronized(mLock,
                mInitialiseTasks.push_back(&aTask);
            )
            return true;
        }

        virtual void Update(){
            solaire_synchronized(mLock,
                for(Task* task : mInitialiseTasks){
                    task->Schedule();
                    if(task->PreExecute()){
                        mPreExecuteTasks.push_back(task);
                    }
                }
                mInitialiseTasks.clear();

                for(Task* task : mPostExecuteTasks){
                    if(task->PostExecuteCondition()) task->PostExecute();
                }
                mInitialiseTasks.clear();
            )
        }

        virtual size_t ThreadCount() const = 0;

        size_t TaskCount() const{
            size_t count = 0;
            solaire_synchronized(mLock,
                for(const std::pair<ThreadID, Task*>& i : mExecutionTasks){
                    if(i.second != nullptr) ++count;
                }
                count += mInitialiseTasks.size();
                count += mPreExecuteTasks.size();
                count += mPostExecuteTasks.size();
            )
            return count;
        }

        virtual void Wait(const size_t aSleepTime = 33){
            while(TaskCount() > 0){
                Update();
                #ifndef SOLAIRE_DISABLE_MULTITHREADING
                    std::this_thread::sleep_for(std::chrono::milliseconds(aSleepTime));
                #endif
            }
        }
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


#endif
