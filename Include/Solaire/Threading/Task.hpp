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

namespace Solaire {

	class Task : public TaskI {
	protected:
		Configuration mConfig;
	protected :
		// Inherited from TaskI

		Configuration& SOLAIRE_EXPORT_CALL GetConfigurationRef() throw() override {
			return mConfig;
		}

	public:
		Task() throw() {
			mConfig.Callbacks = nullptr;
			mConfig.PauseTime = 0;
			mConfig.PauseDuration = 0;
			mConfig.State = STATE_INITIALISED;
			mConfig.ExecutionMode = EXECUTE_ON_WORKER;
		}

		virtual SOLAIRE_EXPORT_CALL ~Task() throw() {

		}

		// Inherited from TaskI

		Configuration SOLAIRE_EXPORT_CALL GetConfiguration() const throw() override {
			return mConfig;
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
