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

#include "Solaire\Script\CompilerGCC.hpp"

namespace Solaire{
	
	// CompilerGCC

	CompilerGCC::CompilerGCC() {

	}

	CompilerGCC::~CompilerGCC() {

	}

	void CompilerGCC::SetCompilerPath(const char* const aPath) {
		mCompilerPath = aPath;
	}

	void CompilerGCC::SetProgramName(const char* const aFile) {
		mProgramName = aFile;
	}

	void CompilerGCC::SetOutputDirectory(const char* const aPath) {
		mOutputDirectory = aPath;
	}

	void CompilerGCC::AddIncludeDirectory(const char* const aPath) {
		mIncludeDirectories.push_back(aPath);
	}

	void CompilerGCC::AddSourceFile(const char* const aFile) {
		mSourceFiles.push_back(aFile);
	}

	void CompilerGCC::AddArgument(const char* const aArgument) {
		mArguments.push_back(aArgument);
	}

	void CompilerGCC::DefineMacro(const char* const aName) {
		mDefines.push_back(Define(aName, ""));
	}

	void CompilerGCC::DefineMacro(const char* const aName, const char* const aData) {
		mDefines.push_back(Define(aName, aData));
	}

	void CompilerGCC::CompileExecutable() const {
		mLibArgument = "";
		if (!Compile()) throw std::runtime_error("CompilerGCC : Failed to compile");
	}

	void CompilerGCC::CompileStaticLibrary() const {
		//! \todo Implement object archive
		throw std::runtime_error("CompilerGCC : Static library not implemented");
		mLibArgument = "";
		if (!Compile()) throw std::runtime_error("CompilerGCC : Failed to compile");
	}

	void CompilerGCC::CompileSharedLibrary() const {
		mLibArgument = " -shared -fPIC";
		if (!Compile()) throw std::runtime_error("CompilerGCC : Failed to compile");
	}

	const char* CompilerGCC::GetCommandLine() const {
		//static const std::string QUOTE = "\"";
		static const std::string QUOTE = "";

		mCommand.clear();

		mCommand += QUOTE + mCompilerPath + QUOTE;
		for (const std::string& argument : mArguments) {
			mCommand += " " + argument;
		}
		if (mLibArgument.size() > 0) mCommand += " " + mLibArgument;
		for (const Define& define : mDefines) {
			if (define.second.size() == 0) {
				mCommand += " -D " + define.first;
			}
			else {
				mCommand += " -D " + QUOTE + define.first + "=" + define.second + QUOTE;
			}
		}
		for (const std::string& directory : mIncludeDirectories) {
			mCommand += " -Idir " + QUOTE + directory + QUOTE;
		}
		mCommand += " -o " + QUOTE + mOutputDirectory + mProgramName + QUOTE;
		for (const std::string& file : mSourceFiles) {
			mCommand += " " + QUOTE + file + QUOTE;
		}

		return mCommand.c_str();
	}
}