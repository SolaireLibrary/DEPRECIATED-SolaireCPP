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

#include "Solaire\Component\TagComponentImplementation.hpp"

namespace Solaire{

	// TagComponentImplementation

	TagComponentImplementation::TagComponentImplementation() :
		mTags(nullptr),
		mParent(nullptr)
	{}

	void SOLAIRE_EXPORT_CALL TagComponentImplementation::OnAttachment(Composite& aParent) throw() {
		mParent = &aParent;
		Allocator& allocator = aParent.GetAllocator();
		mTags = new(allocator.Allocate(sizeof(DynamicArray<CString>))) DynamicArray<CString>(allocator);
	}

	void SOLAIRE_EXPORT_CALL TagComponentImplementation::OnDetachment() throw() {
		mTags->~DynamicArray();
		mParent->GetAllocator().Deallocate(mTags);
		mTags = nullptr;
		mParent = nullptr;
	}

	bool SOLAIRE_EXPORT_CALL TagComponentImplementation::CanAttachTo(const Composite& aParent) const throw() {
		return true;
	}

	Allocator& SOLAIRE_EXPORT_CALL TagComponentImplementation::GetAllocator() const throw() {
		return mParent->GetAllocator();
	}

	Composite& SOLAIRE_EXPORT_CALL TagComponentImplementation::GetComposite() const throw() {
		return *mParent;
	}

	bool SOLAIRE_EXPORT_CALL TagComponentImplementation::IsAttached() const throw() {
		return mParent != nullptr;
	}

	ComponentID SOLAIRE_EXPORT_CALL TagComponentImplementation::GetClassID() const throw() {
		return TagComponent::CLASS_ID;
	}

	bool SOLAIRE_EXPORT_CALL TagComponentImplementation::CanDetach() const throw() {
		return mParent != nullptr;
	}

	void SOLAIRE_EXPORT_CALL TagComponentImplementation::Destructor() throw() {

	}

	bool SOLAIRE_EXPORT_CALL TagComponentImplementation::AddTag(const ConstString<char>& aTag) throw() {
		if(mTags == nullptr) return false;
		for(const CString& tag : *mTags) {
			if(tag == aTag) return true;
		}
		mTags->PushBack(CString(GetAllocator(), aTag));
		return true;
	}

	bool SOLAIRE_EXPORT_CALL TagComponentImplementation::HasTag(const ConstString<char>& aTag) const throw() {
		if(mTags == nullptr) return false;
		const auto end = mTags->end();
		for(auto i = mTags->begin(); end; ++i) {
			if (*i == aTag){
				return true;
			}
		}
		return false;
	}

	bool SOLAIRE_EXPORT_CALL TagComponentImplementation::RemoveTag(const ConstString<char>& aTag) throw() {
		if(mTags == nullptr) return false;
		const auto end = mTags->end();
		for(auto i = mTags->begin(); end; ++i) {
			if (*i == aTag){
				mTags->Erase(i);
				return true;
			}
		}
		return false;
	}

}
