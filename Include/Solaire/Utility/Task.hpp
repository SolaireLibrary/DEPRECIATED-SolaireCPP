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
        std::exception_ptr mException;
        State mState;

        typedef bool(Task:: *StateCondition)() const;
        typedef void(Task:: *StateAction)();

        bool StateTransition(const StateCondition aCondition, const StateAction aAction, const State aTransition, const std::string& aName){
            if((this->*aCondition)()){
                try{
                    (this->*aAction)();
                    mState = aTransition;
                    return true;
                }catch(std::exception& e){
                    mException = std::make_exception_ptr(e);
                    Cancel();
                }catch(...){
                    mException = std::make_exception_ptr(std::runtime_error("Utility::Task : A non-exception was thrown during " + aName));
                    Cancel();
                }
            }else{
                mException = std::make_exception_ptr(std::runtime_error("Utility::Task : Can only enter " + aName + " while in STATE_SCHEDULED"));
            }
            return false;
        }

        bool PreExecuteCondition() const{
            return mState == STATE_SCHEDULED;
        }

        bool ExecuteCondition() const{
            return mState == STATE_PRE_EXECUTION;
        }

        bool PostExecuteCondition() const{
            return mState == STATE_EXECUTION;
        }

        bool PauseCondition() const{
            return mState == STATE_EXECUTION;
        }

        bool ResumeCondition() const{
            return mState == STATE_PAUSED;
        }

        bool CancelCondition() const{
            return mState != STATE_CANCELED;
        }

        bool ResetCondition() const{
            return mState == STATE_CANCELED || mState == STATE_COMPLETE;
        }

        bool PreExecute(){
            return StateTransition(
                &Task::PreExecuteCondition,
                &Task::OnPreExecute,
                STATE_PRE_EXECUTION,
                "pre-execution"
            );
        }

        bool Execute(){
            return StateTransition(
                &Task::ExecuteCondition,
                &Task::OnExecute,
                STATE_EXECUTION,
                "execution"
            );
        }

        bool PostExecute(){
            return StateTransition(
                &Task::PostExecuteCondition,
                &Task::OnPostExecute,
                STATE_POST_EXECUTION,
                "post-execution"
            );
        }

    protected:
        virtual void OnPreExecute() = 0;
        virtual void OnExecute() = 0;
        virtual void OnPostExecute() = 0;
        virtual void OnPaused() = 0;
        virtual void OnResume() = 0;
        virtual void OnCanceled() = 0;
        virtual void OnReset() = 0;

        bool Pause(){
            return StateTransition(
                &Task::PauseCondition,
                &Task::OnPaused,
                STATE_PAUSED,
                "paused"
            );
        }

        bool Resume(){
            return StateTransition(
                &Task::ResumeCondition,
                &Task::OnResume,
                STATE_PRE_EXECUTION,
                "resumed"
            );
        }
    public:
        State State() const{
            return mState;
        }

        bool Cancel(){
            return StateTransition(
                &Task::CancelCondition,
                &Task::OnCanceled,
                STATE_CANCELED,
                "canceled"
            );
        }

        bool Reset(){
            return StateTransition(
                &Task::ResetCondition,
                &Task::OnReset,
                STATE_INITIALISED,
                "reset"
            );
        }

        //! \TODO Get exception
        //! \TODO Return task
        //! \TODO Wait
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
            typedef std::thread::thread_id ThreadID;
            typedef std::mutex Mutex;
        #endif

        std::vector<Task*> mInitialiseTasks;
        std::vector<Task*> mPreExecuteTasks;
        std::vector<Task*> mPostExecuteTasks;
        std::map<ThreadID, Task*> mExecutionTasks;
        Mutex mLock;

        ThreadID GetThreadID() const{
            #ifdef SOLAIRE_DISABLE_MULTITHREADING
                return 0;
            #else
                return std::this_thread::get_id();
            #endif
        }
    protected:
        void RegisterThread(const ThreadID aID){
            solaire_synchronized(mLock,
                mExecutionTasks.emplace(aID, nullptr);
            )
        }

        bool Execute(){
            Task* task = nullptr;

            solaire_synchronized(mLock,
                if(! mPreExecuteTasks.empty()){
                    task = mPreExecuteTasks.back();
                    mPreExecuteTasks.pop_back();
                    mExecutionTasks.emplace(GetThreadID(), task);
                }
            )

            if(task != nullptr){
                bool success = true;
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
    };
}}


#endif
