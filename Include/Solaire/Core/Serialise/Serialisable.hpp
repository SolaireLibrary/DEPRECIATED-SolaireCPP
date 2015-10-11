#ifndef SOLAIRE_SERIALISABLE_HPP
#define SOLAIRE_SERIALISABLE_HPP

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
	\file Serialisable.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 3.0
	\date
	Created			: 30th September 2015
	Last Modified	: 30th September 2015
*/

#include "..\Strings\NumberParser.hpp"

namespace Solaire{

    namespace Json{
        class Value;
    }

    template<class T, class R = std::shared_ptr<T>, class Enable = void>
    class Serialisable{
        typedef R DeserialiseType;

        static std::shared_ptr<Json::Value> Serialise(Allocator& aParseAllocator, typename PassTypes<T>::ConstType aValue) = delete;
        static DeserialiseType Deserialise(Allocator& aDataAllocator, const Json::Value& aValue) = delete;
    };

    template<class T>
    static std::shared_ptr<Json::Value> Serialise(Allocator& aParseAllocator, typename PassTypes<T>::ConstType aValue){
        return Serialisable<T>::Serialise(aParseAllocator, aValue);
    }

    template<class T>
    static typename Serialisable<T>::DeserialiseType Deserialise(Allocator& aDataAllocator, const Json::Value& aValue){
        return Serialisable<T>::DeserialiseType(aDataAllocator, aValue);
    }


    template<class A, class B>
    static typename Serialisable<A>::DeserialiseType serial_cast(Allocator& aAllocator, typename Serialisable<B>::DeserialiseType aObject){
        const std::shared_ptr<const Json::Value> tmp = Serialise<B>((aAllocator, aObject));
        return Deserialise<B>(aAllocator, *tmp);
    }

    namespace SerialHelper{
        template<class Iterator>
        static Iterator CopyString(const Iterator aBegin, const ConstStringFragment aString){
            const size_t size = aString.Size();
            char* const dst = &(*aBegin);
            const char* const src = &(*aString.begin());
            std::memcpy(dst, src, size);
            return aBegin + size;
        }

        template<class Iterator>
        static Iterator CopyNumber(const Iterator aBegin, const double aNumber){
            char* const begin = &(*aBegin);
            const char* const end = NumericParse::ToString(begin, aNumber);
            return aBegin + (end - begin);
        }

        template<class Iterator>
        static Iterator CopyBool(const Iterator aBegin, const bool aBool){
            return CopyString(aBegin, aBool ? "true" : "false");
        }
    }
}


#endif
