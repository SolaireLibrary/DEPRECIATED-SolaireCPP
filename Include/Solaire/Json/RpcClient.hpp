#ifndef SOLAIRE_JSON_RPC_CLIENT_HPP
#define SOLAIRE_JSON_RPC_CLIENT_HPP

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
\file RpcClient.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 2.0
\date
Created			: 5th October 2015
Last Modified	: 5th October 2015
*/

#include "RpcRequest.hpp"
#include "RpcError.hpp"
#include "RpcResponse.hpp"

namespace Solaire{ namespace Json{

    class RpcClient{
    private:
    protected:
        virtual RpcRequestID GenerateRequestID() const = 0;
    public:
        virtual void SendSerialisedNotification(const RpcRequest& aNotification) = 0;
        virtual RpcResponse SendSerialisedRequest(const RpcRequest& aRequest) = 0;
        virtual Allocator& GetAllocator() const = 0;

        template<class ...Params>
        void SendNotification(ConstStringFragment aMethodName, Params&& ...aParams){
            Allocator& allocator = GetAllocator();

            std::shared_ptr<Value> params = allocator.SharedAllocate<Value>(allocator, TYPE_ARRAY);
            params->pArray.PushBack(JsonSerialise(allocator, allocator, aParams...));

            RpcRequest request(allocator);
            request.SetNotification();
            request.SetMethodName(aMethodName);
            request.SetParams(params);

            SendSerialisedNotification(request);
        }

        template<class R, class ...Params>
        typename Serialisable<R>::DeserialiseType SendRequest(ConstStringFragment aMethodName, Params&& ...aParams){
            Allocator& allocator = GetAllocator();

            std::shared_ptr<Value> params = allocator.SharedAllocate<Value>(allocator, TYPE_ARRAY);
            params->pArray.PushBack(JsonSerialise(allocator, allocator, aParams...));

            RpcRequest request(allocator);
            request.SetMethodName(aMethodName);
            request.SetRequestID(GenerateRequestID());
            request.SetParams(params);

            RpcResponse response = SendSerialisedRequest(request);
            if(response.HasError()) throw RpcErrorException(response.GetError());
            return JsonDeserialise<R>(allocator, allocator, response.GetResult());
        }
    };
}}

#include "JsonRpc.inl"

#endif
