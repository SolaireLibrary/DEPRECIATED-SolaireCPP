#ifndef SOLAIRE_PRIMATIVE_SERIALISABLE_HPP
#define SOLAIRE_PRIMATIVE_SERIALISABLE_HPP

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
	\file PrimativeSerialisable.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 30th September 2015
	Last Modified	: 30th September 2015
*/

#include "Serialisable.hpp"
#include "..\Json\Value.hpp"

namespace Solaire{

    template<class T>
    class Serialisable<T, T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
    {
    public:
        typedef T DeserialiseType;

        static std::shared_ptr<Json::Value> Serialise(Allocator& aParseAllocator, const T aValue){
            return aParseAllocator.SharedAllocate<Json::Value>(aParseAllocator, aValue);
        }

        static DeserialiseType Deserialise(Allocator& aDataAllocator, const Json::Value& aValue){
            return static_cast<T>(aValue.pNumber);
        }
    };

    template<>
    class Serialisable<bool, bool>
    {
    public:
        typedef bool DeserialiseType;

        static std::shared_ptr<Json::Value> Serialise(Allocator& aParseAllocator, const bool aValue){
            return aParseAllocator.SharedAllocate<Json::Value>(aParseAllocator, aValue);
        }

        static bool Deserialise(Allocator& aDataAllocator, const Json::Value& aValue){
            return *aValue.pBool;
        }
    };

    template<>
    class Serialisable<char, char>
    {
    public:
        typedef char DeserialiseType;

        static std::shared_ptr<Json::Value> Serialise(Allocator& aParseAllocator, const bool aValue){
            return aParseAllocator.SharedAllocate<Json::Value>(aParseAllocator, aValue);
        }

        static char Deserialise(Allocator& aDataAllocator, const Json::Value& aValue){
            return *aValue.pString;
        }
    };

    template<class T>
    class Serialisable<std::shared_ptr<T>, typename Serialisable<T>::DeserialiseType>
    {
    public:
        typedef typename Serialisable<T>::DeserialiseType DeserialiseType;

        static std::shared_ptr<Json::Value> Serialise(Allocator& aParseAllocator, const std::shared_ptr<const T> aValue){
            return Serialisable<T>::Serialise(aParseAllocator, *aValue);
        }

        static DeserialiseType Deserialise(Allocator& aDataAllocator, const Json::Value& aValue){
            return Serialisable<T>::Deserialise(aDataAllocator, aValue);
        }
    };
}


#endif
