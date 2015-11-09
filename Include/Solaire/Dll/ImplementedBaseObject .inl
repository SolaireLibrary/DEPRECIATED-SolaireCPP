#ifndef SOLAIRE_DLL_IMPLEMENTED_BASE_OBJECT_INL
#define SOLAIRE_DLL_IMPLEMENTED_BASE_OBJECT_INL

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
\file ImplementedBaseObject.inl
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 9th November 2015
Last Modified	: 9th November 2015
*/

namespace Solaire{namespace Dll{
	
	// ImplementedBaseObject
	
	ImplementedBaseObject::ImplementedBaseObject() :
		mReferenceCount(0),
		mErrorHead(0)
	{}
	
	ImplementedBaseObject::~ImplementedBaseObject(){
		
	}
	
	void ImplementedBaseObject::SetError(const ErrorCode aCode){
		if(mErrorHead == MAX_ERRORS){
			for(int i = 0; i < MAX_ERRORS - 2; ++i){
				mErrorCodes[i] = mErrorCodes[i + 2];
			}
			mErrorCodes[MAX_ERRORS - 2] = aCode;
			mErrorCodes[MAX_ERRORS - 1] = ERROR_OVERFLOW;
		}else{
			mErrorCodes[mErrorHead++] = aCode;
		}
	}
	
	void ImplementedBaseObject::CreateReference(){
		++mReferenceCount;
	}
	
	void ImplementedBaseObject::ReleaseReference(){
		if(mReferenceCount == 0){
			SetError(RELEASED_UNREFERENCED_OBJECT);
		}else{
			--mReferenceCount;
			if(mReferenceCount == 0){
				this->~ImplementedBaseObject();
			}
		}
	}
	
	uint32_t ImplementedBaseObject::GetReferenceCount() const{
		return mReferenceCount;
	}
	
	BaseObject::ErrorCode ImplementedBaseObject::GetError(){
		if(mErrorHead == 0) return NO_ERROR;
		return mErrorCodes[mErrorHead--];
	}
	
}}

#endif
