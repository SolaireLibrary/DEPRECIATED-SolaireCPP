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

        void SendError(const RpcRequestID aID, const RpcErrorID aCode, std::shared_ptr<RpcError> aError);
        void SendError(const RpcRequestID aID, const RpcErrorID aCode, ConstStringFragment aMessage, std::shared_ptr<Value> aErrorData);
        void SendError(const RpcRequestID aID, const RpcErrorID aCode, ConstStringFragment aMessage);

        void SendResponse(const RpcRequestID aID, std::shared_ptr<Value> aData);
        void SendResponse(const RpcRequestID aID);

        void MapFunction(String aString, const RpcFunction);

        void Execute(const RpcRequest& aRequest);
    public:
    };
}}

//#include "RpcServer.inl"

#endif
