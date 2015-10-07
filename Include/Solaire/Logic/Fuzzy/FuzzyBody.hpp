#ifndef SOLAIRE_LOGIC_FUZZY_BODY_HPP
#define SOLAIRE_LOGIC_FUZZY_BODY_HPP

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
\file FuzzyBody.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 7th October 2015
Last Modified	: 7th October 2015
*/

#include "Fuzzifier.hpp"

namespace Solaire {namespace Fuzzy{

    class FBody{
    public:
        virtual ~FBody(){}

        virtual float Execute(const Fuzzifier& aInputs) const = 0;
    };

    class FInput : public FBody{
    private:
        const String mName;
    public:
        FInput(String aName):
            mName(aName)
        {}

        // Inherited from FBody
        float Execute(const Fuzzifier& aInputs) const override{
            return aInputs.GetInputValue(mName);
        }
    };

    class FConstant : public FBody{
    private:
        const float mValue;
    public:
        FConstant(float aValue):
            mValue(aValue)
        {}

        // Inherited from FBody
        float Execute(const Fuzzifier& aInputs) const override{
            return mValue;
        }
    };

    class FUnary : public FBody{
    protected:
        std::shared_ptr<const FBody> mChild;
    public:
        FUnary(std::shared_ptr<const FBody> aChild):
            mChild(aChild)
        {}

        virtual ~FUnary(){

        }
    };

    class FBinary : public FBody{
    protected:
        std::shared_ptr<const FBody> mLeft;
        std::shared_ptr<const FBody> mRight;
    public:
        FBinary(std::shared_ptr<const FBody> aLeft, std::shared_ptr<const FBody> aRight):
            mLeft(aLeft),
            mRight(aRight)
        {}

        virtual ~FBinary(){

        }
    };
}}

#endif
