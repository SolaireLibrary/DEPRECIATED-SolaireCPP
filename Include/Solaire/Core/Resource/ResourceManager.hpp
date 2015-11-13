#ifndef SOLAIRE_RESOURCE_MANAGER_HPP
#define SOLAIRE_RESOURCE_MANAGER_HPP

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
	\file ResourceManager.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 13th November 2015
	Last Modified	: 13th November 2015
*/

#include "Resource.hpp"

namespace Solaire{

	class ResourceManager {
	public:
		typedef uint64_t GroupID;
	private:
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
	public:
		virtual ~ResourceManager(){}

		virtual Resource::ID AddResource(ResourcePtr) = 0;
		virtual bool AddResource(const Resource::ID, ResourcePtr) = 0;
		virtual bool FreeResource(const Resource::ID) = 0;

		virtual void FreeAllResources() = 0;

		virtual ResourcePtr GetResource(const Resource::ID) const = 0;

		virtual GroupID CreateGroup() = 0;
		virtual void AddToGroup(const GroupID, const Resource::ID) = 0;
		virtual void FreeGroup(const GroupID) = 0;
	};
    
}


#endif
