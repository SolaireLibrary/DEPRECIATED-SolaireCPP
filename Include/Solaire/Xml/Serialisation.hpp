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
                    switch(*i->second->pType){
                        case Json::TYPE_NULL:
                            element->AddAttribute(allocator.SharedAllocate<Xml::Attribute>(i->first, String(allocator, "null")));
                            break;
                        case Json::TYPE_BOOL:
                            element->AddAttribute(allocator.SharedAllocate<Xml::Attribute>(i->first, *i->second->pBool));
                            break;
                        case Json::TYPE_NUMBER:
                            element->AddAttribute(allocator.SharedAllocate<Xml::Attribute>(i->first, static_cast<double>(*i->second->pNumber)));
                            break;
                        case Json::TYPE_STRING:
                            element->AddAttribute(allocator.SharedAllocate<Xml::Attribute>(i->first, static_cast<ConstStringFragment>(*i->second->pString)));
                            break;
                        default:
                            {
                                std::shared_ptr<Xml::Element> tmp = JsonToXml(*i->second);
                                tmp->SetName(i->first);
                                element->AddChild(tmp);
                            }
                            break;
                    }
                }
            }
            break;
        }
        return element;
    }

    static std::shared_ptr<Json::Value> XmlToJson(const Xml::Element& aElement){
        Allocator& allocator = aElement.GetAllocator();
        std::shared_ptr<Json::Value> value = allocator.SharedAllocate<Json::Value>(allocator, Json::TYPE_NULL);

        const size_t attributeCount = aElement.GetAttributeCount();
        const size_t elementCount = aElement.GetChildCount();

        const auto AttributeToValue = [&](const Xml::Attribute& aAttribute)->std::shared_ptr<Json::Value>{
            std::shared_ptr<Json::Value> attributeValue = allocator.SharedAllocate<Json::Value>(allocator, Json::TYPE_NULL);

            if(aAttribute.IsBool()){
                *attributeValue->pBool = aAttribute.GetBool();
            }else if(aAttribute.IsNumber()){
                *attributeValue->pNumber = aAttribute.GetNumber();
            }else if(aAttribute.IsString()){
                *attributeValue->pString = aAttribute.GetString();
            }

            return attributeValue;
        };

        const auto ElementToValue = [&](const Xml::Element& aElement)->std::shared_ptr<Json::Value>{
            std::shared_ptr<Json::Value> elementValue;

            if(! aElement.IsNull()){
                if(aElement.GetAttributeCount() > 0 || aElement.GetChildCount() > 0){
                    elementValue = XmlToJson(aElement);
                }else{
                    elementValue = allocator.SharedAllocate<Json::Value>(allocator, Json::TYPE_NULL);
                    if(aElement.IsBool()){
                        *elementValue->pBool = aElement.GetBool();
                    }else if(aElement.IsNumber()){
                        *elementValue->pNumber = aElement.GetNumber();
                    }else if(aElement.IsString()){
                        *elementValue->pString = aElement.GetString();
                    }
                }
            }else{
                 elementValue = allocator.SharedAllocate<Json::Value>(allocator, Json::TYPE_NULL);
            }

            return elementValue;
        };

        //! \TODO check for attribute / element name collisions

        if(attributeCount > 0){
             goto PARSE_OBJECT;
        }else{
            const auto begin = aElement.ChildBegin();
            const auto end = aElement.ChildEnd();
            for(auto i = begin; i != end; ++i){
                for(auto j = begin; j != end; ++j){
                    if(i != j){
                        if((**i).GetName() == (**j).GetName()) goto PARSE_ARRAY;
                    }
                }
            }
            goto PARSE_OBJECT;
        }

        PARSE_OBJECT:
        {
            *value = Json::TYPE_OBJECT;
            const auto attributeBegin = aElement.AttributeBegin();
            const auto attributeEnd = aElement.AttributeEnd();
            const auto elementBegin = aElement.ChildBegin();
            const auto elementEnd = aElement.ChildEnd();

            for(auto i = attributeBegin; i != attributeEnd; ++i){
                const Xml::Attribute& attribute = **i;
                value->pObject->Add(attribute.GetName(), AttributeToValue(attribute));
            }

            for(auto i = elementBegin; i != elementEnd; ++i){
                const Xml::Element& element = **i;
                value->pObject->Add(element.GetName(), ElementToValue(element));
            }
        }
        return value;

        PARSE_ARRAY:
        {
            *value = Json::TYPE_ARRAY;
            const auto attributeBegin = aElement.AttributeBegin();
            const auto attributeEnd = aElement.AttributeEnd();
            const auto elementBegin = aElement.ChildBegin();
            const auto elementEnd = aElement.ChildEnd();

            for(auto i = attributeBegin; i != attributeEnd; ++i){
                value->pArray->PushBack(AttributeToValue(**i));
            }

            for(auto i = elementBegin; i != elementEnd; ++i){
                value->pArray->PushBack(ElementToValue(**i));
            }
        }
        return value;
    }

}


#endif
