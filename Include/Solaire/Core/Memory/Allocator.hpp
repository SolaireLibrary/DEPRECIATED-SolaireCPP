#ifndef SOLAIRE_ALLOCATOR_HPP
#define SOLAIRE_ALLOCATOR_HPP

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

#include <limits>
#include <type_traits>
#include <memory>

namespace Solaire{
    template<class T>
    using SharedPointer = std::shared_ptr<T>;

    template<class T>
    using UniquePointer = std::unique_ptr<T, std::function<void(void*)>>;

    class Allocator{
    public:
        virtual ~Allocator(){}

        virtual uint32_t GetAllocatedBytes() const = 0;
        virtual uint32_t GetFreeBytes() const = 0;

        virtual void* Allocate(const size_t aBytes) = 0;
        virtual void Deallocate(void* const aObject, const size_t aBytes) = 0;
    };

    template<class T, class ...PARAMS>
    static SharedPointer<T> SharedAllocate(Allocator& aAllocator, PARAMS&&... aParams){
        return SharedPointer<T>(
            new(aAllocator.Allocate(sizeof(T))) T(aParams...),
            [&](T* const aObject){
                aAllocator.Deallocate(aObject, sizeof(T));
                aObject->~T();
            }
        );
    }

    template<class T, class ...PARAMS>
    static UniquePointer<T> UniqueAllocate(Allocator& aAllocator, PARAMS&&... aParams){
        return UniquePointer<T>(
            new(aAllocator.Allocate(sizeof(T))) T(aParams...),
            [&](void* aObject){
                aAllocator.Deallocate(aObject, sizeof(T));
                static_cast<T*>(aObject)->~T();
            }
        );
    }


    Allocator& GetDefaultAllocator(){
        class DefaultAllocator : public Allocator{
        private:
            uint32_t mAllocatedBytes;
        public:
            DefaultAllocator():
                mAllocatedBytes(0)
            {}

            uint32_t GetAllocatedBytes() const override{
                return mAllocatedBytes;
            }

            uint32_t GetFreeBytes() const override{
                return UINT32_MAX - mAllocatedBytes;
            }

            void* Allocate(const size_t aBytes) override{
                void* const tmp = operator new(aBytes);
                mAllocatedBytes += aBytes;
                return tmp;
            }

            void Deallocate(void* const aObject, const size_t aBytes) override{
                operator delete(aObject);
                mAllocatedBytes -= aBytes;
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
            return ALLOCATOR.Allocate(sizeof(T) * aCount);
        }

        void construct(pointer aAddress, const T& aValue){
            new(aAddress)T(aValue);
        }

        void destroy(pointer aAddress){
            aAddress->~T();
        }

        void deallocate(pointer aAddress, size_type aCount){
            ALLOCATOR.Deallocate(aAddress, sizeof(T) * aCount);
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

}

#endif
