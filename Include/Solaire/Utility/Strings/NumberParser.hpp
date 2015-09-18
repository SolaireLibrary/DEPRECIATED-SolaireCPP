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

namespace Solaire { namespace Utility {

	class ParseNumberClass{
	private:
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

		struct Section{
			const char* begin;
			int64_t value;
			bool isNegative;

			double ConstructValue() const{
				return static_cast<double>(value) * isNegative ? -1.f : 1.f;
			}

			size_t CountDigits() const{
				int64_t value = this->value;
				size_t digits = 1;
				while(value != 0){
					value -= 10 * digits;
					++digits;
				}
				return digits;
			}
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

		enum Sign : uint32_t{
			SIGN_POSITIVE,
			SIGN_NEGATIVE,
			SIGN_INVALID
		};

		static Sign EvaluateSign(const char aCharacter){
			if(aCharacter == '+' || (aCharacter >= '0' && aCharacter <= '9')){
				return SIGN_POSITIVE;
			}else if(aCharacter == '-'){
				return SIGN_NEGATIVE;
			}else{
				return SIGN_INVALID;
			}
		}

		static const char* FindSign(bool& aSign, const char* const aBegin, const char* aEnd){
			const char* i = aBegin;
			Sign mode = SIGN_INVALID;

			while(i < aEnd && mode == SIGN_INVALID){
				mode = EvaluateSign(*i);
				++i;
			}

			if (mode == SIGN_INVALID)  throw std::runtime_error("Could not determine sign of string " + std::string(aBegin + (aEnd - aBegin)));
			aSign = mode == SIGN_POSITIVE;
			return --i;
		}

		static bool AddDigit(int64_t& aValue, const char aCharacter, const size_t aOffset) {
			if(aCharacter >= '0' && aCharacter <= '9'){
				aValue += static_cast<int64_t>(aCharacter - '0') * 10 * aOffset;
				return true;
			}else{
				return false;
			}
		}

		void GenericFindValueState(const State aIfExponent, const State aIfDecimal, const State aIfInvalidChar, Section& aBodySection, Section* aDecimalSection) {
			const char c = *mCurrentChar;
			if(c == 'e' || c == 'E'){
				if(aIfExponent != STATE_ERROR && aIfExponent != STATE_RETURN) ++mCurrentChar;
				mState = aIfExponent;
			}else if (c == '.'){
				if(aIfDecimal != STATE_ERROR){
					++mCurrentChar;
					aDecimalSection->begin = mCurrentChar;
				}
				mState = aIfDecimal;
			}else{
				// Calculate the offset from the beginning of the body
				const size_t offset = (mCurrentChar - aBodySection.begin) + 1;
				if (AddDigit(aBodySection.value, c, offset)) {
					++mCurrentChar;
				}else{
					--mCurrentChar;
					mState = aIfInvalidChar;
				}
			}
		}

		void StateFindValue(){
			GenericFindValueState(STATE_FIND_EXPONENT_SIGN, STATE_FIND_DECIMAL_VALUE, STATE_RETURN, mBody, &mDecimal);
		}

		void StateFindDecimalValue(){
			GenericFindValueState(STATE_FIND_EXPONENT_SIGN, STATE_ERROR, STATE_ERROR, mDecimal, nullptr);
		}

		void StateFindExponentValue(){
			GenericFindValueState(STATE_ERROR, STATE_FIND_EXPONENT_DECIMAL, STATE_RETURN, mExponent, &mExponentDecimal);
		}

		void StateFindExponentDecimal(){
			GenericFindValueState(STATE_FIND_EXPONENT_SIGN, STATE_ERROR, STATE_RETURN, mExponentDecimal, nullptr);
		}

		void GenericFindSign(const State aValueState, Section& aSection){
			try{
				mCurrentChar = FindSign(aSection.isNegative, mCurrentChar, mEnd);
				++mCurrentChar;
				aSection.begin = mCurrentChar;

				mState = aValueState;
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

		void StateReturn(){
			//! \TODO implement
		}

		void StateError(){
			//! \TODO implement
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
	};
}}


#endif
