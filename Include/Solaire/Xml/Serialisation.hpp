#ifndef SOLAIRE_XML_SERIALISATION_HPP
#define SOLAIRE_XML_SERIALISATION_HPP

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
	\file Serialisation.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 2nd October 2015
	Last Modified	: 2nd October 2015
*/

#include "Element.hpp"
#include "..\Core\Json\Value.hpp"
namespace Solaire{

    static std::shared_ptr<Xml::Element> JsonToXml(const Json::Value& aValue){
        Allocator& allocator = const_cast<Allocator&>(*aValue.pAllocator);
        std::shared_ptr<Xml::Element> element = allocator.SharedAllocate<Xml::Element>(allocator);

        switch(*aValue.pType){
        case Json::TYPE_NULL:
            element->SetName("Null");
            break;
        case Json::TYPE_BOOL:
            element->SetName("Bool");
            element->SetBool(*aValue.pBool);
            break;
        case Json::TYPE_NUMBER:
            element->SetName("Number");
            element->SetNumber(*aValue.pNumber);
            break;
        case Json::TYPE_STRING:
            element->SetName("String");
            element->SetString(*aValue.pString);
            break;
        case Json::TYPE_ARRAY:
            {
                const size_t size = aValue.pArray->Size();
                for(size_t i = 0; i < size; ++i){
                    element->AddChild(JsonToXml((*aValue.pArray)[i]));
                }
            }
            break;
        case Json::TYPE_OBJECT:
            {
                const auto end = aValue.pObject->end();
                for(auto i = aValue.pObject->begin(); i != end; ++i){
                    std::shared_ptr<Xml::Element> tmp = JsonToXml(*i->second);
                    tmp->SetName(i->first);
                    element->AddChild(tmp);
                }
            }
            break;
        }
        return element;
    }

    static std::shared_ptr<Json::Value> XmlToJson(const Xml::Element& aElement){

    }

}


#endif
