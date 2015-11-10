#ifndef SOLAIRE_SPARSE_ARRAY_HPP
#define SOLAIRE_SPARSE_ARRAY_HPP

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
	\file SparseArray.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 26th September 2015
	Last Modified	: 26th September 2015
*/

#include <type_traits>
#include <map>
#include "..\Iterators\PairIterator.hpp"
#include "..\Iterators\ConstIterator.hpp"
#include "..\Iterators\ReverseIterator.hpp"

namespace Solaire{

    template<class K, class T>
    class SparseArray{
    public:
        static_assert(std::is_integral<K>::value, "SparseArray : Key type must be an integer value");

        typedef K Key;
        typedef T Type;
        typedef Type* Pointer;
        typedef const Type* ConstPointer;
        typedef Type&& Move;
        typedef Type& Reference;
        typedef const Type& ConstReference;

        typedef PairSecondIteratorWrapper<Type, typename std::map<Key,Type>::iterator> Iterator;
        typedef ConstIteratorWrapper<Type, Iterator> ConstIterator;
        typedef PairSecondIteratorWrapper<Type, typename std::map<Key,Type>::reverse_iterator> ReverseIterator;
        typedef ConstIteratorWrapper<Type, ReverseIterator> ConstReverseIterator;
    private:
        std::map<Key,Type> mMap;
    public:
        SparseArray(){

        }

        SparseArray(const std::initializer_list<Type> aList){
            const auto begin = aList.begin();
            const auto end = aList.end();
            for(auto i = begin; i != end; ++i){
                mMap.emplace(i - begin, *i);
            }
        }

        SparseArray(const std::initializer_list<std::pair<Key, Type>> aList){
            const auto end = aList.end();
            for(auto i = aList.begin(); i != end; ++i){
                mMap.emplace(i->first, i->second);
            }
        }

        ~SparseArray(){

        }

        bool HasEntryAtIndex(const Key aKey) const{
            return mMap.find(aKey) != mMap.end();
        }

        Reference operator[](const Key aKey){
            auto it = mMap.find(aKey);
            if(it == mMap.end()){
                it = mMap.emplace(aKey, Type()).first;
            }
            return it->second;
        }

        ConstReference operator[](const Key aKey) const{
            auto it = mMap.find(aKey);
            if(it == mMap.end()){
                throw std::runtime_error("SparseArray does not contain key");
            }
            return it->second;
        }

        Iterator begin(){
            return Iterator(mMap.begin());
        }

        ConstIterator begin() const{
            return ConstIterator(mMap.begin());
        }

        Iterator end(){
            return Iterator(mMap.end());
        }

        ConstIterator end() const{
            return ConstIterator(mMap.end());
        }

        ReverseIterator rbegin(){
            return ReverseIterator(mMap.rbegin());
        }

        ConstReverseIterator rbegin() const{
            return ConstReverseIterator(mMap.rbegin());
        }

        ReverseIterator rend(){
            return ReverseIterator(mMap.rend());
        }

        ConstReverseIterator rend() const{
            return ConstReverseIterator(mMap.rend());
        }
    };

}


#endif
