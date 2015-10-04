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
#include "..\Core\Streams\StreamIterator.hpp"

namespace Solaire{ namespace Xml{

    class XmlSerialSystem;
    class XmlSerialObject;

    class XmlSerialArray : public SerialArray{
    private:
        Allocator::SharedPointer<Element> mElement;

        ElementPointer LookupOrCreateIndex(const SerialIndex aIndex){
            const SerialIndex size = mElement->GetChildCount();

            if(aIndex < size){
                return *(mElement->ChildBegin() + 1);
            }else{
                Allocator& allocator = mElement->GetAllocator();
                for(SerialIndex i = size; i <= aIndex; ++i){
                    ElementPointer element = allocator.SharedAllocate<Element>(String("Null", allocator));
                    mElement->AddChild(element);
                }
                return *(mElement->ChildBegin() + 1);
            }
        }

        ElementPointer LookupIndex(const SerialIndex aIndex) const{
            const SerialIndex size = mElement->GetChildCount();
            if(aIndex < size){
                return *(mElement->ChildBegin() + 1);
            }else{
                return ElementPointer();
            }
        }

    public:
        friend XmlSerialObject;
        friend XmlSerialSystem;

        XmlSerialArray(ElementPointer aElement) :
            mElement(aElement)
        {}

        ~XmlSerialArray(){

        }

        // Inherited from SerialArray

        SerialIndex Size() const override{
            return mElement->GetChildCount();
        }

        SerialType TypeOf(const SerialIndex aIndex) const override{
            const ElementPointer element = LookupIndex(aIndex);
            const ConstStringFragment name = element->GetName();

            if(name == "Null" || element->IsNull()){
                return SERIAL_TYPE_N;
            }else if(name == "Bool" || element->IsBool()){
                return SERIAL_TYPE_B;
            }else if(name == "Number" || element->IsNumber()){
                return SERIAL_TYPE_D;
            }else if(name == "String" || element->IsString()){
                return SERIAL_TYPE_S;
            }else if(name == "Array"){
                return SERIAL_TYPE_A;
            }else{
                return SERIAL_TYPE_O;
            }
        }
    protected:
        // Inherited from SerialArray

        bool ReadB(const SerialIndex aIndex) const override{
            return LookupIndex(aIndex)->GetBool();
        }

        char ReadC(const SerialIndex aIndex) const override{
            return LookupIndex(aIndex)->GetString()[0];
        }

        uint8_t ReadU8(const SerialIndex aIndex) const override{
            return static_cast<uint8_t>(LookupIndex(aIndex)->GetNumber());
        }

        uint16_t ReadU16(const SerialIndex aIndex) const override{
            return static_cast<uint16_t>(LookupIndex(aIndex)->GetNumber());
        }

        uint32_t ReadU32(const SerialIndex aIndex) const override{
            return static_cast<uint32_t>(LookupIndex(aIndex)->GetNumber());
        }

        uint64_t ReadU64(const SerialIndex aIndex) const override{
            return static_cast<uint64_t>(LookupIndex(aIndex)->GetNumber());
        }

        int8_t ReadI8(const SerialIndex aIndex) const override{
            return static_cast<int8_t>(LookupIndex(aIndex)->GetNumber());
        }

        int16_t ReadI16(const SerialIndex aIndex) const override{
            return static_cast<int16_t>(LookupIndex(aIndex)->GetNumber());
        }

        int32_t ReadI32(const SerialIndex aIndex) const override{
            return static_cast<int32_t>(LookupIndex(aIndex)->GetNumber());
        }

        int64_t ReadI64(const SerialIndex aIndex) const override{
            return static_cast<int64_t>(LookupIndex(aIndex)->GetNumber());
        }

        float ReadF(const SerialIndex aIndex) const override{
            return static_cast<float>(LookupIndex(aIndex)->GetNumber());
        }

        double ReadD(const SerialIndex aIndex) const override{
            return static_cast<double>(LookupIndex(aIndex)->GetNumber());
        }

        SerialString ReadS(const SerialIndex aIndex) const override{
            return LookupIndex(aIndex)->GetString();
        }

        SerialArrayPtr ReadA(const SerialIndex aIndex) override{
            return mElement->GetAllocator().SharedAllocate<XmlSerialArray>(LookupIndex(aIndex));
        }

        SerialObjectPtr ReadO(const SerialIndex aIndex) override;

        ConstSerialArrayPtr ReadA(const SerialIndex aIndex) const override{
            return mElement->GetAllocator().SharedAllocate<XmlSerialArray>(LookupIndex(aIndex));
        }

        ConstSerialObjectPtr ReadO(const SerialIndex aIndex) const override;

        void WriteN(const SerialIndex aIndex) override{
            ElementPointer element = LookupOrCreateIndex(aIndex);
            element->SetNull();
            element->SetName("Null");
        }

        void WriteB(const SerialIndex aIndex, const bool aValue) override{
            ElementPointer element = LookupOrCreateIndex(aIndex);
            element->SetBool(aValue);
            element->SetName("Bool");
        }

        void WriteC(const SerialIndex aIndex, const char aValue) override{
            WriteS(aIndex, SerialString(&aValue, (&aValue) + 1));
        }

        void WriteU8(const SerialIndex aIndex, const uint8_t aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteU16(const SerialIndex aIndex, const uint16_t aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteU32(const SerialIndex aIndex, const uint32_t aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteU64(const SerialIndex aIndex, const uint64_t aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteI8(const SerialIndex aIndex, const int8_t aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteI16(const SerialIndex aIndex, const int16_t aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteI32(const SerialIndex aIndex, const int32_t aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteI64(const SerialIndex aIndex, const int64_t aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteF(const SerialIndex aIndex, const float aValue) override{
            WriteD(aIndex, aValue);
        }

        void WriteD(const SerialIndex aIndex, const double aValue) override{
            ElementPointer element = LookupOrCreateIndex(aIndex);
            element->SetNumber(aValue);
            element->SetName("Number");
        }

        void WriteS(const SerialIndex aIndex, const SerialString aValue) override{
            ElementPointer element = LookupOrCreateIndex(aIndex);
            element->SetString(aValue);
            element->SetName("String");
        }

        void WriteA(const SerialIndex aIndex, SerialArrayPtr aArray) override{
            ElementPointer element = std::static_pointer_cast<XmlSerialArray>(aArray)->mElement;
            element->SetName("Array");
            mElement->AddChild(element);
        }

        void WriteO(const SerialIndex aIndex, SerialObjectPtr aObject) override;
    };

    class XmlSerialObject : public SerialObject{
    private:
        ElementPointer mElement;

        ElementPointer LookupOrCreateTagE(const SerialTag aTag){
            Element::ConstElementIterator it = mElement->FindFirstChild(aTag);

            if(it != mElement->ChildEnd()){
                return *it;
            }else{
                Allocator& allocator = mElement->GetAllocator();
                ElementPointer element = allocator.SharedAllocate<Element>(String(aTag, allocator));
                element->AddAttribute(allocator.SharedAllocate<Attribute>(String("type", allocator)));
                mElement->AddChild(element);
                return element;
            }
        }

        AttributePointer LookupOrCreateTagA(const SerialTag aTag){
            Element::AttributeIterator it = mElement->FindAttribute(aTag);

            if(it != mElement->AttributeEnd()){
                return *it;
            }else{
                Allocator& allocator = mElement->GetAllocator();
                AttributePointer attribute = allocator.SharedAllocate<Attribute>(String(aTag, allocator));
                mElement->AddAttribute(attribute);
                return attribute;
            }
        }

        ElementPointer LookupTagE(const SerialTag aTag) const{
            Element::ConstElementIterator it = mElement->FindFirstChild(aTag);
            if(it == mElement->ChildEnd()) return ElementPointer();
            return *it;
        }

        AttributePointer LookupTagA(const SerialTag aTag) const{
            Element::AttributeIterator it = mElement->FindAttribute(aTag);
            if(it == mElement->AttributeEnd()) return AttributePointer();
            return *it;
        }

    public:
        friend XmlSerialArray;
        friend XmlSerialSystem;

        XmlSerialObject(ElementPointer aElement) :
            mElement(aElement)
        {}

        ~XmlSerialObject(){

        }
        // Inherited from SerialObject

        SerialType TypeOf(const SerialTag aTag) const override{
            const ConstAttributePointer attribute = LookupTagA(aTag);
            if(attribute){
                if(attribute->IsNull()){
                    return SERIAL_TYPE_N;
                }else if(attribute->IsBool()){
                    return SERIAL_TYPE_B;
                }else if(attribute->IsNumber()){
                    return SERIAL_TYPE_D;
                }else if(attribute->IsString()){
                    return SERIAL_TYPE_S;
                }
                throw std::runtime_error("Xml::SerialObject : Unknown type");
            }

            const ConstElementPointer element = LookupTagE(aTag);
            if(element){
                if(element->IsNull()){
                    return SERIAL_TYPE_N;
                }else if(element->IsBool()){
                    return SERIAL_TYPE_B;
                }else if(element->IsNumber()){
                    return SERIAL_TYPE_D;
                }else if(element->IsString()){
                    return SERIAL_TYPE_S;
                }else{
                    return SERIAL_TYPE_O;
                }
            }

            throw std::runtime_error("Xml::SerialObject : Could not find tag");
        }
    protected:
        // Inherited from SerialArray

        bool ReadB(const SerialTag aTag) const override{
            const ConstAttributePointer attribute = LookupTagA(aTag);
            if(attribute) return attribute->GetBool();

            const ConstElementPointer element = LookupTagE(aTag);
            if(element) return element->GetBool();

            throw std::runtime_error("Xml::SerialObject : Could not find tag");
        }

        char ReadC(const SerialTag aTag) const override{
            return ReadS(aTag)[0];
        }

        uint8_t ReadU8(const SerialTag aTag) const override{
            return static_cast<uint8_t>(ReadD(aTag));
        }

        uint16_t ReadU16(const SerialTag aTag) const override{
            return static_cast<uint16_t>(ReadD(aTag));
        }

        uint32_t ReadU32(const SerialTag aTag) const override{
            return static_cast<uint32_t>(ReadD(aTag));
        }

        uint64_t ReadU64(const SerialTag aTag) const override{
            return static_cast<uint64_t>(ReadD(aTag));
        }

        int8_t ReadI8(const SerialTag aTag) const override{
            return static_cast<int8_t>(ReadD(aTag));
        }

        int16_t ReadI16(const SerialTag aTag) const override{
            return static_cast<int16_t>(ReadD(aTag));
        }

        int32_t ReadI32(const SerialTag aTag) const override{
            return static_cast<int32_t>(ReadD(aTag));
        }

        int64_t ReadI64(const SerialTag aTag) const override{
            return static_cast<int64_t>(ReadD(aTag));
        }

        float ReadF(const SerialTag aTag) const override{
            return static_cast<float>(ReadD(aTag));
        }

        double ReadD(const SerialTag aTag) const override{
            const ConstAttributePointer attribute = LookupTagA(aTag);
            if(attribute) return attribute->GetNumber();

            const ConstElementPointer element = LookupTagE(aTag);
            if(element) return element->GetNumber();

            throw std::runtime_error("Xml::SerialObject : Could not find tag");
        }

        SerialString ReadS(const SerialTag aTag) const override{
            const ConstAttributePointer attribute = LookupTagA(aTag);
            if(attribute) return attribute->GetString();

            const ConstElementPointer element = LookupTagE(aTag);
            if(element) return element->GetString();

            throw std::runtime_error("Xml::SerialObject : Could not find tag");
        }

        SerialArrayPtr ReadA(const SerialTag aTag) override{
            const ElementPointer element = LookupTagE(aTag);
            if(element) return mElement->GetAllocator().SharedAllocate<XmlSerialArray>(element);
            throw std::runtime_error("Xml::SerialObject : Could not find tag");
        }

        SerialObjectPtr ReadO(const SerialTag aTag) override{
            const ElementPointer element = LookupTagE(aTag);
            if(element) return mElement->GetAllocator().SharedAllocate<XmlSerialObject>(element);
            throw std::runtime_error("Xml::SerialObject : Could not find tag");
        }

        ConstSerialArrayPtr ReadA(const SerialTag aTag) const override{
            const ElementPointer element = LookupTagE(aTag);
            if(element) return mElement->GetAllocator().SharedAllocate<XmlSerialArray>(element);
            throw std::runtime_error("Xml::SerialObject : Could not find tag");
        }

        ConstSerialObjectPtr ReadO(const SerialTag aTag) const override{
            const ElementPointer element = LookupTagE(aTag);
            if(element) return mElement->GetAllocator().SharedAllocate<XmlSerialObject>(element);
            throw std::runtime_error("Xml::SerialObject : Could not find tag");
        }

        void WriteN(const SerialTag aTag) override{
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

        void WriteB(const SerialTag aTag, const bool aValue) override{
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

        void WriteC(const SerialTag aTag, const char aValue) override{
            WriteS(aTag, SerialString(&aValue, &aValue + 1));
        }

        void WriteU8(const SerialTag aTag, const uint8_t aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteU16(const SerialTag aTag, const uint16_t aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteU32(const SerialTag aTag, const uint32_t aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteU64(const SerialTag aTag, const uint64_t aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteI8(const SerialTag aTag, const int8_t aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteI16(const SerialTag aTag, const int16_t aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteI32(const SerialTag aTag, const int32_t aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteI64(const SerialTag aTag, const int64_t aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteF(const SerialTag aTag, const float aValue) override{
            WriteD(aTag, static_cast<double>(aValue));
        }

        void WriteD(const SerialTag aTag, const double aValue) override{
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

        void WriteS(const SerialTag aTag, const SerialString aValue) override{
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

        void WriteA(const SerialTag aTag, SerialArrayPtr aArray) override{
            Allocator::SharedPointer<XmlSerialArray> ptr = std::static_pointer_cast<XmlSerialArray>(aArray);
            auto it = mElement->FindFirstChild(aTag);
            if(it != mElement->ChildEnd()) mElement->EraseChild(it);
            mElement->AddChild(ptr->mElement);
        }

        void WriteO(const SerialTag aTag, SerialObjectPtr aObject) override{
            Allocator::SharedPointer<XmlSerialObject> ptr = std::static_pointer_cast<XmlSerialObject>(aObject);
            auto it = mElement->FindFirstChild(aTag);
            if(it != mElement->ChildEnd()) mElement->EraseChild(it);
            mElement->AddChild(ptr->mElement);
        }
    };


    SerialObjectPtr XmlSerialArray::ReadO(const SerialIndex aIndex){
        return mElement->GetAllocator().SharedAllocate<XmlSerialObject>(LookupIndex(aIndex));
    }

    ConstSerialObjectPtr XmlSerialArray::ReadO(const SerialIndex aIndex) const{
        return mElement->GetAllocator().SharedAllocate<XmlSerialObject>(LookupIndex(aIndex));
    }

    void XmlSerialArray::WriteO(const SerialIndex aIndex, SerialObjectPtr aObject){
        ElementPointer element = std::static_pointer_cast<XmlSerialObject>(aObject)->mElement;
        element->SetName("Object");
        mElement->AddChild(element);
    }

    class XmlSerialSystem : public SerialSystem{
    private:
        Allocator& mParseAllocator;
        Allocator& mDataAllocator;
        Element::ParseMode mMode;

        void WriteElement(std::ostream& aStream, const ConstElementPointer aElement) const{
            const size_t size = Element::EstimateSerialLength(*aElement, mMode);
            String string(GetParseAllocator());
            for(size_t i = 0; i < size; ++i) string.PushBack('\0');
            String::Iterator it = string.begin();
            Element::Serialise(it, string.end(), it, *aElement, mMode);
            aStream << string;
        }

        ElementPointer ReadElement(std::istream& aStream) const{
            IStreamIterator begin(aStream);
            IStreamIterator end;
            return Element::Deserialise<IStreamIterator>(begin, end, begin, GetParseAllocator(), GetDataAllocator());
        }

    public:
        XmlSerialSystem(Allocator& aParseAllocator, Allocator& aDataAllocator, Element::ParseMode aMode) :
            mParseAllocator(aParseAllocator),
            mDataAllocator(aDataAllocator),
            mMode(aMode)
        {}

        ~XmlSerialSystem(){

        }

        // Inherited from SerialSystem

        SerialArrayPtr CreateA() const override{
            return mParseAllocator.SharedAllocate<XmlSerialArray>(mParseAllocator.SharedAllocate<Element>(mParseAllocator));
        }

        SerialObjectPtr CreateO() const override{
            return mParseAllocator.SharedAllocate<XmlSerialObject>(mParseAllocator.SharedAllocate<Element>(mParseAllocator));
        }

        void WriteA(std::ostream& aStream, const ConstSerialArrayPtr aArray) const override{
            WriteElement(aStream, std::static_pointer_cast<const XmlSerialArray>(aArray)->mElement);
        }

        void WriteO(std::ostream& aStream, const ConstSerialObjectPtr aObject) const override{
            WriteElement(aStream, std::static_pointer_cast<const XmlSerialObject>(aObject)->mElement);
        }

        SerialArrayPtr ReadA(std::istream& aStream) const override{
            return GetParseAllocator().SharedAllocate<XmlSerialArray>(ReadElement(aStream));

        }

        SerialObjectPtr ReadO(std::istream& aStream) const override{
            return GetParseAllocator().SharedAllocate<XmlSerialObject>(ReadElement(aStream));
        }

        Allocator& GetParseAllocator() const override{
            return mParseAllocator;
        };

        Allocator& GetDataAllocator() const override{
            return mDataAllocator;
        };

    };
}}


#endif
