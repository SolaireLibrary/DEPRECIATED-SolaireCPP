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

#include "Solaire\Component\Composite.hpp"
#include "Solaire\Component\Component.hpp"
#include "Solaire\DataStructures\DynamicArray.hpp"

namespace Solaire{

	class CompositeImplementation : public Composite {
	private:
		DynamicArray<Component*> mComponents;
	public:
		CompositeImplementation(Allocator& aAllocator) :
			mComponents(aAllocator)
		{}

		SOLAIRE_EXPORT_CALL ~CompositeImplementation() throw() {
			for (Component* i : mComponents) {
				i->OnDetachment();
				i->~Component();
				i->GetAllocator().Deallocate(i);
			}
		}

		// Inherited from Composite

		bool SOLAIRE_EXPORT_CALL Attach(Component& aComponent) throw() override {
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
		bool SOLAIRE_EXPORT_CALL Detach(Component& aComponent, const bool aDestroy) throw() override {
			if(! aComponent.IsAttachedTo(*this)) return false;

			if(! aComponent.CanDetach()) return false;

			mComponents.Erase(mComponents.FindFirst(&aComponent));
			aComponent.OnDetachment();

			if(aDestroy) {
				aComponent.~Component();
				return aComponent.GetAllocator().Deallocate(&aComponent);
			}

			return true;
		}

		bool SOLAIRE_EXPORT_CALL IsAttached(const Component& aComponent) const throw() override {
			return aComponent.IsAttachedTo(*this);
		}

		uint32_t SOLAIRE_EXPORT_CALL GetComponentCount() const throw() override {
			return mComponents.Size();
		}

		Component* SOLAIRE_EXPORT_CALL GetComponent(const uint32_t aIndex) const throw() override {
			return aIndex >= mComponents.Size() ? nullptr : mComponents[aIndex];
		}

		Component* SOLAIRE_EXPORT_CALL GetComponentWithID(const ComponentID aID) const throw() override {
			for(const Component* i : mComponents) {
				if(i->GetClassID() == aID) return const_cast<Component*>(i);
			}
			return nullptr;
		}

		Allocator& SOLAIRE_EXPORT_CALL GetAllocator() const throw() override {
			return mComponents.GetAllocator();
		}
	};

	extern "C" {
		SOLAIRE_EXPORT_API Composite* SOLAIRE_EXPORT_CALL _CreateComposite(Allocator& aAllocator) {
			return aAllocator.AllocateObject<CompositeImplementation>(aAllocator);
		}
	}
}
