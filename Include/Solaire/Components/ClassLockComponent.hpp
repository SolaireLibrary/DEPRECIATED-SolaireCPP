#ifndef SOLAIRE_COMPONENTS_CLASS_LOCK_COMPONENT_HPP
#define SOLAIRE_COMPONENTS_CLASS_LOCK_COMPONENT_HPP

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
	\file ClassLockComponent.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 21st October 2015
	Last Modified	: 21st October 2015
*/

#include "..\Core\Threading\ThreadTypes.hpp"
#include "LockComponent.hpp"

namespace Solaire{ namespace Components{

    template<class T>
	class ClassLockComponent : public LockComponent{
    private:
        //! \todo Change to read / write lock
        static RecursiveMutex LOCK;
    protected:
        // Inherited from Component
		void AttachCopy(Composite& aComposite) const override{
		   aComposite.Attach<ClassLockComponent<T>>();
		}
	public:
		ClassLockComponent(Composite& aParent):
		    LockComponent(aParent)
        {}

		~ClassLockComponent(){

		}

		// Inherited from LockComponent

		void ReadLock() const override{
            LOCK.lock();
		}

		void ReadUnlock() const override{
            LOCK.unlock();
		}

		bool TryReadLock() const override{
            return LOCK.try_lock();
		}

		void WriteLock() const override{
            LOCK.lock();
		}

		void WriteUnlock() const override{
            LOCK.unlock();
		}

		bool TryWriteLock() const override{
            return LOCK.try_lock();
		}

	};
}}


#endif
