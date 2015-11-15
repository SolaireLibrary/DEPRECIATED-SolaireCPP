#ifndef SOLAIRE_COLLECTION_HPP
#define SOLAIRE_COLLECTION_HPP

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
\file Collection.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 11th November 2015
Last Modified	: 11th November 2015
*/

#include <cstdint>
#include <functional>
#include "Link.inl"

namespace Solaire{namespace Dll{

	template<class T>
	class SOLAIRE_EXPORT_API Iterator{
	public:
		virtual bool HasPrevious() const = 0;
		virtual bool HasNext() const = 0;
		virtual uint32_t GetIndex() const = 0;
		virtual T& Peak() const = 0;
		virtual T& Next() = 0;
		virtual T& Previous() = 0;
	};

	template<class T>
	class SOLAIRE_EXPORT_API Collection{
	public:
		typedef std::function<bool(const T&)> Condition;
	public:
		virtual uint32_t Size() const = 0;
		virtual T& Get(const uint32_t) = 0;
		virtual const T& Get(const uint32_t) const = 0;
		
		virtual Iterator<T>& GetIterator() = 0;
		virtual Iterator<const T>& GetIterator() const = 0;
		
		virtual Iterator<const T>& FindFirst(const T&) const = 0;
		virtual Iterator<const T>& FindNext(const Iterator<const T>&, const T&) const = 0;
		virtual Iterator<const T>& FindLast(const T&) const = 0;
		
		virtual Iterator<const T>& FindFirst(const Condition&) const = 0;
		virtual Iterator<const T>& FindNext(const Iterator<const T>&, const Condition&) const = 0;
		virtual Iterator<const T>& FindLast(const Condition&) const = 0;
	};
	

	template<class T>
	class SOLAIRE_EXPORT_API DynamicCollection : public Collection<T>{
	public:
		virtual T& Add(const T&) = 0;
		virtual void Remove(const Iterator<const T>&) = 0;
		virtual void Clear() = 0;
	};
}}

#endif
