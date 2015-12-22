#ifndef SOLAIRE_SHARED_SCRIPT_HPP
#define SOLAIRE_SHARED_SCRIPT_HPP

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
	\file ScriptI.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 22nd December 2015
	Last Modified	: 22nd December 2015
*/

#include "SharedLibrary.hpp"

namespace Solaire {

	typedef SharedAllocation<SharedLibrary> ScriptLib;
	typedef int(SOLAIRE_EXPORT_CALL *ScriptFunction)();

	SOLAIRE_EXPORT_INTERFACE ScriptLoaderCallback{
		virtual void SOLAIRE_EXPORT_CALL operator()(ScriptLib, ScriptFunction) throw() = 0;
	};

	SOLAIRE_EXPORT_INTERFACE ScriptLoaderI {
	public:
		virtual void SOLAIRE_EXPORT_CALL AddScript(const char* const, const char* const, ScriptLoaderCallback&) throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL AddIncludeFile(const char* const) throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL AddSourceFile(const char* const) throw() = 0;
		virtual ScriptLib SOLAIRE_EXPORT_CALL CompileScripts() throw() = 0;
		virtual SOLAIRE_EXPORT_CALL ~ScriptLoaderI() {}
	};

	extern "C" {
		SOLAIRE_EXPORT_API ScriptLoaderI* SOLAIRE_EXPORT_CALL CreateScriptLoader(Allocator&);
	}
}


#endif
