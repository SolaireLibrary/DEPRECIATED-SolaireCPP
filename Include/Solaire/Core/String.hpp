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
#include "DataStructures/DynamicArray.hpp"

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

        typedef String Self;
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

        Reference PushBack(Type aChar){
            return mContainer.InsertBefore(mContainer.end() - 1, aChar);
        }

        Reference PushFront(Type aChar){
            return mContainer.PushFront(aChar);
        }

        size_t Size() const{
            return mContainer.Size();
        }

        size_t Capacity() const{
            return mContainer.Capacity();
        }

        Reference operator[](const size_t aIndex){
            return mContainer[aIndex];
        }

        ConstReference operator[](const size_t aIndex) const{
            return mContainer[aIndex];
        }

        Self& operator+=(const char aValue){
            PushBack(aValue);
            return *this;
        }

        Self& operator+=(const char* aValue){
            while(*aValue != '\0'){
                PushBack(*aValue);
                ++aValue;
            }
            return *this;
        }

        template<class T>
        Self& operator+=(const std::basic_string<T>& aValue){
            for(const T c : aValue) PushBack(static_cast<char>(c));
            return *this;
        }

        template<class T>
        Self& operator+=(const Self& aValue){
            for(const T c : aValue) PushBack(c);
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

        bool operator==(const Self& aOther) const{
            if(Size() != aOther.Size()) return false;
            return std::memcmp(CString(), aOther.CString(), sizeof(Type) * Size()) == 0;
        }

        bool operator!=(const Self& aOther) const{
            if(Size() != aOther.Size()) return false;
            return std::memcmp(CString(), aOther.CString(), sizeof(Type) * Size()) != 0;
        }

        bool operator<(const Self& aOther) const{
            return std::memcmp(CString(), aOther.CString(), sizeof(Type) * std::min(Size(), aOther.Size())) == -1;
        }

        bool operator>(const Self& aOther) const{
            return std::memcmp(CString(), aOther.CString(), sizeof(Type) * std::min(Size(), aOther.Size())) == 1;
        }

        bool operator<=(const Self& aOther) const{
            return std::memcmp(CString(), aOther.CString(), sizeof(Type) * std::min(Size(), aOther.Size())) <= 0;
        }

        bool operator>=(const Self& aOther) const{
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

        friend std::ostream& operator<<(std::ostream& aStream, const Self& aString) {
            aStream.write(aString.CString(), aString.Size());
        }

        friend std::istream& operator>>(std::istream& aStream, Self& aString){
            char c;
            while(! aStream.eof()){
                aStream >> c;
                aString += c;
            }
        }
    };
}}


#endif
