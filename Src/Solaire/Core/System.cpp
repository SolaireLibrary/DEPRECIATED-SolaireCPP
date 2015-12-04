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

#include "Solaire\Core\System.hpp"
#include <cstring>

namespace Solaire {

	extern "C" {
		bool SOLAIRE_EXPORT_CALL RunExecutable(const char* const aPath, Iterator<const char*>& aParam, int* const aReturnCode) {
		#if SOLAIRE_OS == SOLAIRE_WINDOWS
			char buf[MAX_PATH * 2];
			uint32_t offset = 0;
			uint32_t size;

			size = std::strlen(aPath);
			std::memcpy(buf + offset, aPath, size);
			offset += size;

			while(! aParam.AtEnd()) {
				buf[offset++] = ' ';
				const char* const param = *aParam;
				size = std::strlen(param);
				std::memcpy(buf + offset, param, size);
				offset += size;
				++aParam;
			}

			buf[offset++] = '\0';

			PROCESS_INFORMATION processInformation;
			STARTUPINFOA startupInfo;
			memset(&processInformation, 0, sizeof(processInformation));
			memset(&startupInfo, 0, sizeof(startupInfo));
			startupInfo.cb = sizeof(startupInfo);

			if(CreateProcessA(nullptr, buf, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInformation)) {
				WaitForSingleObject(processInformation.hProcess, INFINITE);
				if(aReturnCode) {
					DWORD code;
					GetExitCodeProcess(processInformation.hProcess, &code);
					*aReturnCode = code;
				}
				CloseHandle(processInformation.hProcess);
				CloseHandle(processInformation.hThread);
				return true;
			}else {
				return false;
			}
		#else
			return false;
		#endif
		}
	}

}
