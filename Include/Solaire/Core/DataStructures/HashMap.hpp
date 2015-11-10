#ifndef SOLAIRE_HASH_MAP_HPP
#define SOLAIRE_HASH_MAP_HPP

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
	\file HashMap.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 10th October 2015
	Last Modified	: 10th October 2015
*/

#include "DynamicArray.hpp"

namespace Solaire{

    template<class KEY, class TYPE, class HASH_TYPE, class HASH_CLASS, const uint16_t BUCKET_COUNT>
	class HashMap{
	public:
	    typedef KEY Key;
		typedef typename PassTypes<Key>::ConstType PassKey;

		typedef TYPE Type;
		typedef typename PassTypes<Type>::ConstType PassType;

		typedef HashMap<KEY, TYPE, HASH_TYPE, HASH_CLASS, BUCKET_COUNT> Self;
        typedef std::pair<Key, Type> Mapping;

        //! \TODO Implement Iterators
		typedef Mapping* Iterator;
		typedef ConstIteratorWrapper<Mapping, Iterator> ConstIterator;
		typedef ReverseIteratorWrapper<Mapping, Iterator> ReverseIterator;
		typedef ConstIteratorWrapper<Mapping, ReverseIterator> ConstReverseIterator;
	private:
        typedef DynamicArray<Mapping, const Mapping, uint16_t> Bucket;
        Allocator* mAllocator;
        Bucket* mBuckets;
        HASH_CLASS mHash;
    private:
        template<class KEY_2 = Key>
        HASH_TYPE HashKey(typename PassTypes<KEY_2>::ConstType aKey) const{
            return HashWrapper<KEY_2, HASH_TYPE>::Hash(mHash, aKey) % BUCKET_COUNT;
        }

        HashMap(const HashMap&) = delete;
        HashMap(HashMap&&) = delete;
        HashMap& operator=(const HashMap&) = delete;
        HashMap& operator=(HashMap&&) = delete;
	public:
	    HashMap(Allocator& aAllocator):
	        mAllocator(&aAllocator),
	        mBuckets(static_cast<Bucket*>(aAllocator.AllocateAndRegister<Bucket>(BUCKET_COUNT))),
	        mHash()
	    {
	        Bucket* end = mBuckets + BUCKET_COUNT;
	        for(Bucket* i = mBuckets; i != end; ++i){
                new(i) Bucket(*mAllocator, 8);
	        }
	    }

        ~HashMap(){
            if(mBuckets != nullptr){
                mAllocator->Deallocate<Bucket>(mBuckets, BUCKET_COUNT);
            }
        }

        template<class KEY_2 = Key>
	    Type& Emplace(typename PassTypes<KEY_2>::ConstType aKey, PassType aValue){
            Bucket& bucket = mBuckets[HashKey<KEY_2>(aKey)];

            const Mapping mapping(aKey, aValue);
            if(bucket.IsEmpty()){
                return bucket.PushBack(mapping).second;
            }else{
                for(Mapping& i : bucket){
                    if(i.first == aKey){
                        return i.second = aValue;
                    }
                }
                return bucket.PushBack(mapping).second;
            }

	    }

        template<class KEY_2 = Key>
        bool Erase(typename PassTypes<KEY_2>::ConstType aKey){
            Bucket& bucket = mBuckets[HashKey<KEY_2>(aKey)];
            auto it = bucket.FindFirst([aKey](const Mapping& aMapping)->bool{
                return aMapping.first == aKey;
            });

            if(it == bucket.end()) return false;
            bucket.Erase(it);
            return true;
        }

        template<class KEY_2 = Key>
        ConstIterator FindFirst(typename PassTypes<KEY_2>::ConstType aKey) const{
            Bucket& bucket = mBuckets[HashKey<KEY_2>(aKey)];
            for(const Mapping& i : bucket){
                if(i.first == aKey) return const_cast<Mapping*>(&i);
            }
            return nullptr;
        }

		Allocator& GetAllocator() const{
		    return *mAllocator;
		}
	};
}

#endif
