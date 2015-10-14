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

#include "..\Core\Init.hpp"
#include<stdio.h>
#include<winsock2.h>

namespace Solaire{

    namespace UdpImplementation{

        enum{
            BUFFER_SIZE = 512
        };

        static bool InitialiseWinsock(WSADATA& aWsa){
            return WSAStartup(MAKEWORD(2,2), &aWsa) != 0;
        }

        static bool CreateSocket(SOCKET& aSocket){
            return (aSocket = socket(AF_INET, SOCK_DGRAM, 0 )) == INVALID_SOCKET;
        }

        struct Data{
            SOCKET socketStruct;
            sockaddr_in addressThis;
            sockaddr_in addressOther;
            int slen , recv_len;
            char buf[BUFFER_SIZE];
            WSADATA wsa;

            Data(){
                InitialiseWinsock(wsa);
                CreateSocket(socketStruct);
            }

            ~Data(){
                closesocket(socketStruct);
            }

            bool Bind(const UdpPort aPort){
                addressThis.sin_family = AF_INET;
                addressThis.sin_addr.s_addr = INADDR_ANY;
                addressThis.sin_port = htons(aPort);
                return bind(socketStruct, reinterpret_cast<sockaddr*>(&addressThis), sizeof(sockaddr_in)) == SOCKET_ERROR;
            }
        };
    }

    // UdpSocket

    UdpSocket::UdpSocket():
        mListenPort(0),
        mImplementationData(new UdpImplementation::Data())
    {

    }

    UdpSocket::UdpSocket(const UdpPort aListenPort):
        mListenPort(0),
        mImplementationData(new UdpImplementation::Data())
    {
        ListenToPort(aListenPort);
    }

    UdpSocket::~UdpSocket(){
        delete mImplementationData;
    }

    void UdpSocket::ListenToPort(const UdpPort aListenPort){
        mImplementationData->Bind(aListenPort);
    }

    UdpSocket::Status UdpSocket::Send(const UdpSessionIPv4 aDestination, const void* const aData, const size_t aBytes){

    }

    UdpSocket::Status UdpSocket::Send(const UdpSessionIPv4 aDestination, const Packet& aPacket){

    }

    UdpSocket::Status UdpSocket::Receive(UdpSessionIPv4& aSource, size_t& aBytesRead, void* const aData, const size_t aDataSize){

    }

    UdpSocket::Status UdpSocket::Receive(UdpSessionIPv4& aSource, Packet& aPacket){

    }

}


#endif
