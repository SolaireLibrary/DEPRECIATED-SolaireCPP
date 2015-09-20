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

#define SOLAIRE_WINDOWS 0
#define SOLAIRE_LINUX 1
#define SOLAIRE_ERROR -1

#ifdef _WIN32
    #define SOLAIRE_OS SOLAIRE_WINDOWS
    #define SOLAIRE_OS_BITS 32
#endif

#ifdef _WIN64
    #define SOLAIRE_OS SOLAIRE_WINDOWS
    #define SOLAIRE_OS_BITS 64
#endif

#ifndef SOLAIRE_OS
    #error SolaireCPP : Could not determine target OS
#endif

#ifndef SOLAIRE_OS_BITS
    #error SolaireCPP : Could not determine target bit size
#endif

#define SOLAIRE_EXCEPTION(aName, aMessage)\
class aName : public std::exception {\
public:\
	const char* what() const override {\
		return aMessage;\
	}\
};

#define solaire_runtime_assert(aCondition, aMessage) if(! (aCondition)) throw std::runtime_error(aMessage);

#define solaire_synchronized(aLock, aCode)\
{\
	std::lock_guard<decltype(aLock)> _solaire_guard(aLock);\
	aCode\
}

#endif
