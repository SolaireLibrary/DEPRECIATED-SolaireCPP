#ifndef SOLAIRE_STRING_FILE_FACTORY_HPP
#define SOLAIRE_STRING_FILE_FACTORY_HPP

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
	\file StringFileResource.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 27th September 2015
	Last Modified	: 17th October 2015
*/

#include <fstream>
#include "StringResource.hpp"

namespace Solaire{


    class StringFileResource : public StringResource{
    protected:
        String mFilename;
    public:
        StringFileResource(ResourceManager& aManager, const String& aFilename):
            StringResource(aManager, aFilename.GetAllocator()),
            mFilename(aFilename)
        {}

        // Inherited from resource

        void Reload() override{
            std::ifstream file(mFilename.CString());
            if(! file.is_open()) throw std::runtime_error("StringFileResource : Could not open requested file for reading");

            char c;
            while(! file.eof()){
                file >> c;
                mString += c;
            }

            file.close();
        }
    };

}

#endif
