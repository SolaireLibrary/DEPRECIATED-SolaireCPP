#ifndef SOLAIRE_TASK_EXECUTOR_HPP
#define SOLAIRE_TASK_EXECUTOR_HPP

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
\file TaskExecutorI.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 6th December 2015
Last Modified	: 8th December 2015
*/

namespace Solaire {

	class Allocator;
	class TaskI;

	class TaskExecutorI {
	public:
		virtual bool SOLAIRE_EXPORT_CALL Schedule(TaskI&) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Update() throw() = 0;
		virtual SOLAIRE_EXPORT_CALL ~TaskExecutorI() throw(){}
	};

	extern "C" {
		SOLAIRE_EXPORT_API TaskExecutorI* SOLAIRE_EXPORT_CALL CreateThreadPool(Allocator&, const uint32_t) throw();
	}
}


#endif
