#ifndef SOLAIRE_ITERATOR_MACROS_HPP
#define SOLAIRE_ITERATOR_MACROS_HPP

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
	\file IteratorMacros.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 8th October 2015
	Last Modified	: 8th October 2015
*/

namespace Solaire{

    #define SOLAIRE_BASIC_ITERATOR(aName, aType, aData, aPlus, aMinus, aGet)\
	class aName{\
	private:\
		aData mData;\
	public:\
		aName(const aData aOther) :\
			mData(aOther)\
		{}\
        \
		aName& operator++(){\
			aPlus\
			return *this;\
		}\
        \
		inline aName operator++(int){\
		    const aName tmp(mData);\
		    operator++();\
			return tmp;\
		}\
        \
		aName& operator--(){\
			aMinus\
			return *this;\
		}\
        \
		inline aName operator--(int){\
			const aName tmp(mData);\
		    operator--();\
			return tmp;\
		}\
        \
		inline aName& operator+=(const size_t aNumber){\
			for(size_t i = 0; i < aNumber; ++i){\
                operator++();\
			}\
			return *this;\
		}\
        \
		aName operator+(const size_t aNumber) const{\
			aName tmp(mData);\
		    tmp += aNumber;\
			return tmp;\
		}\
        \
		inline aName& operator-=(const size_t aNumber){\
			for(size_t i = 0; i < aNumber; ++i){\
                operator--();\
			}\
			return *this;\
		}\
        \
		inline aName operator-(const size_t aNumber) const{\
			aName tmp(mData);\
		    tmp -= aNumber;\
			return tmp;\
		}\
        \
		aType& operator*() const{\
			aGet\
		}\
        \
		inline aType* operator->() const{\
			return &operator*();\
		}\
        \
		inline bool operator==(const aName aOther) const{\
			return mData == aOther.mData;\
		}\
        \
		inline bool operator!=(const aName aOther) const{\
			return mData != aOther.mData;\
		}\
        \
		inline bool operator<(const aName aOther) const{\
			return mData < aOther.mData;\
		}\
        \
		inline bool operator>(const aName aOther) const{\
			return mData > aOther.mData;\
		}\
        \
		inline bool operator<=(const aName aOther) const{\
			return mData <= aOther.mData;\
		}\
        \
		inline bool operator>=(const aName aOther) const{\
			return mData >= aOther.mData;\
		}\
	};

    #define SOLAIRE_CONST_BASIC_ITERATOR(aName, aType, aData, aPlus, aMinus, aGet)\
	class aName{\
	private:\
		aData mData;\
	public:\
		aName(const aData aOther) :\
			mData(aOther)\
		{}\
        \
		aName& operator++(){\
			aPlus\
			return *this;\
		}\
        \
		inline aName operator++(int){\
		    const aName tmp(mData);\
		    operator++();\
			return tmp;\
		}\
        \
		aName& operator--(){\
			aMinus\
			return *this;\
		}\
        \
		inline aName operator--(int){\
			const aName tmp(mData);\
		    operator--();\
			return tmp;\
		}\
        \
		inline aName& operator+=(const size_t aNumber){\
			for(size_t i = 0; i < aNumber; ++i){\
                operator++();\
			}\
			return *this;\
		}\
        \
		aName operator+(const size_t aNumber) const{\
			aName tmp(mData);\
		    tmp += aNumber;\
			return tmp;\
		}\
        \
		inline aName& operator-=(const size_t aNumber){\
			for(size_t i = 0; i < aNumber; ++i){\
                operator--();\
			}\
			return *this;\
		}\
        \
		inline aName operator-(const size_t aNumber) const{\
			aName tmp(mData);\
		    tmp -= aNumber;\
			return tmp;\
		}\
        \
		const aType& operator*() const{\
			aGet\
		}\
        \
		inline const aType* operator->() const{\
			return &operator*();\
		}\
        \
		inline bool operator==(const aName aOther) const{\
			return mData == aOther.mData;\
		}\
        \
		inline bool operator!=(const aName aOther) const{\
			return mData != aOther.mData;\
		}\
        \
		inline bool operator<(const aName aOther) const{\
			return mData < aOther.mData;\
		}\
        \
		inline bool operator>(const aName aOther) const{\
			return mData > aOther.mData;\
		}\
        \
		inline bool operator<=(const aName aOther) const{\
			return mData <= aOther.mData;\
		}\
        \
		inline bool operator>=(const aName aOther) const{\
			return mData >= aOther.mData;\
		}\
	};
}


#endif
