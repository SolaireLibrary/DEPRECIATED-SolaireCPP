#ifndef SOLAIRE_CORE_ALLOCATOR_HPP
#define SOLAIRE_CORE_ALLOCATOR_HPP

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
	\file Allocator.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 25th September 2015
	Last Modified	: 25th September 2015
*/

#include <type_traits>
#include <map>
#include <vector>
#include "SmartPointer.hpp"

namespace Solaire{ namespace Core{

    class Allocator{
    public:
        typedef void(*DestructorFn)(void*);

        virtual ~Allocator(){}

        virtual void* Allocate(const size_t aBytes) = 0;
        virtual void Deallocate(void* const aObject, const size_t aBytes) = 0;
        virtual void OnDestroyed(void* const aObject, const DestructorFn aFn) = 0;

        template<class T>
        typename std::enable_if<! std::is_fundamental<T>::value, void>::type
        RegisterDestructor(T* const aObject){
            OnDestroyed(aObject, [](void* const aObject){static_cast<T*>(aObject)->~T();});
        }

        template<class T>
        T* Allocate(){
            void*(Allocator::*RawAllocate)(const size_t) = &Allocator::Allocate;
            return static_cast<T*>((this->*RawAllocate)(sizeof(T)));
        }

        template<class T>
        T* Allocate(const size_t aCount){
            void*(Allocator::*RawAllocate)(const size_t) = &Allocator::Allocate;
            return static_cast<T*>((this->*RawAllocate)(sizeof(T) * aCount));
        }

        template<class T>
        void Deallocate(T* const aObject){
            void(Allocator::*RawDeallocate)(void* const, const size_t) = &Allocator::Deallocate;
            (this->*RawDeallocate)(aObject, sizeof(T));
        }

        template<class T>
        void Deallocate(T* const aObject, const size_t aCount){
            void(Allocator::*RawDeallocate)(void* const, const size_t) = &Allocator::Deallocate;
            (this->*RawDeallocate)(aObject, sizeof(T) * aCount);
        }

        template<class T>
        T* AllocateAndRegister(){
            T* const tmp = Allocate<T>();
            RegisterDestructor<T>(tmp);
            return tmp;
        }

        template<class T>
        T* AllocateAndRegister(const size_t aCount){
            T* const tmp = Allocate<T>(aCount);
            for(size_t i = 0; i < aCount; ++i){
                RegisterDestructor<T>(tmp + i);
            }
            return tmp;
        }

        typedef std::function<void(void*)> UniqueDeleter;

        template<class T>
        using SharedPointer = std::shared_ptr<T>;

        template<class T>
        using UniquePointer = std::unique_ptr<T, UniqueDeleter>;

        template<class T, class ...PARAMS>
        SharedPointer<T> SharedAllocate(PARAMS&&... aParams){
            return SharedPointer<T>(
                new(AllocateAndRegister<T>()) T(aParams...),
                [&](T* const aObject){
                    Deallocate<T>(aObject);
                }
            );
        }

        template<class T, class ...PARAMS>
        UniquePointer<T> UniqueAllocate(PARAMS&&... aParams){
            return UniquePointer<T>(
                new(AllocateAndRegister<T>()) T(aParams...),
                UniqueDeleter([&](void* aObject){
                    Deallocate<T>(static_cast<T*>(aObject));
                })
            );
        }
    };

    class DestructorMapAllocator : public Allocator{
    private:
        //! \bug DestructorMapAllocator does not check mDestructorList in own constructor
        //! \bug DestructorMapAllocator is not type safe
        std::map<void*, std::vector<DestructorFn>> mDestructorList;
    protected:
        void OnDeallocate(void* const aObject){
            auto it = mDestructorList.find(aObject);
            if(it == mDestructorList.end()) return;
            const std::vector<DestructorFn>& list = it->second;
            for(DestructorFn i : list){
                i(aObject);
            }
            mDestructorList.erase(it);
        }
    public:
        virtual ~DestructorMapAllocator(){

        }

        virtual void OnDestroyed(void* const aObject, const DestructorFn aFn) override{
            auto it = mDestructorList.find(aObject);
            if(it == mDestructorList.end()) it = mDestructorList.emplace(aObject, std::vector<DestructorFn>()).first;
            it->second.push_back(aFn);
        }
    };

    Allocator& GetDefaultAllocator(){
        class DefaultAllocator : public DestructorMapAllocator{
        public:
            void* Allocate(const size_t aBytes) override{
                void* const tmp = operator new(aBytes);
                return tmp;
            }

            void Deallocate(void* const aObject, const size_t aBytes) override{
                OnDeallocate(aObject);
                operator delete(aObject);
            }
        };

        static DefaultAllocator ALLOCATOR;
        return ALLOCATOR;
    }

    template <class T, Allocator& ALLOCATOR>
    class STLAllocator {
    public:
        typedef T           value_type;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;

        template <class U>
        struct rebind {
            typedef STLAllocator<U, ALLOCATOR> other;
        };

        pointer address (reference aValue) const {
            return &aValue;
        }

        const_pointer address (const_reference aValue) const {
            return &aValue;
        }

        STLAllocator() throw(){

        }

        STLAllocator(const STLAllocator&) throw(){

        }

        template <class U>
        STLAllocator(const STLAllocator<U, ALLOCATOR>&) throw(){

        }

        ~STLAllocator() throw(){

        }

        size_type max_size() const throw(){
            return std::numeric_limits<std::size_t>::max() / sizeof(T);;
        }

        pointer allocate(size_type aCount, const void* = 0){
            return ALLOCATOR.Allocate<T>(aCount);
        }

        void construct(pointer aAddress, const T& aValue){
            new(aAddress)T(aValue);
        }

        void destroy(pointer aAddress){
            aAddress->~T();
        }

        void deallocate(pointer aAddress, size_type aCount){
            ALLOCATOR.Deallocate<T>(aAddress, aCount);
        }
    };

    template <class T1, class T2, const Allocator& ALLOCATOR>
    bool operator==(const STLAllocator<T1, ALLOCATOR>&, const STLAllocator<T2, ALLOCATOR>&) throw(){
        return true;
    }

    template <class T1, class T2, const Allocator& ALLOCATOR>
    bool operator!=(const STLAllocator<T1, ALLOCATOR>&, const STLAllocator<T2, ALLOCATOR>&) throw(){
        return false;
    }

}}

#endif
