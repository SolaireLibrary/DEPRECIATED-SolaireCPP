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

#include "Solaire\Link\ImplementedLinkObject.hpp"

namespace Solaire{namespace Dll{
	
	// ImplementedLinkObject
	
	ImplementedLinkObject::ImplementedLinkObject() :
		mReferenceCount(0),
		mErrorHead(0)
	{}
	
	ImplementedLinkObject::~ImplementedLinkObject(){
		
	}
	
	void ImplementedLinkObject::SetError(const ErrorCode aCode){
		if(mErrorHead == MAX_ERRORS){
			for(int i = 0; i < MAX_ERRORS - 2; ++i){
				mErrorCodes[i] = mErrorCodes[i + 2];
			}
			mErrorCodes[MAX_ERRORS - 2] = aCode;
			mErrorCodes[MAX_ERRORS - 1] = DLL_ERROR_OVERFLOW;
		}else{
			mErrorCodes[mErrorHead++] = aCode;
		}
	}
	
	void ImplementedLinkObject::CreateReference(){
		++mReferenceCount;
	}
	
	void ImplementedLinkObject::ReleaseReference(){
		if(mReferenceCount == 0){
			SetError(DLL_RELEASED_UNREFERENCED_OBJECT);
		}else{
			--mReferenceCount;
			if(mReferenceCount == 0){
				this->~ImplementedLinkObject();
			}
		}
	}
	
	uint32_t ImplementedLinkObject::GetReferenceCount() const{
		return mReferenceCount;
	}
	
	LinkObject::ErrorCode ImplementedLinkObject::GetError(){
		if(mErrorHead == 0) return DLL_NO_ERROR;
		return mErrorCodes[mErrorHead--];
	}
	
}}