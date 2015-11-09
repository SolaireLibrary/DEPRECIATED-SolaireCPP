#ifndef SOLAIRE_DLL_INL
#define SOLAIRE_DLL_INL

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
\file Dll.inl
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 9th November 2015
Last Modified	: 9th November 2015
*/

#include "..\Init.hpp"

#if SOLAIRE_OS == SOLAIRE_WINDOWS
	#define SOLAIRE_DLL_IMPORT __declspec(dllimport)
	#define SOLAIRE_DLL_EXPORT __declspec(dllexport)
#else
	#define SOLAIRE_DLL_IMPORT
	#define SOLAIRE_DLL_EXPORT
#endif

#ifndef SOLAIRE_DLL_API
	#define SOLAIRE_DLL_API SOLAIRE_DLL_IMPORT
#endif

#endif