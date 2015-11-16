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

#include "Solaire\Maths\Hash\Sdbm.hpp"

namespace Solaire{

    // Sdbm
       
	Sdbm::HashType SOLAIRE_EXPORT_CALL Sdbm::Hash(const void* const aValue, const size_t aBytes) const throw() {
        HashType hash = 0;
		const uint8_t* const data = static_cast<const uint8_t*>(aValue);
		for(size_t i = 0; i < aBytes; ++i){
			hash = data[i] + (hash << 6) + (hash << 16) - hash;
		}
		return hash;
    }
}