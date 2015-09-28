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
        String(Allocator<Type>& aAllocator = GetDefaultAllocator<char>()) :
            mContainer(256, aAllocator)
        {
            mContainer.PushBack('\0');
        }

        String(const ConstPointer aPointer, const size_t aSize, Allocator<Type>& aAllocator = GetDefaultAllocator<char>()) :
            mContainer(aPointer, aPointer + aSize, aAllocator)
        {
            mContainer.PushBack('\0');
        }

        String(const std::basic_string<Type>& aOther, Allocator<Type>& aAllocator = GetDefaultAllocator<char>()) :
            mContainer(aOther.begin(), aOther.end(), aAllocator)
        {
            mContainer.PushBack('\0');
        }

        template<class ExternalIterator>
        String(const ExternalIterator aBegin, const ExternalIterator aEnd, Allocator<Type>& aAllocator = GetDefaultAllocator<char>()) :
            mContainer(aBegin, aEnd, aAllocator)
        {
            if(mContainer.Back() != '\0') mContainer.PushBack('\0');
        }

        void Erase(const ConstIterator aPos){
            mContainer.Erase(aPos);
        }

        void Erase(const ConstIterator aPos, size_t aCount){
            while(aCount != 0){
                if(aPos == mContainer.end()) throw std::runtime_error("Core::String::Erase : Cannot erase past end of string");
                mContainer.Erase(aPos);
                --aCount;
            }
        }

        Reference InsertBefore(const ConstIterator aPos, const Type aChar){
            return mContainer.InsertBefore(aPos, aChar);
        }

        Reference InsertAfter(const ConstIterator aPos, const Type aChar){
            return mContainer.InsertAfter(aPos, aChar);
        }

        StringFragment InsertBefore(const ConstIterator aPos, const ConstStringFragment aFragment){
            const size_t pos = aPos - begin();
            const ConstReverseIterator end = aFragment.rend();
            for(ConstReverseIterator i = aFragment.rbegin(); i != end; ++i){
                InsertBefore(begin() + pos, *i);
            }
            return StringFragment(begin() + pos, aFragment.Size());
        }

        StringFragment InsertAfter(const ConstIterator aPos, const ConstStringFragment aFragment){
            return InsertBefore(aPos + 1, aFragment);
        }

        Reference PushBack(Type aChar){
            return mContainer.InsertBefore(mContainer.end() - 1, aChar);
        }

        Reference PushFront(Type aChar){
            return mContainer.PushFront(aChar);
        }

        size_t Size() const{
            return mContainer.Size() - 1;
        }

        size_t Capacity() const{
            return mContainer.Capacity() - 1;
        }

        Reference operator[](const size_t aIndex){
            return mContainer[aIndex];
        }

        Type operator[](const size_t aIndex) const{
            return mContainer[aIndex];
        }

        String& operator+=(const char aValue){
            PushBack(aValue);
            return *this;
        }

        String& operator+=(const char* aValue){
            while(*aValue != '\0'){
                PushBack(*aValue);
                ++aValue;
            }
            return *this;
        }

        template<class T>
        String& operator+=(const std::basic_string<T>& aValue){
            for(const T c : aValue) PushBack(static_cast<char>(c));
            return *this;
        }

        String& operator+=(const String& aValue){
            for(const Type c : aValue) PushBack(c);
            return *this;
        }

        String& operator+=(const StringFragment aValue){
            for(const Type c : aValue) PushBack(c);
            return *this;
        }

        template<class T>
        String& operator+=(const T& aValue){
            std::stringstream ss;
            ss << aValue;
            ss >> *this;
            return *this;
        }

        ConstPointer CString() const{
            return &mContainer[0];
        }

        operator StringFragment(){
            return StringFragment(begin(), end());
        }

        operator ConstStringFragment() const{
            return ConstStringFragment(begin(), end());
        }

        bool operator==(const String& aOther) const{
            if(Size() != aOther.Size()) return false;
            return std::memcmp(CString(), aOther.CString(), sizeof(Type) * Size()) == 0;
        }

        bool operator!=(const String& aOther) const{
            if(Size() != aOther.Size()) return false;
            return std::memcmp(CString(), aOther.CString(), sizeof(Type) * Size()) != 0;
        }

        bool operator<(const String& aOther) const{
            return std::memcmp(CString(), aOther.CString(), sizeof(Type) * std::min(Size(), aOther.Size())) == -1;
        }

        bool operator>(const String& aOther) const{
            return std::memcmp(CString(), aOther.CString(), sizeof(Type) * std::min(Size(), aOther.Size())) == 1;
        }

        bool operator<=(const String& aOther) const{
            return std::memcmp(CString(), aOther.CString(), sizeof(Type) * std::min(Size(), aOther.Size())) <= 0;
        }

        bool operator>=(const String& aOther) const{
            return std::memcmp(CString(), aOther.CString(), sizeof(Type) * std::min(Size(), aOther.Size())) >= 0;
        }

        Reference Back(){
            return *begin();
        }

        ConstReference Back() const{
            return *begin();
        }

        Reference Front(){
            return mContainer.Front();
        }

        ConstReference Front() const{
           return mContainer.Front();
        }

        Iterator begin(){
            return mContainer.begin();
        }

        ConstIterator begin() const{
            return mContainer.begin();
        }

        Iterator end(){
            return mContainer.end() - 1;
        }

        ConstIterator end() const{
            return mContainer.end() - 1;
        }

        ReverseIterator rbegin(){
            return mContainer.rbegin() + 1;
        }

        ConstReverseIterator rbegin() const{
            return mContainer.rbegin() + 1;
        }

        ReverseIterator rend(){
            return mContainer.rend();
        }

        ConstReverseIterator rend() const{
            return mContainer.rend();
        }

        Iterator FindFirst(const Type aChar){
            return StringFragment(begin(), end()).FindFirst(aChar);
        }

        Iterator FindNext(const Iterator aPos, const Type aChar){
            return StringFragment(begin(), end()).FindNext(aPos, aChar);
        }

        Iterator FindLast(const Type aChar){
            return StringFragment(begin(), end()).FindLast(aChar);
        }

        ConstIterator FindFirst(const Type aChar) const{
            return ConstStringFragment(begin(), end()).FindFirst(aChar);
        }

        ConstIterator FindNext(const Iterator aPos, const Type aChar) const{
            return ConstStringFragment(begin(), end()).FindNext(aPos, aChar);
        }

        ConstIterator FindLast(const Type aChar) const{
            return ConstStringFragment(begin(), end()).FindLast(aChar);
        }

        Iterator FindFirst(const ConstStringFragment aFragment){
            return StringFragment(begin(), end()).FindFirst(aFragment);
        }

        Iterator FindNext(const Iterator aPos, const ConstStringFragment aFragment){
            return StringFragment(begin(), end()).FindNext(aPos, aFragment);
        }

        Iterator FindLast(const ConstStringFragment aFragment){
            return StringFragment(begin(), end()).FindLast(aFragment);
        }

        ConstIterator FindFirst(const ConstStringFragment aFragment) const{
            return ConstStringFragment(begin(), end()).FindFirst(aFragment);
        }

        ConstIterator FindNext(const Iterator aPos, ConstStringFragment aFragment) const{
            return ConstStringFragment(begin(), end()).FindNext(aPos, aFragment);
        }

        ConstIterator FindLast(const ConstStringFragment aFragment) const{
            return ConstStringFragment(begin(), end()).FindLast(aFragment);
        }

		static void ToLowerCase(const Iterator aPos){
		    StringFragment::ToLowerCase(aPos);
		}

		static void ToUpperCase(const Iterator aPos){
		     StringFragment::ToUpperCase(aPos);
		}

		static void ToggleCase(const Iterator aPos){
		    StringFragment::ToggleCase(aPos);
		}

		String& ToLowerCase(){
		    const Iterator end = this->end();
		    for(Iterator i = begin(); i != end; ++i) ToLowerCase(i);
		    return *this;
		}

		String& ToUpperCase(){
		    const Iterator end = this->end();
		    for(Iterator i = begin(); i != end; ++i) ToUpperCase(i);
		    return *this;
		}

		String& ToggleCase(){
		    const Iterator end = this->end();
		    for(Iterator i = begin(); i != end; ++i) ToggleCase(i);
		    return *this;
		}

		String& ReplaceFirst(const Type aTarget, const Type aReplacement){
            return ReplaceNext(begin(), aTarget, aReplacement);
        }

        String& ReplaceNext(const ConstIterator aPos, const Type aTarget, const Type aReplacement){
            const Iterator pos = FindNext(const_cast<Iterator>(aPos), aTarget);
            if(pos != end()) *pos = aReplacement;

            return *this;
        }

        String& ReplaceLast(const Type aTarget, const Type aReplacement){
            return ReplaceNext(FindLast(aTarget), aTarget, aReplacement);
        }

        String& ReplaceAll(const Type aTarget, const Type aReplacement){
            const ConstIterator end = this->end();
            Iterator it = FindFirst(aTarget);
            while(it != end){
                *it = aReplacement;
                it = FindNext(it + 1, aTarget);
            }
            return *this;
        }

        String& ReplaceFirst(const ConstStringFragment aTarget, const ConstStringFragment aReplacement){
            return ReplaceNext(begin(), aTarget, aReplacement);
        }

        String& ReplaceNext(const ConstIterator aPos, const ConstStringFragment aTarget, const ConstStringFragment aReplacement){
            const size_t targetSize = aTarget.Size();
            const size_t replacementSize = aTarget.Size();

            const Iterator pos = FindNext(const_cast<Iterator>(aPos), aTarget);
            const ConstIterator end = this->end();
            if(pos != end){
                if(targetSize == replacementSize){
                    std::memcpy(pos, aTarget.begin(), sizeof(Type) * targetSize);
                }else{
                    Erase(pos, targetSize);
                    InsertBefore(pos, aReplacement);
                }
            }
            return *this;
        }

        String& ReplaceLast(const ConstStringFragment aTarget, const ConstStringFragment aReplacement){
            return ReplaceNext(FindLast(aTarget), aTarget, aReplacement);
        }

        String& ReplaceAll(const ConstStringFragment aTarget, const ConstStringFragment aReplacement){
            const ConstIterator end = this->end();
            Iterator it = FindFirst(aTarget);
            while(it != end){
                ReplaceNext(it, aTarget, aReplacement);
                it = FindNext(it + 1, aTarget);
            }
            return *this;
        }

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


#endif
