#ifndef SOLAIRE_JSON_SERIALISATION_HPP
#define SOLAIRE_JSON_SERIALISATION_HPP

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
	\file Serialisation.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 30th September 2015
	Last Modified	: 30th September 2015
*/

#include "Parse.hpp"
#include "..\Core\Serialise\SerialisationInterface.hpp"

namespace Solaire{ namespace Json{

    class JsonSerialSystem;
    class JsonSerialObject;

    class JsonSerialArray : public SerialArray{
    private:
        Allocator::SharedPointer<Value> mValue;

        template<class F1, class F2>
        void WriteFn(const SerialIndex aIndex, F1 aNewFn, F2 aAssignFn){
            const SerialIndex size = mValue->GetArray().Size();
            if(aIndex == size){
                mValue->GetArray().PushBack(aNewFn());
            }else if(aIndex < size){
                aAssignFn();
            }else{
                Allocator& allocator = mValue->GetAllocator();
                for(SerialIndex i = size; i <= aIndex; ++i){
                    Allocator::SharedPointer<Value> val = allocator.SharedAllocate<Value>(TYPE_NULL, allocator);
                    mValue->GetArray().PushBack(val);
                }
                aAssignFn();
            }
        }

        template<class T>
        void WriteCopy(const SerialIndex aIndex, const T aValue, Value&(Value::*SetFn)(const T)){
            WriteFn(
                aIndex,
                [&](){
                    Allocator& allocator = mValue->GetAllocator();
                    return allocator.SharedAllocate<Value>(aValue, allocator);
                },
                [&](){
                    Value* ptr = mValue->GetArray()[aIndex].operator->();
                    (ptr->*SetFn)(aValue);
                }
            );
        }

        template<class T>
        void WriteMove(const SerialIndex aIndex, T&& aValue, Value&(Value::*SetFn)(T&&)){
            WriteFn(
                aIndex,
                [&](){
                    Allocator& allocator = mValue->GetAllocator();
                    return allocator.SharedAllocate<Value>(std::move(aValue), allocator);
                },
                [&](){
                    Value* ptr = mValue->GetArray()[aIndex].operator->();
                    (ptr->*SetFn)(std::move(aValue));
                }
            );
        }


    public:
        friend JsonSerialObject;
        friend JsonSerialSystem;

        JsonSerialArray(Allocator::SharedPointer<Value> aValue) :
            mValue(aValue)
        {}

        ~JsonSerialArray(){

        }

        // Inherited from SerialArray

        SerialIndex Size() const override{
            return mValue->GetArray().Size();
        }

        SerialType TypeOf(const SerialIndex aIndex) const override{
            switch(mValue->GetArray()[aIndex]->GetType()){
            case TYPE_NULL:
                return SERIAL_TYPE_N;
            case TYPE_BOOL:
                return SERIAL_TYPE_B;
            case TYPE_NUMBER:
                return SERIAL_TYPE_D;
            case TYPE_STRING:
                return SERIAL_TYPE_S;
            case TYPE_ARRAY:
                return SERIAL_TYPE_A;
            case TYPE_OBJECT:
                return SERIAL_TYPE_O;
            default:
                throw std::runtime_error("Json::SerialObject : Unknown type");
            }
        }
    protected:
        // Inherited from SerialArray

        bool ReadB(const SerialIndex aIndex) const override{
            return mValue->GetArray()[aIndex]->GetBool();
        }

        char ReadC(const SerialIndex aIndex) const override{
            return mValue->GetArray()[aIndex]->GetString()[0];
        }

        uint8_t ReadU8(const SerialIndex aIndex) const override{
            return static_cast<uint8_t>(mValue->GetArray()[aIndex]->GetNumber());
        }

        uint16_t ReadU16(const SerialIndex aIndex) const override{
            return static_cast<uint16_t>(mValue->GetArray()[aIndex]->GetNumber());
        }

        uint32_t ReadU32(const SerialIndex aIndex) const override{
            return static_cast<uint32_t>(mValue->GetArray()[aIndex]->GetNumber());
        }

        uint64_t ReadU64(const SerialIndex aIndex) const override{
            return static_cast<uint64_t>(mValue->GetArray()[aIndex]->GetNumber());
        }

        int8_t ReadI8(const SerialIndex aIndex) const override{
            return static_cast<int8_t>(mValue->GetArray()[aIndex]->GetNumber());
        }

        int16_t ReadI16(const SerialIndex aIndex) const override{
            return static_cast<int16_t>(mValue->GetArray()[aIndex]->GetNumber());
        }

        int32_t ReadI32(const SerialIndex aIndex) const override{
            return static_cast<int32_t>(mValue->GetArray()[aIndex]->GetNumber());
        }

        int64_t ReadI64(const SerialIndex aIndex) const override{
            return static_cast<int64_t>(mValue->GetArray()[aIndex]->GetNumber());
        }

        float ReadF(const SerialIndex aIndex) const override{
            return static_cast<float>(mValue->GetArray()[aIndex]->GetNumber());
        }

        double ReadD(const SerialIndex aIndex) const override{
            return static_cast<double>(mValue->GetArray()[aIndex]->GetNumber());
        }

        SerialString ReadS(const SerialIndex aIndex) const override{
            return mValue->GetArray()[aIndex]->GetString();
        }

        SerialArrayPtr ReadA(const SerialIndex aIndex) override{
            Allocator& allocator = mValue->GetAllocator();
            return allocator.SharedAllocate<JsonSerialArray>(mValue->GetArray()[aIndex]);
        }

        SerialObjectPtr ReadO(const SerialIndex aIndex) override;

        ConstSerialArrayPtr ReadA(const SerialIndex aIndex) const override{
            Allocator& allocator = mValue->GetAllocator();
            return allocator.SharedAllocate<JsonSerialArray>(mValue->GetArray()[aIndex]);
        }

        ConstSerialObjectPtr ReadO(const SerialIndex aIndex) const override;

        void WriteN(const SerialIndex aIndex) override{
            WriteFn(
                aIndex,
                [&](){
                    Allocator& allocator = mValue->GetAllocator();
                    return allocator.SharedAllocate<Value>(TYPE_NULL, allocator);
                },
                [&](){
                    mValue->GetArray()[aIndex]->SetNull();
                }
            );
        }

        void WriteB(const SerialIndex aIndex, const bool aValue) override{
            WriteCopy<Bool>(aIndex, aValue, &Value::SetBool);
        }

        void WriteC(const SerialIndex aIndex, const char aValue) override{
            WriteS(aIndex, SerialString(&aValue, &aValue + 1));
        }

        void WriteU8(const SerialIndex aIndex, const uint8_t aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteU16(const SerialIndex aIndex, const uint16_t aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteU32(const SerialIndex aIndex, const uint32_t aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteU64(const SerialIndex aIndex, const uint64_t aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteI8(const SerialIndex aIndex, const int8_t aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteI16(const SerialIndex aIndex, const int16_t aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteI32(const SerialIndex aIndex, const int32_t aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteI64(const SerialIndex aIndex, const int64_t aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteF(const SerialIndex aIndex, const float aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteD(const SerialIndex aIndex, const double aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteS(const SerialIndex aIndex, const SerialString aValue) override{
            WriteCopy<const ConstStringFragment>(aIndex, aValue, &Value::SetString);
        }

        void WriteA(const SerialIndex aIndex, SerialArrayPtr aArray) override{
            Allocator::SharedPointer<Value> value = std::static_pointer_cast<JsonSerialArray>(aArray)->mValue;
            WriteMove<Array&&>(aIndex, std::move(value->GetArray()), &Value::SetArray);
        }

        void WriteO(const SerialIndex aIndex, SerialObjectPtr aObject) override;
    };

    class JsonSerialObject : public SerialObject{
    private:
        Allocator::SharedPointer<Value> mValue;

        template<class F1, class F2>
        void WriteFn(const SerialTag aTag, F1 aNewFn, F2 aAssignFn){
            auto it = mValue->GetObject().find(aTag);
            if(it == mValue->GetObject().end()){
                aAssignFn(*it->second);
            }else{
                mValue->GetObject().emplace(aTag, aNewFn());
            }
        }

        template<class T>
        void WriteCopy(const SerialTag aTag, const T aValue, Value&(Value::*SetFn)(const T)){
            WriteFn(
                aTag,
                [&](){
                    Allocator& allocator = mValue->GetAllocator();
                    return allocator.SharedAllocate<Value>(aValue, allocator);
                },
                [&](Value& aVal){
                    (aVal.*SetFn)(aValue);
                }
            );
    }

        template<class T>
        void WriteMove(const SerialTag aTag, T&& aValue, Value&(Value::*SetFn)(T&&)){
            WriteFn(
                aTag,
                [&](){
                    Allocator& allocator = mValue->GetAllocator();
                    return allocator.SharedAllocate<Value>(std::move(aValue), allocator);
                },
                [&](Value& aVal){
                    (aVal.*SetFn)(std::move(aValue));
                }
            );
        }


    public:
        friend JsonSerialArray;
        friend JsonSerialSystem;

        JsonSerialObject(Allocator::SharedPointer<Value> aValue) :
            mValue(aValue)
        {}

        ~JsonSerialObject(){

        }

        // Inherited from SerialObject

        SerialType TypeOf(const SerialTag aTag) const override{
            switch(mValue->GetObject()[aTag]->GetType()){
            case TYPE_NULL:
                return SERIAL_TYPE_N;
            case TYPE_BOOL:
                return SERIAL_TYPE_B;
            case TYPE_NUMBER:
                return SERIAL_TYPE_D;
            case TYPE_STRING:
                return SERIAL_TYPE_S;
            case TYPE_ARRAY:
                return SERIAL_TYPE_A;
            case TYPE_OBJECT:
                return SERIAL_TYPE_O;
            default:
                throw std::runtime_error("Json::SerialObject : Unknown type");
            }
        }
    protected:
        // Inherited from SerialArray

        bool ReadB(const SerialTag aTag) const override{
            return mValue->GetObject()[aTag]->GetBool();
        }

        char ReadC(const SerialTag aTag) const override{
            return mValue->GetObject()[aTag]->GetString()[0];
        }

        uint8_t ReadU8(const SerialTag aTag) const override{
            return static_cast<uint8_t>(mValue->GetObject()[aTag]->GetNumber());
        }

        uint16_t ReadU16(const SerialTag aTag) const override{
            return static_cast<uint16_t>(mValue->GetObject()[aTag]->GetNumber());
        }

        uint32_t ReadU32(const SerialTag aTag) const override{
            return static_cast<uint32_t>(mValue->GetObject()[aTag]->GetNumber());
        }

        uint64_t ReadU64(const SerialTag aTag) const override{
            return static_cast<uint64_t>(mValue->GetObject()[aTag]->GetNumber());
        }

        int8_t ReadI8(const SerialTag aTag) const override{
            return static_cast<int8_t>(mValue->GetObject()[aTag]->GetNumber());
        }

        int16_t ReadI16(const SerialTag aTag) const override{
            return static_cast<int16_t>(mValue->GetObject()[aTag]->GetNumber());
        }

        int32_t ReadI32(const SerialTag aTag) const override{
            return static_cast<int32_t>(mValue->GetObject()[aTag]->GetNumber());
        }

        int64_t ReadI64(const SerialTag aTag) const override{
            return static_cast<int64_t>(mValue->GetObject()[aTag]->GetNumber());
        }

        float ReadF(const SerialTag aTag) const override{
            return static_cast<float>(mValue->GetObject()[aTag]->GetNumber());
        }

        double ReadD(const SerialTag aTag) const override{
            return static_cast<double>(mValue->GetObject()[aTag]->GetNumber());
        }

        SerialString ReadS(const SerialTag aTag) const override{
            return mValue->GetObject()[aTag]->GetString();
        }

        SerialArrayPtr ReadA(const SerialTag aTag) override{
            Allocator& allocator = mValue->GetAllocator();
            return allocator.SharedAllocate<JsonSerialArray>(mValue->GetObject()[aTag]);
        }

        SerialObjectPtr ReadO(const SerialTag aTag) override{
            Allocator& allocator = mValue->GetAllocator();
            return allocator.SharedAllocate<JsonSerialObject>(mValue->GetObject()[aTag]);
        }

        ConstSerialArrayPtr ReadA(const SerialTag aTag) const override{
            Allocator& allocator = mValue->GetAllocator();
            return allocator.SharedAllocate<JsonSerialArray>(mValue->GetObject()[aTag]);
        }

        ConstSerialObjectPtr ReadO(const SerialTag aTag) const override{
            Allocator& allocator = mValue->GetAllocator();
            return allocator.SharedAllocate<JsonSerialObject>(mValue->GetObject()[aTag]);
        }

        void WriteN(const SerialTag aTag) override{
            WriteFn(
                aTag,
                [&](){
                    Allocator& allocator = mValue->GetAllocator();
                    return allocator.SharedAllocate<Value>(TYPE_NULL, allocator);
                },
                [&](Value& aValue){
                    aValue.SetNull();
                }
            );
        }

        void WriteB(const SerialTag aTag, const bool aValue) override{
            WriteCopy<Bool>(aTag, aValue, &Value::SetBool);
        }

        void WriteC(const SerialTag aTag, const char aValue) override{
            WriteS(aTag, SerialString(&aValue, &aValue + 1));
        }

        void WriteU8(const SerialTag aTag, const uint8_t aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteU16(const SerialTag aTag, const uint16_t aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteU32(const SerialTag aTag, const uint32_t aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteU64(const SerialTag aTag, const uint64_t aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteI8(const SerialTag aTag, const int8_t aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteI16(const SerialTag aTag, const int16_t aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteI32(const SerialTag aTag, const int32_t aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteI64(const SerialTag aTag, const int64_t aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteF(const SerialTag aTag, const float aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteD(const SerialTag aTag, const double aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteS(const SerialTag aTag, const SerialString aValue) override{
            WriteCopy<const ConstStringFragment>(aTag, aValue, &Value::SetString);
        }

        void WriteA(const SerialTag aTag, SerialArrayPtr aArray) override{
            Allocator::SharedPointer<JsonSerialArray> ptr = std::static_pointer_cast<JsonSerialArray>(aArray);
            WriteMove<Array&&>(aTag, std::move(ptr->mValue->GetArray()), &Value::SetArray);
        }

        void WriteO(const SerialTag aTag, SerialObjectPtr aObject) override{
            Allocator::SharedPointer<JsonSerialObject> ptr = std::static_pointer_cast<JsonSerialObject>(aObject);
            WriteMove<Object&&>(aTag, std::move(ptr->mValue->GetObject()), &Value::SetObject);
        }
    };

    SerialObjectPtr JsonSerialArray::ReadO(const SerialIndex aIndex){
        Allocator& allocator = mValue->GetAllocator();
        return allocator.SharedAllocate<JsonSerialObject>(mValue->GetArray()[aIndex]);
    }

    ConstSerialObjectPtr JsonSerialArray::ReadO(const SerialIndex aIndex) const{
        Allocator& allocator = mValue->GetAllocator();
        return allocator.SharedAllocate<JsonSerialObject>((mValue->GetArray())[aIndex]);
    }

    void JsonSerialArray::WriteO(const SerialIndex aIndex, SerialObjectPtr aObject){
        Allocator::SharedPointer<JsonSerialObject> ptr = std::static_pointer_cast<JsonSerialObject>(aObject);
        WriteMove<Object&&>(aIndex, std::move(ptr->mValue->GetObject()), &Value::SetObject);
    }

    class JsonSerialSystem : public SerialSystem{
    private:
        Allocator& mParseAllocator;
        Allocator& mDataAllocator;
    public:
        JsonSerialSystem(Allocator& aParseAllocator, Allocator& aDataAllocator) :
            mParseAllocator(aParseAllocator),
            mDataAllocator(aDataAllocator)
        {}

        ~JsonSerialSystem(){

        }

        // Inherited from SerialSystem

        SerialArrayPtr CreateA() const override{
            return mParseAllocator.SharedAllocate<JsonSerialArray>(mParseAllocator.SharedAllocate<Value>(TYPE_ARRAY, mParseAllocator));
        }

        SerialObjectPtr CreateO() const override{
            return mParseAllocator.SharedAllocate<JsonSerialObject>(mParseAllocator.SharedAllocate<Value>(TYPE_OBJECT, mParseAllocator));
        }

        void WriteA(std::ostream& aStream, const ConstSerialArrayPtr aArray) const override{
            Allocator::SharedPointer<Value> ptr = std::static_pointer_cast<const JsonSerialArray>(aArray)->mValue;
            aStream << ptr->Parse(mParseAllocator);
        }

        void WriteO(std::ostream& aStream, const ConstSerialObjectPtr aObject) const override{
            Allocator::SharedPointer<Value> ptr = std::static_pointer_cast<const JsonSerialObject>(aObject)->mValue;
            aStream << ptr->Parse(mParseAllocator);
        }

        SerialArrayPtr ReadA(std::istream& aStream) const override{
            ArrayParser parser(mParseAllocator);
            aStream >> parser;
            Allocator::SharedPointer<Value> valuePtr = parser.Get(mParseAllocator, mParseAllocator);
            return mParseAllocator.SharedAllocate<JsonSerialArray>(valuePtr);

        }

        SerialObjectPtr ReadO(std::istream& aStream) const override{
            ObjectParser parser(mParseAllocator);
            aStream >> parser;
            Allocator::SharedPointer<Value> valuePtr = parser.Get(mParseAllocator, mParseAllocator);
            return mParseAllocator.SharedAllocate<JsonSerialObject>(valuePtr);
        }

        Allocator& GetParseAllocator() const override{
            return mParseAllocator;
        };

        Allocator& GetDataAllocator() const override{
            return mDataAllocator;
        };

    };


    template<class T>
    std::shared_ptr<Value> JsonSerialise(Allocator& aParseAllocator, Allocator& aDataAllocator, T aValue){
        JsonSerialSystem system(aParseAllocator, aDataAllocator);
        std::shared_ptr<JsonSerialObject> object = aParseAllocator.SharedAllocate<JsonSerialObject>(aParseAllocator.SharedAllocate<Value>(TYPE_ARRAY, aParseAllocator));

        Serialisable<T>::Serialise(aValue, system, "rpcSerialise", object);
        return object->mValue->GetObject()["rpcSerialise"];
    }

    template<class T>
    std::shared_ptr<T> JsonDeserialise(Allocator& aParseAllocator, Allocator& aDataAllocator, std::shared_ptr<Value> aValue){
        JsonSerialSystem system(aParseAllocator, aDataAllocator);
        std::shared_ptr<JsonSerialObject> object = aParseAllocator.SharedAllocate<JsonSerialObject>(aParseAllocator.SharedAllocate<Value>(TYPE_ARRAY, aParseAllocator));

        return object->mValue->GetObject().emplace("rpcDeserialise", aValue);
        return Serialisable<T>::Deserialise(object, "rpcDeserialise", aValue);
    }
}}


#endif
