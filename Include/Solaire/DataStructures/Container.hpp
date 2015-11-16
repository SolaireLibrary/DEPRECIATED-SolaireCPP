#ifndef SOLAIRE_CONTAINER_HPP
#define SOLAIRE_CONTAINER_HPP

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
	\file Container.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 16th September 2015
	Last Modified	: 16th September 2015
*/

#include <cstdint>
#include <functional>
#include "..\Core\Init.hpp"

namespace Solaire{

	template<class T>
	class SOLAIRE_EXPORT_API Iterator {
	public:
		typedef T Type;
		typedef typename TypeTraits<T>::ConstPassType PassType;
	public:
		virtual bool SOLAIRE_EXPORT_CALL HasPrevious() const = 0;
		virtual bool SOLAIRE_EXPORT_CALL HasNext() const = 0;
		virtual uint32_t SOLAIRE_EXPORT_CALL GetIndex() const = 0;
		virtual PassType SOLAIRE_EXPORT_CALL Peak() const = 0;
		virtual Type& SOLAIRE_EXPORT_CALL Next() = 0;
		virtual Type& SOLAIRE_EXPORT_CALL Previous() = 0;

		inline bool SOLAIRE_EXPORT_CALL operator==(const Iterator& aOther) const {
			return GetIndex() == aOther.GetIndex();
		}

		inline bool SOLAIRE_EXPORT_CALL operator!=(const Iterator& aOther) const {
			return GetIndex() != aOther.GetIndex();
		}

		inline bool SOLAIRE_EXPORT_CALL operator<(const Iterator& aOther) const {
			return GetIndex() < aOther.GetIndex();
		}

		inline bool SOLAIRE_EXPORT_CALL operator>(const Iterator& aOther) const {
			return GetIndex() > aOther.GetIndex();
		}

		inline bool SOLAIRE_EXPORT_CALL operator<=(const Iterator& aOther) const {
			return GetIndex() <= aOther.GetIndex();
		}

		inline bool SOLAIRE_EXPORT_CALL operator>=(const Iterator& aOther) const {
			return GetIndex() >= aOther.GetIndex();
		}
	};

	template<class T>
	class SOLAIRE_EXPORT_API Container {
	public:
		typedef T Type;
		typedef typename TypeTraits<T>::ConstPassType PassType;

		typedef Iterator<Type> Iterator;
		typedef Iterator<const Type> ConstIterator;
		typedef Iterator<Type> ReverseIterator;
		typedef Iterator<const Type> ConstReverseIterator;

		typedef std::function<bool(PassType)> Condition;
	public:
		virtual uint32_t SOLAIRE_EXPORT_CALL Size() const throw() = 0;
		virtual T& SOLAIRE_EXPORT_CALL operator[](const uint32_t) = 0;

		virtual T& SOLAIRE_EXPORT_CALL Front() throw() = 0;
		virtual T& SOLAIRE_EXPORT_CALL Back() throw() = 0;

		virtual Iterator& SOLAIRE_EXPORT_CALL Begin() throw() = 0;
		virtual Iterator& SOLAIRE_EXPORT_CALL End() const throw() = 0;

		virtual ReverseIterator& SOLAIRE_EXPORT_CALL ReverseBegin() throw() = 0;
		virtual ReverseIterator& SOLAIRE_EXPORT_CALL ReverseEnd() throw() = 0;

		virtual Iterator& SOLAIRE_EXPORT_CALL FindFirstOf(PassType) throw() = 0;
		virtual Iterator& SOLAIRE_EXPORT_CALL FindNextOf(const Iterator&, const PassType) throw() = 0;
		virtual Iterator& SOLAIRE_EXPORT_CALL FindLastOf(PassType) throw() = 0;

		virtual Iterator& SOLAIRE_EXPORT_CALL FindFirst(const Condition&) throw() = 0;
		virtual Iterator& SOLAIRE_EXPORT_CALL FindNext(const Iterator&, const Condition&) throw() = 0;
		virtual Iterator& SOLAIRE_EXPORT_CALL FindLast(const Condition&) throw() = 0;
	}; 
	
	template<class T>
	class SOLAIRE_EXPORT_API StackContainer : public Container {
	public:
		virtual bool SOLAIRE_EXPORT_CALL PushBack(PassType) throw() = 0;
		virtual T SOLAIRE_EXPORT_CALL PopBack() throw() = 0;
	};

	template<class T>
	class SOLAIRE_EXPORT_API DequeContainer : public StackContainer {
	public:
		virtual bool SOLAIRE_EXPORT_CALL PushFront(PassType) throw() = 0;
		virtual T SOLAIRE_EXPORT_CALL PopFront() throw() = 0;
	};

	template<class T>
	class SOLAIRE_EXPORT_API ListContainer : public DequeContainer {
	public:
		virtual bool SOLAIRE_EXPORT_CALL InsertBefore(const Iterator&, PassType) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL InsertAfter(const Iterator&, PassType) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Erase(const Iterator&) throw() = 0;
	};
}

#endif
