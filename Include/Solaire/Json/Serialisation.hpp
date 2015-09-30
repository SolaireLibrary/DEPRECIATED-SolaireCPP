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

    class SerialArray : public Core::SerialArray{
    private:
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
        SerialArray(Array& aArray) :
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
            return SolaireUniqueAllocate(mArray.GetAllocator(), SerialArray, (mArray[aIndex]->GetArray()));
        }

        Core::SerialObjectPtr ReadO(const Core::SerialIndex aIndex) override{
            //return SolaireUniqueAllocate(mArray.GetAllocator(), SerialObject, (mArray[aIndex]->GetObject()));
        }

        Core::ConstSerialArrayPtr ReadA(const Core::SerialIndex aIndex) const override{
            return SolaireUniqueAllocate(mArray.GetAllocator(), SerialArray, (mArray[aIndex]->GetArray()));
        }

        Core::ConstSerialObjectPtr ReadO(const Core::SerialIndex aIndex) const override{
            //return SolaireUniqueAllocate(mArray.GetAllocator(), SerialObject, (mArray[aIndex]->GetObject()));
        }

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

        void WriteO(const Core::SerialIndex aIndex, Core::SerialObjectPtr aObject) override{
            //SerialObject& ref = static_cast<SerialObject&>(*aObject);
            //WriteMove<Object&&>(aIndex, std::move(ref.mObject), &Value::SetObject);
        }

    };

    class SerialObject;/*{
    public:
        virtual ~SerialObject(){}

        virtual SerialType TypeOf(const SerialTag aTag) const = 0;

        virtual bool ReadB(const SerialTag aTag) const = 0;
        virtual char ReadC(const SerialTag aTag) const = 0;
        virtual uint8_t ReadU8(const SerialTag aTag) const = 0;
        virtual uint16_t ReadU16(const SerialTag aTag) const = 0;
        virtual uint32_t ReadU32(const SerialTag aTag) const = 0;
        virtual uint64_t ReadU64(const SerialTag aTag) const = 0;
        virtual int8_t ReadI8(const SerialTag aTag) const = 0;
        virtual int16_t ReadI16(const SerialTag aTag) const = 0;
        virtual int32_t ReadI32(const SerialTag aTag) const = 0;
        virtual int64_t ReadI64(const SerialTag aTag) const = 0;
        virtual float ReadF(const SerialTag aTag) const = 0;
        virtual double ReadD(const SerialTag aTag) const = 0;
        virtual SerialString ReadS(const SerialTag aTag) const = 0;

        virtual SerialArrayPtr ReadA(const SerialTag aTag) = 0;
        virtual SerialObjectPtr ReadO(const SerialTag aTag) = 0;
        virtual ConstSerialArrayPtr ReadA(const SerialTag aTag) const = 0;
        virtual ConstSerialObjectPtr ReadO(const SerialTag aTag) const = 0;

        virtual void WriteN(const SerialTag aTag) = 0;
        virtual void WriteB(const SerialTag aTag, const bool aValue) = 0;
        virtual void WriteC(const SerialTag aTag, const char aValue) = 0;
        virtual void WriteU8(const SerialTag aTag, const uint8_t aValue) = 0;
        virtual void WriteU16(const SerialTag aTag, const uint16_t aValue) = 0;
        virtual void WriteU32(const SerialTag aTag, const uint32_t aValue) = 0;
        virtual void WriteU64(const SerialTag aTag, const uint64_t aValue) = 0;
        virtual void WriteI8(const SerialTag aTag, const int8_t aValue) = 0;
        virtual void WriteI16(const SerialTag aTag, const int16_t aValue) = 0;
        virtual void WriteI32(const SerialTag aTag, const int32_t aValue) = 0;
        virtual void WriteI64(const SerialTag aTag, const int64_t aValue) = 0;
        virtual void WriteF(const SerialTag aTag, const float aValue) = 0;
        virtual void WriteD(const SerialTag aTag, const double aValue) = 0;
        virtual void WriteS(const SerialTag aTag, const SerialString aValue) = 0;

        virtual void WriteA(const SerialTag aTag, SerialArrayPtr) = 0;
        virtual void WriteO(const SerialTag aTag, SerialObjectPtr) = 0;
    };*/

    class SerialSystem : public Core::SerialSystem{
    private:
        Core::Allocator* mAllocator;
    public:
        SerialSystem(Core::Allocator& aAllocator) :
            mAllocator(&aAllocator)
        { }

        ~SerialSystem(){

        }

        // Inherited from SerialSystem

        Core::SerialArrayPtr CreateA() const override{
            //return SolaireUniqueAllocate(*mAllocator, SerialArray, (*mAllocator));
        }

        Core::SerialObjectPtr CreateO() const override{
            //return SolaireUniqueAllocate(*mAllocator, SerialObject, (*mAllocator));
        }

        void WriteA(std::ostream& aStream, const Core::ConstSerialArrayPtr& aArray) const override{
            throw 0;
        }

        void WriteO(std::ostream& aStream, const Core::ConstSerialObjectPtr& aArray) const override{
            throw 0;
        }

        Core::SerialArrayPtr ReadA(std::istream& aStream) const override{
            throw 0;
        }

        Core::SerialObjectPtr ReadO(std::istream& aStream) const override{
            throw 0;
        }

    };
}}


#endif
