#ifndef SOLAIRE_RESOURCE_FUNCTIONAL_HPP
#define SOLAIRE_RESOURCE_FUNCTIONAL_HPP

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
	\file ResourceFunctional.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 11th December 2015
	Last Modified	: 11th December 2015
*/

#include "Resource.hpp"

namespace Solaire {

	template<typename ID_TYPE, class TYPE, class F>
	class FunctionalResource : public Resource<ID_TYPE, TYPE> {
	public:
		F Function;
	private:
		F mFunction;
	protected:
		// Inherited from Resource
		SharedAllocation<TYPE> SOLAIRE_EXPORT_CALL CreateResource() throw() override {
			return mFunction();
		}
	public:
		FunctionalResource(const ID aID, const F aFunction) :
			Resource(aID),
			mFunction(aFunction)
		{}

		SOLAIRE_EXPORT_CALL ~FunctionalResource() throw() {

		}
	};
    
}


#endif
