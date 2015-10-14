#ifndef SOLAIRE_TASK_INL
#define SOLAIRE_TASK_INL

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
	\file Task.inl
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 2.0
	\date
	Created			: 30th September 2015
	Last Modified	: 30th September 2015
*/

#include "Task.hpp"
#include "TaskManager.hpp"

namespace Solaire{

    class TaskImplementationDetails
    {
    private:
        Task& mTask;
    public:
        TaskImplementationDetails(Task& aTask):
            mTask(aTask)
        {}

        typedef bool(TaskImplementationDetails:: *StateCondition)() const;
        typedef void(TaskImplementationDetails:: *StateAction)();

        bool StateTransition(const StateCondition aCondition, const StateAction aAction, const Task::State aTransition, const std::string& aName){
            if((this->*aCondition)()){
                try{
                    (this->*aAction)();
                    mTask.mState = aTransition;
                    return true;
                }catch(...){
                    mTask.mException = std::current_exception();
                    mTask.Cancel();
                }
            }else{
                mTask.mException = std::make_exception_ptr(std::runtime_error("Utility::Task : Can only enter " + aName));
            }
            return false;
        }

        bool PreExecuteCondition() const{
            return mTask.mState == Task::STATE_SCHEDULED && ! mTask.CaughtException();
        }

        bool ExecuteCondition() const{
            return mTask.mState == Task::STATE_PRE_EXECUTION && ! mTask.CaughtException();
        }

        bool PostExecuteCondition() const{
            return mTask.mState == Task::STATE_EXECUTION && ! mTask.CaughtException();
        }

        bool PauseCondition() const{
            return mTask.mState == Task::STATE_EXECUTION && ! mTask.CaughtException();
        }

        bool ResumeCondition() const{
            return mTask.mState == Task::STATE_PAUSED && ! mTask.CaughtException();
        }

        bool CancelCondition() const{
            return mTask.mState != Task::STATE_COMPLETE && mTask.mState != Task::STATE_CANCELED && ! mTask.CaughtException();
        }

        bool ResetCondition() const{
            return mTask.mState == Task::STATE_CANCELED || mTask.mState == Task::STATE_COMPLETE;
        }

        void OnPreExecuteInternal(){
            mTask.OnPreExecute();
        }

        void OnExecuteInternal(){
            mTask.OnExecute(mTask.mPauseLocation);
            mTask.mPauseLocation = 0;
        }

        void OnPostExecuteInternal(){
            mTask.OnPostExecute();
            mTask.OnEndState();
        }

        void OnPauseInternal(){
            mTask.OnPause();
        }

        void OnResumeInternal(){
            mTask.OnResume(mTask.mPauseLocation);
        }

        void OnResetInternal(){
            mTask.mPauseLocation = 0;
            mTask.mException = nullptr;
            mTask.mLock.unlock();
            mTask.OnReset();
        }

        void OnCancelInternal(){
            mTask.OnCanceled();
        }
    };

    // Task

    void Task::Schedule(TaskManager& aManager){
        mLock.lock();
        mManager = &aManager;
    }

    void Task::OnEndState(){
        mLock.unlock();
        mManager = nullptr;
        for(void* i : mProgressList){
            DestroyProgress(i);
        }
    }

    bool Task::PreExecute(){
        return TaskImplementationDetails(*this).StateTransition(
            &TaskImplementationDetails::PreExecuteCondition,
            &TaskImplementationDetails::OnPreExecuteInternal,
            STATE_PRE_EXECUTION,
            "pre-execution"
        );
    }

    bool Task::Execute(){
        return TaskImplementationDetails(*this).StateTransition(
            &TaskImplementationDetails::ExecuteCondition,
            &TaskImplementationDetails::OnExecuteInternal,
            STATE_EXECUTION,
            "execution"
        );
    }

    bool Task::PostExecute(){
        return TaskImplementationDetails(*this).StateTransition(
            &TaskImplementationDetails::PostExecuteCondition,
            &TaskImplementationDetails::OnPostExecuteInternal,
            STATE_POST_EXECUTION,
            "post-execution"
        );
    }

    bool Task::Resume(){
        return TaskImplementationDetails(*this).StateTransition(
            &TaskImplementationDetails::ResumeCondition,
            &TaskImplementationDetails::OnResumeInternal,
            STATE_PRE_EXECUTION,
            "resumed"
        );
    }

    void Task::HandleProgress(){
        for(void* i : mProgressList){
            OnProgress(i);
            DestroyProgress(i);
        }
        mProgressList.Clear();
    }

    void Task::SendProgress(void* const aProgress){
        Task& lock = *this;
        solaire_synchronized(lock,
            mProgressList.PushBack(aProgress);
        )
    }

    void Task::Task::lock(){
        GetManager().mLock.lock();
    }

    void Task::unlock(){
        GetManager().mLock.unlock();
    }

    bool Task::try_lock(){
        return GetManager().mLock.try_lock();
    }

    bool Task::Pause(const uint16_t aLocation){
        mPauseLocation = aLocation;
        return TaskImplementationDetails(*this).StateTransition(
            &TaskImplementationDetails::PauseCondition,
            &TaskImplementationDetails::OnPauseInternal,
            STATE_PAUSED,
            "paused"
        );
    }

    Task::Task(Allocator& aAllocator) :
        mProgressList(aAllocator, 8),
        mManager(nullptr),
        mPauseLocation(0)
    {}

    Task::~Task(){
        for(void* i : mProgressList){
            //! \TODO Uncomment lines
            //OnProgress(i);
            //DestroyProgress(i);
        }
    }

    Task::State Task::GetState() const{
        return mState;
    }

    bool Task::Cancel(){
        return TaskImplementationDetails(*this).StateTransition(
            &TaskImplementationDetails::CancelCondition,
            &TaskImplementationDetails::OnCancelInternal,
            STATE_CANCELED,
            "canceled"
        );
    }

    bool Task::Reset(){
        return TaskImplementationDetails(*this).StateTransition(
            &TaskImplementationDetails::ResetCondition,
            &TaskImplementationDetails::OnResetInternal,
            STATE_INITIALISED,
            "reset"
        );
    }

    bool Task::CaughtException() const{
        return mException != nullptr;
    }

    void Task::RethrowException(){
        std::rethrow_exception(mException);
    }

    TaskManager& Task::GetManager(){
        if(mManager == nullptr) throw std::runtime_error("This task is not currently registered with a TaskManager");
        return *mManager;
    }

    const TaskManager& Task::GetManager() const{
        if(mManager == nullptr) throw std::runtime_error("This task is not currently registered with a TaskManager");
        return *mManager;
    }

    void Task::Wait() const{
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

    bool Task::TryWait() const{
        switch(mState){
        case STATE_CANCELED:
        case STATE_COMPLETE:
        case STATE_INITIALISED:
            return true;
        default:
            if(mLock.try_lock()){
                mLock.unlock();
                return true;
            }else{
                return false;
            }
        }
    }

    /*template<class T>
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
    };*/
}


#endif
