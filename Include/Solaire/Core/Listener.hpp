#ifndef SOLAIRE_CORE_LISTENER_HPP
#define SOLAIRE_CORE_LISTENER_HPP

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
	\file Listener.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 26th September 2015
	Last Modified	: 26th September 2015
*/

#include "DataStructures/DynamicArray.hpp"

namespace Solaire{

    class ListenerBase;

    class SourceBase{
    private:
        DynamicArray<ListenerBase*> mListeners;
    protected:
        virtual void OnListenerAdded(ListenerBase& aListener) = 0;
        virtual void OnListenerRemoved(ListenerBase& aListener) = 0;

        typedef typename DynamicArray<ListenerBase*>::Iterator ListenerIterator;
        typedef typename DynamicArray<ListenerBase*>::ConstIterator ConstListenerIterator;

        ListenerIterator ListenerBegin(){
            return mListeners.begin();
        }

        ConstListenerIterator ListenerBegin() const{
            return mListeners.begin();
        }

        ListenerIterator ListenerEnd(){
            return mListeners.end();
        }

        ConstListenerIterator ListenerEnd() const{
            return mListeners.end();
        }

        bool AddListener(ListenerBase& aListener);
        bool RemoveListener(ListenerBase& aListener);
    public:
        friend ListenerBase;

        SourceBase(Allocator& aAllocator = GetDefaultAllocator()) :
            mListeners(32, aAllocator)
        {}

        virtual ~SourceBase();
    };

    class ListenerBase{
    private:
        DynamicArray<SourceBase*> mSources;
    protected:
        virtual void OnSourceAdded(SourceBase& aSource) = 0;
        virtual void OnSourceRemoved(SourceBase& aSource) = 0;

        typedef typename DynamicArray<SourceBase*>::Iterator SourceIterator;
        typedef typename DynamicArray<SourceBase*>::ConstIterator ConstSourceIterator;

        SourceIterator SourceBegin(){
            return mSources.begin();
        }

        ConstSourceIterator SourceBegin() const{
            return mSources.begin();
        }

        SourceIterator SourceEnd(){
            return mSources.end();
        }

        ConstSourceIterator SourceEnd() const{
            return mSources.end();
        }

        bool AddSource(SourceBase& aSource){
            return aSource.AddListener(*this);
        }

        bool RemoveSource(SourceBase& aSource){
            return aSource.RemoveListener(*this);
        }
    public:
        friend SourceBase;

        ListenerBase(Allocator& aAllocator = GetDefaultAllocator()) :
            mSources(32, aAllocator)
        {}

        virtual ~ListenerBase(){
            while(! mSources.IsEmpty()){
                mSources.Back()->RemoveListener(*this);
            }
        }
    };


    bool SourceBase::AddListener(ListenerBase& aListener){
        if(static_cast<void*>(&aListener) == static_cast<void*>(this)) return false;
        if(mListeners.FindFirst(&aListener) != mListeners.end()) return false;

        mListeners.PushBack(&aListener);
        aListener.mSources.PushBack(this);

        OnListenerAdded(aListener);
        aListener.OnSourceAdded(*this);

        return true;
    }

    bool SourceBase::RemoveListener(ListenerBase& aListener){
        const auto it = mListeners.FindFirst(&aListener);
        if(it == mListeners.end()) return false;

        mListeners.Erase(it);
        aListener.mSources.Erase(aListener.mSources.FindFirst(this));

        OnListenerRemoved(aListener);
        aListener.OnSourceRemoved(*this);

        return true;
    }

    SourceBase::~SourceBase(){
        for(ListenerBase* const i : mListeners){
            //OnListenerRemoved(*i);
            i->OnSourceRemoved(*this);
        }
    }

    template<class SourceType, class ListenerType>
    class Source : private SourceBase
    {
    protected:
        virtual void OnListenerAdded(ListenerType& aListener) = 0;
        virtual void OnListenerRemoved(ListenerType& aListener) = 0;

        typedef typename DynamicArray<ListenerType*>::Iterator ListenerIterator;
        typedef typename DynamicArray<ListenerType*>::ConstIterator ConstListenerIterator;

        ListenerIterator ListenerBegin(){
            return reinterpret_cast<ListenerIterator>(SourceBase::ListenerBegin());
        }

        ConstListenerIterator ListenerBegin() const{
            return reinterpret_cast<ConstListenerIterator>(SourceBase::ListenerBegin());
        }

        ListenerIterator ListenerEnd(){
            return reinterpret_cast<ListenerIterator>(SourceBase::ListenerEnd());
        }

        ConstListenerIterator ListenerEnd() const{
            return reinterpret_cast<ConstListenerIterator>(SourceBase::ListenerEnd());
        }

        // Inherited from SourceBase

        void OnListenerAdded(ListenerBase& aListener){
            OnListenerAdded(reinterpret_cast<ListenerType&>(aListener));
        }

        void OnListenerRemoved(ListenerBase& aListener){
            OnListenerRemoved(reinterpret_cast<ListenerType&>(aListener));
        }
    public:
        friend ListenerType;

        Source(Allocator& aAllocator = GetDefaultAllocator()) :
            SourceBase(aAllocator)
        {
            static_assert(std::is_base_of<SourceBase, SourceType>::value, "Ui::Source : SourceType must inherit SourceBase");
            static_assert(std::is_base_of<ListenerBase, ListenerType>::value, "Ui::Source : ListenerType must inherit ListenerBase");
        }

        bool AddListener(ListenerType& aListener){
            return SourceBase::AddListener(aListener);
        }

        bool RemoveListener(ListenerType& aListener){
            return SourceBase::RemoveListener(aListener);
        }

        virtual ~Source(){

        }
    };

    template<class SourceType, class ListenerType>
    class Listener : private ListenerBase
    {
    protected:
        virtual void OnSourceAdded(SourceType& aSource) = 0;
        virtual void OnSourceRemoved(SourceType& aSource) = 0;

        typedef typename DynamicArray<SourceType*>::Iterator SourceIterator;
        typedef typename DynamicArray<SourceType*>::ConstIterator ConstSourceIterator;

        SourceIterator SourceBegin(){
            return reinterpret_cast<SourceIterator>(ListenerBase::SourceBegin());
        }

        ConstSourceIterator SourceBegin() const{
            return reinterpret_cast<ConstSourceIterator>(ListenerBase::SourceBegin());
        }

        SourceIterator SourceEnd(){
            return reinterpret_cast<SourceIterator>(ListenerBase::SourceEnd());
        }

        ConstSourceIterator SourceEnd() const{
            return reinterpret_cast<ConstSourceIterator>(ListenerBase::SourceEnd());
        }

        // Inherited from ListenerBase

        void OnSourceAdded(SourceBase& aSource){
            OnSourceAdded(reinterpret_cast<SourceType&>(aSource));
        }

        void OnSourceRemoved(SourceBase& aSource){
            OnSourceRemoved(reinterpret_cast<SourceType&>(aSource));
        }
    public:
        friend SourceType;

        Listener(Allocator& aAllocator = GetDefaultAllocator()) :
            ListenerBase(aAllocator)
        {
            static_assert(std::is_base_of<SourceBase, SourceType>::value, "Ui::Source : SourceType must inherit SourceBase");
            static_assert(std::is_base_of<ListenerBase, ListenerType>::value, "Ui::Source : ListenerType must inherit ListenerBase");
        }

        bool AddSource(SourceType& aSource){
            return ListenerBase::AddSource(aSource);
        }

        bool RemoveSource(SourceType& aSource){
            return ListenerBase::RemoveSource(aSource);
        }

        virtual ~Listener(){

        }
    };

}


#endif
