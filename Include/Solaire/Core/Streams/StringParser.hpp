#ifndef SOLAIRE_CORE_STRING_PARSER_HPP
#define SOLAIRE_CORE_STRING_PARSER_HPP

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
	\file StringParser.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 2nd October 2015
	Last Modified	: 2nd October 2015
*/

#include "ByteParser.hpp"

namespace Solaire{ namespace Core{

    template<class T>
    class StringParser : public ResultByteParser<T>{
    protected:
        typedef uint16_t Flags;
    private:
        Flags mFlags;
        char mLastChar;
    protected:
        enum : Flags{
            FLAG_IS_FIRST_CHAR = 0x1,
            FLAG_IS_ESCAPED    = 0x2,
            FLAG_IS_QUOTED     = 0x4
        };

        virtual ByteParser::Status Accept(const Flags aFlags, const char aChar);
    public:
        StringParser() :
            mLastChar('\0'),
            mFlags(FLAG_IS_FIRST_CHAR)
        {}

        virtual ~StringParser(){

        }

        // Inherited from ByteParser

        ByteParser::Status Accept(const uint8_t aByte){
            const char char_ = aByte;

            if(mFlags & FLAG_IS_FIRST_CHAR){
                switch(mLastChar){
                case '\\':
                    mFlags ^= FLAG_IS_ESCAPED;
                    break;
                default:
                    break;
                }
            }

            switch(char_){
            case '"':
                mFlags ^= (mFlags & FLAG_IS_ESCAPED) ? FLAG_IS_QUOTED : 0;
                break;
            default:
                break;
            }

            const ByteParser::Status status = Accept(mFlags, char_);

            mLastChar = char_;
            mFlags &= ~(FLAG_IS_FIRST_CHAR | FLAG_IS_ESCAPED);

            return status;
        }

        virtual void Reset() override{
            mFlags = FLAG_IS_FIRST_CHAR;
        }
    };
}}


#endif
