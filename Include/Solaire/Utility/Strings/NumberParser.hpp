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

#include "StringFragment.hpp"

namespace Solaire { namespace Utility {

	namespace NumberParserInternals{

		enum {
			MAX_DIGITS = 11
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
		public:
			Section();

			operator int64_t() const;
			Section& operator+=(const char aChar);

			void SetSign(const Sign aSign);
			Sign GetSign() const;
			void Clear();
		};
	}

	template<class T>
	class NumberParser{
	private:
		typedef NumberParserInternals::Section Section;

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

		void GenericFindValueState(const State aIfExponent, const State aIfDecimal, const State aIfInvalidChar, Section& aBodySection, Section* aDecimalSection);
		void GenericFindSign(const State aValueState, Section& aSection);

		void StateFindValue();
		void StateFindDecimalValue();
		void StateFindExponentValue();
		void StateFindExponentDecimal();
		void StateFindSign();
		void StateFindExponentSign();
		void StateReturn();
		void StateError();
		void EvaluateState();

		void Clear();
	public:
		std::pair<T, const char*> operator()(const char* const aBegin, const char* const aEnd);
		std::pair<T, const char*> operator()(const char* const aBegin, const size_t aSize);
		std::pair<T, const char*> operator()(const ConstStringFragment aString);
		std::pair<T, const char*> operator()(const std::string& aString);
	};
}}

#include "NumberParser.inl"

#endif
