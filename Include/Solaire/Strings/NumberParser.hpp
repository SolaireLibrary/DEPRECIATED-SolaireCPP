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
//#include "..\Strings\StringFragment.hpp"
//#include "..\DataStructures\DynamicArray.hpp"

namespace Solaire {
	namespace Implementation {

		static const char* ParseUint(const char* const aBegin, const char* const aEnd, uint32_t& aValue, uint32_t& aZeros) {
			const char* i = aBegin;
			aZeros = 0;
			bool nonZero = false;

			while(i != aEnd) {
				const char c = *i;
				if(c == '0') {
					if (! nonZero) ++aZeros;
				}else if(c >= '1' && c <= '9') {
					nonZero = true;
				}else {
					break;
				}
				++i;
			}

			if(i == aBegin) return aBegin;

			const uint32_t offset = i - aBegin;
			if(offset == 0) return aBegin;
			aValue = 0;

			uint32_t index = 0;
			const char* j = i - 1;
			const char* const end = aBegin - 1;
			while (j != end) {
				aValue += static_cast<uint32_t>(std::pow(10, index)) * (*j - '0');
				++index;
				--j;
			}
			return i;
		}

		static const char* ParseInt(const char* const aBegin, const char* const aEnd, int32_t& aValue, uint32_t& aZeros) {
			int32_t sign = 1;
			uint32_t tmp;
			const char* begin = aBegin;

			if(*begin == '-') {
				++begin;
				sign = -1;
			}

			const char* const end = ParseUint(begin, aEnd, tmp, aZeros);
			if(end != begin) {
				aValue = static_cast<int32_t>(tmp) * sign;
			}
			return end;
		}

		template<
			class T1, 
			class T2, 
			class R, 
			const char*(*F1)(const char* const, const char* const, T1&, uint32_t&),
			const char*(*F2)(const char* const, const char* const, T1&, uint32_t&)
		>
		static const char* ParseExponent(const char* const aBegin, const char* const aEnd, R& aValue, uint32_t& aZeros) {
			T1 tmp1;
			T2 tmp2;
			uint32_t zero2;

			const char* end = F1(aBegin, aEnd, tmp1, aZeros);
			if(end == aBegin) return aBegin;

			if(end != aEnd && (*end == 'e' || *end == 'E')) {
				++end;
				if(*end == '+') ++end;
				const char* const end2 = F2(end, aEnd, tmp2, zero2);
				if(end2 == end) return end2;

				aValue = static_cast<R>(tmp1) * static_cast<R>(std::pow(10, tmp2));

				return end2;
			}else {
				aValue = static_cast<R>(tmp1);
				return end;
			}
		}

		template<
			class T1, 
			class T2, 
			class R, 
			const char*(*F1)(const char* const, const char* const, T1&, uint32_t&),
			const char*(*F2)(const char* const, const char* const, T1&, uint32_t&)
		>
		static const char* ParseDecimal(const char* const aBegin, const char* const aEnd, R& aValue, uint32_t& aZeros) {
			T1 tmp1;
			T2 tmp2;
			uint32_t zero2;

			const char* end = F1(aBegin, aEnd, tmp1, aZeros);
			if(end == aBegin) return aBegin;

			if(end != aEnd && *end == '.'){
				++end;
				if(*end == '+') ++end;
				const char* const end2 = F2(end, aEnd, tmp2, zero2);
				if(end2 == end) return end2;

				aValue = static_cast<R>(tmp2);
				while(aValue >= static_cast<R>(1) && aValue != static_cast<R>(0)){
					aValue /= static_cast<R>(10);
				}

				uint32_t zeros = zero2;
				while(zeros > 0 && aValue != static_cast<R>(0)){
					aValue /= static_cast<R>(10);
					--zeros;
				}

				aValue = static_cast<R>(tmp1) + aValue;

				return end2;
			}else {
				aValue = static_cast<R>(tmp1);
				return end;
			}
		}
	}

	template<class T>
	static const char* ParseNumber(const char* const aBegin, const char* const aEnd, T& aValue);

	template<>
	const char* ParseNumber<uint32_t>(const char* const aBegin, const char* const aEnd, uint32_t& aValue) {
		uint32_t zeros;
		const char* const end = Implementation::ParseExponent<
			uint32_t, 
			uint32_t, 
			uint32_t, 
			&Implementation::ParseUint, 
			&Implementation::ParseUint
		>(aBegin, aEnd, aValue, zeros);
		return end;
	}

	template<>
	const char* ParseNumber<uint8_t>(const char* const aBegin, const char* const aEnd, uint8_t& aValue) {
		uint32_t tmp;
		const char* const end = ParseNumber<uint32_t>(aBegin, aEnd, tmp);
		aValue = static_cast<uint8_t>(tmp);
		return end;
	}

	template<>
	const char* ParseNumber<uint16_t>(const char* const aBegin, const char* const aEnd, uint16_t& aValue) {
		uint32_t tmp;
		const char* const end = ParseNumber<uint32_t>(aBegin, aEnd, tmp);
		aValue = static_cast<uint16_t>(tmp);
		return end;
	}

	template<>
	const char* ParseNumber<uint64_t>(const char* const aBegin, const char* const aEnd, uint64_t& aValue) {
		uint32_t tmp;
		const char* const end = ParseNumber<uint32_t>(aBegin, aEnd, tmp);
		aValue = static_cast<uint64_t>(tmp);
		return end;
	}

	template<>
	const char* ParseNumber<int32_t>(const char* const aBegin, const char* const aEnd, int32_t& aValue) {
		uint32_t zeros;
		const char*  end = Implementation::ParseExponent<
			int32_t, 
			int32_t, 
			int32_t, 
			&Implementation::ParseInt, 
			&Implementation::ParseInt
		>(aBegin, aEnd, aValue, zeros);
		return end;
	}

	template<>
	const char* ParseNumber<int8_t>(const char* const aBegin, const char* const aEnd, int8_t& aValue) {
		int32_t tmp;
		const char* const end = ParseNumber<int32_t>(aBegin, aEnd, tmp);
		aValue = static_cast<int8_t>(tmp);
		return end;
	}

	template<>
	const char* ParseNumber<int16_t>(const char* const aBegin, const char* const aEnd, int16_t& aValue) {
		int32_t tmp;
		const char* const end = ParseNumber<int32_t>(aBegin, aEnd, tmp);
		aValue = static_cast<int16_t>(tmp);
		return end;
	}

	template<>
	const char* ParseNumber<int64_t>(const char* const aBegin, const char* const aEnd, int64_t& aValue) {
		int32_t tmp;
		const char* const end = ParseNumber<int32_t>(aBegin, aEnd, tmp);
		aValue = static_cast<int64_t>(tmp);
		return end;
	}

	template<>
	const char* ParseNumber<double>(const char* const aBegin, const char* const aEnd, double& aValue) {
		uint32_t zeros;
		const char* const end = Implementation::ParseDecimal<
			double,
			double,
			double,
			&Implementation::ParseExponent<
				double,
				double,
				double,
				&Implementation::ParseDecimal<
					int32_t,
					int32_t,
					double,
					&Implementation::ParseInt,
					&Implementation::ParseInt
				>,
				&Implementation::ParseDecimal<
					int32_t,
					int32_t,
					double,
					&Implementation::ParseInt,
					&Implementation::ParseInt
				>
			>, 
			&Implementation::ParseExponent<
				double,
				double,
				double,
				&Implementation::ParseDecimal<
					int32_t,
					int32_t,
					double,
					&Implementation::ParseInt,
					&Implementation::ParseInt
				>,
				&Implementation::ParseDecimal<
					int32_t,
					int32_t,
					double,
					&Implementation::ParseInt,
					&Implementation::ParseInt
				>
			>
		>(aBegin, aEnd, aValue, zeros);
		return end;
	}

	template<>
	const char* ParseNumber<float>(const char* const aBegin, const char* const aEnd, float& aValue) {
		double tmp;
		const char* const end = ParseNumber<double>(aBegin, aEnd, tmp);
		aValue = static_cast<float>(tmp);
		return end;
	}
}
//    template<class T, class Iterator>
//    T ParseNumber(const Iterator aBegin, const Iterator aEnd, Iterator& aParseEnd){
//        typename NumericParser<T>::Type parser;
//
//        aParseEnd = aBegin;
//        while(aParseEnd != aEnd){
//
//            if(! parser.Accept(*aParseEnd)){
//                break;
//            }
//        }
//
//        try{
//            const T tmp = parser.Get();
//            return tmp;
//        }catch(...){
//            aParseEnd = aBegin;
//            return static_cast<T>(0);
//        }
//    }
//
//}
//
//
#endif
