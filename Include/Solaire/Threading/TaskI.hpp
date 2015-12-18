#ifndef SOLAIRE_TASKI_HPP
#define SOLAIRE_TASKI_HPP

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
\file TaskI.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 6th December 2015
Last Modified	: 8th December 2015
*/

#include <cstdint>
#include "..\Core\ModuleHeader.hpp"
#include "TaskCallbacks.hpp"

namespace Solaire {

	class TaskImplementation;
	class TaskCallbacks;

	template<const int INIT_MODE, const int PRE_MODE, const int EXE_MODE, const int POST_MODE>
	class TaskGroup;

	SOLAIRE_EXPORT_INTERFACE TaskI {
	public :
		template<const int, const int, const int, const int>
		friend class TaskGroup;

		enum ExecutionMode : uint8_t {
			EXECUTE_ON_WORKER,
			EXECUTE_ON_MAIN
		};

		friend TaskImplementation;
		enum State : uint8_t{
			STATE_INITIALISED,
			STATE_PRE_EXECUTE,
			STATE_EXECUTE,
			STATE_PAUSED,
			STATE_POST_EXECUTE,
			STATE_CANCELED,
			STATE_COMPLETE
		};

		struct Configuration {
			TaskCallbacks* Callbacks;
			uint64_t PauseTime;
			uint16_t PauseDuration;
			struct {
				uint16_t State : 3;
				uint16_t ExecutionMode : 1;
			};
		};
	protected :
		virtual bool SOLAIRE_EXPORT_CALL InitialiseI() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnPreExecuteI() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnExecuteI() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnPostExecuteI() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnResumeI() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnCancelI() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Pause(const uint64_t) throw() = 0;
		virtual Configuration& SOLAIRE_EXPORT_CALL GetConfigurationRef() throw() = 0;

		SOLAIRE_FORCE_INLINE void SOLAIRE_DEFAULT_CALL Unpause() throw() {
			GetConfigurationRef().PauseDuration = 0;
		}
	public:
		virtual bool SOLAIRE_EXPORT_CALL Cancel() throw() = 0;
		virtual Configuration SOLAIRE_EXPORT_CALL GetConfiguration() const throw() = 0;
		virtual SOLAIRE_EXPORT_CALL ~TaskI() throw(){}

		SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL Wait() const throw() {
			const Configuration config = GetConfiguration();
			return config.Callbacks == nullptr ?
				config.State == STATE_COMPLETE || config.State == STATE_CANCELED :
				config.Callbacks->Wait();
		}

		SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL WaitFor(const uint32_t aMilliseconds) const throw() {
			const Configuration config = GetConfiguration();
			return config.Callbacks == nullptr ?
				config.State == STATE_COMPLETE || config.State == STATE_CANCELED :
				config.Callbacks->WaitFor(aMilliseconds);
		}

		SOLAIRE_FORCE_INLINE State SOLAIRE_DEFAULT_CALL GetState() const throw() {
			return static_cast<State>(GetConfiguration().State);
		}

		SOLAIRE_FORCE_INLINE ExecutionMode SOLAIRE_DEFAULT_CALL GetExecutionMode() const throw() {
			return static_cast<ExecutionMode>(GetConfiguration().ExecutionMode);
		}

		SOLAIRE_FORCE_INLINE uint64_t SOLAIRE_DEFAULT_CALL GetPauseTime() const throw() {
			return GetConfiguration().PauseTime;
		}

		SOLAIRE_FORCE_INLINE uint64_t SOLAIRE_DEFAULT_CALL GetPauseDuration() const throw() {
			return GetConfiguration().PauseDuration;
		}

		SOLAIRE_FORCE_INLINE uint64_t SOLAIRE_DEFAULT_CALL GetResumeTime() const throw() {
			const Configuration tmp = GetConfiguration();
			return tmp.PauseTime + tmp.PauseDuration;
		}
	};
}


#endif
