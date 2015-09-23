#ifndef SOLAIRE_CORE_INIT_HPP
#define SOLAIRE_CORE_INIT_HPP

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
	\file Init.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 22nd September 2015
	Last Modified	: 22nd September 2015
*/

// Misc
#define SOLAIRE_ERROR -1

// SOLAIRE_OS

#define SOLAIRE_WINDOWS 0
#define SOLAIRE_LINUX 1

// SOLAIRE_COMPILER

#define SOLAIRE_MSVC 0
#define SOLAIRE_GCC 1
#define SOLAIRE_MINGW 2
#define SOLAIRE_CLANG 3

// Detect environment

#include "Init/DetectCompiler.inl"
#include "Init/DetectOS.inl"

// Misc
#ifndef SOLAIRE_THREADLOCAL
    #ifdef SOLAIRE_DISABLE_MULTITHREADING
        #define SOLAIRE_THREADLOCAL
    #else
        #define SOLAIRE_THREADLOCAL thread_local
    #endif
#endif

#define SOLAIRE_ENABLE_IF(aMods, aCondition, aReturn)\
template<class RETURN_TYPE = aReturn>\
aMods typename std::enable_if<aCondition, RETURN_TYPE>::type

#include <cstdint>

namespace Solaire{
    typedef uint16_t ErrorCode;

    enum : ErrorCode{
        ERROR_NO_ERROR = 0,
        ERROR_INDEX_OUT_OF_BOUNDS = 1,
        ERROR_NULL_POINTER = 2
    };
}

#endif
