#ifndef SOLAIRE_JSON_RPC_RESPOSE_HPP
#define SOLAIRE_JSON_RPC_RESPOSE_HPP

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
\file RpcResponse.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 5th October 2015
Last Modified	: 5th October 2015
*/

#include "RpcRequest.hpp"
#include "RpcError.hpp"

namespace Solaire{ namespace Json{

    class RpcResponse{
    private:
        std::shared_ptr<RpcError> mError;
        std::shared_ptr<Value> mResult;
        RpcRequestID mID;
    protected:
        enum : RpcRequestID{
            RPC_NOTIFICATION_ID = INT64_MAX
        };
    public:
        static std::shared_ptr<Value> Serialise(const RpcRequest& aRequest);
    public:
        RpcResponse(Allocator& aAllocator);
        ~RpcResponse();

        void SetRequestID(const RpcRequestID aID);
        RpcRequestID GetRequestID() const;

        bool HasResult() const;
        void SetResult(std::shared_ptr<Value> aResult);
        std::shared_ptr<const Value> GetResult() const;

        bool HasError() const;
        void SetError(std::shared_ptr<RpcError> aError);
        std::shared_ptr<const RpcError> GetError() const;

        Allocator& GetAllocator() const;
    };

}}

//#include "RpcRequest.inl"

#endif
