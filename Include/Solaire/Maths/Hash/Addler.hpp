#ifndef SOLAIRE_HASH_ADDLER32_HPP
#define SOLAIRE_HASH_ADDLER32_HPP

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
\file Addler32.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 1st October 2015
Last Modified	: 1st October 2015
*/

#include "HashFunction.hpp"

namespace Solaire{

    template<class T, typename Enable = typename std::enable_if<std::is_same<T, uint32_t>::value, void>::type>
    class Addler : public HashFunction<uint32_t>
    {
    public:
        // Inherited from HashFunction
        HashType SOLAIRE_EXPORT_CALL Hash(const void* const aValue, const size_t aBytes) const throw() override{
			const uint8_t* const data = static_cast<const uint8_t*>(aValue);
			HashType s1 = 1;
			HashType s2 = 0;

			for(size_t i = 0; i < aBytes; ++i){
				s1 = (s1 + data[i]) % 65521;
				s2 = (s2 + s1) % 65521;
			}
			return (s2 << 16) | s1;
        }
    };

    typedef Addler<uint32_t> Addler32;
}

#endif
