#ifndef SOLAIRE_GL_OBJECT_HPP
#define SOLAIRE_GL_OBJECT_HPP

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

#include "GLHeader.hpp"

namespace Solaire {
	class Object {
	public:
		enum ID : GLuint {
			NULL_ID = 0
		};
	public:
		virtual bool SOLAIRE_EXPORT_CALL Create() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Destroy() throw() = 0;
		virtual ID SOLAIRE_EXPORT_CALL GetID() const throw() = 0;
		virtual SOLAIRE_EXPORT_CALL ~Object() throw() {}

		inline bool IsCreated() const throw() {
			return GetID() != NULL_ID;
		}
	};

	class Shader : public Object {

	};

	class Program : public Object {

	};

	class Texture : public Object {
	public:
		class Params;
	};

	class Texture1D : public Object {
	public:
		class Data;
	};

	class Texture2D : public Object {
	public:
		class Data;
	};

	class Texture3D : public Object {
	public:
		class Data;
	};
}


#endif
