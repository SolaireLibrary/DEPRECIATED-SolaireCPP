#ifndef SOLAIRE_DLL_SHARED_LIBRARY_HPP
#define SOLAIRE_DLL_SHARED_LIBRARY_HPP

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
\file SharedLibrary.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 11th November 2015
Last Modified	: 11th November 2015
*/

#include "Link.inl"

namespace Solaire{
	class SharedLibrary{
	public:
#if SOLAIRE_OS == SOLAIRE_WINDOWS
		typedef int(__stdcall  *FunctionPointer)();
#else
		typedef int(*FunctionPointer)();
#endif
	public:
		virtual ~SharedLibrary(){}

		virtual void DefineFunction(const char* const, FunctionPointer&) = 0;

		virtual void Load(const char* const) = 0;
		virtual void Unload() = 0;
		virtual bool IsLoaded() const = 0;
	};
}

#endif
