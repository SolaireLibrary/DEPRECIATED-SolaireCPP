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

namespace Solaire {

	class TaskImplementation;
	class TaskCallbacks;

	class TaskI {
	public :
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
	protected :
		virtual bool SOLAIRE_EXPORT_CALL InitialiseI(TaskCallbacks&) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnPreExecuteI() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnExecuteI() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnPostExecuteI() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnResumeI() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnCancelI() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Pause() throw() = 0;
	public:
		virtual bool SOLAIRE_EXPORT_CALL Cancel() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Wait() const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL WaitFor(const uint32_t) const throw() = 0;
		virtual State SOLAIRE_EXPORT_CALL GetState() const throw() = 0;
		virtual SOLAIRE_EXPORT_CALL ~TaskI() throw(){}
	};
}


#endif
