#ifndef SOLAIRE_COMPONENTS_TAG_COMPONENT_HPP
#define SOLAIRE_COMPONENTS_TAG_COMPONENT_HPP

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
	\file TagComponent.hpp
	\brief Contains the code for the TagComponent class.
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 3.0
	\date
	Created			: 7th September 2015
	Last Modified	: 19th October 2015
*/

#include "Composite.hpp"
#include "..\Core\Strings\String.hpp"

namespace Solaire{ namespace Components{

	class TagComponent : public Component
	{
	private:
		DynamicArray<String> mTags;
	public:
		typedef DynamicArray<String>::ConstIterator ConstIterator;
		typedef DynamicArray<String>::ConstReverseIterator ConstReverseIterator;

		TagComponent(Composite& aParent):
		    Component(aParent),
		    mTags(aParent.GetAllocator(), 8)
        {}

		TagComponent(Composite& aParent, const String& aTag):
		    Component(aParent),
		    mTags(aParent.GetAllocator(), 8)
        {
            mTags.PushBack(aTag);
        }

		TagComponent(Composite& aParent, const std::initializer_list<String> aTags):
		    Component(aParent),
		    mTags(aParent.GetAllocator(), 8)
        {
            Add(aTags);
        }

		~TagComponent(){

		}
		ConstIterator begin() const{
		    return mTags.begin();
		}

		ConstIterator end() const{
		    return mTags.end();
		}

		ConstReverseIterator rbegin() const{
		    return mTags.rbegin();
		}

		ConstReverseIterator rend() const{
		    return mTags.rend();
		}

		size_t Size() const{
		    return mTags.Size();
		}

		void Add(const String& aTag){
            if(mTags.FindFirst(aTag) != mTags.end()) return;
            mTags.PushBack(aTag);
		}

		void Add(const std::initializer_list<String> aTags){
            for(const String& tag : aTags){
                Add(tag);
            }
		}

		void Erase(const ConstIterator aPos){
		    mTags.Erase(aPos);
		}

		bool Contains(const String& aTag){
		    return mTags.FindFirst(aTag) != mTags.end();
		}

		bool Contains(const std::initializer_list<String> aTags){
		    for(const String& tag : aTags){
                if(! Contains(tag)) return false;
		    }
		    return true;
		}

		ConstIterator FindFirst(const String& aTag) const{return mTags.FindFirst(aTag);}
		ConstIterator FindNext(ConstIterator aPos, const String& aTag) const{return mTags.FindNext(aPos, aTag);}
		ConstIterator FindLast(const String& aTag) const{return mTags.FindLast(aTag);}

		template<class F>
		ConstIterator FindFirstIf(const F aCondition) const{return mTags.FindFirstIf(aCondition);}

		template<class F>
		ConstIterator FindNextIf(ConstIterator aPos, const F aCondition) const{return mTags.FindNext(aPos, aCondition);}

		template<class F>
		ConstIterator FindLastIf(const F aCondition) const{return mTags.FindLastIf(aCondition);}
	};
}}


#endif
