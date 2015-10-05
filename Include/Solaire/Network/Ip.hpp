#ifndef SOLAIRE_NETWORK_IP_HPP
#define SOLAIRE_NETWORK_IP_HPP

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
	\file Ip.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 5th September 2015
	Last Modified	: 5th September 2015
*/

#include "..\Core\Serialise\Serialisable.hpp"

namespace Solaire{

    struct IPv4{
        union{
            uint8_t bytes[4];
            uint32_t address;
        };

        constexpr IPv4();
        constexpr IPv4(const uint8_t, const uint8_t, const uint8_t, const uint8_t);
        constexpr IPv4(const uint32_t);
    };

    struct IPv6{
        union{
            uint8_t bytes[8];
        };

        IPv6();
    };

    template<>
    class Serialisable<IPv4>{
        typedef IPv4 DeserialiseType;

        static void Serialise(IPv4 aValue, const SerialSystem& aSystem, const SerialIndex aIndex, SerialArray& aRoot);
        static DeserialiseType Deserialise(const SerialSystem& aSystem, const SerialIndex aIndex, const SerialArray& aRoot);
    };

    template<>
    class Serialisable<IPv6>{
        typedef IPv6 DeserialiseType;

        static void Serialise(IPv6 aValue, const SerialSystem& aSystem, const SerialIndex aIndex, SerialArray& aRoot);
        static DeserialiseType Deserialise(const SerialSystem& aSystem, const SerialIndex aIndex, const SerialArray& aRoot);
    };

}

#include "Ip.inl"

#endif
