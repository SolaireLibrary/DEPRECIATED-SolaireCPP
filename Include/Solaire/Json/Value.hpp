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

#include <type_traits>
#include "..\Core\Strings\String.hpp"
#include "..\Core\Strings\NumberParser.hpp"

namespace Solaire{ namespace Json{

    enum Type : uint8_t{};

    class String;
    class Number;
    class Bool;
    class Null;
    class Object;
    class Array;

    template<class T>
    static constexpr Type TypeID() = delete;

    template<>
    constexpr Type TypeID<Null>(){return static_cast<Type>(0);}

    template<>
    constexpr Type TypeID<Bool>(){return static_cast<Type>(1);}

    template<>
    constexpr Type TypeID<Number>(){return static_cast<Type>(2);}

    template<>
    constexpr Type TypeID<String>(){return static_cast<Type>(3);}

    template<>
    constexpr Type TypeID<Array>(){return static_cast<Type>(4);}

    template<>
    constexpr Type TypeID<Object>(){return static_cast<Type>(5);}

    class Value{
    private:
    public:
        virtual ~Value(){

        }

        template<class T>
        const bool Is() const{
            return GetType() == TypeID<T>();
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

        virtual Type GetType() const = 0;
        virtual Core::String Parse(Core::Allocator<Core::String::Type>& aAllocator) const = 0;
    };

    class PrimativeValue : public Value{
    private:
        Core::String* mJson;
        Core::StringFragment mFragment;
    protected:
        Core::ConstStringFragment Get() const{
            return mFragment;
        }

        void Set(const Core::ConstStringFragment aFragment){
            mFragment = mJson->ReplaceNext(mFragment.begin(), mFragment, aFragment);
        }
    public:
        PrimativeValue():
            mJson(nullptr),
            mFragment(nullptr, nullptr)
        {}

        PrimativeValue(Core::String& aString, const Core::String::Iterator aBegin, Core::String::Iterator aEnd):
            mJson(&aString),
            mFragment(aBegin, aEnd)
        {}

        virtual ~PrimativeValue(){

        }

        // Inherited from Value

        virtual Core::String Parse(Core::Allocator<Core::String::Type>& aAllocator) const override{
            return Core::String(mFragment.begin(), mFragment.end(), aAllocator);
        }
    };

    class String : public PrimativeValue{
    public:
        String():
            PrimativeValue()
        {}

        String(Core::String& aString, const Core::String::Iterator aBegin, Core::String::Iterator aEnd):
            PrimativeValue(aString, aBegin, aEnd)
        {}

        operator Core::ConstStringFragment() const{
            return Get();
        }

        operator Core::String::Type() const{
            return Get()[0];
        }

        String& operator=(const Core::String::Type aChar){
            Core::String::Type buf[2] = {aChar, '\0'};
            Set(Core::StringFragment(buf, buf + 1));
            return *this;
        }

        String& operator=(const Core::ConstStringFragment aFragment){
            Set(aFragment);
            return *this;
        }

        // Inherited from Value

        Type GetType() const override{
            return TypeID<String>();
        }

        Core::String Parse(Core::Allocator<Core::String::Type>& aAllocator) const override{
            Core::String str = PrimativeValue::Parse(aAllocator);
            str.PushFront('"');
            str.PushBack('"');
            return str;
        }
    };

    class Number : public PrimativeValue{
    public:
        Number():
            PrimativeValue()
        {}

        Number(Core::String& aString, const Core::String::Iterator aBegin, Core::String::Iterator aEnd):
            PrimativeValue(aString, aBegin, aEnd)
        {}

        operator double() const{
            return Core::NumericParse::Parse<double>(Get()).first;
        }

        Number& operator=(const double aValue){
            Core::String::Type buf[32];
            Core::String::Pointer end = Core::NumericParse::Parse(buf, aValue);
            Set(Core::StringFragment(buf, end));
            return *this;
        }

        // Inherited from Value

        Type GetType() const override{
            return TypeID<Number>();
        }
    };

    class Bool : public PrimativeValue{
    public:
        Bool():
            PrimativeValue()
        {}

        Bool(Core::String& aString, const Core::String::Iterator aBegin, Core::String::Iterator aEnd):
            PrimativeValue(aString, aBegin, aEnd)
        {}

        operator bool() const{
            return Get() == "true";
        }

        Bool& operator=(const bool aValue){
            Set(aValue ? "true" : "false");
            return *this;
        }

        // Inherited from Value

        Type GetType() const override{
            return TypeID<Bool>();
        }
    };

    class Null : public Value{
    public:
        Null():
            Value()
        {}

        // Inherited from Value

        Type GetType() const override{
            return TypeID<Null>();
        }
    };

    class Array : public Value{
    private:

    public:
        Array():
            Value()
        {}

        // Inherited from Value

        Type GetType() const override{
            return TypeID<Array>();
        }

        Core::String Parse(Core::Allocator<Core::String::Type>& aAllocator) const override{
            //! \TODO Implement
            return Core::String(aAllocator);
        }
    };

    class Object : public Value{
    private:

    public:
        Object():
            Value()
        {}

        // Inherited from Value

        Type GetType() const override{
            return TypeID<Object>();
        }

        Core::String Parse(Core::Allocator<Core::String::Type>& aAllocator) const override{
            //! \TODO Implement
            return Core::String(aAllocator);
        }
    };
}}

#endif
