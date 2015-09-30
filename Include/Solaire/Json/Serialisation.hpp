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

#include <list>
#include "Parse.hpp"
#include "..\Core\Serialise\SerialisationInterface.hpp"

namespace Solaire{ namespace Json{

    class SerialSystem;
    class SerialObject;

    class SerialArray : public Core::SerialArray{
    private:
        Value& mValue;
        Array& mArray;

        template<class F1, class F2>
        void WriteFn(const Core::SerialIndex aIndex, F1 aNewFn, F2 aAssignFn){
            const Core::SerialIndex size = mArray.Size();
            if(aIndex == size){
                mArray.PushBack(aNewFn());
            }else if(aIndex < size){
                aAssignFn();
            }else{
                for(Core::SerialIndex i = size; i <= aIndex; ++i){
                    Core::SharedPointer<Value> val = SolaireSharedAllocate(mArray.GetAllocator(), Value, (TYPE_NULL, mArray.GetAllocator()));
                    mArray.PushBack(val);
                }
                aAssignFn();
            }
        }

        template<class T>
        void WriteCopy(const Core::SerialIndex aIndex, const T aValue, Value&(Value::*SetFn)(const T)){
            WriteFn(
                aIndex,
                [&](){
                    return SolaireSharedAllocate(mArray.GetAllocator(), Value, (aValue, mArray.GetAllocator()));
                },
                [&](){
                    Value* ptr = mArray[aIndex].operator->();
                    (ptr->*SetFn)(aValue);
                }
            );
        }

        template<class T>
        void WriteMove(const Core::SerialIndex aIndex, T&& aValue, Value&(Value::*SetFn)(T&&)){
            WriteFn(
                aIndex,
                [&](){
                    return SolaireSharedAllocate(mArray.GetAllocator(), Value, (std::move(aValue), mArray.GetAllocator()));
                },
                [&](){
                    Value* ptr = mArray[aIndex].operator->();
                    (ptr->*SetFn)(std::move(aValue));
                }
            );
        }


    public:
        friend SerialObject;
        friend SerialSystem;

        SerialArray(Value& aValue, Array& aArray) :
            mValue(aValue),
            mArray(aArray)
        {}

        ~SerialArray(){

        }

        // Inherited from SerialArray

        Core::SerialIndex Size() const override{
            return mArray.Size();
        }

        Core::SerialType TypeOf(const Core::SerialIndex aIndex) const override{
            switch(mArray[aIndex]->GetType()){
            case TYPE_NULL:
                return Core::SERIAL_TYPE_N;
            case TYPE_BOOL:
                return Core::SERIAL_TYPE_B;
            case TYPE_NUMBER:
                return Core::SERIAL_TYPE_D;
            case TYPE_STRING:
                return Core::SERIAL_TYPE_S;
            case TYPE_ARRAY:
                return Core::SERIAL_TYPE_A;
            case TYPE_OBJECT:
                return Core::SERIAL_TYPE_O;
            default:
                throw std::runtime_error("Json::SerialObject : Unknown type");
            }
        }

        bool ReadB(const Core::SerialIndex aIndex) const override{
            return mArray[aIndex]->GetBool();
        }

        char ReadC(const Core::SerialIndex aIndex) const override{
            return mArray[aIndex]->GetString()[0];
        }

        uint8_t ReadU8(const Core::SerialIndex aIndex) const override{
            return static_cast<uint8_t>(mArray[aIndex]->GetNumber());
        }

        uint16_t ReadU16(const Core::SerialIndex aIndex) const override{
            return static_cast<uint16_t>(mArray[aIndex]->GetNumber());
        }

        uint32_t ReadU32(const Core::SerialIndex aIndex) const override{
            return static_cast<uint32_t>(mArray[aIndex]->GetNumber());
        }

        uint64_t ReadU64(const Core::SerialIndex aIndex) const override{
            return static_cast<uint64_t>(mArray[aIndex]->GetNumber());
        }

        int8_t ReadI8(const Core::SerialIndex aIndex) const override{
            return static_cast<int8_t>(mArray[aIndex]->GetNumber());
        }

        int16_t ReadI16(const Core::SerialIndex aIndex) const override{
            return static_cast<int16_t>(mArray[aIndex]->GetNumber());
        }

        int32_t ReadI32(const Core::SerialIndex aIndex) const override{
            return static_cast<int32_t>(mArray[aIndex]->GetNumber());
        }

        int64_t ReadI64(const Core::SerialIndex aIndex) const override{
            return static_cast<int64_t>(mArray[aIndex]->GetNumber());
        }

        float ReadF(const Core::SerialIndex aIndex) const override{
            return static_cast<float>(mArray[aIndex]->GetNumber());
        }

        double ReadD(const Core::SerialIndex aIndex) const override{
            return static_cast<double>(mArray[aIndex]->GetNumber());
        }

        Core::SerialString ReadS(const Core::SerialIndex aIndex) const override{
            return mArray[aIndex]->GetString();
        }

        Core::SerialArrayPtr ReadA(const Core::SerialIndex aIndex) override{
            return SolaireSharedAllocate(mArray.GetAllocator(), SerialArray, (*mArray[aIndex], mArray[aIndex]->GetArray()));
        }

        Core::SerialObjectPtr ReadO(const Core::SerialIndex aIndex) override;

        Core::ConstSerialArrayPtr ReadA(const Core::SerialIndex aIndex) const override{
            return SolaireSharedAllocate(mArray.GetAllocator(), SerialArray, (*mArray[aIndex], mArray[aIndex]->GetArray()));
        }

        Core::ConstSerialObjectPtr ReadO(const Core::SerialIndex aIndex) const override;

        void WriteN(const Core::SerialIndex aIndex) override{
            WriteFn(
                aIndex,
                [&](){
                    return SolaireSharedAllocate(mArray.GetAllocator(), Value, (TYPE_NULL, mArray.GetAllocator()));
                },
                [&](){
                    mArray[aIndex]->SetNull();
                }
            );
        }

        void WriteB(const Core::SerialIndex aIndex, const bool aValue) override{
            WriteCopy<Bool>(aIndex, aValue, &Value::SetBool);
        }

        void WriteC(const Core::SerialIndex aIndex, const char aValue) override{
            WriteS(aIndex, Core::SerialString(&aValue, &aValue + 1));
        }

        void WriteU8(const Core::SerialIndex aIndex, const uint8_t aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteU16(const Core::SerialIndex aIndex, const uint16_t aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteU32(const Core::SerialIndex aIndex, const uint32_t aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteU64(const Core::SerialIndex aIndex, const uint64_t aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteI8(const Core::SerialIndex aIndex, const int8_t aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteI16(const Core::SerialIndex aIndex, const int16_t aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteI32(const Core::SerialIndex aIndex, const int32_t aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteI64(const Core::SerialIndex aIndex, const int64_t aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteF(const Core::SerialIndex aIndex, const float aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteD(const Core::SerialIndex aIndex, const double aValue) override{
            WriteCopy<Number>(aIndex, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteS(const Core::SerialIndex aIndex, const Core::SerialString aValue) override{
            WriteCopy<const Core::ConstStringFragment>(aIndex, aValue, &Value::SetString);
        }

        void WriteA(const Core::SerialIndex aIndex, Core::SerialArrayPtr aArray) override{
            SerialArray& ref = static_cast<SerialArray&>(*aArray);
            WriteMove<Array&&>(aIndex, std::move(ref.mArray), &Value::SetArray);
        }

        void WriteO(const Core::SerialIndex aIndex, Core::SerialObjectPtr aObject) override;
    };

    class SerialObject : public Core::SerialObject{
    private:
        Object& mObject;
        Value& mValue;

        template<class F1, class F2>
        void WriteFn(const Core::SerialTag aTag, F1 aNewFn, F2 aAssignFn){
            auto it = mObject.find(aTag);
            if(it == mObject.end()){
                aAssignFn(*it->second);
            }else{
                mObject.emplace(aTag, aNewFn());
            }
        }

        template<class T>
        void WriteCopy(const Core::SerialTag aTag, const T aValue, Value&(Value::*SetFn)(const T)){
            WriteFn(
                aTag,
                [&](){
                    return SolaireSharedAllocate(mValue.GetAllocator(), Value, (aValue, mValue.GetAllocator()));
                },
                [&](Value& aVal){
                    (aVal.*SetFn)(aValue);
                }
            );
        }

        template<class T>
        void WriteMove(const Core::SerialTag aTag, T&& aValue, Value&(Value::*SetFn)(T&&)){
            WriteFn(
                aTag,
                [&](){
                    return SolaireSharedAllocate(mValue.GetAllocator(), Value, (std::move(aValue), mValue.GetAllocator()));
                },
                [&](Value& aVal){
                    (aVal.*SetFn)(std::move(aValue));
                }
            );
        }


    public:
        friend SerialArray;
        friend SerialSystem;

        SerialObject(Value& aValue, Object& aObject) :
            mValue(aValue),
            mObject(aObject)
        {}

        ~SerialObject(){

        }

        // Inherited from SerialObject

        Core::SerialType TypeOf(const Core::SerialTag aTag) const override{
            switch(mObject[aTag]->GetType()){
            case TYPE_NULL:
                return Core::SERIAL_TYPE_N;
            case TYPE_BOOL:
                return Core::SERIAL_TYPE_B;
            case TYPE_NUMBER:
                return Core::SERIAL_TYPE_D;
            case TYPE_STRING:
                return Core::SERIAL_TYPE_S;
            case TYPE_ARRAY:
                return Core::SERIAL_TYPE_A;
            case TYPE_OBJECT:
                return Core::SERIAL_TYPE_O;
            default:
                throw std::runtime_error("Json::SerialObject : Unknown type");
            }
        }

        bool ReadB(const Core::SerialTag aTag) const override{
            return mObject[aTag]->GetBool();
        }

        char ReadC(const Core::SerialTag aTag) const override{
            return mObject[aTag]->GetString()[0];
        }

        uint8_t ReadU8(const Core::SerialTag aTag) const override{
            return static_cast<uint8_t>(mObject[aTag]->GetNumber());
        }

        uint16_t ReadU16(const Core::SerialTag aTag) const override{
            return static_cast<uint16_t>(mObject[aTag]->GetNumber());
        }

        uint32_t ReadU32(const Core::SerialTag aTag) const override{
            return static_cast<uint32_t>(mObject[aTag]->GetNumber());
        }

        uint64_t ReadU64(const Core::SerialTag aTag) const override{
            return static_cast<uint64_t>(mObject[aTag]->GetNumber());
        }

        int8_t ReadI8(const Core::SerialTag aTag) const override{
            return static_cast<int8_t>(mObject[aTag]->GetNumber());
        }

        int16_t ReadI16(const Core::SerialTag aTag) const override{
            return static_cast<int16_t>(mObject[aTag]->GetNumber());
        }

        int32_t ReadI32(const Core::SerialTag aTag) const override{
            return static_cast<int32_t>(mObject[aTag]->GetNumber());
        }

        int64_t ReadI64(const Core::SerialTag aTag) const override{
            return static_cast<int64_t>(mObject[aTag]->GetNumber());
        }

        float ReadF(const Core::SerialTag aTag) const override{
            return static_cast<float>(mObject[aTag]->GetNumber());
        }

        double ReadD(const Core::SerialTag aTag) const override{
            return static_cast<double>(mObject[aTag]->GetNumber());
        }

        Core::SerialString ReadS(const Core::SerialTag aTag) const override{
            return mObject[aTag]->GetString();
        }

        Core::SerialArrayPtr ReadA(const Core::SerialTag aTag) override{
            Value& val = *mObject[aTag];
            return SolaireSharedAllocate(mValue.GetAllocator(), SerialArray, (val, val.GetArray()));
        }

        Core::SerialObjectPtr ReadO(const Core::SerialTag aTag) override{
            Value& val = *mObject[aTag];
            return SolaireSharedAllocate(mValue.GetAllocator(), SerialObject, (val, val.GetObject()));
        }

        Core::ConstSerialArrayPtr ReadA(const Core::SerialTag aTag) const override{
            Value& val = *mObject[aTag];
            return SolaireSharedAllocate(mValue.GetAllocator(), SerialArray, (val, val.GetArray()));
        }

        Core::ConstSerialObjectPtr ReadO(const Core::SerialTag aTag) const override{
            Value& val = *mObject[aTag];
            return SolaireSharedAllocate(mValue.GetAllocator(), SerialObject, (val, val.GetObject()));
        }

        void WriteN(const Core::SerialTag aTag) override{
            WriteFn(
                aTag,
                [&](){
                    return SolaireSharedAllocate(mValue.GetAllocator(), Value, (TYPE_NULL, mValue.GetAllocator()));
                },
                [&](Value& aValue){
                    aValue.SetNull();
                }
            );
        }

        void WriteB(const Core::SerialTag aTag, const bool aValue) override{
            WriteCopy<Bool>(aTag, aValue, &Value::SetBool);
        }

        void WriteC(const Core::SerialTag aTag, const char aValue) override{
            WriteS(aTag, Core::SerialString(&aValue, &aValue + 1));
        }

        void WriteU8(const Core::SerialTag aTag, const uint8_t aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteU16(const Core::SerialTag aTag, const uint16_t aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteU32(const Core::SerialTag aTag, const uint32_t aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteU64(const Core::SerialTag aTag, const uint64_t aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteI8(const Core::SerialTag aTag, const int8_t aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteI16(const Core::SerialTag aTag, const int16_t aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteI32(const Core::SerialTag aTag, const int32_t aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteI64(const Core::SerialTag aTag, const int64_t aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteF(const Core::SerialTag aTag, const float aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteD(const Core::SerialTag aTag, const double aValue) override{
            WriteCopy<Number>(aTag, static_cast<Number>(aValue), &Value::SetNumber);
        }

        void WriteS(const Core::SerialTag aTag, const Core::SerialString aValue) override{
            WriteCopy<const Core::ConstStringFragment>(aTag, aValue, &Value::SetString);
        }

        void WriteA(const Core::SerialTag aTag, Core::SerialArrayPtr aArray) override{
            SerialArray& ref = static_cast<SerialArray&>(*aArray);
            WriteMove<Array&&>(aTag, std::move(ref.mArray), &Value::SetArray);
        }

        void WriteO(const Core::SerialTag aTag, Core::SerialObjectPtr aObject) override{
            SerialObject& ref = static_cast<SerialObject&>(*aObject);
            WriteMove<Object&&>(aTag, std::move(ref.mObject), &Value::SetObject);
        }
    };

    Core::SerialObjectPtr SerialArray::ReadO(const Core::SerialIndex aIndex){
        return SolaireSharedAllocate(mArray.GetAllocator(), SerialObject, (*mArray[aIndex], mArray[aIndex]->GetObject()));
    }

    Core::ConstSerialObjectPtr SerialArray::ReadO(const Core::SerialIndex aIndex) const{
        return SolaireSharedAllocate(mArray.GetAllocator(), SerialObject, (*mArray[aIndex], mArray[aIndex]->GetObject()));
    }

    void SerialArray::WriteO(const Core::SerialIndex aIndex, Core::SerialObjectPtr aObject){
        SerialObject& ref = static_cast<SerialObject&>(*aObject);
        WriteMove<Object&&>(aIndex, std::move(ref.mObject), &Value::SetObject);
    }

    class SerialSystem : public Core::SerialSystem{
    private:
        Core::Allocator& mAllocator;
        mutable std::list<Value*> mValueList;
    public:
        SerialSystem(Core::Allocator& aAllocator) :
            mAllocator(aAllocator),
            mValueList()
        {}

        ~SerialSystem(){
            for(Value* i : mValueList) mAllocator.Deallocate<Value>(i);
        }

        // Inherited from SerialSystem

        Core::SerialArrayPtr CreateA() const override{
            Value* value = new(mAllocator.Allocate<Value>()) Value(TYPE_ARRAY, mAllocator);
            mAllocator.RegisterDestructor<Value>(value);
            mValueList.push_back(value);
            return SolaireSharedAllocate(mAllocator, SerialArray, (*value, value->GetArray()));
        }

        Core::SerialObjectPtr CreateO() const override{
            Value* value = new(mAllocator.Allocate<Value>()) Value(TYPE_ARRAY, mAllocator);
            mAllocator.RegisterDestructor<Value>(value);
            mValueList.push_back(value);
            return SolaireSharedAllocate(mAllocator, SerialObject, (*value, value->GetObject()));
        }

        void WriteA(std::ostream& aStream, const Core::ConstSerialArrayPtr& aArray) const override{
            Value& val = reinterpret_cast<const SerialArray*>(aArray.operator->())->mValue;
            aStream << val.Parse(mAllocator);
        }

        void WriteO(std::ostream& aStream, const Core::ConstSerialObjectPtr& aObject) const override{
            Value& val = reinterpret_cast<const SerialObject*>(aObject.operator->())->mValue;
            aStream << val.Parse(mAllocator);
        }

        Core::SerialArrayPtr ReadA(std::istream& aStream) const override{
            ArrayParser parser(mAllocator);
            char c;
            while(! aStream.eof()){
                aStream >> c;
                if(! parser.Append(c)){
                    break;
                }
            }

            Value* value = parser.Get(mAllocator, mAllocator).Release();
            mAllocator.RegisterDestructor<Value>(value);
            mValueList.push_back(value);

            return SolaireSharedAllocate(mAllocator, SerialArray, (*value, value->GetArray()));

        }

        Core::SerialObjectPtr ReadO(std::istream& aStream) const override{
            ObjectParser parser(mAllocator);
            char c;
            while(! aStream.eof()){
                aStream >> c;
                if(! parser.Append(c)){
                    break;
                }
            }

            Value* value = parser.Get(mAllocator, mAllocator).Release();
            mAllocator.RegisterDestructor<Value>(value);
            mValueList.push_back(value);

            return SolaireSharedAllocate(mAllocator, SerialObject, (*value, value->GetObject()));
        }

    };
}}


#endif
