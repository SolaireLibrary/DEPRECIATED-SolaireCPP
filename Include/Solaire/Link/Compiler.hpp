#ifndef SOLAIRE_COMPILER_HPP
#define SOLAIRE_COMPILER_HPP

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
	\file Compiler.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 3rd December 2015
	Last Modified	: 3rd December 2015
*/

#include <cstdint>
#include "..\Core\ModuleHeader.hpp"

namespace Solaire {

	SOLAIRE_EXPORT_INTERFACE Compiler {
	public:
		virtual void SOLAIRE_EXPORT_CALL SetCompilerPath(const char* const) throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL SetOutputPath(const char* const) throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL SetOutputName(const char* const) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL CompileExecutable() throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL AddFlag(const char* const) throw() = 0;
		virtual const char* SOLAIRE_EXPORT_CALL GetLog() const throw() = 0;
		virtual ~Compiler(){}
	};

	SOLAIRE_EXPORT_INTERFACE LibCompiler : public Compiler {
	public:
		virtual bool SOLAIRE_EXPORT_CALL CompileStaticLib() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL CompileSharedLib() throw() = 0;
		virtual ~LibCompiler(){}
	};

	SOLAIRE_EXPORT_INTERFACE CppCompiler : public LibCompiler {
	public:
		virtual void SOLAIRE_EXPORT_CALL AddSourceFile(const char* const) throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL AddIncludePath(const char* const) throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL AddDefine(const char*, const char*) throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL GenerateImportLibrary(const bool) throw () = 0;
		virtual ~CppCompiler() {}

		SOLAIRE_FORCE_INLINE void SOLAIRE_EXPORT_CALL AddDefine(const char* const aName) throw() {
			AddDefine(aName, nullptr);
		}
	};
}


#endif
