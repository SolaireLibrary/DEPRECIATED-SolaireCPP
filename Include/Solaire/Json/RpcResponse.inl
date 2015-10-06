#ifndef SOLAIRE_JSON_RPC_RESPOSE_INL
#define SOLAIRE_JSON_RPC_RESPOSE_INL

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
\file RpcResponse.inl
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

    // RpcResponse

    std::shared_ptr<Value> RpcResponse::Serialise(const RpcResponse& aResponse){
        Allocator& allocator = aResponse.GetAllocator();

        std::shared_ptr<Value> response = allocator.SharedAllocate<Value>(allocator, TYPE_OBJECT);

        response->pObject.Add("jsonrpc", allocator, "2.0");
        if(aResponse.mResult){
            response->pObject.Add("result", aResponse.mResult);
        }else if(aResponse.mError){
            response->pObject.Add("error", RpcError::Serialise(*aResponse.mError));
        }else{
            response->pObject.Add("result", allocator, TYPE_NULL);
        }
        response->pObject.Add("id", allocator, aResponse.mID);

        return response;
    }

    RpcResponse::RpcResponse(Allocator& aAllocator):
        mAllocator(&aAllocator)
    {}

    RpcResponse::~RpcResponse(){

    }

    void RpcResponse::SetRequestID(const RpcRequestID aID){
        mID = aID;
    }

    RpcRequestID RpcResponse::GetRequestID() const{
        return mID;
    }

    bool RpcResponse::HasResult() const{
        return mResult ? true : false;
    }

    void RpcResponse::SetResult(std::shared_ptr<Value> aResult){
        if(HasError()) throw std::runtime_error("Json::RpcResponse : RpcResponse already has an error set, cannot also set result");
        mResult.swap(aResult);
    }

    std::shared_ptr<const Value> RpcResponse::GetResult() const{
        return mResult;
    }

    bool RpcResponse::HasError() const{
        return mError ? true : false;
    }

    void RpcResponse::SetError(std::shared_ptr<RpcError> aError){
        if(HasResult()) throw std::runtime_error("Json::RpcResponse : RpcResponse already has a result set, cannot also set error");
        mError.swap(aError);
    }

    std::shared_ptr<const RpcError> RpcResponse::GetError() const{
        return mError;
    }

    Allocator& RpcResponse::GetAllocator() const{
        return *mAllocator;
    }

}}

#endif
