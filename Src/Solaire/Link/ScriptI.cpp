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

#include <vector>
#include <string>
#include <functional>
#include <mutex>
#include <fstream>

#include "Solaire\Memory\Allocator.hpp"
#include "Solaire\Link\ScriptI.hpp"
#include "Solaire\Link\CompilerMinGW.hpp"

namespace Solaire {

	class ScriptLoaderImplementation : public ScriptLoaderI {
	private:
		struct ScriptData {
			std::string ScriptName;
			std::string ScriptSource;
			ScriptLoaderCallback* Callback;
		};
	private:
		Allocator& mAllocator;
		std::vector<ScriptData> mScripts;
		std::vector<std::string> mIncludeFiles;
		std::vector<std::string> mSourceFiles;
	private:
		static void _WriteFuntion(std::ostream& aStream, const ScriptData& aData) {
			aStream << "	static int SOLAIRE_EXPORT_CALL " << aData.ScriptName << "() throw() {" << std::endl;
			aStream << "		#include \"" << aData.ScriptSource << "\"" << std::endl;
			aStream << "	}" << std::endl << std::endl;
		}

		static void _WriteSelector(std::ostream& aStream, const std::vector<ScriptData>& aScripts) {
			aStream << "	typedef int(SOLAIRE_EXPORT_CALL *ScriptPtr)();" << std::endl;
			aStream << "	extern \"C\" SOLAIRE_EXPORT_API ScriptPtr SOLAIRE_EXPORT_CALL GetScript(const int aIndex) throw() {" << std::endl;
			aStream << "		switch(aIndex){" << std::endl;
			const int size = aScripts.size();
			for(int i = 0; i < size; ++i) {
				aStream << "			case " << i << " : " << std::endl;
				aStream << "				return &" << aScripts[i].ScriptName << ";" << std::endl;
			}
			aStream << "			default  : " << std::endl;
			aStream << "				return (ScriptPtr) nullptr;" << std::endl;
			aStream << "		}" << std::endl;
			aStream << "	}" << std::endl << std::endl;
		}
	public:
		ScriptLoaderImplementation(Allocator& aAllocator) :
			mAllocator(aAllocator)
		{}

		SOLAIRE_EXPORT_CALL ~ScriptLoaderImplementation() {

		}

		// Inherited from ScriptLoaderI

		void SOLAIRE_EXPORT_CALL AddScript(const char* const aScriptName, const char* const aScriptSource, ScriptLoaderCallback& aCallback) throw() override {
			ScriptData data;
			data.ScriptName = aScriptName;
			data.ScriptSource = aScriptSource;
			data.Callback = &aCallback;
			mScripts.push_back(data);
		}

		void SOLAIRE_EXPORT_CALL AddIncludeFile(const char* const aFile) throw() override {
			mIncludeFiles.push_back(aFile);
		}

		void SOLAIRE_EXPORT_CALL AddSourceFile(const char* const aFile) throw() override {
			mSourceFiles.push_back(aFile);
		}

		ScriptLib SOLAIRE_EXPORT_CALL CompileScripts() throw() override {
			char pathBuf[MAX_PATH];
			//GetTemporaryDirectory(pathBuf);
			std::strcpy(pathBuf, "tmp\\");
			const std::string tmpDirectory = pathBuf;
			const std::string outputName = "SOLAIRE_SCRIPT"; //! \todo create a random dll name
			const std::string source = tmpDirectory + outputName + ".cpp";
			const std::string outputFile = tmpDirectory + outputName + ".dll";
			
			// Get Scripts
			std::vector<ScriptData> scripts;
			std::vector<std::string> includeFiles;
			std::vector<std::string> sourceFiles;
			//SCRIPT_LOCK.lock();
			std::swap(mScripts, scripts);
			std::swap(mIncludeFiles, includeFiles);
			std::swap(mSourceFiles, sourceFiles);
			//SCRIPT_LOCK.unlock();

			if(scripts.empty()) return ScriptLib();

			//includeFiles.push_back("Solaire\\Memory\\Allocator.hpp");
			sourceFiles.push_back(source);
			
			// Generate source
			{
				std::ofstream sourceFile(source);
				if(! sourceFile.is_open()) return ScriptLib();

				for(const std::string& i : includeFiles) {
					sourceFile << "#include \"" << i << "\"" << std::endl;
				}
				sourceFile << "#undef SOLAIRE_EXPORT_API" << std::endl;
				sourceFile << "#define SOLAIRE_EXPORT_API __declspec(dllexport)" << std::endl;
				sourceFile << std::endl;
				sourceFile << "namespace Solaire {" << std::endl;
				for(const ScriptData& i : scripts) _WriteFuntion(sourceFile, i);
				_WriteSelector(sourceFile, scripts);
				sourceFile << "}" << std::endl;
			}
			
			// Initialise compiler
			SharedAllocation<CppCompiler> compiler(mAllocator, CreateCompilerMinGW(mAllocator));
			//compiler->SetCompilerPath("MinGW\\bin\\mingw32-g++.exe");
			compiler->SetCompilerPath("C:\\Program Files\\CodeBlocks\\MinGW\\bin\\mingw32-g++.exe");
			compiler->SetOutputPath(tmpDirectory.c_str());
			compiler->SetOutputName(outputName.c_str());
			for(const std::string& i : sourceFiles) {
				compiler->AddSourceFile(i.c_str());
			}
			compiler->AddFlag("-std=c++11");
			compiler->AddDefine("var", "auto");
			compiler->AddDefine("SOLAIRE_EXPORT_CALL", "__stdcall");
			compiler->AddDefine("SOLAIRE_EXPORT_API", "__declspec(dllimport)");
			
			// Compile
			compiler->CompileSharedLib();
			
			// Load

			ScriptLib lib(mAllocator, CreateSharedLibrary(mAllocator));
			if(! lib) return ScriptLib();
			//if(! lib->Open(outputFile.c_str())) return ScriptLib();
			{
				typedef ScriptFunction(SOLAIRE_EXPORT_CALL *GetScriptType)(const int);
				//GetScriptType GetScript = lib->LoadFunction<int, const int>("GetScript"); //! \todo Check correct export name
				GetScriptType GetScript = (GetScriptType) lib->LoadUntypedFunction("GetScript"); //! \todo Check correct export name

				const int size = scripts.size();
				for(int i = 0; i < size; ++i) {
					const ScriptData& data = scripts[i];
					//data.Callback(lib, GetFunction(i));
					//data.Callback(lib, (ScriptFunction) nullptr);
				}
			}
			
			return lib;
		}

	};

	extern "C" {
		SOLAIRE_EXPORT_API ScriptLoaderI* SOLAIRE_EXPORT_CALL CreateScriptLoader(Allocator& aAllocator) {
			return new(aAllocator.Allocate(sizeof(ScriptLoaderImplementation))) ScriptLoaderImplementation(aAllocator);
		}
	}
}