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

	template<class T>
	class NumberParser{
	private:

		static_assert(std::is_arithmetic<T>::value, "Solaire::Utility::NumberParser Parsed type must be a number");

		static bool CanHaveDecimal(){
			return ! std::is_integral<T>::value;
		}

		static bool CanHaveExponent() {
			return true;
		}

		static bool CanBeNegative() {
			return ! std::is_unsigned<T>::value;
		}

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

		class Section{
		private:
			static void AddDigit(T& aValue, const char aCharacter, const size_t aOffset) {
				aValue += static_cast<int64_t>(aCharacter - '0') * static_cast<T>(std::pow(10, aOffset));
			}
			
			Section(const Section&) = delete;
			Section(Section&&) = delete;

			Section& operator=(const Section&) = delete;
			Section& operator=(Section&&) = delete;

			std::vector<char> mChars;
			bool mIsPositive;
		public:
			operator T() const{
				T value = static_cast<T>(0);

				const auto end = mChars.rend();
				const auto begin = mChars.rbegin();
				for(auto i = begin; i != end; ++i){
					AddDigit(value, *i, i - begin);
				}

				return mIsPositive ? value : value * static_cast<T>(-1);
			}

			Section& operator+=(const char aChar){
				mChars.push_back(aChar);
				return *this;
			}

			void SetSign(const bool aSign){
				mIsPositive = aSign;
			}

			bool GetSign() const{
				return mIsPositive;
			}

			void Clear(){
				mChars.clear();
				mIsPositive = true;
			}

			Section() :
				mChars(16),
				mIsPositive(true)
			{}
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
					aSection.SetSign(true);
					return i + 1;
				}else if(c >= '0' && c <= '9'){
					aSection.SetSign(true);
					return i;
				}else if(c == '-'){
					aSection.SetSign(false);
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
				CanHaveExponent() ? STATE_FIND_EXPONENT_SIGN : STATE_ERROR, 
				CanHaveDecimal() ? STATE_FIND_DECIMAL_VALUE : STATE_ERROR,
				STATE_RETURN, 
				mBody, 
				&mDecimal
			);
		}

		void StateFindDecimalValue(){
			GenericFindValueState(
				CanHaveExponent() ? STATE_FIND_EXPONENT_SIGN : STATE_ERROR,
				STATE_ERROR, 
				STATE_RETURN,
				mDecimal, 
				nullptr
			);
		}

		void StateFindExponentValue(){
			GenericFindValueState(
				STATE_ERROR,
				CanHaveDecimal() ? STATE_FIND_EXPONENT_DECIMAL : STATE_ERROR,
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
				mState = (! aSection.GetSign()) && (! CanBeNegative()) ? STATE_ERROR : aValueState;
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

		static T ConstructBody(const T aBody, const T aDecimal){
			if(CanHaveDecimal()){
				// Shift the decimal value down
				double decimal = static_cast<double>(aDecimal);
				while(decimal >= 1.0) decimal /= 10.0;

				// Combine the values
				if(aBody < static_cast<T>(0)) decimal *= -1.0;
				return static_cast<T>(static_cast<double>(aBody) + decimal);
			}else{
				return static_cast<T>(aBody);
			}
		}

		void StateReturn(){
			const T body = ConstructBody(mBody, mDecimal);
			const T exponent = CanHaveExponent() ? ConstructBody(mExponent, mExponentDecimal) : static_cast<T>(0);
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
			mReturnValue = 0.f;
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
