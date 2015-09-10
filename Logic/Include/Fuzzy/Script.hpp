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

    class ContainerCommand : public Command{
    public:
        virtual ~ContainerCommand(){

        }

        virtual void operator()(Controller& aController) const = 0;
    };

    template<typename R>
    class FunctionCommand : public Command{
    public:
        virtual ~FunctionCommand(){

        }

        virtual R operator()(Controller& aController) const = 0;
    };

    template<typename R>
    class OutputCommand : public Command{
    public:
        virtual ~OutputCommand(){

        }

        virtual void operator()(Controller& aController, const R aCommand) const = 0;
    };

    class Input : public FunctionCommand<truth_t>{
    private:
        const std::string mInput;
    public:
        Input(const std::string aInput) :
            mInput(aInput)
        {

        }

        static Input* Compile(const char* aCodeBegin){
            // Locate Keywords
            const char* _if = std::strstr(aCodeBegin, "IF");
            if(! _if) return nullptr;

            // Locate values
            const char* inputBegin = FindWordBegin(_if + 2);
            const char* inputEnd = FindWordEnd(inputEnd);

            if(! (inputBegin && inputEnd)) return nullptr;

            // Build command
            return new Input(
                std::string(inputBegin, inputEnd - inputBegin)
            );
        }

        // Inherited

        truth_t operator()(Controller& aController) const override{
            return aController.GetInput(mInput);
        }
    };

    class Then : public OutputCommand<truth_t>{
    private:
        const std::string mOutput;
    public:
        Then(const std::string aOutput) :
            mOutput(aOutput)
        {

        }

        static Then* Compile(const char* aCodeBegin){
            // Locate Keywords
            const char* _then = std::strstr(aCodeBegin, "THEN");

            if(! _then) return nullptr;

            // Locate values
            const char* outputBegin = FindWordBegin(_then + 4);
            const char* outputEnd = FindWordEnd(outputBegin);

            if(! (outputBegin && outputEnd)) return nullptr;

            // Build command
            return new Then(
                std::string(outputBegin, outputEnd - outputBegin)
            );
        }

        // Inherited

        void operator()(Controller& aFuzzifier, const truth_t aValue) const override{
            return aFuzzifier.SetOutput(mOutput, aValue);
        }
    };

    template<typename R>
    class UnaryOperator : public FunctionCommand<R>{
    protected:
        const FunctionCommand<R>* const mBody;
    public:
        UnaryOperator(const FunctionCommand<R>* const aBody) :
            mBody(aBody)
        {

        }

        virtual ~UnaryOperator(){
            delete mBody;
        }
    };

    template<typename R>
    class BinaryOperator : public FunctionCommand<R>{
    protected:
        const FunctionCommand<R>* const mLeft;
        const FunctionCommand<R>* const mRight;
    public:
        template<class T>
        static T* Compile(const char* aCode){
            static_assert(std::is_base_of<Command, T>::value, "Must inherit from BinaryOperator");
            //! \TODO Implement BinaryOperator.Compile
            return nullptr;
        }

        BinaryOperator(const FunctionCommand<R>* const aLeft, const FunctionCommand<R>* const aRight) :
            mLeft(aLeft),
            mRight(aRight)
        {

        }

        virtual ~BinaryOperator(){
            delete mLeft;
            delete mRight;
        }
    };

    class Not : public UnaryOperator<truth_t>{
    public:
        static Not* Compile(const char* aCode){
            //! \TODO Implement Not.Compile
            return nullptr;
        }

        Not(const FunctionCommand<truth_t>* const aBody) :
            UnaryOperator(aBody)
        {

        }

        // Inherited

        truth_t operator()(Controller& aController) const override{
            return Fuzzy::Not(mBody->operator()(aController));
        }
    };

    class Brackets : public UnaryOperator<truth_t>{
    public:
        static Brackets* Compile(const char* aCode){
            //! \TODO Implement Brackets.Compile
            return nullptr;
        }

        Brackets(const FunctionCommand<truth_t>* const aBody) :
            UnaryOperator(aBody)
        {

        }

        // Inherited

        truth_t operator()(Controller& aController) const override{
            return mBody->operator()(aController);
        }
    };

    class And : public BinaryOperator<truth_t>{
    public:
        static And* Compile(const char* aCode){
            //! \TODO Implement And.Compile
            return nullptr;
        }

        And(const FunctionCommand<truth_t>* const aLeft, const FunctionCommand<truth_t>* const aRight) :
            BinaryOperator(aLeft, aRight)
        {

        }

        // Inherited

        truth_t operator()(Controller& aController) const override{
            return Fuzzy::And(
                mLeft->operator()(aController),
                mRight->operator()(aController)
            );
        }
    };

    class Or : public BinaryOperator<truth_t>{
    public:
        static Or* Compile(const char* aCode){
            //! \TODO Implement Or.Compile
            return nullptr;
        }

        Or(const FunctionCommand<truth_t>* const aLeft, const FunctionCommand<truth_t>* const aRight) :
            BinaryOperator(aLeft, aRight)
        {

        }

        // Inherited

        truth_t operator()(Controller& aController) const override{
            return Fuzzy::Or(
                mLeft->operator()(aController),
                mRight->operator()(aController)
            );
        }
    };

    //! \TODO implement Not as seperate command
    //! \TODO impelement Bracket command

    class Line : public ContainerCommand{
    private:
        const FunctionCommand<truth_t>* const mBody;
        const OutputCommand<truth_t>* const mOutput;
    public:
        static Line* Compile(const char* aCode){
            //! \TODO Implement Line.Compile
            return nullptr;
        }

        Line(const FunctionCommand<truth_t>* const aBody, const OutputCommand<truth_t>* aOutput) :
            mBody(aBody),
            mOutput(aOutput)
        {

        }

        ~Line(){
            delete mBody;
            delete mOutput;
        }

        // Inherited

        void operator()(Controller& aController) const override{
            mOutput->operator()(aController, mBody->operator()(aController));
        }
    };

    class Script : public ContainerCommand{
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

        // Inherited

        void operator()(Controller& aController) const override{
            for(const Line* line : mLines){
                line->operator()(aController);
            }
        }
    };
}}}


#endif
