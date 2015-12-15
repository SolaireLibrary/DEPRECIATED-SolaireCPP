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
		uint64_t mPauseTime;
		uint64_t mPauseDuration;
		TaskCallbacks* mCallbacks;
		State mState;
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
			switch (mState) {
			case STATE_INITIALISED:
				mCallbacks = &aCallbacks;
				return true;
			case STATE_CANCELED:
			case STATE_COMPLETE:
				mCallbacks = &aCallbacks;
				mState = STATE_INITIALISED;
				mPauseTime = 0;
				mPauseDuration = 0;
				return Initialise();
			default:
				return false;
			}
		}
		
		bool SOLAIRE_EXPORT_CALL OnPreExecuteI() throw() override {
			if(mState != STATE_INITIALISED) return false;
			mState = STATE_PRE_EXECUTE;
			return OnPreExecute();
		}

		bool SOLAIRE_EXPORT_CALL OnExecuteI() throw() override {
			if(mState != STATE_PRE_EXECUTE) return false;
			mState = STATE_EXECUTE;
			const bool result = OnExecute();
			if(mState == STATE_EXECUTE) mState = STATE_POST_EXECUTE;
			return result;
		}

		bool SOLAIRE_EXPORT_CALL OnPostExecuteI() throw() override {
			if(mState != STATE_POST_EXECUTE) return false;
			const bool result = OnPostExecute();
			mState = STATE_COMPLETE;
			return result;
		}

		bool SOLAIRE_EXPORT_CALL Pause(const uint64_t aMilliseconds) throw() override {
			if(mState != STATE_EXECUTE) return false;
			const bool result = OnPause();
			mState = STATE_PAUSED;
			mPauseTime = GetTimeMilliseconds();
			mPauseDuration = aMilliseconds;
			return result;
		}

		uint64_t SOLAIRE_EXPORT_CALL GetPauseTime() const throw() override {
			return mPauseTime;
		}

		uint64_t SOLAIRE_EXPORT_CALL GetPauseDuration() const throw()  override {
			return mPauseDuration;
		}
		
		void SOLAIRE_EXPORT_CALL SetPauseDuration(const uint64_t aTime) throw() {
			mPauseDuration = aTime;
		}


		bool SOLAIRE_EXPORT_CALL OnResumeI() throw() override {
			if(mState != STATE_PAUSED) return false;
			mState = STATE_EXECUTE;
			return OnResume();
		}

		bool SOLAIRE_EXPORT_CALL OnCancelI() throw() override {
			if(mState != STATE_CANCELED || mState != STATE_COMPLETE || mState == STATE_INITIALISED) return false;
			const bool result = OnCancel();
			mState = STATE_CANCELED;
			return result;
		}

	public:
		Task() throw() :
			mPauseTime(0),
			mPauseDuration(0),
			mCallbacks(nullptr),
			mState(STATE_INITIALISED)
		{}

		virtual SOLAIRE_EXPORT_CALL ~Task() throw() {

		}

		// Inherited from TaskI

		bool SOLAIRE_EXPORT_CALL Cancel() throw() override {
			return mCallbacks == nullptr ? false : mCallbacks->Cancel();
		}

		bool SOLAIRE_EXPORT_CALL Wait() const throw() override {
			return mCallbacks == nullptr ? 
				mState == STATE_COMPLETE || mState == STATE_CANCELED : 
				mCallbacks->Wait();
		}

		bool SOLAIRE_EXPORT_CALL WaitFor(const uint32_t aMilliseconds) const throw() override {
			return mCallbacks == nullptr ? 
				mState == STATE_COMPLETE || mState == STATE_CANCELED : 
				mCallbacks->WaitFor(aMilliseconds);
		}

		State SOLAIRE_EXPORT_CALL GetState() const throw() override {
			return mState;
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
