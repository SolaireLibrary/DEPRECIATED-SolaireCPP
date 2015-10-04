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

#include "..\Streams\StringParser.hpp"
#include "..\Strings\StringFragment.hpp"
#include "..\DataStructures\DynamicArray.hpp"

namespace Solaire{

    namespace NumericParse{

        class UnsignedValue : public StringParser<uint32_t>{
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

            void Reset() override{
                mStatus = STATUS_FAIL;
                StringParser::Reset();
                mCount = 0;
            }

            Status Accept(const Flags aFlags, const char aChar) override{
                if(aChar < '0' || aChar > '9') return (mCount > 0 ? ByteParser::STATUS_COMPLETE : ByteParser::STATUS_FAIL);
                mChars[mCount++] = aChar;
                return ByteParser::STATUS_SUCCESS;
            }

            NumericType Get(Allocator& aParseAllocator, Allocator& aDataAllocator) const override{
                if(GetStatus() == ByteParser::STATUS_FAIL) throw std::runtime_error("NumericParser : Failed to parse unsigned value");
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

            Status GetStatus() const override{
                return mStatus;
            }
        };

        class SignedValue : public StringParser<int32_t>{
        public:
            typedef int32_t NumericType;
        private:
            UnsignedValue mValue;
            int8_t mSign;
        public:
            SignedValue():
                mValue(),
                mSign(1)
            {}

            uint8_t PrecedingZeros() const{
                return mValue.PrecedingZeros();
            }

            // Inherited from Value

            void Reset() override{
                StringParser::Reset();
                mValue.Reset();
            }

            Status Accept(const Flags aFlags, const char aChar) override{
                if(aFlags & FLAG_IS_FIRST_CHAR){
                     if(aChar == '-'){
                        mSign = -1;
                        return ByteParser::STATUS_SUCCESS;
                    }else{
                        mSign = 1;
                    }
                }
                return static_cast<ByteParser&>(mValue).Accept(static_cast<uint8_t>(aChar));
            }

            NumericType Get(Allocator& aParseAllocator, Allocator& aDataAllocator) const override{
                if(GetStatus() == ByteParser::STATUS_FAIL) throw std::runtime_error("NumericParser : Failed to parse signed value");
                return static_cast<NumericType>(mValue.Get(aParseAllocator, aDataAllocator)) * static_cast<NumericType>(mSign);
            }

            Status GetStatus() const override{
                return mValue.GetStatus();
            }
        };

        template<class Body, class Decimal>
        class DecimalValue : public StringParser<double>{
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

            void Reset() override{
                StringParser::Reset();
                mBody.Reset();
                mDecimal.Reset();
                mDecimalFlag = false;
            }

            Status Accept(const Flags aFlags, const char aChar) override{
                if(aChar == '.' && ! mDecimalFlag){
                    mDecimalFlag = true;
                    return ByteParser::STATUS_SUCCESS;
                }

                return mDecimalFlag ?
                    static_cast<ByteParser&>(mDecimal).Accept(static_cast<uint8_t>(aChar)) :
                    static_cast<ByteParser&>(mBody).Accept(static_cast<uint8_t>(aChar));
            }

            NumericType Get(Allocator& aParseAllocator, Allocator& aDataAllocator) const override{
                if(mDecimalFlag){
                    double decimal = mDecimal.Get(aParseAllocator, aDataAllocator);
                    while(decimal >= 1.0 && decimal != 0.0){
                        decimal /= 10.0;
                    }
                    int8_t zeros = mDecimal.PrecedingZeros();
                    while(zeros > 0 && decimal != 0.0){
                        decimal /= 10.0;
                        --zeros;
                    }
                    return mBody.Get(aParseAllocator, aDataAllocator) + decimal;
                }else{
                    return mBody.Get(aParseAllocator, aDataAllocator);
                }
            }

            Status GetStatus() const override{
                if(mDecimalFlag){
                    return mBody.GetStatus() == ByteParser::STATUS_FAIL ? ByteParser::STATUS_FAIL : mDecimal.GetStatus();
                }else{
                    return mBody.GetStatus();
                }
            }
        };

        template<class Body, class Exponent>
        class ExponentValue : public StringParser<typename Body::NumericType>{
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

            void Reset() override{
                StringParser<typename Body::NumericType>::Reset();
                mBody.Reset();
                mExponent.Reset();
                mExponentFlag = 0;
                mExponentLastFlag = 0;
            }

            ByteParser::Status Accept(const uint16_t aFlags, const char aChar) override{
                if((aChar == 'e' || aChar == 'E') && ! mExponentFlag){
                    mExponentFlag = 1;
                    mExponentLastFlag = 1;
                    return ByteParser::STATUS_SUCCESS;
                }else if(aChar == '+' && mExponentLastFlag){
                    mExponentLastFlag = 0;
                    return ByteParser::STATUS_SUCCESS;
                }
                mExponentLastFlag = 0;

                return mExponentFlag ?
                    static_cast<ByteParser&>(mExponent).Accept(static_cast<uint8_t>(aChar)) :
                    static_cast<ByteParser&>(mBody).Accept(static_cast<uint8_t>(aChar));
            }

            NumericType Get(Allocator& aParseAllocator, Allocator& aDataAllocator) const override{
                if(mExponentFlag){
                    return mBody.Get(aParseAllocator, aDataAllocator) * static_cast<NumericType>(std::pow(10.0, mExponent.Get(aParseAllocator, aDataAllocator)));
                }else{
                    return mBody.Get(aParseAllocator, aDataAllocator);
                }
            }

            ByteParser::Status GetStatus() const override{
                if(mExponentFlag){
                    return mBody.GetStatus() == ByteParser::STATUS_FAIL ? ByteParser::STATUS_FAIL : mExponent.GetStatus();
                }else{
                    return mBody.GetStatus();
                }
            }
        };

        StringFragment::Pointer ToString(StringFragment::Pointer aBegin, double aValue){
            // Separate aValue into components
            double body = std::floor(aValue);
            const bool negative = body < 0.0;
            double decimal = aValue - body;
            if(negative) body *= -1.0;

            // Shift the body down
            while(body < 0.0 && body != 0.0) body /= 10.0;

            if(negative){
                *aBegin = '-';
                ++aBegin;
            }

            if(body == 0.0){
                *aBegin = '0';
                ++aBegin;
            }

            while(body != 0.0){
                body *= 10;
                const double digit = std::floor(body);
                body -= digit;
                *aBegin = '0' + digit;
                ++aBegin;
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
            }

            return aBegin;
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

}


#endif
