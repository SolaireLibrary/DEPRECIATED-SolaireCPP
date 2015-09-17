#ifndef SOLAIRE_LOGIC_FUZZY_INTERNALS_HPP
#define SOLAIRE_LOGIC_FUZZY_INTERNALS_HPP

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
	\file Internals.hpp
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
    typedef float truth_t;

    static constexpr truth_t TRUE = 1.f;
    static constexpr truth_t FALSE = 0.f;

    static constexpr truth_t Not(const truth_t aValue){
        return aValue >= 1.f ? 0.f : aValue <= 0.f ? 1.f : 1.f - aValue;
    }

    static constexpr truth_t And(const truth_t aLeft, const truth_t aRight){
        return aLeft <= aRight ? aLeft : aRight;
    }

    static constexpr truth_t Or(const truth_t aLeft, const truth_t aRight){
        return aLeft >= aRight ? aLeft : aRight;
    }
}}}


#endif
