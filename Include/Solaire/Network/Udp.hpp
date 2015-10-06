#ifndef SOLAIRE_NETWORK_UDP_HPP
#define SOLAIRE_NETWORK_UDP_HPP

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
	\file Udp.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 5th September 2015
	Last Modified	: 5th September 2015
*/

#include "Ip.hpp"
#include "Socket.hpp"

namespace Solaire{

    typedef uint16_t UdpPort;

    template<class IP>
    using UdpSession = std::pair<IP, UdpPort>;

    typedef UdpSession<IPv4> UdpSessionIPv4;
    typedef UdpSession<IPv6> UdpSessionIPv6;

    class UdpSocket : public Socket{
    private:
        UdpPort mListenPort;
    public:
        UdpSocket();
        UdpSocket(const UdpPort aListenPort);
        ~UdpSocket();

        void SetListenPort(const UdpPort aListenPort);

        Status Send(const UdpSessionIPv4 aDestination, const void* const aData, const size_t aBytes);
        Status Send(const UdpSessionIPv4 aDestination, const Packet& aPacket);

        Status Receive(UdpSessionIPv4& aSource, size_t& aBytesRead, void* const aData, const size_t aDataSize);
        Status Receive(UdpSessionIPv4& aSource, Packet& aPacket);
    };
}


#endif
