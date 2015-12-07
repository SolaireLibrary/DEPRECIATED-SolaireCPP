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

#include "Solaire\Link\CompilerMinGW.hpp"
#include "Solaire\Memory\Allocator.hpp"
#include "Solaire\Core\System.hpp"
#include "Solaire\IO\File.hpp"
#include "Solaire\Strings\String.hpp"
#include <vector>
#include <string>

namespace Solaire{

	class CompilerMinGW : public CppCompiler{
	private:
		std::string mOutputName;
		std::string mCompilerPath;
		std::string mOutputPath;
		std::vector<std::string> mSourceFiles;
		std::vector<std::string> mIncludeDirs;
		typedef std::pair<std::string, std::string> Define;
		std::vector<Define> mDefines;
		std::vector<std::string> mFlags;
		std::string mLog;
		bool mGenerateImportLib;
	private:
		bool ExecuteMinGW(const std::vector<std::string>& aFlags) throw() {
			const uint32_t flagCount = aFlags.size();
			const char** const flags = new const char*[flagCount];
			for (uint32_t i = 0; i < flagCount; ++i) flags[i] = aFlags[i].c_str();

			int returnCode;
			if (! RunExecutable(
				mCompilerPath.c_str(),
				IteratorPtr<const char*>(flags),
				IteratorPtr<const char*>(flags, flagCount), 
				&returnCode
			)){
				delete[] flags;
				mLog = "Failed to call compiler .exe";
				return false;
			}
			delete[] flags;
			//! \todo implementent log
			return returnCode == 0;
		}

		static void AddIncludeFlags(std::vector<std::string>& aFlags, const std::vector<std::string>& aIncludes) throw() {
			for(const std::string& i : aIncludes) {
				aFlags.push_back("-iquotedir\"" + i + "\"");
			}
		}

		static void AddDefineFlags(std::vector<std::string>& aFlags, const std::vector<Define>& aDefines) throw() {
			for(const Define& i : aDefines) {
				aFlags.push_back(i.second.empty() ?
					"-D \"" + i.first + "\"" :
					"-D \"" + i.first + "\"=\"" + i.second + "\""
				);
			}
		}

		static void AddSourceFlags(std::vector<std::string>& aFlags, const std::vector<std::string>& aFiles) throw() {
			std::string flag = "-c ";
			for(const std::string& i : aFiles) {
				flag += "\"" + i + "\" ";
			}
			aFlags.push_back(flag);
		}
	public:
		CompilerMinGW(Allocator& aAllocator) :
			mGenerateImportLib(false)
		{}

		// Inherited from Compiler

		void SOLAIRE_EXPORT_CALL SetCompilerPath(const char* const aPath) throw() override {
			mCompilerPath = aPath;
		}

		void SOLAIRE_EXPORT_CALL SetOutputPath(const char* const aPath) throw() override {
			mOutputPath = aPath;
		}

		void SOLAIRE_EXPORT_CALL SetOutputName(const char* const aPath) throw() override {
			mOutputName = aPath;
		}

		void SOLAIRE_EXPORT_CALL AddSourceFile(const char* const aPath) throw() override {
			mSourceFiles.push_back(std::string(aPath));
		}

		void SOLAIRE_EXPORT_CALL AddIncludePath(const char* const aPath) throw() override {
			mIncludeDirs.push_back(std::string(aPath));
		}

		void SOLAIRE_EXPORT_CALL AddDefine(const char* aName, const char* aValue) throw() override {
			mDefines.push_back(Define(std::string(aName), aValue ? std::string(aValue) : std::string()));
		}

		bool SOLAIRE_EXPORT_CALL CompileExecutable() throw() override {
			return false;
			//! \todo Implement CompilerMinGW::CompileExecutable
		}

		bool SOLAIRE_EXPORT_CALL CompileStaticLib() throw() override {
			return false;
			//! \todo Implement CompilerMinGW::CompileStaticLib
		}

		bool SOLAIRE_EXPORT_CALL CompileSharedLib() throw() override {
			if(mSourceFiles.size() == 0 || mSourceFiles.size() > 1) false;
			//! \todo Compiler shared lib with more than 1 source file

			std::vector<std::string> flags = mFlags;
			bool passA = false;
			bool passB = false;

			const std::string oFile = "dll.o";
			//const std::string defFile = "\"" + tmpFolder + "dll.def\"";
			const std::string aFile = "\"" + mOutputPath + mOutputName + ".a\"";

			// Build .o
			flags.push_back("-DBUILD_DLL");
			AddSourceFlags(flags, mSourceFiles);
			flags.push_back("-o " + oFile);
			AddIncludeFlags(flags, mIncludeDirs);
			AddDefineFlags(flags, mDefines);
			passA = ExecuteMinGW(flags);
			flags.clear();

			// Build .dll
			if(passA) {
				flags.push_back("-shared");
				//flags.push_back("-Wl,--output-def=\"" + defFile + "\"");
				if(mGenerateImportLib) flags.push_back("-Wl,--out-implib=\"" + aFile + "\"");
				flags.push_back("-Wl,--dll " + oFile);
				flags.push_back(" -o \"" + mOutputPath + mOutputName + ".dll\"");
				flags.push_back("-s");
				flags.push_back("-luser32");
				passB = ExecuteMinGW(flags);

				File::Delete(oFile.c_str());
			}

			return passA && passB;
		}

		void SOLAIRE_EXPORT_CALL AddFlag(const char* const aFlag) throw() override {
			mFlags.push_back(aFlag);
		}

		const char* SOLAIRE_EXPORT_CALL GetLog() const throw() override {
			return mLog.c_str();
		}
	
		void SOLAIRE_EXPORT_CALL GenerateImportLibrary(const bool aGenerate) throw () override {
			mGenerateImportLib = aGenerate;
		}
	};

	SOLAIRE_EXPORT_API CppCompiler* CreateCompilerMinGW(Allocator& aAllocator) {
		return new(aAllocator.Allocate(sizeof(CompilerMinGW))) CompilerMinGW(aAllocator);
	}
}
