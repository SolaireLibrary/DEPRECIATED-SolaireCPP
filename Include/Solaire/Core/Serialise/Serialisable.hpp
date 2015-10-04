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

    template<class T>
    class Serialisable{

        template<class Iterator>
        Allocator::SharedPointer<T> Read(const SerialSystem& aSystem) = delete;

        template<class Iterator>
        void Write(const T& aValue, SerialSystem& aSystem, std::ostream& aStream) = delete;
    };

    template<class T, class ...PARAMS>
    static Allocator::SharedPointer<T> Deserialise(const SerialSystem& aSystem, std::istream& aStream, PARAMS&&... aParams){
        return Serialisable<T>().Read(aSystem, aStream, aParams...);
    }

    template<class T, class ...PARAMS>
    static void Serialise(const T& aValue, const SerialSystem& aSystem, std::ostream& aStream, PARAMS&&... aParams){
        Serialisable<T>().Write(aValue, aSystem, aStream, aParams...);
    }

    template<class A, class B>
    static Allocator::SharedPointer<A> serial_cast(Allocator::SharedPointer<B> aPointer, const SerialSystem& aSystem){
        std::stringstream ss;
        Serialise<B>(*aPointer, aSystem, ss);
        return Deserialise<A>(aSystem, ss);
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
