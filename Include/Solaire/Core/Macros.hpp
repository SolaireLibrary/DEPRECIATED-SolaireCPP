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
	Last Modified	: 14th September 2015
*/

#define SOLAIRE_ERROR -1

// Detect OS config

#define SOLAIRE_WINDOWS 0
#define SOLAIRE_LINUX 1

#define SOLAIRE_OS SOLAIRE_ERROR
#define SOLAIRE_OS_BITS SOLAIRE_ERROR

#ifdef _WIN32
    #undef SOLAIRE_OS
    #undef SOLAIRE_OS_BITS

    #define SOLAIRE_OS SOLAIRE_WINDOWS
    #define SOLAIRE_OS_BITS 32
#endif

#ifdef _WIN64
    #undef SOLAIRE_OS
    #undef SOLAIRE_OS_BITS

    #define SOLAIRE_OS SOLAIRE_WINDOWS
    #define SOLAIRE_OS_BITS 64
#endif

#if SOLAIRE_OS == SOLAIRE_ERROR
    #error SolaireCPP : Could not determine target OS
#endif

#if SOLAIRE_OS_BITS == SOLAIRE_ERROR
    #error SolaireCPP : Could not determine target bit size
#endif

// Detect compiler

#define SOLAIRE_MSVC 0
#define SOLAIRE_GCC 1
#define SOLAIRE_MINGW 2

#define SOLAIRE_COMPILER SOLAIRE_ERROR
#define SOLAIRE_COMPILER_VERSION SOLAIRE_ERROR

#ifdef _MSC_VER
    #undef SOLAIRE_COMPILER
    #undef SOLAIRE_COMPILER_VERSION

    #define SOLAIRE_COMPILER SOLAIRE_MSVC
    #define SOLAIRE_COMPILER_VERSION _MSC_VER
#endif

#ifdef __GNUC__
    #undef SOLAIRE_COMPILER
    #undef SOLAIRE_COMPILER_VERSION

    #define SOLAIRE_COMPILER SOLAIRE_GCC
    #define SOLAIRE_COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#endif

#ifdef __MINGW32__
    #undef SOLAIRE_COMPILER
    #undef SOLAIRE_COMPILER_VERSION

    #define SOLAIRE_COMPILER SOLAIRE_MINGW
    #define SOLAIRE_COMPILER_VERSION (__MINGW32_MAJOR_VERSION * 100 + __MINGW32_MINOR_VERSION)
#endif

#ifdef __MINGW64__
    #undef SOLAIRE_COMPILER
    #undef SOLAIRE_COMPILER_VERSION

    #define SOLAIRE_COMPILER SOLAIRE_MINGW
    #define SOLAIRE_COMPILER_VERSION (__MINGW64_MAJOR_VERSION * 100 + __MINGW64_MINOR_VERSION)
#endif

// Compiler specific settings

#if SOLAIRE_COMPILER == SOLAIRE_MINGW
    #define SOLAIRE_DISABLE_MULTITHREADING
    #warning SolaireCPP : Multithreading is disabled when compiling with MinGW
#endif


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
