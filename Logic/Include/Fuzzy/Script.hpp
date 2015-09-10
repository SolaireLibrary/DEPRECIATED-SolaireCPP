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

namespace Solaire{ namespace Logic{ namespace Fuzzy{
    class System;

    namespace Commands{

        class Command(){
        public:
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

            static IfStatement* Compile(const char* aCode){
                // Locate Keywords
                const char* _if;
                const char* _is;
                const char* _then;

                // Locate values
                const char* inputBegin;
                const char* inputEnd;
                const char* membershipBegin;
                const char* membershipEnd;
                const char* outputBegin;
                const char* outputEnd;

                // Build command
                return new IfStatement(
                    std::string(inputBegin, inputEnd - inputBegin),
                    std::string(membershipBegin, membershipEnd - membershipBegin),
                    std::string(outputBegin, outputEnd - outputBegin)
                );
            }
        };
    }

    class Script{
    private:

    };
}}}


#endif
