#ifndef SOLAIRE_HASH_FUNCTION_HPP
#define SOLAIRE_HASH_FUNCTION_HPP

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
\file HashFunction.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 1st October 2015
Last Modified	: 1st October 2015
*/

#include <cstdint>
#include "..\..\Core\Init.hpp"

namespace Solaire{

    template<class HASH_TYPE, typename Enable = typename std::enable_if<std::is_unsigned<HASH_TYPE>::value, void>::type>
    class HashFunction
    {
    public:
        typedef HASH_TYPE HashType;

        virtual ~HashFunction(){

        }

        virtual HashType Hash(const void* const aValue, const size_t aBytes) const = 0;
    };

    template<class T, class HASH_TYPE>
    struct HashWrapper{
        static HASH_TYPE Hash(const HashFunction<HASH_TYPE>& aFunction, typename TypeTraits<T>::ConstPassType aValue){
            return aFunction.Hash(&aValue, sizeof(T));
        }
    };

    typedef HashFunction<uint8_t> Hash8;
    typedef HashFunction<uint16_t> Hash16;
    typedef HashFunction<uint32_t> Hash32;
    typedef HashFunction<uint64_t> Hash64;

}

#endif
