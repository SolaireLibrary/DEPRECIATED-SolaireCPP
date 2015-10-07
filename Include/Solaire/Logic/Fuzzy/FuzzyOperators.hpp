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

    template<const char Sign>
    class FUnaryOperator : public FUnary{
    public:
        FUnaryOperator(std::shared_ptr<const FBody> aChild):
            FUnary(aChild)
        {}

        template<const char S = Sign, typename std::enable_if<S == '!', int>::type = 0>
        float OperatorExecute(const Fuzzifier& aInputs) const{
            return 1.f - mChild->Execute(aInputs);
        }

        // Inherited from FBody
        float Execute(const Fuzzifier& aInputs) const override{
            return OperatorExecute<Sign>(aInputs);
        }
    };

    template<const char Sign>
    class FBinaryOperator : public FBinary{
    public:
        FBinaryOperator(std::shared_ptr<const FBody> aLeft, std::shared_ptr<const FBody> aRight):
            FBinary(aLeft, aRight)
        {}

        template<const char S = Sign, typename std::enable_if<S == '+', int>::type = 0>
        float OperatorExecute(const Fuzzifier& aInputs) const{
            return mLeft->Execute(aInputs) + mRight->Execute(aInputs);
        }

        template<const char S = Sign, typename std::enable_if<S == '-', int>::type = 0>
        float OperatorExecute(const Fuzzifier& aInputs) const{
            return mLeft->Execute(aInputs) - mRight->Execute(aInputs);
        }

        template<const char S = Sign, typename std::enable_if<S == '*', int>::type = 0>
        float OperatorExecute(const Fuzzifier& aInputs) const{
            return mLeft->Execute(aInputs) * mRight->Execute(aInputs);
        }

        template<const char S = Sign, typename std::enable_if<S == '/', int>::type = 0>
        float OperatorExecute(const Fuzzifier& aInputs) const{
            return mLeft->Execute(aInputs) / mRight->Execute(aInputs);
        }

        template<const char S = Sign, typename std::enable_if<S == '&', int>::type = 0>
        float OperatorExecute(const Fuzzifier& aInputs) const{
            return std::min(mLeft->Execute(aInputs), mRight->Execute(aInputs));
        }

        template<const char S = Sign, typename std::enable_if<S == '|', int>::type = 0>
        float OperatorExecute(const Fuzzifier& aInputs) const{
            return std::max(mLeft->Execute(aInputs), mRight->Execute(aInputs));
        }

        // Inherited from FBody
        float Execute(const Fuzzifier& aInputs) const override{
            return OperatorExecute<Sign>(aInputs);
        }
    };

    typedef FUnaryOperator<'!'> OperatorNot;
    typedef FBinaryOperator<'+'> OperatorAdd;
    typedef FBinaryOperator<'-'> OperatorSubtract;
    typedef FBinaryOperator<'*'> OperatorMultiply;
    typedef FBinaryOperator<'/'> OperatorDivide;
    typedef FBinaryOperator<'&'> OperatorAnd;
    typedef FBinaryOperator<'|'> OperatorOr;
}}

#endif
