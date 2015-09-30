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
#include <functional>
#include <map>


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
            mDestructor = [=](T* const aPtr){tmp(const_cast<T2*>(aPtr));};

            return *this;
        }

        template<class T2>
        typename std::enable_if<std::is_base_of<T, T2>::value, SmartPointer<T>&>::type
        operator=(SmartPointer<T2>&& aOther){
            Clear();

            mObject = reinterpret_cast<T*>(aOther.operator->());
            aOther.Release();

            std::function<void(T2*)> tmp = aOther.GetDestructor();
            mDestructor = [=](T* const aPtr){tmp((T2*)(aPtr));};

            return *this;
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

        T* Release(T* const aObject, std::function<void(T*)> aDestructor = DefaultDestructor){
            T* tmp = aObject;
            std::swap(tmp, mObject);
            mDestructor = aDestructor;
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

    namespace SharedPointerInternal{
        typedef std::pair<std::function<void(void*)>, uint32_t> SharedData;
        static std::map<void*, SharedData> SHARED_DATA;

        static bool InstanceExists(const void* const aAddress){
            if(aAddress == nullptr) false;
            return SHARED_DATA.find(const_cast<void*>(aAddress)) != SHARED_DATA.end();
        }

        static void CreateInstance(void* const aAddress){
            if(aAddress == nullptr) return;
            auto it = SHARED_DATA.find(const_cast<void*>(aAddress));
            if(it == SHARED_DATA.end()){
                throw std::runtime_error("Core::SharedPointer : Pointer does not exist");
            }else{
                ++(it->second.second);
            }
        }

        static void CreateInstance(void* const aAddress, std::function<void(void*)> aDestructor){
            if(aAddress == nullptr) return;
            auto it = SHARED_DATA.find(const_cast<void*>(aAddress));
            if(it == SHARED_DATA.end()){
                SHARED_DATA.emplace(aAddress, SharedData(aDestructor, 1));
            }else{
                throw std::runtime_error("Core::SharedPointer : Pointer already exists");
            }
        }

        static uint32_t UserCount(void* const aAddress){
            if(aAddress == nullptr) return 0;
            auto it = SHARED_DATA.find(const_cast<void*>(aAddress));
            if(it == SHARED_DATA.end()) return 0;
            return it->second.second;
        }

        void DestroyInstance(void* const aAddress){
            if(aAddress == nullptr) return;
            auto it = SHARED_DATA.find(const_cast<void*>(aAddress));
            if(it == SHARED_DATA.end()) throw std::runtime_error("Core::SharedPointer : Cannot destroy unknown pointer");
            if(--(it->second.second) == 0){
                it->second.first(aAddress);
                SHARED_DATA.erase(it);
            }
        }
    }

    template<class T>
    class SharedPointer{
    private:
        void* mObject;

        static void DefaultDestructor(void* const aObject){
            delete static_cast<T*>(aObject);
        }
    public:
        SharedPointer():
            mObject(nullptr)
        {}

        SharedPointer(T* const aObject):
            mObject(aObject)

        {
            using namespace SharedPointerInternal;
            if(InstanceExists(aObject)){
                CreateInstance(aObject);
            }else{
                CreateInstance(aObject, DefaultDestructor);
            }
        }

        SharedPointer(T* const aObject, std::function<void(T*)> aDestructor):
            mObject(aObject)

        {
            using namespace SharedPointerInternal;
            if(InstanceExists(aObject)){
                CreateInstance(aObject);
            }else{
                CreateInstance(aObject, DefaultDestructor);
            }
        }

        SharedPointer(SharedPointer<T>&& aOther):
            mObject(aOther.mObject)
        {
            SharedPointerInternal::CreateInstance(mObject);
        }

        SharedPointer(const SharedPointer<T>& aOther):
            mObject(aOther.mObject)
        {
            SharedPointerInternal::CreateInstance(mObject);
        }

        template<class T2>
        SharedPointer(SharedPointer<T2>&& aOther, typename std::enable_if<std::is_same<T,  typename std::add_const<T2>::type>::value>::type* = 0):
            mObject(aOther.operator->())

        {
            SharedPointerInternal::CreateInstance(mObject);
        }

        template<class T2>
        SharedPointer(SharedPointer<T2>&& aOther, typename std::enable_if<std::is_base_of<T, T2>::value>::type* = 0):
            mObject(aOther.operator->())

        {
            SharedPointerInternal::CreateInstance(mObject);
        }

        template<class T2>
        SharedPointer(const SharedPointer<T2>& aOther, typename std::enable_if<std::is_same<T,  typename std::add_const<T2>::type>::value>::type* = 0):
            mObject(aOther.operator->())

        {
            SharedPointerInternal::CreateInstance(mObject);
        }

        template<class T2>
        SharedPointer(const SharedPointer<T2>& aOther, typename std::enable_if<std::is_base_of<T, T2>::value>::type* = 0):
            mObject(const_cast<T2*>(aOther.operator->()))

        {
            SharedPointerInternal::CreateInstance(mObject);
        }

        ~SharedPointer(){
            Clear();
        }

        uint32_t UserCount() const{
            return  SharedPointerInternal::UserCount(mObject);
        }

        SharedPointer<T>& operator=(const SharedPointer<T>& aOther){
            Clear();
            mObject = const_cast<void*>(aOther.operator->());
            SharedPointerInternal::CreateInstance(mObject);
            return *this;
        }

        SharedPointer<T>& operator=(SharedPointer<T>&& aOther){
            Clear();
            mObject = aOther.operator->();
            SharedPointerInternal::CreateInstance(mObject);
            return *this;
        }

        template<class T2>
        typename std::enable_if<std::is_same<T,  typename std::add_const<T2>::type>::value, SharedPointer<T>&>::type
        operator=(const SharedPointer<T2>& aOther){
            Clear();
            mObject = const_cast<T2*>(aOther.operator->());
            SharedPointerInternal::CreateInstance(mObject);
            return *this;
        }

        template<class T2>
        typename std::enable_if<std::is_base_of<T, T2>::value, SharedPointer<T>&>::type
        operator=(const SharedPointer<T2>& aOther){
            Clear();
            mObject = const_cast<T2*>(aOther.operator->());
            SharedPointerInternal::CreateInstance(mObject);
            return *this;
        }

        template<class T2>
        typename std::enable_if<std::is_same<T,  typename std::add_const<T2>::type>::value, SharedPointer<T>&>::type
        operator=(SharedPointer<T2>&& aOther){
            Clear();
            mObject = aOther.operator->();
            SharedPointerInternal::CreateInstance(mObject);
            return *this;
        }

        template<class T2>
        typename std::enable_if<std::is_base_of<T, T2>::value, SharedPointer<T>&>::type
        operator=(SharedPointer<T2>&& aOther){
            Clear();
            mObject = aOther.operator->();
            SharedPointerInternal::CreateInstance(mObject);
            return *this;
        }

        void Clear(){
            if(mObject != nullptr){
                SharedPointerInternal::DestroyInstance(mObject);
                mObject = nullptr;
            }
        }

       void Release(){
            Clear();
        }

        void Release(T* const aObject, std::function<void(T*)> aDestructor = DefaultDestructor){
            operator=(SmartPointer<T>(aObject, aDestructor));
        }

        void swap(SmartPointer<T>& aOther){
            std::swap(mObject, aOther.mObject);
        }

        T& operator*(){
            return *static_cast<T*>(mObject);
        }

        const T& operator*() const{
            return *static_cast<T*>(mObject);
        }

        T* operator->(){
            return static_cast<T*>(mObject);
        }

        const T* operator->() const{
            return static_cast<T*>(mObject);
        }
    };

}}

#endif
