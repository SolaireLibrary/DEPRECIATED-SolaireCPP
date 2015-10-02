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

namespace Solaire{ namespace Xml{

    class Attribute;
    class Element;

    typedef Core::Allocator::SharedPointer<Element> ElementPointer;
    typedef Core::Allocator::SharedPointer<const Element> ConstElementPointer;

    typedef Core::Allocator::SharedPointer<Attribute> AttributePointer;
    typedef Core::Allocator::SharedPointer<const Attribute> ConstAttributePointer;

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
        static void Serialise(std::ostream& aStream, const Attribute& aAttribute){
            aStream << aAttribute.mName;
            aStream << '=';
            aStream << '"';
            switch(aAttribute.mType){
            case TYPE_STRING:
                {
                    Core::String value = *aAttribute.mString;
                    SerialiseXmlGlyphs(value);
                    aStream << value;
                }
                break;
            case TYPE_NUMBER:
                aStream << aAttribute.mNumber;
                break;
            case TYPE_BOOL:
                aStream << aAttribute.mBool;
                break;
            default:
                break;
            }
            aStream << '"';
        }

        static AttributePointer Deserialise(std::istream& aStream, Core::Allocator& aParseAllocator, Core::Allocator& aDataAllocator, size_t& aCharsRead){
            Core::String name(aDataAllocator);
            Core::String value(aParseAllocator);
            size_t count = 0;
            char currentChar;

            const auto ReadChar = [&]()->char{
                char c;
                if(aStream.eof()) throw std::runtime_error("Xml::Attribute : End of stream reached unexpectedly");
                aStream >> c;
                ++count;
                return c;
            };

            currentChar = ReadChar();
            goto STATE_FIND_NAME;

            STATE_FIND_NAME:
            {
                switch(currentChar){
                case ' ':
                case '\t':
                case '\n':
                    currentChar = ReadChar();
                    goto STATE_FIND_NAME;
                default:
                    goto STATE_PARSE_NAME;
                }
            }

            STATE_PARSE_NAME:
            {
                switch(currentChar){
                case '=':
                    if(name.Size() == 0) goto STATE_FAIL;
                    currentChar = ReadChar();
                    goto STATE_OPEN_VALUE;
                default:
                    name += currentChar;
                    goto STATE_PARSE_NAME;
                }
            }

            STATE_OPEN_VALUE:
            {
                switch(currentChar){
                case '"':
                    currentChar = ReadChar();
                    goto STATE_PARSE_VALUE;
                default:
                    goto STATE_FAIL;
                }
            }

            STATE_PARSE_VALUE:
            {
                switch(currentChar){
                case '"':
                    goto STATE_SUCCESS;
                default:
                    value += currentChar;
                    currentChar = ReadChar();
                    goto STATE_PARSE_VALUE;
                }
            }

            STATE_SUCCESS:
            aCharsRead += count;
            if(value.Size() == 0){
                return aDataAllocator.SharedAllocate<Attribute>(name);
            }else{
                DeserialiseXmlGlyphs(value);
                return aDataAllocator.SharedAllocate<Attribute>(name, value);
            }

            STATE_FAIL:
            for(size_t i = 0; i < count; ++i) aStream.unget();
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
                new(GetAllocator().AllocateAndRegister<Core::String>()) Core::String(aValue);
                mType = TYPE_NUMBER;
            }
        }

        Core::ConstStringFragment GetName() const{
            return mName;
        }
    };

    class Element : public std::enable_shared_from_this<Attribute>{
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
        static void Serialise(std::ostream& aStream, const Element& aElement){
            aStream << '<';
            aStream << aElement.mName;
            aStream << ' ';
            for(const ConstAttributePointer i : aElement.mAttributes){
                Attribute::Serialise(aStream, *i);
            }
            aStream << ' ';

            switch(aElement.mType){
            case TYPE_EMPTY:
                aStream << ' ';
                aStream << '/';
                aStream << '>';
                return;
            case TYPE_CHILDREN:
                for(const ConstElementPointer i : *aElement.mChildren){
                    Element::Serialise(aStream, *i);
                }
                break;
            case TYPE_BODY:
                if(aElement.mValue->IsBool()){
                    aStream << aElement.mValue->GetBool() ? "true" : "false";
                }else if(aElement.mValue->IsNumber()){
                    aStream << aElement.mValue->GetNumber();
                }else if(aElement.mValue->IsString()){
                    Core::String value = aElement.mValue->GetString();
                    SerialiseXmlGlyphs(value);
                    aStream << value;
                }else{
                    throw std::runtime_error("Xml::Element unexpected type");
                }
                break;
            default:
                throw std::runtime_error("Xml::Element unexpected type");
            }
            aStream << ' ';
            aStream << '>';
        }

        static ElementPointer Deserialise(std::istream& aStream, Core::Allocator& aParseAllocator, Core::Allocator& aDataAllocator, size_t& aCharsRead){
            Core::String name(aDataAllocator);
            Core::String value(aParseAllocator);
            Core::DynamicArray<AttributePointer> attributes(8, aParseAllocator);
            Core::DynamicArray<ElementPointer> children(8, aParseAllocator);

            size_t count = 0;
            char currentChar;

            const auto ReadChar = [&]()->char{
                char c;
                if(aStream.eof()) throw std::runtime_error("Xml::Attribute : End of stream reached unexpectedly");
                aStream >> c;
                ++count;
                return c;
            };

            currentChar = ReadChar();
            goto STATE_OPEN_START_TAG;

            STATE_OPEN_START_TAG:
            {
                switch(currentChar){
                case ' ':
                case '\t':
                case '\n':
                    currentChar = ReadChar();
                    goto STATE_OPEN_START_TAG;
                case '<':
                    currentChar = ReadChar();
                    goto STATE_OPEN_BEGIN_NAME;
                default:
                    goto STATE_FAIL;
                }
            }

            STATE_OPEN_BEGIN_NAME:
            {
                switch(currentChar){
                case ' ':
                case '\t':
                case '\n':
                    currentChar = ReadChar();
                    goto STATE_OPEN_BEGIN_NAME;
                default:
                    goto STATE_PARSE_BEGIN_NAME;
                }
            }

            STATE_PARSE_BEGIN_NAME:
            {
                switch(currentChar){
                case ' ':
                case '\t':
                case '\n':
                    currentChar = ReadChar();
                    goto STATE_OPEN_ATTRIBUTE;
                default:
                    name += currentChar;
                    currentChar = ReadChar();
                    goto STATE_PARSE_BEGIN_NAME;
                }
            }

            STATE_OPEN_ATTRIBUTE:
            {
                switch(currentChar){
                case ' ':
                case '\t':
                case '\n':
                    currentChar = ReadChar();
                    goto STATE_OPEN_ATTRIBUTE;
                case '/':
                    currentChar = ReadChar();
                    if(currentChar == '>'){
                        goto STATE_SUCCESS;
                    }else{
                        goto STATE_FAIL;
                    }
                case '>':
                    currentChar = ReadChar();
                    goto STATE_PARSE_VALUE;
                default:
                    goto STATE_PARSE_ATTRIBUTE;
                }
            }

            STATE_PARSE_ATTRIBUTE:
            {
                AttributePointer ptr = Attribute::Deserialise(aStream, aParseAllocator, aDataAllocator, aCharsRead);
                if(ptr){
                    attributes.PushBack(ptr);
                    goto STATE_OPEN_ATTRIBUTE;
                }else{
                    goto STATE_FAIL;
                }
            }

            STATE_PARSE_VALUE:
            {
                //! \TODO Check for child elements

                switch(currentChar){
                case '<':
                    currentChar = ReadChar();
                    goto STATE_PARSE_END_NAME;
                default:
                    value += currentChar;
                    currentChar = ReadChar();
                    goto STATE_PARSE_VALUE;
                }
            }

            STATE_PARSE_END_NAME:
            {
                Core::String endName(aParseAllocator);

                STATE_END_NAME_INTERNAL:
                switch(currentChar){
                case '>':
                    if(name == endName){
                        goto STATE_SUCCESS;
                    }else{
                        goto STATE_FAIL;
                    }
                default:
                    endName += currentChar;
                    currentChar = ReadChar();
                    goto STATE_END_NAME_INTERNAL;
                }
            }

            STATE_SUCCESS:
            {
                aCharsRead += count;
                ElementPointer element; //! \bug this fails -> = aDataAllocator.SharedAllocate<Element>(name);

                for(AttributePointer i : attributes){
                    element->AddAttribute(i);
                }

                if(value.Size() != 0){
                    DeserialiseXmlGlyphs(value);
                    element->SetString(value);
                }

                for(ElementPointer i : children){
                    element->AddChild(i);
                }

                return element;
            }

            STATE_FAIL:
            for(size_t i = 0; i < count; ++i) aStream.unget();
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

        bool IsNull(){
            return mType == TYPE_EMPTY;
        }

        bool IsBool(){
            return mType == TYPE_BODY ? mValue->IsBool() : false;
        }

        bool IsNumber(){
            return mType == TYPE_BODY ? mValue->IsNumber() : false;
        }

        bool IsString(){
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

        ConstElementIterator FindChild(const Core::ConstStringFragment aName) const{
            if(mType != TYPE_CHILDREN) return ConstElementIterator();
            return mChildren->FindFirst([&](const ConstElementPointer aPtr){
                return aPtr->GetName() == aName;
            });
        }

        ElementIterator FindChild(const Core::ConstStringFragment aName){
            if(mType != TYPE_CHILDREN) return ElementIterator();
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

        bool EraseChild(const Core::ConstStringFragment aName){
            if(mType != TYPE_CHILDREN) return false;
            return EraseChild(FindChild(aName));
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

        Core::ConstStringFragment GetName() const{
            return mName;
        }

        Core::Allocator& GetAllocator() const{
            return mAttributes.GetAllocator();
        }
    };

}}

#endif
