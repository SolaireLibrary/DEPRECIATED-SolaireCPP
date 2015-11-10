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

#include "Solaire\Core\Memory\DefaultAllocator.hpp"
#include "Solaire\Core\Listener\ListenerBase.hpp"
#include "Solaire\Core\Listener\SourceBase.hpp"

namespace Solaire{

    // ListenerBase

	ListenerBase::SourceIterator ListenerBase::SourceBegin(){
        return mSources.begin();
    }

	ListenerBase::ConstSourceIterator ListenerBase::SourceBegin() const{
        return mSources.begin();
    }

	ListenerBase::SourceIterator ListenerBase::SourceEnd(){
        return mSources.end();
    }

	ListenerBase::ConstSourceIterator ListenerBase::SourceEnd() const{
        return mSources.end();
    }

    bool ListenerBase::AddSource(SourceBase& aSource){
        return aSource.AddListener(*this);
    }

    bool ListenerBase::RemoveSource(SourceBase& aSource){
        return aSource.RemoveListener(*this);
    }

	ListenerBase::ListenerBase() :
		mSources(DEFAULT_ALLOCATOR, 32)
	{}

	ListenerBase::ListenerBase(Allocator& aAllocator) :
        mSources(aAllocator, 32)
    {}

	ListenerBase::~ListenerBase(){
        while(! mSources.IsEmpty()){
            mSources.Back()->RemoveListener(*this);
        }
    }

}
