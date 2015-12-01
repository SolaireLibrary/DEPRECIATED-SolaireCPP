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

#include "Solaire\Component\CompositeImplementation.hpp"
#include "Solaire\Component\Component.hpp"

namespace Solaire{

	// CompositeImplementation

	CompositeImplementation::CompositeImplementation() :
		mComponents(GetDefaultAllocator())
	{}

	CompositeImplementation::CompositeImplementation(Allocator& aAllocator) :
		mComponents(aAllocator)
	{}

	bool SOLAIRE_EXPORT_CALL CompositeImplementation::Attach(Component& aComponent) throw() {
		if(! aComponent.CanAttachTo(*this)) return false;

		if(aComponent.IsAttached()) {
			if(aComponent.IsAttachedTo(*this)) {
				return true;
			}else{
				if(! aComponent.Detach()) return false;
			}
		}

		mComponents.PushBack(&aComponent);
		aComponent.OnAttachment(*this);

		return true;
	}

	bool SOLAIRE_EXPORT_CALL CompositeImplementation::Detach(Component& aComponent) throw() {
		if(! aComponent.IsAttachedTo(*this)) return false;

		if(! aComponent.CanDetach()) return false;

		mComponents.Erase(mComponents.FindFirst(&aComponent));
		aComponent.OnDetachment();

		return true;
	}

	bool SOLAIRE_EXPORT_CALL CompositeImplementation::IsAttached(const Component& aComponent) const throw() {
		return aComponent.IsAttachedTo(*this);
	}

	uint32_t SOLAIRE_EXPORT_CALL CompositeImplementation::GetComponentCount() const throw() {
		return mComponents.Size();
	}

	Component* SOLAIRE_EXPORT_CALL CompositeImplementation::GetComponent(const uint32_t aIndex) const throw() {
		return aIndex >= mComponents.Size() ? nullptr : mComponents[aIndex];
	}

	Component* SOLAIRE_EXPORT_CALL CompositeImplementation::GetComponentWithID(const ComponentID aID) const throw() {
		for(const Component* i : mComponents) {
			if (i->GetClassID() == aID) return const_cast<Component*>(i);
		}
		return nullptr;
	}

	void SOLAIRE_EXPORT_CALL CompositeImplementation::Destructor() throw() {
		for(Component* i : mComponents) {
			i->OnDetachment();
			i->Destructor();
			i->GetAllocator().Deallocate(i);
		}
		mComponents.~DynamicArray();
	}

	Allocator& SOLAIRE_EXPORT_CALL CompositeImplementation::GetAllocator() const throw() {
		return mComponents.GetAllocator();
	}

}
