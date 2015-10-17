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
	Created			: 17th October 2015
	Last Modified	: 17th October 2015
*/

#include "Resource.hpp"
#include "IDGenerator.hpp"

namespace Solaire{

    class ResourceManager{
    private:
        IDGenerator<ResourceID, true> mIDGenerator;
        DynamicArray<ResourcePtr> mResources;
    public:
        ResourceManager(Allocator& aAllocator):
            mIDGenerator(aAllocator),
            mResources(aAllocator)
        {}

        template<class T, class ...PARAMS, typename Enable = typename std::enable_if<std::is_base_of<Resource, T>::value>::type>
        std::shared_ptr<T> Generate(PARAMS ...aParams){
            std::shared_ptr<T> tmp = mResources.GetAllocator().SharedAllocate<T>(mIDGenerator, aParams...);
            const ResourceID id = tmp->GetID();

            if(id < mResources.Size()){
                mResources.PushBack(tmp);
            }else{
                mResources[id].swap(tmp);
            }

            return tmp;
        }

        void Delete(const ConstResourcePtr aResource){
            const ResourceID id = aResource->GetID();
            ResourcePtr tmp;
            mResources[id].swap(tmp);
        }

        template<class T, class ...PARAMS, typename Enable = typename std::enable_if<std::is_base_of<Resource, T>::value>::type>
        std::shared_ptr<T> Get(const ResourceID aID){
            if(aID < mResources.Size()){
                return std::shared_ptr<T>();
            }else{
                //! \bug Resource return is not typechecked
                return std::static_pointer_cast<T>(mResources[aID]);
            }
        }

    };

}

#endif
