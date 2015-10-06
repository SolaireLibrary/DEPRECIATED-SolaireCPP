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

namespace Solaire{

    enum class PropertyReturn{
        COPY,
        REFERENCE,
        MOVE
    };

    template<class T, const PropertyReturn ReturnMode>
    struct PropertyReturnType{
        typedef void Type;
        typedef void ConstType;
    };

    template<class T>
    struct PropertyReturnType<T, PropertyReturn::COPY>{
        typedef T Type;
        typedef const T ConstType;
    };

    template<class T>
    struct PropertyReturnType<T, PropertyReturn::REFERENCE>{
        typedef T& Type;
        typedef const T& ConstType;
    };

    template<class T>
    struct PropertyReturnType<T, PropertyReturn::MOVE>{
        typedef T&& Type;
        typedef const T&& ConstType;
    };

    template<class Parent, class Type, const PropertyReturn ReturnMode, Type Parent::* const Member>
    class ReadValueProperty{
    public:
        typedef typename PropertyReturnType<Type, ReturnMode>::ConstType ConstReturnType;
    private:
        ReadValueProperty(const ReadValueProperty&) = delete;
        ReadValueProperty(ReadValueProperty&&) = delete;
        ReadValueProperty& operator=(const ReadValueProperty&) = delete;
        ReadValueProperty& operator=(ReadValueProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr ReadValueProperty(Parent& aParent):
            mParent(aParent)
        {}

        inline ConstReturnType operator*() const{
            return mParent.*Member;
        }

        inline const Type* operator->() const{
            return &(mParent.*Member);
        }
    };

    template<class Parent, class Type, const PropertyReturn ReturnMode, Type Parent::* const Member>
    class WriteValueProperty{
    public:
        typedef typename PropertyReturnType<Type, ReturnMode>::Type ReturnType;
        typedef typename PropertyReturnType<Type, ReturnMode>::ConstType ConstReturnType;
    private:
        WriteValueProperty(const WriteValueProperty&) = delete;
        WriteValueProperty(WriteValueProperty&&) = delete;
        WriteValueProperty& operator=(const WriteValueProperty&) = delete;
        WriteValueProperty& operator=(WriteValueProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr WriteValueProperty(Parent& aParent):
            mParent(aParent)
        {}

        inline ReturnType operator*(){
            return mParent.*Member;
        }

        inline Type* operator->(){
            return &(mParent.*Member);
        }

        inline ConstReturnType operator*() const{
            return mParent.*Member;
        }

        inline const Type* operator->() const{
            return &(mParent.*Member);
        }
    };

    template<class Parent, class Type, Type Parent::* const Member>
    using ReadOnlyCopyProperty = ReadValueProperty<Parent, Type, PropertyReturn::COPY, Member>;

    template<class Parent, class Type, Type Parent::* const Member>
    using ReadOnlyReferenceProperty = ReadValueProperty<Parent, Type, PropertyReturn::REFERENCE, Member>;

    template<class Parent, class Type, Type Parent::* const Member>
    using ReadOnlyMoveProperty = ReadValueProperty<Parent, Type, PropertyReturn::MOVE, Member>;

    template<class Parent, class Type, Type Parent::* const Member>
    using CopyProperty = WriteValueProperty<Parent, Type, PropertyReturn::COPY, Member>;

    template<class Parent, class Type, Type Parent::* const Member>
    using ReferenceProperty = WriteValueProperty<Parent, Type, PropertyReturn::REFERENCE, Member>;

    template<class Parent, class Type, Type Parent::* const Member>
    using MoveProperty = WriteValueProperty<Parent, Type, PropertyReturn::MOVE, Member>;

    //

    template<class Parent, class Type, const PropertyReturn ReturnMode, Type* Parent::* const Member>
    class ReadDereferenceValueProperty{
    public:
        typedef typename PropertyReturnType<Type, ReturnMode>::ConstType ConstReturnType;
    private:
        ReadDereferenceValueProperty(const ReadDereferenceValueProperty&) = delete;
        ReadDereferenceValueProperty(ReadDereferenceValueProperty&&) = delete;
        ReadDereferenceValueProperty& operator=(const ReadDereferenceValueProperty&) = delete;
        ReadDereferenceValueProperty& operator=(ReadDereferenceValueProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr ReadDereferenceValueProperty(Parent& aParent):
            mParent(aParent)
        {}

        inline const Type* operator->() const{
            return mParent.*Member;
        }

        inline ConstReturnType operator*() const{
            return *(mParent.*Member);
        }
    };

    template<class Parent, class Type, const PropertyReturn ReturnMode, Type* Parent::* const Member>
    class WriteDereferenceValueProperty{
    public:
        typedef typename PropertyReturnType<Type, ReturnMode>::Type ReturnType;
        typedef typename PropertyReturnType<Type, ReturnMode>::ConstType ConstReturnType;
    private:
        WriteDereferenceValueProperty(const WriteDereferenceValueProperty&) = delete;
        WriteDereferenceValueProperty(WriteDereferenceValueProperty&&) = delete;
        WriteDereferenceValueProperty& operator=(const WriteDereferenceValueProperty&) = delete;
        WriteDereferenceValueProperty& operator=(WriteDereferenceValueProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr WriteDereferenceValueProperty(Parent& aParent):
            mParent(aParent)
        {}

        inline Type* operator->(){
            return mParent.*Member;
        }

        inline ReturnType operator*(){
            return *(mParent.*Member);
        }

        inline const Type* operator->() const{
            return mParent.*Member;
        }

        inline ConstReturnType operator*() const{
            return *(mParent.*Member);
        }
    };


    template<class Parent, class Type, Type* Parent::* const Member>
    using ReadOnlyCopyDreferenceProperty = ReadDereferenceValueProperty<Parent, Type, PropertyReturn::COPY, Member>;

    template<class Parent, class Type, Type* Parent::* const Member>
    using ReadOnlyReferenceDreferenceProperty = ReadDereferenceValueProperty<Parent, Type, PropertyReturn::REFERENCE, Member>;

    template<class Parent, class Type, Type* Parent::* const Member>
    using ReadOnlyMoveDereferenceProperty = ReadDereferenceValueProperty<Parent, Type, PropertyReturn::MOVE, Member>;

    template<class Parent, class Type, Type* Parent::* const Member>
    using CopyDereferenceProperty = WriteDereferenceValueProperty<Parent, Type, PropertyReturn::COPY, Member>;

    template<class Parent, class Type, Type* Parent::* const Member>
    using ReferenceDereferenceProperty = WriteDereferenceValueProperty<Parent, Type, PropertyReturn::REFERENCE, Member>;

    template<class Parent, class Type, Type* Parent::* const Member>
    using MoveDereferenceProperty = WriteDereferenceValueProperty<Parent, Type, PropertyReturn::MOVE, Member>;
}


#endif
