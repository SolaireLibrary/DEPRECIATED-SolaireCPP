#ifndef SOLAIRE_CORE_SMART_POINTER_HPP
#define SOLAIRE_CORE_SMART_POINTER_HPP

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
	\file SmartPointer.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 30th September 2015
	Last Modified	: 30th September 2015
*/

#include <type_traits>
#include <memory>
#include <functional>

namespace Solaire{ namespace Core{

    template<class T>
    class SmartPointer{
    private:
        std::function<void(T*)> mDestructor;
        T* mObject;

        SmartPointer(const SmartPointer<T>&) = delete;
        SmartPointer& operator=(const SmartPointer<T>&) = delete;

        static void DefaultDestructor(T* const aObject){
            delete aObject;
        }
    public:
        SmartPointer():
            mDestructor(DefaultDestructor),
            mObject(nullptr)
        {}

        SmartPointer(T* const aObject):
            mDestructor(DefaultDestructor),
            mObject(aObject)

        {}

        SmartPointer(T* const aObject, std::function<void(T*)> aDestructor):
            mDestructor(aDestructor),
            mObject(aObject)

        {}

        SmartPointer(SmartPointer<T>&& aOther):
            mDestructor(aOther.mDestructor),
            mObject(aOther.mObject)

        {
            aOther.mObject = nullptr;
        }

        template<class T2>
        SmartPointer(SmartPointer<T2>&& aOther, typename std::enable_if<std::is_same<T,  typename std::add_const<T2>::type>::value>::type* = 0):
            mDestructor(),
            mObject(nullptr)

        {
            operator=(std::move(aOther));
        }

        template<class T2>
        SmartPointer(SmartPointer<T2>&& aOther, typename std::enable_if<std::is_base_of<T, T2>::value>::type* = 0):
            mDestructor(),
            mObject(nullptr)

        {
            operator=(std::move(aOther));
        }

        ~SmartPointer(){
            Clear();
        }

        std::function<void(T*)> GetDestructor() const{
            return mDestructor;
        }

        SmartPointer<T>& operator=(SmartPointer<T>&& aOther){
            Clear();

            mDestructor = aOther.mDestructor;
            mObject = aOther.mObject;
            aOther.mObject = nullptr;

            return *this;
        }

        template<class T2>
        typename std::enable_if<std::is_same<T,  typename std::add_const<T2>::type>::value, SmartPointer<T>&>::type
        operator=(SmartPointer<T2>&& aOther){
            Clear();

            mObject = aOther.operator->();
            aOther.Release();

            std::function<void(T2*)> tmp = aOther.GetDestructor();
            mDestructor = [=](T* const aObject){tmp(const_cast<T2*>(aObject));};
        }

        template<class T2>
        typename std::enable_if<std::is_base_of<T, T2>::value, SmartPointer<T>&>::type
        operator=(SmartPointer<T2>&& aOther){
            Clear();

            mObject = reinterpret_cast<T*>(aOther.operator->());
            aOther.Release();

            std::function<void(T2*)> tmp = aOther.GetDestructor();
            mDestructor = [=](T* const aObject){tmp(reinterpret_cast<T2*>(aObject));};
        }

        void Clear(){
            if(mObject != nullptr){
                mDestructor(mObject);
                mObject = nullptr;
            }
        }

        T* Release(){
            T* tmp = nullptr;
            std::swap(tmp, mObject);
            return tmp;
        }

        void swap(SmartPointer<T>& aOther){
            std::swap(mObject, aOther.mObject);
            std::swap(mDestructor, aOther.mDestructor);
        }

        T& operator*(){
            return *mObject;
        }

        const T& operator*() const{
            return *mObject;
        }

        T* operator->(){
            return mObject;
        }

        const T* operator->() const{
            return mObject;
        }
    };

}}

#endif
