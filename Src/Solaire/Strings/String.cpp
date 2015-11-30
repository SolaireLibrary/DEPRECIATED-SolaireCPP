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

#include <cstring>
#include "Solaire\Strings\String.hpp"
#include "Solaire\Strings\NumberParser.hpp"

namespace Solaire{

    // String

    Allocator& String::GetAllocator() const{
        return mContainer.GetAllocator();
    }

    String& String::operator=(const ConstString<char>& aOther){
        Clear();
        operator+=(aOther);
        return *this;
    }

    String& String::operator=(const std::basic_string<Type>& aOther){
        Clear();
        operator+=(ConstCString(aOther.c_str(), aOther.size()));
        return *this;
    }

    String& String::operator=(const ConstPointer aOther){
        Clear();
        operator+=(ConstCString(aOther));
        return *this;
    }

    String& String::operator=(const String& aOther){
        mContainer = aOther.mContainer;
        return *this;
    }

    String& String::operator=(String&& aOther){
        mContainer = std::move(aOther.mContainer);
        return *this;
    }

    String::String(const String& aOther) :
        mContainer(aOther.mContainer)
    {}

    String::String(String&& aOther) :
        mContainer(std::move(aOther.mContainer))
    {}

    String::String(Allocator& aAllocator) :
        mContainer(aAllocator, 128)
    {
        mContainer.PushBack('\0');
    }

    String::String(Allocator& aAllocator, const ConstPointer aPointer) :
        mContainer(aAllocator, aPointer, aPointer + std::strlen(aPointer))
    {
        mContainer.PushBack('\0');
    }

    String::String(Allocator& aAllocator, const ConstPointer aPointer, const size_t aSize) :
        mContainer(aAllocator, aPointer, aPointer + aSize)
    {
        mContainer.PushBack('\0');
    }

    String::String(Allocator& aAllocator, const std::basic_string<Type>& aOther) :
        mContainer(aAllocator, aOther.begin(), aOther.end())
    {
        mContainer.PushBack('\0');
    }

    String::String(Allocator& aAllocator, const ConstString<char>& aOther) :
        mContainer(aAllocator)
    {
		const uint32_t size = aOther.Size();
		for(uint32_t i = 0; i < size; ++i) {
			mContainer.PushBack(aOther[i]);
		}
        mContainer.PushBack('\0');
    }

    String::String(Allocator& aAllocator, const Type aChar, const size_t aCount) :
        mContainer(aAllocator, aChar, aCount)
    {
        mContainer.PushBack('\0');
    }

    void String::Erase(const ConstIterator aPos){
        mContainer.Erase(aPos);
    }

    void String::Erase(const ConstIterator aPos, size_t aCount){
        while(aCount != 0){
            if(aPos == mContainer.end()) throw std::runtime_error("String::Erase : Cannot erase past end of string");
            mContainer.Erase(aPos);
            --aCount;
        }
    }

    void String::Clear(){
        mContainer.Clear();
        mContainer.PushBack('\0');
    }

    String::Reference String::InsertBefore(const ConstIterator aPos, const Type aChar){
        return mContainer.InsertBefore(aPos, aChar);
    }

    String::Reference String::InsertAfter(const ConstIterator aPos, const Type aChar){
        return mContainer.InsertAfter(aPos, aChar);
    }

    void String::InsertBefore(const ConstIterator aPos, const ConstString<char>& aFragment){
        const size_t pos = aPos - begin();
		//! \todo Implement InsertBefore
        //const ConstReverseIterator end = aFragment.rend();
        //for(ConstReverseIterator i = aFragment.rbegin(); i != end; ++i){
        //    InsertBefore(begin() + pos, *i);
        //}
    }

    void String::InsertAfter(const ConstIterator aPos, const ConstString<char>& aFragment){
        return InsertBefore(aPos + 1, aFragment);
    }

    String::Reference String::PushBack(Type aChar){
		Reference ref = mContainer.Back() = aChar;
		mContainer.PushBack('\0');
		return ref;
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

    String::Type String::Back() const{
        return *(end() - 1);
    }

    String::Reference String::Front(){
        return mContainer.Front();
    }

    String::Type String::Front() const{
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

    String& String::operator+=(const ConstString<char>& aValue){
		mContainer.PopBack();
		const uint32_t size = aValue.Size();
		for (uint32_t i = 0; i < size; ++i) {
			mContainer.PushBack(aValue[i]);
		}
		mContainer.PushBack('\0');
		return *this;
    }

	String& String::operator+=(const uint8_t aValue) {
		return operator+=(WriteNumber(GetAllocator(), static_cast<uint32_t>(aValue)));
	}

	String& String::operator+=(const uint16_t aValue) {
		return operator+=(WriteNumber(GetAllocator(), static_cast<uint32_t>(aValue)));
	}

	String& String::operator+=(const uint32_t aValue) {
		return operator+=(WriteNumber(GetAllocator(), aValue));
	}

	String& String::operator+=(const uint64_t aValue) {
		return operator+=(WriteNumber(GetAllocator(), static_cast<uint32_t>(aValue)));
	}

	String& String::operator+=(const int8_t aValue) {
		return operator+=(WriteNumber(GetAllocator(), static_cast<int32_t>(aValue)));
	}

	String& String::operator+=(const int16_t aValue) {
		return operator+=(WriteNumber(GetAllocator(), static_cast<int32_t>(aValue)));
	}

	String& String::operator+=(const int32_t aValue) {
		return operator+=(WriteNumber(GetAllocator(), aValue));
	}

	String& String::operator+=(const int64_t aValue) {
		return operator+=(WriteNumber(GetAllocator(), static_cast<int32_t>(aValue)));
	}

	String& String::operator+=(const float aValue) {
		return operator+=(WriteNumber(GetAllocator(), static_cast<double>(aValue)));
	}

	String& String::operator+=(const double aValue) {
		return operator+=(WriteNumber(GetAllocator(), aValue));
	}

    String::ConstPointer String::CString() const{
        return &const_cast<Container&>(mContainer)[0];
    }

    String::operator ConstCString() const{
        return ConstCString(begin(), Size());
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
        return std::memcmp(CString(), aOther.CString(), sizeof(Type) * Min(Size(), aOther.Size())) == -1;
    }

    bool String::operator>(const String& aOther) const{
        return std::memcmp(CString(), aOther.CString(), sizeof(Type) * Min(Size(), aOther.Size())) == 1;
    }

    bool String::operator<=(const String& aOther) const{
        return std::memcmp(CString(), aOther.CString(), sizeof(Type) * Min(Size(), aOther.Size())) <= 0;
    }

    bool String::operator>=(const String& aOther) const{
        return std::memcmp(CString(), aOther.CString(), sizeof(Type) * Min(Size(), aOther.Size())) >= 0;
    }

    bool String::operator==(const ConstString<char>& aOther) const{
        return static_cast<ConstCString>(*this) == aOther;
    }

    bool String::operator!=(const ConstString<char>& aOther) const{
        return static_cast<ConstCString>(*this) != aOther;
    }

    bool String::operator<(const ConstString<char>& aOther) const{
        return static_cast<ConstCString>(*this) < aOther;
    }

    bool String::operator>(const ConstString<char>& aOther) const{
        return static_cast<ConstCString>(*this) > aOther;
    }

    bool String::operator<=(const ConstString<char>& aOther) const{
        return static_cast<ConstCString>(*this) <= aOther;
    }

    bool String::operator>=(const ConstString<char>& aOther) const{
        return static_cast<ConstCString>(*this) >= aOther;
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
		return end();// StringFragment(begin(), end()).FindFirst(aChar);
    }

    String::Iterator String::FindNext(const Iterator aPos, const Type aChar){
        return end();// StringFragment(begin(), end()).FindNext(aPos, aChar);
    }

    String::Iterator String::FindLast(const Type aChar){
        return end();// StringFragment(begin(), end()).FindLast(aChar);
    }

    String::ConstIterator String::FindFirst(const Type aChar) const{
        return end();// ConstStringFragment(begin(), end()).FindFirst(aChar);
    }

    String::ConstIterator String::FindNext(const Iterator aPos, const Type aChar) const{
        return end();// ConstStringFragment(begin(), end()).FindNext(aPos, aChar);
    }

    String::ConstIterator String::FindLast(const Type aChar) const{
        return end();// ConstStringFragment(begin(), end()).FindLast(aChar);
    }

	String::Iterator String::FindFirst(const ConstString<char>& aFragment){
        return end();// StringFragment(begin(), end()).FindFirst(aFragment);
    }

	String::Iterator String::FindNext(const Iterator aPos, const ConstString<char>& aFragment){
        return end();// StringFragment(begin(), end()).FindNext(aPos, aFragment);
    }

	String::Iterator String::FindLast(const ConstString<char>& aFragment){
        return end();// StringFragment(begin(), end()).FindLast(aFragment);
    }

	String::ConstIterator String::FindFirst(const ConstString<char>& aFragment) const{
        return end();// ConstStringFragment(begin(), end()).FindFirst(aFragment);
    }

	String::ConstIterator String::FindNext(const Iterator aPos, ConstString<char>& aFragment) const{
        return end();// ConstStringFragment(begin(), end()).FindNext(aPos, aFragment);
    }

    String::ConstIterator String::FindLast(const ConstString<char>& aFragment) const{
        return end();// ConstStringFragment(begin(), end()).FindLast(aFragment);
    }

    void String::ToLowerCase(const Iterator aPos){
        //StringFragment::ToLowerCase(aPos);
    }

    void String::ToUpperCase(const Iterator aPos){
        //StringFragment::ToUpperCase(aPos);
    }

    void String::ToggleCase(const Iterator aPos){
        //StringFragment::ToggleCase(aPos);
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

    String& String::ReplaceFirst(const ConstString<char>& aTarget, const ConstString<char>& aReplacement){
        return ReplaceNext(begin(), aTarget, aReplacement);
    }

    String& String::ReplaceNext(const ConstIterator aPos, const ConstString<char>& aTarget, const ConstString<char>& aReplacement){
        const size_t targetSize = aTarget.Size();
        const size_t replacementSize = aReplacement.Size();

        const Iterator pos = FindNext(const_cast<Iterator>(aPos), aTarget);
        const ConstIterator end = this->end();
        if(pos != end){
            if(targetSize == replacementSize){
                //std::memcpy(pos, aReplacement.begin(), sizeof(Type) * replacementSize);
            }else{
                Erase(pos, targetSize);
                InsertBefore(pos, aReplacement);
            }
        }
        return *this;
    }

    String& String::ReplaceLast(const ConstString<char>& aTarget, const ConstString<char>& aReplacement){
        return ReplaceNext(FindLast(aTarget), aTarget, aReplacement);
    }

    String& String::ReplaceAll(const ConstString<char>& aTarget, const ConstString<char>& aReplacement){
        Iterator it = FindFirst(aTarget);
        while(it != end()){
            ReplaceNext(it, aTarget, aReplacement);
            it = FindNext(it + 1, aTarget);
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

    String& String::EraseAll(const ConstString<char>& aFragment){
        Iterator it = FindFirst(aFragment);
        while(it != end()){
            Erase(it, aFragment.Size());
            it = FindNext(it, aFragment);
        }
        return *this;
    }
}

#ifdef SOLAIRE_ENABLE_TESTING

#include "../Test/Test.hpp"

namespace Solaire{ namespace Test{

    template<>
    struct TestSource<String>{
        static void AddTests(TestManager& aManager){

            aManager.Add(BuildTest<String>("PushBack(char)", [](String& aMessage)->bool{
                String tmp(GetDefaultAllocator());
                tmp.PushBack('T');
                tmp.PushBack('e');
                tmp.PushBack('s');
                tmp.PushBack('t');

                if(tmp.Size() != 4){
                    aMessage = "String returns incorrect size";
                    return false;
                }else if(std::strcmp(tmp.CString(), "Test") != 0){
                    aMessage = "String comparison failed";
                    return false;
                }

                aMessage = "No errors";
                return true;
            }));

            aManager.Add(BuildTest<String>("Append(char)", [](String& aMessage)->bool{
                String tmp(GetDefaultAllocator());
                tmp += 'T';
                tmp += 'e';
                tmp += 's';
                tmp += 't';

                if(tmp.Size() != 4){
                    aMessage = "String returns incorrect size";
                    return false;
                }else if(std::strcmp(tmp.CString(), "Test") != 0){
                    aMessage = "String comparison failed";
                    return false;
                }

                aMessage = "No errors";
                return true;
            }));

            aManager.Add(BuildTest<String>("Append(const char*)", [](String& aMessage)->bool{
                String tmp(GetDefaultAllocator());
                tmp += "Test";

                if(tmp.Size() != 4){
                    aMessage = "String returns incorrect size";
                    return false;
                }else if(std::strcmp(tmp.CString(), "Test") != 0){
                    aMessage = "String comparison failed";
                    return false;
                }

                aMessage = "No errors";
                return true;
            }));

            aManager.Add(BuildTest<String>("Append(ConstStringFragment)", [](String& aMessage)->bool{
                ConstStringFragment frag("Test\0this should not be appended");
                String tmp(GetDefaultAllocator());
                tmp += frag;

                if(tmp.Size() != 4){
                    aMessage = "String returns incorrect size";
                    return false;
                }else if(std::strcmp(tmp.CString(), "Test") != 0){
                    aMessage = "String comparison failed";
                    return false;
                }

                aMessage = "No errors";
                return true;
            }));

            aManager.Add(BuildTest<String>("FindFirst(char)", [](String& aMessage)->bool{
                String tmp(GetDefaultAllocator());
                tmp += "abcdefghijklmnopqrstuvwxyz";

                auto i = tmp.FindFirst('c');
                const size_t expectedPos = 'c' - 'a';

                if(i == tmp.end()){
                    aMessage = "Could not find expected character in any position";
                    return false;
                }else if(i != tmp.begin() + expectedPos){
                    aMessage = "Could not find expected character in expected position";
                    return false;
                }else if(tmp.FindFirst('5') != tmp.end()){
                    aMessage = "Found character that does not exist in string";
                    return false;
                }

                aMessage = "No errors";
                return true;
            }));

            aManager.Add(BuildTest<String>("FindFirst(ConstStringFragment)", [](String& aMessage)->bool{
                String tmp(GetDefaultAllocator());
                tmp += "abcdefghijklmnopqrstuvwxyz";

                const ConstStringFragment frag("lmnop");

                const ConstStringFragment i = tmp.FindFirst(frag);
                const size_t expectedPos = 'l' - 'a';

                if(i == tmp.end()){
                    aMessage = "Could not find expected fragment in any position";
                    return false;
                }else if(i.begin() != tmp.begin() + expectedPos){
                    aMessage = "Could not find expected fragment in expected position";
                    return false;
                }else if(i != frag){
                    aMessage = "Fragments do not match";
                    return false;
                }

                aMessage = "No errors";
                return true;
            }));

        }
    };
}}

#endif
