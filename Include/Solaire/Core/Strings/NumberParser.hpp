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

    namespace NumberParserInternal{

        class Value{
        public:
            virtual ~Value(){

            }

            virtual bool Append(const char aChar) = 0;
            virtual void Reset() = 0;
            virtual double Get() const = 0;
        };


        class UnsignedValue : public Value{
        private:
            char mChars[16];
            uint8_t mCount;
        public:
            UnsignedValue():
                mCount(0)
            {}

            // Inherited from value

            void Reset() override{
                mCount = 0;
            }

            bool Append(const char aChar) override{
                if(aChar < '0' || aChar > '9') return false;
                mChars[mCount++] = aChar;
                return true;
            }

            double Get() const override{
                double val = 0;

                uint32_t index = 0;
                const char* i = mChars + mCount - 1;
                const char* const end = mChars - 1;
                while(i != end){
                    val += std::pow(10, index) * static_cast<double>(*i - '0');
                    ++index;
                    --i;
                }

                return val;
            }
        };

        class SignedValue : public Value{
        private:
            UnsignedValue mValue;
            struct{
                uint8_t mSignSet : 1;
                uint8_t mSign : 1;
            };
        public:
            SignedValue():
                mValue(),
                mSignSet(0),
                mSign(1)
            {}

            // Inherited from Value

            void Reset() override{
                mValue.Reset();
                mSignSet = 0;
            }

            bool Append(const char aChar) override{
                if(aChar == '-'){
                    if(! mSignSet){
                        mSignSet = 1;
                        mSign = 0;
                        return true;
                    }
                }
                if(mValue.Append(aChar)){
                    if(! mSignSet){
                        mSignSet = 1;
                        mSign = 1;
                        return true;
                    }else{
                        return true;
                    }
                    return false;
                }
            }

            double Get() const override{
                return mValue.Get() * mSign ? 1.0 : -1.0;
            }
        };

        template<class Body, class Decimal>
        class DecimalValue : public Value{
        private:
            Body mBody;
            Decimal mDecimal;
            struct{
                uint8_t mDecimalFlag : 1;
            };
        public:
            DecimalValue():
                mBody(),
                mDecimal(),
                mDecimalFlag(0)
            {}

            // Inherited from Value

            void Reset() override{
                mBody.Reset();
                mDecimal.Reset();
                mDecimalFlag = 0;
            }

            bool Append(const char aChar) override{
                if(aChar == '.'){
                    if(! mDecimalFlag){
                        mDecimalFlag = 1;
                        return true;
                    }
                }

                return (mDecimalFlag ? mDecimal : mBody).AppendChar(aChar);
            }

            double Get() const override{
                double decimal = mDecimal.Get();
                while(decimal < 1) decimal /= 10.0;
                return mBody.Get() + decimal;
            }
        };

        template<class Body, class Exponent>
        class ExponentValue : public Value{
        private:
            Body mBody;
            Exponent mExponent;
            struct{
                uint8_t mExponentFlag : 1;
                uint8_t mExponentLastFlag : 1;
            };
        public:
            ExponentValue():
                mBody(),
                mExponent(),
                mExponentFlag(0),
                mExponentLastFlag(0)
            {}

            // Inherited from Value

            void Reset() override{
                mBody.Reset();
                mExponent.Reset();
                mExponentFlag = 0;
                mExponentLastFlag = 0;
            }

            bool Append(const char aChar) override{
                if(aChar == 'e' || aChar == 'E'){
                    if(! mExponentFlag){
                        mExponentFlag = 1;
                        mExponentLastFlag = 0;
                        return true;
                    }
                }else if(aChar == '+'){
                    if(mExponentLastFlag){
                        mExponentLastFlag = 0;
                        return true;
                    }
                }
                mExponentLastFlag = 0;
                return (mExponentFlag ? mExponentFlag : mBody).AppendChar(aChar);
            }

            double Get() const override{
                return mBody.Get() * std::pow(10.0, mExponent.Get());
            }
        };

        typedef ExponentValue<DecimalValue<SignedValue, UnsignedValue>, DecimalValue<SignedValue, UnsignedValue>> RealValue;
    }

    class NumberParser{
    private:
    public:

    };

}}


#endif
