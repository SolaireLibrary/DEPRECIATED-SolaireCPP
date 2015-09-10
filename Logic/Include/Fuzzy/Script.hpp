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
    public:
        const std::string Input;
        const std::string Output;
        const std::string MembershipFunction;

        IfStatement(const std::string aInput, const std::string aMembership, const std::string aOutput) :
            Input(aInput),
            Output(aOutput),
            MembershipFunction(aMembership)
        {

        }

        static IfStatement* Compile(const char* aCodeBegin){
            // Locate Keywords
            const char* _if = std::strstr(aCodeBegin, "IF");
            const char* _is = _if ? std::strstr(_if, "IS") : nullptr;
            const char* _then = _is ? std::strstr(_is, "THEN") : nullptr;

            if(! (_if && _is && _then)) return nullptr;

            // Locate values
            const char* inputBegin = FindWordBegin(_if + 2);
            const char* inputEnd = FindWordEnd(inputEnd);
            const char* membershipBegin = FindWordBegin(_is + 2);
            const char* membershipEnd = FindWordEnd(membershipBegin);
            const char* outputBegin = FindWordBegin(_then + 4);
            const char* outputEnd = FindWordEnd(outputBegin);

            if(! (inputBegin && inputEnd && membershipBegin && membershipEnd && outputBegin && outputEnd)) return nullptr;

            // Build command
            return new IfStatement(
                std::string(inputBegin, inputEnd - inputBegin),
                std::string(membershipBegin, membershipEnd - membershipBegin),
                std::string(outputBegin, outputEnd - outputBegin)
            );
        }
    };

    class Line : public Command{
    private:

    };

    class Script : public Command{
    private:

    };
}}}


#endif
