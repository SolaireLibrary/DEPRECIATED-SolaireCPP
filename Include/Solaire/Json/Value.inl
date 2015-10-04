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
           return mParent->mData.bool_;
        case TYPE_NUMBER:
            return mParent->mData.double_ > 0.0;
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
        mParent->mData.bool_ = aValue;
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
           return mParent->mData.bool_ ? 1.0 : 0.0;
        case TYPE_NUMBER:
            return mParent->mData.double_;
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
        mParent->mData.double_ = aValue;
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
           return mParent->mData.bool_ ? '0' : '1';
        case TYPE_NUMBER:
            return static_cast<char>(mParent->mData.double_);
        case TYPE_STRING:
            return (*mParent->mData.string_)[0];
        default:
            throw std::runtime_error("Json::StringValue : Cannot convert value to char");
        }
    }

    StringValue::operator ConstStringFragment() const{
        //! \TODO Buffer temp string for non string types
        switch(mParent->mType){
        case TYPE_STRING:
            return *mParent->mData.string_;
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
        (*mParent->mData.string_).Clear();
        (*mParent->mData.string_) += aValue;
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
        (*mParent->mData.string_) = aValue;
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
            return mParent->mData.array_->Size();
        case TYPE_OBJECT:
            return mParent->mData.object_->Size();
            break;
        default:
            return 0;
        }
    }

    Value& ArrayValue::operator[](const size_t aIndex){
        switch(mParent->mType){
        case TYPE_ARRAY:
            return *(*mParent->mData.array_)[0];
        case TYPE_OBJECT:
            return *(*mParent->mData.object_)[0].second;
            break;
        default:
            throw std::runtime_error("Json::ArrayValue : Cannot access this type by index");
        }
    }

    const Value& ArrayValue::operator[](const size_t aIndex) const{
        switch(mParent->mType){
        case TYPE_ARRAY:
            return *(*mParent->mData.array_)[0];
        case TYPE_OBJECT:
            return *(*mParent->mData.object_)[0].second;
            break;
        default:
            throw std::runtime_error("Json::ArrayValue : Cannot access this type by index");
        }
    }

    Value& ArrayValue::PushBack(std::shared_ptr<Value> aValue){
        switch(mParent->mType){
        case TYPE_ARRAY:
            return *mParent->mData.array_->PushBack(aValue);
        default:
            throw std::runtime_error("Json::ArrayValue : Type is not array");
        }
    }

    void ArrayValue::Erase(const size_t aIndex){
        switch(mParent->mType){
        case TYPE_ARRAY:
            mParent->mData.array_->Erase(mParent->mData.array_->begin() + aIndex);
        case TYPE_OBJECT:
            mParent->mData.object_->Erase(mParent->mData.object_->begin() + aIndex);
            break;
        default:
            throw std::runtime_error("Json::ArrayValue : Cannot erase from this type by index");
        }
    }

    void ArrayValue::Erase(const Value& aValue){
        const std::shared_ptr<Value> value = const_cast<Value&>(aValue).shared_from_this();
        switch(mParent->mType){
        case TYPE_ARRAY:
            mParent->mData.array_->Erase(mParent->mData.array_->FindFirst(value));
        case TYPE_OBJECT:
            mParent->mData.object_->Erase(mParent->mData.object_->FindFirst([=](const std::pair<String, std::shared_ptr<Value>>& i){
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
        return mParent->mData.object_->FindFirst([=](const std::pair<String, std::shared_ptr<Value>>& i){
            return i.first == aName;
        });
    }

    DynamicArray<std::pair<String, std::shared_ptr<Value>>>::ConstIterator ObjectValue::SearchByObjectValue(const Value& aValue) const{
        std::shared_ptr<Value> value = const_cast<Value&>(aValue).shared_from_this();
        return mParent->mData.object_->FindFirst([=](const std::pair<String, std::shared_ptr<Value>>& i){
            return i.second == value;
        });
    }

    size_t ObjectValue::Size() const{
        switch(mParent->mType){
        case TYPE_OBJECT:
            return mParent->mData.object_->Size();
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
            return SearchByObjectName(aName) != mParent->mData.object_->end();
        default:
            throw std::runtime_error("Json::ObjectValue : Cannot search this type by name");
        }
    }

    Value& ObjectValue::Add(const ConstStringFragment aName, std::shared_ptr<Value> aValue){
        switch(mParent->mType){
        case TYPE_OBJECT:
            if(SearchByObjectName(aName) != mParent->mData.object_->end())  throw std::runtime_error("Json::ObjectValue : Already contains named value with this name");
            return *mParent->mData.object_->PushBack(std::pair<String, std::shared_ptr<Value>>(String(aName, mParent->GetAllocator()), aValue)).second;
            break;
        default:
            throw std::runtime_error("Json::ObjectValue : Cannot add named value to this type");
        }
    }

    void ObjectValue::Erase(const ConstStringFragment aName){
        switch(mParent->mType){
        case TYPE_OBJECT:
            mParent->mData.object_->Erase(SearchByObjectName(aName));
            break;
        default:
            throw std::runtime_error("Json::ObjectValue : Cannot erase from this type by name");
        }
    }

    void ObjectValue::Erase(const Value& aValue){
        switch(mParent->mType){
        case TYPE_ARRAY:
            mParent->mData.array_->Erase(mParent->mData.array_->FindFirst(const_cast<Value&>(aValue).shared_from_this()));
            break;
        case TYPE_OBJECT:
            mParent->mData.object_->Erase(SearchByObjectValue(aValue));
            break;
        default:
            throw std::runtime_error("Json::ObjectValue : Cannot erase from this type by value");
        }
    }


    // Value

    bool Value::Serialise(const  std::function<char(void)> aGetFn,  std::function<void(void)> aForwardFn,  std::function<void(void)> aBackwardFn, const Value& aValue){
        throw 0;
    }

    std::shared_ptr<Value> Value::Deserialise(const  std::function<void(char)> aSetFn,  std::function<void(void)> aForwardFn,  std::function<void(void)> aBackwardFn, const Value& aValue){
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
            mData.bool_ = aOther.mData.bool_;
            break;
        case TYPE_NUMBER:
            mData.double_ = aOther.mData.double_;
            break;
        case TYPE_STRING:
            mData.string_ = aOther.mData.string_;
            aOther.mData.string_ = nullptr;
            break;
        case TYPE_ARRAY:
            mData.array_ = aOther.mData.array_;
            aOther.mData.array_ = nullptr;
            break;
        case TYPE_OBJECT:
            mData.object_ = aOther.mData.object_;
            aOther.mData.object_ = nullptr;
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
            mAllocator->Deallocate<String>(mData.string_);
            mData.string_ = nullptr;
            break;
        case TYPE_ARRAY:
            mAllocator->Deallocate<ArrayType>(mData.array_);
            mData.array_ = nullptr;
            break;
        case TYPE_OBJECT:
            mAllocator->Deallocate<ObjectType>(mData.object_);
            mData.object_ = nullptr;
            break;
        default:
            break;
        }

        switch(aType){
        case TYPE_STRING:
            mData.string_ = new(mAllocator->AllocateAndRegister<String>()) String(*mAllocator);
            break;
        case TYPE_ARRAY:
            mData.array_ = new(mAllocator->AllocateAndRegister<ArrayType>()) ArrayType(8, *mAllocator);
            break;
        case TYPE_OBJECT:
            mData.object_ = new(mAllocator->AllocateAndRegister<ObjectType>()) ObjectType(8, *mAllocator);
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

    NullValue& Value::AsNull(){
        return mValue.null_;
    }

    BoolValue& Value::AsBool(){
        return mValue.bool_;
    }

    NumberValue& Value::AsNumber(){
        return mValue.double_;
    }

    StringValue& Value::AsString(){
        return mValue.string_;
    }

    ArrayValue& Value::AsArray(){
        return mValue.array_;
    }

    ObjectValue& Value::AsObject(){
        return mValue.object_;
    }

    const NullValue& Value::AsNull() const{
        return mValue.null_;
    }

    const BoolValue& Value::AsBool() const{
        return mValue.bool_;
    }

    const NumberValue& Value::AsNumber() const{
        return mValue.double_;
    }

    const StringValue& Value::AsString() const{
        return mValue.string_;
    }

    const ArrayValue& Value::AsArray() const{
        return mValue.array_;
    }

    const ObjectValue& Value::AsObject() const{
        return mValue.object_;
    }

    Allocator& Value::GetAllocator() const{
        return *mAllocator;
    }

}}

#endif
