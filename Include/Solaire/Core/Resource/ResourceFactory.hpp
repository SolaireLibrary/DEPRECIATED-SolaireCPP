#ifndef SOLAIRE_RESOURCE_FACTORY_HPP
#define SOLAIRE_RESOURCE_FACTORY_HPP

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
	\file ResourceFactory.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 20th September 2015
	Last Modified	: 17th October 2015
*/

#include <cstdint>
#include "Resource.hpp"

namespace Solaire{

    class ResourceFactory{
    private:
        uint16_t mUsers;
    private:
        ResourceFactory& operator=(const ResourceFactory& aOther) = delete;
        ResourceFactory& operator=(ResourceFactory&& aOther) = delete;
    protected:
        virtual void Lock() = 0;
        virtual void Unlock() = 0;
    public:
        ResourceFactory() :
            mUsers(0)
        {}

        ResourceFactory(const ResourceFactory& aOther) :
            mUsers(0)
        {}

        ResourceFactory(ResourceFactory&& aOther) :
            mUsers(0)
        {}

        virtual ~ResourceFactory(){

        }

        bool IsLocked() const{
            return mUsers != 0;
        }

        size_t UserCount() const{
            return mUsers;
        }

        void lock(){
            const uint16_t users = mUsers++;
            if(users == 0){
                Lock();
            }
        }

        void unlock(){
            const uint16_t users = mUsers--;
            if(users == 0){
                Unlock();
            }
        }

        bool try_lock(){
            lock();
            return true;
        }
    };

}

#endif
