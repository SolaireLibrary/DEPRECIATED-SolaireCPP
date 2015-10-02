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

#include <cstdint>

namespace Solaire{ namespace Core{

    class ByteParser{
    public:
        enum Status : uint8_t{
            STATUS_ACCEPTED,
            STATUS_COMPLETED,
            STATUS_FAILURE
        };

        virtual ~ByteParser(){

        }

        virtual Status Accept(const uint8_t aByte) = 0;
        virtual void Reset() = 0;
        virtual Status GetStatus() const = 0;

        friend std::istream& operator>>(std::istream& aStream, ByteParser& aParser){
            uint8_t b;
            int16_t count = 0;
            Status status = STATUS_FAILURE;
            while(! aStream.eof()){
                aStream >> b;
                ++count;
                status = aParser.Accept(b);

                switch(status){
                case STATUS_COMPLETED :
                    return aStream;
                case STATUS_FAILURE :
                    goto REWIND_STREAM;
                default:
                    break;
                }
            }

            REWIND_STREAM:
            aStream.seekg(-count, std::ios_base::cur);

            return aStream;
        }

        template<class Iterator>
        Iterator Parse(const Iterator aBegin, const Iterator aEnd){
            for(Iterator i = aBegin; i != aEnd; ++i){
                switch(Accept(*i)){
                case STATUS_COMPLETED:
                    return i;
                case STATUS_FAILURE:
                    return aBegin;
                default:
                    break;
                }
            }
            return aBegin;
        }
    };

    template<class T>
    class ResultByteParser : public ByteParser{
    public:
        virtual ~ResultByteParser(){}

        virtual T Get() const = 0;
    };
}}


#endif
