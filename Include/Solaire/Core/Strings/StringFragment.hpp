#ifndef SOLAIRE_CORE_STRING_FRAGMENT_HPP
#define SOLAIRE_CORE_STRING_FRAGMENT_HPP

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
\file StringFragment.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 17th September 2015
Last Modified	: 28th September 2015
*/

#include <iostream>
#include "..\Iterators\ReverseIterator.hpp"
#include "..\Iterators\ConstIterator.hpp"

namespace Solaire {
    class ConstStringFragment;

    struct StringComparison{
        enum : uint16_t{
            FULL_LENGTH
        };

        bool ignoreCase;
        uint16_t length;

        constexpr StringComparison(bool aIgnoreCase = true, uint16_t aLength = FULL_LENGTH);

        int8_t operator()(const ConstStringFragment aFirst, const ConstStringFragment aSecond) const;
    };

	class StringFragment{
    public:
        typedef char Type;
        typedef const char ConstType;
        typedef Type& Reference;
        typedef ConstType& ConstReference;
        typedef Type* Pointer;
        typedef ConstType* ConstPointer;
        typedef Pointer Iterator;
        typedef ConstPointer ConstIterator;
        typedef ReverseIteratorWrapper<Type, Iterator> ReverseIterator;
        typedef ConstIteratorWrapper<Type, ReverseIterator> ConstReverseIterator;
	private:
		Pointer mBegin;
		Pointer mEnd;

		enum : char{
		    CASE_DIFFERENCE = 'A' - 'a'
		};
	public:
	    friend ConstStringFragment;

		constexpr StringFragment(const Pointer aBegin, const Pointer aEnd);
		constexpr StringFragment(const Pointer aBegin, const size_t aLength);
		StringFragment(const Pointer aBegin);

		Iterator begin();
		constexpr ConstIterator begin() const;
		Iterator end();
		constexpr ConstIterator end() const;
        ReverseIterator rbegin();
		ConstReverseIterator rbegin() const;
		ReverseIterator rend();
		ConstReverseIterator rend() const;

		constexpr Type operator[](const size_t aIndex) const;
		Reference operator[](const size_t aIndex);
		constexpr size_t Size() const;

		bool operator==(const StringFragment aOther) const;
		bool operator!=(const StringFragment aOther) const;
		bool operator<(const StringFragment aOther) const;
		bool operator>(const StringFragment aOther) const;
		bool operator<=(const StringFragment aOther) const;
		bool operator>=(const StringFragment aOther) const;

		Iterator FindFirst(const Type aChar);
		Iterator FindNext(const Iterator aPos, const Type aChar);
		Iterator FindLast(const Type aChar);
		StringFragment FindFirst(const ConstStringFragment aFragment);
		StringFragment FindNext(const Iterator aPos, const ConstStringFragment aFragment);
		StringFragment FindLast(const ConstStringFragment aFragment);

		ConstIterator FindFirst(const Type aChar) const;
		ConstIterator FindNext(const ConstIterator aPos, const Type aChar) const;
		ConstIterator FindLast(const Type aChar) const;
		ConstStringFragment FindFirst(const ConstStringFragment aFragment) const;
		ConstStringFragment FindNext(const ConstIterator aPos, const ConstStringFragment aFragment) const;
		ConstStringFragment FindLast(const ConstStringFragment aFragment) const;

		StringFragment& ReplaceFirst(const Type aTarget, const Type aReplacement);
		StringFragment& ReplaceNext(const ConstIterator aPos, const Type aTarget, const Type aReplacement);
		StringFragment& ReplaceLast(const Type aTarget, const Type aReplacement);
		StringFragment& ReplaceAll(const Type aTarget, const Type aReplacement);

		StringFragment& ReplaceFirst(const ConstStringFragment aTarget, const ConstStringFragment aReplacement);
		StringFragment& ReplaceNext(const ConstIterator aPos, const ConstStringFragment aTarget, const ConstStringFragment aReplacement);
		StringFragment& ReplaceLast(const ConstStringFragment aTarget, const ConstStringFragment aReplacement);
		StringFragment& ReplaceAll(const ConstStringFragment aTarget, const ConstStringFragment aReplacement);

		static void ToLowerCase(const Iterator aPos);
		static void ToUpperCase(const Iterator aPos);
		static void ToggleCase(const Iterator aPos);

		StringFragment& ToLowerCase();
		StringFragment& ToUpperCase();
		StringFragment& ToggleCase();

        friend std::ostream& operator<<(std::ostream& aStream, const StringFragment& aFragment){
            aStream.write(aFragment.mBegin, aFragment.Size());
            return aStream;
        }

        friend std::istream& operator>>(std::istream& aStream, StringFragment& aFragment){
            aStream.read(aFragment.mBegin, aFragment.Size());
            return aStream;
        }
	};

	class ConstStringFragment{
    public:
        friend StringFragment;

        typedef char Type;
        typedef const char ConstType;
        typedef ConstType& ConstReference;
        typedef ConstType* ConstPointer;
        typedef ConstPointer ConstIterator;
        typedef ConstIteratorWrapper<Type, StringFragment::ReverseIterator> ConstReverseIterator;
	private:
		StringFragment mFragment;
	public:
		constexpr ConstStringFragment(const ConstPointer aBegin, const ConstPointer aEnd);
		constexpr ConstStringFragment(const ConstPointer aBegin, const size_t aLength);
		ConstStringFragment(const ConstPointer aBegin);
		constexpr ConstStringFragment(const StringFragment aOther);

		constexpr ConstIterator begin() const;
		constexpr ConstIterator end() const;
		ConstReverseIterator rbegin() const;
		ConstReverseIterator rend() const;

		constexpr Type operator[](const size_t aIndex) const;
		constexpr size_t Size() const;

		bool operator==(const ConstStringFragment aOther) const;
		bool operator!=(const ConstStringFragment aOther) const;
		bool operator<(const ConstStringFragment aOther) const;
		bool operator>(const ConstStringFragment aOther) const;
		bool operator<=(const ConstStringFragment aOther) const;
		bool operator>=(const ConstStringFragment aOther) const;

		ConstIterator FindFirst(const Type aChar) const;
		ConstIterator FindNext(const ConstIterator aPos, const Type aChar) const;
		ConstIterator FindLast(const Type aChar) const;
		ConstStringFragment FindFirst(const ConstStringFragment aFragment) const;
		ConstStringFragment FindNext(const ConstIterator aPos, const ConstStringFragment aFragment) const;
		ConstStringFragment FindLast(const ConstStringFragment aFragment) const;

        friend std::ostream& operator<<(std::ostream& aStream, const ConstStringFragment& aFragment){
            return aStream << aFragment.mFragment;
        }

        friend std::istream& operator>>(std::istream& aStream, ConstStringFragment& aFragment){
            return aStream >> aFragment.mFragment;
        }
	};
}

#include "StringFragment.inl"

#endif
