#ifndef SOLAIRE_UNIQUE_ALLOCATION_HPP
#define SOLAIRE_UNIQUE_ALLOCATION_HPP

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
	\file UniqueAllocation.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 8th December 2015
	Last Modified	: 4th January 2015
*/

#include "AllocatorI.hpp"

namespace Solaire {

	template<class T>
	class UniqueAllocation {
	private:
		AllocatorI* mAllocator;
		T* mObject;
	private:
		bool DeleteObject() throw() {
			if(mObject == nullptr) return false;
			mObject->~T();
			if(! mAllocator->Deallocate(mObject)) return false;
			mObject = nullptr;
			return true;
		}

		UniqueAllocation(const UniqueAllocation<T>&) = delete;
		UniqueAllocation& operator=(const UniqueAllocation<T>&) = delete;
	public:
		UniqueAllocation() throw() :
			mAllocator(nullptr),
			mObject(nullptr)
		{}

		UniqueAllocation(AllocatorI& aAllocator, T* const aObject) throw() :
			mAllocator(&aAllocator),
			mObject(aObject)
		{}

		template<class T2>
		UniqueAllocation(UniqueAllocation<T2>&& aOther) throw() :
			mAllocator(aOther.mAllocator),
			mObject(aOther.ReleaseOwnership())
		{}

		~UniqueAllocation() throw() {
			DeleteObject();
		}

		template<class T2>
		UniqueAllocation& operator=(UniqueAllocation<T2>&& aOther) throw() {
			Swap(aOther);
			return *this;
		}

		void Swap(UniqueAllocation<T>& aOther) throw() {
			std::swap(mAllocator, aOther.mAllocator);
			std::swap(mObject, aOther.mObject);
		}

		T* ReleaseOwnership() throw() {
			T* const tmp = mObject;
			mObject = nullptr;
			return tmp;
		}

		AllocatorI& GetAllocator() const throw(){
			return *mAllocator;
		}

		operator bool() const throw() {
			return mObject != nullptr;
		}

		T& operator*() const throw() {
			return *mObject;
		}

		T* operator->() const throw() {
			return mObject;
		}
	};
}

#endif
