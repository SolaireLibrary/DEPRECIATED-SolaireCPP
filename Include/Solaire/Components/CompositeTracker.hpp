#ifndef SOLAIRE_COMPONENTS_COMPOSITE_TRACKER_HPP
#define SOLAIRE_COMPONENTS_COMPOSITE_TRACKER_HPP

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
	\file CompositeTracker.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 20th October 2015
	Last Modified	: 20th October 2015
*/

#include "Composite.hpp"

namespace Solaire{ namespace Components{

    class CompositeTracker{
    public:
        friend class CompositeTrackerComponent;
    protected:
        virtual void OnCompositeCreated(Composite& aComposite) = 0;
        virtual void OnCompositeDestroyed(Composite& aComposite) = 0;
    public:
        virtual ~CompositeTracker();
    };

	class CompositeTrackerComponent : public Component
	{
	private:
		CompositeTracker& mTracker;
    protected:
        // Inherited from Component
		void AttachCopy(Composite& aComposite) const override{
		   aComposite.Attach<CompositeTrackerComponent>(mTracker);
		}
	public:
		CompositeTrackerComponent(Composite& aParent, CompositeTracker& aTracker):
		    Component(aParent),
		    mTracker(aTracker)
        {
            mTracker.OnCompositeCreated(GetParent());
        }

		~CompositeTrackerComponent(){
            mTracker.OnCompositeDestroyed(GetParent());
		}
	};
}}


#endif
