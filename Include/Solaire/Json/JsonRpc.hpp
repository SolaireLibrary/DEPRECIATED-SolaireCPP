#ifndef SOLAIRE_JSON_RPC_HPP
#define SOLAIRE_JSON_RPC_HPP

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
\file JsonRpc.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 2.0
\date
Created			: 4th October 2015
Last Modified	: 5th October 2015
*/

#include "RpcRequest.hpp"
#include "RpcError.hpp"
#include "RpcResponse.hpp"

namespace Solaire{ namespace Json{

    /*typedef std::shared_ptr<Value> RpcRequest;
    typedef std::shared_ptr<Value> RpcNotification;
    typedef std::shared_ptr<Value> RpcResponse;
    typedef std::shared_ptr<Value> RpcResult;
    typedef std::shared_ptr<Value> RpcError;*/

    /*class RpcBuilder{
    private:
        enum : RpcRequestID{
            RPC_NOTIFICATION_ID = INT64_MAX
        };

        Allocator* mAllocator;

        RpcResponse BuildRpcResponse(const RpcRequestID aID, std::shared_ptr<Value> aPayload, bool aIsError){
            std::shared_ptr<Value> ptr = mAllocator->SharedAllocate<Value>(TYPE_OBJECT, *mAllocator);
            ObjectValue& request = ptr->AsObject();

            request.Add("jsonrpc", mAllocator->SharedAllocate<Value>(*mAllocator, "2.0"));
            if(aIsError){
                request.Add("error", aPayload);
            }else{
                request.Add("result", aPayload ? aPayload : mAllocator->SharedAllocate<Value>(*mAllocator, TYPE_NULL));
            }
            request.Add("id", mAllocator->SharedAllocate<Value>(*mAllocator, aID));
            return ptr;
        }
    public:
        RpcBuilder(Allocator& aAllocator):
            mAllocator(&aAllocator)
        {}

       RpcRequest BuildRpcRequest(RpcRequestID aID, const ConstStringFragment aMethodName, std::shared_ptr<Value> aParam = std::shared_ptr<Value>()){
            std::shared_ptr<Value> ptr = mAllocator->SharedAllocate<Value>(TYPE_OBJECT, *mAllocator);
            ObjectValue& request = ptr->AsObject();

            request.Add("jsonrpc", mAllocator->SharedAllocate<Value>(*mAllocator, "2.0"));
            request.Add("method", mAllocator->SharedAllocate<Value>(*mAllocator, aMethodName));
            if(aParam){
                if(aParam->GetType() == TYPE_ARRAY){
                    request.Add("params", aParam);
                }else{
                    std::shared_ptr<Value>paramPtr = mAllocator->SharedAllocate<Value>(*mAllocator, TYPE_ARRAY);
                    ArrayValue& params = paramPtr->AsArray();
                    params.PushBack(aParam);
                    request.Add("params", paramPtr);
                }
            }else{
                request.Add("params", mAllocator->SharedAllocate<Value>(*mAllocator, TYPE_NULL));
            }
            if(aID == RPC_NOTIFICATION_ID){
                request.Add("id", mAllocator->SharedAllocate<Value>(*mAllocator, TYPE_NULL));
            }else{
                request.Add("id", mAllocator->SharedAllocate<Value>(*mAllocator, aID));
            }

            return ptr;
        }

        RpcNotification BuildRpcNotification(const ConstStringFragment aMethodName, RpcResult aParam = RpcResult()){
            return BuildRpcRequest(RPC_NOTIFICATION_ID, aMethodName, aParam);
        }

        RpcNotification BuildRpcError(const RpcErrorID aCode, const ConstStringFragment aMessage, std::shared_ptr<Value> aData = std::shared_ptr<Value>()){
            std::shared_ptr<Value> ptr = mAllocator->SharedAllocate<Value>(TYPE_OBJECT, *mAllocator);
            Object& request = ptr->GetObject();

            request.emplace("code", mAllocator->SharedAllocate<Value>(static_cast<Number>(aCode), *mAllocator));
            request.emplace("message", mAllocator->SharedAllocate<Value>(aMessage, *mAllocator));

            if(aData){
                request.emplace("data", aData);
            }else{
                request.emplace("data", mAllocator->SharedAllocate<Value>(TYPE_NULL, *mAllocator));
            }

            return ptr;
        }

        RpcResponse BuildRpcResponseSuccess(const RpcRequestID aID, RpcResult aResult = RpcResult()){
            return BuildRpcResponse(aID, aResult, false);
        }

        RpcResponse BuildRpcResponseFail(const RpcRequestID aID, RpcError aError){
            return BuildRpcResponse(aID, aError, true);
        }
    };*/

    /*class RpcServer{

    };

    class RpcClient{
    private:
        template<class ...Params>
        std::shared_ptr<Value> SerialiseParams(Params&& ...aParams){
            std::shared_ptr<Value> value = mAllocator->SharedAllocate<Value>(TYPE_ARRAY, *mAllocator);
            Array& array_ = value->GetArray();

            array_.PushBack(JsonSerialise(aParams...));

            return value;
        }

        void ThrowIfError(const RpcResponse aResponse) const{
            Object& response = aResponse->GetObject();
            auto it = response.find("error");
            if(it != response.end()){
                ConstStringFragment message = it->second->GetObject()["message"]->GetString();
                throw std::runtime_error(std::string(message.begin(), message.end()));
            }
        }

        Allocator* mAllocator;
        RpcBuilder mBuilder;
    protected:
        virtual void SendNotification(RpcNotification aNotification) = 0;
        virtual RpcResponse SendRequest(RpcRequest aRequest) = 0;
        virtual RpcRequestID GenerateRequestID() const = 0;
    public:
        template<class R, class ...Params>
        typename Serialisable<R>::DeserialiseType SendRequest(const ConstStringFragment aMethodName, Params... aParams){
            RpcRequest requst = mBuilder.BuildRpcRequest(GenerateRequestID(), aMethodName, SerialiseParams(aParams...));
            RpcResponse response = SendRequest(requst);
            ThrowIfError(response);
            return JsonDeserialise<R>(response->GetObject()["data"]);
        }

        template<class ...Params>
        void SendNotification(const ConstStringFragment aMethodName, Params... aParams){
            RpcNotification notification = mBuilder.BuildRpcNotification(aMethodName, SerialiseParams(aParams...));
            SendNotification(notification);
        }
    };*/
}}

#include "JsonRpc.inl"

#endif
