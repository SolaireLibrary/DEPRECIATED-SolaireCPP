#ifndef SOLAIRE_UTILITY_STRING_PROCESSOR_HPP
#define SOLAIRE_UTILITY_STRING_PROCESSOR_HPP

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
	\file StringProcessor.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 11th September 2015
	Last Modified	: 11th September 2015
*/

#include <string>

namespace Solaire{ namespace Utility{ namespace Strings{

    enum{
        CASE_DIFFERENCE = 'A' - 'a'
    };

    static constexpr char LOWER_CASE_CHARACTERS[]   = "abcdefghijklmnopqrstuvwxyz";
    static constexpr char UPPER_CASE_CHARACTERS[]   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static constexpr char ALL_CHARACTERS[]          = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static constexpr char NUMBERS[]                 = "0123456789";
    static constexpr char WHITESPACE[]              = " \t";

    static constexpr char IsWhitespace(const char aChar){
        return aChar == WHITESPACE[0] || aChar == WHITESPACE[1];
    }

    static constexpr char IsLowerCase(const char aChar){
        return aChar >= 'a' && aChar <= 'z';
    }

    static constexpr char IsUpperCase(const char aChar){
        return aChar >= 'A' && aChar <= 'Z';
    }

    static constexpr char IsNumber(const char aChar){
        return aChar >= '0' && aChar <= '9';
    }

    static constexpr char ToUpperCase(const char aChar){
        return IsLowerCase(aChar) ? aChar + CASE_DIFFERENCE : aChar;
    }

    static constexpr char ToLowerCase(const char aChar){
        return IsUpperCase(aChar) ? aChar - CASE_DIFFERENCE : aChar;
    }

    static constexpr char ToggleCase(const char aChar){
        return IsUpperCase(aChar) ? ToLowerCase(aChar) : IsLowerCase(aChar) ? ToUpperCase(aChar) : aChar;
    }

    static bool IsLowerCase(const char* const aBegin, const char* const aEnd){
        for(const char* i = aBegin; i != aEnd; ++i){
            if(! IsLowerCase(*i)) return false;
        }
        return true;
    }

    static bool IsUpperCase(const char* const aBegin, const char* const aEnd){
        for(const char* i = aBegin; i != aEnd; ++i){
            if(! IsUpperCase(*i)) return false;
        }
        return true;
    }

    static size_t ToLowerCase(char* const aBegin, const char* const aEnd){
        size_t count = 0;
        for(char* i = aBegin; i != aEnd; ++i){
            const char c = *i;
            *i = ToLowerCase(c);
            if(*i != c){
                ++count;
            }
        }
        return count;
    }

    static size_t ToUpperCase(char* const aBegin, const char* const aEnd){
        size_t count = 0;
        for(char* i = aBegin; i != aEnd; ++i){
            const char c = *i;
            *i = ToUpperCase(c);
            if(*i != c){
                ++count;
            }
        }
        return count;
    }

    static size_t ToggleCase(char* const aBegin, const char* const aEnd){
        size_t count = 0;
        for(char* i = aBegin; i != aEnd; ++i){
            const char c = *i;
            *i = ToggleCase(c);
            if(*i != c){
                ++count;
            }
        }
        return count;
    }

    static size_t ToLowerCase(std::string& aString){
        char* const begin = &aString[0];
        return ToLowerCase(begin, begin + aString.size());
    }

    static size_t ToUpperCase(std::string& aString){
        char* const begin = &aString[0];
        return ToUpperCase(begin, begin + aString.size());
    }

    static size_t ToggleCase(std::string& aString){
        char* const begin = &aString[0];
        return ToggleCase(begin, begin + aString.size());
    }

    static const char* FindNext(const char* aBegin, const char* aEnd, const char aChar){
        for(const char* i = aBegin; i != aEnd; ++i){
            if(*i == aChar) return i;
        }
        return nullptr;
    }

    static const char* FindNext(const char* aBegin, const char* aEnd, const char* aTargetBegin, const char* aTargetEnd){
        const char* k;
        bool found;
        for(const char* i = aBegin; i != aEnd; ++i){
            k = i;
            found = true;
            for(const char* j = aTargetBegin; j != aTargetEnd; ++j){
                if(*j != *k){
                    found = false;
                    break;
                }
                ++k;
            }
            if(found) return i;
        }
        return nullptr;
    }

    static const char* FindNextNot(const char* aBegin, const char* aEnd, const char aChar){
        for(const char* i = aBegin; i != aEnd; ++i){
            if(*i != aChar) return i;
        }
        return nullptr;
    }

    template<class F>
    static const char* FindNextConditional(const char* aBegin, const char* aEnd, const F aCondition){
        for(const char* i = aBegin; i != aEnd; ++i){
            if(aCondition(*i)) return i;
        }
        return nullptr;
    }

    static const char* FindNextIn(const char* aBegin, const char* aEnd, const char* aTargetBegin, const char* aTargetEnd){
        char c;
        for(const char* i = aBegin; i != aEnd; ++i){
            c = *i;
            for(const char* j = aTargetBegin; j != aTargetEnd; ++j){
                if(*j == c) return i;
            }
        }
        return nullptr;
    }

    static const char* FindNextNotIn(const char* aBegin, const char* aEnd, const char* aTargetBegin, const char* aTargetEnd){
        bool found;
        char c;
        for(const char* i = aBegin; i != aEnd; ++i){
            found = false;
            c = *i;
            for(const char* j = aTargetBegin; j != aTargetEnd; ++j){
                if(*j == c){
                    found = true;
                    break;
                }
            }
            if(! found) return i;
        }
        return nullptr;
    }

    static size_t PointerToIndex(const std::string& aString, const char* const aPtr){
        return aPtr == nullptr ? aString.size() : (aString.c_str() + aString.size()) - aPtr;
    }

    static const size_t FindNext(const std::string& aString, const char aChar){
        return PointerToIndex(aString, FindNext(aString.c_str(), aString.c_str() + aString.size(), aChar));
    }

    static const size_t FindNextNot(const std::string& aString, const char aChar){
        return PointerToIndex(aString, FindNextNot(aString.c_str(), aString.c_str() + aString.size(), aChar));
    }

    static const size_t FindNextIn(const std::string& aString, const char* const aTargetBegin, const char* const aTargetEnd){
        return PointerToIndex(aString, FindNextIn(aString.c_str(), aString.c_str() + aString.size(), aTargetBegin, aTargetEnd));
    }

    static const size_t FindNextIn(const std::string& aString, const std::string& aTarget){
        return FindNextIn(aString, aTarget.c_str(), aTarget.c_str() + aTarget.size());
    }

    static const size_t FindNextNotIn(const std::string& aString, const char* const aTargetBegin, const char* const aTargetEnd){
        return PointerToIndex(aString, FindNextNotIn(aString.c_str(), aString.c_str() + aString.size(), aTargetBegin, aTargetEnd));
    }

    static const size_t FindNextNotIn(const std::string& aString, const std::string& aTarget){
        return FindNextNotIn(aString, aTarget.c_str(), aTarget.c_str() + aTarget.size());
    }

    static const char* FindNextNested(const char* aBegin, const char* aEnd, const char aOpen, const char aClose){
        size_t depth = 0;
        char c;
        for(const char* i = aBegin; i != aEnd; ++i){
            c = *i;
            if(c == aOpen){
                ++depth;
            }else if(c == aClose){
                if(depth == 0){
                    return i;
                }else{
                    --depth;
                }
            }
        }
        return nullptr;
    }

    static size_t FindNextNested(const std::string& aString, const char aOpen, const char aClose){
        return PointerToIndex(aString, FindNextNested(aString.c_str(), aString.c_str() + aString.size(), aOpen, aClose));
    }

    static bool Equals(const char* const aFirstBegin, const char* const aFirstEnd, const char* const aSecondBegin, const char* const aSecondEnd){
        const size_t len0 = aFirstEnd - aFirstBegin;
        const size_t len1 = aSecondEnd - aSecondBegin;
        return len0 != len1 ? false : std::memcmp(aFirstBegin, aSecondBegin, len0) == 0;
    }

    static bool Equals(const std::string& aString, const char* const aSecondBegin, const char* const aSecondEnd){
        return Equals(aString.c_str(), aString.c_str() + aString.size(), aSecondBegin, aSecondEnd);
    }

    enum class ParseResult{
        FAIL,             // Failed to parse
        SUCCESS,            // Parsed using all available characters
        PARTIAL_SUCCESS     // Parsed with characters remaining
    };

    static ParseResult ParseDigit(const char aChar, uint8_t& aOutput){
        if(IsNumber(aChar)){
            aOutput = aChar - '0';
            return ParseResult::SUCCESS;
        }else{
            return ParseResult::FAIL;
        }
    }

    static ParseResult ParseNumber(const char* const aBegin, const char* const aEnd, double& aOutput, const bool aCanBeNegative, const bool aCanBeDecimal){
        enum State{
            FIND_SIGN,
            FIND_VALUE,
            FIND_DECIMAL_VALUE,
            FIND_E_SIGN,
            FIND_E_VALUE,
            RETURN
        };

        const size_t length = aEnd - aBegin;
        bool negative = false;
        bool eNegative = false;
        int64_t eValue = 0;
        int64_t value = 0;
        int64_t decimalValue = 0;
        uint32_t decimalPlaces = 0;
        uint8_t digit;
        const char* eBegin = nullptr;
        const char* decimalBegin = nullptr;
        const char* i = aBegin;
        State state = State::FIND_SIGN;

        while(true){
            if(i >= aEnd) state = RETURN;

            switch(state){
            case FIND_SIGN:
                if(*i == '-'){
                    negative = true;
                    ++i;
                }
                state = FIND_VALUE;
                break;
            case FIND_VALUE:
                if(*i == 'e' || *i == 'E'){
                    state = FIND_E_SIGN;
                    ++i;
                }else if(*i == '.'){
                    state = FIND_DECIMAL_VALUE;
                    decimalBegin = i;
                    ++i;
                }else{
                    ParseResult res = ParseDigit(*i, digit);
                    if(res == ParseResult::FAIL){
                        state = RETURN;
                    }else{
                        value += static_cast<int64_t>(digit) * 10 * ((i - aBegin) + 1);
                        ++i;
                    }
                }
                break;
            case FIND_DECIMAL_VALUE:
                if(*i == 'e' || *i == 'E'){
                    state = FIND_E_SIGN;
                    ++i;
                }else{
                    ParseResult res = ParseDigit(*i, digit);
                    if(res == ParseResult::FAIL){
                        state = RETURN;
                    }else{
                        decimalValue += static_cast<int64_t>(digit) * 10 * ((i - decimalBegin) + 1);
                        ++decimalPlaces;
                        ++i;
                    }
                }
                break;
            case FIND_E_SIGN:
                if(*i == '-'){
                    eNegative = true;
                    ++i;
                }else if(*i == '+'){
                    eNegative = false;
                    ++i;
                }
                eBegin = i;
                state = FIND_E_VALUE;
                break;
            case FIND_E_VALUE:
                if(ParseDigit(*i, digit) == ParseResult::FAIL){
                    state = RETURN;
                }else{
                    eValue += static_cast<int64_t>(digit) * 10 * ((i - eBegin) + 1);
                    ++i;
                }
                break;
            case RETURN:
                if(
                   (i == aBegin) ||
                   (negative && ! aCanBeNegative) ||
                   (decimalValue != 0 && ! aCanBeDecimal) ||
                   (decimalBegin != nullptr && decimalPlaces == 0)
                ){
                    aOutput = 0;
                    return ParseResult::FAIL;
                }else{
                    double val = value;

                    if(negative) val *= -1.0;

                    if(decimalPlaces > 0){
                        double dec = decimalValue;
                        dec /= 10 * decimalPlaces;
                        val += dec;
                    }

                    if(eValue != 0){
                        if(eNegative) eValue *= -1;
                        val *= std::pow(10, eValue);
                    }

                    aOutput = val;
                    return i == aEnd ? ParseResult::SUCCESS : ParseResult::PARTIAL_SUCCESS;
                }
                break;
            }
        }
     }

    template<typename T>
    static ParseResult Parse(const char* const aBegin, const char* const aEnd, T& aOutput) = delete;

    template<>
    ParseResult Parse<double>(const char* const aBegin, const char* const aEnd, double& aOutput){
        return ParseNumber(aBegin, aEnd, aOutput, true, true);
    }

    template<>
    ParseResult Parse<float>(const char* const aBegin, const char* const aEnd, float& aOutput){
        double tmp;
        const ParseResult res = ParseNumber(aBegin, aEnd, tmp, true, true);
        aOutput = static_cast<float>(tmp);
        return res;
    }

    template<>
    ParseResult Parse<int64_t>(const char* const aBegin, const char* const aEnd, int64_t& aOutput){
        double tmp;
        const ParseResult res = ParseNumber(aBegin, aEnd, tmp, true, false);
        aOutput = static_cast<int64_t>(tmp);
        return res;
    }

    template<>
    ParseResult Parse<int32_t>(const char* const aBegin, const char* const aEnd, int32_t& aOutput){
        double tmp;
        const ParseResult res = ParseNumber(aBegin, aEnd, tmp, true, false);
        aOutput = static_cast<int32_t>(tmp);
        return res;
    }

    template<>
    ParseResult Parse<int16_t>(const char* const aBegin, const char* const aEnd, int16_t& aOutput){
        double tmp;
        const ParseResult res = ParseNumber(aBegin, aEnd, tmp, true, false);
        aOutput = static_cast<int16_t>(tmp);
        return res;
    }

    template<>
    ParseResult Parse<int8_t>(const char* const aBegin, const char* const aEnd, int8_t& aOutput){
        double tmp;
        const ParseResult res = ParseNumber(aBegin, aEnd, tmp, true, false);
        aOutput = static_cast<int8_t>(tmp);
        return res;
    }

    template<>
    ParseResult Parse<uint64_t>(const char* const aBegin, const char* const aEnd, uint64_t& aOutput){
        double tmp;
        const ParseResult res = ParseNumber(aBegin, aEnd, tmp, false, false);
        aOutput = static_cast<uint64_t>(tmp);
        return res;
    }

    template<>
    ParseResult Parse<uint32_t>(const char* const aBegin, const char* const aEnd, uint32_t& aOutput){
        double tmp;
        const ParseResult res = ParseNumber(aBegin, aEnd, tmp, false, false);
        aOutput = static_cast<uint32_t>(tmp);
        return res;
    }

    template<>
    ParseResult Parse<uint16_t>(const char* const aBegin, const char* const aEnd, uint16_t& aOutput){
        double tmp;
        const ParseResult res = ParseNumber(aBegin, aEnd, tmp, false, false);
        aOutput = static_cast<uint16_t>(tmp);
        return res;
    }

    template<>
    ParseResult Parse<uint8_t>(const char* const aBegin, const char* const aEnd, uint8_t& aOutput){
        double tmp;
        const ParseResult res = ParseNumber(aBegin, aEnd, tmp, false, false);
        aOutput = static_cast<uint8_t>(tmp);
        return res;
    }

     template<typename T>
     static T Parse(const char* const aBegin, const char* const aEnd){
         T tmp;
         const ParseResult res = Parse<T>(aBegin, aEnd, tmp);
         if(res == ParseResult::FAIL) throw std::runtime_error("Failed to parse value");
         return tmp;
     }
}}}


#endif
