#ifndef SOLAIRE_JSON_VALUE_HPP
#define SOLAIRE_JSON_VALUE_HPP

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
\file Value.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 2.0
\date
Created			: 29th September 2015
Last Modified	: 4th October 2015
*/

#include <Type_traits>
#include <map>
#include <list>
#include "..\Core\Strings\String.hpp"
#include "..\Core\Strings\NumberParser.hpp"
#include "..\Core\Iterators\DereferenceIterator.hpp"
#include "..\Core\Property.hpp"

namespace Solaire{ namespace Json{

    enum TypeID : uint8_t{
        TYPE_NULL,
        TYPE_BOOL,
        TYPE_NUMBER,
        TYPE_STRING,
        TYPE_ARRAY,
        TYPE_OBJECT
    };

    class Value;

    class NullValue{
    public:
        friend Value;
    private:
        NullValue(const NullValue&) = delete;
        NullValue(NullValue&&) = delete;
        NullValue& operator=(const NullValue&) = delete;
        NullValue& operator=(NullValue&&) = delete;

        NullValue(Value* aParent);
    private:
        Value* mParent;
    public:
    };

    class BoolValue{
    public:
        friend Value;
    private:
        BoolValue(const BoolValue&) = delete;
        BoolValue(BoolValue&&) = delete;
        BoolValue& operator=(const BoolValue&) = delete;
        BoolValue& operator=(BoolValue&&) = delete;

        BoolValue(Value* aParent);
    private:
        Value* mParent;
    public:
        operator bool() const;
        BoolValue& operator=(const bool aValue);
    };

    class NumberValue{
    public:
        friend Value;
    private:
        NumberValue(const NumberValue&) = delete;
        NumberValue(NumberValue&&) = delete;
        NumberValue& operator=(const NumberValue&) = delete;
        NumberValue& operator=(NumberValue&&) = delete;

        NumberValue(Value* aParent);
    private:
        Value* mParent;
    public:
        operator uint8_t() const;
        operator uint16_t() const;
        operator uint32_t() const;
        operator uint64_t() const;
        operator int8_t() const;
        operator int16_t() const;
        operator int32_t() const;
        operator int64_t() const;
        operator float() const;
        operator double() const;

        NumberValue& operator=(const uint8_t aValue);
        NumberValue& operator=(const uint16_t aValue);
        NumberValue& operator=(const uint32_t aValue);
        NumberValue& operator=(const uint64_t aValue);
        NumberValue& operator=(const int8_t aValue);
        NumberValue& operator=(const int16_t aValue);
        NumberValue& operator=(const int32_t aValue);
        NumberValue& operator=(const int64_t aValue);
        NumberValue& operator=(const float aValue);
        NumberValue& operator=(const double aValue);
    };

    class StringValue{
    public:
        friend Value;
    private:
        StringValue(const StringValue&) = delete;
        StringValue(StringValue&&) = delete;
        StringValue& operator=(const StringValue&) = delete;
        StringValue& operator=(StringValue&&) = delete;

        StringValue(Value* aParent);
    private:
        Value* mParent;
    public:
        operator char() const;
        operator ConstStringFragment() const;
        StringValue& operator=(const char aValue);
        StringValue& operator=(const ConstStringFragment aValue);
    };

    class ArrayValue{
    public:
        friend Value;
    private:
        ArrayValue(const ArrayValue&) = delete;
        ArrayValue(ArrayValue&&) = delete;
        ArrayValue& operator=(const ArrayValue&) = delete;
        ArrayValue& operator=(ArrayValue&&) = delete;

        ArrayValue(Value* aParent);
    private:
        Value* mParent;
    public:
        size_t Size() const;

        Value& operator[](const size_t aIndex);
        const Value& operator[](const size_t aIndex) const;

        Value& PushBack(std::shared_ptr<Value> aValue);
        void Erase(const size_t aIndex);
        void Erase(const Value& aValue);

        template<class T, typename Enable = typename std::enable_if<! std::is_same<T, std::shared_ptr<Value>>::value>::type>
        Value& PushBack(const ConstStringFragment aName, Allocator& aAllocator, T aValue){
            return PushBack(aName, aAllocator.SharedAllocate<Value>(aAllocator, aValue));
        }
    };

    class ObjectValue{
    public:
        friend Value;
    private:
        ObjectValue(const ObjectValue&) = delete;
        ObjectValue(ObjectValue&&) = delete;
        ObjectValue& operator=(const ObjectValue&) = delete;
        ObjectValue& operator=(ObjectValue&&) = delete;

        ObjectValue(Value* aParent);
    private:
        Value* mParent;
    private:
        DynamicArray<std::pair<String, std::shared_ptr<Value>>>::ConstIterator SearchByObjectName(const ConstStringFragment aName) const;
        DynamicArray<std::pair<String, std::shared_ptr<Value>>>::ConstIterator SearchByObjectValue(const Value& aValue) const;
    public:
        size_t Size() const;

        Value& operator[](const ConstStringFragment aName);
        const Value& operator[](const ConstStringFragment aName) const;

        bool Contains(const ConstStringFragment aName) const;
        Value& Add(const ConstStringFragment aName, std::shared_ptr<Value> aValue);

        void Erase(const ConstStringFragment aName);
        void Erase(const Value& aValue);

        template<class ...Params>
        Value& Add(const ConstStringFragment aName, Allocator& aAllocator, Params... aParams){
            return Add(aName, aAllocator.SharedAllocate<Value>(aAllocator, aParams...));
        }
    };

    class Value : public std::enable_shared_from_this<Value>{
    public:
        friend NullValue;
        friend BoolValue;
        friend NumberValue;
        friend StringValue;
        friend ArrayValue;
        friend ObjectValue;
    private:
        typedef DynamicArray<std::shared_ptr<Value>> ArrayType;
        typedef DynamicArray<std::pair<String, std::shared_ptr<Value>>> ObjectType;
    private:
        Value(const Value&) = delete;
        Value& operator=(const Value&) = delete;
    private:
        Allocator* mAllocator;
        union{
            bool mDataBool;
            double mDataNumber;
            String* mDataString;
            ArrayType* mDataArray;
            ObjectType* mDataObject;
        };
        union{
            NullValue mNull;
            BoolValue mBool;
            NumberValue mNumber;
            StringValue mString;
            ArrayValue mArray;
            ObjectValue mObject;
        };
        TypeID mType;
    private:
        static bool InternalSerialise(const std::function<void(char)>& aSetFn, const std::function<void(void)>& aForwardFn, const std::function<void(void)>& aBackwardFn, const Value& aValue);
        static std::shared_ptr<Value> InternalDeserialise(const std::function<char(void)>& aGetFn, const std::function<void(void)>& aForwardFn, const std::function<void(void)>& aBackwardFn, Allocator& aParseAllocator, Allocator& aDataAllocator);
    public:
        static size_t EstimateSerialLength(const Value& aValue);

        template<class Iterator>
        static bool Serialise(const Iterator aBegin, const Iterator aEnd, Iterator& aParseEnd, const Value& aValue){
            aParseEnd = aBegin;
            const std::function<void(char)> setFn = [&](char aChar)->void{*aParseEnd = aChar;};
            const std::function<void(void)> forwardFn = [&]()->void{++aParseEnd;};
            const std::function<void(void)> backwardFn = [&]()->void{--aParseEnd;};
            return InternalSerialise(
                setFn,
                forwardFn,
                backwardFn,
                aValue
            );
        }

        template<class Iterator>
        static std::shared_ptr<Value> Deserialise(const Iterator aBegin, const Iterator aEnd, Iterator& aParseEnd, Allocator& aParseAllocator, Allocator& aDataAllocator){
            aParseEnd = aBegin;
            const std::function<char(void)> getFn = [&]()->char{return *aParseEnd;};
            const std::function<void(void)> forwardFn = [&]()->void{++aParseEnd;};
            const std::function<void(void)> backwardFn = [&]()->void{--aParseEnd;};
            return InternalDeserialise(
                getFn,
                forwardFn,
                backwardFn,
                aParseAllocator,
                aDataAllocator
            );
        }
    public:
        Value(Value&& aOther);
        Value(Allocator& aAllocator);
        Value(Allocator& aAllocator, const TypeID aType);
        Value(Allocator& aAllocator, const bool aValue);
        Value(Allocator& aAllocator, const uint8_t aValue);
        Value(Allocator& aAllocator, const uint16_t aValue);
        Value(Allocator& aAllocator, const uint32_t aValue);
        Value(Allocator& aAllocator, const uint64_t aValue);
        Value(Allocator& aAllocator, const int8_t aValue);
        Value(Allocator& aAllocator, const int16_t aValue);
        Value(Allocator& aAllocator, const int32_t aValue);
        Value(Allocator& aAllocator, const int64_t aValue);
        Value(Allocator& aAllocator, const float aValue);
        Value(Allocator& aAllocator, const double aValue);
        Value(Allocator& aAllocator, const ConstStringFragment aValue);
        ~Value();

        Value& operator=(Value&& aOther);
        Value& operator=(const TypeID);

        union{
            ObjectProperty<Value, NullValue, &Value::mNull> pNull;
            ObjectProperty<Value, BoolValue, &Value::mBool> pBool;
            ObjectProperty<Value, NumberValue, &Value::mNumber> pNumber;
            ObjectProperty<Value, StringValue, &Value::mString> pString;
            ObjectProperty<Value, ArrayValue, &Value::mArray> pArray;
            ObjectProperty<Value, ObjectValue, &Value::mObject> pObject;
            ReadOnlyObjectProperty<Value, TypeID, &Value::mType> pType;
            DereferenceObjectProperty<Value, Allocator, &Value::mAllocator> pAllocator;
        };
    };
}}

#include "Value.inl"

#endif
