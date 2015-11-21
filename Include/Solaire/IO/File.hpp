#ifndef SOLAIRE_FILE_HPP
#define SOLAIRE_FILE_HPP

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
	\file File.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 20th November 2015
	Last Modified	: 20th November 2015
*/

#include <cstdint>
#include "..\Core\Maths.hpp"
#include "..\Strings\String.hpp"

#undef CreateFile
#undef CreateDirectory
#undef GetCurrentDirectory

namespace Solaire {
	
	struct File{

		typedef uint8_t Attributes;

		enum : Attributes {
			READ			= BIT_0,
			WRITE			= BIT_1,
			EXECUTABLE		= BIT_2,
			HIDDEN			= BIT_3,
			FILE			= BIT_4,
			DIRECTORY		= BIT_5,

			NO_ATTRIBUTES	= 0,
			READ_WRITE		= READ | WRITE,
		};

		enum : char {
			#if SOLAIRE_OS == SOLAIRE_WINDOWS
				FILE_SEPERATOR = '\\'
			#else
				FILE_SEPERATOR = '/'
			#endif
		};

		static Attributes SOLAIRE_EXPORT_CALL GetAttributes(const ConstStringFragment) throw();

		static bool SOLAIRE_EXPORT_CALL Exists(const ConstStringFragment) throw();
		static bool SOLAIRE_EXPORT_CALL CreateFile(const ConstStringFragment, const Attributes) throw();
		static bool SOLAIRE_EXPORT_CALL CreateDirectory(const ConstStringFragment) throw();
		static bool SOLAIRE_EXPORT_CALL Delete(const ConstStringFragment) throw();

		static String SOLAIRE_EXPORT_CALL GetParent(const ConstStringFragment) throw();
		static String SOLAIRE_EXPORT_CALL GetName(const ConstStringFragment) throw();
		static String SOLAIRE_EXPORT_CALL GetExtension(const ConstStringFragment) throw();

		static uint32_t SOLAIRE_EXPORT_CALL Size(const ConstStringFragment) throw();

		static DynamicArray<String> SOLAIRE_EXPORT_CALL GetFileList(const ConstStringFragment) throw();
		static String SOLAIRE_EXPORT_CALL GetCurrentDirectory() throw();
		static String SOLAIRE_EXPORT_CALL GetTemporaryDirectory() throw();

		static bool SOLAIRE_EXPORT_CALL Rename(const ConstStringFragment, const ConstStringFragment) throw();
		static bool SOLAIRE_EXPORT_CALL Copy(const ConstStringFragment, const ConstStringFragment) throw();
		static bool SOLAIRE_EXPORT_CALL Move(const ConstStringFragment, const ConstStringFragment) throw();
	};
}

#endif
