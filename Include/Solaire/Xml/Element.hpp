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
    typedef Core::Allocator::SharedPointer<Attribute> AttributePointer;
    typedef Core::Allocator::SharedPointer<const Element> ConstElementPointer;
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
        static void Serialise(std::ostream& aStream, const Attribute& aAttribute){
            aStream << aAttribute.mName;
            aStream << '=';
            aStream << '"';
            switch(aAttribute.mType){
            case TYPE_STRING:
                aStream << *aAttribute.mString;
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

        static AttributePointer Deserialise(std::istream& aStream, Core::Allocator& aParseAllocator, Core::Allocator& aDataAllocator){
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
            if(value.Size() == 0){
                return aDataAllocator.SharedAllocate<Attribute>(name);
            }else{
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
    };

    class Element : std::enable_shared_from_this<Attribute>{
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

        Core::DynamicArray<AttributePointer> mAttributes;
        union{
            Core::DynamicArray<ElementPointer>* mChildren;
            Attribute* mValue;
        };
        Type mType;
    public:
        typedef Core::DynamicArray<ElementPointer>::Iterator ElementIterator;
        typedef Core::DynamicArray<ElementPointer>::ConstIterator ConstElementIterator;
        typedef Core::DynamicArray<ElementPointer>::ReverseIterator ReverseElementIterator;
        typedef Core::DynamicArray<ElementPointer>::ConstReverseIterator ConstReverseElementIterator;

        typedef Core::DynamicArray<AttributePointer>::Iterator Attributeterator;
        typedef Core::DynamicArray<AttributePointer>::ConstIterator ConstAttributeIterator;
        typedef Core::DynamicArray<AttributePointer>::ReverseIterator ReverseAttributeIterator;
        typedef Core::DynamicArray<AttributePointer>::ConstReverseIterator ConstReverseAttributeIterator;
    };

}}

#endif
