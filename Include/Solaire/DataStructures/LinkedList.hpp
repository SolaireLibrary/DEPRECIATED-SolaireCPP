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
#include "..\Iterators\IteratorMacros.hpp"
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
		//typedef ConstPointer ConstIterator;
		//typedef ReverseIteratorWrapper<Type, Iterator> ReverseIterator;
		//typedef ConstIteratorWrapper<Type, ReverseIterator> ConstReverseIterator;
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
        SOLAIRE_BASIC_ITERATOR(
            Iterator,
            Type,
            Node*,
            return mData = mData == nullptr ? nullptr : mData->next;,
            return mData = mData == nullptr ? nullptr : mData->previous;,
            return *mData->GetObject();
        );

        SOLAIRE_CONST_BASIC_ITERATOR(
            ConstIterator,
            Type,
            Node*,
            return mData = mData == nullptr ? nullptr : mData->next;,
            return mData = mData == nullptr ? nullptr : mData->previous;,
            return *mData->GetObject();
        );

        SOLAIRE_BASIC_ITERATOR(
            ReverseIterator,
            Type,
            Node*,
            return mData = mData == nullptr ? nullptr : mData->previous;,
            return mData = mData == nullptr ? nullptr : mData->next;,
            return *mData->GetObject();
        );

        SOLAIRE_CONST_BASIC_ITERATOR(
            ConstReverseIterator,
            Type,
            Node*,
            return mData = mData == nullptr ? nullptr : mData->previous;,
            return mData = mData == nullptr ? nullptr : mData->next;,
            return *mData->GetObject();
        );
	private:
        Allocator* mAllocator;
		Node* mFirst;
		Node* mLast;
		uint32_t mSize;
    private:
        static void LinkForwards(Node* const aLeft, Node* const aMiddle){
            aLeft->next = aMiddle;
            aMiddle->previous = aLeft;
        }

        static void LinkBackwards(Node* const aMiddle, Node* const aRight){
            aMiddle->next = aRight;
            aRight->previous = aMiddle;
        }

        static void LinkBoth(Node* const aLeft, Node* const aMiddle, Node* const aRight){
            if(aLeft) LinkForwards(aLeft, aMiddle);
            if(aRight) LinkBackwards(aMiddle, aRight);
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
    private:
        bool pEmptyFn() const{return mSize == 0;}
        Type& pFrontFn(){return *mFirst->GetObject();}
        const Type& pFrontConstFn() const{return *mFirst->GetObject();}
        Type& pBackFn(){return *mLast->GetObject();}
        const Type& pBackConstFn() const{return *mLast->GetObject();}
    public:
        union{
            R_Val_FunctionProperty<LinkedList<Type>, bool, &LinkedList<Type>::pEmptyFn> pEmpty;
            RW_Ref_FunctionProperty<LinkedList<Type>, Type, &LinkedList<Type>::pFrontConstFn, &LinkedList<Type>::pFrontFn> pFront;
            RW_Ref_FunctionProperty<LinkedList<Type>, Type, &LinkedList<Type>::pBackConstFn, &LinkedList<Type>::pBackFn> pBack;
            R_Val_MemberProperty<LinkedList<Type>, uint32_t, &LinkedList<Type>::mSize> pSize;
            RW_Ref_MemberProperty<LinkedList<Type>, Allocator, &LinkedList<Type>::mAllocator> pAllocator;
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

        Reference InsertBefore(const ConstIterator aPosition, ConstReference aValue){
            Node* const node = &*(aPosition);
            Node* const newNode = AllocateNode(aValue);

            if(node == mFirst){
                LinkBackwards(newNode, node);
                mFirst = newNode;
            }else{
                LinkForwards(node->previous, newNode);
                LinkBackwards(newNode, node);
            }
            ++mSize;
            return *newNode->GetObject();
        }

        Reference InsertAfter(const ConstIterator aPosition, ConstReference aValue){
            Node* const node = &*(aPosition);
            Node* const newNode = AllocateNode(aValue);

            if(node == mLast){
                LinkForwards(node, newNode);
                mLast = newNode;
            }else{
                LinkForwards(newNode, node->next);
                LinkForwards(node, newNode);
            }
            ++mSize;
            return *newNode->GetObject();
        }

        Reference PushBack(ConstReference aValue){
            Node* const node = AllocateNode(aValue);

            if(mFirst){
                LinkForwards(mLast, node);
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
                LinkForwards(node, mFirst);
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

        Iterator begin(){
            return Iterator(mFirst);
        }

        ConstIterator begin() const{
            return Iterator(mFirst);
        }

        Iterator end(){
            return Iterator(nullptr);
        }

        ConstIterator end() const{
            return Iterator(nullptr);
        }

        ReverseIterator rbegin(){
            return ReverseIterator(mLast);
        }

        ConstReverseIterator rbegin() const{
            return ConstReverseIterator(mLast);
        }

        ReverseIterator rend(){
            return ReverseIterator(nullptr);
        }

        ConstReverseIterator rend() const{
            return ConstReverseIterator(nullptr);
        }
	};
}

#endif
