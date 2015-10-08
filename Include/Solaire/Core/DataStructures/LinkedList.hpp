#ifndef SOLAIRE_LINKED_LIST_HPP
#define SOLAIRE_LINKED_LIST_HPP

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
	\file LinkedList.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 7th October 2015
	Last Modified	: 7th October 2015
*/

#include <cstdint>
#include "..\Memory\Allocator.hpp"
#include "..\Property.hpp"

namespace Solaire{

    template<class TYPE>
	class LinkedList{
	public:
		typedef TYPE Type;
		typedef const TYPE ConstType;
		typedef Type& Reference;
		typedef ConstType& ConstReference;
		typedef Type&& Move;
		typedef Type* Pointer;
		typedef ConstType* ConstPointer;
		typedef Pointer Iterator;
		typedef ConstPointer ConstIterator;
		typedef ReverseIteratorWrapper<Type, Iterator> ReverseIterator;
		typedef ConstIteratorWrapper<Type, ReverseIterator> ConstReverseIterator;
		typedef LinkedList<TYPE> Self;
    private:
        struct Node{
        private:
            uint8_t object[sizeof(Type)];
        public:
            Node* previous;
            Node* next;
        public:
            Node():
                previous(nullptr),
                next(nullptr)
            {}

            Type* GetObject() const{
                return reinterpret_cast<Type*>(object);
            }
        };
    public:
        /*class EmptyProperty{
        private:
            EmptyProperty(const EmptyProperty&) = delete;
            EmptyProperty(EmptyProperty&&) = delete;
            EmptyProperty& operator=(const EmptyProperty&) = delete;
            EmptyProperty& operator=(EmptyProperty&&) = delete;
        private:
            LinkedList<Type>& mParent;
        public:
            constexpr EmptyProperty(LinkedList<Type>& aParent):
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
        };*/
	private:
        Allocator* mAllocator;
		Node* mFirst;
		Node* mLast;
		uint32_t mSize;
    private:
        static void LinkLeft(Node* const aLeft, Node* const aMiddle){
            aLeft->next = aMiddle;
            aMiddle->previous = aLeft;
        }

        static void LinkRight(Node* const aMiddle, Node* const aRight){
            aMiddle->next = aRight;
            aRight->previous = aMiddle;
        }

        static void LinkBoth(Node* const aLeft, Node* const aMiddle, Node* const aRight){
            if(aLeft) LinkLeft(aLeft, aMiddle);
            if(aRight) LinkRight(aMiddle, aRight);
        }

        Node* AllocateNode(ConstReference aValue){
            Node* const node = new(mAllocator->Allocate<Node>()) Node();
            new(node->GetObject()) Type(aValue);
            mAllocator->OnDestroyed(node, [](void* const aNode)->void{
                Node* const node = static_cast<Node*>(aNode);
                node->GetObject()->~Type();
                node->~Node();
            });
        }
    public:
        union{
            ReadOnlyValueProperty<LinkedList<Type>, uint32_t, &LinkedList<Type>::mSize> pSize;
            DereferenceValueProperty<LinkedList<Type>, Allocator, &LinkedList<Type>::mAllocator> pAllocator;
        };
    public:
        LinkedList(Allocator& aAllocator):
            mAllocator(&aAllocator),
            mFirst(nullptr),
            mLast(nullptr),
            mSize(0)
        {}

        ~LinkedList(){
            Clear();
        }

        Allocator& GetAllocator() const{
            return *mAllocator;
        }

        Reference PushBack(ConstReference aValue){
            Node* const node = AllocateNode(aValue);

            if(mFirst){
                LinkLeft(mLast, node);
            }else{
                mFirst = node;
            }
            ++mSize;
            mLast = node;
            return *node->GetObject();
        }

        Reference PushFront(ConstReference aValue){
            Node* const node = AllocateNode(aValue);

            if(mFirst){
                LinkLeft(node, mFirst);
            }else{
                mLast = node;
            }

            ++mSize;
            mFirst = node;
            return *node->GetObject();
        }

        void Clear(){
            Node* i = mLast;
            Node*j = nullptr;
            while(i){
                j = i->previous;
                mAllocator->Deallocate<Node>(i);
                i = j;
            }
            mSize = 0;
            mFirst = nullptr;
            mLast = nullptr;
        }
	};
}

#endif
