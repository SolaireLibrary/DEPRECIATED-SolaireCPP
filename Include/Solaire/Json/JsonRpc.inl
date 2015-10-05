#ifndef SOLAIRE_JSON_RPC_INL
#define SOLAIRE_JSON_RPC_INL

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
\file Value.inl
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

    // RpcRequest

    std::shared_ptr<Value> RpcRequest::Serialise(const RpcRequest& aRequest){
        if(aRequest.mMethodName.Size() == 0) return std::shared_ptr<Value>();

        Allocator& allocator = aRequest.GetAllocator();

        std::shared_ptr<Value> ptr = allocator.SharedAllocate<Value>(allocator, TYPE_OBJECT);
        ObjectValue& request = ptr->AsObject();

        request.Add("jsonrpc", allocator.SharedAllocate<Value>(allocator, "2.0"));
        request.Add("method", allocator.SharedAllocate<Value>(allocator, aRequest.mMethodName));
        if(aRequest.mParams){
            request.Add("params", aRequest.mParams);
        }else{
            request.Add("params", allocator.SharedAllocate<Value>(allocator, TYPE_NULL));
        }

        if(aRequest.mID == RPC_NOTIFICATION_ID){
            request.Add("id", allocator.SharedAllocate<Value>(allocator, TYPE_NULL));
        }else{
            request.Add("id", allocator.SharedAllocate<Value>(allocator, aRequest.mID));
        }

        return ptr;
    }

    RpcRequest::RpcRequest(Allocator& aAllocator):
        mMethodName(aAllocator),
        mID(RPC_NOTIFICATION_ID)
    {}

    RpcRequest::~RpcRequest(){

    }

    void RpcRequest::SetMethodName(const ConstStringFragment aName){
        mMethodName = aName;
    }

    ConstStringFragment RpcRequest::GetMethodName() const{
        return mMethodName;
    }

    bool RpcRequest::IsRequest() const{
        return mID != RPC_NOTIFICATION_ID;
    }

    bool RpcRequest::IsNotification() const{
        return mID != RPC_NOTIFICATION_ID;
    }

    void RpcRequest::SetRequestID(const RpcRequestID aID){
        mID = aID;
    }

    void RpcRequest::SetNotification(){
        mID = RPC_NOTIFICATION_ID;
    }

    RpcRequestID RpcRequest::GetRequestID() const{
        if(mID == RPC_NOTIFICATION_ID) throw std::runtime_error("Json::RpcRequest : Request is a notification with now RequestID");
        return mID;
    }

    void RpcRequest::SetParams(std::shared_ptr<Value> aParams){
        mParams.swap(aParams);
    }

    std::shared_ptr<const Value> RpcRequest::GetParams() const{
        return mParams;
    }

    Allocator& RpcRequest::GetAllocator() const{
        return mMethodName.GetAllocator();
    }
}}

#endif
