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
    class ReadOnlyObjectProperty{
    public:
        typedef typename PassTypes<Type, PassMode>::Type Return;
        typedef typename PassTypes<Type, PassMode>::ConstType ConstReturn;
    private:
        ReadOnlyObjectProperty(const ReadOnlyObjectProperty&) = delete;
        ReadOnlyObjectProperty(ReadOnlyObjectProperty&&) = delete;
        ReadOnlyObjectProperty& operator=(const ReadOnlyObjectProperty&) = delete;
        ReadOnlyObjectProperty& operator=(ReadOnlyObjectProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr ReadOnlyObjectProperty(Parent& aParent):
            mParent(aParent)
        {}

        inline ConstReturn operator*() const{
            return mParent.*Member;
        }

        inline const Type* operator->() const{
            return &(mParent.*Member);
        }
    };

    template<class Parent, class Type, Type Parent::* const Member, const int PassMode = TypeTraits<Type>::PassMode>
    class ObjectProperty{
    public:
        typedef typename PassTypes<Type, PassMode>::Type Return;
        typedef typename PassTypes<Type, PassMode>::ConstType ConstReturn;
    private:
        ObjectProperty(const ObjectProperty&) = delete;
        ObjectProperty(ObjectProperty&&) = delete;
        ObjectProperty& operator=(const ObjectProperty&) = delete;
        ObjectProperty& operator=(ObjectProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr ObjectProperty(Parent& aParent):
            mParent(aParent)
        {}

        inline Return operator*(){
            return mParent.*Member;
        }

        inline Type* operator->(){
            return &(mParent.*Member);
        }

        inline ConstReturn operator*() const{
            return mParent.*Member;
        }

        inline const Type* operator->() const{
            return &(mParent.*Member);
        }
    };

    template<class Parent, class Type, Type Parent::* const Member>
    using ReadOnlyCopyObjectProperty = ReadOnlyObjectProperty<Parent, Type, Member, PASS_BY_VALUE>;

    template<class Parent, class Type, Type Parent::* const Member>
    using ReadOnlyReferenceObjectProperty = ReadOnlyObjectProperty<Parent, Type, Member, PASS_BY_REFERENCE>;

    template<class Parent, class Type, Type Parent::* const Member>
    using CopyObjectProperty = ObjectProperty<Parent, Type, Member, PASS_BY_VALUE>;

    template<class Parent, class Type, Type Parent::* const Member>
    using ReferenceObjectProperty = ObjectProperty<Parent, Type, Member, PASS_BY_REFERENCE>;

    ////

    template<class Parent, class Type, Type* Parent::* const Member, const int PassMode = TypeTraits<Type>::PassMode>
    class ReadOnlyDereferenceObjectProperty{
    public:
        typedef typename PassTypes<Type, PassMode>::Type Return;
        typedef typename PassTypes<Type, PassMode>::ConstType ConstReturn;
    private:
        ReadOnlyDereferenceObjectProperty(const ReadOnlyDereferenceObjectProperty&) = delete;
        ReadOnlyDereferenceObjectProperty(ReadOnlyDereferenceObjectProperty&&) = delete;
        ReadOnlyDereferenceObjectProperty& operator=(const ReadOnlyDereferenceObjectProperty&) = delete;
        ReadOnlyDereferenceObjectProperty& operator=(ReadOnlyDereferenceObjectProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr ReadOnlyDereferenceObjectProperty(Parent& aParent):
            mParent(aParent)
        {}

        inline ConstReturn operator*() const{
            return *(mParent.*Member);
        }

        inline const Type* operator->() const{
            return mParent.*Member;
        }
    };

    template<class Parent, class Type, Type* Parent::* const Member, const int PassMode = TypeTraits<Type>::PassMode>
    class DereferenceObjectProperty{
    public:
        typedef typename PassTypes<Type, PassMode>::Type Return;
        typedef typename PassTypes<Type, PassMode>::ConstType ConstReturn;
    private:
        DereferenceObjectProperty(const DereferenceObjectProperty&) = delete;
        DereferenceObjectProperty(DereferenceObjectProperty&&) = delete;
        DereferenceObjectProperty& operator=(const DereferenceObjectProperty&) = delete;
        DereferenceObjectProperty& operator=(DereferenceObjectProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr DereferenceObjectProperty(Parent& aParent):
            mParent(aParent)
        {}

        inline Return operator*(){
            return *(mParent.*Member);
        }

        inline Type* operator->(){
            return mParent.*Member;
        }

        inline ConstReturn operator*() const{
            return *(mParent.*Member);
        }

        inline const Type* operator->() const{
            return mParent.*Member;
        }
    };

    template<class Parent, class Type, Type* Parent::* const Member>
    using ReadOnlyCopyDereferenceObjectProperty = ReadOnlyDereferenceObjectProperty<Parent, Type, Member, PASS_BY_VALUE>;

    template<class Parent, class Type, Type* Parent::* const Member>
    using ReadOnlyReferenceDereferenceObjectProperty = ReadOnlyDereferenceObjectProperty<Parent, Type, Member, PASS_BY_REFERENCE>;

    template<class Parent, class Type, Type* Parent::* const Member>
    using CopyDereferenceObjectProperty = DereferenceObjectProperty<Parent, Type, Member, PASS_BY_VALUE>;

    template<class Parent, class Type, Type* Parent::* const Member>
    using ReferenceDereferenceObjectProperty = DereferenceObjectProperty<Parent, Type, Member, PASS_BY_REFERENCE>;

    ////

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
        Parent& mParent;
    public:
        constexpr ReadOnlyValueProperty(Parent& aParent):
            mParent(aParent)
        {}

        operator ConstReturn() const{
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

        operator ConstReturn() const{
            return mParent.*Member;
        }

        operator const Return(){
            return mParent.*Member;
        }

        Type& operator=(ConstReturn aValue){
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
        Parent& mParent;
    public:
        constexpr ReadOnlyDereferenceValueProperty(Parent& aParent):
            mParent(aParent)
        {}

        operator ConstReturn() const{
            return *mParent.*Member;
        }
    };

    template<class Parent, class Type, Type Parent::* const Member, const int PassMode = TypeTraits<Type>::PassMode>
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

        operator ConstReturn() const{
            return *mParent.*Member;
        }

        operator const Return(){
            return *mParent.*Member;
        }

        Type& operator=(ConstReturn aValue){
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

    template<class Parent, class Return, Return(*Funtion)(const Parent&)>
    class FuntionProperty{
    private:
        FuntionProperty(const FuntionProperty&) = delete;
        FuntionProperty(FuntionProperty&&) = delete;
        FuntionProperty& operator=(const FuntionProperty&) = delete;
        FuntionProperty& operator=(FuntionProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr FuntionProperty(Parent& aParent):
            mParent(aParent)
        {}

        operator Return() const{
            Function(mParent);
        }
    };
}


#endif
