#ifndef SOLAIRE_NETWORK_IP_INL
#define SOLAIRE_NETWORK_IP_INL

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
	\file Ip.inl
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 5th September 2015
	Last Modified	: 5th September 2015
*/

#include <algorithm>
#include "..\Core\Strings\NumberParser.hpp"

namespace Solaire{

    // IPv4
    constexpr IPv4::IPv4() :
        address(0)
    {}

    constexpr IPv4::IPv4(const uint8_t aByte0, const uint8_t aByte1, const uint8_t aByte2, const uint8_t aByte3):
        address((aByte0 << 24) | (aByte1 << 16) | (aByte2 << 8) | aByte3)
    {}

    constexpr IPv4::IPv4(const uint32_t aAddress):
        address(aAddress)
    {}

    // IPv6
    IPv6::IPv6(){
        bytes[0] = 0;
        bytes[1] = 0;
        bytes[2] = 0;
        bytes[3] = 0;
        bytes[4] = 0;
        bytes[5] = 0;
        bytes[6] = 0;
        bytes[7] = 0;
    }

    // Serialisable<IPv4>

    void Serialisable<IPv4>::Serialise(IPv4 aValue, const SerialSystem& aSystem, const SerialIndex aIndex, SerialArray& aRoot){
        char buf[16];
        char* head = buf;

        head = NumericParse::ToString(head, aValue.bytes[0]);
        *head = '.';
        ++head;
        head = NumericParse::ToString(head, aValue.bytes[1]);
        *head = '.';
        ++head;
        head = NumericParse::ToString(head, aValue.bytes[2]);
        *head = '.';
        ++head;
        head = NumericParse::ToString(head, aValue.bytes[3]);

        aRoot.Write<ConstStringFragment>(aIndex, ConstStringFragment(buf, head));
    }

    Serialisable<IPv4>::DeserialiseType Serialisable<IPv4>::Deserialise(const SerialSystem& aSystem, const SerialIndex aIndex, const SerialArray& aRoot){
        char buf[16];
        uint8_t length;
        char* byte0 = buf;
        char* byte1 = buf;
        char* byte2 = buf;
        char* byte3 = buf;

        {
            ConstStringFragment str = aRoot.Read<ConstStringFragment>(aIndex);
            length = std::min<uint8_t>(16, str.Size());
            std::memcpy(buf, str.begin(), length);
        }

        char* const end = buf + length;

        for(char* i = byte0; i != end; ++i) if(*i == '.'){
            byte1 = i + 1;
            break;
        }

        for(char* i = byte1; i != end; ++i) if(*i == '.'){
            byte2 = i + 1;
            break;
        }

        for(char* i = byte2; i != end; ++i) if(*i == '.'){
            byte3 = i + 1;
            break;
        }

        char* it = byte0;
        IPv4 ip;
        ip.bytes[0] = ParseNumber<uint8_t, char*>(byte0, byte1 - 1, it);
        ip.bytes[1] = ParseNumber<uint8_t, char*>(byte1, byte2 - 1, it);
        ip.bytes[2] = ParseNumber<uint8_t, char*>(byte2, byte3 - 1, it);
        ip.bytes[3] = ParseNumber<uint8_t, char*>(byte3, end, it);
        return ip;
    }

    // Serialisable<IPv6>

    void Serialisable<IPv6>::Serialise(IPv6 aValue, const SerialSystem& aSystem, const SerialIndex aIndex, SerialArray& aRoot){

    }

    Serialisable<IPv6>::DeserialiseType Serialisable<IPv6>::Deserialise(const SerialSystem& aSystem, const SerialIndex aIndex, const SerialArray& aRoot){

    }


}


#endif
