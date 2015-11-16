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

#include <stdexcept>
#include "Solaire\Core\Init.hpp"

#if SOLAIRE_OS == SOLAIRE_WINDOWS

	#include "Solaire\Script\DLL.hpp"

	namespace Solaire{

		// DLL

		DLL::DLL() :
			mLibrary(nullptr)
		{}

		DLL::DLL(DLL&& aOther) :
			mLibrary(aOther.mLibrary),
			mFunctions(aOther.mFunctions)
		{
			aOther.mLibrary = nullptr;
			aOther.mFunctions.clear();
		}

		DLL::~DLL(){
			if (mLibrary) Unload();
		}

		DLL& DLL::operator=(DLL&& aOther){
			std::swap(mLibrary, aOther.mLibrary);
			std::swap(mFunctions, aOther.mFunctions);
			return *this;
		}

		void DLL::DefineFunction(const char* const aName, FunctionPointer& aLocation){
		    //! \todo Fix gcc bug?
			//mFunctions.emplace(std::string(aName), &aLocation);
			if(mLibrary){
				FunctionPointer const tmp = reinterpret_cast<FunctionPointer>(GetProcAddress(mLibrary, aName));
				if(! tmp) throw std::runtime_error("Solaire::Dll::Library : Failed to load function");
				aLocation = tmp;
			}
		}

		void DLL::Load(const char* const aFile){
			if(mLibrary) FreeLibrary(mLibrary);
			mLibrary = LoadLibraryA(aFile);
			if(! mLibrary) throw std::runtime_error("DLL : Failed to load .dll library");

			for(std::pair<const std::string, FunctionPointer*>& function : mFunctions){
				FunctionPointer const tmp = reinterpret_cast<FunctionPointer>(GetProcAddress(mLibrary, function.first.c_str()));
				if(! tmp) throw std::runtime_error("Solaire::Dll::Library : Failed to load function");
				*function.second = tmp;
			}
		}

		void DLL::Unload(){
			FreeLibrary(mLibrary);
			for(std::pair<const std::string, FunctionPointer*>& function : mFunctions){
				*function.second = nullptr;
			}
		}

		bool DLL::IsLoaded() const{
			return mLibrary ? true : false;
		}
	}
#endif
