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
        Parent& mParent;
    public:
        constexpr ReadOnlyValueProperty(Parent& aParent):
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
    using ReadOnlyCopyValueProperty = ReadOnlyValueProperty<Parent, Type, Member, PASS_BY_VALUE>;

    template<class Parent, class Type, Type Parent::* const Member>
    using ReadOnlyReferenceValueProperty = ReadOnlyValueProperty<Parent, Type, Member, PASS_BY_REFERENCE>;

    template<class Parent, class Type, Type Parent::* const Member>
    using CopyValueProperty = ValueProperty<Parent, Type, Member, PASS_BY_VALUE>;

    template<class Parent, class Type, Type Parent::* const Member>
    using ReferenceValueProperty = ValueProperty<Parent, Type, Member, PASS_BY_REFERENCE>;

    ////

    template<class Parent, class Type, Type* Parent::* const Member, const int PassMode = TypeTraits<Type>::PassMode>
    class ReadOnlyDereferenceProperty{
    public:
        typedef typename PassTypes<Type, PassMode>::Type Return;
        typedef typename PassTypes<Type, PassMode>::ConstType ConstReturn;
    private:
        ReadOnlyDereferenceProperty(const ReadOnlyDereferenceProperty&) = delete;
        ReadOnlyDereferenceProperty(ReadOnlyDereferenceProperty&&) = delete;
        ReadOnlyDereferenceProperty& operator=(const ReadOnlyDereferenceProperty&) = delete;
        ReadOnlyDereferenceProperty& operator=(ReadOnlyDereferenceProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr ReadOnlyDereferenceProperty(Parent& aParent):
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
    class DereferenceProperty{
    public:
        typedef typename PassTypes<Type, PassMode>::Type Return;
        typedef typename PassTypes<Type, PassMode>::ConstType ConstReturn;
    private:
        DereferenceProperty(const DereferenceProperty&) = delete;
        DereferenceProperty(DereferenceProperty&&) = delete;
        DereferenceProperty& operator=(const DereferenceProperty&) = delete;
        DereferenceProperty& operator=(DereferenceProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr DereferenceProperty(Parent& aParent):
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

    template<class Parent, class Type, Type Parent::* const Member>
    using ReadOnlyCopyDereferenceProperty = ReadOnlyDereferenceProperty<Parent, Type, Member, PASS_BY_VALUE>;

    template<class Parent, class Type, Type Parent::* const Member>
    using ReadOnlyReferenceDereferenceProperty = ReadOnlyDereferenceProperty<Parent, Type, Member, PASS_BY_REFERENCE>;

    template<class Parent, class Type, Type Parent::* const Member>
    using CopyDereferenceProperty = DereferenceProperty<Parent, Type, Member, PASS_BY_VALUE>;

    template<class Parent, class Type, Type Parent::* const Member>
    using ReferenceDereferenceProperty = DereferenceProperty<Parent, Type, Member, PASS_BY_REFERENCE>;

    ////
}


#endif
