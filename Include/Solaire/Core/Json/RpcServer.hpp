#ifndef SOLAIRE_JSON_RPC_SERVER_HPP
#define SOLAIRE_JSON_RPC_SERVER_HPP

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
\file RpcServer.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 5th October 2015
Last Modified	: 5th October 2015
*/

#include <map>
#include "RpcRequest.hpp"
#include "RpcError.hpp"
#include "RpcResponse.hpp"

namespace Solaire{ namespace Json{

    class RpcServer{
    public:
        typedef std::function<void(const RpcRequest&)> RpcFunction;
    private:
        std::map<String, RpcFunction> mFunctions;
    protected:
        virtual void SendResponse(const RpcResponse& aResponse) = 0;

        void SendError(const RpcRequestID aID, std::shared_ptr<RpcError> aError);
        void SendError(const RpcRequestID aID, const RpcErrorCode aCode, ConstStringFragment aMessage, std::shared_ptr<Value> aErrorData);
        void SendError(const RpcRequestID aID, const RpcErrorCode aCode, ConstStringFragment aMessage);

        void SendResponse(const RpcRequestID aID, std::shared_ptr<Value> aData);
        void SendResponse(const RpcRequestID aID);

        template<class T>
        void SendAndSerialiseResponse(const RpcRequestID aID, T aValue){
            Allocator& allocator = GetAllocator();
            SendResponse(aID, JsonSerialise<T>(allocator, allocator, aValue));
        }

        template<class ...Params>
        void SendAndSerialiseResponse(const RpcRequestID aID, Params&& ...aParams){
            Allocator& allocator = GetAllocator();

            std::shared_ptr<Value> results = allocator.SharedAllocate<Value>(allocator, TYPE_ARRAY);
            results->pArray->PushBack(JsonSerialise(allocator, allocator, aParams...));

            SendResponse(aID, results);
        }

        void Execute(const RpcRequest& aRequest);

        void MapFunction(String aString, const RpcFunction aFn);
        void MapFunction(String aString, const std::function<void(const std::shared_ptr<const Value>)> aFn);
        void MapFunction(String aString, const std::function<void()> aFn);

        template<class R>
        void MapAutoSerialiseFunction(String aString, const std::function<R(const RpcRequestID, const std::shared_ptr<const Value>)> aFn){
            const RpcFunction wrapper = [=](const RpcRequest& aRequest){
                Allocator& allocator = GetAllocator();

                if(aRequest.IsNotification()){
                    throw std::runtime_error("Json::RpcServer : Expected request, received notification instead");
                }

                const RpcRequestID id = aRequest.GetRequestID();
                try{
                    SendAndSerialiseResponse<R>(aFn(id, aRequest.GetParams()));
                }catch(std::exception& e){
                    SendError(id, RPC_EXCEPTION_THROWN_ON_SERVER, String(e.what(), allocator));
                }
            };

            MapFunction(aString, wrapper);
        }

        template<class R>
        void MapAutoSerialiseFunction(String aString, const std::function<R(const RpcRequestID)> aFn){
            const RpcFunction wrapper = [=](const RpcRequest& aRequest){

                if(aRequest.IsNotification()){
                    throw std::runtime_error("Json::RpcServer : Expected request, received notification instead");
                }

                Allocator& allocator = GetAllocator();
                const RpcRequestID id = aRequest.GetRequestID();
                try{
                    SendAndSerialiseResponse<R>(aFn(id));
                }catch(std::exception& e){
                    SendError(id, RPC_EXCEPTION_THROWN_ON_SERVER, String(e.what(), allocator));
                }
            };

            MapFunction(aString, wrapper);
        }
    public:
        virtual Allocator& GetAllocator() const = 0;
    };
}}

#include "RpcServer.inl"

#endif