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

#include "Solaire\Core\Init.hpp"

#if SOLAIRE_OS == SOLAIRE_WINDOWS

#include "Solaire\Link\SharedLibrary.hpp"
#include "Solaire\Memory\Allocator.hpp"
#include <new>

namespace Solaire {

	class SharedLibraryDll : public SharedLibrary {
	private:
		Allocator& mAllocator;
		HMODULE mModule;
	public:
		SharedLibraryDll(Allocator& aAllocator) :
			mAllocator(aAllocator),
			mModule(nullptr)
		{}

		~SharedLibraryDll() {
			if(IsOpen()) {
				Close();
			}
		}

		// Inherited from SharedLibrary

		bool SOLAIRE_EXPORT_CALL Open(const char* const aPath) throw() override {
			if(IsOpen()) return false;
			mModule = LoadLibraryA(aPath);
			return IsOpen();
		}

		bool SOLAIRE_EXPORT_CALL Close() throw() override {
			if(! IsOpen()) return false;
			return FreeLibrary(mModule) > 0;
		}

		bool SOLAIRE_EXPORT_CALL IsOpen() const throw() override {
			return mModule != nullptr;
		}

		FunctionPtr SOLAIRE_EXPORT_CALL LoadFunction(const char* const aName) const throw() override {
			return reinterpret_cast<FunctionPtr>(GetProcAddress(mModule, aName));
		}

		Allocator& GetAllocator() const throw() override {
			return mAllocator;
		}

	};

	extern "C" {
		SOLAIRE_EXPORT_API SharedLibrary* SOLAIRE_EXPORT_CALL CreateSharedLibrary(Allocator& aAllocator) {
			return new(aAllocator.Allocate(sizeof(SharedLibraryDll))) SharedLibraryDll(aAllocator);
		}
	}
}

#endif