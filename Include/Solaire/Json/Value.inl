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
#include "..\Core\Strings\String.hpp"
#include "..\Core\Strings\NumberParser.hpp"
#include "..\Core\Iterators\DereferenceIterator.hpp"

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
            return *(*mParent->mDataArray)[0];
        case TYPE_OBJECT:
            return *(*mParent->mDataObject)[0].second;
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
            mParent->mDataObject->Erase(mParent->mDataObject->FindFirst([=](const std::pair<String, std::shared_ptr<Value>>& i){
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
        return mParent->mDataObject->FindFirst([=](const std::pair<String, std::shared_ptr<Value>>& i){
            return i.first == aName;
        });
    }

    DynamicArray<std::pair<String, std::shared_ptr<Value>>>::ConstIterator ObjectValue::SearchByObjectValue(const Value& aValue) const{
        std::shared_ptr<Value> value = const_cast<Value&>(aValue).shared_from_this();
        return mParent->mDataObject->FindFirst([=](const std::pair<String, std::shared_ptr<Value>>& i){
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
            return *mParent->mDataObject->PushBack(std::pair<String, std::shared_ptr<Value>>(String(aName, mParent->GetAllocator()), aValue)).second;
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


    // Value

    bool Value::Serialise(const std::function<void(char)>& aSetFn, std::function<void(void)>& aForwardFn, std::function<void(void)>& aBackwardFn, const Value& aValue){
        const auto WriteString = [&](ConstStringFragment aString)->void{
            for(const char c : aString){
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
        case TYPE_ARRAY:
            {
                aSetFn('[');
                aForwardFn();
                ArrayType::Iterator end = aValue.mDataArray->end();
                for(ArrayType::Iterator i = aValue.mDataArray->begin(); i != end; ++i){
                    if(! Serialise(aSetFn, aForwardFn, aBackwardFn, **i)) return false;
                    if((i - 1) != end){
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
                    if(! Serialise(aSetFn, aForwardFn, aBackwardFn, *i->second)) return false;
                    if((i - 1) != end){
                        aSetFn(',');
                        aForwardFn();
                    }
                }
                aSetFn('}');
                aForwardFn();
                return false;
            }
        default:
            return false;
        }
    }

    std::shared_ptr<Value> Value::Deserialise(const std::function<char(void)>& aGetFn, std::function<void(void)>& aForwardFn, std::function<void(void)>& aBackwardFn, const Value& aValue){
        throw 0;
    }

    size_t Value::EstimateSerialLength(const Value& aValue){
        throw 0;
    }

    /*Value::Value(Value&& aOther):
        mAllocator(aOther.mAllocator),
        mType(aOther.mType)
    {
        switch(mType){

        }
    }

    Value::Value(Allocator& aAllocator){

    }

    Value::Value(Allocator& aAllocator, const TypeID aType){

    }

    Value::Value(Allocator& aAllocator, const bool aValue){

    }

    Value::Value(Allocator& aAllocator, const uint8_t aValue){

    }

    Value::Value(Allocator& aAllocator, const uint16_t aValue){

    }

    Value::Value(Allocator& aAllocator, const uint32_t aValue){

    }

    Value(Allocator& aAllocator, const uint64_t aValue){

    }

    Value::Value(Allocator& aAllocator, const int8_t aValue){

    }

    Value::Value(Allocator& aAllocator, const int16_t aValue){

    }

    Value::Value(Allocator& aAllocator, const int32_t aValue){

    }

    Value::Value(Allocator& aAllocator, const int64_t aValue){

    }

    Value::Value(Allocator& aAllocator, const float aValue){

    }

    Value::Value(Allocator& aAllocator, const double aValue){

    }

    Value::Value(Allocator& aAllocator, const ConstStringFragment aValue){

    }*/

    Value& Value::operator=(Value&& aOther){
        operator=(aOther.mType);
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
            mDataArray = new(mAllocator->AllocateAndRegister<ArrayType>()) ArrayType(8, *mAllocator);
            break;
        case TYPE_OBJECT:
            mDataObject = new(mAllocator->AllocateAndRegister<ObjectType>()) ObjectType(8, *mAllocator);
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

    TypeID Value::GetType() const{
        return mType;
    }

    NullValue& Value::AsNull(){
        return mValueNull;
    }

    BoolValue& Value::AsBool(){
        return mValueBool;
    }

    NumberValue& Value::AsNumber(){
        return mValueNumber;
    }

    StringValue& Value::AsString(){
        return mValueString;
    }

    ArrayValue& Value::AsArray(){
        return mValueArray;
    }

    ObjectValue& Value::AsObject(){
        return mValueObject;
    }

    const NullValue& Value::AsNull() const{
        return mValueNull;
    }

    const BoolValue& Value::AsBool() const{
        return mValueBool;
    }

    const NumberValue& Value::AsNumber() const{
        return mValueNumber;
    }

    const StringValue& Value::AsString() const{
        return mValueString;
    }

    const ArrayValue& Value::AsArray() const{
        return mValueArray;
    }

    const ObjectValue& Value::AsObject() const{
        return mValueObject;
    }

    Allocator& Value::GetAllocator() const{
        return *mAllocator;
    }

}}

#endif
