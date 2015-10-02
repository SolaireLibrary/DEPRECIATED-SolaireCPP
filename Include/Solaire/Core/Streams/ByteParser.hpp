#ifndef SOLAIRE_CORE_BYTE_PARSER_HPP
#define SOLAIRE_CORE_BYTE_PARSER_HPP

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
	\file ByteParser.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 2nd October 2015
	Last Modified	: 2nd October 2015
*/

#inlude <cstdint>

namespace Solaire{ namespace Core{

    class ByteParser class <T>{
    public:
        enum Status : uint8_t{
            STATUS_ACCEPTED,
            STATUS_COMPLETED,
            STATUS_FAILURE
        };

        virtual ~ByteParser(){

        }

        virtual Status Append(const uint8_t aChar) = 0;
        virtual void Reset() = 0;
        virtual T Get() const = 0;
}}


#endif
