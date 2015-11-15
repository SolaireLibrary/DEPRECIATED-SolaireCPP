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

#include "Solaire\Memory\DefaultAllocator.hpp"
#include "Solaire\Listener\ListenerBase.hpp"
#include "Solaire\Listener\SourceBase.hpp"

namespace Solaire{

    // SourceBase

	SourceBase::ListenerIterator SourceBase::ListenerBegin(){
        return mListeners.begin();
    }

	SourceBase::ConstListenerIterator SourceBase::ListenerBegin() const{
        return mListeners.begin();
    }

	SourceBase::ListenerIterator SourceBase::ListenerEnd(){
        return mListeners.end();
    }

	SourceBase::ConstListenerIterator SourceBase::ListenerEnd() const{
        return mListeners.end();
    }

	bool SourceBase::AddListener(ListenerBase& aListener) {
		if (static_cast<void*>(&aListener) == static_cast<void*>(this)) return false;
		if (mListeners.FindFirst(&aListener) != mListeners.end()) return false;

		mListeners.PushBack(&aListener);
		aListener.mSources.PushBack(this);

		OnListenerAdded(aListener);
		aListener.OnSourceAdded(*this);

		return true;
	}

	bool SourceBase::RemoveListener(ListenerBase& aListener) {
		const auto it = mListeners.FindFirst(&aListener);
		if (it == mListeners.end()) return false;

		mListeners.Erase(it);
		aListener.mSources.Erase(aListener.mSources.FindFirst(this));

		OnListenerRemoved(aListener);
		aListener.OnSourceRemoved(*this);

		return true;
	}

	SourceBase::~SourceBase() {
		for(ListenerBase* const i : mListeners) {
			OnListenerRemoved(*i);
			i->OnSourceRemoved(*this);
		}
	}

	SourceBase::SourceBase() :
		mListeners(DEFAULT_ALLOCATOR, 32)
	{}

	SourceBase::SourceBase(Allocator& aAllocator) :
        mListeners(aAllocator, 32)
    {}
}