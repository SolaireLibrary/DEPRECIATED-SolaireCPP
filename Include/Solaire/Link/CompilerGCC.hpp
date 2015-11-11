#ifndef SOLAIRE_COMPILER_GCC_HPP
#define SOLAIRE_COMPILER_GCC_HPP

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

#include <string>
#include <vector>
#include "CompilerExternal.hpp"

namespace Solaire{
	class CompilerGCC : public ExternalCompiler {
	private:
		typedef std::pair<std::string, std::string> Define;
	private:
		std::string mCompilerPath;
		std::string mProgramName;
		std::string mOutputDirectory;
		std::vector<std::string> mIncludeDirectories;
		std::vector<std::string> mSourceFiles;
		std::vector<std::string> mArguments;
		std::vector<Define> mDefines;
		mutable std::string mCommand;
		mutable std::string mLibArgument;
	private:
		std::string BuildCommand() const;
	protected:
		// Inherited from ExternalCompiler

		const char* GetCommandLine() const override;
	public:
		CompilerGCC();
		~CompilerGCC();

		// Inherited from Compiler

		void SetCompilerPath(const char* const)override;
		void SetProgramName(const char* const) override;
		void SetOutputDirectory(const char* const) override;
		void AddIncludeDirectory(const char* const) override;
		void AddSourceFile(const char* const) override;
		void AddArgument(const char* const) override;
		void DefineMacro(const char* const) override;
		void DefineMacro(const char* const, const char* const) override;

		void CompileExecutable() const override;
		void CompileStaticLibrary() const override;
		void CompileSharedLibrary() const override;
	};
}

#endif
