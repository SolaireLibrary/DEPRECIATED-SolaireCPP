#ifndef SOLAIRE_LOGIC_FUZZY_OPERATORS_HPP
#define SOLAIRE_LOGIC_FUZZY_OPERATORS_HPP

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
\file FuzzyOperators.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 7th October 2015
Last Modified	: 7th October 2015
*/

#include "FuzzyBody.hpp"

namespace Solaire {namespace Fuzzy{

    enum{
        OPERATOR_NOT            = '!',
        OPERATOR_ADD            = '+',
        OPERATOR_SUBTRACT       = '-',
        OPERATOR_MULTIPLY       = '*',
        OPERATOR_DIVIDE         = '/',
        OPERATOR_AND            = '&',
        OPERATOR_OR             = '|',
        OPERATOR_LESS_THAN      = '<',
        OPERATOR_GREATER_THAN   = '>'
    };

    template<const int Sign>
    static constexpr float OperatorExecute(const float aValue) = delete;

    template<>
    constexpr float OperatorExecute<OPERATOR_NOT>(const float aValue){
        return 1.f - aValue;
    }

    template<const int Sign>
    static constexpr float OperatorExecute(const float aLeft, const float aRight) = delete;

    template<>
    constexpr float OperatorExecute<OPERATOR_ADD>(const float aLeft, const float aRight){
        return aLeft + aRight;
    }

    template<>
    constexpr float OperatorExecute<OPERATOR_SUBTRACT>(const float aLeft, const float aRight){
        return aLeft - aRight;
    }

    template<>
    constexpr float OperatorExecute<OPERATOR_MULTIPLY>(const float aLeft, const float aRight){
        return aLeft * aRight;
    }

    template<>
    constexpr float OperatorExecute<OPERATOR_DIVIDE>(const float aLeft, const float aRight){
        return aLeft / aRight;
    }

    template<>
    constexpr float OperatorExecute<OPERATOR_AND>(const float aLeft, const float aRight){
        return aLeft < aRight ? aLeft : aRight;
    }

    template<>
    constexpr float OperatorExecute<OPERATOR_OR>(const float aLeft, const float aRight){
        return aLeft >  aRight ? aLeft : aRight;
    }

    template<>
    constexpr float OperatorExecute<OPERATOR_LESS_THAN>(const float aLeft, const float aRight){
        return aLeft - aRight;
    }

    template<>
    constexpr float OperatorExecute<OPERATOR_GREATER_THAN>(const float aLeft, const float aRight){
        return aRight - aLeft;
    }

    template<const int Sign>
    static constexpr int OperatorPrecedence() = delete;

    template<>
    constexpr int OperatorPrecedence<OPERATOR_NOT>(){
        return 1;
    }

    template<>
    constexpr int OperatorPrecedence<OPERATOR_ADD>(){
        return 0;
    }

    template<>
    constexpr int OperatorPrecedence<OPERATOR_SUBTRACT>(){
        return 0;
    }

    template<>
    constexpr int OperatorPrecedence<OPERATOR_MULTIPLY>(){
        return 1;
    }

    template<>
    constexpr int OperatorPrecedence<OPERATOR_DIVIDE>(){
        return 1;
    }

    template<>
    constexpr int OperatorPrecedence<OPERATOR_AND>(){
        return -2;
    }

    template<>
    constexpr int OperatorPrecedence<OPERATOR_OR>(){
        return -3;
    }

    template<>
    constexpr int OperatorPrecedence<OPERATOR_LESS_THAN>(){
        return -1;
    }

    template<>
    constexpr int OperatorPrecedence<OPERATOR_GREATER_THAN>(){
        return -1;
    }

    template<const int Sign>
    class FUnaryOperator : public FUnary{
    public:
        static constexpr int SIGN = Sign;

        FUnaryOperator(std::shared_ptr<const FBody> aChild):
            FUnary(aChild)
        {}

        // Inherited from FBody
        float Execute(const Fuzzifier& aInputs) const override{
            return OperatorExecute<Sign>(mChild->Execute(aInputs));
        }
    };

    template<const int Sign>
    class FBinaryOperator : public FBinary{
    public:
        static constexpr int SIGN = Sign;

        template<class Iterator>
        static float PreCalculateConstantChain(Iterator aBegin, const Iterator aEnd){
            float value = *aBegin;
            ++ aBegin;
            while(aBegin != aEnd){
                value = OperatorExecute<Sign>(value, *aBegin);
                ++aBegin;
            }

            return value;
        }

        FBinaryOperator(std::shared_ptr<const FBody> aLeft, std::shared_ptr<const FBody> aRight):
            FBinary(aLeft, aRight)
        {}

        // Inherited from FBody
        float Execute(const Fuzzifier& aInputs) const override{
            return OperatorExecute<Sign>(mLeft->Execute(aInputs), mRight->Execute(aInputs));
        }
    };

    typedef FUnaryOperator<OPERATOR_NOT> OperatorNot;
    typedef FBinaryOperator<OPERATOR_ADD> OperatorAdd;
    typedef FBinaryOperator<OPERATOR_SUBTRACT> OperatorSubtract;
    typedef FBinaryOperator<OPERATOR_MULTIPLY> OperatorMultiply;
    typedef FBinaryOperator<OPERATOR_DIVIDE> OperatorDivide;
    typedef FBinaryOperator<OPERATOR_AND> OperatorAnd;
    typedef FBinaryOperator<OPERATOR_OR> OperatorOr;
    typedef FBinaryOperator<OPERATOR_LESS_THAN> OperatorLessThan;
    typedef FBinaryOperator<OPERATOR_GREATER_THAN> OperatorGreaterThan;
}}

#endif
