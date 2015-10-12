#ifndef SOLAIRE_NETWORK_UDP_WINDOWS_HPP
#define SOLAIRE_NETWORK_UDP_WINDOWS_HPP

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
	\file UdpWindows.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 12th October 2015
	Last Modified	: 12th October 2015
*/

#include<stdio.h>
#include<winsock2.h>

namespace Solaire{

    namespace UdpWindows{
        struct Data{

        };
    }

    // UdpSocket

    UdpSocket::UdpSocket():
        mListenPort(0),
        mImplementationData(new UdpWindows::Data())
    {

    }

    UdpSocket::UdpSocket(const UdpPort aListenPort):
        mListenPort(0),
        mImplementationData(new UdpWindows::Data())
    {
        ListenToPort(aListenPort);
    }

    UdpSocket::~UdpSocket(){

    }

    void UdpSocket::ListenToPort(const UdpPort aListenPort){

    }

    Status UdpSocket::Send(const UdpSessionIPv4 aDestination, const void* const aData, const size_t aBytes){

    }

    Status UdpSocket::Send(const UdpSessionIPv4 aDestination, const Packet& aPacket){

    }

    Status UdpSocket::Receive(UdpSessionIPv4& aSource, size_t& aBytesRead, void* const aData, const size_t aDataSize){

    }

    Status UdpSocket::Receive(UdpSessionIPv4& aSource, Packet& aPacket){

    }

}


#endif
