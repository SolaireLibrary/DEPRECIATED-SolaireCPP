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


    class FAnd : public FBinary{
    public:
        FAnd(std::shared_ptr<const FBody> aLeft, std::shared_ptr<const FBody> aRight):
            FBinary(aLeft, aRight)
        {}

        // Inherited from FBody
        float Execute(const Fuzzifier& aInputs) const override{
            return std::min<float>(mLeft->Execute(aInputs), mRight->Execute(aInputs));
        }
    };

    class FOr : public FBinary{
    public:
        FOr(std::shared_ptr<const FBody> aLeft, std::shared_ptr<const FBody> aRight):
            FBinary(aLeft, aRight)
        {}

        // Inherited from FBody
        float Execute(const Fuzzifier& aInputs) const override{
            return std::max<float>(mLeft->Execute(aInputs), mRight->Execute(aInputs));
        }
    };

    class FBracket : public FUnary{
    public:
        FBracket(std::shared_ptr<const FBody> aChild):
            FUnary(aChild)
        {}

        // Inherited from FBody
        float Execute(const Fuzzifier& aInputs) const override{
            return mChild->Execute(aInputs);
        }
    };
}}

#endif
