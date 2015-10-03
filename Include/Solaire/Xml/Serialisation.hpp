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
        Core::Allocator::SharedPointer<Element> mValue;

        AttributePointer GetValueAttribute(const Core::SerialIndex aIndex){
            const Core::SerialIndex size = mValue->GetChildCount();
            Core::Allocator& allocator = mValue->GetAllocator();

            if(aIndex < size){
                ElementPointer element = *(mValue->ChildBegin() + 1);
                return *(element->FindAttribute("value"));
            }else{
                for(Core::SerialIndex i = size; i <= aIndex; ++i){
                    ElementPointer element = allocator.SharedAllocate<Element>("value");
                    element->AddAttribute(allocator.SharedAllocate<Attribute>("value"));
                    mValue->AddChild(element);
                }
                return GetValueAttribute(aIndex);
            }
        }

        const AttributePointer GetValueAttribute(const Core::SerialIndex aIndex) const{
            const Core::SerialIndex size = mValue->GetChildCount();
            if(aIndex < size){
                ElementPointer element = *(mValue->ChildBegin() + 1);
                return *(element->FindAttribute("value"));
            }else{
                return AttributePointer();
            }
        }

    public:
        friend SerialObject;
        friend SerialSystem;

        SerialArray(Core::Allocator::SharedPointer<Element> aValue) :
            mValue(aValue)
        {}

        ~SerialArray(){

        }

        // Inherited from SerialArray

        Core::SerialIndex Size() const override{
            return mValue->GetChildCount();
        }

        Core::SerialType TypeOf(const Core::SerialIndex aIndex) const override;

        bool ReadB(const Core::SerialIndex aIndex) const override{
            return GetValueAttribute(aIndex)->GetBool();
        }

        char ReadC(const Core::SerialIndex aIndex) const override{
            return GetValueAttribute(aIndex)->GetString()[0];
        }

        uint8_t ReadU8(const Core::SerialIndex aIndex) const override{
            return static_cast<uint8_t>(GetValueAttribute(aIndex)->GetNumber());
        }

        uint16_t ReadU16(const Core::SerialIndex aIndex) const override{
            return static_cast<uint16_t>(GetValueAttribute(aIndex)->GetNumber());
        }

        uint32_t ReadU32(const Core::SerialIndex aIndex) const override{
            return static_cast<uint32_t>(GetValueAttribute(aIndex)->GetNumber());
        }

        uint64_t ReadU64(const Core::SerialIndex aIndex) const override{
            return static_cast<uint64_t>(GetValueAttribute(aIndex)->GetNumber());
        }

        int8_t ReadI8(const Core::SerialIndex aIndex) const override{
            return static_cast<int8_t>(GetValueAttribute(aIndex)->GetNumber());
        }

        int16_t ReadI16(const Core::SerialIndex aIndex) const override{
            return static_cast<int16_t>(GetValueAttribute(aIndex)->GetNumber());
        }

        int32_t ReadI32(const Core::SerialIndex aIndex) const override{
            return static_cast<int32_t>(GetValueAttribute(aIndex)->GetNumber());
        }

        int64_t ReadI64(const Core::SerialIndex aIndex) const override{
            return static_cast<int64_t>(GetValueAttribute(aIndex)->GetNumber());
        }

        float ReadF(const Core::SerialIndex aIndex) const override{
            return static_cast<float>(GetValueAttribute(aIndex)->GetNumber());
        }

        double ReadD(const Core::SerialIndex aIndex) const override{
            return static_cast<double>(GetValueAttribute(aIndex)->GetNumber());
        }

        Core::SerialString ReadS(const Core::SerialIndex aIndex) const override{
            return GetValueAttribute(aIndex)->GetString();
        }

        Core::SerialArrayPtr ReadA(const Core::SerialIndex aIndex) override{
            return mValue->GetAllocator().SharedAllocate<SerialArray>(*(mValue->ChildBegin() + aIndex));
        }

        Core::SerialObjectPtr ReadO(const Core::SerialIndex aIndex) override;

        Core::ConstSerialArrayPtr ReadA(const Core::SerialIndex aIndex) const override{
            return mValue->GetAllocator().SharedAllocate<SerialArray>(*(mValue->ChildBegin() + aIndex));
        }

        Core::ConstSerialObjectPtr ReadO(const Core::SerialIndex aIndex) const override;

        void WriteN(const Core::SerialIndex aIndex) override{
            GetValueAttribute(aIndex)->SetNull();
        }

        void WriteB(const Core::SerialIndex aIndex, const bool aValue) override{
            GetValueAttribute(aIndex)->SetBool(aValue);
        }

        void WriteC(const Core::SerialIndex aIndex, const char aValue) override{
            GetValueAttribute(aIndex)->SetString(Core::SerialString(&aValue, (&aValue) + 1));
        }

        void WriteU8(const Core::SerialIndex aIndex, const uint8_t aValue) override{
            GetValueAttribute(aIndex)->SetNumber(aValue);
        }

        void WriteU16(const Core::SerialIndex aIndex, const uint16_t aValue) override{
            GetValueAttribute(aIndex)->SetNumber(aValue);
        }

        void WriteU32(const Core::SerialIndex aIndex, const uint32_t aValue) override{
            GetValueAttribute(aIndex)->SetNumber(aValue);
        }

        void WriteU64(const Core::SerialIndex aIndex, const uint64_t aValue) override{
            GetValueAttribute(aIndex)->SetNumber(aValue);
        }

        void WriteI8(const Core::SerialIndex aIndex, const int8_t aValue) override{
            GetValueAttribute(aIndex)->SetNumber(aValue);
        }

        void WriteI16(const Core::SerialIndex aIndex, const int16_t aValue) override{
            GetValueAttribute(aIndex)->SetNumber(aValue);
        }

        void WriteI32(const Core::SerialIndex aIndex, const int32_t aValue) override{
            GetValueAttribute(aIndex)->SetNumber(aValue);
        }

        void WriteI64(const Core::SerialIndex aIndex, const int64_t aValue) override{
            GetValueAttribute(aIndex)->SetNumber(aValue);
        }

        void WriteF(const Core::SerialIndex aIndex, const float aValue) override{
            GetValueAttribute(aIndex)->SetNumber(aValue);
        }

        void WriteD(const Core::SerialIndex aIndex, const double aValue) override{
            GetValueAttribute(aIndex)->SetNumber(aValue);
        }

        void WriteS(const Core::SerialIndex aIndex, const Core::SerialString aValue) override{
            GetValueAttribute(aIndex)->SetString(aValue);
        }

        void WriteA(const Core::SerialIndex aIndex, Core::SerialArrayPtr aArray) override{
            ElementPointer element = std::static_pointer_cast<SerialArray>(aArray)->mValue;
            element->SetName("ArrayElement");
            mValue->AddChild(element);
        }

        void WriteO(const Core::SerialIndex aIndex, Core::SerialObjectPtr aObject) override;
    };

    class SerialObject : public Core::SerialObject{
    private:
        Core::Allocator::SharedPointer<Element> mValue;

        AttributePointer GetValueAttribute(const Core::SerialTag aTag){
            auto it = mValue->FindFirstChild(aTag);
            if(it == mValue->ChildEnd()){
                Core::Allocator& allocator = mValue->GetAllocator();
                ElementPointer element = allocator.SharedAllocate<Element>(aTag);
                element->AddAttribute(allocator.SharedAllocate<Attribute>("value"));
                mValue->AddChild(element);
                return GetValueAttribute(aTag);
            }else{
                const ElementPointer element = *it;
                return *(element->FindAttribute("value"));
            }
        }

        const AttributePointer GetValueAttribute(const Core::SerialTag aTag) const{
            auto it = mValue->FindFirstChild(aTag);
            if(it == mValue->ChildEnd()){
                return AttributePointer();
            }else{
                const ConstElementPointer element = *it;
                return *(element->FindAttribute("value"));
            }
        }

        static bool IsObject(const ConstElementPointer aElement){
            typedef Element::ConstElementIterator Iterator;
            const Iterator begin = aElement->ChildBegin();
            const Iterator end = aElement->ChildEnd();

            for(Iterator i = begin; i != end; ++i){
                for(Iterator j = begin; j != end; ++j){
                    if(i == j) continue;
                    if((**i).GetName() == (**j).GetName()) return false;
                }
            }

            return true;
        }

    public:
        friend SerialArray;
        friend SerialSystem;

        SerialObject(Core::Allocator::SharedPointer<Element> aValue) :
            mValue(aValue)
        {}

        ~SerialObject(){

        }

        // Inherited from SerialArray

        Core::SerialType TypeOf(const Core::SerialTag aTag) const override{
            auto it = mValue->FindFirstChild(aTag);
            if(it == mValue->ChildEnd())  throw std::runtime_error("Xml::SerialObject : Object does not contain value with tag");
            const ConstElementPointer element = *it;

            if(element->IsNull()){
                return Core::SERIAL_TYPE_N;
            }else if(element->IsBool()){
                return Core::SERIAL_TYPE_D;
            }else if(element->IsString()){
                return Core::SERIAL_TYPE_S;
            }else if(SerialObject::IsObject(element)){
                return Core::SERIAL_TYPE_O;
            }else{
                return Core::SERIAL_TYPE_A;
            }
        }

        bool ReadB(const Core::SerialTag aTag) const override{
            return GetValueAttribute(aTag)->GetBool();
        }

        char ReadC(const Core::SerialTag aTag) const override{
            return GetValueAttribute(aTag)->GetString()[0];
        }

        uint8_t ReadU8(const Core::SerialTag aTag) const override{
            return static_cast<uint8_t>(GetValueAttribute(aTag)->GetNumber());
        }

        uint16_t ReadU16(const Core::SerialTag aTag) const override{
            return static_cast<uint16_t>(GetValueAttribute(aTag)->GetNumber());
        }

        uint32_t ReadU32(const Core::SerialTag aTag) const override{
            return static_cast<uint32_t>(GetValueAttribute(aTag)->GetNumber());
        }

        uint64_t ReadU64(const Core::SerialTag aTag) const override{
            return static_cast<uint64_t>(GetValueAttribute(aTag)->GetNumber());
        }

        int8_t ReadI8(const Core::SerialTag aTag) const override{
            return static_cast<int8_t>(GetValueAttribute(aTag)->GetNumber());
        }

        int16_t ReadI16(const Core::SerialTag aTag) const override{
            return static_cast<int16_t>(GetValueAttribute(aTag)->GetNumber());
        }

        int32_t ReadI32(const Core::SerialTag aTag) const override{
            return static_cast<int32_t>(GetValueAttribute(aTag)->GetNumber());
        }

        int64_t ReadI64(const Core::SerialTag aTag) const override{
            return static_cast<int64_t>(GetValueAttribute(aTag)->GetNumber());
        }

        float ReadF(const Core::SerialTag aTag) const override{
            return static_cast<float>(GetValueAttribute(aTag)->GetNumber());
        }

        double ReadD(const Core::SerialTag aTag) const override{
            return static_cast<double>(GetValueAttribute(aTag)->GetNumber());
        }

        Core::SerialString ReadS(const Core::SerialTag aTag) const override{
            return GetValueAttribute(aTag)->GetString();
        }

        Core::SerialArrayPtr ReadA(const Core::SerialTag aTag) override{
            return mValue->GetAllocator().SharedAllocate<SerialArray>(*(mValue->FindFirstChild(aTag)));
        }

        Core::SerialObjectPtr ReadO(const Core::SerialTag aTag) override{
            return mValue->GetAllocator().SharedAllocate<SerialObject>(*(mValue->FindFirstChild(aTag)));
        }

        Core::ConstSerialArrayPtr ReadA(const Core::SerialTag aTag) const override{
            return mValue->GetAllocator().SharedAllocate<SerialArray>(*(mValue->FindFirstChild(aTag)));
        }

        Core::ConstSerialObjectPtr ReadO(const Core::SerialTag aTag) const override{
            return mValue->GetAllocator().SharedAllocate<SerialObject>(*(mValue->FindFirstChild(aTag)));
        }

        void WriteN(const Core::SerialTag aTag) override{
            GetValueAttribute(aTag)->SetNull();
        }

        void WriteB(const Core::SerialTag aTag, const bool aValue) override{
            GetValueAttribute(aTag)->SetBool(aValue);
        }

        void WriteC(const Core::SerialTag aTag, const char aValue) override{
            GetValueAttribute(aTag)->SetString(Core::SerialString(&aValue, (&aValue) + 1));
        }

        void WriteU8(const Core::SerialTag aTag, const uint8_t aValue) override{
            GetValueAttribute(aTag)->SetNumber(aValue);
        }

        void WriteU16(const Core::SerialTag aTag, const uint16_t aValue) override{
            GetValueAttribute(aTag)->SetNumber(aValue);
        }

        void WriteU32(const Core::SerialTag aTag, const uint32_t aValue) override{
            GetValueAttribute(aTag)->SetNumber(aValue);
        }

        void WriteU64(const Core::SerialTag aTag, const uint64_t aValue) override{
            GetValueAttribute(aTag)->SetNumber(aValue);
        }

        void WriteI8(const Core::SerialTag aTag, const int8_t aValue) override{
            GetValueAttribute(aTag)->SetNumber(aValue);
        }

        void WriteI16(const Core::SerialTag aTag, const int16_t aValue) override{
            GetValueAttribute(aTag)->SetNumber(aValue);
        }

        void WriteI32(const Core::SerialTag aTag, const int32_t aValue) override{
            GetValueAttribute(aTag)->SetNumber(aValue);
        }

        void WriteI64(const Core::SerialTag aTag, const int64_t aValue) override{
            GetValueAttribute(aTag)->SetNumber(aValue);
        }

        void WriteF(const Core::SerialTag aTag, const float aValue) override{
            GetValueAttribute(aTag)->SetNumber(aValue);
        }

        void WriteD(const Core::SerialTag aTag, const double aValue) override{
            GetValueAttribute(aTag)->SetNumber(aValue);
        }

        void WriteS(const Core::SerialTag aTag, const Core::SerialString aValue) override{
            GetValueAttribute(aTag)->SetString(aValue);
        }

        void WriteA(const Core::SerialTag aTag, Core::SerialArrayPtr aArray) override{
            auto it = mValue->FindFirstChild(aTag);
            if(it != mValue->ChildEnd()) throw std::runtime_error("Xml::SerialObject : Object already contains value with tag");
            ElementPointer element = std::static_pointer_cast<SerialArray>(aArray)->mValue;
            element->SetName(aTag);
            mValue->AddChild(element);
        }

        void WriteO(const Core::SerialTag aTag, Core::SerialObjectPtr aObject) override{
            auto it = mValue->FindFirstChild(aTag);
            if(it != mValue->ChildEnd()) throw std::runtime_error("Xml::SerialObject : Object already contains value with tag");
            ElementPointer element = std::static_pointer_cast<SerialObject>(aObject)->mValue;
            element->SetName(aTag);
            mValue->AddChild(element);
        }
    };

    Core::SerialType SerialArray::TypeOf(const Core::SerialIndex aIndex) const{
        const ConstElementPointer element = *(mValue->ChildBegin() + aIndex);

        if(element->IsNull()){
            return Core::SERIAL_TYPE_N;
        }else if(element->IsBool()){
            return Core::SERIAL_TYPE_D;
        }else if(element->IsString()){
            return Core::SERIAL_TYPE_S;
        }else if(SerialObject::IsObject(element)){
            return Core::SERIAL_TYPE_O;
        }else{
            return Core::SERIAL_TYPE_A;
        }
    }

    Core::ConstSerialObjectPtr SerialArray::ReadO(const Core::SerialIndex aIndex) const{
        return mValue->GetAllocator().SharedAllocate<SerialObject>(*(mValue->ChildBegin() + aIndex));
    }

    void SerialArray::WriteO(const Core::SerialIndex aIndex, Core::SerialObjectPtr aObject){
        ElementPointer element = std::static_pointer_cast<SerialObject>(aObject)->mValue;
        element->SetName("ArrayElement");
        mValue->AddChild(element);
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

    public:
        SerialSystem(Core::Allocator& aParseAllocator, Core::Allocator& aDataAllocator, Element::ParseMode aMode) :
            mMode(aMode),
            mParseAllocator(aParseAllocator),
            mDataAllocator(aDataAllocator)
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
            //WriteElement(aStream, std::static_pointer_cast<SerialArray>(aArray)->mValue);
        }

        void WriteO(std::ostream& aStream, const Core::ConstSerialObjectPtr aObject) const override{
            //WriteElement(aStream, std::static_pointer_cast<SerialArray>(aObject)->mValue);
        }

        Core::SerialArrayPtr ReadA(std::istream& aStream) const override{
            //ArrayParser parser(mParseAllocator);
            //aStream >> parser;
            //Core::Allocator::SharedPointer<Value> valuePtr = parser.Get(mParseAllocator, mParseAllocator);
            //return mParseAllocator.SharedAllocate<SerialArray>(valuePtr);

        }

        Core::SerialObjectPtr ReadO(std::istream& aStream) const override{
            //ObjectParser parser(mParseAllocator);
            //aStream >> parser;
            //Core::Allocator::SharedPointer<Value> valuePtr = parser.Get(mParseAllocator, mParseAllocator);
            //return mParseAllocator.SharedAllocate<SerialObject>(valuePtr);
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
