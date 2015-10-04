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
\file Value.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 4th October 2015
Last Modified	: 4th October 2015
*/

#include "Value.hpp"

namespace Solaire{ namespace Json{

    typedef int64_t RpcRequestID;
    typedef int64_t RpcErrorID;

    enum : RpcErrorID{
        RPC_PARSE_ERROR         = -32700,
        RPC_INVALID_REQUEST     = -32600,
        RPC_METHOD_NOT_FOUND    = -32601,
        RPC_INVALID_PARAMS      = -32602,
        RPC_INTERNAL_ERROR      = -32603
    };

    typedef std::shared_ptr<Value> RpcRequest;
    typedef std::shared_ptr<Value> RpcNotification;
    typedef std::shared_ptr<Value> RpcResponse;
    typedef std::shared_ptr<Value> RpcResult;
    typedef std::shared_ptr<Value> RpcError;

    class RpcBuilder{
    private:
        enum : RpcRequestID{
            RPC_NOTIFICATION_ID = INT64_MAX
        };

        Allocator* mAllocator;

        RpcResponse BuildRpcResponse(const RpcRequestID aID, std::shared_ptr<Value> aPayload, bool aIsError){
            std::shared_ptr<Value> ptr = mAllocator->SharedAllocate<Value>(TYPE_OBJECT, *mAllocator);
            Object& request = ptr->GetObject();

            request.emplace("jsonrpc", mAllocator->SharedAllocate<Value>("1.0", *mAllocator));
            if(aIsError){
                request.emplace("error", aPayload);
            }else{
                request.emplace("result", aPayload ? aPayload : mAllocator->SharedAllocate<Value>(TYPE_NULL, *mAllocator));
            }
            request.emplace("id", mAllocator->SharedAllocate<Value>(static_cast<Number>(aID), *mAllocator));
            return ptr;
        }
    public:
        RpcBuilder(Allocator& aAllocator):
            mAllocator(&aAllocator)
        {}

       RpcRequest BuildRpcRequest(RpcRequestID aID, const ConstStringFragment aMethodName, std::shared_ptr<Value> aParam = std::shared_ptr<Value>()){
            std::shared_ptr<Value> ptr = mAllocator->SharedAllocate<Value>(TYPE_OBJECT, *mAllocator);
            Object& request = ptr->GetObject();

            request.emplace("jsonrpc", mAllocator->SharedAllocate<Value>("1.0", *mAllocator));
            request.emplace("method", mAllocator->SharedAllocate<Value>(aMethodName, *mAllocator));
            if(aParam){
                if(aParam->IsArray()){
                    request.emplace("params", aParam);
                }else{
                    std::shared_ptr<Value>paramPtr = mAllocator->SharedAllocate<Value>(TYPE_ARRAY, *mAllocator);
                    Array& params = paramPtr->GetArray();
                    params.PushBack(aParam);
                    request.emplace("params", paramPtr);
                }
            }else{
                request.emplace("params", mAllocator->SharedAllocate<Value>(TYPE_NULL, *mAllocator));
            }
            if(aID == RPC_NOTIFICATION_ID){
                request.emplace("id", mAllocator->SharedAllocate<Value>(TYPE_NULL, *mAllocator));
            }else{
                request.emplace("id", mAllocator->SharedAllocate<Value>(static_cast<Number>(aID), *mAllocator));
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
    };
}}

#endif
