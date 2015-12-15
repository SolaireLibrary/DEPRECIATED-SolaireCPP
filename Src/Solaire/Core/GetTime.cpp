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
#include <chrono>

namespace Solaire {

	extern "C" {
		uint64_t SOLAIRE_EXPORT_CALL GetTimeMilliseconds() throw() {
			auto time = std::chrono::high_resolution_clock::now();
			auto duration = time.time_since_epoch();
			return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		}
	}

}
