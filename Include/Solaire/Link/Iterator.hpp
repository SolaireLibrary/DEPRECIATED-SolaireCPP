#ifndef SOLAIRE_COLLECTION_HPP
#define SOLAIRE_COLLECTION_HPP

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
\file Collection.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 11th November 2015
Last Modified	: 15th November 2015
*/

#include <cstdint>
#include "Object.hpp"

namespace Solaire{namespace Link{

	template<class T>
	class SOLAIRE_EXPORT_API Iterator : public Object{
	public:
		virtual bool HasPrevious() const = 0;
		virtual bool HasNext() const = 0;
		virtual uint32_t GetIndex() const = 0;
		virtual T& Peak() const = 0;
		virtual T& Next() = 0;
		virtual T& Previous() = 0;
	};
}}

#endif
