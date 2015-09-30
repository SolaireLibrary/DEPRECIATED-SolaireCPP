#ifndef SOLAIRE_CORE_SERIALISATION_INTERFACE_HPP
#define SOLAIRE_CORE_SERIALISATION_INTERFACE_HPP

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
	\file SerialisationInterface.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 30th September 2015
	Last Modified	: 30th September 2015
*/

#include <cstdint>
#include "..\Strings\StringFragment.hpp"

namespace Solaire{ namespace Core{

    typedef Core::ConstStringFragment SerialTag;
    typedef Core::ConstStringFragment SerialString;
    typedef size_t SerialIndex;

    class SerialArray;
    class SerialObject;

    typedef std::shared_ptr<SerialArray> SerialArrayPtr;
    typedef std::shared_ptr<SerialObject> SerialObjectPtr;
    typedef std::shared_ptr<const SerialArray> ConstSerialArrayPtr;
    typedef std::shared_ptr<const SerialObject> ConstSerialObjectPtr;

    enum SerialType{
        SERIAL_TYPE_U8,
        SERIAL_TYPE_U16,
        SERIAL_TYPE_U32,
        SERIAL_TYPE_U64,
        SERIAL_TYPE_I8,
        SERIAL_TYPE_I16,
        SERIAL_TYPE_I32,
        SERIAL_TYPE_I64,
        SERIAL_TYPE_B,
        SERIAL_TYPE_C,
        SERIAL_TYPE_F,
        SERIAL_TYPE_D,
        SERIAL_TYPE_S,
        SERIAL_TYPE_A,
        SERIAL_TYPE_O,
        SERIAL_TYPE_N
    };

    class SerialSystem{
    public:
        virtual ~SerialSystem(){}

        virtual SerialArrayPtr CreateA() const = 0;
        virtual SerialObjectPtr CreateO() const = 0;

        virtual void WriteA(std::ostream& aStream, const ConstSerialArrayPtr aArray) const = 0;
        virtual void WriteO(std::ostream& aStream, const ConstSerialObjectPtr aArray) const = 0;

        virtual SerialArrayPtr ReadA(std::istream& aStream) const = 0;
        virtual SerialObjectPtr ReadO(std::istream& aStream) const = 0;

        virtual Allocator& GetParseAllocator() const = 0;
        virtual Allocator& GetDataAllocator() const = 0;
    };

    class SerialArray{
    public:
        virtual ~SerialArray(){}

        virtual SerialIndex Size() const = 0;
        virtual SerialType TypeOf(const SerialIndex aIndex) const = 0;

        virtual bool ReadB(const SerialIndex aIndex) const = 0;
        virtual char ReadC(const SerialIndex aIndex) const = 0;
        virtual uint8_t ReadU8(const SerialIndex aIndex) const = 0;
        virtual uint16_t ReadU16(const SerialIndex aIndex) const = 0;
        virtual uint32_t ReadU32(const SerialIndex aIndex) const = 0;
        virtual uint64_t ReadU64(const SerialIndex aIndex) const = 0;
        virtual int8_t ReadI8(const SerialIndex aIndex) const = 0;
        virtual int16_t ReadI16(const SerialIndex aIndex) const = 0;
        virtual int32_t ReadI32(const SerialIndex aIndex) const = 0;
        virtual int64_t ReadI64(const SerialIndex aIndex) const = 0;
        virtual float ReadF(const SerialIndex aIndex) const = 0;
        virtual double ReadD(const SerialIndex aIndex) const = 0;
        virtual SerialString ReadS(const SerialIndex aIndex) const = 0;

        virtual SerialArrayPtr ReadA(const SerialIndex aIndex) = 0;
        virtual SerialObjectPtr ReadO(const SerialIndex aIndex) = 0;
        virtual ConstSerialArrayPtr ReadA(const SerialIndex aIndex) const = 0;
        virtual ConstSerialObjectPtr ReadO(const SerialIndex aIndex) const = 0;

        virtual void WriteN(const SerialIndex aIndex) = 0;
        virtual void WriteB(const SerialIndex aIndex, const bool aValue) = 0;
        virtual void WriteC(const SerialIndex aIndex, const char aValue) = 0;
        virtual void WriteU8(const SerialIndex aIndex, const uint8_t aValue) = 0;
        virtual void WriteU16(const SerialIndex aIndex, const uint16_t aValue) = 0;
        virtual void WriteU32(const SerialIndex aIndex, const uint32_t aValue) = 0;
        virtual void WriteU64(const SerialIndex aIndex, const uint64_t aValue) = 0;
        virtual void WriteI8(const SerialIndex aIndex, const int8_t aValue) = 0;
        virtual void WriteI16(const SerialIndex aIndex, const int16_t aValue) = 0;
        virtual void WriteI32(const SerialIndex aIndex, const int32_t aValue) = 0;
        virtual void WriteI64(const SerialIndex aIndex, const int64_t aValue) = 0;
        virtual void WriteF(const SerialIndex aIndex, const float aValue) = 0;
        virtual void WriteD(const SerialIndex aIndex, const double aValue) = 0;
        virtual void WriteS(const SerialIndex aIndex, const SerialString aValue) = 0;

        virtual void WriteA(const SerialIndex aIndex, SerialArrayPtr) = 0;
        virtual void WriteO(const SerialIndex aIndex, SerialObjectPtr) = 0;
    };

    class SerialObject{
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
    };
}}


#endif
