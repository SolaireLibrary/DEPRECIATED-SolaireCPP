#ifndef SOLAIRE_JSON_RPC_ERROR_INL
#define SOLAIRE_JSON_RPC_ERROR_INL

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
\file RpcError.inl
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 5th October 2015
Last Modified	: 5th October 2015
*/

namespace Solaire{ namespace Json{

    // RpcError

    std::shared_ptr<Value> RpcError::Serialise(const RpcError& aError){
        Allocator& allocator = aError.GetAllocator();

        std::shared_ptr<Value> value = allocator.SharedAllocate<Value>(allocator, TYPE_OBJECT);
        ObjectValue& object = value->AsObject();

        object.Add(allocator, "code", aError.mCode);
        if(mMessage.Size() > 0){
            object.Add(allocator, "message", aError.mMessage);
        }else{
            object.Add(allocator, "message", aError.TYPE_NULL);
        }

        if(aError.mData){
            object.Add("data", aError.mData);
        }else{
            object.Add(allocator, "message", aError.TYPE_NULL);
        }

        return value;
    }


    RpcError::RpcError(Allocator& aAllocator):
        mMessage(aAllocator)
    {}

    RpcError::~RpcError(){

    }

    void RpcError::SetMessage(const ConstStringFragment aMessage){
        mMessage = aMessage;
    }

    ConstStringFragment RpcError::GetMessage() const{
        return mMessage;
    }

    void RpcError::SetCode(const RpcErrorCode aCode){
        mCode = aCode;
    }

    RpcErrorCode RpcError::GetCode() const{
        return mCode;
    }

    void RpcError::SetData(std::shared_ptr<Value> aParams){
        mData.swap(aParams);
    }

    std::shared_ptr<const Value> RpcError::GetData() const{
        return mData;
    }

    Allocator& RpcError::GetAllocator() const{
        mMessage.GetAllocator();
    }


    // RpcErrorException

    RpcErrorException::RpcErrorException(std::shared_ptr<RpcError> aError):
        mError(aError)
    {}

    RpcErrorCode RpcErrorException::GetErrorCode() const{
        return mError->GetCode();
    }

    std::shared_ptr<const Value> RpcErrorException::GetData() const{
        return mError->GetData();
    }

    const char* RpcErrorException::what() throw(){
        return mError->GetMessage().begin();
    }

}}

#endif
