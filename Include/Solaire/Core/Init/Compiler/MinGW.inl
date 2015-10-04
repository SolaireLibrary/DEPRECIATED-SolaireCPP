#ifndef SOLAIRE_INIT_MINGW_INL
#define SOLAIRE_INIT_MINGW_INL

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
	\file MinGW.inl
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 22nd September 2015
	Last Modified	: 22nd September 2015
*/

#undef SOLAIRE_COMPILER
#define SOLAIRE_COMPILER SOLAIRE_MINGW

#ifndef SOLAIRE_DISABLE_MULTITHREADING
    #define SOLAIRE_DISABLE_MULTITHREADING
    #warning SolaireCPP : Multithreading is disabled when compiling with MinGW
#endif

#endif
