#ifndef SOLAIRE_NETWORK_PACKET_HPP
#define SOLAIRE_NETWORK_PACKET_HPP

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
	\file Packet.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 5th September 2015
	Last Modified	: 5th September 2015
*/

namespace Solaire{

    class Socket;

    class Packet{
    public:
        friend Socket;
    protected:
        virtual void OnPreSend() = 0;
        virtual void OnPostSend() = 0;
        virtual void OnPostRecieve() = 0;
    public:
    };
}


#endif
