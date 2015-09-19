#ifndef SOLAIRE_UTILITY_NUMBER_PARSER_INL
#define SOLAIRE_UTILITY_NUMBER_PARSER_INL

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
\file NumberParser.inl
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 18th September 2015
Last Modified	: 18th September 2015
*/

#include <type_traits>
#include "NumberParser.hpp"

#define SOLAIRE_INL_HAS_DECIMAL (! std::is_integral<T>::value)
#define SOLAIRE_INL_HAS_EXPONENT true
#define SOLAIRE_INL_CAN_BE_NEGATIVE (! std::is_unsigned<T>::value)

namespace Solaire { namespace Utility {

	namespace NumberParserInternals{
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

		// Section

		static_assert(MAX_DIGITS < 16, "NumberParser::Section Stack head compression failed");

		Section::Section() :
			mHead(0),
			mSign(SIGN_POSITIVE)
		{}

		Section::operator int64_t() const{
			int64_t value = 0;

			const int32_t end = -1;
			for(int32_t i = mHead - 1; i != end; --i) {
				value += static_cast<int64_t>(mStack[i] - '0') * static_cast<int64_t>(POWERS_10[10, mHead - i]);
			}

			return mSign == SIGN_POSITIVE ? value : value * -1;
		}

		Section& Section::operator+=(const char aChar){
			if(mHead == MAX_DIGITS) throw std::runtime_error("NumberParser::Section Maximum digits exceeded");
			mStack[mHead++] = aChar;
			return *this;
		}

		void Section::SetSign(const Sign aSign){
			mSign = aSign;
		}

		Section::Sign Section::GetSign() const{
			return static_cast<Sign>(mSign);
		}

		void Section::Clear(){
			mHead = 0;
			mSign = 1;
		}

		template<class T>
		T ConstructBody(const int64_t aBody, const int64_t aDecimal){
			if(SOLAIRE_INL_HAS_DECIMAL){
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

		template<class T>
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
	}

	// NumberParser

	template<class T>
	void NumberParser<T>::GenericFindValueState(const State aIfExponent, const State aIfDecimal, const State aIfInvalidChar, Section& aBodySection, Section* aDecimalSection) {
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

	template<class T>
	void NumberParser<T>::StateFindValue(){
		GenericFindValueState(
			SOLAIRE_INL_HAS_EXPONENT ? STATE_FIND_EXPONENT_SIGN : STATE_ERROR,
			SOLAIRE_INL_HAS_EXPONENT ? STATE_FIND_DECIMAL_VALUE : STATE_ERROR,
			STATE_RETURN, 
			mBody, 
			&mDecimal
		);
	}

	template<class T>
	void NumberParser<T>::StateFindDecimalValue(){
		GenericFindValueState(
			SOLAIRE_INL_HAS_EXPONENT ? STATE_FIND_EXPONENT_SIGN : STATE_ERROR,
			STATE_ERROR, 
			STATE_RETURN,
			mDecimal, 
			nullptr
		);
	}

	template<class T>
	void NumberParser<T>::StateFindExponentValue(){
		GenericFindValueState(
			STATE_ERROR,
			SOLAIRE_INL_HAS_DECIMAL ? STATE_FIND_EXPONENT_DECIMAL : STATE_ERROR,
			STATE_RETURN,
			mExponent,
			&mExponentDecimal
		);
	}

	template<class T>
	void NumberParser<T>::StateFindExponentDecimal(){
		GenericFindValueState(
			STATE_FIND_EXPONENT_SIGN, 
			STATE_ERROR, 
			STATE_RETURN, 
			mExponentDecimal, 
			nullptr
		);
	}

	template<class T>
	void NumberParser<T>::GenericFindSign(const State aValueState, Section& aSection){
		try{
			mCurrentChar = NumberParserInternals::FindSign<T>(aSection, mCurrentChar, mEnd);
			mState = (! aSection.GetSign()) && (! SOLAIRE_INL_CAN_BE_NEGATIVE) ? STATE_ERROR : aValueState;
		}catch(...){
			mState = STATE_ERROR;
		}
	}

	template<class T>
	void NumberParser<T>::StateFindSign(){
		GenericFindSign(STATE_FIND_VALUE, mBody);
	}

	template<class T>
	void NumberParser<T>::StateFindExponentSign(){
		GenericFindSign(STATE_FIND_EXPONENT_VALUE, mExponent);
	}

	template<class T>
	void NumberParser<T>::StateReturn(){
		const T body = NumberParserInternals::ConstructBody<T>(mBody, mDecimal);
		const T exponent = SOLAIRE_INL_HAS_EXPONENT ? NumberParserInternals::ConstructBody<T>(mExponent, mExponentDecimal) : static_cast<T>(0);
		mReturnValue = exponent == static_cast<T>(0) ? body : body * static_cast<T>(std::pow(10, exponent));
	}

	template<class T>
	void NumberParser<T>::StateError(){
			
	}

	template<class T>
	void NumberParser<T>::EvaluateState(){
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

	template<class T>
	void NumberParser<T>::Clear(){
		static_assert(std::is_arithmetic<T>::value, "Solaire::Utility::NumberParser Parsed type must be a number");

		mState = STATE_FIND_SIGN;
		mEnd = nullptr;
		mReturnValue = static_cast<T>(0);
		mBody.Clear();
		mDecimal.Clear();
		mExponent.Clear();
		mExponentDecimal.Clear();
	}

	template<class T>
	std::pair<T, const char*> NumberParser<T>::operator()(const char* const aBegin, const char* const aEnd){
		// Initialise
		Clear();
		mEnd = aEnd;
		mCurrentChar = aBegin;

		// FSA loop
		do{
			EvaluateState();
		}while(mState != STATE_ERROR && mState != STATE_RETURN);

		EvaluateState();

		// Determine output of FSA
		return std::pair<T, const char*>(mReturnValue, mState == STATE_ERROR ?  nullptr : mCurrentChar);
	}

	template<class T>
	std::pair<T, const char*> NumberParser<T>::operator()(const char* const aBegin, const size_t aSize) {
		return operator()(aBegin, aBegin + aSize);
	}

	template<class T>
	std::pair<T, const char*> NumberParser<T>::operator()(const ConstStringFragment aString){
		return operator()(aString.begin(), aString.Size());
	}

	template<class T>
	std::pair<T, const char*> NumberParser<T>::operator()(const std::string& aString){
		return operator()(aString.c_str(), aString.size());
	}
}}

#undef SOLAIRE_INL_HAS_DECIMAL
#undef SOLAIRE_INL_HAS_EXPONENT
#undef SOLAIRE_INL_CAN_BE_NEGATIVE


#endif
