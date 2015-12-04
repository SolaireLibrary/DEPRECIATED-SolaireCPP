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

namespace Solaire { namespace File{


	typedef uint8_t AttributeFlags;

	enum : AttributeFlags {
		FLAG_READ			= BIT_0,
		FLAG_WRITE			= BIT_1,
		FLAG_EXECUTABLE		= BIT_2,
		FLAG_HIDDEN			= BIT_3,
		FLAG_FILE			= BIT_4,
		FLAG_DIRECTORY		= BIT_5,
		FLAG_EXISTS			= BIT_6,

		FLAG_NONE			= 0,
		FLAG_READ_WRITE		= FLAG_READ | FLAG_WRITE
	};

	enum {
		MAX_PATH_LENGTH = 512
	};

	enum : char {
		#if SOLAIRE_OS == SOLAIRE_WINDOWS
			FILE_SEPERATOR = '\\'
		#else
			FILE_SEPERATOR = '/'
		#endif
	};

	#undef Createfile
	#undef CreateDirectory
	#undef GetCurrentDirectory

	extern "C" {
		SOLAIRE_EXPORT_API AttributeFlags SOLAIRE_EXPORT_CALL GetAttributes(const char* const) throw();

		SOLAIRE_EXPORT_API bool SOLAIRE_EXPORT_CALL CreateFile(const char* const, const AttributeFlags) throw();
		SOLAIRE_EXPORT_API bool SOLAIRE_EXPORT_CALL CreateDirectory(const char* const) throw();
		SOLAIRE_EXPORT_API bool SOLAIRE_EXPORT_CALL Delete(const char* const) throw();

		SOLAIRE_EXPORT_API uint32_t SOLAIRE_EXPORT_CALL GetParent(const char* const aFilename, char* const aOutput) throw();
		SOLAIRE_EXPORT_API uint32_t SOLAIRE_EXPORT_CALL GetName(const char* const, char* const) throw();
		SOLAIRE_EXPORT_API uint32_t SOLAIRE_EXPORT_CALL GetExtension(const char* const, char* const) throw();

		SOLAIRE_EXPORT_API uint32_t SOLAIRE_EXPORT_CALL Size(const char* const) throw();

		SOLAIRE_EXPORT_API uint32_t SOLAIRE_EXPORT_CALL GetFileList(const char* const, const char** const, const uint32_t) throw();
		SOLAIRE_EXPORT_API uint32_t SOLAIRE_EXPORT_CALL GetCurrentDirectory(char* const) throw();
		SOLAIRE_EXPORT_API uint32_t SOLAIRE_EXPORT_CALL GetTemporaryDirectory(char* const) throw();

		SOLAIRE_EXPORT_API bool SOLAIRE_EXPORT_CALL Rename(const char* const, const char* const) throw();
		SOLAIRE_EXPORT_API bool SOLAIRE_EXPORT_CALL Copy(const char* const, const char* const) throw();
		SOLAIRE_EXPORT_API bool SOLAIRE_EXPORT_CALL Move(const char* const, const char* const) throw();
	}
}}

#endif
