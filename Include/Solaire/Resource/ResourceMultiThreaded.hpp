#ifndef SOLAIRE_RESOURCE_THREADED_HPP
#define SOLAIRE_RESOURCE_THREADED_HPP

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
	\file ResourceMultiThreaded.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 11th December 2015
	Last Modified	: 11th December 2015
*/

#include "Resource.hpp"

namespace Solaire {

	template<typename ID_TYPE, class TYPE>
	class MultiThreadedResource : public Resource<ID_TYPE, TYPE> {
	private:
		std::mutex mLock;
		std::wait_condition mLoadCondition;
		uint16_t mUsers;
		bool mLoaded;
	protected:
		virtual bool SOLAIRE_EXPORT_CALL OnLoad() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnUnload() throw() = 0;
		virtual SharedAllocation<TYPE> SOLAIRE_EXPORT_CALL CreateResourceFromLoad() const throw() = 0;
	
		// Inherited from Resource
	
		SharedAllocation<TYPE> SOLAIRE_EXPORT_CALL CreateResource() throw() override {
			if(mUsers > 0){
				++mUsers;
				if(! mLoaded){
					std::unique_lock<std::mutex> lock(mLock);
					mLoadCondition.wait(lock);
				}
			}else{
				mUsers = 1;
				if(! OnLoad()) return SharedAllocation<TYPE>();
				mLoaded = true;
				mLoadCondition.notify_all();
			}
		
			const SharedAllocation<TYPE> tmp = CreateResourceFromLoad();
		
			if(mUsers > 1) {
				--mUsers;
			}else if(mUsers == 1) {
				mUsers = 0;
				if(! OnUnload()) return SharedAllocation<TYPE>();
				mLoaded = false;
			}
		
			return tmp;
		}
	public:
		MultiThreadedResourceFactory(const ID aID) :
			Resource(aID),
			mUsers(0),
			mLoaded(false)
		{}
	
		virtual SOLAIRE_EXPORT_CALL ~MultiThreadedResourceFactory() throw() {
		
		}
	};
    
}


#endif
