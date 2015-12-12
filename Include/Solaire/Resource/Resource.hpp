#ifndef SOLAIRE_RESOURCE_HPP
#define SOLAIRE_RESOURCE_HPP

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
	\file Resource.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 11th December 2015
	Last Modified	: 11th December 2015
*/

#include "ResourceI.hpp"

namespace Solaire {

	template<typename ID_TYPE, class TYPE>
	class Resource<TYPE> : public ResourceI<ID_TYPE> {
	public:
		typedef TYPE Type;
		typedef SharedAllocation<Type> Ptr;
		typedef SharedAllocation<const Type> ConstPtr;
	private:
		Ptr mResource;
		ID mID;
	protected:
		virtual SOLAIRE_EXPORT_CALL SharedAllocation<Type> CreateResource() const throw() = 0;
	public:
		Resource(const ID aID) :
			mResource(),
			mID(aID)
		{}

		virtual SOLAIRE_EXPORT_CALL ~Resource() throw() {

		}

		Ptr Get() throw() {
			return mResource;
		}

		ConstPtr Get() throw() const {
			return mResource;
		}

		// Inherited from ResourceI

		bool SOLAIRE_EXPORT_CALL Create() throw() override {
			if (mResource) return false;
			Ptr tmp = CreateResource();
			if (!tmp) return false;
			mResource.Swap(tmp);
			return true;
		}

		bool SOLAIRE_EXPORT_CALL Recreate() throw() override {
			Ptr tmp = CreateResource();
			if (!tmp) return false;
			mResource.Swap(tmp);
			return true;
		}

		bool SOLAIRE_EXPORT_CALL Destroy() throw() {
			mResource.Swap(Ptr());
		}

		bool SOLAIRE_EXPORT_CALL IsCreated() throw() {
			return mResource ? true : false;
		}

		bool SOLAIRE_EXPORT_CALL CompareID(const ID aID) const throw() override {
			return mID == aID;
		}
	};

	template<typename ID_TYPE, class TYPE>
	using ResourcePtr = SharedAllocation<Resource<ID_TYPE, TYPE>>;

	template<typename ID_TYPE, class TYPE>
	using ConstResourcePtr = SharedAllocation<const Resource<ID_TYPE, TYPE>>;

}


#endif
