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
Created			: 11th November 2015
Last Modified	: 11th November 2015
*/

#include "..\Core\Init.hpp"

namespace Solaire{
	class Compiler {
	public:
		//! \todo static libraries

		virtual ~Compiler(){}

		virtual void SetCompilerPath(const char* const) = 0;
		virtual void SetProgramName(const char* const) = 0;
		virtual void SetOutputDirectory(const char* const) = 0;
		virtual void AddIncludeDirectory(const char* const) = 0;
		virtual void AddSourceFile(const char* const) = 0;
		virtual void AddArgument(const char* const) = 0;
		virtual void DefineMacro(const char* const) = 0;
		virtual void DefineMacro(const char* const, const char* const) = 0;

		virtual void CompileExecutable() const = 0;
		virtual void CompileStaticLibrary() const = 0;
		virtual void CompileSharedLibrary() const = 0;
	};
}

#endif
