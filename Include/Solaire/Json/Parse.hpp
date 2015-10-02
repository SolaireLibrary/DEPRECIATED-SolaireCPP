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

    class Parser : public Core::StringParser<std::shared_ptr<Value>>, public std::enable_shared_from_this<Parser>{
    public:
        virtual ~Parser(){}
    };

    static std::shared_ptr<Parser> AllocateParser(Core::Allocator&, const TypeID);
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

        Status Accept(const Flags aFlags, const char aChar) override{
            if(! mFirstQuote){
                switch(aChar){
                case ' ':
                case '\t':
                case '\n':
                    return STATUS_SUCCESS;
                case '"':
                    mFirstQuote = 1;
                    return STATUS_SUCCESS;
                }
            }else if(! mSecondQuote){
                if(aChar == '"' && ! (aFlags & FLAG_IS_ESCAPED)){
                    mSecondQuote = 1;
                    return STATUS_COMPLETE;
                }else{
                    mString += aChar;
                    return STATUS_SUCCESS;
                }
            }
            return STATUS_FAIL;
        }

        std::shared_ptr<Value> Get(Core::Allocator& aParseAllocator, Core::Allocator& aDataAllocator) const override{
            if(! (mFirstQuote && mSecondQuote)) return nullptr;
            return aParseAllocator.SharedAllocate<Value>(mString, aDataAllocator);
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

        Status Accept(const Flags aFlags, const char aChar) override{
            switch(mCount){
            case 0:
                switch(aChar){
                case ' ':
                case '\t':
                case '\n':
                    return STATUS_SUCCESS;
                case 't':
                case 'f':
                    mString[mCount++] = aChar;
                    return STATUS_SUCCESS;
                default:
                    return STATUS_FAIL;
                }
            case 1:
                switch(aChar){
                case 'r':
                    if(mString[mCount - 1] != 't') return STATUS_FAIL;
                    mString[mCount++] = aChar;
                    return STATUS_SUCCESS;
                case 'a':
                    if(mString[mCount - 1] != 'f') return STATUS_FAIL;
                    mString[mCount++] = aChar;
                    return STATUS_SUCCESS;
                default:
                    return STATUS_FAIL;
                }
            case 2:
                switch(aChar){
                case 'u':
                    if(mString[mCount - 1] != 'r') return STATUS_FAIL;
                    mString[mCount++] = aChar;
                    return STATUS_SUCCESS;
                case 'l':
                    if(mString[mCount - 1] != 'a') return STATUS_FAIL;
                    mString[mCount++] = aChar;
                    return STATUS_SUCCESS;
                default:
                    return STATUS_FAIL;
                }
            case 3:
                switch(aChar){
                case 'e':
                    if(mString[mCount - 1] != 'u') return STATUS_FAIL;
                    mString[mCount++] = aChar;
                    return STATUS_COMPLETE;
                case 's':
                    if(mString[mCount - 1] != 'l') return STATUS_FAIL;
                    mString[mCount++] = aChar;
                    return STATUS_SUCCESS;
                default:
                    return STATUS_FAIL;
                }
            case 4:
                switch(aChar){
                case 'e':
                    if(mString[mCount - 1] != 's') return STATUS_FAIL;
                    mString[mCount++] = aChar;
                    return STATUS_COMPLETE;
                default:
                    return STATUS_FAIL;
                }
            default:
                return STATUS_FAIL;
            }
        }

        std::shared_ptr<Value> Get(Core::Allocator& aParseAllocator, Core::Allocator& aDataAllocator) const override{
            if(mCount == 4 && std::memcmp(mString, "true", 4) == 0){
                return aParseAllocator.SharedAllocate<Value>(true, aDataAllocator);
            }else if(mCount == 5 && std::memcmp(mString, "false", 5) == 0){
                return aParseAllocator.SharedAllocate<Value>(false, aDataAllocator);
            }else{
                return nullptr;
            }

        }
    };

    class NumberParser : public Parser{
    private:
        typedef Core::NumericParse::DecimalValue<Core::NumericParse::SignedValue, Core::NumericParse::UnsignedValue> DecimalValue;
        Core::NumericParser<double>::Type mValue;
    public:
        NumberParser():
            mValue()
        {}

        // Inherited from aAllocator

        Status Accept(const Flags aFlags, const char aChar) override{
            Core::ByteParser& parser = mValue;
            return parser.Accept(aChar);
        }

       std::shared_ptr<Value> Get(Core::Allocator& aParseAllocator, Core::Allocator& aDataAllocator) const override{
            return aParseAllocator.SharedAllocate<Value>(mValue.Get(aParseAllocator, aDataAllocator), aDataAllocator);
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

        Status Accept(const Flags aFlags, const char aChar) override{
            switch(mCount){
            case 0:
                if(aChar == 'n'){
                    mString[mCount++] = 'n';
                    return STATUS_SUCCESS;
                }else{
                    return STATUS_FAIL;
                }
            case 1:
                if(aChar == 'u'){
                    mString[mCount++] = 'u';
                    return STATUS_SUCCESS;
                }else{
                    return STATUS_FAIL;
                }
            case 2:
                if(aChar == 'l'){
                    mString[mCount++] = 'l';
                    return STATUS_SUCCESS;
                }else{
                    return STATUS_FAIL;
                }
            case 3:
                if(aChar == 'l'){
                    mString[mCount++] = 'l';
                    return STATUS_COMPLETE;
                }else{
                    return STATUS_FAIL;
                }
            default:
                return STATUS_FAIL;
            }
        }

        std::shared_ptr<Value> Get(Core::Allocator& aParseAllocator, Core::Allocator& aDocAllocator) const override{
            if(mCount == 4 && std::memcmp(mString, "null", 4) == 0){
                return aParseAllocator.SharedAllocate<Value>(TYPE_NULL, aDocAllocator);
            }else{
                return nullptr;
            }

        }
    };

    class ArrayParser : public Parser{
    private:
        Core::DynamicArray<std::shared_ptr<Parser>> mParsers;
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

        Status Accept(const Flags aFlags, const char aChar) override{
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
                return STATUS_SUCCESS;
            case '[':
                mState = STATE_IDENTIFY_CHILD;
                return STATUS_SUCCESS;
            default:
                return STATUS_FAIL;
            }

            LABEL_CLOSE_ARRAY:
            return STATUS_COMPLETE;

            LABEL_PARSE_CHILD:
            {
                const Status  status = mParsers.Back()->Accept(aChar);
                if(status == STATUS_SUCCESS){
                    return STATUS_SUCCESS;
                }else if(status == STATUS_FAIL){
                    return STATUS_FAIL;
                }else if(aChar == ']'){
                    mState = STATE_CLOSE_ARRAY;
                    goto LABEL_START;
                }else{
                    mState = STATE_CHILD_SEPARATOR;
                    goto LABEL_START;
                }
            }

            LABEL_CHILD_SEPARATOR:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return STATUS_SUCCESS;
            case ',':
                mState = STATE_IDENTIFY_CHILD;
                return STATUS_SUCCESS;
            case ']':
                mState = STATE_CLOSE_ARRAY;
                goto LABEL_START;
            default:
                return STATUS_FAIL;
            }

            LABEL_IDENTIFY_CHILD:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return STATUS_SUCCESS;
            case ']':
                if(mParsers.IsEmpty()){
                    mState = STATE_CLOSE_ARRAY;
                    goto LABEL_START;
                }else{
                    return STATUS_FAIL;
                }
            default:
                TypeID id;
                try{
                    id = IdentifyTypeID(aChar);
                }catch(...){
                    return STATUS_FAIL;
                }
                mParsers.PushBack(AllocateParser(mAllocator, id));

                mState = STATE_PARSE_CHILD;
                goto LABEL_START;
            }
        }

        std::shared_ptr<Value> Get(Core::Allocator& aParseAllocator, Core::Allocator& aDocAllocator) const override{
            if(mState != STATE_CLOSE_ARRAY) return nullptr;
            std::shared_ptr<Value> ptr = aParseAllocator.SharedAllocate<Value>(TYPE_ARRAY, aDocAllocator);
            Array& array_ = *ptr;
            for(std::shared_ptr<Parser> i : mParsers){
                array_.PushBack(i->Get(aParseAllocator, aDocAllocator));
            }
            return ptr;
        }
    };

    class ObjectParser : public Parser{
    private:
        Core::DynamicArray<std::pair<Core::String, std::shared_ptr<Parser>>> mParsers;
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

        Status Accept(const Flags aFlags, const char aChar) override{
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
                return STATUS_SUCCESS;
            case '{':
                mState = STATE_OPEN_NAME;
                return STATUS_SUCCESS;
            default:
                return STATUS_FAIL;
            }

            LABEL_CLOSE_OBJECT:
            return STATUS_COMPLETE;

            LABEL_OPEN_NAME:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return STATUS_SUCCESS;
            case '"':
                mState = STATE_PARSE_NAME;
                return STATUS_SUCCESS;
            case '}':
                if(mParsers.IsEmpty()){
                    mState = STATE_CLOSE_OBJECT;
                    goto LABEL_START;
                }else{
                    return STATUS_FAIL;
                }
            default:
                return STATUS_FAIL;
            }

            LABEL_PARSE_NAME:
            if(aChar == '"' && ! (aFlags & FLAG_IS_ESCAPED)){
                mState = STATE_NAME_SEPERATOR;
                return STATUS_SUCCESS;
            }
            mName += aChar;
            return STATUS_SUCCESS;

            LABEL_NAME_SEPERATOR:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return STATUS_SUCCESS;
            case ':':
                mState = STATE_IDENTIFY_CHILD;
                return STATUS_SUCCESS;
            default:
                return STATUS_FAIL;
            }

            LABEL_PARSE_CHILD:
            {
                const Status status = mParsers.Back().second->Accept(aChar);
                if(status == STATUS_SUCCESS){
                    return STATUS_SUCCESS;
                }else if(status == STATUS_FAIL){
                    return STATUS_FAIL;
                }else if(aChar == '}'){
                    mState = STATE_CLOSE_OBJECT;
                    goto LABEL_START;
                }else{
                    mState = STATE_CHILD_SEPARATOR;
                    goto LABEL_START;
                }
            }

            LABEL_CHILD_SEPARATOR:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return STATUS_SUCCESS;
            case ',':
                mState = STATE_OPEN_NAME;
                return STATUS_SUCCESS;
            case '}':
                mState = STATE_CLOSE_OBJECT;
                goto LABEL_START;
            default:
                return STATUS_FAIL;
            }

            LABEL_IDENTIFY_CHILD:
            switch(aChar){
            case ' ':
            case '\t':
            case '\n':
                return STATUS_SUCCESS;
            case '}':
                mState = STATE_CLOSE_OBJECT;
                goto LABEL_START;
            default:
                TypeID id;
                try{
                    id = IdentifyTypeID(aChar);
                }catch(...){
                    return STATUS_FAIL;
                }

                mParsers.PushBack(std::pair<Core::String, std::shared_ptr<Parser>>(mName,  AllocateParser(mAllocator, id)));
                mName.Clear();

                mState = STATE_PARSE_CHILD;
                goto LABEL_START;
            }
        }

        std::shared_ptr<Value> Get(Core::Allocator& aParseAllocator, Core::Allocator& aDocAllocator) const override{
            if(mState != STATE_CLOSE_OBJECT) return nullptr;

            std::shared_ptr<Value> ptr = aParseAllocator.SharedAllocate<Value>(TYPE_OBJECT, aDocAllocator);
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

    static std::shared_ptr<Parser> AllocateParser(Core::Allocator& aAllocator, const TypeID aID){
        switch(aID){
        case GetTypeID<Null>():
            return aAllocator.SharedAllocate<NullParser>();
        case GetTypeID<Bool>():
            return aAllocator.SharedAllocate<BoolParser>();
        case GetTypeID<Number>():
            return aAllocator.SharedAllocate<NumberParser>();
        case GetTypeID<String>():
            return aAllocator.SharedAllocate<StringParser>(aAllocator);
        case GetTypeID<Array>():
            return aAllocator.SharedAllocate<ArrayParser>(aAllocator);
        case GetTypeID<Object>():
            return aAllocator.SharedAllocate<ObjectParser>(aAllocator);
        default:
            return nullptr;
        }
    }

}}

#endif
