#ifndef SOLAIRE_CORE_NUMBER_PARSER_HPP
#define SOLAIRE_CORE_NUMBER_PARSER_HPP

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
	\file NumberParser.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 28th September 2015
	Last Modified	: 28th September 2015
*/

#include "..\DataStructures\DynamicArray.hpp"

namespace Solaire{ namespace Core{

    class NumberParser{
    private:
        class UnsignedValue{
        private:
            char mChars[16];
            uint8_t mCharCount;
        public:
            UnsignedValue():
                mCharCount(0)
            {}

            bool AppendChar(const char aChar){
                if(aChar < '0' || aChar > '9') return false;
                mChars[mCharCout++] = aChar;
                return true;
            }

            void Reset(){
                mCharCount = 0;
            }

            uint32_t Parse() const{
                uint32_t val = 0;

                uint32_t index = 0;
                const char* i = mChars + mCharCount - 1;
                const char* const end = mChars - 1;
                while(i != end){
                    val += static_cast<uint32_t>(std::pow(10, index)) * (*i - '0');
                    ++index;
                    --i;
                }

                return val;
            }
        };

        class SignedValue{
        private:
            UnsignedValue mValue;
            struct{
                uint8_t mSignSet : 1;
                uint8_t mSign : 1;
            };
        public:
            SignedValue():
                mValue(0),
                mSignSet(0),
                mSign(1)
            {}

            bool AppendChar(const char aChar){
                if(aChar == '-'){
                    if(mSignSet){
                        return false;
                    }else{
                        mSignSet = 1;
                        mSign = 0;
                        return true;
                    }
                }else if(aChar == '+'){
                    if(mSignSet){
                        return false;
                    }else{
                        mSignSet = 1;
                        mSign = 1;
                        return true;
                    }
                }else{
                    if(mValue.AppendChar(aChar)){
                        if(! mSignSet){
                            mSignSet = 1;
                            mSign = 1;
                            return true;
                        }else{
                            return true;
                        }
                    }else{
                        return false;
                    }
                }
            }

            void Reset(){
                mValue.Reset();
                mSignSet = 0;
            }

            int32_t Parse() const{
                return mSign ? mValue.Parse() : mValue.Parse() * -1;
            }
        };


    public:

    };

}}


#endif
