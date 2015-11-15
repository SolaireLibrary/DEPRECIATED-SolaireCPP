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

#include "Solaire\Script\CompilerExternal.hpp"

namespace Solaire{
	
	// ExternalCompiler

	ExternalCompiler::~ExternalCompiler() {

	}

	bool ExternalCompiler::Compile() const {
		#ifdef SOLAIRE_OS == SOLAIRE_WINDOWS
			STARTUPINFOA si;
			PROCESS_INFORMATION pi;

			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));

			if (!CreateProcessA(
				nullptr,
				const_cast<char*>(GetCommandLine()),
				nullptr,
				nullptr,
				false,
				0,
				nullptr,
				nullptr,
				&si,
				&pi
			)) {
				return false;
			}

			WaitForSingleObject(pi.hProcess, INFINITE);

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			return true;
		#else
			return false;
		#endif
	}
}