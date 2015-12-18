#ifndef SOLAIRE_TASK_IMPLEMENTATION_HPP
#define SOLAIRE_TASK_IMPLEMENTATION_HPP
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

#include "Solaire\Threading\TaskI.hpp"
#include "Solaire\Threading\TaskCallbacks.hpp"

#include <mutex>
#include <condition_variable>
#include <memory>

namespace Solaire {

	class TaskImplementation : public TaskCallbacks {
	private:
		std::mutex mLock;
		std::condition_variable mWaitCondition;
		TaskI& mTask;
	public:
		TaskImplementation(TaskI& aTask) throw() :
			mTask(aTask)
		{}

		SOLAIRE_EXPORT_CALL ~TaskImplementation() throw() {
			mTask.RemoveCallbacks();
		}

		SOLAIRE_FORCE_INLINE uint64_t SOLAIRE_DEFAULT_CALL GetPauseTime() const throw() {
			return mTask.GetPauseTime();
		}

		SOLAIRE_FORCE_INLINE uint64_t SOLAIRE_DEFAULT_CALL GetPauseDuration() const throw() {
			return mTask.GetPauseDuration();
		}

		SOLAIRE_FORCE_INLINE void SOLAIRE_DEFAULT_CALL SetPauseDuration(const uint64_t aMilliseconds) throw() {
			return mTask.SetPauseDuration(aMilliseconds);
		}

		SOLAIRE_FORCE_INLINE void SOLAIRE_DEFAULT_CALL Unpause() throw() {
			return mTask.Unpause();
		}

		SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL Initialise() throw() {
			return mTask.InitialiseI(*this);
		}

		SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL PreExecute() throw() {
			return mTask.OnPreExecuteI();
		}

		SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL Execute() throw() {
			return mTask.OnExecuteI();
		}

		SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL Resume() throw() {
			return mTask.OnResumeI();
		}

		SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL PostExecute() throw() {
			const bool result = mTask.OnPostExecuteI();
			mWaitCondition.notify_all();
			return result;
		}

		SOLAIRE_FORCE_INLINE TaskI::State SOLAIRE_DEFAULT_CALL GetState() const throw() {
			return mTask.GetState();
		}

		SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL ExecuteOnMain() const throw() {
			return mTask.ExecuteOnMain();
		}

		// Inherited from TaskCallbacks
		
		bool SOLAIRE_EXPORT_CALL Cancel() throw() override {
			const bool result = mTask.OnCancelI();
			mWaitCondition.notify_all();
			return result;
		}

		bool SOLAIRE_EXPORT_CALL Wait() throw() override {
			std::unique_lock<std::mutex> lock(mLock);
			mWaitCondition.wait(lock);
			return true;
		}

		bool SOLAIRE_EXPORT_CALL WaitFor(const uint32_t aMilliseconds) throw() override {
			std::unique_lock<std::mutex> lock(mLock);
			return mWaitCondition.wait_for(lock, std::chrono::milliseconds(aMilliseconds)) == std::cv_status::no_timeout;
		}
	};

}

#endif