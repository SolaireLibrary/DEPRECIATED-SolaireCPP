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
#include "Memory/SmartPointer.hpp"

namespace Solaire{ namespace Core{

    class Allocator{
    public:
        typedef void(*DestructorFn)(void*);

        virtual ~Allocator(){}

        virtual void* Allocate(const size_t aBytes) = 0;
        virtual void Deallocate(void* const aObject, const size_t aBytes) = 0;
        virtual void OnDestroyed(void* const aObject, const DestructorFn aFn) = 0;

        template<class T>
        void RegisterDestructor(T* const aObject){
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
    };

    class DestructorMapAllocator : public Allocator{
    private:
        //! \bug DestructorMapAllocator is not type safe
        std::map<void*, std::vector<DestructorFn>> mDestructorList;
    protected:
        void OnAllocate(void* const aObject){
            mDestructorList.emplace(aObject, std::vector<DestructorFn>());
        }

        void OnDeallocate(void* const aObject){
            auto it = mDestructorList.find(aObject);
            if(it == mDestructorList.end()) throw std::runtime_error("Core::DestructorMapAllocator : Could not find allocated object");
            const std::vector<DestructorFn>& list = it->second;
            for(DestructorFn i : list){
                i(aObject);
            }
            mDestructorList.erase(it);
        }
    public:
        virtual void OnDestroyed(void* const aObject, const DestructorFn aFn) override{
            auto it = mDestructorList.find(aObject);
            if(it == mDestructorList.end()) throw std::runtime_error("Core::DestructorMapAllocator : Could not find allocated object");
            it->second.push_back(aFn);
        }
    };

    Allocator& GetDefaultAllocator(){
        class DefaultAllocator : public DestructorMapAllocator{
        public:
            void* Allocate(const size_t aBytes) override{
                void* const tmp = operator new(aBytes);
                OnAllocate(tmp);
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

    #define SolaireSmartAllocate(aAllocator, aType, aParams)\
        Core::SharedPointer<aType>(\
            new(aAllocator.Allocate(sizeof(aType))) aType aParams,\
            [&](aType* const aObject){\
                aObject->~aType();\
                aAllocator.Deallocate(aObject, sizeof(aType));\
            }\
        )
}}

#endif
