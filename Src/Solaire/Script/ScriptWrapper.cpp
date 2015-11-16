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

#include "Solaire\Script\ScriptWrapper.hpp"
		
namespace Solaire{

	// ScriptWrapper

	bool ScriptWrapper::GenerateHeaderFile(std::ostream& aOutput, const DynamicArray<String>& aScriptHeaders) {
		#if SOLAIRE_OS == SOLAIRE_WINDOWS
			aOutput << "#ifndef SOLAIRE_SCRIPT_MAIN_H" << std::endl;
			aOutput << "#define SOLAIRE_SCRIPT_MAIN_H" << std::endl;
			aOutput << std::endl;
			aOutput << "#include <windows.h>" << std::endl;
			aOutput << "#include <cstdint>" << std::endl;
			aOutput << std::endl;
			aOutput << "		class Script;" << std::endl;
			aOutput << "		class Allocator;" << std::endl;
			aOutput << std::endl;
			aOutput << "		extern \"C\" {" << std::endl;
			aOutput << "			uint32_t __declspec(dllexport) GetScriptCount();" << std::endl;
			aOutput << "			Script* __declspec(dllexport) CreateScript(Allocator&, const uint32_t);" << std::endl;
			aOutput << "		}" << std::endl;
			return true;
		#else
			#error SolaireCPP : ScriptWrapper only implemented for Windows
			return false;
		#endif
	}

	static ConstStringFragment ParseScriptName(const String& aFileName) {
		const auto directory = aFileName.FindLast('\\');
		return ConstStringFragment(
			directory == aFileName.end() ? aFileName.begin() : (directory + 1),
			aFileName.FindFirst('.')
		);
	}

	bool ScriptWrapper::GenerateSourceFile(std::ostream& aOutput, const DynamicArray<String>& aScriptHeaders) {
		#if SOLAIRE_OS == SOLAIRE_WINDOWS
			aOutput << "#include \"main.h\"" << std::endl;
			for(const String& file : aScriptHeaders) {
				aOutput << "#include \"" << file << '"' << std::endl;
			}
			aOutput << std::endl;
			aOutput << "uint32_t __declspec(dllexport) GetScriptCount(){" << std::endl;
			aOutput << "	return " << aScriptHeaders.Size() << ';' << std::endl;
			aOutput << "}" << std::endl;
			aOutput << std::endl;
			aOutput << "Script* __declspec(dllexport) CreateScript(Allocator& aAllocator, const uint32_t aScriptNo){" << std::endl;
			aOutput << "	switch(aScriptNo){" << std::endl;
			uint32_t i = 0;
			for(const String& file : aScriptHeaders) {
				aOutput << "	case " << i << file << ':' << std::endl;
				const ConstStringFragment scriptClass = ParseScriptName(file);
				aOutput << "		return new(aAllocator.Allocate(sizeof(" << scriptClass << "))) " << scriptClass << "(aAllocator);" << std::endl;
				++i;
			}
			aOutput << "	default :" << std::endl;
			aOutput << "		return nullptr;;" << std::endl;
			aOutput << "	}" << std::endl;
			aOutput << "}" << std::endl;
			aOutput << std::endl;
			aOutput << "extern \"C\" __declspec(dllexport) BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){" << std::endl;
			aOutput << "	return TRUE;" << std::endl;
			aOutput << "}" << std::endl;

			return true;
		#else
			#error SolaireCPP : ScriptWrapper only implemented for Windows
			return false;
		#endif
	}
	
}
