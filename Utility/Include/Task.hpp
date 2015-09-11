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

        virtual void Update() = 0;
    };

    class AsyncManager;

    class Task{
    private:
        friend AsyncManager;

        std::atomic_bool mCanceled;
        TaskManager* mManager;

        Task(const Task&) = delete;
        Task(Task&&) = delete;
        Task& operator=(const Task&) = delete;
        Task& operator=(Task&&) = delete;
    protected:
        virtual void OnRecieveProgress(void* aProgress) = 0;

        void SendProgress(void* aProgress){
            mManager->SendProgress(this, aProgress);
        }

        void Schedule(){
            mManager->Schedule(this);
        }

        bool HasBeenCancled() const{
            return mCanceled;
        }

        virtual void PreExecute() = 0;
        virtual void Execute() = 0;
        virtual void PostExecute() = 0;
    public:
        Task() :
            mCanceled(false),
            mManager(nullptr)
        {

        }

        virtual ~Task(){

        }

        void Cancel(){
            mCanceled = true;
        }

        virtual bool IsRunning() const = 0;
    };

    template<typename RESULT, typename PROGRESS>
    class ReturnTask : public Task{
    public:
        static_assert(std::is_default_constructible<RESULT>::value, "Task.result_t must have a default constructor");
        static_assert(std::is_copy_assignable<RESULT>::value, "Task.result_t must have a copy assignment");
        static_assert(std::is_copy_constructible<PROGRESS>::value, "Task.progress_t must have a copy constructor");

        typedef RESULT result_t;
        typedef PROGRESS progress_t;
    private:
        std::atomic_bool mRunning;
        result_t mResult;
    protected:
        virtual result_t ExecuteForResult() = 0;
        virtual void OnRecieveProgress(progress_t& aProgress) = 0;


        void SendProgress(progress_t&& aProgress){
            progress_t* tmp = new progress_t(std::move(aProgress));
            Task::SendProgress(tmp);
        }

        // Inherited from Task

        void Execute() override{
            mRunning = true;
            mResult = ExecuteForResult();
            mRunning = false;
        }

        void OnRecieveProgress(void* aProgress) override{
            progress_t* ptr = static_cast<progress_t*>(aProgress);
            OnRecieveProgress(*ptr);
            delete ptr;
        }
    public:
        ReturnTask() :
            mRunning(false)
        {

        }

        virtual ~ReturnTask(){

        }

        result_t Get(){
            // wait
            //! \TODO Implement Task.Wait
            throw std::runtime_error("Not implemented");
        }

        // Inherited from Task

        bool IsRunning() const override{
            return mRunning;
        }
    };

}}


#endif
