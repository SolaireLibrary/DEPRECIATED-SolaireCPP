#ifndef SOLAIRE_JSON_RPC_SERVER_INL
#define SOLAIRE_JSON_RPC_SERVER_INL

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
\file RpcServer.inl
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

    // RpcServer

    void RpcServer::SendError(const RpcRequestID aID, std::shared_ptr<RpcError> aError){
        RpcResponse response(GetAllocator());
        response.SetRequestID(aID);
        response.SetError(aError);

        SendResponse(response);
    }

    void RpcServer::SendError(const RpcRequestID aID, const RpcErrorCode aCode, ConstStringFragment aMessage, std::shared_ptr<Value> aErrorData){
        Allocator& allocator = GetAllocator();

        std::shared_ptr<RpcError> error = allocator.SharedAllocate<RpcError>(allocator);
        error->SetCode(aCode);
        error->SetMessage(aMessage);
        error->SetData(aErrorData);

        SendError(aID, error);
    }

    void RpcServer::SendError(const RpcRequestID aID, const RpcErrorCode aCode, ConstStringFragment aMessage){
        Allocator& allocator = GetAllocator();

        std::shared_ptr<RpcError> error = allocator.SharedAllocate<RpcError>(allocator);
        error->SetCode(aCode);
        error->SetMessage(aMessage);

        SendError(aID, error);
    }

    void RpcServer::SendResponse(const RpcRequestID aID, std::shared_ptr<Value> aData){
        RpcResponse response(GetAllocator());
        response.SetRequestID(aID);
        response.SetResult(aData);

        SendResponse(response);
    }

    void RpcServer::SendResponse(const RpcRequestID aID){
        Allocator& allocator = GetAllocator();
        SendResponse(aID, allocator.SharedAllocate<Value>(allocator, TYPE_NULL));
    }

    void RpcServer::Execute(const RpcRequest& aRequest){
        auto it = mFunctions.find(String(GetAllocator(), aRequest.GetMethodName()));
        if(it == mFunctions.end()){
            if(aRequest.IsRequest()){
                SendError(aRequest.GetRequestID(), RPC_METHOD_NOT_FOUND, "Could not find requested method on server mapping");
            }
        }else{
            it->second(aRequest);
        }
    }

    void RpcServer::MapFunction(String aString, const RpcFunction aFn){
        mFunctions.emplace(aString, aFn);
    }

    void RpcServer::MapFunction(String aString, const std::function<void(const std::shared_ptr<const Value>)> aFn){
        const RpcFunction wrapper = [=](const RpcRequest& aRequest){
            if(aRequest.IsNotification()){
                throw std::runtime_error("Json::RpcServer : Expected request, received notification instead");
            }

            aFn(aRequest.GetParams());
        };

        MapFunction(aString, aFn);
    }

    void RpcServer::MapFunction(String aString, const std::function<void()> aFn){
        const RpcFunction wrapper = [=](const RpcRequest& aRequest){
            if(aRequest.IsRequest()){
                Allocator& allocator = GetAllocator();
                const RpcRequestID id = aRequest.GetRequestID();

                try{
                    aFn();
                    SendResponse(id);
                }catch(std::exception& e){
                    SendError(id, RPC_EXCEPTION_THROWN_ON_SERVER, String(allocator, e.what()));
                }
            }else{
                aFn();
            }
        };

        MapFunction(aString, aFn);
    }


}}

#endif
