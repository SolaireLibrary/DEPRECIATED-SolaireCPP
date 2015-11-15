#ifndef SOLAIRE_COMPILER_EXTERNAL_HPP
#define SOLAIRE_COMPILER_EXTERNAL_HPP

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
\file CompilerExternal.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 11th November 2015
Last Modified	: 11th November 2015
*/

#include "Compiler.hpp"

#undef GetCommandLine

namespace Solaire{
	class ExternalCompiler : public Compiler {
	protected:
		virtual const char* GetCommandLine() const = 0;

		bool Compile() const;
	public:
		virtual ~ExternalCompiler();
	};
}

#endif
