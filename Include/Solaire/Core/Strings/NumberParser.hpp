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

namespace Solaire{ namespace Core{

    namespace NumericParse{

        class Value{
        public:
            virtual ~Value(){

            }

            virtual bool Append(const char aChar) = 0;
            virtual void Reset() = 0;
            virtual double Get() const = 0;

            template<class T>
            std::pair<T, ConstStringFragment> Parse(const ConstStringFragment aFragment){
                std::pair<double, ConstStringFragment> tmp = Parse<double>(aFragment);
                return std::pair<T, ConstStringFragment>(static_cast<T>(tmp.first), tmp.second);
            }
        };

        template<>
        std::pair<double, ConstStringFragment> Value::Parse<double>(const ConstStringFragment aFragment){
            Reset();
            ConstStringFragment::ConstIterator i = aFragment.begin();
            const ConstStringFragment::ConstIterator end = aFragment.end();
            for(i; i != end; ++i){
                if(! Append(*i)) break;
            }
            return std::pair<double, ConstStringFragment>(Get(), ConstStringFragment(aFragment.begin(), i));
        }


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
                    }
                    return true;
                }
                return false;
            }

            double Get() const override{
                return mValue.Get() * (mSign ? 1.0 : -1.0);
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
                if(aChar == '.' && ! mDecimalFlag){
                    mDecimalFlag = 1;
                    return true;
                }

                return mDecimalFlag ? mDecimal.Append(aChar) : mBody.Append(aChar);
            }

            double Get() const override{
                double decimal = mDecimal.Get();
                while(decimal > 1.0 && decimal != 0.0) decimal /= 10.0;
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
                if((aChar == 'e' || aChar == 'E') && ! mExponentFlag){
                    mExponentFlag = 1;
                    mExponentLastFlag = 1;
                    return true;
                }else if(aChar == '+' && mExponentLastFlag){
                    mExponentLastFlag = 0;
                    return true;
                }
                mExponentLastFlag = 0;
                return mExponentFlag ? mExponent.Append(aChar) : mBody.Append(aChar);
            }

            double Get() const override{
                return mBody.Get() * std::pow(10.0, mExponent.Get());
            }
        };

        template<class T>
        static std::pair<T, ConstStringFragment> Parse(const ConstStringFragment aFragment) = delete;

        template<>
        std::pair<uint8_t, ConstStringFragment> Parse<uint8_t>(const ConstStringFragment aFragment){
            return ExponentValue<UnsignedValue, UnsignedValue>().Parse<uint8_t>(aFragment);
        }

        template<>
        std::pair<uint16_t, ConstStringFragment> Parse<uint16_t>(const ConstStringFragment aFragment){
            return ExponentValue<UnsignedValue, UnsignedValue>().Parse<uint16_t>(aFragment);
        }

        template<>
        std::pair<uint32_t, ConstStringFragment> Parse<uint32_t>(const ConstStringFragment aFragment){
            return ExponentValue<UnsignedValue, UnsignedValue>().Parse<uint32_t>(aFragment);
        }

        template<>
        std::pair<uint64_t, ConstStringFragment> Parse<uint64_t>(const ConstStringFragment aFragment){
            return ExponentValue<UnsignedValue, UnsignedValue>().Parse<uint64_t>(aFragment);
        }

        template<>
        std::pair<int8_t, ConstStringFragment> Parse<int8_t>(const ConstStringFragment aFragment){
            return ExponentValue<SignedValue, SignedValue>().Parse<int8_t>(aFragment);
        }

        template<>
        std::pair<int16_t, ConstStringFragment> Parse<int16_t>(const ConstStringFragment aFragment){
            return ExponentValue<SignedValue, SignedValue>().Parse<int16_t>(aFragment);
        }

        template<>
        std::pair<int32_t, ConstStringFragment> Parse<int32_t>(const ConstStringFragment aFragment){
            return ExponentValue<SignedValue, SignedValue>().Parse<int32_t>(aFragment);
        }

        template<>
        std::pair<int64_t, ConstStringFragment> Parse<int64_t>(const ConstStringFragment aFragment){
            return ExponentValue<SignedValue, SignedValue>().Parse<int64_t>(aFragment);
        }

        template<>
        std::pair<float, ConstStringFragment> Parse<float>(const ConstStringFragment aFragment){
            return ExponentValue<DecimalValue<SignedValue, UnsignedValue>, DecimalValue<SignedValue, UnsignedValue>>().Parse<float>(aFragment);
        }

        template<>
        std::pair<double, ConstStringFragment> Parse<double>(const ConstStringFragment aFragment){
            return ExponentValue<DecimalValue<SignedValue, UnsignedValue>, DecimalValue<SignedValue, UnsignedValue>>().Parse<double>(aFragment);
        }

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

}}


#endif
