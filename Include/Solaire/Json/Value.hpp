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
#include <map>
#include <list>
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
    private:
        Value(const Value&) = delete;
        Value(Value&&) = delete;
        Value& operator=(const Value&) = delete;
        Value& operator=(Value&&) = delete;
    protected:
        virtual Core::String Parse(Core::Allocator<Core::String::Type>& aAllocator) const = 0;
    public:
        friend Array;
        friend Object;

        Value(){
        }

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

    class Bool : public Value{
    private:
        bool mValue;
    protected:
        // Inherited from Value

        Core::String Parse(Core::Allocator<Core::String::Type>& aAllocator) const override{
            return Core::String(mValue ? "true" : "false", aAllocator);
        }
    public:
        Bool():
            mValue(false)
        {}

        Bool(const bool aValue):
            mValue(aValue)
        {}

        operator bool() const{
            return mValue;
        }

        Bool& operator=(const bool aValue){
            mValue = aValue;
            return *this;
        }

        // Inherited from Value

        TypeID GetTypeID() const override{
            return Json::GetTypeID<Bool>();
        }
    };

    class Number : public Value{
    private:
        double mValue;
    protected:
        // Inherited from Value

        Core::String Parse(Core::Allocator<Core::String::Type>& aAllocator) const override{
            Core::String::Type buf[32];
            Core::String::Pointer end = Core::NumericParse::ToString(buf, mValue);

            return Core::String(buf, end, aAllocator);
        }
    public:
        Number():
            mValue(0.0)
        {}

        Number(const double aValue):
            mValue(aValue)
        {}

        operator double() const{
            return mValue;
        }

        Number& operator=(const double aValue){
            mValue = aValue;
            return *this;
        }

        // Inherited from Value

        TypeID GetTypeID() const override{
            return Json::GetTypeID<Number>();
        }
    };

    class String : public Value{
    private:
        Core::String mValue;
    protected:
        // Inherited from Value

        Core::String Parse(Core::Allocator<Core::String::Type>& aAllocator) const override{
            return Core::String(mValue.begin(), mValue.end(), aAllocator);
        }
    public:
        String(Core::Allocator<Core::String::Type>& aAllocator = Core::GetDefaultAllocator<Core::String::Type>()):
            mValue(aAllocator)
        {}

        String(const Core::ConstStringFragment aValue, Core::Allocator<Core::String::Type>& aAllocator = Core::GetDefaultAllocator<Core::String::Type>()):
            mValue(aValue, aAllocator)
        {}

        operator Core::String&(){
            return mValue;
        }

        operator const Core::String&() const{
            return mValue;
        }

        String& operator=(const Core::ConstStringFragment aValue){
            mValue = aValue;
            return *this;
        }

        // Inherited from Value

        TypeID GetTypeID() const override{
            return Json::GetTypeID<String>();
        }
    };

    class Null : public Value{
    protected:
        // Inherited from Value

        Core::String Parse(Core::Allocator<Core::String::Type>& aAllocator) const override{
            return Core::String("null", aAllocator);
        }
    public:
        // Inherited from Value

        TypeID GetTypeID() const override{
            return Json::GetTypeID<Null>();
        }
    };

    class Array : public Value{
    public:
		typedef Value Type;
		typedef const Type ConstType;
		typedef Type& Reference;
		typedef ConstType& ConstReference;
        typedef Type&& Move;
		typedef Type* Pointer;
		typedef ConstType* ConstPointer;
		typedef typename Core::DynamicArray<Pointer>::Iterator Iterator;
		typedef typename Core::DynamicArray<Pointer>::ConstIterator ConstIterator;
		typedef typename Core::DynamicArray<Pointer>::ReverseIterator ReverseIterator;
		typedef typename Core::DynamicArray<Pointer>::ConstReverseIterator ConstReverseIterator;
    private:
        Core::DynamicArray<Pointer> mValues;
    protected:

        // Inherited from Value

        Core::String Parse(Core::Allocator<Core::String::Type>& aAllocator) const override{
            Core::String tmp(aAllocator);
            tmp += '[';
            const ConstIterator end = mValues.end();
            for(ConstIterator i = mValues.begin(); i != end; ++i){
                tmp += (**i).Parse(aAllocator);
                if(i + 1 != end) tmp += ',';
            }
            tmp += ']';
            return tmp;
        }
    public:

        // Delegated to mValues

        Reference InsertBefore(const ConstIterator aPos, Value& aValue){return *mValues.InsertBefore(aPos, &aValue);}
        Reference InsertAfter(const ConstIterator aPos, Value& aValue){return *mValues.InsertAfter(aPos, &aValue);}
        void Erase(const ConstIterator aPos){mValues.Erase(aPos);}

        //Iterator Find(const Value& aValue){return mValues.Find(&aValue);}
        //ConstIterator Find(const Value& aValue) const{return mValues.Find(&aValue);}

        Reference PushBack(Value& aValue){return *mValues.PushBack(&aValue);}
        Reference PushFront(Value& aValue){return *mValues.PushFront(&aValue);}

        Reference Front(){return *mValues.Front();}
        ConstReference Front() const{return *mValues.Front();}
        Reference Back(){return *mValues.Front();}
        ConstReference Back() const{return *mValues.Front();}

        Reference operator[](const size_t aIndex){return *mValues[aIndex];}
        ConstReference operator[](const size_t aIndex) const{return *mValues[aIndex];}

        Iterator begin(){return mValues.begin();}
        ConstIterator begin() const{return mValues.begin();}
        Iterator end(){return mValues.end();}
        ConstIterator end() const{return mValues.end();}
        ReverseIterator rbegin(){return mValues.rbegin();}
        ConstReverseIterator rbegin() const{return mValues.rbegin();}
        ReverseIterator rend(){return mValues.rend();}
        ConstReverseIterator rend() const{return mValues.rend();}


        // Inherited from Value

        TypeID GetTypeID() const override{
            return Json::GetTypeID<Array>();
        }
    };

    class Object : public Value{
    public:
		typedef std::pair<Core::String, Value*> Type;
		typedef const Type ConstType;
		typedef Type& Reference;
		typedef ConstType& ConstReference;
        typedef Type&& Move;
		typedef Type* Pointer;
		typedef ConstType* ConstPointer;
		typedef typename std::map<Core::ConstStringFragment, Value*>::iterator Iterator;
		typedef typename std::map<Core::ConstStringFragment, Value*>::const_iterator ConstIterator;
		typedef typename std::map<Core::ConstStringFragment, Value*>::reverse_iterator ReverseIterator;
		typedef typename std::map<Core::ConstStringFragment, Value*>::const_reverse_iterator ConstReverseIterator;
    private:
        std::list<Core::String> mNames;
        std::map<Core::ConstStringFragment, Value*> mValues;
    protected:

        // Inherited from Value

        Core::String Parse(Core::Allocator<Core::String::Type>& aAllocator) const override{
            Core::String tmp(aAllocator);
            tmp += '{';
            for(const Type& i : mValues){
                tmp += '"';
                tmp += i.first;
                tmp += '"';
                tmp += ':';
                tmp += i.second->Parse(aAllocator);
                if(++mValues.find(i.first) != mValues.end()) tmp += ',';
            }
            tmp += '}';
            return tmp;
        }
    public:

        Value& Insert(const Core::ConstStringFragment aName, Value& aValue){
            auto nameIt = std::find(mNames.begin(), mNames.end(), aName);
            if(nameIt == mNames.end()){
                mNames.push_back(aName);
                nameIt = --mNames.end();
            }

            return *(mValues.emplace(Core::ConstStringFragment(nameIt->begin(), nameIt->end()), &aValue).first->second);
        }

        void Erase(const Core::ConstStringFragment aName){
            auto nameIt = std::find(mNames.begin(), mNames.end(), aName);
            if(nameIt == mNames.end()) return;

            auto valueIt = mValues.find(aName);
            if(valueIt == mValues.end()) return;

            mNames.erase(nameIt);
            mValues.erase(valueIt);
        }

        // Delegated to mValues

        Iterator Find(const Core::ConstStringFragment aName){return mValues.find(aName);}
        ConstIterator Find(const Core::ConstStringFragment aName) const{return mValues.find(aName);}

        Value& operator[](const Core::ConstStringFragment aName){return *(mValues.find(aName)->second);}
        const Value& operator[](const Core::ConstStringFragment aName) const{return *(mValues.find(aName)->second);}

        Iterator begin(){return mValues.begin();}
        ConstIterator begin() const{return mValues.begin();}
        Iterator end(){return mValues.end();}
        ConstIterator end() const{return mValues.end();}
        ReverseIterator rbegin(){return mValues.rbegin();}
        ConstReverseIterator rbegin() const{return mValues.rbegin();}
        ReverseIterator rend(){return mValues.rend();}
        ConstReverseIterator rend() const{return mValues.rend();}

        // Inherited from Value

        TypeID GetTypeID() const override{
            return Json::GetTypeID<Object>();
        }
    };
}}

#endif
