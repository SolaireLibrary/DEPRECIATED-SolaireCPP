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

    static bool ParseDigit(const char aChar, uint8_t& aOutput){
        if(IsNumber(aChar)){
            aOutput = aChar - '0';
            return true;
        }else{
            return false;
        }
    }

    static const char* ParseNumber(const char* const aBegin, const char* const aEnd, double& aOutput, const bool aCanBeNegative, const bool aCanBeDecimal){
        // The states for the parsing FSA
        enum State{
            FIND_SIGN,                          // Initial state, determines if the value is positive or negative
            FIND_VALUE,                         // Looking for the body of the value
            FIND_DECIMAL_VALUE,                 // Looking for the decimal places in the value
            FIND_EXPONENT_SIGN,                 // Determines if an exponent is in the form eN, e+N or e-N
            FIND_EXPONENT_VALUE,                // Looking for the exponent value
            RETURN                              // Final parsing of the value for return
        };

        bool negative = false;                  // Set to true if the value is determined to be negative
        bool exponentNegative = false;          // Set to true if the exponent is determined to be e-N

        int64_t value = 0;                      // The main value body
        int64_t exponentValue = 0;              // The value of the exponent
        int64_t decimalValue = 0;               // The value of the decimal places

        const char* valueBegin = nullptr;       // Set to the first character in the value
        const char* exponentBegin = nullptr;    // Set to the first character in the exponent value
        const char* decimalBegin = nullptr;     // Set to the first character in the decimal value
        const char* i = aBegin;                 // The current character

        uint32_t decimalPlaces = 0;             // Stores how many decimal places are in the decimal value
        uint8_t digit;                          // Captures the output of ParseDigit
        State state = State::FIND_SIGN;         // The current FSA state

        // Internal FSM logic escapes the loop
        while(true){
            // If there are no more characters to examine jump straight to the RETURN state
            if(i >= aEnd) state = RETURN;

            switch(state){
            case FIND_SIGN:
                if(*i == '-'){          // Check if the current character is a sign
                    negative = true;    // Set the negative value flag
                    ++i;                // Move to the next character, which should be the first number in the value
                }
                valueBegin = i;
                state = FIND_VALUE;     // Jump to the FIND_VALUE state
                break;
            case FIND_VALUE:
                if(*i == 'e' || *i == 'E'){         // Check for an exponent
                    ++i;                            // Move to the next character, which should be the first number in the exponent, or it's sign
                    state = FIND_EXPONENT_SIGN;     // Jump to the FIND_EXPONENT_SIGN state
                }else if(*i == '.'){                // Check for a decimal place
                    ++i;                            // Move to the next character, which should be the first number in the decimal value
                    decimalBegin = i;               // Record which character the decimal value stated from
                    state = FIND_DECIMAL_VALUE;     // Jump to the FIND_DECIMAL_VALUE state
                }else{
                    if(! ParseDigit(*i, digit)){    // Check if the value is not a valid number
                        --i;                        // Move back the the previous character, which was the last character used in the value
                        state = RETURN;             // Jump to the return state
                    }else{
                        const int64_t offset = (i - valueBegin) + 1;            // Calculate the offset from the beginning of the value
                        value += static_cast<int64_t>(digit) * 10 * offset;     // Update the value with the new character
                        ++i;                                                    // Move to the next number in the value
                    }
                }
                break;
            case FIND_DECIMAL_VALUE:
                if(*i == 'e' || *i == 'E'){         // Check for an exponent
                    ++i;                            // Move to the next character, which should be the first number in the exponent, or it's sign
                    state = FIND_EXPONENT_SIGN;     // Jump to the FIND_EXPONENT_SIGN state
                }else{
                    if(! ParseDigit(*i, digit)){    // Check if the value is not a valid number
                        --i;                        // Move back the the previous character, which was the last character used in the decimal value
                        state = RETURN;             // Jump to the return state
                    }else{
                        const int64_t offset = (i - decimalBegin) + 1;                  // Calculate the offset from the beginning of the decimal value
                        decimalValue += static_cast<int64_t>(digit) * 10 * offset;      // Update the decimal value with the new character
                        ++i;                                                            // Move to the next number in the decimal
                    }
                }
                break;
            case FIND_EXPONENT_SIGN:
                if(*i == '-'){                  // Check for an exponent sign
                    exponentNegative = true;    // Set the exponent sign flag
                    ++i;                        // Move to the next character, which should be the first number in the exponent value
                }else if(*i == '+'){            // Check for an exponent sign
                    exponentNegative = false;   // Set the exponent sign flag
                    ++i;                        // Check for an exponent sign
                }else{
                    exponentNegative = false;   // Set the exponent sign flag
                }
                exponentBegin = i;              // Record which character the exponent value started from
                state = FIND_EXPONENT_VALUE;    // Jump to the FIND_EXPONENT_VALUE state
                break;
            case FIND_EXPONENT_VALUE:
                if(! ParseDigit(*i, digit)){    // Check if the character is not a valid number
                    --i;                        // Move back the the previous character, which was the last character used in the exponent value
                    state = RETURN;             // Jump to the return state
                }else{
                    const int64_t offset = (i - exponentBegin) + 1;                  // Calculate the offset from the beginning of the exponent value
                    exponentValue += static_cast<int64_t>(digit) * 10 * offset;      // Update the exponent value with the new character
                    ++i;                                                             // Move to the next number in the exponent value
                }
                break;
            case RETURN:
                if(
                   (valueBegin == nullptr) ||                           // Check if the parse has failed
                   (decimalBegin != nullptr && decimalPlaces == 0) ||
                   (negative && ! aCanBeNegative) ||                    // Check if the parse has produced an illegal number
                   (decimalValue != 0 && ! aCanBeDecimal)
                ){
                    // Return parse failue
                    aOutput = 0.0;
                    return nullptr;
                }else{
                    // Construct the value body
                    double val = value;
                    if(negative) val *= -1.0;

                    if(decimalBegin != nullptr){        // Check if a decimal value was parsed
                        // Add the decimal value
                        double dec = decimalValue;
                        dec /= 10.0 * decimalPlaces;    // Shift the decimal places down
                        val += dec;
                    }

                    if(exponentBegin != nullptr){       // Check if an exponent value was parsed
                        // Multiply by the exponent
                        int64_t exp = exponentValue;
                        if(exponentNegative) exp *= -1;
                        val *= std::pow(10, exp);
                    }

                    // Return parse success
                    aOutput = val;
                    return i;
                }
                break;
            }
        }
     }

    /*!
        \brief Attempt to parse a value from a c-string segment.
        \tparam T The type to parse.
        \param aBegin The address of the first character in the segment.
        \param aBegin The address of the character after the last character in the segment, this will often be be the null terminator (\0).
        \param aOutput A reference that is assigned the parsed value.
        \return Nullptr if the parse failed, else the address of the last character used during the pass. This will be equal to \a aEnd-1 if all characters were used.
    */
    template<typename T>
    static const char* Parse(const char* const aBegin, const char* const aEnd, T& aOutput) = delete;

    template<>
    const char* Parse<double>(const char* const aBegin, const char* const aEnd, double& aOutput){
        return ParseNumber(aBegin, aEnd, aOutput, true, true);
    }

    template<>
    const char* Parse<float>(const char* const aBegin, const char* const aEnd, float& aOutput){
        double tmp;
        const char* const res = ParseNumber(aBegin, aEnd, tmp, true, true);
        aOutput = static_cast<float>(tmp);
        return res;
    }

    template<>
    const char* Parse<int64_t>(const char* const aBegin, const char* const aEnd, int64_t& aOutput){
        double tmp;
        const char* const res = ParseNumber(aBegin, aEnd, tmp, true, false);
        aOutput = static_cast<int64_t>(tmp);
        return res;
    }

    template<>
    const char* Parse<int32_t>(const char* const aBegin, const char* const aEnd, int32_t& aOutput){
        double tmp;
        const char* const res = ParseNumber(aBegin, aEnd, tmp, true, false);
        aOutput = static_cast<int32_t>(tmp);
        return res;
    }

    template<>
    const char* Parse<int16_t>(const char* const aBegin, const char* const aEnd, int16_t& aOutput){
        double tmp;
        const char* const res = ParseNumber(aBegin, aEnd, tmp, true, false);
        aOutput = static_cast<int16_t>(tmp);
        return res;
    }

    template<>
    const char* Parse<int8_t>(const char* const aBegin, const char* const aEnd, int8_t& aOutput){
        double tmp;
        const char* const res = ParseNumber(aBegin, aEnd, tmp, true, false);
        aOutput = static_cast<int8_t>(tmp);
        return res;
    }

    template<>
    const char* Parse<uint64_t>(const char* const aBegin, const char* const aEnd, uint64_t& aOutput){
        double tmp;
        const char* const res = ParseNumber(aBegin, aEnd, tmp, false, false);
        aOutput = static_cast<uint64_t>(tmp);
        return res;
    }

    template<>
    const char* Parse<uint32_t>(const char* const aBegin, const char* const aEnd, uint32_t& aOutput){
        double tmp;
        const char* const res = ParseNumber(aBegin, aEnd, tmp, false, false);
        aOutput = static_cast<uint32_t>(tmp);
        return res;
    }

    template<>
    const char* Parse<uint16_t>(const char* const aBegin, const char* const aEnd, uint16_t& aOutput){
        double tmp;
        const char* const res = ParseNumber(aBegin, aEnd, tmp, false, false);
        aOutput = static_cast<uint16_t>(tmp);
        return res;
    }

    template<>
    const char* Parse<uint8_t>(const char* const aBegin, const char* const aEnd, uint8_t& aOutput){
        double tmp;
        const char* const res = ParseNumber(aBegin, aEnd, tmp, false, false);
        aOutput = static_cast<uint8_t>(tmp);
        return res;
    }

    /*!
        \brief Attempt to parse a value from a c-string segment.
        \detail Throws std::runtime_error if parse fails.
        \tparam T The type to parse.
        \param aBegin The address of the first character in the segment.
        \param aBegin The address of the character after the last character in the segment, this will often be be the null terminator (\0).
        \param The parsed value.
    */
    template<typename T>
    static T Parse(const char* const aBegin, const char* const aEnd){
        T tmp;
        const char* const res = Parse<T>(aBegin, aEnd, tmp);
        if(res == nullptr) throw std::runtime_error("Failed to parse value");
        return tmp;
     }
}}}


#endif
