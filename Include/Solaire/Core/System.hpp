#ifndef SOLAIRE_SYSTEM_HPP
#define SOLAIRE_SYSTEM_HPP

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
	\file System.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 3rd December 2015
	Last Modified	: 3rd December 2015
*/

#include <cstdint>
#include "ModuleHeader.hpp"
#include "Init.hpp"
#include "Iterator.hpp"

namespace Solaire {


    static constexpr char* RunExecutable_FnName = "_RunExecutable@16";
    static constexpr char* GetTimeMilliseconds_FnName = "_GetTimeMilliseconds@0";

	#ifdef SOLAIRE_EXPORT_IMPORT_LIBRARY
        extern "C" SOLAIRE_EXPORT_API bool SOLAIRE_EXPORT_CALL _RunExecutable(const char* const, Iterator<const char*>&, const Iterator<const char*>&, int* const) throw();
		extern "C" SOLAIRE_EXPORT_API uint64_t SOLAIRE_EXPORT_CALL _GetTimeMilliseconds() throw();

        static SOLAIRE_FORCE_INLINE bool RunExecutable(const char* const aPath, Iterator<const char*>& aBegin, const Iterator<const char*>& aEnd, int* const aReturnCode) {
            return _RunExecutable(aPath, aBegin, aEnd, aReturnCode);
        }

        static SOLAIRE_FORCE_INLINE uint64_t GetTimeMilliseconds() {
            return _GetTimeMilliseconds();
        }
    #else
        static bool (SOLAIRE_EXPORT_CALL *RunExecutable)();
        static uint64_t (SOLAIRE_EXPORT_CALL *GetTimeMilliseconds)(const char* const, Iterator<const char*>&, const Iterator<const char*>&, int* const);
    #endif
}


#endif
