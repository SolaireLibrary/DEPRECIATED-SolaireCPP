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

#include "Task I.hpp"
#include "TaskCallbacks.hpp"

namespace Solaire {

	class Task : public TaskI {
	private :
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
		
		bool SOLAIRE_EXPORT_CALL InitialiseI(TaskCallbacks& aCallbacks) throw() {
			if(mCallbacks != nullptr) return false;
			if(mState != STATE_CANCELED && mState != STATE_COMPLETE) return true;
			mCallbacks = aCallbacks;
			mState = STATE_INITIALISED;
			return Initialise();
		}
		
		bool SOLAIRE_EXPORT_CALL OnPreExecuteI() throw() {
			if(mState != STATE_INITIALISED) return false;
			mState = STATE_PRE_EXECUTE;
			return OnPreExecute();
		}

		bool SOLAIRE_EXPORT_CALL OnExecuteI() throw() {
			if(mState != STATE_PRE_EXECUTE) return false;
			mState = STATE_EXECUTE;
			return OnExecute();
		}

		bool SOLAIRE_EXPORT_CALL OnPostExecutei() throw() {
			if(mState != STATE_EXECUTE) return false;
			const bool result = OnPostExecute();
			mCallbacks = nullptr;
			mState = STATE_COMPLETE;
			return result;
		}

		bool SOLAIRE_EXPORT_CALL Pause() throw() {
			if(mState != STATE_EXECUTE) return false;
			const bool result = OnPause();
			mCallbacks = nullptr;
			mState = STATE_PAUSED;
			return result;
		}

		bool SOLAIRE_EXPORT_CALL OnResumeI() throw() {
			if(mState != STATE_PAUSED) return false;
			mState = STATE_EXECUTE;
			return OnResume();
		}

		bool SOLAIRE_EXPORT_CALL OnCancelI() throw() {
			if(mState != STATE_CANCELED || mState = STATE_COMPLETE || mState == STATE_INITIALISED) return false;
			const bool result = OnCancel();
			mCallbacks = nullptr;
			mState = STATE_CANCELED;
			return result;
		}

	public:
		Task() throw() :
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
}


#endif
