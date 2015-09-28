#ifndef SOLAIRE_CORE_STRING_INL
#define SOLAIRE_CORE_STRING_INL

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
	\file String.inl
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 28th September 2015
	Last Modified	: 28th September 2015
*/

namespace Solaire{ namespace Core{

    // String

    String::String(Allocator<Type>& aAllocator) :
        mContainer(256, aAllocator)
    {
        mContainer.PushBack('\0');
    }

    String::String(const ConstPointer aPointer, const size_t aSize, Allocator<Type>& aAllocator) :
        mContainer(aPointer, aPointer + aSize, aAllocator)
    {
        mContainer.PushBack('\0');
    }

    String::String(const std::basic_string<Type>& aOther, Allocator<Type>& aAllocator) :
        mContainer(aOther.begin(), aOther.end(), aAllocator)
    {
        mContainer.PushBack('\0');
    }

    void String::Erase(const ConstIterator aPos){
        mContainer.Erase(aPos);
    }

    void String::Erase(const ConstIterator aPos, size_t aCount){
        while(aCount != 0){
            if(aPos == mContainer.end()) throw std::runtime_error("Core::String::Erase : Cannot erase past end of string");
            mContainer.Erase(aPos);
            --aCount;
        }
    }

    String::Reference String::InsertBefore(const ConstIterator aPos, const Type aChar){
        return mContainer.InsertBefore(aPos, aChar);
    }

    String::Reference String::InsertAfter(const ConstIterator aPos, const Type aChar){
        return mContainer.InsertAfter(aPos, aChar);
    }

    StringFragment String::InsertBefore(const ConstIterator aPos, const ConstStringFragment aFragment){
        const size_t pos = aPos - begin();
        const ConstReverseIterator end = aFragment.rend();
        for(ConstReverseIterator i = aFragment.rbegin(); i != end; ++i){
            InsertBefore(begin() + pos, *i);
        }
        return StringFragment(begin() + pos, aFragment.Size());
    }

    StringFragment String::InsertAfter(const ConstIterator aPos, const ConstStringFragment aFragment){
        return InsertBefore(aPos + 1, aFragment);
    }

    String::Reference String::PushBack(Type aChar){
        return mContainer.InsertBefore(mContainer.end() - 1, aChar);
    }

    String::Reference String::PushFront(Type aChar){
        return mContainer.PushFront(aChar);
    }

    String::Type String::PopBack(){
        const ConstIterator i = end() - 1;
        const Type tmp = *i;
        mContainer.Erase(i);
        return tmp;
    }

    String::Type String::PopFront(){
        return mContainer.PopFront();
    }

    String::Reference String::Back(){
        return *(end() - 1);
    }

    String::ConstReference String::Back() const{
        return *(end() - 1);
    }

    String::Reference String::Front(){
        return mContainer.Front();
    }

    String::ConstReference String::Front() const{
        return mContainer.Front();
    }

    size_t String::Size() const{
        return mContainer.Size() - 1;
    }

    size_t String::Capacity() const{
        return mContainer.Capacity() - 1;
    }

    String::Reference String::operator[](const size_t aIndex){
        return mContainer[aIndex];
    }

    String::Type String::operator[](const size_t aIndex) const{
        return mContainer[aIndex];
    }

    String& String::operator+=(const char aValue){
        PushBack(aValue);
        return *this;
    }

    String& String::operator+=(const char* aValue){
        while(*aValue != '\0'){
            PushBack(*aValue);
            ++aValue;
        }
        return *this;
    }

    String& String::operator+=(const String& aValue){
        for(const Type c : aValue) PushBack(c);
        return *this;
    }

    String& String::operator+=(const ConstStringFragment aValue){
        for(const Type c : aValue) PushBack(c);
        return *this;
    }

    String::ConstPointer String::CString() const{
        return &mContainer[0];
    }

    String::operator StringFragment(){
        return StringFragment(begin(), end());
    }

    String::operator ConstStringFragment() const{
        return ConstStringFragment(begin(), end());
    }

    bool String::operator==(const String& aOther) const{
        if(Size() != aOther.Size()) return false;
        return std::memcmp(CString(), aOther.CString(), sizeof(Type) * Size()) == 0;
    }

    bool String::operator!=(const String& aOther) const{
        if(Size() != aOther.Size()) return false;
        return std::memcmp(CString(), aOther.CString(), sizeof(Type) * Size()) != 0;
    }

    bool String::operator<(const String& aOther) const{
        return std::memcmp(CString(), aOther.CString(), sizeof(Type) * std::min(Size(), aOther.Size())) == -1;
    }

    bool String::operator>(const String& aOther) const{
        return std::memcmp(CString(), aOther.CString(), sizeof(Type) * std::min(Size(), aOther.Size())) == 1;
    }

    bool String::operator<=(const String& aOther) const{
        return std::memcmp(CString(), aOther.CString(), sizeof(Type) * std::min(Size(), aOther.Size())) <= 0;
    }

    bool String::operator>=(const String& aOther) const{
        return std::memcmp(CString(), aOther.CString(), sizeof(Type) * std::min(Size(), aOther.Size())) >= 0;
    }

    String::Iterator String::begin(){
        return mContainer.begin();
    }

    String::ConstIterator String::begin() const{
        return mContainer.begin();
    }

    String::Iterator String::end(){
        return mContainer.end() - 1;
    }

    String::ConstIterator String::end() const{
        return mContainer.end() - 1;
    }

    String::ReverseIterator String::rbegin(){
        return mContainer.rbegin() + 1;
    }

    String::ConstReverseIterator String::rbegin() const{
        return mContainer.rbegin() + 1;
    }

    String::ReverseIterator String::rend(){
        return mContainer.rend();
    }

    String::ConstReverseIterator String::rend() const{
        return mContainer.rend();
    }

    String::Iterator String::FindFirst(const Type aChar){
        return StringFragment(begin(), end()).FindFirst(aChar);
    }

    String::Iterator String::FindNext(const Iterator aPos, const Type aChar){
        return StringFragment(begin(), end()).FindNext(aPos, aChar);
    }

    String::Iterator String::FindLast(const Type aChar){
        return StringFragment(begin(), end()).FindLast(aChar);
    }

    String::ConstIterator String::FindFirst(const Type aChar) const{
        return ConstStringFragment(begin(), end()).FindFirst(aChar);
    }

    String::ConstIterator String::FindNext(const Iterator aPos, const Type aChar) const{
        return ConstStringFragment(begin(), end()).FindNext(aPos, aChar);
    }

    String::ConstIterator String::FindLast(const Type aChar) const{
        return ConstStringFragment(begin(), end()).FindLast(aChar);
    }

    StringFragment String::FindFirst(const ConstStringFragment aFragment){
        return StringFragment(begin(), end()).FindFirst(aFragment);
    }

    StringFragment String::FindNext(const Iterator aPos, const ConstStringFragment aFragment){
        return StringFragment(begin(), end()).FindNext(aPos, aFragment);
    }

    StringFragment String::FindLast(const ConstStringFragment aFragment){
        return StringFragment(begin(), end()).FindLast(aFragment);
    }

    ConstStringFragment String::FindFirst(const ConstStringFragment aFragment) const{
        return ConstStringFragment(begin(), end()).FindFirst(aFragment);
    }

    ConstStringFragment String::FindNext(const Iterator aPos, ConstStringFragment aFragment) const{
        return ConstStringFragment(begin(), end()).FindNext(aPos, aFragment);
    }

    ConstStringFragment String::FindLast(const ConstStringFragment aFragment) const{
        return ConstStringFragment(begin(), end()).FindLast(aFragment);
    }

    void String::ToLowerCase(const Iterator aPos){
        StringFragment::ToLowerCase(aPos);
    }

    void String::ToUpperCase(const Iterator aPos){
        StringFragment::ToUpperCase(aPos);
    }

    void String::ToggleCase(const Iterator aPos){
        StringFragment::ToggleCase(aPos);
    }

    String& String::ToLowerCase(){
        const Iterator end = this->end();
        for(Iterator i = begin(); i != end; ++i) ToLowerCase(i);
        return *this;
    }

    String& String::ToUpperCase(){
        const Iterator end = this->end();
        for(Iterator i = begin(); i != end; ++i) ToUpperCase(i);
        return *this;
    }

    String& String::ToggleCase(){
        const Iterator end = this->end();
        for(Iterator i = begin(); i != end; ++i) ToggleCase(i);
        return *this;
    }

    String& String::ReplaceFirst(const Type aTarget, const Type aReplacement){
        return ReplaceNext(begin(), aTarget, aReplacement);
    }

    String& String::ReplaceNext(const ConstIterator aPos, const Type aTarget, const Type aReplacement){
        const Iterator pos = FindNext(const_cast<Iterator>(aPos), aTarget);
        if(pos != end()) *pos = aReplacement;

        return *this;
    }

    String& String::ReplaceLast(const Type aTarget, const Type aReplacement){
        return ReplaceNext(FindLast(aTarget), aTarget, aReplacement);
    }

    String& String::ReplaceAll(const Type aTarget, const Type aReplacement){
        const ConstIterator end = this->end();
        Iterator it = FindFirst(aTarget);
        while(it != end){
            *it = aReplacement;
            it = FindNext(it + 1, aTarget);
        }
        return *this;
    }

    String& String::ReplaceFirst(const ConstStringFragment aTarget, const ConstStringFragment aReplacement){
        return ReplaceNext(begin(), aTarget, aReplacement);
    }

    String& String::ReplaceNext(const ConstIterator aPos, const ConstStringFragment aTarget, const ConstStringFragment aReplacement){
        const size_t targetSize = aTarget.Size();
        const size_t replacementSize = aReplacement.Size();

        const Iterator pos = FindNext(const_cast<Iterator>(aPos), aTarget).begin();
        const ConstIterator end = this->end();
        if(pos != end){
            if(targetSize == replacementSize){
                std::memcpy(pos, aReplacement.begin(), sizeof(Type) * replacementSize);
            }else{
                Erase(pos, targetSize);
                InsertBefore(pos, aReplacement);
            }
        }
        return *this;
    }

    String& String::ReplaceLast(const ConstStringFragment aTarget, const ConstStringFragment aReplacement){
        return ReplaceNext(FindLast(aTarget).begin(), aTarget, aReplacement);
    }

    String& String::ReplaceAll(const ConstStringFragment aTarget, const ConstStringFragment aReplacement){
        Iterator it = FindFirst(aTarget).begin();
        while(it != end()){
            ReplaceNext(it, aTarget, aReplacement);
            it = FindNext(it + 1, aTarget).begin();
        }
        return *this;
    }

    String& String::EraseAll(const Type aChar){
        Iterator it = FindFirst(aChar);
        while(it != end()){
            Erase(it);
            it = FindNext(it, aChar);
        }
        return *this;
    }

    String& String::EraseAll(const ConstStringFragment aFragment){
        Iterator it = FindFirst(aFragment).begin();
        while(it != end()){
            Erase(it, aFragment.Size());
            it = FindNext(it, aFragment).begin();
        }
        return *this;
    }
}}


#endif
