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

#include "Solaire\Core\Dll\Library.hpp"

namespace Solaire{namespace Dll{
	
	// DLL

	Library::Library() :
		mLibrary(nullptr)
	{}

	Library::Library(Library&& aOther) :
		mLibrary(aOther.mLibrary),
		mFunctions(aOther.mFunctions)
	{
		aOther.mLibrary = nullptr;
		aOther.mFunctions.clear();
	}

	Library::~Library(){
		if (mLibrary) Unload();
	}

	Library& Library::operator=(Library&& aOther){
		std::swap(mLibrary, aOther.mLibrary);
		std::swap(mFunctions, aOther.mFunctions);
		return *this;
	}

	void Library::DefineFunction(const char* const aName, FunctionPointer& aLocation){
		mFunctions.emplace(std::string(aName), &aLocation);
		if(mLibrary){
			#if SOLAIRE_OS == SOLAIRE_WINDOWS
				FunctionPointer const tmp = reinterpret_cast<FunctionPointer>(GetProcAddress(mLibrary, aName));
			#else 
				FunctionPointer const tmp = nullptr;
			#endif
			if(! tmp) throw std::runtime_error("Solaire::Dll::Library : Failed to load function");
			aLocation = tmp;
		}
	}

	void Library::Load(const char* const aFile){
		#if SOLAIRE_OS == SOLAIRE_WINDOWS
			if(mLibrary) FreeLibrary(mLibrary);
			mLibrary = LoadLibraryA(aFile);
		#endif
		if(! mLibrary) throw std::runtime_error("DLL : Failed to load .dll library");

		for(std::pair<const std::string, FunctionPointer*>& function : mFunctions){
			#if SOLAIRE_OS == SOLAIRE_WINDOWS
				FunctionPointer const tmp = reinterpret_cast<FunctionPointer>(GetProcAddress(mLibrary, function.first.c_str()));
			#else 
				FunctionPointer const tmp = nullptr;
			#endif
			if(! tmp) throw std::runtime_error("Solaire::Dll::Library : Failed to load function");
			*function.second = tmp;
		}
	}

	void Library::Unload(){
		#if SOLAIRE_OS == SOLAIRE_WINDOWS
			FreeLibrary(mLibrary);
		#endif
		for(std::pair<const std::string, FunctionPointer*>& function : mFunctions){
			*function.second = nullptr;
		}
	}

	bool Library::IsLoaded() const{
		return mLibrary ? true : false;
	}
}}