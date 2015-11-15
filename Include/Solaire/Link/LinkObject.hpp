#ifndef SOLAIRE_LINK_OBJECT_HPP
#define SOLAIRE_LINK_OBJECT_HPP

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
\file LinkObject.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 9th November 2015
Last Modified	: 11th November 2015
*/

#include <cstdint>
#include "Link.inl"

namespace Solaire{
	class SOLAIRE_EXPORT_API LinkObject{
	public:
		typedef uint32_t ErrorCode;
		
		enum : ErrorCode{
			DLL_NO_ERROR,
			DLL_RELEASED_UNREFERENCED_OBJECT,
			DLL_ERROR_OVERFLOW
		};
	private:
		//LinkObject(const LinkObject&) = delete;
		//LinkObject(LinkObject&&) = delete;
		//LinkObject& operator=(const LinkObject&) = delete;
		//LinkObject& operator=(LinkObject&&) = delete;
	public:
		virtual void SOLAIRE_EXPORT_CALL CreateReference() = 0;
		virtual void SOLAIRE_EXPORT_CALL ReleaseReference() = 0;
		virtual uint32_t SOLAIRE_EXPORT_CALL GetReferenceCount() const = 0;
		virtual ErrorCode SOLAIRE_EXPORT_CALL GetError() = 0;
	};
}

#endif
