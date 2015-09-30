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
#include "..\Core\Memory\SmartPointer.hpp"

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
    typedef void Null;
    typedef bool Bool;
    typedef double Number;
    typedef Core::String String;
    typedef Core::DynamicArray<Core::SharedPointer<Value>> Array;
    typedef std::map<Core::String, Core::SharedPointer<Value>> Object;

    template<class T>
    static constexpr TypeID GetTypeID() = delete;

    template<>
    constexpr TypeID GetTypeID<Null>(){return TYPE_NULL;}

    template<>
    constexpr TypeID GetTypeID<Bool>(){return TYPE_BOOL;}

    template<>
    constexpr TypeID GetTypeID<Number>(){return TYPE_NUMBER;}

    template<>
    constexpr TypeID GetTypeID<String>(){return TYPE_STRING;}

    template<>
    constexpr TypeID GetTypeID<Array>(){return TYPE_ARRAY;}

    template<>
    constexpr TypeID GetTypeID<Object>(){return TYPE_OBJECT;}

    class Value{
    public:
    private:
        Value(const Value&) = delete;
        Value& operator=(const Value&) = delete;

        union{
            Bool mBool;
            Number mNumber;
            String* mString;
            Array* mArray;
            Object* mObject;
        };
        TypeID mID;

        void Clear(){
            switch(mID){
            case TYPE_STRING:
                if(mString != nullptr){
                    delete mString;
                    mString = nullptr;
                }
                break;
            case TYPE_ARRAY:
                if(mArray != nullptr){
                    delete mArray;
                    mArray = nullptr;
                }
                break;
            case TYPE_OBJECT:
                if(mObject != nullptr){
                    delete mObject;
                    mObject = nullptr;
                }
                break;
            default:
                break;
            }
            mID = TYPE_NULL;
        }
    public:
        Value(const TypeID aType = TYPE_NULL):
            mID(aType)
        {
            switch(aType){
            case TYPE_BOOL:
                mBool = false;
                break;
            case TYPE_NUMBER:
                mNumber = 0.0;
                break;
            case TYPE_STRING:
                mString = new String();
                break;
            case TYPE_ARRAY:
                mArray = new Array();
                break;
            case TYPE_OBJECT:
                mObject = new Object();
                break;
            default:
                break;
            }
        }

        Value(Value&& aOther):
            mID(aOther.mID),
            mString(aOther.mString)
        {
            if(sizeof(String*) >= sizeof(Number)){
                aOther.mString = nullptr;
            }else{
                aOther.mNumber = 0;
            }
            aOther.mID = TYPE_NULL;
        }

        Value(const Bool aValue):
            mID(GetTypeID<Bool>()),
            mBool(aValue)
        {}

        Value(const Number aValue):
            mID(GetTypeID<Number>()),
            mNumber(aValue)
        {}

        Value(const Core::ConstStringFragment aValue):
            mID(GetTypeID<String>()),
            mString(new String(aValue))
        {}

        Value(Array&& aValue):
            mID(GetTypeID<Array>()),
            mArray(new Array(std::move(aValue)))
        {}

        Value(Object&& aValue):
            mID(GetTypeID<Object>()),
            mObject(new Object(std::move(aValue)))
        {}

        ~Value(){
            Clear();
        }

        template<class T>
        bool Is() const{
            return mID == GetTypeID<T>();
        }
        bool IsNull() const{return Is<Null>();}
        bool IsBool() const{return Is<Bool>();}
        bool IsNumber() const{return Is<Number>();}
        bool IsString() const{return Is<String>();}
        bool IsArray() const{return Is<Array>();}
        bool IsObject() const{return Is<Object>();}

        Bool GetBool() const{
            switch(mID){
            case TYPE_BOOL:
                return mBool;
            case TYPE_NUMBER:
                return mNumber > 0.0;
            case TYPE_STRING:{
                String& string = *mString;
                if(string == "1" || string == "true"){
                    return true;
                }else if(string == "0" || string == "false"){
                    return false;
                }
            }default:
                throw std::runtime_error("Json::Value : Cannot cast type to Bool");
            }
        }

        Number GetNumber() const{
            switch(mID){
            case TYPE_BOOL:
                return mBool ? 1.0 : 0.0;
            case TYPE_NUMBER:
                return mNumber;
            case TYPE_STRING:{
                return Core::NumericParse::Parse<Number>(*mString).first;
            }default:
                throw std::runtime_error("Json::Value : Cannot cast type to Number");
            }
        }

        String& GetString(){
            switch(mID){
            case TYPE_STRING:{
                return *mString;
            }default:
                throw std::runtime_error("Json::Value : Cannot cast type to String");
            }
        }

        const String& GetString() const{
            switch(mID){
            case TYPE_STRING:{
                return *mString;
            }default:
                throw std::runtime_error("Json::Value : Cannot cast type to String");
            }
        }

        Array& GetArray(){
            switch(mID){
            case TYPE_ARRAY:{
                return *mArray;
            }default:
                throw std::runtime_error("Json::Value : Cannot cast type to Array");
            }
        }

        const Array& GetArray() const{
            switch(mID){
            case TYPE_ARRAY:{
                return *mArray;
            }default:
                throw std::runtime_error("Json::Value : Cannot cast type to Array");
            }
        }

        Object& GetObject(){
            switch(mID){
            case TYPE_OBJECT:{
                return *mObject;
            }default:
                throw std::runtime_error("Json::Value : Cannot cast type to Object");
            }
        }

        const Object& GetObject() const{
            switch(mID){
            case TYPE_OBJECT:{
                return *mObject;
            }default:
                throw std::runtime_error("Json::Value : Cannot cast type to Object");
            }
        }

        operator Bool() const{return GetBool();}
        operator Number() const{return GetNumber();}
        operator String&(){return GetString();}
        operator const String&() const{return GetString();}
        operator Array&(){return GetArray();}
        operator const Array&() const{return GetArray();}
        operator Object&(){return GetObject();}
        operator const Object&() const{return GetObject();}

        Value& SetBool(const Bool aValue){
            if(mID != GetTypeID<Bool>()){
                Clear();
                mID = GetTypeID<Bool>();
            }
            mBool = aValue;
            return *this;
        }

        Value& SetNumber(const Number aValue){
            if(mID != GetTypeID<Number>()){
                Clear();
                mID = GetTypeID<Number>();
            }
            mNumber = aValue;
            return *this;
        }

        Value& SetString(const Core::ConstStringFragment aValue){
            if(mID != GetTypeID<String>()){
                Clear();
                mID = GetTypeID<String>();
                mString = new String();
            }
            *mString = aValue;
            return *this;
        }

        Value& SetArray(Array&& aValue){
            if(mID != GetTypeID<Array>()){
                Clear();
                mID = GetTypeID<Array>();
                mArray = new Array();
            }
            *mArray = std::move(aValue);
            return *this;
        }

        Value& SetObject(Object&& aValue){
            if(mID != GetTypeID<Object>()){
                Clear();
                mID = GetTypeID<Object>();
                mObject = new Object();
            }
            *mObject = std::move(aValue);
            return *this;
        }

        Value& operator=(const Bool aValue){return SetBool(aValue);}
        Value& operator=(const Number aValue){return SetNumber(aValue);}
        Value& operator=(const Core::ConstStringFragment aValue){return SetString(aValue);}
        Value& operator=(Array&& aValue){return SetArray(std::move(aValue));}
        Value& operator=(Object&& aValue){return SetObject(std::move(aValue));}

        Value& operator=(Value&& aValue){
            std::swap(mID, aValue.mID);
            if(sizeof(String*) >= sizeof(Number)){
                std::swap(mString, mString);
            }else{
                std::swap(mNumber, mNumber);
            }
            return *this;
        }
    };

    //template<>
    //Bool& Value::Get<Bool>(){

    //};

    /*class Value : public std::enable_shared_from_this<Value>{
    private:
        Value(const Value&) = delete;
        Value(Value&&) = delete;
        Value& operator=(const Value&) = delete;
        Value& operator=(Value&&) = delete;
    protected:
        virtual Core::String Parse(Core::Allocator<void>& aAllocator) const = 0;
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

        Core::String Parse(Core::Allocator<void>& aAllocator) const override{
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

        Core::String Parse(Core::Allocator<void>& aAllocator) const override{
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

        Core::String Parse(Core::Allocator<void>& aAllocator) const override{
            return Core::String(mValue.begin(), mValue.end(), aAllocator);
        }
    public:
        String(Core::Allocator<void>& aAllocator = Core::GetDefaultAllocator<void>()):
            mValue(aAllocator)
        {}

        String(const Core::ConstStringFragment aValue, Core::Allocator<void>& aAllocator = Core::GetDefaultAllocator<void>()):
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

        Core::String Parse(Core::Allocator<void>& aAllocator) const override{
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
		typedef std::shared_ptr<Type> Pointer;
		typedef std::shared_ptr<const Type> ConstPointer;
		typedef typename Core::DynamicArray<Pointer>::Iterator Iterator;
		typedef typename Core::DynamicArray<Pointer>::ConstIterator ConstIterator;
		typedef typename Core::DynamicArray<Pointer>::ReverseIterator ReverseIterator;
		typedef typename Core::DynamicArray<Pointer>::ConstReverseIterator ConstReverseIterator;
    private:
        Core::DynamicArray<Pointer> mValues;
    protected:

        // Inherited from Value

        Core::String Parse(Core::Allocator<void>& aAllocator) const override{
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
        Array(Core::Allocator<void>& aAllocator) :
            mValues(32, aAllocator)
        {}
        // Delegated to mValues

        Pointer InsertBefore(const ConstIterator aPos, Pointer aValue){return mValues.InsertBefore(aPos, aValue);}
        Pointer InsertAfter(const ConstIterator aPos, Pointer aValue){return mValues.InsertAfter(aPos, aValue);}
        void Erase(const ConstIterator aPos){mValues.Erase(aPos);}

        //Iterator Find(const Value& aValue){return mValues.Find(&aValue);}
        //ConstIterator Find(const Value& aValue) const{return mValues.Find(&aValue);}

        Reference PushBack(Pointer aValue){return *mValues.PushBack(aValue);}
        Reference PushFront(Pointer aValue){return *mValues.PushFront(aValue);}

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
		typedef std::pair<Core::String, std::shared_ptr<Value>> Type;
		typedef const Type ConstType;
		typedef Type& Reference;
		typedef ConstType& ConstReference;
        typedef Type&& Move;
		typedef Type* Pointer;
		typedef ConstType* ConstPointer;
		typedef typename std::map<Core::String, std::shared_ptr<Value>>::iterator Iterator;
		typedef typename std::map<Core::String, std::shared_ptr<Value>>::const_iterator ConstIterator;
		typedef typename std::map<Core::String, std::shared_ptr<Value>>::reverse_iterator ReverseIterator;
		typedef typename std::map<Core::String, std::shared_ptr<Value>>::const_reverse_iterator ConstReverseIterator;
    private:
        std::map<Core::String, std::shared_ptr<Value>> mValues;
    protected:

        // Inherited from Value

        Core::String Parse(Core::Allocator<void>& aAllocator) const override{
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

        Value& Add(const Core::ConstStringFragment aName, std::shared_ptr<Value> aValue){
            return *(mValues.emplace(aName, aValue).first->second);
        }

        void Erase(const Core::ConstStringFragment aName){
            auto valueIt = mValues.find(aName);
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
    };*/
}}

#endif
