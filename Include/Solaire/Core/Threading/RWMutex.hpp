#ifndef SOLAIRE_RW_MUTEX_HPP
#define SOLAIRE_RW_MUTEX_HPP

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
	\file RWMutex.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 2.0
	\date
	Created			: 4th October 2015
	Last Modified	: 4th October 2015
*/

#include "ThreadTypes.hpp"


namespace Solaire{

    template<class Mutex>
    class RWMutex{
        RWMutex(const RWMutex&) = delete;
        RWMutex(RWMutex&&) = delete;

        RWMutex operator=(const RWMutex&) = delete;
        RWMutex operator=(RWMutex&&) = delete;
    private:
    public:
    };

}


#endif
