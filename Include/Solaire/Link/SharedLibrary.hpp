#ifndef SOLAIRE_SHARED_LIBRARY_HPP
#define SOLAIRE_SHARED_LIBRARY_HPP

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
	Created			: 3rd December 2015
	Last Modified	: 22nd December 2015
*/

#include "..\Core\ModuleHeader.hpp"

namespace Solaire {

	class Allocator;

	template<class RETURN, class... PARAMS>
	struct _SharedLibraryFunction {
		typedef RETURN(SOLAIRE_EXPORT_CALL *Type)(PARAMS...);
	};

	template<class RETURN, class... PARAMS>
	using SharedLibraryFunction = typename _SharedLibraryFunction<RETURN, PARAMS...>::type;

	SOLAIRE_EXPORT_INTERFACE SharedLibrary {
	protected:
		typedef void(SOLAIRE_EXPORT_CALL *FunctionPtr)();
	protected:
		virtual FunctionPtr SOLAIRE_EXPORT_CALL _LoadFunction(const char* const) const throw() = 0;
	public:
		virtual bool SOLAIRE_EXPORT_CALL Open(const char* const) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Close() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL IsOpen() const throw() = 0;
		virtual Allocator& SOLAIRE_EXPORT_CALL GetAllocator() const throw() = 0;
		virtual SOLAIRE_EXPORT_CALL ~SharedLibrary(){}

		template<class RETURN, class... PARAMS>
		SOLAIRE_FORCE_INLINE SharedLibraryFunction<RETURN, PARAMS...> SOLAIRE_DEFAULT_CALL LoadFunction(const char* const aName) {
			return reinterpret_cast<SharedLibraryFunction<RETURN, PARAMS...>>(_LoadFunction(aName));
		}
	};

	extern "C" {
		SOLAIRE_EXPORT_API SharedLibrary* SOLAIRE_EXPORT_CALL CreateSharedLibrary(Allocator&);
	}
}


#endif
