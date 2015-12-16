#ifndef SOLAIRE_TASK_GROUP_HPP
#define SOLAIRE_TASK_GROUP_HPP

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
\file TaskGroup.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 16th December 2015
Last Modified	: 16th December 2015
*/

#include "TaskI.hpp"
#include "TaskCallbacks.hpp"
#include "..\Core\System.hpp"
#include "..\Memory\Allocator.hpp"
#include <vector>

namespace Solaire {

	enum : int {
		TASK_GROUP_ALL,
		TASK_GROUP_ONE,
		TASK_GROUP_ANY
	};

	template<const int INIT_MODE, const int PRE_MODE, const int EXE_MODE, const int POST_MODE>
	class TaskGroup : public TaskI {
	private :
		uint64_t mPauseTime;
		uint64_t mPauseDuration;
		TaskCallbacks* mCallbacks;
		std::vector<SharedAllocation<TaskI>> mTasks;
		std::vector<SharedAllocation<TaskI>> mLiveTasks;
		std::vector<SharedAllocation<TaskI>> mTaskBuffer;
		State mState;
	private:
		// GroupInitialise

		template<const int MODE>
		std::enable_if_t<MODE == TASK_GROUP_ALL, bool > SOLAIRE_EXPORT_CALL GroupInitialise(TaskCallbacks& aCallbacks) throw() {
			for(SharedAllocation<TaskI> i : mLiveTasks) {
				if(! i->InitialiseI(aCallbacks)) {
					return false;
				}
			}
			std::swap(mLiveTasks, mTaskBuffer);
			return true;
		}

		template<const int MODE>
		std::enable_if_t<MODE == TASK_GROUP_ONE, bool > SOLAIRE_EXPORT_CALL GroupInitialise(TaskCallbacks& aCallbacks) throw() {
			for(SharedAllocation<TaskI> i : mLiveTasks) {
				if(i->InitialiseI(aCallbacks)) {
					mTaskBuffer.push_back(i);
					return true;
				}
			}
			return false;
		}

		template<const int MODE>
		std::enable_if_t<MODE == TASK_GROUP_ANY, bool > SOLAIRE_EXPORT_CALL GroupInitialise(TaskCallbacks& aCallbacks) throw() {
			for(SharedAllocation<TaskI> i : mLiveTasks) {
				if(i->InitialiseI(aCallbacks)) {
					mTaskBuffer.push_back(i);
				}else {
					return false;
				}
			}
			return ! mTaskBuffer.empty();
		}

		// GroupPreExecute

		template<const int MODE>
		std::enable_if_t<MODE == TASK_GROUP_ALL, bool > SOLAIRE_EXPORT_CALL GroupPreExecute() throw() {
			for (SharedAllocation<TaskI> i : mLiveTasks) {
				if (! i->OnPreExecuteI()) {
					return false;
				}
			}
			std::swap(mLiveTasks, mTaskBuffer);
			return true;
		}

		template<const int MODE>
		std::enable_if_t<MODE == TASK_GROUP_ONE, bool > SOLAIRE_EXPORT_CALL GroupPreExecute() throw() {
			for (SharedAllocation<TaskI> i : mLiveTasks) {
				if (i->OnPreExecuteI()) {
					mTaskBuffer.push_back(i);
					return true;
				}
			}
			return false;
		}

		template<const int MODE>
		std::enable_if_t<MODE == TASK_GROUP_ANY, bool > SOLAIRE_EXPORT_CALL GroupPreExecute() throw() {
			for (SharedAllocation<TaskI> i : mLiveTasks) {
				if(i->OnPreExecuteI()) {
					mTaskBuffer.push_back(i);
				}
			}
			return ! mTaskBuffer.empty();
		}

		// GroupExecute

		template<const int MODE>
		std::enable_if_t<MODE == TASK_GROUP_ALL, bool > SOLAIRE_EXPORT_CALL GroupExecute() throw() {
			for(SharedAllocation<TaskI> i : mLiveTasks) {
				if(!i->OnExecuteI()) {
					return false;
				}
			}
			std::swap(mLiveTasks, mTaskBuffer);
			return true;
		}

		template<const int MODE>
		std::enable_if_t<MODE == TASK_GROUP_ONE, bool > SOLAIRE_EXPORT_CALL GroupExecute() throw() {
			for(SharedAllocation<TaskI> i : mLiveTasks) {
				if(i->OnExecuteI()) {
					mTaskBuffer.push_back(i);
					return true;
				}
			}
			return false;
		}

		template<const int MODE>
		std::enable_if_t<MODE == TASK_GROUP_ANY, bool > SOLAIRE_EXPORT_CALL GroupExecute() throw() {
			for(SharedAllocation<TaskI> i : mLiveTasks) {
				if(i->OnExecuteI()) {
					mTaskBuffer.push_back(i);
				}
			}
			return ! mTaskBuffer.empty();
		}

		// GroupPostExecute

		template<const int MODE>
		std::enable_if_t<MODE == TASK_GROUP_ALL, bool > SOLAIRE_EXPORT_CALL GroupPostExecute() throw() {
			for(SharedAllocation<TaskI> i : mLiveTasks) {
				if(!i->OnPostExecuteI()) {
					return false;
				}
			}
			std::swap(mLiveTasks, mTaskBuffer);
			return true;
		}

		template<const int MODE>
		std::enable_if_t<MODE == TASK_GROUP_ONE, bool > SOLAIRE_EXPORT_CALL GroupPostExecute() throw() {
			for(SharedAllocation<TaskI> i : mLiveTasks) {
				if(i->OnPostExecuteI()) {
					mTaskBuffer.push_back(i);
					return true;
				}
			}
			return false;
		}

		template<const int MODE>
		std::enable_if_t<MODE == TASK_GROUP_ANY, bool > SOLAIRE_EXPORT_CALL GroupPostExecute() throw() {
			for(SharedAllocation<TaskI> i : mLiveTasks) {
				if(i->OnPostExecuteI()) {
					mTaskBuffer.push_back(i);
				}
			}
			return ! mTaskBuffer.empty();
		}

		// GroupResume

		template<const int MODE>
		std::enable_if_t<MODE == TASK_GROUP_ALL, bool > SOLAIRE_EXPORT_CALL GroupResume() throw() {
			for (SharedAllocation<TaskI> i : mLiveTasks) {
				if (!i->OnResumeI()) {
					return false;
				}
			}
			std::swap(mLiveTasks, mTaskBuffer);
			return true;
		}

		template<const int MODE>
		std::enable_if_t<MODE == TASK_GROUP_ONE, bool > SOLAIRE_EXPORT_CALL GroupResume() throw() {
			for (SharedAllocation<TaskI> i : mLiveTasks) {
				if (i->OnResumeI()) {
					mTaskBuffer.push_back(i);
					return true;
				}
			}
			return false;
		}

		template<const int MODE>
		std::enable_if_t<MODE == TASK_GROUP_ANY, bool > SOLAIRE_EXPORT_CALL GroupResume() throw() {
			for (SharedAllocation<TaskI> i : mLiveTasks) {
				if (i->OnResumeI()) {
					mTaskBuffer.push_back(i);
				}
			}
			return !mTaskBuffer.empty();
		}
	protected :
		// Inherited from TaskI
		
		void SOLAIRE_EXPORT_CALL RemoveCallbacks() throw() override{
			mCallbacks = nullptr;
			for(SharedAllocation<TaskI> i : mTasks) i->RemoveCallbacks();
		}
		
		bool SOLAIRE_EXPORT_CALL InitialiseI(TaskCallbacks& aCallbacks) throw() override {
			mLiveTasks = mTasks;
			const bool result = GroupInitialise<INIT_MODE>(aCallbacks);
			mLiveTasks.swap(mTaskBuffer);
			mTaskBuffer.clear();
			mState = STATE_INITIALISED;
			return result;
		}
		
		bool SOLAIRE_EXPORT_CALL OnPreExecuteI() throw() override {
			if(mState != STATE_INITIALISED) return false;
			const bool result = GroupPreExecute<PRE_MODE>();
			mLiveTasks.swap(mTaskBuffer);
			mTaskBuffer.clear();
			mState = STATE_PRE_EXECUTE;
			return result;
		}

		bool SOLAIRE_EXPORT_CALL OnExecuteI() throw() override {
			if(mState != STATE_PRE_EXECUTE) return false;
			mState = STATE_EXECUTE;
			const bool result = GroupExecute<EXE_MODE>();
			mLiveTasks.swap(mTaskBuffer);
			mTaskBuffer.clear();
			if(mState == STATE_EXECUTE) mState = STATE_POST_EXECUTE;
			return result;
		}

		bool SOLAIRE_EXPORT_CALL OnPostExecuteI() throw() override {
			if(mState != STATE_POST_EXECUTE) return false;
			mState = STATE_EXECUTE;
			const bool result = GroupPostExecute<POST_MODE>();
			mLiveTasks.swap(mTaskBuffer);
			mTaskBuffer.clear();
			mState = STATE_COMPLETE;
			return result;
		}

		bool SOLAIRE_EXPORT_CALL Pause(const uint64_t aMilliseconds) throw() override {
			if(mState != STATE_EXECUTE) return false;
			for(SharedAllocation<TaskI> i : mLiveTasks) {
				if(i->GetState() != STATE_PAUSED) if(! i->Pause(aMilliseconds)) return false;
			}
			mState = STATE_PAUSED;
			return true;
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
			const bool result = GroupResume<EXE_MODE>();
			mLiveTasks.swap(mTaskBuffer);
			mTaskBuffer.clear();
			if(mState == STATE_EXECUTE) mState = STATE_POST_EXECUTE;
			return result;
		}

		bool SOLAIRE_EXPORT_CALL OnCancelI() throw() override {
			if(mState != STATE_CANCELED || mState != STATE_COMPLETE || mState == STATE_INITIALISED) return false;
			mState = STATE_CANCELED;
			for(SharedAllocation<TaskI> i : mLiveTasks) {
				if (!i->OnCancelI()) return false;
			}
			return true;
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
			//return mCallbacks == nullptr ? false : mCallbacks->Cancel();
		}

		bool SOLAIRE_EXPORT_CALL Wait() const throw() override {
			//return mCallbacks == nullptr ? 
			//	mState == STATE_COMPLETE || mState == STATE_CANCELED : 
			//	mCallbacks->Wait();
		}

		bool SOLAIRE_EXPORT_CALL WaitFor(const uint32_t aMilliseconds) const throw() override {
			//return mCallbacks == nullptr ? 
			//	mState == STATE_COMPLETE || mState == STATE_CANCELED : 
			//	mCallbacks->WaitFor(aMilliseconds);
		}

		State SOLAIRE_EXPORT_CALL GetState() const throw() override {
			return mState;
		}
	};

	typedef TaskGroup<TASK_GROUP_ALL, TASK_GROUP_ALL, TASK_GROUP_ALL, TASK_GROUP_ALL> TaskGroupAll;
	typedef TaskGroup<TASK_GROUP_ANY, TASK_GROUP_ANY, TASK_GROUP_ANY, TASK_GROUP_ANY> TaskGroupAny;
	typedef TaskGroup<TASK_GROUP_ONE, TASK_GROUP_ONE, TASK_GROUP_ONE, TASK_GROUP_ONE> TaskGroupOne;
}


#endif
