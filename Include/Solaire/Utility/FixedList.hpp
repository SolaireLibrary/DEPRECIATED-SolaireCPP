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

    template<class TYPE, const size_t SIZE, class INDEX = uint16_t>
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
			INDEX mPrevious;
			INDEX mNext;

			Node(Node&& aOther) = delete;
			Node(const Node& aOther) = delete;
			Node& operator=(Node&& aOther) = delete;
			Node& operator=(const Node& aOther) = delete;
		public:
			typedef FixedList<data_t, SIZE, INDEX> list_t;

			Node() :
				mPrevious(NODE_NULL),
				mNext(NODE_NULL)
			{}

			~Node(){
				Clear();
			}

			void LinkForward(const list_t& aList, Node& aNext){
				Node* const next = &aNext;
				if(next == &aList.mEnd){
					mNext = NODE_END;
				}else if(next == &aList.mBegin) {
					throw std::runtime_error("Node next pointer cannot be node begin");
				}else{
					mNext = next - aList.mNodes;
				}
				next->mPrevious = this - aList.mNodes;
			}

			void LinkBackward(const list_t& aList, Node& aPrevious){
				Node* const previous = &aPrevious;
				if(previous == &aList.mEnd){
					throw std::runtime_error("Node next pointer cannot be node begin");
				}else if(previous == &aList.mBegin) {
					mPrevious = NODE_BEGIN;
				}else{
					mPrevious = previous - aList.mNodes;
				}
				previous->mNext = this - aList.mNodes;
			}

			bool HasNext(){
				return mNext != NODE_NULL;
			}

			bool HasPrevious() {
				return mPrevious != NODE_NULL;
			}

			Node& GetNext(list_t& aList){
				switch(mNext)
				{
				case NODE_NULL:
					throw std::runtime_error("Node does not have next pointer");
				case NODE_END:
					return aList.mEnd;
				case NODE_BEGIN:
					throw std::runtime_error("Node next pointer cannot point to list begin");
				default:
					return aList.mNodes[mNext];
				}
			}

			const Node& GetNext(const list_t& aList) const{
				switch (mNext)
				{
				case NODE_NULL:
					throw std::runtime_error("Node does not have next pointer");
				case NODE_END:
					return aList.mEnd;
				case NODE_BEGIN:
					throw std::runtime_error("Node next pointer cannot point to list begin");
				default:
					return aList.mNodes[mNext];
				}
			}

			Node& GetPrevious(list_t& aList){
				switch(mPrevious)
				{
				case NODE_NULL:
					throw std::runtime_error("Node does not have previous pointer");
				case NODE_END:
					throw std::runtime_error("Node previous pointer cannot point to list end");
				case NODE_BEGIN:
					return aList.mBegin;
				default:
					return aList.mNodes[mPrevious];
				}
			}

			const Node& GetPrevious(const list_t& aList) const{
				switch(mPrevious)
				{
				case NODE_NULL:
					throw std::runtime_error("Node does not have previous pointer");
				case NODE_END:
					throw std::runtime_error("Node previous pointer cannot point to list end");
				case NODE_BEGIN:
					return aList.mBegin;
				default:
					return aList.mNodes[mPrevious];
				}
			}

			bool IsLive() const{
				return mNext != NODE_NULL;
			}

			void Clear(){
				if(IsLive()) Get().~data_t();
				mPrevious = NODE_NULL;
				mNext = NODE_NULL;
			}

			pointer_t GetMemory(){
				return reinterpret_cast<pointer_t>(mData);
			}

			const_pointer_t GetMemory() const{
				return reinterpret_cast<const_pointer_t>(mData);
			}

			ref_t Get(){
				if(! IsLive()) throw std::runtime_error("FixedList::Node can only be dereferenced while live");
				return *GetMemory();
			}

			const_ref_t Get() const{
				if(! IsLive()) throw std::runtime_error("FixedList::Node can only be dereferenced while live");
				return *GetMemory();
			}

			ref_t Set(const_ref_t aData){
				pointer_t ptr = reinterpret_cast<pointer_t>(mData);
				if(IsLive()){
					*ptr = aData;
				}else{
					new(ptr) data_t(aData);
				}
				return *ptr;
			}
		};

		Node mNodes[SIZE];
		Node mBegin;
		Node mEnd;

		enum : INDEX{
			NODE_NULL = std::numeric_limits<INDEX>::max(),
			NODE_END = NODE_NULL - 1,
			NODE_BEGIN = NODE_END - 1
		};

		static_assert(SIZE < NODE_BEGIN, "FixedList::SIZE is too large");
		static_assert((sizeof(data_t) * 2) >= sizeof(INDEX), "Cannot store INDEX type in begin/end nodes");
		
		INDEX ReadSize() const{
			if(sizeof(data_t) >= sizeof(INDEX)){
				return *reinterpret_cast<const INDEX*>(mBegin.GetMemory());
			}else{
				const uint8_t* src = reinterpret_cast<const uint8_t*>(mBegin.GetMemory());
				uint8_t data[sizeof(INDEX)];

				for(size_t i = 0; i < sizeof(INDEX); ++i){
					if(i == (sizeof(data_t) + 1)){
						src = reinterpret_cast<const uint8_t*>(mEnd.GetMemory());
					}
					data[i] = *src;
					++src;
				}
				return *reinterpret_cast<const uint16_t*>(data);
			}
		}

		void WriteSize(const INDEX aSize){
			if(sizeof(data_t) >= sizeof(INDEX)){
				*reinterpret_cast<INDEX*>(mBegin.GetMemory()) = aSize;
			}else{
				const uint8_t* const data = reinterpret_cast<const uint8_t*>(&aSize);
				uint8_t* dst = reinterpret_cast<uint8_t*>(mBegin.GetMemory());

				for (size_t i = 0; i < sizeof(INDEX); ++i) {
					if(i == (sizeof(data_t) + 1)){
						dst = reinterpret_cast<uint8_t*>(mEnd.GetMemory());
					}
					*dst = data[i];
					++dst;
				}
			}
		}

	public:
		class const_iterator_t;

		class iterator_t{
		private:
			FixedList<data_t, SIZE, INDEX>* mParent;
			Node* mNode;
		public:
			friend FixedList<data_t, SIZE, INDEX>;
			friend const_iterator_t;

			iterator_t() :
				mParent(nullptr),
				mNode(nullptr)
			{}

			iterator_t(FixedList<data_t, SIZE, INDEX>& aParent, Node& aNode) :
				mParent(&aParent),
				mNode(&aNode)
			{}

			size_t operator-(const iterator_t aOther) const{
				if(mParent != aOther.mParent) throw std::runtime_error("Cannot compare iterators from seperate lists");
				if(mNode == aOther.mNode) return 0;
				size_t count = 0;
				const Node* n = mNode;
				while(n->HasPrevious()){
					if(n == aOther.mNode) return count;
					++count;
					n = &n->GetPrevious(*this);
				}
				throw std::runtime_error("Second iterator is not less than first iterator");
			}

			iterator_t& operator++(){
				mNode = &mNode->GetNext(*mParent);
				return *this;
			}

			iterator_t operator++(int){
				iterator_t tmp(*this);
				++*this;
				return tmp;
			}

			iterator_t& operator--(){
				mNode = &mNode->GetPrevious(*mParent);
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

			iterator_t& operator-=(size_t aNumber){
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
				return mNode !=
					aOther.mNode;
			}

			bool operator<(const iterator_t aOther) const{
				if(mParent != aOther.mParent) return false;
				const Node* n = aOther.mNode->GetPrevious(*mParent);
				while(n->HasPrevious()){
					if(n == mNode) return true;
					n = &n->GetPrevious(&mParent);
				}
				return false;
			}

			bool operator>(const iterator_t aOther) const{
				if(mParent != aOther.mParent) return false;
				const Node* n = aOther.mNode->GetNext(*mParent);
				while(n->HasNext()){
					if(n == mNode) return true;
					n = &n->GetNext(&mParent);
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
			const FixedList<data_t, SIZE, INDEX>* mParent;
			const Node* mNode;

			inline iterator_t& Cast(){
				return *reinterpret_cast<iterator_t*>(this);
			}

			inline const iterator_t& Cast() const{
				return *reinterpret_cast<const iterator_t*>(this);
			}
		public:
			friend FixedList<data_t, SIZE, INDEX>;

			const_iterator_t() :
				mParent(nullptr),
				mNode(nullptr)
			{}

			const_iterator_t(FixedList<data_t, SIZE, INDEX>& aParent, Node& aNode) :
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

		FixedList(){
			WriteSize(0);
			mBegin.LinkForward(*this, mEnd);
		}

		FixedList(const_ref_t aValue, const size_t aCopies){
			WriteSize(0);
			mBegin.LinkFoward(*this, mEnd);

			for(size_t i = 0; i < aCopies; ++i){
				InsertBack(aValue);
			}
		}

		FixedList(const FixedList<data_t, SIZE, INDEX>& aOther){
			WriteSize(0);
			mBegin.LinkForward(*this, mEnd);
			operator=(aOther);
		}

		FixedList(FixedList<data_t, SIZE, INDEX>&& aOther) {
			WriteSize(0);
			mBegin.LinkForward(*this, mEnd);
			operator=(std::move(aOther));
		}

		~FixedList(){

		}

		FixedList<data_t, SIZE, INDEX>& operator=(const FixedList<data_t, SIZE, INDEX>& aOther) {
			Clear();
			for(const data_t& i : aOther) AddBack(i);
			return *this;
		}

		FixedList<data_t, SIZE, INDEX>& operator=(FixedList<data_t, SIZE, INDEX>&& aOther) {
			Clear();
			for(const data_t& i : aO.ther) AddBack(i);
			aOther.Clear();
			return *this;
		}

		void Clear(){
			for(Node& i : mNodes) i.Clear();
			mBegin.LinkFoward(*this, mEnd);
			WriteSize(0);
		}

		bool IsEmpty() const{
			return ReadSize() == 0;
		}

		size_t Size() const{
			return ReadSize();
		}

		size_t Capacity() const{
			return SIZE;
		}

		ref_t operator[](const size_t aIndex){
			size_t count = 0;
			Node* n = mBegin;
			const Node* end = &mEnd;
			while(n != end){
				if(count == aIndex + 1) return n->Get();
				n = &n->GetNext(*this);
				++count;
			}
			throw std::runtime_error("Index out of bounds");
		}

		const_ref_t operator[](const size_t aIndex) const{
			size_t count = 0;
			const Node* n = mBegin;
			const Node* end = &mEnd;
			while(n != end) {
				if(count == aIndex + 1) return n->Get();
				n = &n->GetNext(*this);
				++count;
			}
			throw std::runtime_error("Index out of bounds");
		}

		ref_t InsertBefore(const_iterator_t aPos, const_ref_t aItem){
			Node& target = const_cast<Node&>(*aPos.mNode);

			// Find empty node
			Node* n = nullptr;
			for(Node& i : mNodes) if(! i.IsLive()){
				n = &i;
				break;
			}

			if(n == nullptr) throw std::runtime_error("FixedList is full");

			// Link nodes
			n->LinkBackward(*this, target.GetPrevious(*this));
			n->LinkForward(*this, target);

			// Assign node
			n->Set(aItem);
			WriteSize(ReadSize() + 1);

			// Return reference
			return n->Get();
		}

		ref_t InsertAfter(const_iterator_t aPos, const_ref_t aItem) {
			return InsertBefore(aPos - 1, aItem);
		}

		ref_t AddBack(const_ref_t aItem){
			return InsertBefore(end(), aItem);
		}

		ref_t AddFront(const_ref_t aItem){
			return InsertBefore(begin(), aItem);
		}

		void Remove(const_iterator_t aPos){
			Node& target = const_cast<Node&>(*aPos.mNode);
			if (&target == &mEnd) throw std::runtime_error("Cannot remove FixedList end");

			// Link nodes
			target.GetNext(*this).LinkBackward(*this, target.GetPrevious(*this));
			WriteSize(ReadSize() - 1);

			// Delete value
			target.Clear();
		}

		void RemoveBack(){
			Remove(end() - 1);
		}

		void RemoveFront(){
			Remove(begin());
		}

		ref_t Front(){
			return *begin();
		}

		const_ref_t Front() const{
			return *begin();
		}

		ref_t Back(){
			return *(end() - 1);
		}

		const_ref_t Back() const{
			return *(end() - 1);
		}

		iterator_t begin(){
			return iterator_t(*this, mBegin.GetNext(*this));
		}

		const_iterator_t begin() const{
			return iterator_t(*this, mBegin.GetNext(*this));
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
