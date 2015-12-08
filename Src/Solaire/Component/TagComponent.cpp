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

#include "Solaire\Component\TagComponent.hpp"
#include "Solaire\DataStructures\DynamicArray.hpp"
#include "solaire\Strings\String.hpp"

namespace Solaire{

	class TagComponentImplementation : public TagComponent {
	private:
		DynamicArray<CString> mTags;
		Composite* mParent;
	protected:
		// Inherited from Component
		void SOLAIRE_EXPORT_CALL OnAttachment(Composite& aParent) throw() override {
			mParent = &aParent;
			Allocator& allocator = aParent.GetAllocator();
			mTags.Clear();
		}

		void SOLAIRE_EXPORT_CALL OnDetachment() throw() override {
			mParent = nullptr;
		}

		bool SOLAIRE_EXPORT_CALL CanAttachTo(const Composite&) const throw() override {
			return true;
		}

	public:
		TagComponentImplementation(Allocator& aAllocator) :
			mTags(aAllocator),
			mParent(nullptr)
		{}

		SOLAIRE_EXPORT_CALL ~TagComponentImplementation() {

		}

		// Inherited from Component
		Allocator& SOLAIRE_EXPORT_CALL GetAllocator() const throw() override {
			return mParent->GetAllocator();
		}

		Composite& SOLAIRE_EXPORT_CALL GetComposite() const throw() override {
			return *mParent;
		}

		bool SOLAIRE_EXPORT_CALL IsAttached() const throw() override {
			return mParent != nullptr;
		}

		ComponentID SOLAIRE_EXPORT_CALL GetClassID() const throw() override {
			return TagComponent::CLASS_ID;
		}

		bool SOLAIRE_EXPORT_CALL CanDetach() const throw() override {
			return mParent != nullptr;
		}

		// Inherited from TagComponent
		bool SOLAIRE_EXPORT_CALL AddTag(const ConstString<char>& aTag) throw() override {
			for(const CString& tag : mTags) {
				if (tag == aTag) return true;
			}
			mTags.PushBack(CString(aTag));
			return true;
		}

		bool SOLAIRE_EXPORT_CALL HasTag(const ConstString<char>& aTag) const throw() override {
			const auto end = mTags.end();
			for(auto i = mTags.begin(); end; ++i) {
				if(*i == aTag) {
					return true;
				}
			}
			return false;
		}

		bool SOLAIRE_EXPORT_CALL RemoveTag(const ConstString<char>& aTag) throw() override {
			const auto end = mTags.end();
			for(auto i = mTags.begin(); end; ++i) {
				if (*i == aTag){
					mTags.Erase(i);
					return true;
				}
			}
			return false;
		}
	};

	extern "C" {
		SOLAIRE_EXPORT_API TagComponent* SOLAIRE_EXPORT_CALL _CreateTagComponent(Allocator& aAllocator) {
			return aAllocator.AllocateObject<TagComponentImplementation>(aAllocator);
		}
	}

}
