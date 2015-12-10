#ifndef SOLAIRE_GL_HEADER_HPP
#define SOLAIRE_GL_HEADER_HPP

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
\file GlHeader.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 9th December 2015
Last Modified	: 9th December 2015
*/

#include "..\ModuleHeader.hpp"
#include "..\..\Core\ModuleHeader.hpp"

#define SOLAIRE_GL_VER_EQ(aMajor, aMinor) (SOLAIRE_GL_VER_MAJOR == aMajor && SOLAIRE_GL_VER_MINOR == aMinor)
#define SOLAIRE_GL_VER_LT(aMajor, aMinor) (SOLAIRE_GL_VER_MAJOR < aMajor || (SOLAIRE_GL_VER_MAJOR == aMajor && SOLAIRE_GL_VER_MINOR < aMinor))
#define SOLAIRE_GL_VER_LTE(aMajor, aMinor) (SOLAIRE_GL_VER_MAJOR < aMajor || (SOLAIRE_GL_VER_MAJOR == aMajor && SOLAIRE_GL_VER_MINOR <= aMinor))
#define SOLAIRE_GL_VER_GT(aMajor, aMinor) (SOLAIRE_GL_VER_MAJOR > aMajor || (SOLAIRE_GL_VER_MAJOR == aMajor && SOLAIRE_GL_VER_MINOR > aMinor))
#define SOLAIRE_GL_VER_GTE(aMajor, aMinor) (SOLAIRE_GL_VER_MAJOR > aMajor || (SOLAIRE_GL_VER_MAJOR == aMajor && SOLAIRE_GL_VER_MINOR >= aMinor))

#define SOLAIRE_GL_VER_MAJOR 3
#define SOLAIRE_GL_VER_MINOR 0

#if SOLAIRE_GL_VER_MAJOR == 4
	#if SOLAIRE_GL_VER_MINOR == 5
		#include "GLLoadGen\gl_core_4.5.h"
	#endif
	#if SOLAIRE_GL_VER_MINOR == 4
		#include "GLLoadGen\gl_core_4.4.h"
	#endif
	#if SOLAIRE_GL_VER_MINOR == 3
		#include "GLLoadGen\gl_core_4.3.h"
	#endif
	#if SOLAIRE_GL_VER_MINOR == 2
		#include "GLLoadGen\gl_core_4.2.h"
	#endif
	#if SOLAIRE_GL_VER_MINOR == 1
		#include "GLLoadGen\gl_core_4.1.h"
	#endif
	#if SOLAIRE_GL_VER_MINOR == 0
		#include "GLLoadGen\gl_core_4.0.h"
	#endif
#endif

#if SOLAIRE_GL_VER_MAJOR == 3
	#if SOLAIRE_GL_VER_MINOR == 3
		#include "GLLoadGen\gl_core_3.3.h"
	#endif
	#if SOLAIRE_GL_VER_MINOR == 2
		#include "GLLoadGen\gl_core_3.2.h"
	#endif
	#if SOLAIRE_GL_VER_MINOR == 1
		#include "GLLoadGen\gl_core_3.1.h"
	#endif
	#if SOLAIRE_GL_VER_MINOR == 0
		#include "GLLoadGen\gl_core_3.0.h"
	#endif
#endif



#endif
