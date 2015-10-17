#ifndef SOLAIRE_RESOURCE_HPP
#define SOLAIRE_RESOURCE_HPP

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
	\file Resource.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 17th October 2015
	Last Modified	: 17th October 2015
*/

#include <memory>

namespace Solaire{

    class Resource;
    class ResourceManager;

    typedef uint16_t ResourceID;
    typedef std::shared_ptr<Resource> ResourcePtr;
    typedef std::shared_ptr<const Resource> ConstResourcePtr;

    class Resource : std::enable_shared_from_this<Resource>{
    private:
        ResourceID mID;
    public:
        friend ResourceManager;

        virtual ~Resource(){

        }

        ResourceID GetResourceID() const{
            return mID;
        }

        virtual ResourcePtr Reload() const = 0;

        bool operator==(const Resource& aOther) const{
            return mID == aOther->mID;
        }

        bool operator!=(const Resource& aOther) const{
            return mID != aOther->mID;
        }
    };
}

#endif
