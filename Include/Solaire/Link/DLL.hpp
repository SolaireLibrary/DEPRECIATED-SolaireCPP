#ifndef SOLAIRE_DLL_HPP
#define SOLAIRE_DLL_HPP

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
\file Library.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 9th November 2015
Last Modified	: 11th November 2015
*/

#include <map>
#include <string>
#include "SharedLibrary.hpp"

#if SOLAIRE_OS != SOLAIRE_WINDOWS
	#error SolaireCPP : DLL class is only avaliable on Windows
#endif

namespace Solaire{
	class DLL : public SharedLibrary{
	private:
		HMODULE mLibrary;
		std::map<std::string, FunctionPointer*> mFunctions;
	private:
		DLL(const DLL&);
		DLL& operator=(const DLL&);
	public:
		DLL();
		DLL(DLL&& Library);
		~DLL();
		
		DLL& operator=(DLL&&);

		// Inherited from SharedLibrary

		void DefineFunction(const char* const, FunctionPointer&) override;

		void Load(const char* const) override;
		void Unload() override;
		bool IsLoaded() const override;
	};
}

#endif
