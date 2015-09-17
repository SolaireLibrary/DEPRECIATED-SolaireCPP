#ifndef SOLAIRE_LOGIC_FUZZY_CONTROLLER_HPP
#define SOLAIRE_LOGIC_FUZZY_CONTROLLER_HPP

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
	\file Controller.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 10th September 2015
	Last Modified	: 10th September 2015
*/

#include <string>
#include "Internals.hpp"

namespace Solaire{ namespace Logic{ namespace Fuzzy{

    class Controller{
    public:
        virtual ~Controller();
        virtual truth_t CalculateMembership(const std::string& aMembership, const truth_t aInput) const = 0;
        virtual truth_t GetInput(const std::string& aInput) const = 0;
        virtual void SetOutput(const std::string& aOutput, const truth_t aValue) = 0;
    };

}}}


#endif
