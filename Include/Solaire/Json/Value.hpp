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
\version 1.0
\date
Created			: 29th September 2015
Last Modified	: 29th September 2015
*/

#include <Type_traits>
#include "..\Core\Strings\String.hpp"
#include "..\Core\Strings\NumberParser.hpp"
#include "..\Core\Iterators\DereferenceIterator.hpp"

namespace Solaire{ namespace Json{

    enum TypeID : uint8_t{};

    class String;
    class Number;
    class Bool;
    class Null;
    class Object;
    class Array;
    class Document;

    template<class T>
    static constexpr TypeID GetTypeID() = delete;

    template<>
    constexpr TypeID GetTypeID<Null>(){return static_cast<TypeID>(0);}

    template<>
    constexpr TypeID GetTypeID<Bool>(){return static_cast<TypeID>(1);}

    template<>
    constexpr TypeID GetTypeID<Number>(){return static_cast<TypeID>(2);}

    template<>
    constexpr TypeID GetTypeID<String>(){return static_cast<TypeID>(3);}

    template<>
    constexpr TypeID GetTypeID<Array>(){return static_cast<TypeID>(4);}

    template<>
    constexpr TypeID GetTypeID<Object>(){return static_cast<TypeID>(5);}

    class Value{
    protected:
        typedef Core::DynamicArray<Value*> ValueArray;
    private:
        Core::String* mDocumentString;
        ValueArray* mValueArray;
        uint32_t mFragmentBegin;
        uint32_t mFragmentEnd;
    protected:
        Value(ValueArray& aValueArray, Core::String& aString, const uint32_t aBegin, uint32_t aEnd):
            mDocumentString(&aString),
            mValueArray(&aValueArray),
            mFragmentBegin(aBegin),
            mFragmentEnd(aEnd)
        {}

        virtual void OnErase(const uint32_t aOffset, const uint32_t aCount){
            if(mFragmentBegin > aOffset) mFragmentBegin -= aCount;
            if(mFragmentEnd > aOffset) mFragmentEnd -= aCount;
        }

        virtual void OnInsert(const uint32_t aOffset, const uint32_t aCount){
            if(mFragmentBegin > aOffset) mFragmentBegin += aCount;
            if(mFragmentEnd > aOffset) mFragmentEnd += aCount;
        }

        void EraseFromDocumentString(const Core::String::ConstIterator aPos, const uint32_t aCount){
            mDocumentString->Erase(aPos, aCount);

            const uint32_t offset = aPos - mDocumentString->begin();
            for(Value* value : *mValueArray){
                value->OnErase(offset, aCount);
            }
        }

        void InsertIntoDocumentString(const Core::String::ConstIterator aPos, const Core::ConstStringFragment aFragment){
            mDocumentString->InsertBefore(aPos, aFragment);

            const Core::String::ConstIterator begin = aFragment.begin();
            const uint32_t offset = begin - mDocumentString->begin();
            const uint32_t size = aFragment.Size();
            for(Value* value : *mValueArray){
                value->OnInsert(offset, size);
            }
        }

        void OverwriteDocumentString(const Core::String::ConstIterator aPos, const uint32_t aEraseCount, const Core::ConstStringFragment aFragment){
            EraseFromDocumentString(aPos, aEraseCount);
            InsertIntoDocumentString(aPos, aFragment);
        }

        Core::String::ConstIterator GetStringBegin() const{
            return mDocumentString->begin();
        }

        Core::ConstStringFragment GetThisFragment() const{
            const Core::String::Iterator begin = mDocumentString->begin();
            return Core::ConstStringFragment(begin + mFragmentBegin, begin + mFragmentEnd);
        }
    public:
        virtual ~Value(){

        }

        template<class T>
        const bool Is() const{
            return GetTypeID() == Json::GetTypeID<T>();
        }

        template<class T>
        T& As(){
            if(! Is<T>()) throw std::runtime_error("Json::Value : Invalid cast");
            return *reinterpret_cast<T*>(this);
        }

        template<class T>
        const T& As() const{
            if(! Is<T>()) throw std::runtime_error("Json::Value : Invalid cast");
            return *reinterpret_cast<const T*>(this);
        }

        virtual TypeID GetTypeID() const = 0;
    };

    class PrimativeValue : public Value{
    private:
        uint32_t mValueBegin;
        uint32_t mValueEnd;
    protected:
        PrimativeValue(
            ValueArray& aValueArray,
            Core::String& aString,
            const uint32_t aThisBegin,
            const uint32_t aThisEnd,
            const uint32_t aValueBegin,
            const uint32_t aValueEnd
        ):
            Value(aValueArray, aString, aThisBegin, aThisEnd),
            mValueBegin(aValueBegin),
            mValueEnd(aValueEnd)
        {}

        Core::ConstStringFragment GetValueFragment() const{
            const Core::String::ConstIterator begin = GetStringBegin();
            return Core::ConstStringFragment(begin + mValueBegin, begin + mValueEnd);
        }

        void OverwriteValue(const Core::ConstStringFragment aFragment){
            OverwriteDocumentString(GetStringBegin() + mValueBegin, mValueEnd - mValueBegin, aFragment);
        }

        // Inherited from value

        virtual void OnErase(const uint32_t aOffset, const uint32_t aCount){
            Value::OnErase(aOffset, aCount);
            if(mValueBegin > aOffset) mValueBegin -= aCount;
            if(mValueEnd > aOffset) mValueEnd -= aCount;
        }

        virtual void OnInsert(const uint32_t aOffset, const uint32_t aCount){
            Value::OnInsert(aOffset, aCount);
            if(mValueBegin > aOffset) mValueBegin += aCount;
            if(mValueEnd > aOffset) mValueEnd += aCount;
        }
    public:
        virtual ~PrimativeValue(){

        }
    };

    /*class GenericValue{
    private:
        Core::String* mDocumentString;
        Value* mValue;

        GenericValue(Core::String& aString, const Core::String::Iterator aBegin, Core::String::Iterator aEnd):
            mDocumentString(&aString),
            mValue(nullptr)
        {}

        void Clear(){
            if(mValue != nullptr){
                mDocumentString->Erase(mThisFragment.begin(), mThisFragment.Size());
            }
        }
    public:
        friend Document;
        friend Array;
        friend Object;

        GenericValue& operator=(const Value& aValue){
            Clear();
            mDocumentString->Erase(mThisFragment.begin(), mThisFragment.Size());
            return *this;
        }

        // Inherited from Value

        TypeID GetTypeID() const override{
            return mValue->GetValueType();
        }
    };*/

    class String : public PrimativeValue{
    private:
        String(
            ValueArray& aValueArray,
            Core::String& aString,
            const uint32_t aThisBegin,
            const uint32_t aThisEnd,
            const uint32_t aValueBegin,
            const uint32_t aValueEnd
        ):
            PrimativeValue(aValueArray, aString, aThisBegin, aThisEnd, aValueBegin, aValueEnd)
        {}
    public:
        friend Document;

        operator Core::ConstStringFragment() const{
            const Core::ConstStringFragment frag = GetValueFragment();
            return Core::ConstStringFragment(frag.begin() + 1, frag.end() - 1);
        }

        operator Core::String::Type() const{
            return GetValueFragment()[1];
        }

        String& operator=(const Core::String::Type aChar){
            Core::String::Type buf[2] = {aChar, '\0'};
            OverwriteValue(Core::StringFragment(buf, buf + 1));
            return *this;
        }

        String& operator=(const Core::ConstStringFragment aFragment){
            OverwriteValue(aFragment);
            return *this;
        }

        // Inherited from Value

        TypeID GetTypeID() const override{
            return Json::GetTypeID<String>();
        }
    };

    class Number : public PrimativeValue{
    private:
        Number(
            ValueArray& aValueArray,
            Core::String& aString,
            const uint32_t aThisBegin,
            const uint32_t aThisEnd,
            const uint32_t aValueBegin,
            const uint32_t aValueEnd
        ):
            PrimativeValue(aValueArray, aString, aThisBegin, aThisEnd, aValueBegin, aValueEnd)
        {}
    public:
        friend Document;

        operator double() const{
            return Core::NumericParse::Parse<double>(GetValueFragment()).first;
        }

        Number& operator=(const double aValue){
            Core::String::Type buf[32];
            Core::String::Pointer end = Core::NumericParse::Parse(buf, aValue);
            OverwriteValue(Core::StringFragment(buf, end));
            return *this;
        }

        // Inherited from Value

        TypeID GetTypeID() const override{
            return Json::GetTypeID<Number>();
        }
    };

    class Bool : public PrimativeValue{
    private:
        Bool(
            ValueArray& aValueArray,
            Core::String& aString,
            const uint32_t aThisBegin,
            const uint32_t aThisEnd,
            const uint32_t aValueBegin,
            const uint32_t aValueEnd
        ):
            PrimativeValue(aValueArray, aString, aThisBegin, aThisEnd, aValueBegin, aValueEnd)
        {}
    public:
        friend Document;

        operator bool() const{
            return GetValueFragment() == "true";
        }

        Bool& operator=(const bool aValue){
            OverwriteValue(aValue ? "true" : "false");
            return *this;
        }

        // Inherited from Value

        TypeID GetTypeID() const override{
            return Json::GetTypeID<Bool>();
        }
    };

    class Null : public PrimativeValue{
    private:
        Null(
            ValueArray& aValueArray,
            Core::String& aString,
            const uint32_t aThisBegin,
            const uint32_t aThisEnd,
            const uint32_t aValueBegin,
            const uint32_t aValueEnd
        ):
            PrimativeValue(aValueArray, aString, aThisBegin, aThisEnd, aValueBegin, aValueEnd)
        {}
    public:
        friend Document;

        // Inherited from Value

        TypeID GetTypeID() const override{
            return Json::GetTypeID<Null>();
        }
    };

    class Array : public Value{
    public:
		typedef Value Type;
		typedef const Value ConstType;
		typedef Type& Reference;
		typedef ConstType& ConstReference;
        typedef Type&& Move;
		typedef Type* Pointer;
		typedef ConstType* ConstPointer;
		//typedef Core::DereferenceIterator<Value, Pointer> Iterator;
		//typedef Core::ConstIterator<Value, Iterator> ConstIterator;
		//typedef Core::ReverseIterator<Value, Iterator> ReverseIterator;
		//typedef Core::ConstIterator<Value, ReverseIterator> ConstReverseIterator;
    private:
        Core::DynamicArray<Pointer> mValues;

        Array(
            ValueArray& aValueArray,
            Core::String& aString,
            const uint32_t aThisBegin,
            const uint32_t aThisEnd
        ):
            Value(aValueArray, aString, aThisBegin, aThisEnd)
        {}
    public:
        friend Document;

        // Inherited from Value

        TypeID GetTypeID() const override{
            return Json::GetTypeID<Array>();
        }
    };

    class Object : public Value{
    private:
        Object(
            ValueArray& aValueArray,
            Core::String& aString,
            const uint32_t aThisBegin,
            const uint32_t aThisEnd
        ):
            Value(aValueArray, aString, aThisBegin, aThisEnd)
        {}
    public:
        friend Document;

        // Inherited from Value

        TypeID GetTypeID() const override{
            return Json::GetTypeID<Object>();
        }
    };
}}

#endif
