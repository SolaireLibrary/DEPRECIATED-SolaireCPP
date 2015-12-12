#ifndef SOLAIRE_RESOURCEI_HPP
#define SOLAIRE_RESOURCEI_HPP

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
	\file ResourceI.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 11th December 2015
	Last Modified	: 11th December 2015
*/

#include "..\Memory\Allocator.hpp"

namespace Solaire {

	template<typename ID_TYPE>
	class ResourceI {
	public:
		typedef ID_TYPE ID;
	public:
		virtual bool SOLAIRE_EXPORT_CALL Create() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Recreate() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Destroy() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL IsCreated() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL CompareID(const ID) const throw() = 0;
		virtual SOLAIRE_EXPORT_CALL ~ResourceI() throw() {}
	};
	
	template<typename ID_TYPE>
	using ResourceIPtr = SharedAllocation<ResourceI<ID_TYPE>>;

	template<typename ID_TYPE>
	using ConstResourceIPtr = SharedAllocation<const ResourceI<ID_TYPE>>;
}


#endif
