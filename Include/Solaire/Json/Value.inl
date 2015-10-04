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
        }
    }

    BoolValue& BoolValue::operator=(const bool aValue){

    }


    // NumberValue

    NumberValue::NumberValue(Value* aParent){

    }

    NumberValue::operator uint8_t() const{

    }

    NumberValue::operator uint16_t() const{

    }

    NumberValue::operator uint32_t() const{

    }

    NumberValue::operator uint64_t() const{

    }

    NumberValue::operator int8_t() const{

    }

    NumberValue::operator int16_t() const{

    }

    NumberValue::operator int32_t() const{

    }

    NumberValue::operator int64_t() const{

    }

    NumberValue::operator float() const{

    }

    NumberValue::operator double() const{

    }

    NumberValue& NumberValue::operator=(const uint8_t aValue){

    }

    NumberValue& NumberValue::operator=(const uint16_t aValue){

    }

    NumberValue& NumberValue::operator=(const uint32_t aValue){

    }

    NumberValue& NumberValue::operator=(const uint64_t aValue){

    }

    NumberValue& NumberValue::operator=(const int8_t aValue){

    }

    NumberValue& NumberValue::operator=(const int16_t aValue){

    }

    NumberValue& NumberValue::operator=(const int32_t aValue){

    }

    NumberValue& NumberValue::operator=(const int64_t aValue){

    }

    NumberValue& NumberValue::operator=(const float aValue){

    }

    NumberValue& NumberValue::operator=(const double aValue){

    }


    // StringValue

    StringValue::StringValue(Value* aParent):
        mParent(aParent)
    {}

    StringValue::operator char() const{

    }

    StringValue::operator ConstStringFragment() const{

    }

    StringValue& StringValue::operator=(const char aValue){

    }

    StringValue& StringValue::operator=(const ConstStringFragment aValue){

    }


    // ArrayValue

    ArrayValue::ArrayValue(Value* aParent):
        mParent(aParent)
    {}

    size_t ArrayValue::Size() const{

    }

    Value& ArrayValue::operator[](const size_t aIndex){

    }

    const Value& ArrayValue::operator[](const size_t aIndex) const{

    }

    Value& ArrayValue::PushBack(std::shared_ptr<Value> aValue){

    }

    void ArrayValue::Erase(const size_t aIndex){

    }

    void ArrayValue::Erase(const Value& aValue){

    }


    // ObjectValue

    ObjectValue::ObjectValue(Value* aParent):
        mParent(aParent)
    {}

    size_t ObjectValue::Size() const{

    }

    Value& ObjectValue::operator[](const ConstStringFragment aName){

    }

    const Value& ObjectValue::operator[](const ConstStringFragment aName) const{

    }

    bool ObjectValue::Contains(const ConstStringFragment aName) const{

    }

    Value& ObjectValue::Add(const ConstStringFragment aName, std::shared_ptr<Value> aValue){

    }

    void ObjectValue::Erase(const ConstStringFragment aName){

    }

    void ObjectValue::Erase(const Value& aValue){

    }


    // Value

    bool Value::Serialise(const  std::function<char(void)> aGetFn,  std::function<void(void)> aForwardFn,  std::function<void(void)> aBackwardFn, const Value& aValue){

    }

    std::shared_ptr<Value> Value::Deserialise(const  std::function<void(char)> aSetFn,  std::function<void(void)> aForwardFn,  std::function<void(void)> aBackwardFn, const Value& aValue){

    }

    size_t Value::EstimateSerialLength(const Value& aValue){

    }

    Value::Value(Value&& aOther){

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

    }

    Value& oValue::perator=(Value&& aOther){

    }

    NullValue& Value::AsNull(){

    }

    BoolValue& Value::AsBool(){

    }

    NumberValue& Value::AsNumber(){

    }

    StringValue& Value::AsString(){

    }

    ArrayValue& Value::AsArray(){

    }

    ObjectValue& Value::AsObject(){

    }

    const NullValue& Value::AsNull() const{

    }

    const BoolValue& Value::AsBool() const{

    }

    const NumberValue& Value::AsNumber() const{

    }

    const StringValue& Value::AsString() const{

    }

    const ArrayValue& Value::AsArray() const{

    }

    const ObjectValue& Value::AsObject() const{

    }

    Allocator& Value::GetAllocator() const{

    }

}}

#endif
