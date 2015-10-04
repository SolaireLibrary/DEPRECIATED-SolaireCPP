#ifndef SOLAIRE_INIT_DETECT_COMPILER
#define SOLAIRE_INIT_DETECT_COMPILER

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
	\file DetectCompiler.inl
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 22nd September 2015
	Last Modified	: 22nd September 2015
*/

// Pre-checks

#ifdef SOLAIRE_COMPILER
    #error SolaireCPP : SOLAIRE_COMPILER was defined before DetectCompiler.inl was included
#endif

// Detect compiler and include config file

#ifdef _MSC_VER
    #include "Compiler/MSVC.inl"
#endif

#ifdef __clang__
    #include "Compiler/Clang.inl"
#endif

#ifdef __GNUC__
    #include "Compiler/GCC.inl"
    #if defined(__MINGW32__) || defined(__MINGW64__)
        #include "Compiler/MinGW.inl"
    #endif
#endif

// Post-checks

#ifndef SOLAIRE_COMPILER
    #error SolaireCPP : DetectCompiler.inl could not detect the current compiler
#endif


#endif
