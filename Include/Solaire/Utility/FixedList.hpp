#ifndef SOLAIRE_UTILITY_FIXED_LIST_HPP
#define SOLAIRE_UTILITY_FIXED_LIST_HPP

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
	\file FixedList.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 20th September 2015
	Last Modified	: 20th September 2015
*/

#include <cstdint>
#include <stdexcept>

namespace Solaire{ namespace Utility{

    template<class TYPE, const size_t SIZE>
	class FixedList{
	public:
		typedef TYPE data_t;
		typedef data_t& ref_t;
		typedef const data_t& const_ref_t;
		typedef data_t&& move_t;
		typedef data_t* pointer_t;
		typedef const data_t* const_pointer_t;
	private:
		class Node{
		private:
			uint8_t mData[sizeof(data_t)];

			Node(Node&& aOther) = delete;
			Node(const Node& aOther) = delete;
			Node& operator=(Node&& aOther) = delete;
			Node& operator=(const Node& aOther) = delete;
		public:
			Node* previous;
			Node* next;
			struct{
				uint8_t live : 1;
			};

			Node() :
				previous(nullptr),
				next(nullptr),
				live(0)
			{}

			~Node(){
				Clear();
			}

			void Clear(){
				if(live) Get().~data_t();
				previous = nullptr;
				next = nullptr;
				live = 0;
			}

			ref_t Get(){
				return *reinterpret_cast<pointer_t>(mData);
			}

			const_ref_t Get() const{
				return *reinterpret_cast<pointer_t>(mData);
			}

			Node& operator=(move_t aData){
				pointer_t ptr = reinterpret_cast<pointer_t>(mData);
				if(live){
					*ptr = std::move(aData);
				}else{
					new(ptr) data_t(std::move(aData));
					live = 1;
				}
				return *this;
			}

			Node& operator=(const_ref_t aData){
				pointer_t ptr = reinterpret_cast<pointer_t>(mData);
				if(live){
					*ptr = aData;
				}else{
					new(ptr) data_t(aData);
					live = 1;
				}
				return *this;
			}
		};
		Node* mBegin;
		Node mNodes[SIZE];
		Node mEnd;
	public:
		class const_iterator_t;

		class iterator_t{
		private:
			FixedList<data_t, SIZE>* mParent;
			Node* mNode;
		public:
			friend FixedList<data_t, SIZE>;
			friend const_iterator_t;

			iterator_t() :
				mParent(nullptr),
				mNode(nullptr)
			{}

			iterator_t(FixedList<data_t, SIZE>& aParent, Node& aNode) :
				mParent(&aParent),
				mNode(&aNode)
			{}

			size_t operator-(const iterator_t aOther) const{
				
			}

			iterator_t& operator++(){
				mNode = mNode->next;
				return *this;
			}

			iterator_t operator++(int){
				iterator_t tmp(*this);
				++*this;
				return tmp;
			}

			iterator_t& operator--(){
				mNode = mNode->previous;
				return *this;
			}

			iterator_t operator--(int){
				iterator_t tmp(*this);
				--*this;
				return tmp;
			}

			iterator_t& operator+=(size_t aNumber){
				while(aNumber > 0){
					++*this;
					--aNumber;
				}
				return *this;
			}

			iterator_t operator+(const size_t aNumber) const{
				iterator_t tmp(*this);
				tmp += aNumber;
				return tmp;
			}

			iterator_t& operator-=(const size_t aNumber){
				while(aNumber > 0){
					--*this;
					--aNumber;
				}
				return *this;
			}

			iterator_t operator-(const size_t aNumber) const{
				iterator_t tmp(*this);
				tmp -= aNumber;
				return tmp;
			}

			ref_t operator*(){
				return mNode->Get();
			}

			const_ref_t operator*() const{
				return mNode->Get();
			}

			ref_t operator->(){
				return mNode->Get();
			}

			const_ref_t operator->() const{
				return mNode->Get();
			}

			bool operator==(const iterator_t aOther) const{
				return mNode == aOther.mNode;
			}

			bool operator!=(const iterator_t aOther) const{
				return mNode !- aOther.mNode;
			}

			bool operator<(const iterator_t aOther) const{
				if(mParent != aOther.mParent || aOther.mNode->previous == nullptr) return false;
				const Node* n = aOther.mNode->previous;
				while(n != nullptr){
					if(n == mNode) return true;
					n = n->previous;
				}
				return false;
			}

			bool operator>(const iterator_t aOther) const{
				const Node* const end = &mParent->mEnd;
				if (mParent != aOther.mParent || aOther.mNode->next == end) return false;
				const Node* n = aOther.mNode->next;
				while(n != end){
					if (n == mNode) return true;
					n = n->next;
				}
				return false;
			}

			bool operator<=(const iterator_t aOther) const{
				return operator==(aOther) || operator<(aOther);
			}

			bool operator>=(const iterator_t aOther) const{
				return operator==(aOther) || operator>(aOther);
			}
		};

		class const_iterator_t{
		private:
			const FixedList<data_t, SIZE>* mParent;
			const Node* mNode;

			inline iterator_t& Cast(){
				return *reinterpret_cast<iterator_t*>(this);
			}

			inline const iterator_t& Cast() const{
				return *reinterpret_cast<const iterator_t*>(this);
			}
		public:
			const_iterator_t() :
				mParent(nullptr),
				mNode(nullptr)
			{}

			const_iterator_t(FixedList<data_t, SIZE>& aParent, Node& aNode) :
				mParent(&aParent),
				mNode(&aNode)
			{}

			const_iterator_t(const iterator_t aOther) :
				mParent(aOther.mParent),
				mNode(aOther.mNode)
			{}

			size_t operator-(const const_iterator_t aOther) const{
				return Cast() - aOther.Cast();
			}

			const_iterator_t& operator++(){
				return reinterpret_cast<const_iterator_t&>(++Cast());
			}

			const_iterator_t operator++(int){
				return const_iterator_t(Cast()++);
			}

			const_iterator_t& operator--(){
				return reinterpret_cast<const_iterator_t&>(--Cast());
			}

			const_iterator_t operator--(int){
				return const_iterator_t(Cast()++);
			}

			const_iterator_t& operator+=(size_t aNumber){
				return reinterpret_cast<const_iterator_t&>(Cast() += aNumber);
			}

			const_iterator_t operator+(const size_t aNumber) const{
				return const_iterator_t(Cast() + aNumber);
			}

			const_iterator_t& operator-=(const size_t aNumber){
				return reinterpret_cast<const_iterator_t&>(Cast() -= aNumber);
			}

			const_iterator_t operator-(const size_t aNumber) const{
				return const_iterator_t(Cast() - aNumber);
			}

			const_ref_t operator*() const{
				return *Cast();
			}

			ref_t operator->(){
				return *Cast();
			}

			const_ref_t operator->() const{
				return *Cast();
			}

			bool operator==(const const_iterator_t aOther) const{
				return Cast() == aOther.Cast();
			}

			bool operator!=(const const_iterator_t aOther) const{
				return Cast() != aOther.Cast();
			}

			bool operator<(const const_iterator_t aOther) const{
				return Cast() < aOther.Cast();
			}

			bool operator>(const const_iterator_t aOther) const{
				return Cast() > aOther.Cast();
			}

			bool operator<=(const const_iterator_t aOther) const{
				return Cast() <= aOther.Cast();
			}

			bool operator>=(const const_iterator_t aOther) const{
				return Cast() >= aOther.Cast();
			}
		};

		FixedList() :
			mBegin(nullptr)
		{}

		FixedList(const_ref_t aValue, const size_t aCopies) :
			mBegin(nullptr)
		{
			for(size_t i = 0; i < aCopies; ++i){
				InsertBack(aValue);
			}
		}

		~FixedList(){

		}

		void Clear(){
			for(Node& i : mNodes) i.Clear();
			mBegin = nullptr;
			mEnd = nullptr;
		}

		bool IsEmpty() const{
			return mBegin != nullptr;
		}

		size_t Size() const{
			size_t count = 0;
			const Node* n = mBegin;
			while(n != nullptr){
				n = n->next;
				++count;
			}
			return count;
		}

		size_t Capacity() const{
			return SIZE;
		}

		ref_t operator[](const size_t aIndex){
			size_t count = 0;
			const Node* n = mBegin;
			const Node* end = &mEnd;
			while (n != end){
				if(count == aIndex) return n->Get();
				n = n->next;
				++count;
			}
			throw std::runtime_error("Index out of bounds");
		}

		const_ref_t operator[](const size_t aIndex) const{
			size_t count = 0;
			const Node* n = mBegin;
			const Node* end = &mEnd;
			while (n != end){
				if(count == aIndex) return n->Get();
				n = n->next;
				++count;
			}
			throw std::runtime_error("Index out of bounds");
		}

		ref_t InsertBefore(const_iterator_t aPos, const_ref_t aItem){
			Node& target = *aPos.mNode;

			// Find empty node
			Node* n = nullptr;
			for (Node& i : mNodes) if (! i.live){
				n = &i;
				break;
			}

			if(n == nullptr) throw std::runtime_error("FixedList is full");

			// Link nodes

			if(&target == mBegin) mBegin = n;

			n->next = &target;
			n->previous->target.previous;

			target.previous->next = n;
			target.previous = n

			n->next = &target;
			n->previous = target.previous;

			// Assign node
			*n = aItem;

			// Return reference
			return n->Get();
		}

		ref_t InsertAfter(const_iterator_t aPos, const_ref_t aItem) {
			return InsertBefore(aPos - 1, aItem);
		}

		ref_t AddBack(const_ref_t aItem){
			return InsertBefore(begin() + (size() - 1), aItem);
		}

		ref_t AddFront(const_ref_t aItem){
			return InsertBefore(begin(), aItem);
		}

		void Remove(const_iterator_t aPos){
			if (n >= end()) throw std::runtime_error("Cannot remove FixedList end");

			Node& target = *aPos.mNode;

			// Link nodes

			if (&target == mBegin) mBegin = target.next;

			target.next->previous = target.previous;
			target.previous->next = target.next; //! \TODO Fix nullptr exception when target is mBegin

			// Delete value
			target.Clear();
		}

		void RemoveBack(){
			Remove(begin() + (size() - 1));
		}

		void RemoveFront(){
			Remove(begin());
		}

		ref_t Front(){
			if(mBegin == nullptr) throw std::runtime_error("FixedList is empty");
			return mBegin->Get();
		}

		const_ref_t Front() const{
			if(mBegin == nullptr) throw std::runtime_error("FixedList is empty");
			return mBegin->Get();
		}

		ref_t Back(){
			if(mEnd == nullptr) throw std::runtime_error("FixedList is empty");
			return mEnd->Get();
		}

		const_ref_t Back() const{
			if(mEnd == nullptr) throw std::runtime_error("FixedList is empty");
			return mEnd->Get();
		}

		iterator_t begin(){
			return iterator_t(*this, *mBegin);
		}

		const_iterator_t begin() const{
			return const_iterator_t(*this, *mBegin);
		}

		iterator_t end(){
			return iterator_t(*this, mEnd);
		}

		const_iterator_t end() const{
			return const_iterator_t(*this, mEnd);
		}
	};
}}

#endif
