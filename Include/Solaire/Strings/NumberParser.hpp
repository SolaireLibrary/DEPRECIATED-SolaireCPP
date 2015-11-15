#ifndef SOLAIRE_NUMBER_PARSER_HPP
#define SOLAIRE_NUMBER_PARSER_HPP

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

#include <algorithm>
#include <cmath>
#include "..\Strings\StringFragment.hpp"
#include "..\DataStructures\DynamicArray.hpp"

namespace Solaire{

    namespace NumericParse{

        class UnsignedValue{
        public:
            typedef uint32_t NumericType;
        private:
            char mChars[16];
            uint8_t mCount;
        public:
            UnsignedValue():
                mCount(0)
            {}

            uint8_t PrecedingZeros() const{
                uint8_t count = 0;
                for(uint8_t i = 0; i < mCount; ++i){
                    if(mChars[i] == '0'){
                        ++count;
                    }else{
                        break;
                    }
                }
                return count;
            }

            // Inherited from value

            void Reset(){
                mCount = 0;
            }

            bool Accept(const char aChar){
                if(aChar < '0' || aChar > '9') return false;
                mChars[mCount++] = aChar;
                return true;
            }

            NumericType Get() const{
                if(mCount == 0) throw std::runtime_error("NumericParse::UnsignedValue : Cannot parse 0 length value");
                double val = 0;

                uint32_t index = 0;
                const char* i = mChars + mCount - 1;
                const char* const end = mChars - 1;
                while(i != end){
                    val += std::pow(10, index) * static_cast<double>(*i - '0');
                    ++index;
                    --i;
                }

                return static_cast<NumericType>(val);
            }
        };

        class SignedValue{
        public:
            typedef int32_t NumericType;
        private:
            UnsignedValue mValue;
            int8_t mSign;
            bool mIsFirst;
        public:
            SignedValue():
                mValue(),
                mSign(1),
                mIsFirst(true)
            {}

            uint8_t PrecedingZeros() const{
                return mValue.PrecedingZeros();
            }

            // Inherited from Value

            void Reset(){
                mValue.Reset();
                mIsFirst = true;
            }

            bool Accept(const char aChar){
                if(mIsFirst){
                     if(aChar == '-'){
                        mSign = -1;
                        mIsFirst = false;
                        return true;
                    }else{
                        mSign = 1;
                    }
                }
                mIsFirst = false;
                return mValue.Accept(aChar);
            }

            NumericType Get() const{
                return static_cast<NumericType>(mValue.Get()) * static_cast<NumericType>(mSign);
            }
        };

        template<class Body, class Decimal>
        class DecimalValue{
        public:
            typedef double NumericType;
        private:
            Body mBody;
            Decimal mDecimal;
            bool mDecimalFlag;
        public:
            DecimalValue():
                mBody(),
                mDecimal(),
                mDecimalFlag(false)
            {}

            uint8_t PrecedingZeros() const{
                return mBody.PrecedingZeros();
            }

            // Inherited from Value

            void Reset(){
                mBody.Reset();
                mDecimal.Reset();
                mDecimalFlag = false;
            }

            bool Accept(const char aChar){
                if(aChar == '.' && ! mDecimalFlag){
                    mDecimalFlag = true;
                    return true;
                }

                return mDecimalFlag ? mDecimal.Accept(aChar) : mBody.Accept(aChar);
            }

            NumericType Get() const{
                if(mDecimalFlag){
                    double decimal = mDecimal.Get();
                    while(decimal >= 1.0 && decimal != 0.0){
                        decimal /= 10.0;
                    }
                    int8_t zeros = mDecimal.PrecedingZeros();
                    while(zeros > 0 && decimal != 0.0){
                        decimal /= 10.0;
                        --zeros;
                    }
                    return mBody.Get() + decimal;
                }else{
                    return mBody.Get();
                }
            }
        };

        template<class Body, class Exponent>
        class ExponentValue{
        public:
            typedef typename Body::NumericType NumericType;
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

            uint8_t PrecedingZeros() const{
                return mBody.PrecedingZeros();
            }

            // Inherited from Value

            void Reset(){
                mBody.Reset();
                mExponent.Reset();
                mExponentFlag = 0;
                mExponentLastFlag = 0;
            }

            bool Accept(const char aChar){
                if((aChar == 'e' || aChar == 'E') && ! mExponentFlag){
                    mExponentFlag = 1;
                    mExponentLastFlag = 1;
                    return true;
                }else if(aChar == '+' && mExponentLastFlag){
                    mExponentLastFlag = 0;
                    return true;
                }
                mExponentLastFlag = 0;

                return mExponentFlag ? mExponent.Accept(aChar) : mBody.Accept(aChar);
            }

            NumericType Get() const{
                if(mExponentFlag){
                    return mBody.Get() * static_cast<NumericType>(std::pow(10.0, mExponent.Get()));
                }else{
                    return mBody.Get();
                }
            }
        };

        StringFragment::Pointer ToString(StringFragment::Pointer aBegin, double aValue){
            const auto ParseUint = [](char* const aBegin, uint32_t aValue)->char*{
                uint8_t digit;
                uint8_t count = 0;

                if(aValue == 0){
                    aBegin[count++] = '0';
                }else{
                    while(aValue > 0){
                        digit = (aValue % 10);
                        aBegin[count++] = '0' + digit;
                        aValue /= 10;
                    }
                }

                std::reverse(aBegin, aBegin + count);

                return aBegin + count;
            };

            int32_t body = static_cast<int32_t>(aValue);
            const bool negative = body < 0.0;
            double decimal = aValue - body;

            if(negative){
                body *= -1;
                *aBegin = '-';
                ++aBegin;
            }

            aBegin = ParseUint(aBegin, static_cast<uint32_t>(body));

            if(decimal != 0.0){
                for(uint8_t i = 0; i < 8; ++i){
                    if(decimal - std::floor(decimal) > 0.01){
                         decimal *= 10.0;
                    }else{
                        break;
                    }
                }
                *aBegin = '.';
                ++aBegin;
                aBegin = ParseUint(aBegin, static_cast<uint32_t>(decimal));
            }

            return aBegin;

            // Shift the body down

            /*if(body == 0.0){
                *aBegin = '0';
                ++aBegin;
            }else{
                while(body >= 1.0){
                     body /= 10.0;
                }

                if(negative){
                    *aBegin = '-';
                    ++aBegin;
                }

                while(body != 0.0){
                    body *= 10;
                    const double digit = std::floor(body);
                    body -= digit;
                    *aBegin = '0' + digit;
                    ++aBegin;
                }
            }

            if(decimal != 0.0){
                *aBegin = '.';
                ++aBegin;
            }

            while(decimal != 0.0){
                decimal *= 10;
                const double digit = std::floor(decimal);
                decimal -= digit;
                *aBegin = '0' + digit;
                ++aBegin;
            }*/
        }


    }

    typedef NumericParse::ExponentValue<NumericParse::UnsignedValue, NumericParse::UnsignedValue> NumericParserU8;
    typedef NumericParserU8 NumericParserU16;
    typedef NumericParserU8 NumericParserU32;
    typedef NumericParserU8 NumericParserU64;
    typedef NumericParse::ExponentValue<NumericParse::SignedValue, NumericParse::SignedValue> NumericParserI8;
    typedef NumericParserI8 NumericParserI16;
    typedef NumericParserI8 NumericParserI32;
    typedef NumericParserI8 NumericParserI64;
    typedef NumericParse::ExponentValue<NumericParse::DecimalValue<NumericParse::SignedValue, NumericParse::UnsignedValue>, NumericParse::DecimalValue<NumericParse::SignedValue, NumericParse::UnsignedValue>> NumericParserF;
    typedef NumericParserF NumericParserD;

    template<class T> struct NumericParser{typedef void Type;};
    template<> struct NumericParser<uint8_t>{typedef NumericParserU8 Type;};
    template<> struct NumericParser<uint16_t>{typedef NumericParserU16 Type;};
    template<> struct NumericParser<uint32_t>{typedef NumericParserU32 Type;};
    template<> struct NumericParser<uint64_t>{typedef NumericParserU64 Type;};

    template<> struct NumericParser<int8_t>{typedef NumericParserI8 Type;};
    template<> struct NumericParser<int16_t>{typedef NumericParserI16 Type;};
    template<> struct NumericParser<int32_t>{typedef NumericParserI32 Type;};
    template<> struct NumericParser<int64_t>{typedef NumericParserI64 Type;};
    template<> struct NumericParser<float>{typedef NumericParserF Type;};
    template<> struct NumericParser<double>{typedef NumericParserD Type;};

    template<class T, class Iterator>
    T ParseNumber(const Iterator aBegin, const Iterator aEnd, Iterator& aParseEnd){
        typename NumericParser<T>::Type parser;

        aParseEnd = aBegin;
        while(aParseEnd != aEnd){

            if(! parser.Accept(*aParseEnd)){
                break;
            }
        }

        try{
            const T tmp = parser.Get();
            return tmp;
        }catch(...){
            aParseEnd = aBegin;
            return static_cast<T>(0);
        }
    }

}


#endif
