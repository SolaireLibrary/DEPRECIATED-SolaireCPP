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
	\version 1.0
	\date
	Created			: 30th September 2015
	Last Modified	: 30th September 2015
*/

#include <sstream>
#include "SerialisationInterface.hpp"
#include "..\Strings\NumberParser.hpp"

namespace Solaire{

    template<class T, class R = std::shared_ptr<T>, class Enable = void>
    class Serialisable{
        typedef R DeserialiseType;

        static void Serialise(T aValue, const SerialSystem& aSystem, const SerialIndex aIndex, SerialArray& aRoot) = delete;
        static DeserialiseType Deserialise(const SerialSystem& aSystem, const SerialIndex aIndex, const SerialArray& aRoot) = delete;
    };


    /*template<class A, class B>
    static Allocator::SharedPointer<A> serial_cast(Allocator::SharedPointer<B> aPointer, const SerialSystem& aSystem){
        std::stringstream ss;
        Serialise<B>(*aPointer, aSystem, ss);
        return Deserialise<A>(aSystem, ss);
    }*/

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


    // Primatives

    template<class T>
    class Serialisable<T, T, typename std::enable_if<IsSerialPrimative<T>>::type>
    {
    public:
        typedef T DeserialiseType;

        static void Serialise(T aValue, const SerialSystem& aSystem, const SerialIndex aIndex, SerialArray& aRoot){
            aRoot.Write<T>(aIndex, aValue);
        }

        static DeserialiseType Deserialise(const SerialSystem& aSystem, const SerialIndex aIndex, const SerialArray& aRoot){
            return aRoot.Read<T>(aIndex);
        }
    };

    template<class T>
    class Serialisable<std::shared_ptr<T>, typename Serialisable<T>::DeserialiseType, typename std::enable_if<IsSerialPrimative<T>>::type>
    {
    public:
        typedef typename Serialisable<T>::DeserialiseType DeserialiseType;

        static void Serialise(std::shared_ptr<T> aValue, const SerialSystem& aSystem, const SerialIndex aIndex, SerialArray& aRoot){
            Serialisable<T>::Serialise(*aValue, aSystem, aIndex, aRoot);
        }

        static DeserialiseType Deserialise(const SerialSystem& aSystem, const SerialIndex aIndex, const SerialArray& aRoot){
            return Serialisable<T>::DeserialiseType(aSystem, aIndex, aRoot);
        }
    };
}


#endif
