#ifndef SOLAIRE_UTILITY_FIXED_STACK_HPP
#define SOLAIRE_UTILITY_FIXED_STACK_HPP

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
	\file FixedStack.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 19th September 2015
	Last Modified	: 19th September 2015
*/

#include <cstdint>
#include <stdexcept>

namespace Solaire{ namespace Utility{

    template<class TYPE, const size_t SIZE>
	class FixedStack{
	public:
		typedef TYPE data_t;
		typedef data_t& ref_t;
		typedef const data_t& const_ref_t;
		typedef data_t&& move_t;
		typedef data_t* pointer_t;
		typedef const data_t* const_pointer_t;
		typedef pointer_t iterator_t;
		typedef const_pointer_t const_iterator_t;
	private:
		uint8_t mData[sizeof(data_t) * SIZE];
		uint16_t mHead;
	public:
		FixedStack() :
			mHead(0)
		{}

		FixedStack(const_ref_t aValue, const size_t aCopies) :
			mHead(0)
		{
			for(size_t i = 0; i < aCopies; ++i){
				Push(aValue);
			}
		}

		FixedStack(const std::initializer_list<data_t> aList) :
			mHead(0)
		{
			for(data_t i : aList) Push(i);
		}

		template<class external_iterator_t>
		FixedStack(external_iterator_t aBegin, const external_iterator_t aEnd) :
			mHead(0)
		{
			while(aBegin != aEnd) {
				Push(*aBegin);
				++aBegin;
			}
		}

		~FixedStack(){

		}

		void Clear(){
			const iterator_t begin = this->begin();
			const iterator_t end = this->end();
			iterator_t i = begin;
			while(i != end){
				i->~data_t();
				++i;
			}
			mHead = 0;
		}

		bool IsEmpty() const{
			return mHead == 0;
		}

		size_t Size() const{
			return mHead;
		}

		size_t Capacity() const{
			return SIZE;
		}

		ref_t operator[](const size_t aIndex){
			if(aIndex < mHead) throw std::runtime_error("Index out of bounds");
			return reinterpret_cast<pointer_t>(mData)[aIndex];
		}

		const_ref_t operator[](const size_t aIndex) const{
			if(aIndex < mHead) throw std::runtime_error("Index out of bounds");
			return reinterpret_cast<const_pointer_t>(mData)[aIndex];
		}

		ref_t Push(move_t aItem){
			if(mHead == SIZE) throw std::runtime_error("FixedStack is full");

			const pointer_t ptr = reinterpret_cast<pointer_t>(mData) + mHead++;
			new(ptr) data_t(std::move(aItem));
			return *ptr;
		}

		ref_t Push(const_ref_t aItem) {
			if(mHead == SIZE) throw std::runtime_error("FixedStack is full");

			const pointer_t ptr = reinterpret_cast<pointer_t>(mData) + mHead++;
			new(ptr) data_t(aItem);
			return *ptr;
		}

		data_t Pop(){
			if(mHead > 0) throw std::runtime_error("FixedStack is empty");

			const pointer_t ptr = &Back();
			--mHead;

			data_t tmp(*ptr);
			ptr->~data_t();

			return tmp;
		}

		ref_t Front(){
			return reinterpret_cast<pointer_t>(mData)[0];
		}

		const_ref_t Front() const{
			return reinterpret_cast<const_pointer_t>(mData)[0];
		}

		ref_t Back(){
			return reinterpret_cast<pointer_t>(mData)[mHead - 1];
		}

		const_ref_t Back() const{
			return reinterpret_cast<const_pointer_t>(mData)[mHead - 1];
		}

		iterator_t begin(){
			return reinterpret_cast<pointer_t>(mData);
		}

		const_iterator_t begin() const{
			return reinterpret_cast<const_pointer_t>(mData);
		}

		iterator_t end(){
			return reinterpret_cast<pointer_t>(mData) + mHead;
		}

		const_iterator_t end() const{
			return reinterpret_cast<const_pointer_t>(mData) + mHead;
		}
	};
}}

#endif
