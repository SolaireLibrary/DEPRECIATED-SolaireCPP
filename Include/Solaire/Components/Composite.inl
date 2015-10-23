#ifndef SOLAIRE_COMPONENTS_COMPOSITE_INL
#define SOLAIRE_COMPONENTS_COMPOSITE_INL

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
	\file Composite.inl
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 4.1
	\date
	Created			: 23rd October 2015
	Last Modified	: 23rd October 2015
*/

namespace Solaire{ namespace Components{

    // Composite

    Composite& Composite::operator=(Composite&& aOther){
        std::swap(mComponents, aOther.mComponents);

        for(Component* component : mComponents){
            component->OnParentMove(aOther, *this);
        }

        for(Component* component : aOther.mComponents){
            component->OnParentMove(*this, aOther);
        }

        return *this;
    }

    Composite::Composite(Allocator& aAllocator):
        mComponents(aAllocator, 16)
    {}

    Composite::Composite(const Composite& aOther):
        mComponents(aOther.GetAllocator(), 16)
    {
        for(const Component* component : aOther.mComponents){
            component->AttachCopy(*this);
        }
    }

    Composite::Composite(Composite&& aOther):
        mComponents(std::move(aOther.mComponents))
    {
        for(Component* component : mComponents){
            component->OnParentMove(aOther, *this);
        }
    }

    Composite::~Composite(){
        Allocator& allocator = GetAllocator();
        for(Component* component : mComponents){
            component->~Component();
            //! \bug Size of original component class is not deallocated
            allocator.Deallocate(component, sizeof(Component));
        }
    }

    Allocator& Composite::GetAllocator() const{
        return mComponents.GetAllocator();
    }

    Composite::Iterator Composite::begin(){
        return mComponents.begin();
    }

    Composite::ConstIterator Composite::begin() const{
        return Iterator(const_cast<DynamicArray<Component*>&>(mComponents).begin());
    }

    Composite::Iterator Composite::end(){
        return mComponents.end();
    }

    Composite::ConstIterator Composite::end() const{
        return Iterator(const_cast<DynamicArray<Component*>&>(mComponents).end());
    }

    Composite::ReverseIterator Composite::rbegin(){
        return mComponents.rbegin();
    }

    Composite::ConstReverseIterator Composite::rbegin() const{
        return ReverseIterator(const_cast<DynamicArray<Component*>&>(mComponents).rbegin());
    }

    Composite::ReverseIterator Composite::rend(){
        return mComponents.rend();
    }

    Composite::ConstReverseIterator Composite::rend() const{
        return ReverseIterator(const_cast<DynamicArray<Component*>&>(mComponents).rend());
    }

    Composite::ConstIterator Composite::FindFirst(Component& aValue) const{
        return Iterator(const_cast<DynamicArray<Component*>&>(mComponents).FindFirst(&aValue));
    }

    Composite::ConstIterator Composite::FindNext(ConstIterator aPos, Component& aValue) const{
        return Iterator(const_cast<DynamicArray<Component*>&>(mComponents).FindNext(mComponents.begin() + (end() - aPos), &aValue));
    }

    Composite::ConstIterator Composite::FindLast(Component& aValue) const{
        return Iterator(const_cast<DynamicArray<Component*>&>(mComponents).FindLast(&aValue));
    }

    Composite::Iterator Composite::FindFirst(Component& aValue){
        return Iterator(mComponents.FindFirst(&aValue));
    }

    Composite::Iterator Composite::FindNext(ConstIterator aPos, Component& aValue){
        return Iterator(mComponents.FindNext(mComponents.begin() + (ConstIterator(end()) - aPos), &aValue));
    }

    Composite::Iterator Composite::FindLast(Component& aValue){
        return Iterator(mComponents.FindLast(&aValue));
    }
}}


#endif
