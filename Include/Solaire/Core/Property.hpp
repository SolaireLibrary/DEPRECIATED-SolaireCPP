#ifndef SOLAIRE_PROPERTY_HPP
#define SOLAIRE_PROPERTY_HPP

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
	\file Property.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 6th October 2015
	Last Modified	: 6th October 2015
*/

#include "Init.hpp"

namespace Solaire{

    template<class Parent, class Type, Type Parent::* const Member, const int PassMode = TypeTraits<Type>::PassMode>
    class ReadOnlyValueProperty{
    public:
        typedef typename PassTypes<Type, PassMode>::Type Return;
        typedef typename PassTypes<Type, PassMode>::ConstType ConstReturn;
    private:
        ReadOnlyValueProperty(const ReadOnlyValueProperty&) = delete;
        ReadOnlyValueProperty(ReadOnlyValueProperty&&) = delete;
        ReadOnlyValueProperty& operator=(const ReadOnlyValueProperty&) = delete;
        ReadOnlyValueProperty& operator=(ReadOnlyValueProperty&&) = delete;
    private:
        const Parent& mParent;
    public:
        constexpr ReadOnlyValueProperty(const Parent& aParent):
            mParent(aParent)
        {}

        SOLAIRE_INLINE ConstReturn operator*() const{
            return mParent.*Member;
        }

        SOLAIRE_INLINE const Type* operator->() const{
            return &(mParent.*Member);
        }

        SOLAIRE_INLINE operator ConstReturn() const{
            return mParent.*Member;
        }
    };

    template<class Parent, class Type, Type Parent::* const Member, const int PassMode = TypeTraits<Type>::PassMode>
    class ValueProperty{
    public:
        typedef typename PassTypes<Type, PassMode>::Type Return;
        typedef typename PassTypes<Type, PassMode>::ConstType ConstReturn;
    private:
        ValueProperty(const ValueProperty&) = delete;
        ValueProperty(ValueProperty&&) = delete;
        ValueProperty& operator=(const ValueProperty&) = delete;
        ValueProperty& operator=(ValueProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr ValueProperty(Parent& aParent):
            mParent(aParent)
        {}

        SOLAIRE_INLINE Return operator*(){
            return mParent.*Member;
        }

        SOLAIRE_INLINE Type* operator->(){
            return &(mParent.*Member);
        }

        SOLAIRE_INLINE ConstReturn operator*() const{
            return mParent.*Member;
        }

        SOLAIRE_INLINE const Type* operator->() const{
            return &(mParent.*Member);
        }

        SOLAIRE_INLINE operator Return(){
            return mParent.*Member;
        }

        SOLAIRE_INLINE operator ConstReturn() const{
            return mParent.*Member;
        }

        SOLAIRE_INLINE Type& operator=(ConstReturn aValue){
            return mParent.*Member = aValue;
        }
    };

    template<class Parent, class Type, Type Parent::* const Member>
    using ReadOnlyCopyValueProperty = ReadOnlyValueProperty<Parent, Type, Member, PASS_BY_VALUE>;

    template<class Parent, class Type, Type Parent::* const Member>
    using ReadOnlyReferenceValueProperty = ReadOnlyValueProperty<Parent, Type, Member, PASS_BY_REFERENCE>;

    template<class Parent, class Type, Type Parent::* const Member>
    using CopyValueProperty = ValueProperty<Parent, Type, Member, PASS_BY_VALUE>;

    template<class Parent, class Type, Type Parent::* const Member>
    using ReferenceValueProperty = ValueProperty<Parent, Type, Member, PASS_BY_REFERENCE>;

    ////

    template<class Parent, class Type, Type* Parent::* const Member, const int PassMode = TypeTraits<Type>::PassMode>
    class ReadOnlyDereferenceValueProperty{
    public:
        typedef typename PassTypes<Type, PassMode>::Type Return;
        typedef typename PassTypes<Type, PassMode>::ConstType ConstReturn;
    private:
        ReadOnlyDereferenceValueProperty(const ReadOnlyDereferenceValueProperty&) = delete;
        ReadOnlyDereferenceValueProperty(ReadOnlyDereferenceValueProperty&&) = delete;
        ReadOnlyDereferenceValueProperty& operator=(const ReadOnlyDereferenceValueProperty&) = delete;
        ReadOnlyDereferenceValueProperty& operator=(ReadOnlyDereferenceValueProperty&&) = delete;
    private:
        const Parent& mParent;
    public:
        constexpr ReadOnlyDereferenceValueProperty(const Parent& aParent):
            mParent(aParent)
        {}

        SOLAIRE_INLINE ConstReturn operator*() const{
            return *(mParent.*Member);
        }

        SOLAIRE_INLINE const Type* operator->() const{
            return mParent.*Member;
        }

        SOLAIRE_INLINE operator ConstReturn() const{
            return *mParent.*Member;
        }
    };

    template<class Parent, class Type, Type* Parent::* const Member, const int PassMode = TypeTraits<Type>::PassMode>
    class DereferenceValueProperty{
    public:
        typedef typename PassTypes<Type, PassMode>::Type Return;
        typedef typename PassTypes<Type, PassMode>::ConstType ConstReturn;
    private:
        DereferenceValueProperty(const DereferenceValueProperty&) = delete;
        DereferenceValueProperty(DereferenceValueProperty&&) = delete;
        DereferenceValueProperty& operator=(const DereferenceValueProperty&) = delete;
        DereferenceValueProperty& operator=(DereferenceValueProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr DereferenceValueProperty(Parent& aParent):
            mParent(aParent)
        {}

        SOLAIRE_INLINE Return operator*(){
            return *(mParent.*Member);
        }

        SOLAIRE_INLINE Type* operator->(){
            return mParent.*Member;
        }

        SOLAIRE_INLINE ConstReturn operator*() const{
            return *(mParent.*Member);
        }

        SOLAIRE_INLINE const Type* operator->() const{
            return mParent.*Member;
        }

        SOLAIRE_INLINE operator Return(){
            return *mParent.*Member;
        }

        SOLAIRE_INLINE operator ConstReturn() const{
            return *mParent.*Member;
        }

        SOLAIRE_INLINE Type& operator=(ConstReturn aValue){
            return *mParent.*Member = aValue;
        }
    };

    template<class Parent, class Type, Type* Parent::* const Member>
    using ReadOnlyCopyDereferenceValueProperty = ReadOnlyDereferenceValueProperty<Parent, Type, Member, PASS_BY_VALUE>;

    template<class Parent, class Type, Type* Parent::* const Member>
    using ReadOnlyReferenceDereferenceValueProperty = ReadOnlyDereferenceValueProperty<Parent, Type, Member, PASS_BY_REFERENCE>;

    template<class Parent, class Type, Type* Parent::* const Member>
    using CopyDereferenceValueProperty = DereferenceValueProperty<Parent, Type, Member, PASS_BY_VALUE>;

    template<class Parent, class Type, Type* Parent::* const Member>
    using ReferenceDereferenceValueProperty = DereferenceValueProperty<Parent, Type, Member, PASS_BY_REFERENCE>;

    ////

    template<class Parent, class Return, Return(*Function)(Parent&)>
    class StaticFunctionProperty{
    private:
        StaticFunctionProperty(const StaticFunctionProperty&) = delete;
        StaticFunctionProperty(StaticFunctionProperty&&) = delete;
        StaticFunctionProperty& operator=(const StaticFunctionProperty&) = delete;
        StaticFunctionProperty& operator=(StaticFunctionProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr StaticFunctionProperty(Parent& aParent):
            mParent(aParent)
        {}

        SOLAIRE_INLINE Return operator*(){
            return Function(mParent);
        }

        SOLAIRE_INLINE Return* operator->(){
            return &Function(mParent);
        }

        SOLAIRE_INLINE operator Return(){
            return Function(mParent);
        }

        SOLAIRE_INLINE Return& operator=(Return aValue) const{
            return Function(mParent) = aValue;
        }
    };

    template<class Parent, class Return, Return(*Function)(const Parent&)>
    class ReadOnlyStaticFunctionProperty{
    private:
        ReadOnlyStaticFunctionProperty(const ReadOnlyStaticFunctionProperty&) = delete;
        ReadOnlyStaticFunctionProperty(ReadOnlyStaticFunctionProperty&&) = delete;
        ReadOnlyStaticFunctionProperty& operator=(const ReadOnlyStaticFunctionProperty&) = delete;
        ReadOnlyStaticFunctionProperty& operator=(ReadOnlyStaticFunctionProperty&&) = delete;
    private:
        const Parent& mParent;
    public:
        constexpr ReadOnlyStaticFunctionProperty(const Parent& aParent):
            mParent(aParent)
        {}

        SOLAIRE_INLINE Return operator*() const{
            return Function(mParent);
        }

        SOLAIRE_INLINE Return* operator->() const{
            return &Function(mParent);
        }

        SOLAIRE_INLINE operator Return() const{
            return Function(mParent);
        }
    };

    ////

    template<class Parent, class Return, Return(Parent::*Function)()>
    class LocalFunctionProperty{
    private:
        LocalFunctionProperty(const LocalFunctionProperty&) = delete;
        LocalFunctionProperty(LocalFunctionProperty&&) = delete;
        LocalFunctionProperty& operator=(const LocalFunctionProperty&) = delete;
        LocalFunctionProperty& operator=(LocalFunctionProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr LocalFunctionProperty(Parent& aParent):
            mParent(aParent)
        {}

        SOLAIRE_INLINE Return operator*(){
            return mParent.*Function();
        }

        SOLAIRE_INLINE Return* operator->(){
            return &mParent.*Function();
        }

        SOLAIRE_INLINE operator Return(){
            return mParent.*Function();
        }

        SOLAIRE_INLINE Return& operator=(Return aValue) const{
            return mParent.*Function() = aValue;
        }
    };

    template<class Parent, class Return, Return(Parent::*Function)() const>
    class ReadOnlyLocalFunctionProperty{
    private:
        ReadOnlyLocalFunctionProperty(const ReadOnlyLocalFunctionProperty&) = delete;
        ReadOnlyLocalFunctionProperty(ReadOnlyLocalFunctionProperty&&) = delete;
        ReadOnlyLocalFunctionProperty& operator=(const ReadOnlyLocalFunctionProperty&) = delete;
        ReadOnlyLocalFunctionProperty& operator=(ReadOnlyLocalFunctionProperty&&) = delete;
    private:
        const Parent& mParent;
    public:
        constexpr ReadOnlyLocalFunctionProperty(const Parent& aParent):
            mParent(aParent)
        {}

        SOLAIRE_INLINE Return operator*() const{
            return mParent.*Function();
        }

        SOLAIRE_INLINE Return* operator->() const{
            return &mParent.*Function();
        }

        SOLAIRE_INLINE operator Return() const{
            return mParent.*Function();
        }
    };

    ////

    #define SOLAIRE_READ_PROPERTY(aName, aParentType, aRefType, aPtrType, aCode)\
    class aName{\
    private:\
        aName(const aName&) = delete;\
        aName(aName&&) = delete;\
        aName& operator=(const aName&) = delete;\
        aName& operator=(aName&&) = delete;\
    private:\
        const aParentType& mParent;\
    public:\
        constexpr aName(const aParentType& aParent):\
            mParent(aParent)\
        {}\
        \
        aRefType operator*() const{\
            aCode\
        }\
        \
        SOLAIRE_INLINE aPtrType operator->() const{\
            return &operator*();\
        }\
        \
        SOLAIRE_INLINE operator aRefType() const{\
            return operator*();\
        }\
    };

    #define SOLAIRE_WRITE_PROPERTY(aName, aParentType, aRefType, aPtrType, aCode)\
    class aName{\
    private:\
        aName(const aName&) = delete;\
        aName(aName&&) = delete;\
        aName& operator=(const aName&) = delete;\
        aName& operator=(aName&&) = delete;\
    private:\
        aParentType& mParent;\
    public:\
        constexpr aName(aParentType& aParent):\
            mParent(aParent)\
        {}\
        \
        aRefType operator*(){\
            aCode\
        }\
        \
        SOLAIRE_INLINE aPtrType operator->(){\
            return &operator*();\
        }\
        \
        SOLAIRE_INLINE operator aRefType(){\
            return operator*();\
        }\
        \
        SOLAIRE_INLINE aRefType operator=(aRefType aValue){\
            return operator*() = aValue;\
        }\
    };

    #define SOLAIRE_READ_WRITE_PROPERTY(aName, aParentType, aRefType, aPtrType, aCode)\
    class aName{\
    private:\
        aName(const aName&) = delete;\
        aName(aName&&) = delete;\
        aName& operator=(const aName&) = delete;\
        aName& operator=(aName&&) = delete;\
    private:\
        aParentType& mParent;\
    public:\
        constexpr aName(aParentType& aParent):\
            mParent(aParent)\
        {}\
        \
        aRefType operator*(){\
            aCode\
        }\
        \
        SOLAIRE_INLINE aPtrType operator->(){\
            return &operator*();\
        }\
        \
        SOLAIRE_INLINE operator aRefType(){\
            return operator*();\
        }\
        \
        SOLAIRE_INLINE aRefType operator=(aRefType aValue){\
            return operator*() = aValue;\
        }\
        \
        SOLAIRE_INLINE const aRefType operator*() const{\
            return const_cast<aName*>(this)->operator*();\
        }\
        \
        SOLAIRE_INLINE const aPtrType operator->() const{\
            return &operator*();\
        }\
        \
        SOLAIRE_INLINE operator const aRefType() const{\
            return operator*();\
        }\
    };
}


#endif
