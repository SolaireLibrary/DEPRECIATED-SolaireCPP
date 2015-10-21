#ifndef SOLAIRE_COMPONENTS_LOCK_COMPONENT_HPP
#define SOLAIRE_COMPONENTS_LOCK_COMPONENT_HPP

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
	\file LockComponent.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 21st October 2015
	Last Modified	: 21st October 2015
*/

#include "Composite.hpp"

namespace Solaire{ namespace Components{

	class LockComponent : public Component{
	public:
		LockComponent(Composite& aParent, CompositeTracker& aTracker):
		    Component(aParent)
        {}

		virtual ~LockComponent(){

		}

		virtual void ReadLock() const = 0;
		virtual void ReadUnlock() const = 0;
		virtual bool TryReadLock() const = 0;

		virtual void WriteLock() const = 0;
		virtual void WriteUnlock() const = 0;
		virtual bool TryWriteLock() const = 0;
	};

	class ReadComponentLockGuard{
    private:
        LockComponent* mLock;
    public:
        ReadComponentLockGuard(Composite& aComposite):
            mLock(nullptr)
        {
            //! \todo Get lock component
            if(mLock) mLock->ReadLock();
        }

        ~ReadComponentLockGuard(){
            if(mLock) mLock->ReadUnlock();
        }
	};

	class WriteComponentLockGuard{
    private:
        LockComponent* mLock;
    public:
        WriteComponentLockGuard(Composite& aComposite):
            mLock(nullptr)
        {
            //! \todo Get lock component
            if(mLock) mLock->WriteLock();
        }

        ~WriteComponentLockGuard(){
            if(mLock) mLock->WriteUnlock();
        }
	};
}}


#endif
