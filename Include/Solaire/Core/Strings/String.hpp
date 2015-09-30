#ifndef SOLAIRE_CORE_STRING_HPP
#define SOLAIRE_CORE_STRING_HPP

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
	\file String.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 27th September 2015
	Last Modified	: 27th September 2015
*/

#include <type_traits>
#include <sstream>
#include "../DataStructures/DynamicArray.hpp"
#include "StringFragment.hpp"

namespace Solaire{ namespace Core{

    class String{
    public:
        typedef DynamicArray<char, const char, uint32_t> Container;

        typedef typename Container::Type Type;
        typedef typename Container::Type ConstType;
        typedef typename Container::Move Move;
        typedef typename Container::Reference Reference;
        typedef typename Container::ConstReference ConstReference;
        typedef typename Container::Pointer Pointer;
        typedef typename Container::ConstPointer ConstPointer;
        typedef typename Container::Iterator Iterator;
        typedef typename Container::ConstIterator ConstIterator;
        typedef typename Container::ReverseIterator ReverseIterator;
        typedef typename Container::ConstReverseIterator ConstReverseIterator;
    private:
        Container mContainer;
    public:
        // Assignment
        String& operator=(const Core::ConstStringFragment aOther);
        String& operator=(const std::basic_string<Type>& aOther);
        String& operator=(const ConstPointer aOther);
        String& operator=(const String& aOther);
        String& operator=(String&& aOther);

        // Constructors
        String(const String& aOther);
        String(String&& aOther);
        String(Allocator<void>& aAllocator = GetDefaultAllocator<void>());
        String(const Core::ConstStringFragment aOther, Allocator<void>& aAllocator = GetDefaultAllocator<void>());
        String(const std::basic_string<Type>& aOther, Allocator<void>& aAllocator = GetDefaultAllocator<void>());
        String(const ConstPointer aPointer, Allocator<void>& aAllocator = GetDefaultAllocator<void>());
        String(const ConstPointer aPointer, const size_t aSize, Allocator<void>& aAllocator = GetDefaultAllocator<void>());

        template<class ExternalIterator>
        String(const ExternalIterator aBegin, const ExternalIterator aEnd, Allocator<void>& aAllocator = GetDefaultAllocator<void>()) :
            mContainer(aBegin, aEnd, aAllocator)
        {
            if(mContainer.Back() != '\0') mContainer.PushBack('\0');
        }

        // Iterators
        Iterator begin();
        ConstIterator begin() const;
        Iterator end();
        ConstIterator end() const;
        ReverseIterator rbegin();
        ConstReverseIterator rbegin() const;
        ReverseIterator rend();
        ConstReverseIterator rend() const;

        // Erase
        void Erase(const ConstIterator aPos);
        void Erase(const ConstIterator aPos, size_t aCount);
        String& EraseAll(const Type aChar);
        String& EraseAll(const ConstStringFragment aFragment);
        void Clear();

        // Insertion
        Reference InsertBefore(const ConstIterator aPos, const Type aChar);
        Reference InsertAfter(const ConstIterator aPos, const Type aChar);
        StringFragment InsertBefore(const ConstIterator aPos, const ConstStringFragment aFragment);
        StringFragment InsertAfter(const ConstIterator aPos, const ConstStringFragment aFragment);

        // Deque interface
        Reference PushBack(Type aChar);
        Reference PushFront(Type aChar);
        Type PopBack();
        Type PopFront();

        Reference Back();
        ConstReference Back() const;
        Reference Front();
        ConstReference Front() const;

        // Size
        size_t Size() const;
        size_t Capacity() const;

        // Array Offset
        Reference operator[](const size_t aIndex);
        Type operator[](const size_t aIndex) const;

        // Append
        String& operator+=(const char aValue);
        String& operator+=(const char* aValue);
        String& operator+=(const String& aValue);
        String& operator+=(const ConstStringFragment aValue);

        template<class T>
        String& operator+=(const std::basic_string<T>& aValue){
            for(const T c : aValue) PushBack(static_cast<char>(c));
            return *this;
        }

        template<class T>
        String& operator+=(const T& aValue){
            std::stringstream ss;
            ss << aValue;
            ss >> *this;
            return *this;
        }

        //C-String conversion
        ConstPointer CString() const;

        // StringFragment conversion
        operator StringFragment();
        operator ConstStringFragment() const;

        // Comparison
        bool operator==(const String& aOther) const;
        bool operator!=(const String& aOther) const;
        bool operator<(const String& aOther) const;
        bool operator>(const String& aOther) const;
        bool operator<=(const String& aOther) const;
        bool operator>=(const String& aOther) const;

        // Find
        Iterator FindFirst(const Type aChar);
        Iterator FindNext(const Iterator aPos, const Type aChar);
        Iterator FindLast(const Type aChar);
        ConstIterator FindFirst(const Type aChar) const;
        ConstIterator FindNext(const Iterator aPos, const Type aChar) const;
        ConstIterator FindLast(const Type aChar) const;
        StringFragment FindFirst(const ConstStringFragment aFragment);

        StringFragment FindNext(const Iterator aPos, const ConstStringFragment aFragment);
        StringFragment FindLast(const ConstStringFragment aFragment);
        ConstStringFragment FindFirst(const ConstStringFragment aFragment) const;
        ConstStringFragment FindNext(const Iterator aPos, ConstStringFragment aFragment) const;
        ConstStringFragment FindLast(const ConstStringFragment aFragment) const;

        // Replace
		String& ReplaceFirst(const Type aTarget, const Type aReplacement);
        String& ReplaceNext(const ConstIterator aPos, const Type aTarget, const Type aReplacement);
        String& ReplaceLast(const Type aTarget, const Type aReplacement);
        String& ReplaceAll(const Type aTarget, const Type aReplacement);

        String& ReplaceFirst(const ConstStringFragment aTarget, const ConstStringFragment aReplacement);
        String& ReplaceNext(const ConstIterator aPos, const ConstStringFragment aTarget, const ConstStringFragment aReplacement);
        String& ReplaceLast(const ConstStringFragment aTarget, const ConstStringFragment aReplacement);
        String& ReplaceAll(const ConstStringFragment aTarget, const ConstStringFragment aReplacement);

        // Case control
		static void ToLowerCase(const Iterator aPos);
		static void ToUpperCase(const Iterator aPos);
		static void ToggleCase(const Iterator aPos);

		String& ToLowerCase();
		String& ToUpperCase();
		String& ToggleCase();

        // I/Ostream compatibility
        friend std::ostream& operator<<(std::ostream& aStream, const String& aString){
            aStream.write(aString.CString(), aString.Size());
            return aStream;
        }

        friend std::istream& operator>>(std::istream& aStream, String& aString){
            char c;
            while(! aStream.eof()){
                aStream >> c;
                aString += c;
            }
            return aStream;
        }
    };
}}

#include "String.inl"


#endif
