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
Last Modified	: 14th December 2015
*/

#include <cstdint>
#include "..\Core\ModuleHeader.hpp"
#include "..\Memory\Allocator.hpp"

namespace Solaire {

	class Allocator;
	class TaskI;

	SOLAIRE_EXPORT_INTERFACE TaskExecutorI {
	public:
		virtual bool SOLAIRE_EXPORT_CALL Schedule(SharedAllocation<TaskI>) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Update() throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL WaitAll() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL WaitAllFor(const uint32_t) throw() = 0;
		virtual uint32_t SOLAIRE_EXPORT_CALL GetThreadCount() throw() = 0;
		virtual uint32_t SOLAIRE_EXPORT_CALL GetTaskCount() throw() = 0;
		virtual SOLAIRE_EXPORT_CALL ~TaskExecutorI() throw(){}
	};

	#ifdef SOLAIRE_EXPORT_IMPORT_LIBRARY
		extern "C" SOLAIRE_EXPORT_API TaskExecutorI* SOLAIRE_EXPORT_CALL _CreateThreadPool(Allocator&, const uint32_t) throw();
		inline TaskExecutorI* CreateThreadPool(Allocator& aAllocator, const uint32_t aThreads) {return _CreateThreadPool(aAllocator, aThreads);}
	#else 
		static constexpr const char* const CreateThreadPool_FnName = "__CreateThreadPool@8";
		static TaskExecutorI* (SOLAIRE_EXPORT_CALL *CreateThreadPool)(Allocator&, const uint32_t);
	#endif
}


#endif