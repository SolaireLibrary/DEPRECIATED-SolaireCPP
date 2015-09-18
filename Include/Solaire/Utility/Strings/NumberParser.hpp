#ifndef SOLAIRE_UTILITY_NUMBER_PARSER_HPP
#define SOLAIRE_UTILITY_NUMBER_PARSER_HPP

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
Created			: 18th September 2015
Last Modified	: 18th September 2015
*/

#include <vector>
#include <type_traits>
#include "StringFragment.hpp"

namespace Solaire { namespace Utility {

	namespace NumberParserInternals{

		enum {
			MAX_DIGITS = 11
		};

		static constexpr const uint32_t POWERS_10[MAX_DIGITS] = {
			1,
			10,
			100,
			1000,
			10000,
			100000,
			1000000,
			10000000,
			100000000,
			1000000000
		};

		class Section {
		public:
			enum Sign : bool{
				SIGN_NEGATIVE,
				SIGN_POSITIVE
			};
		private:
			char mStack[MAX_DIGITS];
			struct{
				uint8_t mHead : 4;
				uint8_t mSign : 1;
			};
			static_assert(MAX_DIGITS < 16, "NumberParser::Section Stack head compression failed");
		public:
			operator int64_t() const{
				int64_t value = 0;

				const int32_t end = -1;
				for(int32_t i = mHead - 1; i != end; --i) {
					value += static_cast<int64_t>(mStack[i] - '0') * static_cast<int64_t>(POWERS_10[10, mHead - i]);
				}

				return mSign == SIGN_POSITIVE ? value : value * -1;
			}

			Section& operator+=(const char aChar){
				if(mHead == MAX_DIGITS) throw std::runtime_error("NumberParser::Section Maximum digits exceeded");
				mStack[mHead++] = aChar;
				return *this;
			}

			void SetSign(const Sign aSign){
				mSign = aSign;
			}

			Sign GetSign() const{
				return static_cast<Sign>(mSign);
			}

			void Clear(){
				mHead = 0;
				mSign = 1;
			}

			Section() :
				mHead(0),
				mSign(SIGN_POSITIVE)
			{}
		};
	}

	template<class T>
	class NumberParser{
	private:
		static_assert(std::is_arithmetic<T>::value, "Solaire::Utility::NumberParser Parsed type must be a number");

		typedef NumberParserInternals::Section Section;

		enum Traits : bool{
			HAS_DECIMAL = !std::is_integral<T>::value,
			HAS_EXPONENT = true,
			CAN_BE_NEGATIVE = std::is_unsigned<T>::value
		};

		// The states for the parsing FSA
		enum State{
			STATE_FIND_SIGN,					// Initial state, determines if the value is positive or negative
			STATE_FIND_VALUE,					// Looking for the body of the value
			STATE_FIND_DECIMAL_VALUE,			// Looking for the decimal places in the value
			STATE_FIND_EXPONENT_SIGN,			// Determines if an exponent is in the form eN, e+N or e-N
			STATE_FIND_EXPONENT_VALUE,			// Looking for the exponent value
			STATE_FIND_EXPONENT_DECIMAL,
			STATE_RETURN,						// Final parsing of the value for return
			STATE_ERROR
		};

		Section mBody;
		Section mDecimal;
		Section mExponent;
		Section mExponentDecimal;

		const char* mCurrentChar;				// The current character
		const char* mBegin;
		const char* mEnd;

		uint32_t mDecimalPlaces;				// Stores how many decimal places are in the decimal value
		State mState;							// The current FSA state
		T mReturnValue;

		static const char* FindSign(Section& aSection, const char* const aBegin, const char* aEnd){
			const char* i = aBegin;

			while(i < aEnd){
				const char c = *i;
				if(c == '+'){
					aSection.SetSign(Section::SIGN_POSITIVE);
					return i + 1;
				}else if(c >= '0' && c <= '9'){
					aSection.SetSign(Section::SIGN_POSITIVE);
					return i;
				}else if(c == '-'){
					aSection.SetSign(Section::SIGN_NEGATIVE);
					return i + 1;
				}else{
					throw std::runtime_error("Could not determine sign");
				}
				++i;
			}
			throw std::runtime_error("Could not determine sign");
		}

		void GenericFindValueState(const State aIfExponent, const State aIfDecimal, const State aIfInvalidChar, Section& aBodySection, Section* aDecimalSection) {
			const char c = *mCurrentChar;
			if(c == 'e' || c == 'E'){
				if(aIfExponent != STATE_ERROR && aIfExponent != STATE_RETURN) ++mCurrentChar;
				mState = aIfExponent;
			}else if (c == '.'){
				if(aIfDecimal != STATE_ERROR){
					++mCurrentChar;
				}
				mState = aIfDecimal;
			}else{
				if(c >= '0' && c <= '9'){
					aBodySection += c;
					++mCurrentChar;
				}else{
					--mCurrentChar;
					mState = aIfInvalidChar;
				}
			}
		}

		void StateFindValue(){
			GenericFindValueState(
				HAS_EXPONENT ? STATE_FIND_EXPONENT_SIGN : STATE_ERROR,
				HAS_EXPONENT ? STATE_FIND_DECIMAL_VALUE : STATE_ERROR,
				STATE_RETURN, 
				mBody, 
				&mDecimal
			);
		}

		void StateFindDecimalValue(){
			GenericFindValueState(
				HAS_EXPONENT ? STATE_FIND_EXPONENT_SIGN : STATE_ERROR,
				STATE_ERROR, 
				STATE_RETURN,
				mDecimal, 
				nullptr
			);
		}

		void StateFindExponentValue(){
			GenericFindValueState(
				STATE_ERROR,
				HAS_DECIMAL ? STATE_FIND_EXPONENT_DECIMAL : STATE_ERROR,
				STATE_RETURN,
				mExponent,
				&mExponentDecimal
			);
		}

		void StateFindExponentDecimal(){
			GenericFindValueState(
				STATE_FIND_EXPONENT_SIGN, 
				STATE_ERROR, 
				STATE_RETURN, 
				mExponentDecimal, 
				nullptr
			);
		}

		void GenericFindSign(const State aValueState, Section& aSection){
			try{
				mCurrentChar = FindSign(aSection, mCurrentChar, mEnd);
				mState = (! aSection.GetSign()) && (! CAN_BE_NEGATIVE) ? STATE_ERROR : aValueState;
			}catch(...){
				mState = STATE_ERROR;
			}
		}

		void StateFindSign(){
			GenericFindSign(STATE_FIND_VALUE, mBody);
		}

		void StateFindExponentSign(){
			GenericFindSign(STATE_FIND_EXPONENT_VALUE, mExponent);
		}

		static T ConstructBody(const int64_t aBody, const int64_t aDecimal){
			if(HAS_DECIMAL){
				// Shift the decimal value down
				double decimal = static_cast<double>(aDecimal);
				while(decimal >= 1.0) decimal /= 10.0;

				// Combine the values
				if(aBody < 0) decimal *= -1.0;
				return static_cast<T>(static_cast<double>(aBody) + decimal);
			}else{
				return static_cast<T>(aBody);
			}
		}

		void StateReturn(){
			const T body = ConstructBody(mBody, mDecimal);
			const T exponent = HAS_EXPONENT ? ConstructBody(mExponent, mExponentDecimal) : static_cast<T>(0);
			mReturnValue = exponent == static_cast<T>(0) ? body : body * static_cast<T>(std::pow(10, exponent));
		}

		void StateError(){
			
		}

		void EvaluateState(){
			switch (mState)
			{
			case STATE_FIND_SIGN:
				StateFindSign();
				break;
			case STATE_FIND_VALUE:
				StateFindValue();
				break;
			case STATE_FIND_DECIMAL_VALUE:
				StateFindDecimalValue();
				break;
			case STATE_FIND_EXPONENT_SIGN:
				StateFindExponentSign();
				break;
			case STATE_FIND_EXPONENT_VALUE:
				StateFindExponentValue();
				break;
			case STATE_FIND_EXPONENT_DECIMAL:
				StateFindExponentDecimal();
				break;
			case STATE_RETURN:
				StateReturn();
				break;
			default:
				StateError();
			}
		}

		void Clear() {
			mState = STATE_FIND_SIGN;
			mBegin = nullptr;
			mEnd = nullptr;
			mReturnValue = static_cast<T>(0);
			mBody.Clear();
			mDecimal.Clear();
			mExponent.Clear();
			mExponentDecimal.Clear();
		}
	public:
		std::pair<T, const char*> operator()(const char* const aBegin, const char* const aEnd){
			// Initialise
			Clear();
			mBegin = aBegin;
			mEnd = aEnd;
			mCurrentChar = mBegin;

			// FSA loop
			do{
				EvaluateState();
			}while(mState != STATE_ERROR && mState != STATE_RETURN);

			EvaluateState();

			// Determine output of FSA
			return std::pair<T, const char*>(mReturnValue, mState == STATE_ERROR ?  nullptr : mCurrentChar);
		}

		std::pair<T, const char*> operator()(const char* const aBegin, const size_t aSize) {
			return operator()(aBegin, aBegin + aSize);
		}

		std::pair<T, const char*> operator()(const ConstStringFragment aString){
			return operator()(aString.begin(), aString.Size());
		}

		std::pair<T, const char*> operator()(const std::string& aString){
			return operator()(aString.c_str(), aString.size());
		}
	};
}}


#endif
