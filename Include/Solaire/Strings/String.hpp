#ifndef SOLAIRE_STRING_HPP
#define SOLAIRE_STRING_HPP

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
#include "ConstString.hpp"
#include "ConstStringTerminated.hpp"

namespace Solaire{

    class String{
    public:
        typedef DynamicArray<char, const char, uint32_t> Container;

        typedef Container::Type Type;
        typedef Container::Type ConstType;
        typedef Container::Move Move;
        typedef Container::Reference Reference;
        typedef Container::ConstReference ConstReference;
        typedef Container::Pointer Pointer;
        typedef Container::ConstPointer ConstPointer;
        typedef Container::Iterator Iterator;
        typedef Container::ConstIterator ConstIterator;
        typedef Container::ReverseIterator ReverseIterator;
        typedef Container::ConstReverseIterator ConstReverseIterator;
    private:
        Container mContainer;
    public:
        // Assignment
        String& operator=(const ConstString<char>& aOther);
        String& operator=(const std::basic_string<Type>& aOther);
        String& operator=(const ConstPointer aOther);
        String& operator=(const String& aOther);
        String& operator=(String&& aOther);

        // Constructors
        String(const String& aOther);
        String(String&& aOther);
        String(Allocator& aAllocator);
        String(Allocator& aAllocator, const ConstString<char>& aOther);
        String(Allocator& aAllocator, const std::basic_string<Type>& aOther);
        String(Allocator& aAllocator, const ConstPointer aPointer);
        String(Allocator& aAllocator, const ConstPointer aPointer, const size_t aSize);
        String(Allocator& aAllocator, const Type aChar, const size_t aCount);

        template<class ExternalIterator>
        String(Allocator& aAllocator, const ExternalIterator aBegin, const ExternalIterator aEnd) :
            mContainer(aAllocator, aBegin, aEnd)
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

        // Misc
        Allocator& GetAllocator() const;

        // Erase
        void Erase(const ConstIterator aPos);
        void Erase(const ConstIterator aPos, size_t aCount);
        String& EraseAll(const Type aChar);
        String& EraseAll(const ConstString<char>& aFragment);
        void Clear();

        // Insertion
        Reference InsertBefore(const ConstIterator aPos, const Type aChar);
        Reference InsertAfter(const ConstIterator aPos, const Type aChar);
        void InsertBefore(const ConstIterator aPos, const ConstString<char>& aFragment);
        void InsertAfter(const ConstIterator aPos, const ConstString<char>& aFragment);

        // Deque interface
        Reference PushBack(Type aChar);
        Reference PushFront(Type aChar);
        Type PopBack();
        Type PopFront();

        Reference Back();
        Type Back() const;
        Reference Front();
        Type Front() const;

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
        String& operator+=(const ConstString<char>& aValue);
		String& operator+=(const uint8_t aValue);
		String& operator+=(const uint16_t aValue);
		String& operator+=(const uint32_t aValue);
		String& operator+=(const uint64_t aValue);
		String& operator+=(const int8_t aValue);
		String& operator+=(const int16_t aValue);
		String& operator+=(const int32_t aValue);
		String& operator+=(const int64_t aValue);
		String& operator+=(const float aValue);
		String& operator+=(const double aValue);

        template<class T>
        String& operator+=(const std::basic_string<T>& aValue){
            for(const T c : aValue) PushBack(static_cast<char>(c));
            return *this;
        }

        /*template<class T>
        String& operator+=(const T& aValue){
            std::stringstream ss;
            ss << aValue;
            ss >> *this;
            return *this;
        }*/

        //C-String conversion
        ConstPointer CString() const;

        // StringFragment conversion
        operator ConstCString() const;

        // Comparison
        bool operator==(const String& aOther) const;
        bool operator!=(const String& aOther) const;
        bool operator<(const String& aOther) const;
        bool operator>(const String& aOther) const;
        bool operator<=(const String& aOther) const;
        bool operator>=(const String& aOther) const;
        bool operator==(const ConstString<char>& aOther) const;
        bool operator!=(const ConstString<char>& aOther) const;
        bool operator<(const ConstString<char>& aOther) const;
        bool operator>(const ConstString<char>& aOther) const;
        bool operator<=(const ConstString<char>& aOther) const;
        bool operator>=(const ConstString<char>& aOther) const;

        // Find
        Iterator FindFirst(const Type aChar);
        Iterator FindNext(const Iterator aPos, const Type aChar);
        Iterator FindLast(const Type aChar);
        ConstIterator FindFirst(const Type aChar) const;
        ConstIterator FindNext(const Iterator aPos, const Type aChar) const;
        ConstIterator FindLast(const Type aChar) const;

        Iterator FindFirst(const ConstString<char>& aFragment);
		Iterator FindNext(const Iterator aPos, const ConstString<char>& aFragment);
		Iterator FindLast(const ConstString<char>& aFragment);
		ConstIterator FindFirst(const ConstString<char>& aFragment) const;
		ConstIterator FindNext(const Iterator aPos, ConstString<char>& aFragment) const;
		ConstIterator FindLast(const ConstString<char>& aFragment) const;

        template<class F>
        Iterator FindFirstIf(const F aCondition){return mContainer.FindFirstIf<F>(aCondition);}
        template<class F>
        Iterator FindNextIf(const ConstIterator aPos, const F aCondition){return mContainer.FindNextIf<F>(aPos, aCondition);}
        template<class F>
        Iterator FindLastIf(const F aCondition){return mContainer.FindLastIf<F>(aCondition);}
        template<class F>
        ConstIterator FindFirstIf(const F aCondition) const{return mContainer.FindFirstIf<F>(aCondition);}
        template<class F>
        ConstIterator FindNextIf(const ConstIterator aPos, const F aCondition) const{return mContainer.FindNextIf<F>(aPos, aCondition);}
        template<class F>
        ConstIterator FindLastIf(const F aCondition) const{return mContainer.FindLastIf<F>(aCondition);}

        // Replace
		String& ReplaceFirst(const Type aTarget, const Type aReplacement);
        String& ReplaceNext(const ConstIterator aPos, const Type aTarget, const Type aReplacement);
        String& ReplaceLast(const Type aTarget, const Type aReplacement);
        String& ReplaceAll(const Type aTarget, const Type aReplacement);

        String& ReplaceFirst(const ConstString<char>& aTarget, const ConstString<char>& aReplacement);
        String& ReplaceNext(const ConstIterator aPos, const ConstString<char>& aTarget, const ConstString<char>& aReplacement);
        String& ReplaceLast(const ConstString<char>& aTarget, const ConstString<char>& aReplacement);
        String& ReplaceAll(const ConstString<char>& aTarget, const ConstString<char>& aReplacement);

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

    /*template<class HASH_TYPE>
    struct HashWrapper<String, HASH_TYPE>{
        static HASH_TYPE Hash(const HashFunction<HASH_TYPE>& aFunction, const String& aValue){
            return aFunction.Hash(aValue.begin(), aValue.Size());
        }
    };*/
}


#endif
