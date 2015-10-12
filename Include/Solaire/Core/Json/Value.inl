#ifndef SOLAIRE_JSON_VALUE_INL
#define SOLAIRE_JSON_VALUE_INL

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
\file Value.inl
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 4th October 2015
Last Modified	: 4th October 2015
*/

#include <Type_traits>
#include <map>
#include <list>
#include "..\Strings\String.hpp"
#include "..\Strings\NumberParser.hpp"
#include "..\Iterators\DereferenceIterator.hpp"

namespace Solaire{ namespace Json{

    // NullValue

    NullValue::NullValue(Value* aParent):
        mParent(aParent)
    {}

    // BoolValue

    BoolValue::BoolValue(Value* aParent):
        mParent(aParent)
    {}

    BoolValue::operator bool() const{
        switch(mParent->mType){
        case TYPE_BOOL:
           return mParent->mDataBool;
        case TYPE_NUMBER:
            return mParent->mDataNumber > 0.0;
        default:
            throw std::runtime_error("Json::BoolValue : Cannot convert value to bool");
        }
    }

    BoolValue& BoolValue::operator=(const bool aValue){
        switch(mParent->mType){
        case TYPE_NULL:
        case TYPE_BOOL:
        case TYPE_NUMBER:
           break;
        default:
            *mParent = TYPE_NULL;
            break;
        }
        mParent->mDataBool = aValue;
        mParent->mType = TYPE_BOOL;
        return *this;
    }


    // NumberValue

    NumberValue::NumberValue(Value* aParent):
        mParent(aParent)
    {}

    NumberValue::operator uint8_t() const{
        return static_cast<uint8_t>(operator double());
    }

    NumberValue::operator uint16_t() const{
        return static_cast<uint16_t>(operator double());
    }

    NumberValue::operator uint32_t() const{
        return static_cast<uint32_t>(operator double());
    }

    NumberValue::operator uint64_t() const{
        return static_cast<uint64_t>(operator double());
    }

    NumberValue::operator int8_t() const{
        return static_cast<int8_t>(operator double());
    }

    NumberValue::operator int16_t() const{
        return static_cast<int16_t>(operator double());
    }

    NumberValue::operator int32_t() const{
        return static_cast<int32_t>(operator double());
    }

    NumberValue::operator int64_t() const{
        return static_cast<int64_t>(operator double());
    }

    NumberValue::operator float() const{
        return static_cast<float>(operator double());
    }

    NumberValue::operator double() const{
        switch(mParent->mType){
        case TYPE_BOOL:
           return mParent->mDataBool ? 1.0 : 0.0;
        case TYPE_NUMBER:
            return mParent->mDataNumber;
        default:
            throw std::runtime_error("Json::NumberValue : Cannot convert value to double");
        }
    }

    NumberValue& NumberValue::operator=(const uint8_t aValue){
        return operator=(static_cast<double>(aValue));
    }

    NumberValue& NumberValue::operator=(const uint16_t aValue){
        return operator=(static_cast<double>(aValue));
    }

    NumberValue& NumberValue::operator=(const uint32_t aValue){
        return operator=(static_cast<double>(aValue));
    }

    NumberValue& NumberValue::operator=(const uint64_t aValue){
        return operator=(static_cast<double>(aValue));
    }

    NumberValue& NumberValue::operator=(const int8_t aValue){
        return operator=(static_cast<double>(aValue));
    }

    NumberValue& NumberValue::operator=(const int16_t aValue){
        return operator=(static_cast<double>(aValue));
    }

    NumberValue& NumberValue::operator=(const int32_t aValue){
        return operator=(static_cast<double>(aValue));
    }

    NumberValue& NumberValue::operator=(const int64_t aValue){
        return operator=(static_cast<double>(aValue));
    }

    NumberValue& NumberValue::operator=(const float aValue){
        return operator=(static_cast<double>(aValue));
    }

    NumberValue& NumberValue::operator=(const double aValue){
        switch(mParent->mType){
        case TYPE_NULL:
        case TYPE_BOOL:
        case TYPE_NUMBER:
           break;
        default:
            *mParent = TYPE_NULL;
            break;
        }
        mParent->mDataNumber = aValue;
        mParent->mType = TYPE_NUMBER;
        return *this;
    }


    // StringValue

    StringValue::StringValue(Value* aParent):
        mParent(aParent)
    {}

    StringValue::operator char() const{
        switch(mParent->mType){
        case TYPE_BOOL:
           return mParent->mDataBool ? '0' : '1';
        case TYPE_NUMBER:
            return static_cast<char>(mParent->mDataNumber);
        case TYPE_STRING:
            return (*mParent->mDataString)[0];
        default:
            throw std::runtime_error("Json::StringValue : Cannot convert value to char");
        }
    }

    StringValue::operator ConstStringFragment() const{
        //! \TODO Buffer temp string for non string types
        switch(mParent->mType){
        case TYPE_STRING:
            return *mParent->mDataString;
        default:
            throw std::runtime_error("Json::StringValue : Cannot convert value to ConstStringFragment");
        }
    }

    StringValue& StringValue::operator=(const char aValue){
        switch(mParent->mType){
        case TYPE_NULL:
        case TYPE_BOOL:
        case TYPE_NUMBER:
        case TYPE_STRING:
           break;
        default:
            *mParent = TYPE_NULL;
            break;
        }
        (*mParent->mDataString).Clear();
        (*mParent->mDataString) += aValue;
        mParent->mType = TYPE_STRING;
        return *this;
    }

    StringValue& StringValue::operator=(const ConstStringFragment aValue){
        switch(mParent->mType){
        case TYPE_NULL:
        case TYPE_BOOL:
        case TYPE_NUMBER:
        case TYPE_STRING:
           break;
        default:
            *mParent = TYPE_NULL;
            break;
        }
        (*mParent->mDataString) = aValue;
        mParent->mType = TYPE_STRING;
        return *this;
    }


    // ArrayValue

    ArrayValue::ArrayValue(Value* aParent):
        mParent(aParent)
    {}

    size_t ArrayValue::Size() const{
        switch(mParent->mType){
        case TYPE_ARRAY:
            return mParent->mDataArray->Size();
        case TYPE_OBJECT:
            return mParent->mDataObject->Size();
            break;
        default:
            return 0;
        }
    }

    Value& ArrayValue::operator[](const size_t aIndex){
        switch(mParent->mType){
        case TYPE_ARRAY:
            return *(*mParent->mDataArray)[0];
        case TYPE_OBJECT:
            return *(*mParent->mDataObject)[0].second;
            break;
        default:
            throw std::runtime_error("Json::ArrayValue : Cannot access this type by index");
        }
    }

    const Value& ArrayValue::operator[](const size_t aIndex) const{
        switch(mParent->mType){
        case TYPE_ARRAY:
            return *(*mParent->mDataArray)[aIndex];
        case TYPE_OBJECT:
            return *(*mParent->mDataObject)[aIndex].second;
            break;
        default:
            throw std::runtime_error("Json::ArrayValue : Cannot access this type by index");
        }
    }

    Value& ArrayValue::PushBack(std::shared_ptr<Value> aValue){
        switch(mParent->mType){
        case TYPE_ARRAY:
            return *mParent->mDataArray->PushBack(aValue);
        default:
            throw std::runtime_error("Json::ArrayValue : Type is not array");
        }
    }

    void ArrayValue::Erase(const size_t aIndex){
        switch(mParent->mType){
        case TYPE_ARRAY:
            mParent->mDataArray->Erase(mParent->mDataArray->begin() + aIndex);
        case TYPE_OBJECT:
            mParent->mDataObject->Erase(mParent->mDataObject->begin() + aIndex);
            break;
        default:
            throw std::runtime_error("Json::ArrayValue : Cannot erase from this type by index");
        }
    }

    void ArrayValue::Erase(const Value& aValue){
        const std::shared_ptr<Value> value = const_cast<Value&>(aValue).shared_from_this();
        switch(mParent->mType){
        case TYPE_ARRAY:
            mParent->mDataArray->Erase(mParent->mDataArray->FindFirst(value));
        case TYPE_OBJECT:
            mParent->mDataObject->Erase(mParent->mDataObject->FindFirstIf([=](const std::pair<String, std::shared_ptr<Value>>& i){
                return i.second == value;
            }));
            break;
        default:
            throw std::runtime_error("Json::ArrayValue : Cannot erase from this type by index");
        }
    }


    // ObjectValue

    ObjectValue::ObjectValue(Value* aParent):
        mParent(aParent)
    {}

    DynamicArray<std::pair<String, std::shared_ptr<Value>>>::ConstIterator ObjectValue::SearchByObjectName(const ConstStringFragment aName) const{
        return mParent->mDataObject->FindFirstIf([=](const std::pair<String, std::shared_ptr<Value>>& i){
            return i.first == aName;
        });
    }

    DynamicArray<std::pair<String, std::shared_ptr<Value>>>::ConstIterator ObjectValue::SearchByObjectValue(const Value& aValue) const{
        std::shared_ptr<Value> value = const_cast<Value&>(aValue).shared_from_this();
        return mParent->mDataObject->FindFirstIf([=](const std::pair<String, std::shared_ptr<Value>>& i){
            return i.second == value;
        });
    }

    size_t ObjectValue::Size() const{
        switch(mParent->mType){
        case TYPE_OBJECT:
            return mParent->mDataObject->Size();
            break;
        default:
            return 0;
        }
    }

    Value& ObjectValue::operator[](const ConstStringFragment aName){
        switch(mParent->mType){
        case TYPE_OBJECT:
            *SearchByObjectName(aName)->second;
        default:
            throw std::runtime_error("Json::ObjectValue : Cannot search this type by name");
        }
    }

    const Value& ObjectValue::operator[](const ConstStringFragment aName) const{
        switch(mParent->mType){
        case TYPE_OBJECT:
            *SearchByObjectName(aName)->second;
        default:
            throw std::runtime_error("Json::ObjectValue : Cannot search this type by name");
        }
    }

    bool ObjectValue::Contains(const ConstStringFragment aName) const{
        switch(mParent->mType){
        case TYPE_OBJECT:
            return SearchByObjectName(aName) != mParent->mDataObject->end();
        default:
            throw std::runtime_error("Json::ObjectValue : Cannot search this type by name");
        }
    }

    Value& ObjectValue::Add(const ConstStringFragment aName, std::shared_ptr<Value> aValue){
        switch(mParent->mType){
        case TYPE_OBJECT:
            if(SearchByObjectName(aName) != mParent->mDataObject->end())  throw std::runtime_error("Json::ObjectValue : Already contains named value with this name");
            return *mParent->mDataObject->PushBack(std::pair<String, std::shared_ptr<Value>>(String(*mParent->pAllocator, aName), aValue)).second;
            break;
        default:
            throw std::runtime_error("Json::ObjectValue : Cannot add named value to this type");
        }
    }

    void ObjectValue::Erase(const ConstStringFragment aName){
        switch(mParent->mType){
        case TYPE_OBJECT:
            mParent->mDataObject->Erase(SearchByObjectName(aName));
            break;
        default:
            throw std::runtime_error("Json::ObjectValue : Cannot erase from this type by name");
        }
    }

    void ObjectValue::Erase(const Value& aValue){
        switch(mParent->mType){
        case TYPE_ARRAY:
            mParent->mDataArray->Erase(mParent->mDataArray->FindFirst(const_cast<Value&>(aValue).shared_from_this()));
            break;
        case TYPE_OBJECT:
            mParent->mDataObject->Erase(SearchByObjectValue(aValue));
            break;
        default:
            throw std::runtime_error("Json::ObjectValue : Cannot erase from this type by value");
        }
    }

    ObjectValue::ConstIterator ObjectValue::begin() const{
        return mParent->mDataObject->begin();
    }

    ObjectValue::ConstIterator ObjectValue::end() const{
        return mParent->mDataObject->end();
    }

    // Value

    bool Value::InternalSerialise(const std::function<void(char)>& aSetFn, const std::function<void(void)>& aForwardFn, const std::function<void(void)>& aBackwardFn, const Value& aValue){
        const auto WriteString = [&](ConstStringFragment aString)->void{
            for(const char c : aString){
                if(c == '"'){
                    aSetFn('\\');
                    aForwardFn();
                }
                aSetFn(c);
                aForwardFn();
            }
        };

        switch(aValue.mType){
        case TYPE_NULL:
            WriteString("null");
            return true;
        case TYPE_BOOL:
            WriteString(aValue.mDataBool ? "true" : "false");
            return true;
        case TYPE_NUMBER:
            {
                char buf[16];
                WriteString(ConstStringFragment(buf, NumericParse::ToString(buf, aValue.mDataNumber)));
            }
            return true;
        case TYPE_STRING:
            aSetFn('"');
            aForwardFn();
            WriteString(*aValue.mDataString);
            aSetFn('"');
            aForwardFn();

            return true;
        case TYPE_ARRAY:
            {
                aSetFn('[');
                aForwardFn();
                ArrayType::Iterator end = aValue.mDataArray->end();
                for(ArrayType::Iterator i = aValue.mDataArray->begin(); i != end; ++i){
                    if(! InternalSerialise(aSetFn, aForwardFn, aBackwardFn, **i)) return false;
                    if((i + 1) != end){
                        aSetFn(',');
                        aForwardFn();
                    }
                }
                aSetFn(']');
                aForwardFn();
                return false;
            }
        case TYPE_OBJECT:
            {
                aSetFn('{');
                aForwardFn();
                ObjectType::Iterator end = aValue.mDataObject->end();
                for(ObjectType::Iterator i = aValue.mDataObject->begin(); i != end; ++i){
                    aSetFn('"');
                    aForwardFn();
                    WriteString(i->first);
                    aSetFn('"');
                    aForwardFn();
                    aSetFn(':');
                    aForwardFn();
                    if(! InternalSerialise(aSetFn, aForwardFn, aBackwardFn, *i->second)) return false;
                    if((i + 1) != end){
                        aSetFn(',');
                        aForwardFn();
                    }
                }
                aSetFn('}');
                aForwardFn();
                return true;
            }
        default:
            return false;
        }
    }

    std::shared_ptr<Value> Value::InternalDeserialise(const std::function<char(void)>& aGetFn, const std::function<void(void)>& aForwardFn, const std::function<void(void)>& aBackwardFn, Allocator& aParseAllocator, Allocator& aDataAllocator){
        goto STATE_ID;

        STATE_ID:
        {
            switch(aGetFn()){
            case ' ':
            case '\t':
            case '\n':
                goto STATE_ID;
            case 'n':
                goto STATE_NULL;
            case 't':
            case 'f':
                goto STATE_BOOL;
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                goto STATE_NUMBER;
            case '"':
                goto STATE_STRING;
            case '[':
                goto STATE_ARRAY;
            case '{':
                goto STATE_OBJECT;
            default:
                goto STATE_FAIL;
            }
        }

        STATE_NULL:
        {
            aForwardFn();
            if(aGetFn() != 'u') goto STATE_FAIL;
            aForwardFn();
            if(aGetFn() != 'l') goto STATE_FAIL;
            aForwardFn();
            if(aGetFn() != 'l') goto STATE_FAIL;
            aForwardFn();
            return aDataAllocator.SharedAllocate<Value>(aDataAllocator, TYPE_NULL);
        }
        goto STATE_FAIL;

        STATE_BOOL:
        {
            if(aGetFn() == 't'){
                aForwardFn();
                if(aGetFn() != 'r') goto STATE_FAIL;
                aForwardFn();
                if(aGetFn() != 'u') goto STATE_FAIL;
                aForwardFn();
                if(aGetFn() != 'e') goto STATE_FAIL;
                aForwardFn();
                return aDataAllocator.SharedAllocate<Value>(aDataAllocator, true);
            }else{
                aForwardFn();
                if(aGetFn() != 'a') goto STATE_FAIL;
                aForwardFn();
                if(aGetFn() != 'l') goto STATE_FAIL;
                aForwardFn();
                if(aGetFn() != 's') goto STATE_FAIL;
                aForwardFn();
                if(aGetFn() != 'e') goto STATE_FAIL;
                aForwardFn();
                return aDataAllocator.SharedAllocate<Value>(aDataAllocator, false);
            }
        }
        goto STATE_FAIL;

        STATE_NUMBER:
        {
            char buf[24];
            char* bufEnd = buf;

            STATE_NUMBER_BUF:
            *bufEnd = aGetFn();
            switch(*bufEnd){
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '-':
            case '+':
            case 'E':
            case 'e':
            case '.':
                ++bufEnd;
                aForwardFn();
                goto STATE_NUMBER_BUF;
            default:
                {
                    *bufEnd = '\0';
                    char* it = buf;
                    double num = ParseNumber<double>(buf, bufEnd, it);
                    if(it == buf){
                        goto STATE_FAIL;
                    }else{
                        return aDataAllocator.SharedAllocate<Value>(aDataAllocator, num);
                    }
                }
                break;
            }
        }
        goto STATE_FAIL;

        STATE_STRING:
        {
            aForwardFn();
            String string(aParseAllocator);
            bool isEscaped = false;
            char c;

            STATE_STRING_GET:
            c = aGetFn();
            switch(c){
            case '\\':
                isEscaped = ! isEscaped;
                if(! isEscaped) string += '\\';
                aForwardFn();
                goto STATE_STRING_GET;
            case '"':
                if(isEscaped) string += '"';
                goto STATE_STRING_RETURN;
            default:
                string += c;
                aForwardFn();
                goto STATE_STRING_GET;
            }

            STATE_STRING_RETURN:
            return aDataAllocator.SharedAllocate<Value>(aDataAllocator, string);
        }

        STATE_ARRAY:
        {
            std::shared_ptr<Value> value = aDataAllocator.SharedAllocate<Value>(aDataAllocator, TYPE_ARRAY);
            aForwardFn();
            goto STATE_ARRAY_MEMEBER;

            STATE_ARRAY_MEMEBER:
            switch(aGetFn()){
            case ' ':
            case '\t':
            case '\n':
                aForwardFn();
                goto STATE_ARRAY_MEMEBER;
            case ']':
                goto STATE_ARRAY_RETURN;
            default:
                std::shared_ptr<Value> member = InternalDeserialise(aGetFn, aForwardFn, aBackwardFn, aParseAllocator, aDataAllocator);
                if(! member) goto STATE_FAIL;
                value->pArray->PushBack(member);
                aForwardFn();
                goto STATE_ARRAY_SEPERATE_MEMEBER;
            }

            STATE_ARRAY_SEPERATE_MEMEBER:
            switch(aGetFn()){
            case ' ':
            case '\t':
            case '\n':
                aForwardFn();
                goto STATE_ARRAY_SEPERATE_MEMEBER;
            case ',':
                aForwardFn();
                goto STATE_ARRAY_MEMEBER;
            case ']':
                goto STATE_ARRAY_RETURN;
            default:
                goto STATE_FAIL;
            }

            STATE_ARRAY_RETURN:
            return value;
        }

        STATE_OBJECT:
        {
            char c;
            bool isEscaped;
            char nameBuf[64];
            char* nameEnd  = nameBuf;
            std::shared_ptr<Value> value = aDataAllocator.SharedAllocate<Value>(aDataAllocator, TYPE_OBJECT);
            aForwardFn();
            goto STATE_OBJECT_NAME;

            STATE_OBJECT_NAME:
            switch(aGetFn()){
            case ' ':
            case '\t':
            case '\n':
                aForwardFn();
                goto STATE_OBJECT_NAME;
            case '"':
                isEscaped = false;
                aForwardFn();
                goto STATE_OBJECT_NAME_PARSE;
            default:
                goto STATE_FAIL;
            }

            STATE_OBJECT_NAME_PARSE:
            c = aGetFn();
            switch(c){
            case '\\':
                isEscaped = ! isEscaped;
                if(! isEscaped){
                    *nameEnd = '\\';
                    ++nameEnd;
                }
                aForwardFn();
                goto STATE_OBJECT_NAME_PARSE;
            case '"':
                if(isEscaped){
                    *nameEnd = '"';
                    ++nameEnd;
                }
                aForwardFn();
                goto STATE_OBJECT_SEPERATE_NAME;
            case '}':
                goto STATE_OBJECT_RETURN;
            default:
                *nameEnd = c;
                ++nameEnd;
                aForwardFn();
                goto STATE_OBJECT_NAME_PARSE;
            }

            STATE_OBJECT_SEPERATE_NAME:
            switch(aGetFn()){
            case ' ':
            case '\t':
            case '\n':
                aForwardFn();
                goto STATE_OBJECT_SEPERATE_NAME;
            case ':':
                aForwardFn();
                goto STATE_OBJECT_MEMEBER;
            default:
                goto STATE_FAIL;
            }

            STATE_OBJECT_MEMEBER:
            switch(aGetFn()){
            case ' ':
            case '\t':
            case '\n':
                aForwardFn();
                goto STATE_OBJECT_MEMEBER;
            case '}':
                goto STATE_OBJECT_RETURN;
            default:
                std::shared_ptr<Value> member = InternalDeserialise(aGetFn, aForwardFn, aBackwardFn, aParseAllocator, aDataAllocator);
                if(! member) goto STATE_FAIL;
                value->pObject->Add(ConstStringFragment(nameBuf, nameEnd), member);
                nameEnd = nameBuf;
                goto STATE_OBJECT_SEPERATE_MEMEBER;
            }

            STATE_OBJECT_SEPERATE_MEMEBER:
            switch(aGetFn()){
            case ' ':
            case '\t':
            case '\n':
                aForwardFn();
                goto STATE_OBJECT_SEPERATE_MEMEBER;
            case ',':
                aForwardFn();
                goto STATE_OBJECT_NAME;
            case '}':
                goto STATE_OBJECT_RETURN;
            default:
                goto STATE_FAIL;
            }

            STATE_OBJECT_RETURN:
            return value;
        }

        STATE_FAIL:
        return std::shared_ptr<Value>();
    }

    size_t Value::EstimateSerialLength(const Value& aValue){
        size_t count = 0;

        const auto WriteString = [&](ConstStringFragment aString)->void{
            count += aString.Size();
        };

        switch(aValue.mType){
        case TYPE_NULL:
            WriteString("null");
            return count;
        case TYPE_BOOL:
            WriteString(aValue.mDataBool ? "true" : "false");
            return count;
        case TYPE_NUMBER:
            count += 16;
            return count;
        case TYPE_STRING:
            count += 2;
            count += aValue.mDataString->Size();
            return count;
        case TYPE_ARRAY:
            {
                ++count;
                ArrayType::Iterator end = aValue.mDataArray->end();
                for(ArrayType::Iterator i = aValue.mDataArray->begin(); i != end; ++i){
                    count += EstimateSerialLength(**i);
                    if((i - 1) != end){
                        ++count;
                    }
                }
                ++count;
                return count;
            }
        case TYPE_OBJECT:
            {
                ++count;
                ObjectType::Iterator end = aValue.mDataObject->end();
                for(ObjectType::Iterator i = aValue.mDataObject->begin(); i != end; ++i){
                    count += 3;
                    WriteString(i->first);
                    count += EstimateSerialLength(*i->second);
                    if((i - 1) != end){
                        ++count;
                    }
                }
                ++count;
                return count;
            }
        default:
            return false;
        }
    }

    Value::Value(Value&& aOther):
        mAllocator(aOther.mAllocator),
        mNull(this),
        pNull(*this),
        mType(TYPE_NULL)
    {
        operator=(std::move(aOther));
    }

    Value::Value(Allocator& aAllocator):
        mAllocator(&aAllocator),
        mNull(this),
        pNull(*this),
        mType(TYPE_NULL)
    {}

    Value::Value(Allocator& aAllocator, const TypeID aType):
        mAllocator(&aAllocator),
        mNull(this),
        pNull(*this),
        mType(TYPE_NULL)
    {
        operator=(aType);
    }

    Value::Value(Allocator& aAllocator, const bool aValue):
        mAllocator(&aAllocator),
        mNull(this),
        pNull(*this),
        mDataBool(aValue),
        mType(TYPE_BOOL)
    {}

    Value::Value(Allocator& aAllocator, const uint8_t aValue):
        mAllocator(&aAllocator),
        mNull(this),
        pNull(*this),
        mDataNumber(static_cast<double>(aValue)),
        mType(TYPE_NUMBER)
    {}

    Value::Value(Allocator& aAllocator, const uint16_t aValue):
        mAllocator(&aAllocator),
        mNull(this),
        pNull(*this),
        mDataNumber(static_cast<double>(aValue)),
        mType(TYPE_NUMBER)
    {}

    Value::Value(Allocator& aAllocator, const uint32_t aValue):
        mAllocator(&aAllocator),
        mNull(this),
        pNull(*this),
        mDataNumber(static_cast<double>(aValue)),
        mType(TYPE_NUMBER)
    {}

    Value::Value(Allocator& aAllocator, const uint64_t aValue):
        mAllocator(&aAllocator),
        mNull(this),
        pNull(*this),
        mDataNumber(static_cast<double>(aValue)),
        mType(TYPE_NUMBER)
    {}

    Value::Value(Allocator& aAllocator, const int8_t aValue):
        mAllocator(&aAllocator),
        mNull(this),
        pNull(*this),
        mDataNumber(static_cast<double>(aValue)),
        mType(TYPE_NUMBER)
    {}

    Value::Value(Allocator& aAllocator, const int16_t aValue):
        mAllocator(&aAllocator),
        mNull(this),
        pNull(*this),
        mDataNumber(static_cast<double>(aValue)),
        mType(TYPE_NUMBER)
    {}

    Value::Value(Allocator& aAllocator, const int32_t aValue):
        mAllocator(&aAllocator),
        mNull(this),
        pNull(*this),
        mDataNumber(static_cast<double>(aValue)),
        mType(TYPE_NUMBER)
    {}

    Value::Value(Allocator& aAllocator, const int64_t aValue):
        mAllocator(&aAllocator),
        mNull(this),
        pNull(*this),
        mDataNumber(static_cast<double>(aValue)),
        mType(TYPE_NUMBER)
    {}

    Value::Value(Allocator& aAllocator, const float aValue):
        mAllocator(&aAllocator),
        mNull(this),
        pNull(*this),
        mDataNumber(static_cast<double>(aValue)),
        mType(TYPE_NUMBER)
    {}

    Value::Value(Allocator& aAllocator, const double aValue):
        mAllocator(&aAllocator),
        mNull(this),
        pNull(*this),
        mDataNumber(aValue),
        mType(TYPE_NUMBER)
    {}

    Value::Value(Allocator& aAllocator, const ConstStringFragment aValue):
        mAllocator(&aAllocator),
        mNull(this),
        pNull(*this),
        mType(TYPE_NULL)
    {
        operator=(TYPE_STRING);
        *mDataString = aValue;
    }

    Value& Value::operator=(Value&& aOther){
        operator=(TYPE_NULL);
        mType = aOther.mType;

        switch(mType){
        case TYPE_NULL:
            break;
        case TYPE_BOOL:
            mDataBool = aOther.mDataBool;
            break;
        case TYPE_NUMBER:
            mDataNumber = aOther.mDataNumber;
            break;
        case TYPE_STRING:
            mDataString = aOther.mDataString;
            aOther.mDataString = nullptr;
            break;
        case TYPE_ARRAY:
            mDataArray = aOther.mDataArray;
            aOther.mDataArray = nullptr;
            break;
        case TYPE_OBJECT:
            mDataObject = aOther.mDataObject;
            aOther.mDataObject = nullptr;
            break;
        default:
            throw std::runtime_error("Json::Value : Unknown type");
            break;
        }

        mAllocator = aOther.mAllocator;
        return *this;
    }


    Value& Value::operator=(const TypeID aType){
        if(mType == aType) return *this;

        switch(mType){
        case TYPE_STRING:
            mAllocator->Deallocate<String>(mDataString);
            mDataString = nullptr;
            break;
        case TYPE_ARRAY:
            mAllocator->Deallocate<ArrayType>(mDataArray);
            mDataArray = nullptr;
            break;
        case TYPE_OBJECT:
            mAllocator->Deallocate<ObjectType>(mDataObject);
            mDataObject = nullptr;
            break;
        default:
            break;
        }

        switch(aType){
        case TYPE_STRING:
            mDataString = new(mAllocator->AllocateAndRegister<String>()) String(*mAllocator);
            break;
        case TYPE_ARRAY:
            mDataArray = new(mAllocator->AllocateAndRegister<ArrayType>()) ArrayType(*mAllocator, 9);
            break;
        case TYPE_OBJECT:
            mDataObject = new(mAllocator->AllocateAndRegister<ObjectType>()) ObjectType(*mAllocator, 8);
            break;
        default:
            break;
        }

        mType = aType;
        return *this;
    }

    Value::~Value(){
        operator=(TYPE_NULL);
    }

}}

#endif
