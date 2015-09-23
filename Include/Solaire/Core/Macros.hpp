#ifndef SOLAIRE_CORE_MACROS_HPP
#define SOLAIRE_CORE_MACROS_HPP

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
	\file Macros.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 13th September 2015
	Last Modified	: 22nd September 2015
*/

#include "Init.hpp"

#define SOLAIRE_EXCEPTION(aName, aMessage)\
class aName : public std::exception {\
public:\
	const char* what() const override {\
		return aMessage;\
	}\
};

#define solaire_runtime_assert(aCondition, aMessage) if(! (aCondition)) throw std::runtime_error(aMessage)
#define solaire_static_assert static_assert(aCondition, aMessage)

#ifndef SOLAIRE_DISABLE_MULTITHREADING
    #define solaire_synchronized(aLock, aCode)\
    {\
        std::lock_guard<decltype(aLock)> _solaire_guard(aLock);\
        aCode\
    }
#else
    #define solaire_synchronized(aLock, aCode)\
    {\
        aCode\
    }
#endif

#endif
