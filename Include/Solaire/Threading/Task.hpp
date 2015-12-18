#ifndef SOLAIRE_TASK_HPP
#define SOLAIRE_TASK_HPP

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
Created			: 6th December 2015
Last Modified	: 8th December 2015
*/

#include "TaskI.hpp"
#include "TaskCallbacks.hpp"
#include "..\Core\System.hpp"

namespace Solaire {

	class Task : public TaskI {
	private :
		TaskCallbacks* mCallbacks;
		Configuration mConfig;
	protected :
		virtual bool SOLAIRE_EXPORT_CALL Initialise() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnPreExecute() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnExecute() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnPostExecute() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnPause() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnResume() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnCancel() throw() = 0;

		// Inherited from TaskI
		
		void SOLAIRE_EXPORT_CALL RemoveCallbacks() throw() override{
			mCallbacks = nullptr;
		}
		
		bool SOLAIRE_EXPORT_CALL InitialiseI(TaskCallbacks& aCallbacks) throw() override {
			switch(mConfig.State) {
			case STATE_INITIALISED:
				mCallbacks = &aCallbacks;
				return true;
			case STATE_CANCELED:
			case STATE_COMPLETE:
				mCallbacks = &aCallbacks;
				mConfig.PauseTime = 0;
				mConfig.PauseDuration = 0;
				mConfig.State = STATE_INITIALISED;
				mConfig.Execution = EXECUTE_ON_WORKER;
				return Initialise();
			default:
				return false;
			}
		}
		
		bool SOLAIRE_EXPORT_CALL OnPreExecuteI() throw() override {
			if(mConfig.State != STATE_INITIALISED) return false;
			mConfig.State = STATE_PRE_EXECUTE;
			return OnPreExecute();
		}

		bool SOLAIRE_EXPORT_CALL OnExecuteI() throw() override {
			if(mConfig.State != STATE_PRE_EXECUTE) return false;
			mConfig.State = STATE_EXECUTE;
			const bool result = OnExecute();
			if(mConfig.State == STATE_EXECUTE) mConfig.State = STATE_POST_EXECUTE;
			return result;
		}

		bool SOLAIRE_EXPORT_CALL OnPostExecuteI() throw() override {
			if(mConfig.State != STATE_POST_EXECUTE) return false;
			const bool result = OnPostExecute();
			mConfig.State = STATE_COMPLETE;
			return result;
		}

		bool SOLAIRE_EXPORT_CALL Pause(const uint64_t aMilliseconds) throw() override {
			if(mConfig.State != STATE_EXECUTE) return false;
			const bool result = OnPause();
			mConfig.State = STATE_PAUSED;
			mConfig.PauseTime = GetTimeMilliseconds();
			mConfig.PauseDuration = aMilliseconds;
			return result;
		}

		Configuration SOLAIRE_EXPORT_CALL GetConfiguration() const throw() override {
			return mConfig;
		}
		
		void SOLAIRE_EXPORT_CALL SetPauseDuration(const uint64_t aTime) throw() {
			mConfig.PauseDuration = aTime;
		}


		bool SOLAIRE_EXPORT_CALL OnResumeI() throw() override {
			if(mConfig.State != STATE_PAUSED) return false;
			mConfig.State = STATE_EXECUTE;
			return OnResume();
		}

		bool SOLAIRE_EXPORT_CALL OnCancelI() throw() override {
			if(mConfig.State != STATE_CANCELED || mConfig.State != STATE_COMPLETE || mConfig.State == STATE_INITIALISED) return false;
			const bool result = OnCancel();
			mConfig.State = STATE_CANCELED;
			return result;
		}

	public:
		Task() throw() :
			mCallbacks(nullptr)
		{

			mConfig.PauseTime = 0;
			mConfig.PauseDuration = 0;
			mConfig.State = STATE_INITIALISED;
			mConfig.Execution = EXECUTE_ON_WORKER;
		}

		virtual SOLAIRE_EXPORT_CALL ~Task() throw() {

		}

		// Inherited from TaskI

		bool SOLAIRE_EXPORT_CALL Cancel() throw() override {
			return mCallbacks == nullptr ? false : mCallbacks->Cancel();
		}

		bool SOLAIRE_EXPORT_CALL Wait() const throw() override {
			return mCallbacks == nullptr ? 
				mConfig.State == STATE_COMPLETE || mConfig.State == STATE_CANCELED :
				mCallbacks->Wait();
		}

		bool SOLAIRE_EXPORT_CALL WaitFor(const uint32_t aMilliseconds) const throw() override {
			return mCallbacks == nullptr ? 
				mConfig.State == STATE_COMPLETE || mConfig.State == STATE_CANCELED :
				mCallbacks->WaitFor(aMilliseconds);
		}
	};

	template<class T>
	class ReturnTask : public Task {
	private:
		SharedAllocation<T> mReturn;
	protected:
		inline void SetReturn(SharedAllocation<T> aValue) throw() {
			mReturn.Swap(aValue);
		}
	public:
		virtual SOLAIRE_EXPORT_CALL ~ReturnTask() throw() {

		}

		SharedAllocation<T> Get() const throw() {
			if(! Wait()) return SharedAllocation<T>();
			return mReturn;
		}

		SharedAllocation<T> GetFor(const uint32_t aMilliseconds) const throw() {
			if(! WaitFor(aMilliseconds)) return SharedAllocation<T>();
			return mReturn;
		}
	};
}


#endif
