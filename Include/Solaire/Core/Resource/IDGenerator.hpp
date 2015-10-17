#ifndef SOLAIRE_ID_GENERATOR_HPP
#define SOLAIRE_ID_GENERATOR_HPP

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
	\file IDGenerator.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 17th October 2015
	Last Modified	: 17th October 2015
*/

#include "..\Threading\ThreadTypes.hpp"
#include "..\DataStructures\DynamicArray.hpp"

namespace Solaire{

    template<class ID_TYPE, const bool REUSE_IDS>
    class IDGenerator{};

    template<class ID_TYPE>
    class IDGenerator<ID_TYPE, true>{
    public:
        typedef ID_TYPE ID;
    private:
        DynamicArray<ID> mReturnedIDs;
        RecursiveMutex mLock;
        ID mBase;
    public:
        IDGenerator(Allocator& aAllocator):
            mReturnedIDs(aAllocator),
            mBase(static_cast<ID>(0))
        {}

        bool IsUsed(const ID aID){
            return aID < mBase ? mReturnedIDs.FindFirst(aID) != mReturnedIDs.end() : false;
        }

        void ReturnID(const ID aID){
            solaire_synchronized(mLock,
                mReturnedIDs.PushBack(aID);
            )
        }

        ID GenerateID(){
            solaire_synchronized(mLock,
                if(! mReturnedIDs.IsEmpty()){
                    return mReturnedIDs.PopBack();
                }
                return mBase++;
            )
        }

        RecursiveMutex& GetLock() const{
            return mLock;
        }
    };

    template<class ID_TYPE>
    class IDGenerator<ID_TYPE, false>{
    public:
        typedef ID_TYPE ID;
    private:
        ID mBase;
    public:
        IDGenerator():
            mBase(static_cast<ID>(0))
        {}

        bool IsUsed(const ID aID){
            return aID < mBase;
        }

        ID GenerateID(){
            return mBase++;
        }
    };

    template<class ID_TYPE, const bool REUSE_IDS>
    class IDHolder{};

    template<class ID_TYPE>
    class IDHolder<ID_TYPE, true>{
    public:
        typedef ID_TYPE ID;
    private:
        IDGenerator<ID_TYPE, true>& mGenerator;
        ID mID;
    private:
        IDHolder(const IDHolder&) = delete;
        IDHolder(IDHolder&&) = delete;
        IDHolder& operator=(const IDHolder&) = delete;
        IDHolder& operator=(IDHolder&&) = delete;
    public:
        IDHolder(IDGenerator<ID_TYPE, true>& aGenerator):
            mGenerator(aGenerator),
            mID(aGenerator.GenerateID())
        {}

        virtual ~IDHolder(){
            mGenerator.ReturnID(mID);
        }

        ID GetID() const{
            return mID;
        }

        bool operator==(const IDHolder<ID_TYPE, true>& aOther) const{
            return mID == aOther.mID;
        }

        bool operator!=(const IDHolder<ID_TYPE, true>& aOther) const{
            return mID != aOther.mID;
        }
    };

    template<class ID_TYPE>
    class IDHolder<ID_TYPE, false>{
    public:
        typedef ID_TYPE ID;
    private:
        IDGenerator<ID_TYPE, false>& mGenerator;
        ID mID;
    private:
        IDHolder(const IDHolder&) = delete;
        IDHolder(IDHolder&&) = delete;
        IDHolder& operator=(const IDHolder&) = delete;
        IDHolder& operator=(IDHolder&&) = delete;
    public:
        IDHolder(IDGenerator<ID_TYPE, false>& aGenerator):
            mGenerator(aGenerator),
            mID(aGenerator.GenerateID())
        {}

        virtual ~IDHolder(){

        }

        ID GetID() const{
            return mID;
        }

        bool operator==(const IDHolder<ID_TYPE, true>& aOther) const{
            return mID == aOther.mID;
        }

        bool operator!=(const IDHolder<ID_TYPE, true>& aOther) const{
            return mID != aOther.mID;
        }
    };
}

#endif
