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

#include "Solaire\Memory\Allocator.hpp"
#include "Solaire\Link\ScriptI.hpp"
#include "Solaire\Link\CompilerMinGW.hpp"
#include <string>

namespace Solaire {

	class ScriptImplementation : public ScriptI{
	private:
		Allocator& mAllocator;
		SharedLibrary* mSharedLibrary;
		CppCompiler* mCompiler;
		int (SOLAIRE_EXPORT_CALL *mMainFunction)();
	private:
		bool CompileScript() {
			mCompiler->SetCompilerPath("MinGW\\bin\\mingw32-g++.exe");
			mCompiler->SetOutputPath("" /*! \todo tmp directory*/);
			mCompiler->SetOutputName("" /*! \todo random name*/);
			//! \todo Set flags
			return mCompiler->CompileSharedLib();
		}

		bool LoadScript() {
			mSharedLibrary = CreateSharedLibrary(mAllocator);
			if(mSharedLibrary == nullptr) return false;
			//if (!mSharedLibrary->Open(/*dll*/)) return false;
			//mMainFunction = mSharedLibrary->LoadFunction<int>("SolaireMain");
			return mSharedLibrary != nullptr;
		}
	protected:

		// Inherited from SharedLibrary
		FunctionPtr SOLAIRE_EXPORT_CALL _LoadFunction(const char* const aName) const throw() override {
			//return mSharedLibrary ? reinterpret_cast<FunctionPtr>(mSharedLibrary->LoadFunction<void>(aName)) : nullptr;
			return nullptr;
		}

	public:
		ScriptImplementation(Allocator& aAllocator) :
			mAllocator(aAllocator)
		{}

		SOLAIRE_EXPORT_CALL ~ScriptImplementation() {
			if(mSharedLibrary) {
				mSharedLibrary->~SharedLibrary();
				mAllocator.Deallocate(mSharedLibrary);
			}
			if(mCompiler) {
				mCompiler->~CppCompiler();
				mAllocator.Deallocate(mCompiler);
			}
		}

		// Inherited from ScriptI
		int SOLAIRE_EXPORT_CALL operator()() override {
			return mMainFunction ? mMainFunction() : SCRIPT_NOT_COMPILED;
		}


		// Inherited from SharedLibrary
		bool SOLAIRE_EXPORT_CALL Open(const char* const aFilename) throw() override {
			if(mSharedLibrary) {
				return false;
			}else {
				if(! CompileScript()) return false;
				return LoadScript();
			}
		}

		bool SOLAIRE_EXPORT_CALL Close() throw() override {
			return mSharedLibrary ? mSharedLibrary->Close() : false;
		}

		bool SOLAIRE_EXPORT_CALL IsOpen() const throw() override {
			return mSharedLibrary ? mSharedLibrary->IsOpen() : false;
		}

		Allocator& SOLAIRE_EXPORT_CALL GetAllocator() const throw() override {
			return mAllocator;
		}

	};

	extern "C" {
		SOLAIRE_EXPORT_API ScriptI* SOLAIRE_EXPORT_CALL CompileScript(Allocator& aAllocator) {
			return new(aAllocator.Allocate(sizeof(ScriptImplementation))) ScriptImplementation(aAllocator);
		}
	}
}