#ifndef SOLAIRE_XML_SERIALISATION_HPP
#define SOLAIRE_JSON_SERIALISATION_HPP

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
#include "..\Core\Serialise\SerialisationInterface.hpp"

namespace Solaire{ namespace Xml{

    class SerialSystem;
    class SerialObject;

    class SerialArray : public Core::SerialArray{
    private:
        Core::Allocator::SharedPointer<Element> mElement;

        ElementPointer LookupOrCreateIndex(const Core::SerialIndex aIndex){
            const Core::SerialIndex size = mElement->GetChildCount();

            if(aIndex < size){
                return *(mElement->ChildBegin() + 1);
            }else{
                Core::Allocator& allocator = mElement->GetAllocator();
                for(Core::SerialIndex i = size; i <= aIndex; ++i){
                    ElementPointer element = allocator.SharedAllocate<Element>(Core::String("Null", allocator));
                    mElement->AddChild(element);
                }
                return *(mElement->ChildBegin() + 1);
            }
        }

        ElementPointer LookupIndex(const Core::SerialIndex aIndex) const{
            const Core::SerialIndex size = mElement->GetChildCount();
            if(aIndex < size){
                return *(mElement->ChildBegin() + 1);
            }else{
                return ElementPointer();
            }
        }

    public:
        friend SerialObject;
        friend SerialSystem;

        SerialArray(ElementPointer aElement) :
            mElement(aElement)
        {}

        ~SerialArray(){

        }

        // Inherited from SerialArray

        Core::SerialIndex Size() const override{
            return mElement->GetChildCount();
        }

        Core::SerialType TypeOf(const Core::SerialIndex aIndex) const override{
            const ElementPointer element = LookupIndex(aIndex);
            const Core::ConstStringFragment name = element->GetName();

            if(name == "Null" || element->IsNull()){
                return Core::SERIAL_TYPE_N;
            }else if(name == "Bool" || element->IsBool()){
                return Core::SERIAL_TYPE_B;
            }else if(name == "Number" || element->IsNumber()){
                return Core::SERIAL_TYPE_D;
            }else if(name == "String" || element->IsString()){
                return Core::SERIAL_TYPE_S;
            }else if(name == "Array"){
                return Core::SERIAL_TYPE_A;
            }else{
                return Core::SERIAL_TYPE_O;
            }
        }

        bool ReadB(const Core::SerialIndex aIndex) const override{
            return LookupIndex(aIndex)->GetBool();
        }

        char ReadC(const Core::SerialIndex aIndex) const override{
            return LookupIndex(aIndex)->GetString()[0];
        }

        uint8_t ReadU8(const Core::SerialIndex aIndex) const override{
            return static_cast<uint8_t>(LookupIndex(aIndex)->GetNumber());
        }

        uint16_t ReadU16(const Core::SerialIndex aIndex) const override{
            return static_cast<uint16_t>(LookupIndex(aIndex)->GetNumber());
        }

        uint32_t ReadU32(const Core::SerialIndex aIndex) const override{
            return static_cast<uint32_t>(LookupIndex(aIndex)->GetNumber());
        }

        uint64_t ReadU64(const Core::SerialIndex aIndex) const override{
            return static_cast<uint64_t>(LookupIndex(aIndex)->GetNumber());
        }

        int8_t ReadI8(const Core::SerialIndex aIndex) const override{
            return static_cast<int8_t>(LookupIndex(aIndex)->GetNumber());
        }

        int16_t ReadI16(const Core::SerialIndex aIndex) const override{
            return static_cast<int16_t>(LookupIndex(aIndex)->GetNumber());
        }

        int32_t ReadI32(const Core::SerialIndex aIndex) const override{
            return static_cast<int32_t>(LookupIndex(aIndex)->GetNumber());
        }

        int64_t ReadI64(const Core::SerialIndex aIndex) const override{
            return static_cast<int64_t>(LookupIndex(aIndex)->GetNumber());
        }

        float ReadF(const Core::SerialIndex aIndex) const override{
            return static_cast<float>(LookupIndex(aIndex)->GetNumber());
        }

        double ReadD(const Core::SerialIndex aIndex) const override{
            return static_cast<double>(LookupIndex(aIndex)->GetNumber());
        }

        Core::SerialString ReadS(const Core::SerialIndex aIndex) const override{
            return LookupIndex(aIndex)->GetString();
        }

        Core::SerialArrayPtr ReadA(const Core::SerialIndex aIndex) override{
            return mElement->GetAllocator().SharedAllocate<SerialArray>(LookupIndex(aIndex));
        }

        Core::SerialObjectPtr ReadO(const Core::SerialIndex aIndex) override;

        Core::ConstSerialArrayPtr ReadA(const Core::SerialIndex aIndex) const override{
            return mElement->GetAllocator().SharedAllocate<SerialArray>(LookupIndex(aIndex));
        }

        Core::ConstSerialObjectPtr ReadO(const Core::SerialIndex aIndex) const override;

        void WriteN(const Core::SerialIndex aIndex) override{
            ElementPointer element = LookupOrCreateIndex(aIndex);
            element->SetNull();
            element->SetName("Null");
        }

        void WriteB(const Core::SerialIndex aIndex, const bool aValue) override{
            ElementPointer element = LookupOrCreateIndex(aIndex);
            element->SetBool(aValue);
            element->SetName("Bool");
        }

        void WriteC(const Core::SerialIndex aIndex, const char aValue) override{
            WriteS(aIndex, Core::SerialString(&aValue, (&aValue) + 1));
        }

        void WriteU8(const Core::SerialIndex aIndex, const uint8_t aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteU16(const Core::SerialIndex aIndex, const uint16_t aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteU32(const Core::SerialIndex aIndex, const uint32_t aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteU64(const Core::SerialIndex aIndex, const uint64_t aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteI8(const Core::SerialIndex aIndex, const int8_t aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteI16(const Core::SerialIndex aIndex, const int16_t aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteI32(const Core::SerialIndex aIndex, const int32_t aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteI64(const Core::SerialIndex aIndex, const int64_t aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteF(const Core::SerialIndex aIndex, const float aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteD(const Core::SerialIndex aIndex, const double aValue) override{
            ElementPointer element = LookupOrCreateIndex(aIndex);
            element->SetNumber(aValue);
            element->SetName("Number");
        }

        void WriteS(const Core::SerialIndex aIndex, const Core::SerialString aValue) override{
            ElementPointer element = LookupOrCreateIndex(aIndex);
            element->SetString(aValue);
            element->SetName("String");
        }

        void WriteA(const Core::SerialIndex aIndex, Core::SerialArrayPtr aArray) override{
            ElementPointer element = std::static_pointer_cast<SerialArray>(aArray)->mElement;
            element->SetName("Array");
            mElement->AddChild(element);
        }

        void WriteO(const Core::SerialIndex aIndex, Core::SerialObjectPtr aObject) override;
    };

    class SerialObject : public Core::SerialObject{
    private:
        ElementPointer mElement;

        ElementPointer LookupOrCreateTagE(const Core::SerialTag aTag){
            Element::ConstElementIterator it = mElement->FindFirstChild(aTag);

            if(it != mElement->ChildEnd()){
                return *it;
            }else{
                Core::Allocator& allocator = mElement->GetAllocator();
                ElementPointer element = allocator.SharedAllocate<Element>(Core::String(aTag, allocator));
                element->AddAttribute(allocator.SharedAllocate<Attribute>(Core::String("type", allocator)));
                mElement->AddChild(element);
                return element;
            }
        }

        AttributePointer LookupOrCreateTagA(const Core::SerialTag aTag){
            Element::AttributeIterator it = mElement->FindAttribute(aTag);

            if(it != mElement->AttributeEnd()){
                return *it;
            }else{
                Core::Allocator& allocator = mElement->GetAllocator();
                AttributePointer attribute = allocator.SharedAllocate<Attribute>(Core::String(aTag, allocator));
                mElement->AddAttribute(attribute);
                return attribute;
            }
        }

        ElementPointer LookupTagE(const Core::SerialTag aTag) const{
            Element::ConstElementIterator it = mElement->FindFirstChild(aTag);
            if(it == mElement->ChildEnd()) return ElementPointer();
            return *it;
        }

        AttributePointer LookupTagA(const Core::SerialTag aTag) const{
            Element::AttributeIterator it = mElement->FindAttribute(aTag);
            if(it == mElement->AttributeEnd()) return AttributePointer();
            return *it;
        }

    public:
        friend SerialArray;
        friend SerialSystem;

        SerialObject(ElementPointer aElement) :
            mElement(aElement)
        {}

        ~SerialObject(){

        }

        // Inherited from SerialObject

        Core::SerialType TypeOf(const Core::SerialTag aTag) const override{
            const ConstAttributePointer attribute = LookupTagA(aTag);
            if(attribute){
                if(attribute->IsNull()){
                    return Core::SERIAL_TYPE_N;
                }else if(attribute->IsBool()){
                    return Core::SERIAL_TYPE_B;
                }else if(attribute->IsNumber()){
                    return Core::SERIAL_TYPE_D;
                }else if(attribute->IsString()){
                    return Core::SERIAL_TYPE_S;
                }
                throw std::runtime_error("Xml::SerialObject : Unknown type");
            }

            const ConstElementPointer element = LookupTagE(aTag);
            if(element){
                if(element->IsNull()){
                    return Core::SERIAL_TYPE_N;
                }else if(element->IsBool()){
                    return Core::SERIAL_TYPE_B;
                }else if(element->IsNumber()){
                    return Core::SERIAL_TYPE_D;
                }else if(element->IsString()){
                    return Core::SERIAL_TYPE_S;
                }else{
                    return Core::SERIAL_TYPE_O;
                }
            }

            throw std::runtime_error("Xml::SerialObject : Could not find tag");
        }

        bool ReadB(const Core::SerialTag aTag) const override{
            const ConstAttributePointer attribute = LookupTagA(aTag);
            if(attribute) return attribute->GetBool();

            const ConstElementPointer element = LookupTagE(aTag);
            if(element) return element->GetBool();

            throw std::runtime_error("Xml::SerialObject : Could not find tag");
        }

        char ReadC(const Core::SerialTag aTag) const override{
            return ReadS(aTag)[0];
        }

        uint8_t ReadU8(const Core::SerialTag aTag) const override{
            return static_cast<uint8_t>(ReadD(aTag));
        }

        uint16_t ReadU16(const Core::SerialTag aTag) const override{
            return static_cast<uint16_t>(ReadD(aTag));
        }

        uint32_t ReadU32(const Core::SerialTag aTag) const override{
            return static_cast<uint32_t>(ReadD(aTag));
        }

        uint64_t ReadU64(const Core::SerialTag aTag) const override{
            return static_cast<uint64_t>(ReadD(aTag));
        }

        int8_t ReadI8(const Core::SerialTag aTag) const override{
            return static_cast<int8_t>(ReadD(aTag));
        }

        int16_t ReadI16(const Core::SerialTag aTag) const override{
            return static_cast<int16_t>(ReadD(aTag));
        }

        int32_t ReadI32(const Core::SerialTag aTag) const override{
            return static_cast<int32_t>(ReadD(aTag));
        }

        int64_t ReadI64(const Core::SerialTag aTag) const override{
            return static_cast<int64_t>(ReadD(aTag));
        }

        float ReadF(const Core::SerialTag aTag) const override{
            return static_cast<float>(ReadD(aTag));
        }

        double ReadD(const Core::SerialTag aTag) const override{
            const ConstAttributePointer attribute = LookupTagA(aTag);
            if(attribute) return attribute->GetNumber();

            const ConstElementPointer element = LookupTagE(aTag);
            if(element) return element->GetNumber();

            throw std::runtime_error("Xml::SerialObject : Could not find tag");
        }

        Core::SerialString ReadS(const Core::SerialTag aTag) const override{
            const ConstAttributePointer attribute = LookupTagA(aTag);
            if(attribute) return attribute->GetString();

            const ConstElementPointer element = LookupTagE(aTag);
            if(element) return element->GetString();

            throw std::runtime_error("Xml::SerialObject : Could not find tag");
        }

        Core::SerialArrayPtr ReadA(const Core::SerialTag aTag) override{
            const ElementPointer element = LookupTagE(aTag);
            if(element) return mElement->GetAllocator().SharedAllocate<SerialArray>(element);
            throw std::runtime_error("Xml::SerialObject : Could not find tag");
        }

        Core::SerialObjectPtr ReadO(const Core::SerialTag aTag) override{
            const ElementPointer element = LookupTagE(aTag);
            if(element) return mElement->GetAllocator().SharedAllocate<SerialObject>(element);
            throw std::runtime_error("Xml::SerialObject : Could not find tag");
        }

        Core::ConstSerialArrayPtr ReadA(const Core::SerialTag aTag) const override{
            const ElementPointer element = LookupTagE(aTag);
            if(element) return mElement->GetAllocator().SharedAllocate<SerialArray>(element);
            throw std::runtime_error("Xml::SerialObject : Could not find tag");
        }

        Core::ConstSerialObjectPtr ReadO(const Core::SerialTag aTag) const override{
            const ElementPointer element = LookupTagE(aTag);
            if(element) return mElement->GetAllocator().SharedAllocate<SerialObject>(element);
            throw std::runtime_error("Xml::SerialObject : Could not find tag");
        }

        void WriteN(const Core::SerialTag aTag) override{
            const AttributePointer attribute = LookupTagA(aTag);
            if(attribute){
                attribute->SetNull();
                return;
            }

            const ElementPointer element = LookupTagE(aTag);
            if(element){
                element->SetNull();
                return;
            }

            const AttributePointer attribute2 = (LookupOrCreateTagA(aTag));
            attribute2->SetNull();
        }

        void WriteB(const Core::SerialTag aTag, const bool aValue) override{
            const AttributePointer attribute = LookupTagA(aTag);
            if(attribute){
                attribute->SetBool(aValue);
                return;
            }

            const ElementPointer element = LookupTagE(aTag);
            if(element){
                element->SetBool(aValue);
                return;
            }

            const AttributePointer attribute2 = (LookupOrCreateTagA(aTag));
            attribute2->SetBool(aValue);
        }

        void WriteC(const Core::SerialTag aTag, const char aValue) override{
            WriteS(aTag, Core::SerialString(&aValue, &aValue + 1));
        }

        void WriteU8(const Core::SerialTag aTag, const uint8_t aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteU16(const Core::SerialTag aTag, const uint16_t aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteU32(const Core::SerialTag aTag, const uint32_t aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteU64(const Core::SerialTag aTag, const uint64_t aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteI8(const Core::SerialTag aTag, const int8_t aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteI16(const Core::SerialTag aTag, const int16_t aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteI32(const Core::SerialTag aTag, const int32_t aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteI64(const Core::SerialTag aTag, const int64_t aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteF(const Core::SerialTag aTag, const float aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteD(const Core::SerialTag aTag, const double aValue) override{
            const AttributePointer attribute = LookupTagA(aTag);
            if(attribute){
                attribute->SetNumber(aValue);
                return;
            }

            const ElementPointer element = LookupTagE(aTag);
            if(element){
                element->SetNumber(aValue);
                return;
            }

            const AttributePointer attribute2 = (LookupOrCreateTagA(aTag));
            attribute2->SetNumber(aValue);
        }

        void WriteS(const Core::SerialTag aTag, const Core::SerialString aValue) override{
            const AttributePointer attribute = LookupTagA(aTag);
            if(attribute){
                attribute->SetString(aValue);
                return;
            }

            const ElementPointer element = LookupTagE(aTag);
            if(element){
                element->SetString(aValue);
                return;
            }

            const AttributePointer attribute2 = (LookupOrCreateTagA(aTag));
            attribute2->SetString(aValue);
        }

        void WriteA(const Core::SerialTag aTag, Core::SerialArrayPtr aArray) override{
            Core::Allocator::SharedPointer<SerialArray> ptr = std::static_pointer_cast<SerialArray>(aArray);
            auto it = mElement->FindFirstChild(aTag);
            if(it != mElement->ChildEnd()) mElement->EraseChild(it);
            mElement->AddChild(ptr->mElement);
        }

        void WriteO(const Core::SerialTag aTag, Core::SerialObjectPtr aObject) override{
            Core::Allocator::SharedPointer<SerialObject> ptr = std::static_pointer_cast<SerialObject>(aObject);
            auto it = mElement->FindFirstChild(aTag);
            if(it != mElement->ChildEnd()) mElement->EraseChild(it);
            mElement->AddChild(ptr->mElement);
        }
    };


    Core::SerialObjectPtr SerialArray::ReadO(const Core::SerialIndex aIndex){
        return mElement->GetAllocator().SharedAllocate<SerialObject>(LookupIndex(aIndex));
    }

    Core::ConstSerialObjectPtr SerialArray::ReadO(const Core::SerialIndex aIndex) const{
        return mElement->GetAllocator().SharedAllocate<SerialObject>(LookupIndex(aIndex));
    }

    void SerialArray::WriteO(const Core::SerialIndex aIndex, Core::SerialObjectPtr aObject){
        ElementPointer element = std::static_pointer_cast<SerialObject>(aObject)->mElement;
        element->SetName("Object");
        mElement->AddChild(element);
    }

    class SerialSystem : public Core::SerialSystem{
    private:
        Core::Allocator& mParseAllocator;
        Core::Allocator& mDataAllocator;
        Element::ParseMode mMode;

        void WriteElement(std::ostream& aStream, const ConstElementPointer aElement) const{
            const size_t size = Element::EstimateSerialLength(*aElement, mMode);
            Core::String string(GetParseAllocator());
            for(size_t i = 0; i < size; ++i) string.PushBack('\0');
            Core::String::Iterator it = string.begin();
            Element::Serialise(it, string.end(), it, *aElement, mMode);
            aStream << string;
        }

        ElementPointer ReadElement(std::istream& aStream) const{
            Core::String string(GetParseAllocator());
            aStream >> string;
            Core::String::Iterator it = string.begin();
            return Element::Deserialise(it, string.end(), it, GetParseAllocator(), GetDataAllocator());
        }

    public:
        SerialSystem(Core::Allocator& aParseAllocator, Core::Allocator& aDataAllocator, Element::ParseMode aMode) :
            mParseAllocator(aParseAllocator),
            mDataAllocator(aDataAllocator),
            mMode(aMode)
        {}

        ~SerialSystem(){

        }

        // Inherited from SerialSystem

        Core::SerialArrayPtr CreateA() const override{
            return mParseAllocator.SharedAllocate<SerialArray>(mParseAllocator.SharedAllocate<Element>(mParseAllocator));
        }

        Core::SerialObjectPtr CreateO() const override{
            return mParseAllocator.SharedAllocate<SerialObject>(mParseAllocator.SharedAllocate<Element>(mParseAllocator));
        }

        void WriteA(std::ostream& aStream, const Core::ConstSerialArrayPtr aArray) const override{
            WriteElement(aStream, std::static_pointer_cast<const SerialArray>(aArray)->mElement);
        }

        void WriteO(std::ostream& aStream, const Core::ConstSerialObjectPtr aObject) const override{
            WriteElement(aStream, std::static_pointer_cast<const SerialObject>(aObject)->mElement);
        }

        Core::SerialArrayPtr ReadA(std::istream& aStream) const override{
            return GetParseAllocator().SharedAllocate<SerialArray>(ReadElement(aStream));

        }

        Core::SerialObjectPtr ReadO(std::istream& aStream) const override{
            return GetParseAllocator().SharedAllocate<SerialObject>(ReadElement(aStream));
        }

        Core::Allocator& GetParseAllocator() const override{
            return mParseAllocator;
        };

        Core::Allocator& GetDataAllocator() const override{
            return mDataAllocator;
        };

    };
}}


#endif
