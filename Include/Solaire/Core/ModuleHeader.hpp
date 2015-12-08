#ifndef SOLAIRE_MODULE_HEADER_CORE_HPP
#define SOLAIRE_MODULE_HEADER_CORE_HPP

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
	\file ModuleHeader.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 8th December 2015
	Last Modified	: 8th December 2015
*/

#define SOLAIRE_MODULE_CORE 1.0

// Misc
#define SOLAIRE_ERROR -1

// SOLAIRE_OS

#define SOLAIRE_WINDOWS 0
#define SOLAIRE_LINUX 1
#define SOLAIRE_BSD 2
#define SOLAIRE_MAC 3
#define SOLAIRE_ANDROID 4
#define SOLAIRE_IOS 5

// SOLAIRE_COMPILER

#define SOLAIRE_MSVC 0
#define SOLAIRE_GCC 1
#define SOLAIRE_MINGW 2
#define SOLAIRE_CLANG 3

// Detect environment

#include "Init/DetectCompiler.inl"
#include "Init/DetectOS.inl"

// Calling conventions

#ifndef SOLAIRE_DEFAULT_CALL
	#define SOLAIRE_DEFAULT_CALL SOLAIRE_OS_DEFAULT_CALL
#endif

#ifndef SOLAIRE_DEFAULT_API
	#define SOLAIRE_DEFAULT_API SOLAIRE_OS_DEFAULT_API
#endif

#ifndef SOLAIRE_EXPORT_API
	#define SOLAIRE_EXPORT_API SOLAIRE_OS_IMPORT_API
#endif

#ifndef SOLAIRE_EXPORT_CALL
	#define SOLAIRE_EXPORT_CALL SOLAIRE_DEFAULT_CALL
#endif

#endif
