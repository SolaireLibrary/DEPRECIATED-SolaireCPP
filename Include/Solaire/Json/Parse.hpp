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
        virtual Core::SharedPointer<Value> Get(Core::Allocator& aParseAllocator, Core::Allocator& aDocAllocator) const = 0;
    };

    static Core::SharedPointer<Parser> AllocateParser(Core::Allocator&, const TypeID);
    static TypeID IdentifyTypeID(const Core::String::Type);

    class StringParser : public Parser{
    private:
        Core::String mString;
        struct{
            uint8_t mFirstQuote : 1;
            uint8_t mSecondQuote : 1;
        };
    public:
        StringParser(Core::Allocator& aAllocator):
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

        Core::SharedPointer<Value> Get(Core::Allocator& aParseAllocator, Core::Allocator& aDocAllocator) const override{
            if(! (mFirstQuote && mSecondQuote)) return nullptr;
            return SolaireSmartAllocate(aParseAllocator, Value, (mString, aDocAllocator));
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

        Core::SharedPointer<Value> Get(Core::Allocator& aParseAllocator, Core::Allocator& aDocAllocator) const override{
            if(mCount == 4 && std::memcmp(mString, "true", 4) == 0){
                return SolaireSmartAllocate(aParseAllocator, Value, (true, aDocAllocator));
            }else if(mCount == 5 && std::memcmp(mString, "false", 5) == 0){
                return SolaireSmartAllocate(aParseAllocator, Value, (false, aDocAllocator));
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

       Core::SharedPointer<Value> Get(Core::Allocator& aParseAllocator, Core::Allocator& aDocAllocator) const override{
            return SolaireSmartAllocate(aParseAllocator, Value, (mValue.Get(), aDocAllocator));
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

        Core::SharedPointer<Value> Get(Core::Allocator& aParseAllocator, Core::Allocator& aDocAllocator) const override{
            if(mCount == 4 && std::memcmp(mString, "null", 4) == 0){
                return SolaireSmartAllocate(aParseAllocator, Value, (TYPE_NULL, aDocAllocator));
            }else{
                return nullptr;
            }

        }
    };

    class ArrayParser : public Parser{
    private:
        Core::DynamicArray<Core::SharedPointer<Parser>> mParsers;
        Core::Allocator& mAllocator;
        enum{
            STATE_OPEN_ARRAY,
            STATE_CLOSE_ARRAY,
            STATE_IDENTIFY_CHILD,
            STATE_PARSE_CHILD,
            STATE_CHILD_SEPARATOR
        };
        uint8_t mState;
    public:
        ArrayParser(Core::Allocator& aAllocator):
            mParsers(32, aAllocator),
            mAllocator(aAllocator),
            mState(STATE_OPEN_ARRAY)
        {}

        // Inherited from aAllocator

        bool Append(const Core::String::Type aChar) override{
            LABEL_START:
            switch(mState){
            case STATE_OPEN_ARRAY :
                goto LABEL_OPEN_ARRAY;
            case STATE_CLOSE_ARRAY :
                goto LABEL_CLOSE_ARRAY;
            case STATE_IDENTIFY_CHILD :
                goto LABEL_IDENTIFY_CHILD;
            case STATE_PARSE_CHILD :
                goto LABEL_PARSE_CHILD;
            case STATE_CHILD_SEPARATOR :
                goto LABEL_CHILD_SEPARATOR;
            default:
                throw std::runtime_error("Json::ArrayParser : Invalid State");
            }


            LABEL_OPEN_ARRAY:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return true;
            case '[':
                mState = STATE_IDENTIFY_CHILD;
                return true;
            default:
                return false;
            }

            LABEL_CLOSE_ARRAY:
            return false;

            LABEL_PARSE_CHILD:
            if(mParsers.Back()->Append(aChar)){
                return true;
            }else if(aChar == ']'){
                mState = STATE_CLOSE_ARRAY;
                return true;
            }else{
                mState = STATE_CHILD_SEPARATOR;
                goto LABEL_START;
            }

            LABEL_CHILD_SEPARATOR:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return true;
            case ',':
                mState = STATE_IDENTIFY_CHILD;
                return true;
            case ']':
                mState = STATE_CLOSE_ARRAY;
                return true;
            default:
                return false;
            }

            LABEL_IDENTIFY_CHILD:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return true;
            case ']':
                mState = STATE_CLOSE_ARRAY;
                return true;
            default:
                TypeID id;
                try{
                    id = IdentifyTypeID(aChar);
                }catch(...){
                    return false;
                }
                mParsers.PushBack(AllocateParser(mAllocator, id));

                mState = STATE_PARSE_CHILD;
                goto LABEL_START;
            }
        }

        Core::SharedPointer<Value> Get(Core::Allocator& aParseAllocator, Core::Allocator& aDocAllocator) const override{
            if(mState != STATE_CLOSE_ARRAY) return nullptr;
            Core::SharedPointer<Value> ptr = SolaireSmartAllocate(aParseAllocator, Value, (TYPE_ARRAY, aDocAllocator));
            Array& array_ = *ptr;
            for(Core::SharedPointer<Parser> i : mParsers){
                array_.PushBack(i->Get(aParseAllocator, aDocAllocator));
            }
            return ptr;
        }
    };

    class ObjectParser : public Parser{
    private:
        Core::DynamicArray<std::pair<Core::String, Core::SharedPointer<Parser>>> mParsers;
        Core::Allocator& mAllocator;
        Core::String mName;
        struct{
            uint8_t openFound : 1;
            uint8_t closeFound : 1;
            uint8_t nameParsed : 1;
            uint8_t nameOpen : 1;
            uint8_t nameClose : 1;
            uint8_t parseChild : 1;
        };
        enum{
            STATE_OPEN_OBJECT,
            STATE_CLOSE_OBJECT,
            STATE_OPEN_NAME,
            STATE_PARSE_NAME,
            STATE_NAME_SEPERATOR,
            STATE_IDENTIFY_CHILD,
            STATE_PARSE_CHILD,
            STATE_CHILD_SEPARATOR
        };
        uint8_t mState;
    public:
        ObjectParser(Core::Allocator& aAllocator):
            mParsers(32, aAllocator),
            mAllocator(aAllocator),
            mName(aAllocator),
            mState(STATE_OPEN_OBJECT)
        {}

        // Inherited from aAllocator

        bool Append(const Core::String::Type aChar) override{
            LABEL_START:
            switch(mState){
            case STATE_OPEN_OBJECT :
                goto LABEL_OPEN_OBJECT;
            case STATE_CLOSE_OBJECT :
                goto LABEL_CLOSE_OBJECT;
            case STATE_OPEN_NAME :
                goto LABEL_OPEN_NAME;
            case STATE_PARSE_NAME :
                goto LABEL_PARSE_NAME;
            case STATE_NAME_SEPERATOR :
                goto LABEL_NAME_SEPERATOR;
            case STATE_IDENTIFY_CHILD :
                goto LABEL_IDENTIFY_CHILD;
            case STATE_PARSE_CHILD :
                goto LABEL_PARSE_CHILD;
            case STATE_CHILD_SEPARATOR :
                goto LABEL_CHILD_SEPARATOR;
            default:
                throw std::runtime_error("Json::ObjectParser : Invalid state");
            }


            LABEL_OPEN_OBJECT:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return true;
            case '{':
                mState = STATE_OPEN_NAME;
                return true;
            default:
                return false;
            }

            LABEL_CLOSE_OBJECT:
            return false;

            LABEL_OPEN_NAME:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return true;
            case '"':
                mState = STATE_PARSE_NAME;
                return true;
            default:
                return false;
            }

            LABEL_PARSE_NAME:
            if(aChar == '"'){
                //! \TODO Handle escaped character
                mState = STATE_NAME_SEPERATOR;
                return true;
            }
            mName += aChar;
            return true;

            LABEL_NAME_SEPERATOR:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
            case ':':
                return true;
            default:
                mState = STATE_IDENTIFY_CHILD;
                goto LABEL_START;
            }

            LABEL_PARSE_CHILD:
            if(mParsers.Back().second->Append(aChar)){
                return true;
            }else if(aChar == '}'){
                mState = STATE_CLOSE_OBJECT;
                return true;
            }else{
                mState = STATE_CHILD_SEPARATOR;
                goto LABEL_START;
            }

            LABEL_CHILD_SEPARATOR:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return true;
            case ',':
                mState = STATE_OPEN_NAME;
                return true;
            case '}':
                mState = STATE_CLOSE_OBJECT;
                return true;
            default:
                return false;
            }

            LABEL_IDENTIFY_CHILD:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return true;
            case '}':
                mState = STATE_CLOSE_OBJECT;
                return true;
            default:
                TypeID id;
                try{
                    id = IdentifyTypeID(aChar);
                }catch(...){
                    return false;
                }

                mParsers.PushBack(std::pair<Core::String, Core::SharedPointer<Parser>>(mName,  AllocateParser(mAllocator, id)));
                mName.Clear();

                mState = STATE_PARSE_CHILD;
                goto LABEL_START;
            }
        }

        Core::SharedPointer<Value> Get(Core::Allocator& aParseAllocator, Core::Allocator& aDocAllocator) const override{
            if(mState != STATE_CLOSE_OBJECT) return nullptr;

            Core::SharedPointer<Value> ptr = SolaireSmartAllocate(aParseAllocator, Value, (TYPE_OBJECT, aDocAllocator));
            Object& object = *ptr;
            for(auto& i : mParsers){
                object.emplace(i.first, i.second->Get(aParseAllocator, aDocAllocator));
            }
            return ptr;
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

    static Core::SharedPointer<Parser> AllocateParser(Core::Allocator& aAllocator, const TypeID aID){
        switch(aID){
        case GetTypeID<Null>():
            return SolaireSmartAllocate(aAllocator, NullParser, ());
        case GetTypeID<Bool>():
            return SolaireSmartAllocate(aAllocator, BoolParser, ());
        case GetTypeID<Number>():
            return SolaireSmartAllocate(aAllocator, NumberParser, ());
        case GetTypeID<String>():
            return SolaireSmartAllocate(aAllocator, StringParser, (aAllocator));
        case GetTypeID<Array>():
            return SolaireSmartAllocate(aAllocator, ArrayParser, (aAllocator));
        case GetTypeID<Object>():
            return SolaireSmartAllocate(aAllocator, ObjectParser, (aAllocator));
        default:
            return nullptr;
        }
    }

}}

#endif
