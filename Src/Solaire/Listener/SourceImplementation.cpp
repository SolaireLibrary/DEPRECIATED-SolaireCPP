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

#include "Solaire\Listener\SourceImplementation.hpp"

namespace Solaire{

    // SourceImplementation

	SourceImplementation::SourceImplementation() :
		mListeners(GetDefaultAllocator())
	{}

	SourceImplementation::SourceImplementation(Allocator& aAllocator) :
		mListeners(aAllocator)
	{}

	SourceImplementation::~SourceImplementation() {

	}

	bool SOLAIRE_EXPORT_CALL SourceImplementation::AddListener(Listener& aListener) throw() {
		auto it = mListeners.FindFirst(&aListener);
		if(it != mListeners.end()) return false;
		if(! CanAcceptListener(aListener)) return false;
		if(!aListener.CanAcceptSource(*this)) return false;

		mListeners.PushBack(&aListener);
		OnListen(aListener);
		aListener.OnListen(*this);

		return true;
	}

	bool SOLAIRE_EXPORT_CALL SourceImplementation::RemoveListener(Listener& aListener) throw() {
		auto it = mListeners.FindFirst(&aListener);

		if(it == mListeners.end()) return false;
		OnUnlisten(aListener);
		aListener.OnUnlisten(*this);
		return true;
	}

	bool SOLAIRE_EXPORT_CALL SourceImplementation::HasListener(const Listener& aListener) const throw() {
		return mListeners.FindFirst(const_cast<Listener*>(&aListener)) != mListeners.end();
	}

	uint32_t SOLAIRE_EXPORT_CALL  SourceImplementation::GetListenerCount() const throw() {
		return mListeners.Size();
	}

	Listener& SOLAIRE_EXPORT_CALL  SourceImplementation::GetListener(const uint32_t aIndex) const throw() {
		return *mListeners[aIndex];
	}


}
