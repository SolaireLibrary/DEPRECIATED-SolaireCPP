#ifndef SOLAIRE_XML_ELEMENT_HPP
#define SOLAIRE_XML_ELEMENT_HPP

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
\file Element.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 29th September 2015
Last Modified	: 29th September 2015
*/

#include "..\Core\Strings\String.hpp"
#include "..\Core\Strings\NumberParser.hpp"
#include "..\Core\Iterators\DereferenceIterator.hpp"

namespace Solaire{ namespace Core{

    namespace SerialHelper{

        static void DeserialiseXmlGlyphs(Core::String& aString){
            aString.ReplaceAll("&lt;", "<");
            aString.ReplaceAll("&gt;", ">");
            aString.ReplaceAll("&amp;", "&");
            aString.ReplaceAll("&apos;", "'");
            aString.ReplaceAll("&quot;", "\"");
        }

        static void SerialiseXmlGlyphs(Core::String& aString){
            aString.ReplaceAll("<", "&lt;");
            aString.ReplaceAll(">", "&gt;");
            aString.ReplaceAll("&", "&amp;");
            aString.ReplaceAll("'", "&apos;");
            aString.ReplaceAll("\"", "&quot;");
        }
    }

}}

namespace Solaire{ namespace Xml{

    class Attribute;
    class Element;

    typedef Core::Allocator::SharedPointer<Element> ElementPointer;
    typedef Core::Allocator::SharedPointer<const Element> ConstElementPointer;

    typedef Core::Allocator::SharedPointer<Attribute> AttributePointer;
    typedef Core::Allocator::SharedPointer<const Attribute> ConstAttributePointer;

    class Attribute : public std::enable_shared_from_this<Attribute>{
    private:
        Attribute(const Attribute&) = delete;
        Attribute(Attribute&&) = delete;
        Attribute& operator=(const Attribute&) = delete;
        Attribute& operator=(Attribute&&) = delete;

        enum Type : uint8_t{
            TYPE_STRING,
            TYPE_NUMBER,
            TYPE_BOOL,
            TYPE_NULL
        };

        Core::String mName;
        Type mType;
        union{
            Core::String* mString;
            double mNumber;
            bool mBool;
        };
    public:
        static size_t EstimateSerialLength(const Attribute& aAttribute){
            size_t count = 3;
            count += aAttribute.mName.Size();
            switch(aAttribute.mType){
            case TYPE_STRING:
                count += aAttribute.mString->Size() * 2;
                break;
            case TYPE_NUMBER:
                count += 16;
                break;
            case TYPE_BOOL:
                count += 5;
                break;
            default:
                break;
            }
            return count;
        }

        template<class Iterator>
        static bool Serialise(const Iterator aBegin, const Iterator aEnd, Iterator& aParseEnd, const Attribute& aAttribute){
            aParseEnd = aBegin;
            for(const char c : aAttribute.mName){
                *aParseEnd = c;
                ++aParseEnd;
            }
            *aParseEnd = '=';
            ++aParseEnd;
            *aParseEnd = '"';
            ++aParseEnd;
            switch(aAttribute.mType){
            case TYPE_STRING:
                {
                    Core::String value = *aAttribute.mString;
                    Core::SerialHelper::SerialiseXmlGlyphs(value);
                    aParseEnd = Core::SerialHelper::CopyString(aParseEnd, value);
                }
                break;
            case TYPE_NUMBER:
                aParseEnd = Core::SerialHelper::CopyNumber(aParseEnd, aAttribute.mNumber);
                break;
            case TYPE_BOOL:
                aParseEnd = Core::SerialHelper::CopyBool(aParseEnd, aAttribute.mBool);
                break;
            default:
                break;
            }
            *aParseEnd = '"';
            ++aParseEnd;
            return true;
        }

        template<class Iterator>
        static AttributePointer Deserialise(const Iterator aBegin, const Iterator aEnd, Iterator& aParseEnd, Core::Allocator& aParseAllocator, Core::Allocator& aDataAllocator){
            Iterator nameBegin = aParseEnd;
            Iterator nameEnd = aParseEnd;
            Iterator valueBegin = aParseEnd;
            Iterator valueEnd = aParseEnd;

            aParseEnd = aBegin;
            goto STATE_FIND_NAME;

            STATE_FIND_NAME:
            {
                switch(*aParseEnd){
                case ' ':
                case '\t':
                case '\n':
                    ++aParseEnd;
                    goto STATE_FIND_NAME;
                default:
                    nameBegin = aParseEnd;
                    goto STATE_PARSE_NAME;
                }
            }

            STATE_PARSE_NAME:
            {
                switch(*aParseEnd){
                case '=':
                    nameEnd = aParseEnd;
                    if((aParseEnd - nameBegin) == 0) goto STATE_FAIL;
                    ++aParseEnd;
                    goto STATE_OPEN_VALUE;
                default:
                    ++aParseEnd;
                    goto STATE_PARSE_NAME;
                }
            }

            STATE_OPEN_VALUE:
            {
                switch(*aParseEnd){
                case '"':
                    ++aParseEnd;
                    valueBegin = aParseEnd;
                    goto STATE_PARSE_VALUE;
                default:
                    goto STATE_FAIL;
                }
            }

            STATE_PARSE_VALUE:
            {
                switch(*aParseEnd){
                case '"':
                    valueEnd = aParseEnd;
                    goto STATE_SUCCESS;
                default:
                    ++aParseEnd;
                    goto STATE_PARSE_VALUE;
                }
            }

            STATE_SUCCESS:
            {
                Core::String name(nameBegin, nameEnd, aDataAllocator);
                if((valueEnd - valueBegin) == 0){
                    return aDataAllocator.SharedAllocate<Attribute>(name);
                }else{
                    Core::String value(valueBegin, valueEnd, aDataAllocator);
                    Core::SerialHelper::DeserialiseXmlGlyphs(value);
                    return aDataAllocator.SharedAllocate<Attribute>(name, value);
                }
            }

            STATE_FAIL:
            aParseEnd = aBegin;
            return AttributePointer();
        }

        Attribute(Core::String aName):
            mName(aName),
            mType(TYPE_NULL)
        {}

        Attribute(Core::String aName, const bool aValue):
            mName(aName),
            mType(TYPE_BOOL),
            mBool(aValue)
        {}

        Attribute(Core::String aName, const double aValue):
            mName(aName),
            mType(TYPE_NUMBER),
            mNumber(aValue)
        {}

        Attribute(Core::String aName, const Core::ConstStringFragment aValue):
            mName(aName),
            mType(TYPE_NULL)
        {
            SetString(aValue);
        }

        ~Attribute(){
            SetNull();
        }

        Core::Allocator& GetAllocator() const{
            return mName.GetAllocator();
        }

        bool IsNull() const{
            return mType == TYPE_NULL;
        }

        bool IsBool() const{
            return mType == TYPE_BOOL;
        }

        bool IsNumber() const{
            return mType == TYPE_NUMBER;
        }

        bool IsString() const{
            return mType == TYPE_STRING;
        }

        double GetNumber() const{
            switch(mType){
            case TYPE_BOOL:
                return mBool ? 1.0 : 0.0;
            case TYPE_NUMBER:
                return mNumber;
            default:
                throw std::runtime_error("Xml::Attribute : Value is not a number");
            }
        }

        bool GetBool() const{
            switch(mType){
            case TYPE_BOOL:
                return mBool;
            case TYPE_NUMBER:
                return mNumber >= 1.0;
            default:
                throw std::runtime_error("Xml::Attribute : Value is not a bool");
            }
        }

        Core::ConstStringFragment GetString() const{
            switch(mType){
            case TYPE_STRING:
                return *mString;
            default:
                throw std::runtime_error("Xml::Attribute : Value is not a string");
            }
        }

        void SetNull(){
            if(mType == TYPE_STRING){
                GetAllocator().Deallocate<Core::String>(mString);
            }
            mType = TYPE_NULL;
        }

        void SetBool(const bool aValue){
            SetNull();
            mType = TYPE_BOOL;
            mBool = aValue;
        }

        void SetNumber(const double aValue){
            SetNull();
            mType = TYPE_NUMBER;
            mNumber = aValue;
        }

        void SetString(const Core::ConstStringFragment aValue){
            //! \TODO Check if bool
            //! \TODO Check if number

            if(mType == TYPE_STRING){
                *mString = aValue;
            }else{
                mString = new(GetAllocator().AllocateAndRegister<Core::String>()) Core::String(aValue);
                mType = TYPE_STRING;
            }
        }

        Core::ConstStringFragment GetName() const{
            return mName;
        }
    };

    class Element : public std::enable_shared_from_this<Element>{
    public:
        typedef Core::DynamicArray<ElementPointer>::Iterator ElementIterator;
        typedef Core::DynamicArray<ElementPointer>::ConstIterator ConstElementIterator;
        typedef Core::DynamicArray<ElementPointer>::ReverseIterator ReverseElementIterator;
        typedef Core::DynamicArray<ElementPointer>::ConstReverseIterator ConstReverseElementIterator;

        typedef Core::DynamicArray<AttributePointer>::Iterator AttributeIterator;
        typedef Core::DynamicArray<AttributePointer>::ConstIterator ConstAttributeIterator;
        typedef Core::DynamicArray<AttributePointer>::ReverseIterator ReverseAttributeIterator;
        typedef Core::DynamicArray<AttributePointer>::ConstReverseIterator ConstReverseAttributeIterator;
    private:
        Element(const Element&) = delete;
        Element(Element&&) = delete;
        Element& operator=(const Element&) = delete;
        Element& operator=(Element&&) = delete;

        enum Type : uint8_t{
            TYPE_EMPTY,
            TYPE_CHILDREN,
            TYPE_BODY
        };

        Core::String mName;
        Core::DynamicArray<AttributePointer> mAttributes;
        union{
            Core::DynamicArray<ElementPointer>* mChildren;
            Attribute* mValue;
        };
        Type mType;
    public:
        static size_t EstimateSerialLength(const Element& aElement){
            size_t count = 3;
            count += aElement.mName.Size();
            for(const ConstAttributePointer i : aElement.mAttributes){
               count += Attribute::EstimateSerialLength(*i);
            }

            switch(aElement.mType){
            case TYPE_EMPTY:
                count += 3;
                return count;
            case TYPE_CHILDREN:
                for(const ConstElementPointer i : *aElement.mChildren){
                    count += Element::EstimateSerialLength(*i);
                }
                break;
            case TYPE_BODY:
                if(aElement.mValue->IsBool()){
                    count += 5;
                }else if(aElement.mValue->IsNumber()){
                    count += 16;
                }else if(aElement.mValue->IsString()){
                    count += aElement.mValue->GetString().Size() * 2;
                }
                break;
            default:
                break;
            }
            return count + 2;
        }

        struct ParseMode{
            uint16_t depth;
            struct{
                uint8_t newLines : 1;
                uint8_t indent : 1;
            };

            ParseMode():
                depth(0),
                newLines(0),
                indent(0)
            {}

            ParseMode(const bool aNewLines, const bool aIndent):
                depth(0),
                newLines(aNewLines),
                indent(aIndent)
            {}
        };

        template<class Iterator>
        static bool Serialise(const Iterator aBegin, const Iterator aEnd, Iterator& aParseEnd, const Element& aElement, ParseMode aMode){
            aParseEnd = aBegin;
            if(aMode.newLines){
                *aParseEnd = '\n';
                ++aParseEnd;
                if(aMode.indent){
                    for(uint16_t i = 0; i < aMode.depth; ++i){
                        *aParseEnd = '\t';
                        ++aParseEnd;
                    }
                }
            }
            *aParseEnd = '<';
            ++aParseEnd;
            aParseEnd = Core::SerialHelper::CopyString(aParseEnd, aElement.mName);
            for(const ConstAttributePointer i : aElement.mAttributes){
                *aParseEnd = ' ';
                ++aParseEnd;
                Attribute::Serialise(aParseEnd, aEnd, aParseEnd, *i);
            }

            switch(aElement.mType){
            case TYPE_EMPTY:
                *aParseEnd = '/';
                ++aParseEnd;
                *aParseEnd = '>';
                ++aParseEnd;
                return true;
            case TYPE_CHILDREN:
                *aParseEnd = '>';
                ++aParseEnd;
                ++aMode.depth;
                for(const ConstElementPointer i : *aElement.mChildren){
                    Element::Serialise(aParseEnd, aEnd, aParseEnd, *i, aMode);
                }
                --aMode.depth;
                break;
            case TYPE_BODY:
                *aParseEnd = '>';
                ++aParseEnd;
                if(aElement.mValue->IsBool()){
                    aParseEnd = Core::SerialHelper::CopyBool(aParseEnd, aElement.mValue->GetBool());
                }else if(aElement.mValue->IsNumber()){
                    aParseEnd = Core::SerialHelper::CopyNumber(aParseEnd, aElement.mValue->GetNumber());
                }else if(aElement.mValue->IsString()){
                    Core::String value = aElement.mValue->GetString();
                    Core::SerialHelper::SerialiseXmlGlyphs(value);
                    aParseEnd = Core::SerialHelper::CopyString(aParseEnd, value);
                }else{
                    aParseEnd = aBegin;
                    return false;
                }
                break;
            default:
                aParseEnd = aBegin;
                return false;
            }
            if(aMode.newLines){
                *aParseEnd = '\n';
                ++aParseEnd;
                if(aMode.indent){
                    for(uint16_t i = 0; i < aMode.depth; ++i){
                        *aParseEnd = '\t';
                        ++aParseEnd;
                    }
                }
            }
            *aParseEnd = '<';
            ++aParseEnd;
            *aParseEnd = '/';
            ++aParseEnd;
            aParseEnd = Core::SerialHelper::CopyString(aParseEnd, aElement.mName);
            *aParseEnd = '>';
            ++aParseEnd;
            return true;
        }

        template<class Iterator>
        static ElementPointer Deserialise(const Iterator aBegin, const Iterator aEnd, Iterator& aParseEnd, Core::Allocator& aParseAllocator, Core::Allocator& aDataAllocator){
            Iterator nameBegin = aEnd;
            Iterator nameEnd = aEnd;
            Iterator valueBegin = aEnd;
            Iterator valueEnd = aEnd;
            bool hasValue = false;
            Core::DynamicArray<AttributePointer> attributes(8, aParseAllocator);
            Core::DynamicArray<ElementPointer> children(8, aParseAllocator);

            aParseEnd = aBegin;
            goto STATE_OPEN_START_TAG;

            STATE_OPEN_START_TAG:
            {
                switch(*aParseEnd){
                case ' ':
                case '\t':
                case '\n':
                    ++aParseEnd;
                    goto STATE_OPEN_START_TAG;
                case '<':
                    ++aParseEnd;
                    goto STATE_OPEN_BEGIN_NAME;
                default:
                    goto STATE_FAIL;
                }
            }

            STATE_OPEN_BEGIN_NAME:
            {
                switch(*aParseEnd){
                case ' ':
                case '\t':
                case '\n':
                    ++aParseEnd;
                    goto STATE_OPEN_BEGIN_NAME;
                default:
                    nameBegin = aParseEnd;
                    goto STATE_PARSE_BEGIN_NAME;
                }
            }

            STATE_PARSE_BEGIN_NAME:
            {
                switch(*aParseEnd){
                case ' ':
                case '\t':
                case '\n':
                    nameEnd = aParseEnd;
                    ++aParseEnd;
                    goto STATE_OPEN_ATTRIBUTE;
                case '>':
                    nameEnd = aParseEnd;
                    ++aParseEnd;
                    goto STATE_PARSE_VALUE_0;
                default:
                    ++aParseEnd;
                    goto STATE_PARSE_BEGIN_NAME;
                }
            }

            STATE_OPEN_ATTRIBUTE:
            {
                switch(*aParseEnd){
                case ' ':
                case '\t':
                case '\n':
                    ++aParseEnd;
                    goto STATE_OPEN_ATTRIBUTE;
                case '/':
                    ++aParseEnd;
                    if(*aParseEnd == '>'){
                        goto STATE_SUCCESS;
                    }else{
                        goto STATE_FAIL;
                    }
                case '>':
                    ++aParseEnd;
                    goto STATE_PARSE_VALUE_0;
                default:
                    goto STATE_PARSE_ATTRIBUTE;
                }
            }

            STATE_PARSE_ATTRIBUTE:
            {
                --aParseEnd;
                AttributePointer ptr = Attribute::Deserialise(aParseEnd, aEnd, aParseEnd, aParseAllocator, aDataAllocator);
                if(ptr){
                    ++aParseEnd;
                    attributes.PushBack(ptr);
                    goto STATE_OPEN_ATTRIBUTE;
                }else{
                    goto STATE_FAIL;
                }
            }

            STATE_PARSE_VALUE_0:
            {
                switch(*aParseEnd){
                case ' ':
                case '\t':
                case '\n':
                    ++aParseEnd;
                    valueBegin = aParseEnd;
                    goto STATE_PARSE_VALUE_0;
                default:
                    goto STATE_PARSE_VALUE_1;
                }
            }

            STATE_PARSE_VALUE_1:
            {
                switch(*aParseEnd){
                case '<':
                    valueEnd = aParseEnd;
                    ++aParseEnd;
                    if(*aParseEnd == '/'){
                        ++aParseEnd;
                        hasValue = (valueBegin != aEnd) && (valueEnd - valueBegin) > 0;
                        goto STATE_PARSE_END_NAME;
                    }else{
                        --aParseEnd;
                        ElementPointer child = Element::Deserialise(aParseEnd, aEnd, aParseEnd, aParseAllocator, aDataAllocator);
                        if(child){
                            children.PushBack(child);
                        ++aParseEnd;
                            goto STATE_PARSE_VALUE_0;
                        }else{
                            goto STATE_FAIL;
                        }
                    }
                default:
                    ++aParseEnd;
                    goto STATE_PARSE_VALUE_0;
                }
            }

            STATE_PARSE_END_NAME:
            {
                //Core::String endName(aParseAllocator);

                STATE_END_NAME_INTERNAL:
                switch(*aParseEnd){
                case ' ':
                case '\t':
                case '\n':
                    ++aParseEnd;
                    goto STATE_END_NAME_INTERNAL;
                case '>':
                    //if(name == endName){
                        goto STATE_SUCCESS;
                    //}else{
                    //    goto STATE_FAIL;
                    //}
                default:
                    //endName += *aParseEnd;
                    ++aParseEnd;
                    goto STATE_END_NAME_INTERNAL;
                }
            }

            STATE_SUCCESS:
            {
                Core::String name(nameBegin, nameEnd, aDataAllocator);
                ElementPointer element = aDataAllocator.SharedAllocate<Element>(name);

                for(AttributePointer i : attributes){
                    element->AddAttribute(i);
                }

                if(hasValue){
                    Core::String value(valueBegin, valueEnd, aDataAllocator);
                    Core::SerialHelper::DeserialiseXmlGlyphs(value);
                    element->SetString(value);
                }

                for(ElementPointer i : children){
                    element->AddChild(i);
                }

                return element;
            }

            STATE_FAIL:
            aParseEnd = aBegin;
            return ElementPointer();
        }

        Element(Core::String aName):
            mName(aName),
            mAttributes(8, aName.GetAllocator()),
            mChildren(nullptr),
            mType(TYPE_EMPTY)
        {}

        ~Element(){
            SetNull();
        }

        // Value

        bool IsNull() const{
            return mType == TYPE_EMPTY;
        }

        bool IsBool() const{
            return mType == TYPE_BODY ? mValue->IsBool() : false;
        }

        bool IsNumber() const{
            return mType == TYPE_BODY ? mValue->IsNumber() : false;
        }

        bool IsString() const{
            return mType == TYPE_BODY ? mValue->IsString() : false;
        }

        void SetNull(){
            switch(mType){
            case TYPE_CHILDREN:
                GetAllocator().Deallocate<Core::DynamicArray<ElementPointer>>(mChildren);
                break;
            case TYPE_BODY:
                GetAllocator().Deallocate<Attribute>(mValue);
                break;
            default:
                break;
            }
            mChildren = nullptr;
            mType = TYPE_EMPTY;
        }

        void SetBool(const bool aValue){
            if(mType != TYPE_BODY){
                SetNull();
                mValue = new(GetAllocator().AllocateAndRegister<Attribute>()) Attribute("", aValue);
            }else{
                mValue->SetBool(aValue);
            }
        }

        void SetNumber(const double aValue){
            if(mType != TYPE_BODY){
                SetNull();
                mValue = new(GetAllocator().AllocateAndRegister<Attribute>()) Attribute("", aValue);
            }else{
                mValue->SetNumber(aValue);
            }
        }

        void SetString(const Core::ConstStringFragment aValue){
            if(aValue.Size() == 0){
                SetNull();
            }else if(mType != TYPE_BODY){
                SetNull();
                mValue = new(GetAllocator().AllocateAndRegister<Attribute>()) Attribute("", aValue);
            }else{
                mValue->SetString(aValue);
            }
        }

        bool GetBool() const{
            if(mType != TYPE_BODY){
               throw std::runtime_error("Xml::Element : Does not have body value");
            }else{
                mValue->GetBool();
            }
        }

        double GetNumber() const{
            if(mType != TYPE_BODY){
               throw std::runtime_error("Xml::Element : Does not have body value");
            }else{
                mValue->GetNumber();
            }
        }

        Core::ConstStringFragment GetString() const{
            if(mType != TYPE_BODY){
               throw std::runtime_error("Xml::Element : Does not have body value");
            }else{
                mValue->GetString();
            }
        }

        // Attribute

        size_t GetAttributeCount() const{
            return mAttributes.Size();
        }

        AttributeIterator AttributeBegin(){return mAttributes.begin();}
        ConstAttributeIterator AttributeBegin() const{return mAttributes.begin();}
        AttributeIterator AttributeEnd(){return mAttributes.end();}
        ConstAttributeIterator AttributeEnd() const{return mAttributes.end();}
        ReverseAttributeIterator AttributeRBegin(){return mAttributes.rbegin();}
        ConstReverseAttributeIterator AttributeRBegin() const{return mAttributes.rbegin();}
        ReverseAttributeIterator AttributeREnd(){return mAttributes.rend();}
        ConstReverseAttributeIterator AttributeREnd() const{return mAttributes.rend();}

        ConstAttributeIterator FindAttribute(const Core::ConstStringFragment aName) const{
            return mAttributes.FindFirst([&](const ConstAttributePointer aPtr){
                return aPtr->GetName() == aName;
            });
        }

        AttributeIterator FindAttribute(const Core::ConstStringFragment aName){
            return mAttributes.FindFirst([&](const ConstAttributePointer aPtr){
                return aPtr->GetName() == aName;
            });
        }

        bool AddAttribute(const AttributePointer aPtr){
            if(mAttributes.FindFirst(aPtr) != AttributeEnd()) return false;
            mAttributes.PushBack(aPtr);
            return true;
        }

        bool EraseAttribute(const Core::ConstStringFragment aName){
            return EraseAttribute(FindAttribute(aName));
        }

        bool EraseAttribute(const AttributePointer aPtr){
            return EraseAttribute(mAttributes.FindFirst(aPtr));
        }

        bool EraseAttribute(const ConstAttributeIterator aPos){
            if(aPos == AttributeEnd()) return false;
            mAttributes.Erase(aPos);
            return true;
        }

        // Element

        size_t GetChildCount() const{
            if(mType != TYPE_CHILDREN) return 0;
            return mChildren->Size();
        }

        ElementIterator ChildBegin(){
            return mType == TYPE_CHILDREN ? mChildren->begin() : ElementIterator();
        }

        ConstElementIterator ChildBegin() const{
            return mType == TYPE_CHILDREN ? mChildren->begin() : ConstElementIterator();
        }

        ElementIterator ChildEnd(){
            return mType == TYPE_CHILDREN ? mChildren->end() : ElementIterator();
        }

        ConstElementIterator ChildEnd() const{
            return mType == TYPE_CHILDREN ? mChildren->end() : ConstElementIterator();
        }

        ReverseElementIterator ChildRBegin(){
            return mType == TYPE_CHILDREN ? mChildren->rbegin() : ReverseElementIterator();
        }

        ConstReverseElementIterator ChildRBegin() const{
            return mType == TYPE_CHILDREN ? mChildren->rbegin() : ConstReverseElementIterator();
        }

        ReverseElementIterator ChildREnd(){
            return mType == TYPE_CHILDREN ? mChildren->rend() : ReverseElementIterator();
        }

        ConstReverseElementIterator ChildREnd() const{
            return mType == TYPE_CHILDREN ? mChildren->rend() : ConstReverseElementIterator();
        }

        ConstElementIterator FindFirstChild(const Core::ConstStringFragment aName) const{
            if(mType != TYPE_CHILDREN) return ConstElementIterator();
            return mChildren->FindFirst([&](const ConstElementPointer aPtr){
                return aPtr->GetName() == aName;
            });
        }

        ConstElementIterator FindNextChild(const ConstElementIterator aPos, const Core::ConstStringFragment aName) const{
            if(mType != TYPE_CHILDREN) return ConstElementIterator();
            return mChildren->FindNext(aPos, [&](const ConstElementPointer aPtr){
                return aPtr->GetName() == aName;
            });
        }

        ConstElementIterator FindLastChildChild(const Core::ConstStringFragment aName) const{
            if(mType != TYPE_CHILDREN) return ConstElementIterator();
            return mChildren->FindFirst([&](const ConstElementPointer aPtr){
                return aPtr->GetName() == aName;
            });
        }

        bool AddChild(const ElementPointer aPtr){
            if(mType != TYPE_CHILDREN){
                SetNull();
                mChildren = new(GetAllocator().AllocateAndRegister<Core::DynamicArray<ElementPointer>>()) Core::DynamicArray<ElementPointer>(8, GetAllocator());
                mType = TYPE_CHILDREN;
            }else if(mChildren->FindFirst(aPtr) != ChildEnd()){
                return false;
            }

            mChildren->PushBack(aPtr);
            return true;
        }

        bool EraseChild(const ElementPointer aPtr){
            if(mType != TYPE_CHILDREN) return false;
            return EraseChild(mChildren->FindFirst(aPtr));
        }

        bool EraseChild(const ConstElementIterator aPos){
            if(mType != TYPE_CHILDREN) return false;
            if(aPos == ChildEnd()) return false;
            mChildren->Erase(aPos);
            return true;
        }

        // Misc

        void SetName(const Core::ConstStringFragment aName){
            mName = aName;
        }

        Core::ConstStringFragment GetName() const{
            return mName;
        }

        Core::Allocator& GetAllocator() const{
            return mAttributes.GetAllocator();
        }
    };

}}

#endif
