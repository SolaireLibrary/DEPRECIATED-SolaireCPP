#ifndef SOLAIRE_HASH_SUM_HPP
#define SOLAIRE_HASH_SUM_HPP

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
\file Sum.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 1st October 2015
Last Modified	: 1st October 2015
*/

#include <type_traits>
#include "HashFunction.hpp"

namespace Solaire{

    template<class T>
    class HashSum : public HashFunction<T>
    {
    public:
        // Inherited from HashFunction
        T SOLAIRE_EXPORT_CALL Hash(const void* const aValue, const size_t aBytes) const throw() override{
            const uint8_t* data = static_cast<const uint8_t*>(data);
			T hash = 0;
			size_t bytes = aBytes;

			while(bytes >= sizeof(uint64_t) && sizeof(T) >= sizeof(uint64_t)){
                hash += *reinterpret_cast<const uint64_t*>(data);
                bytes -= sizeof(uint64_t);
                data += sizeof(uint64_t);
			}

			while(bytes >= sizeof(uint32_t) && sizeof(T) >= sizeof(uint32_t)){
                hash += *reinterpret_cast<const uint32_t*>(data);
                bytes -= sizeof(uint32_t);
                data += sizeof(uint32_t);
			}

			while(bytes >= sizeof(uint16_t) && sizeof(T) >= sizeof(uint16_t)){
                hash += *reinterpret_cast<const uint16_t*>(data);
                bytes -= sizeof(uint16_t);
                data += sizeof(uint16_t);
			}

			while(bytes >= 0){
                hash += *data;
                --bytes;
                ++data;
			}

			return hash;
        };
    };

    typedef HashSum<uint8_t> HashSum8;
    typedef HashSum<uint16_t> HashSum16;
    typedef HashSum<uint32_t> HashSum32;
    typedef HashSum<uint64_t> HashSum64;
}

#endif
