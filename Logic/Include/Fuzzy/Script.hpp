#ifndef SOLAIRE_LOGIC_FUZZY_SCRIPT_HPP
#define SOLAIRE_LOGIC_FUZZY_SCRIPT_HPP

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
	\file Script.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 10th September 2015
	Last Modified	: 10th September 2015
*/

#include <cstdint>
#include <cstring>
#include <vector>
#include "Controller.hpp"

namespace Solaire{ namespace Logic{ namespace Fuzzy{
    class Command{
    public:
        static const char* FindWordBegin(const char* aString){
            if(aString == nullptr) return nullptr;
            while(true){
                const char c = *aString;
                if(c == '\0'){
                    return nullptr;
                }else if(std::isspace(c)){
                    ++aString;
                }else{
                    return aString;
                }
            }
        }

        static const char* FindWordEnd(const char* aString){
            if(aString == nullptr) return nullptr;
            while(true){
                const char c = *aString;
                if(std::isspace(c) || c == '\0'){
                    return aString;
                }else{
                    ++aString;
                }
            }
        }

        virtual ~Command(){

        }
    };

    class IfStatement : public Command{
    private:
        const std::string mInput;
        const std::string mMembershipFunction;
        const bool mNot;
    public:
        IfStatement(const std::string aInput, const std::string aMembership, const bool aNot = false) :
            mInput(aInput),
            mMembershipFunction(aMembership),
            mNot(aNot)
        {

        }

        static IfStatement* Compile(const char* aCodeBegin){
            // Locate Keywords
            const char* _if = std::strstr(aCodeBegin, "IF");
            const char* _is = _if ? std::strstr(_if, "IS") : nullptr;
            const char* _not = std::strstr(aCodeBegin, "NOT");

            if(! (_if && _is)) return nullptr;

            // Locate values
            const char* inputBegin = FindWordBegin(_if + 2);
            const char* inputEnd = FindWordEnd(inputEnd);
            const char* membershipBegin = FindWordBegin(_is + 2);
            const char* membershipEnd = FindWordEnd(membershipBegin);

            if(! (inputBegin && inputEnd && membershipBegin && membershipEnd)) return nullptr;

            // Build command
            return new IfStatement(
                std::string(inputBegin, inputEnd - inputBegin),
                std::string(membershipBegin, membershipEnd - membershipBegin),
                _not ? true : false
            );
        }

        truth_t operator()(Fuzzifier& aFuzzifier, const Controller& aController) const{
            const truth_t tmp = aController.CalculateMembership(mMembershipFunction, aFuzzifier.GetInput(mInput));
            return mNot ? Fuzzy::Not(tmp) : tmp;
        }
    };

    class ThenStatement : public Command{
    private:
        const std::string mOutput;
    public:
        ThenStatement(const std::string aOutput) :
            mOutput(aOutput)
        {

        }

        static ThenStatement* Compile(const char* aCodeBegin){
            // Locate Keywords
            const char* _then = std::strstr(aCodeBegin, "THEN");

            if(! _then) return nullptr;

            // Locate values
            const char* outputBegin = FindWordBegin(_then + 4);
            const char* outputEnd = FindWordEnd(outputBegin);

            if(! (outputBegin && outputEnd)) return nullptr;

            // Build command
            return new ThenStatement(
                std::string(outputBegin, outputEnd - outputBegin)
            );
        }

        void operator()(Fuzzifier& aFuzzifier, const truth_t aValue) const{
            return aFuzzifier.SetOutput(mOutput, aValue);
        }
    };

    class BinaryOperator : public Command{
    protected:
        const Command* const mLeft;
        const Command* const mRight;
    public:
        template<class T>
        static T* Compile(const char* aCode){
            static_assert(std::is_base_of<Command, T>::value, "Must inherit from BinaryOperator");
            //! \TODO Implement BinaryOperator.Compile
            return nullptr;
        }

        BinaryOperator(const Command* const aLeft, const Command* const aRight) :
            mLeft(aLeft),
            mRight(aRight)
        {

        }

        virtual ~BinaryOperator(){
            delete mLeft;
            delete mRight;
        }

        virtual truth_t operator()(Fuzzifier& aFuzzifier, const Controller& aController) const = 0;
    };

    class And : public BinaryOperator{
    public:
        And(const IfStatement* const aLeft, const IfStatement* const aRight) :
            BinaryOperator(aLeft, aRight)
        {

        }

        // Inherited from BinaryOperator

        truth_t operator()(Fuzzifier& aFuzzifier, const Controller& aController) const override{
            return Fuzzy::And(
                reinterpret_cast<const IfStatement*>(mLeft)->operator()(aFuzzifier, aController),
                reinterpret_cast<const IfStatement*>(mRight)->operator()(aFuzzifier, aController)
            );
        }
    };

    class Or : public BinaryOperator{
    public:
        Or(const IfStatement* const aLeft, const IfStatement* const aRight) :
            BinaryOperator(aLeft, aRight)
        {

        }

        // Inherited from BinaryOperator

        truth_t operator()(Fuzzifier& aFuzzifier, const Controller& aController) const override{
            return Fuzzy::Or(
                reinterpret_cast<const IfStatement*>(mLeft)->operator()(aFuzzifier, aController),
                reinterpret_cast<const IfStatement*>(mRight)->operator()(aFuzzifier, aController)
            );
        }
    };

    class Line : public Command{
    private:
        const Command* const mBody;
        const ThenStatement* const mThen;
    public:
        static Line* Compile(const char* aCode){
            //! \TODO Implement Line.Compile
            return nullptr;
        }

        Line(const IfStatement* const aBody, const ThenStatement* aOutput) :
            mBody(aBody),
            mThen(aOutput)
        {

        }

        Line(const BinaryOperator* const aBody, const ThenStatement* aOutput) :
            mBody(aBody),
            mThen(aOutput)
        {

        }

        ~Line(){
            delete mBody;
            delete mThen;
        }

        void operator()(Fuzzifier& aFuzzifier, const Controller& aController) const{
            const BinaryOperator* const bin = dynamic_cast<const BinaryOperator*>(mBody);
            const IfStatement* const _if = dynamic_cast<const IfStatement*>(mBody);

            truth_t output = 0.f;
            if(bin){
                output = bin->operator()(aFuzzifier, aController);
            }else if(_if){
                output = _if->operator()(aFuzzifier, aController);
            }
            mThen->operator()(aFuzzifier, output);
        }
    };

    class Script : public Command{
    private:
        const std::vector<const Line*> mLines;
    public:
        static Script* Compile(const char* aCode){
            //! \TODO Implement Script.Compile
            return nullptr;
        }

        template<typename iterator>
        Script(iterator aBegin, iterator aEnd):
            mLines(aBegin, aEnd)
        {

        }

        Script(const std::initializer_list<const Line*> aLines):
            mLines(aLines)
        {

        }

        ~Script(){
            for(const Line* line : mLines){
                delete line;
            }
        }

        truth_t operator()(Fuzzifier& aFuzzifier, const Controller& aController) const{
            for(const Line* line : mLines){
                line->operator()(aFuzzifier, aController);
            }
        }
    };
}}}


#endif
