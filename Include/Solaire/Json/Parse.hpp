#ifndef SOLAIRE_JSON_PARSE_HPP
#define SOLAIRE_JSON_PARSE_HPP

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
\file Parse.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 29th September 2015
Last Modified	: 29th September 2015
*/

#include "Value.hpp"

namespace Solaire{ namespace Json{

    class Parser{
    public:
        virtual ~Parser(){}

        virtual bool Append(const Core::String::Type aChar) = 0;
        virtual Value* Get(Core::Allocator<void>& aAllocator) const = 0;
    };

    static Parser* AllocateParser(Core::Allocator<void>&, const TypeID);
    static TypeID IdentifyTypeID(const Core::String::Type);

    class StringParser : public Parser{
    private:
        Core::String mString;
        struct{
            uint8_t mFirstQuote : 1;
            uint8_t mSecondQuote : 1;
        };
    public:
        StringParser(Core::Allocator<void>& aAllocator):
            mString(aAllocator),
            mFirstQuote(0),
            mSecondQuote(0)
        {}

        // Inherited from aAllocator

        bool Append(const Core::String::Type aChar) override{
            if(! mFirstQuote){
                switch(aChar){
                case ' ':
                case '\t':
                case '\n':
                    return true;
                case '"':
                    mFirstQuote = 1;
                    return true;
                }
            }else if(! mSecondQuote){
                if(aChar == '"'){
                    mSecondQuote = 1;
                    return true;
                }else{
                    mString += aChar;
                    return true;
                }
            }
            return false;
        }

        Value* Get(Core::Allocator<void>& aAllocator) const override{
            if(! (mFirstQuote && mSecondQuote)) return nullptr;
            //! \bug Value is never deallocated
            return new(Core::WrapperAllocator<String>(aAllocator).AllocateSingle()) String(mString);
        }
    };

    class BoolParser : public Parser{
    private:
        char mString[5];
        uint8_t mCount;
    public:
        BoolParser():
            mCount(0)
        {}

        // Inherited from aAllocator

        bool Append(const Core::String::Type aChar) override{
            switch(mCount){
            case 0:
                switch(aChar){
                case ' ':
                case '\t':
                case '\n':
                    return true;
                case 't':
                case 'f':
                    mString[mCount++] = aChar;
                    return true;
                default:
                    return false;
                }
            case 1:
                switch(aChar){
                case 'r':
                    if(mString[mCount - 1] != 't') return false;
                    mString[mCount++] = aChar;
                    return true;
                case 'a':
                    if(mString[mCount - 1] != 'f') return false;
                    mString[mCount++] = aChar;
                    return true;
                default:
                    return false;
                }
            case 2:
                switch(aChar){
                case 'u':
                    if(mString[mCount - 1] != 'r') return false;
                    mString[mCount++] = aChar;
                    return true;
                case 'l':
                    if(mString[mCount - 1] != 'a') return false;
                    mString[mCount++] = aChar;
                    return true;
                default:
                    return false;
                }
            case 3:
                switch(aChar){
                case 'e':
                    if(mString[mCount - 1] != 'u') return false;
                    mString[mCount++] = aChar;
                    return true;
                case 's':
                    if(mString[mCount - 1] != 'l') return false;
                    mString[mCount++] = aChar;
                    return true;
                default:
                    return false;
                }
            case 4:
                switch(aChar){
                case 'e':
                    if(mString[mCount - 1] != 's') return false;
                    mString[mCount++] = aChar;
                    return true;
                default:
                    return false;
                }
            default:
                return false;
            }
        }

        Value* Get(Core::Allocator<void>& aAllocator) const override{
            if(mCount == 4 && std::memcmp(mString, "true", 4) == 0){
                //! \bug Value is never deallocated
                return new(Core::WrapperAllocator<Bool>(aAllocator).AllocateSingle()) Bool(true);
            }else if(mCount == 5 && std::memcmp(mString, "false", 5) == 0){
                //! \bug Value is never deallocated
                return new(Core::WrapperAllocator<Bool>(aAllocator).AllocateSingle()) Bool(false);
            }else{
                return nullptr;
            }

        }
    };

    class NumberParser : public Parser{
    private:
        typedef Core::NumericParse::DecimalValue<Core::NumericParse::SignedValue, Core::NumericParse::UnsignedValue> DecimalValue;
        Core::NumericParse::ExponentValue<DecimalValue, DecimalValue> mValue;
    public:
        NumberParser():
            mValue()
        {}

        // Inherited from aAllocator

        bool Append(const Core::String::Type aChar) override{
            return mValue.Append(aChar);
        }

        Value* Get(Core::Allocator<void>& aAllocator) const override{
            //! \bug Value is never deallocated
            return new(Core::WrapperAllocator<Number>(aAllocator).AllocateSingle()) Number(mValue.Get());
        }
    };

    class NullParser : public Parser{
    private:
        char mString[4];
        uint8_t mCount;
    public:
        NullParser():
            mCount(0)
        {}

        // Inherited from aAllocator

        bool Append(const Core::String::Type aChar) override{
            switch(mCount){
            case 0:
                if(aChar == 'n'){
                    mString[mCount++] = 'n';
                    return true;
                }else{
                    return false;
                }
            case 1:
                if(aChar == 'u'){
                    mString[mCount++] = 'u';
                    return true;
                }else{
                    return false;
                }
            case 2:
                if(aChar == 'l'){
                    mString[mCount++] = 'l';
                    return true;
                }else{
                    return false;
                }
            case 3:
                if(aChar == 'l'){
                    mString[mCount++] = 'l';
                    return true;
                }else{
                    return false;
                }
            default:
                return false;
            }
        }

        Value* Get(Core::Allocator<void>& aAllocator) const override{
            if(mCount == 4 && std::memcmp(mString, "null", 4) == 0){
                //! \bug Value is never deallocated
                return new(Core::WrapperAllocator<Null>(aAllocator).AllocateSingle()) Null();
            }else{
                return nullptr;
            }

        }
    };

    class ArrayParser : public Parser{
    private:
        Core::DynamicArray<Parser*> mParsers;
        Core::Allocator<void>& mAllocator;
        struct{
            uint8_t openFound : 1;
            uint8_t closeFound : 1;
            uint8_t parseChild : 1;
        };
    public:
        ArrayParser(Core::Allocator<void>& aAllocator):
            mParsers(32, aAllocator),
            mAllocator(aAllocator),
            openFound(0),
            closeFound(0),
            parseChild(0)
        {}

        ~ArrayParser(){
            for(Parser* i : mParsers){
                i->~Parser();
                //! \bug Parser is not deallocated
            }
        }

        // Inherited from aAllocator

        bool Append(const Core::String::Type aChar) override{
            if(! openFound) goto ARRAY_OPEN;
            if(closeFound) goto ARRAY_CLOSED;
            if(parseChild) goto ARRAY_CHILD;
            goto ARRAY_IDENTIFY;


            ARRAY_OPEN:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return true;
            case '[':
                openFound = 1;
                return true;
            default:
                return false;
            }

            ARRAY_CLOSED:
            return false;

            ARRAY_CHILD:
            if(mParsers.Back()->Append(aChar)){
                return true;
            }else if(aChar == ']'){
                closeFound = 1;
                return true;
            }else{
                parseChild = 0;
                goto ARRAY_IDENTIFY;
            }

            ARRAY_IDENTIFY:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return true;
            case ']':
                closeFound = 1;
                return true;
            default:
                TypeID id;
                try{
                    id = IdentifyTypeID(aChar);
                }catch(...){
                    return false;
                }

                parseChild = 1;
                Parser* parser = AllocateParser(mAllocator, id);
                mParsers.PushBack(parser);
                goto ARRAY_CHILD;
            }
        }

        Value* Get(Core::Allocator<void>& aAllocator) const override{
            return nullptr;
        }
    };

    class ObjectParser : public Parser{
    private:
        Core::DynamicArray<std::pair<Core::String, Parser*>> mParsers;
        Core::Allocator<void>& mAllocator;
        Core::String mName;
        struct{
            uint8_t openFound : 1;
            uint8_t closeFound : 1;
            uint8_t nameParsed : 1;
            uint8_t nameOpen : 1;
            uint8_t nameClose : 1;
            uint8_t parseChild : 1;
        };
    public:
        ObjectParser(Core::Allocator<void>& aAllocator):
            mParsers(32, aAllocator),
            mAllocator(aAllocator),
            mName(aAllocator),
            openFound(0),
            closeFound(0),
            nameParsed(0),
            nameOpen(0),
            nameClose(0),
            parseChild(0)
        {}

        ~ObjectParser(){
            for(std::pair<Core::String, Parser*> i : mParsers){
                i.second->~Parser();
                //! \bug Parser is not deallocated
            }
        }

        // Inherited from aAllocator

        bool Append(const Core::String::Type aChar) override{
            if(! openFound) goto OBJECT_OPEN;
            if(closeFound) goto OBJECT_CLOSED;
            if(! nameParsed) goto OBJECT_NAME;
            if(parseChild) goto OBJECT_CHILD;
            goto OBJECT_IDENTIFY;


            OBJECT_OPEN:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return true;
            case '{':
                openFound = 1;
                return true;
            default:
                return false;
            }

            OBJECT_CLOSED:
            return false;

            OBJECT_NAME:
            if(! nameOpen){
                switch(aChar){
                case ' ':
                case '\t':
                case '\n':
                    return true;
                case '"':
                    nameOpen = 1;
                    return true;
                default:
                    return false;
                }
            }else if(! nameClose){
                if(aChar == '"'){
                    nameClose = 1;
                }else{
                    mName += aChar;
                }
            }else{
               return false;
            }

            OBJECT_CHILD:
            if(mParsers.Back().second->Append(aChar)){
                return true;
            }else if(aChar == '}'){
                closeFound = 1;
                return true;
            }else{
                nameParsed = 0;
                nameOpen = 0;
                nameClose = 0;
                parseChild = 0;
                mName.Clear();
                goto OBJECT_IDENTIFY;
            }

            OBJECT_IDENTIFY:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return true;
            case ']':
                closeFound = 1;
                return true;
            default:
                TypeID id;
                try{
                    id = IdentifyTypeID(aChar);
                }catch(...){
                    return false;
                }

                parseChild = 1;
                Parser* parser = AllocateParser(mAllocator, id);
                mParsers.PushBack(std::pair<Core::String, Parser*>(mName, parser));
                goto OBJECT_CHILD;
            }
        }

        Value* Get(Core::Allocator<void>& aAllocator) const override{
            return nullptr;
        }
    };

    static TypeID IdentifyTypeID(const Core::String::Type aChar){
        switch(aChar){
        case '[' :
            return GetTypeID<Array>();
        case '{' :
            return GetTypeID<Object>();
        case '"' :
            return GetTypeID<String>();
        case '-' :
        case '0' :
        case '1' :
        case '2' :
        case '3' :
        case '4' :
        case '5' :
        case '6' :
        case '7' :
        case '8' :
        case '9' :
            return GetTypeID<Number>();
        case 't' :
        case 'f' :
            return GetTypeID<Bool>();
        case 'n' :
            return GetTypeID<Null>();
        default:
            break;
        }
        throw std::runtime_error("Json::IdentifyTypeID : Could not identify TypeID");
    }

    static Parser* AllocateParser(Core::Allocator<void>& aAllocator, const TypeID aID){
        switch(aID){
        case GetTypeID<Null>():
            return new(Core::WrapperAllocator<NullParser>(aAllocator).AllocateSingle()) NullParser();
        case GetTypeID<Bool>():
            return new(Core::WrapperAllocator<BoolParser>(aAllocator).AllocateSingle()) BoolParser();
        case GetTypeID<Number>():
            return new(Core::WrapperAllocator<NullParser>(aAllocator).AllocateSingle()) NumberParser();
        case GetTypeID<String>():
            return new(Core::WrapperAllocator<StringParser>(aAllocator).AllocateSingle()) StringParser(aAllocator);
        case GetTypeID<Array>():
            return new(Core::WrapperAllocator<ArrayParser>(aAllocator).AllocateSingle()) ArrayParser(aAllocator);
        case GetTypeID<Object>():
            return new(Core::WrapperAllocator<ObjectParser>(aAllocator).AllocateSingle()) ObjectParser(aAllocator);
        default:
            return nullptr;
        }
    }

}}

#endif
