#ifndef SOLAIRE_HISTOGRAM_HPP
#define SOLAIRE_HISTOGRAM_HPP

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
	\file Histogram.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 25th October 2015
	Last Modified	: 25th October 2015
*/

#include <map>

namespace Solaire{

    template<class TYPE, class COUNT_TYPE = int32_t>
    class Histogram{
    public:
        typedef COUNT_TYPE CountType;
        typedef TYPE Type;
		typedef const TYPE ConstType;
		typedef Type& Reference;
		typedef ConstType& ConstReference;
		typedef Type&& Move;
		typedef Type* Pointer;
		typedef ConstType* ConstPointer;
		typedef typename std::map<Type, CountType>::iterator Iterator;
		typedef typename std::map<Type, CountType>::const_iterator ConstIterator;
		typedef typename std::map<Type, CountType>::reverse_iterator ReverseIterator;
		typedef typename std::map<Type, CountType>::const_reverse_iterator ConstReverseIterator;
		typedef typename PassTypes<Type>::ConstType PassType;
    private:
        std::map<Type, CountType> mHistogram;
    public:
        template<class ExternalIterator>
        static Histogram<TYPE, COUNT_TYPE> CountInstances(ExternalIterator aBegin, const ExternalIterator aEnd){
            Histogram<TYPE, COUNT_TYPE> histogram;

            for(aBegin; aBegin != aEnd; ++aBegin){
                histogram.Add(*aBegin);
            }

            return histogram;
        }
    public:

        ////

        CountType Add(PassType aKey, const CountType aIncrement = static_cast<CountType>(1)){
            Iterator it = mHistogram.find(aKey);
            if(it == mHistogram.end()){
                it = mHistogram.emplace(aKey, 0).first;
            }
            it->second += aIncrement;
            return it->second;
        }

        CountType Subtract(PassType aKey, const CountType aIncrement = static_cast<CountType>(1)){
            Iterator it = mHistogram.find(aKey);
            if(it == mHistogram.end()){
                it = mHistogram.emplace(aKey, 0).first;
            }
            it->second -= aIncrement;
            return it->second;
        }

        CountType Multiply(PassType aKey, const CountType aIncrement = static_cast<CountType>(1)){
            Iterator it = mHistogram.find(aKey);
            if(it == mHistogram.end()){
                it = mHistogram.emplace(aKey, 0).first;
            }
            it->second *= aIncrement;
            return it->second;
        }

        CountType Divide(PassType aKey, const CountType aIncrement = static_cast<CountType>(1)){
            Iterator it = mHistogram.find(aKey);
            if(it == mHistogram.end()){
                it = mHistogram.emplace(aKey, 0).first;
            }
            it->second /= aIncrement;
            return it->second;
        }

        ////

        CountType Sum() const{
            CountType sum = 0;
            for(const std::pair<Type, CountType>& i : mHistogram){
                sum += i.second;
            }
            return sum;
        }

        CountType Average() const{
            return Sum() / mHistogram.size();
        }

        ////

        template<class OTHER_COUNT_TYPE>
        Histogram<TYPE, COUNT_TYPE>& operator+=(const Histogram<TYPE, OTHER_COUNT_TYPE>& aOther){
            for(const std::pair<TYPE, OTHER_COUNT_TYPE>& i : aOther.mHistogram){
                Add(i.first, static_cast<CountType>(i.second));
            }

            return *this;
        }

        template<class OTHER_COUNT_TYPE>
        Histogram<TYPE, COUNT_TYPE>& operator-=(const Histogram<TYPE, OTHER_COUNT_TYPE>& aOther){
            for(const std::pair<TYPE, OTHER_COUNT_TYPE>& i : aOther.mHistogram){
                Subtract(i.first, static_cast<CountType>(i.second));
            }

            return *this;
        }

        template<class OTHER_COUNT_TYPE>
        Histogram<TYPE, COUNT_TYPE>& operator*=(const Histogram<TYPE, OTHER_COUNT_TYPE>& aOther){
            for(const std::pair<TYPE, OTHER_COUNT_TYPE>& i : aOther.mHistogram){
                Multiply(i.first, static_cast<CountType>(i.second));
            }

            return *this;
        }

        template<class OTHER_COUNT_TYPE>
        Histogram<TYPE, COUNT_TYPE>& operator/=(const Histogram<TYPE, OTHER_COUNT_TYPE>& aOther){
            for(const std::pair<TYPE, OTHER_COUNT_TYPE>& i : aOther.mHistogram){
                Divide(i.first, static_cast<CountType>(i.second));
            }

            return *this;
        }

        ////

        template<class OTHER_COUNT_TYPE>
        Histogram<TYPE, COUNT_TYPE> operator+(const Histogram<TYPE, OTHER_COUNT_TYPE>& aOther) const{
            return Histogram<TYPE, COUNT_TYPE>(*this) += aOther;
        }

        template<class OTHER_COUNT_TYPE>
        Histogram<TYPE, COUNT_TYPE> operator-(const Histogram<TYPE, OTHER_COUNT_TYPE>& aOther) const{
            return Histogram<TYPE, COUNT_TYPE>(*this) -= aOther;
        }

        template<class OTHER_COUNT_TYPE>
        Histogram<TYPE, COUNT_TYPE> operator*(const Histogram<TYPE, OTHER_COUNT_TYPE>& aOther) const{
            return Histogram<TYPE, COUNT_TYPE>(*this) *= aOther;
        }

        template<class OTHER_COUNT_TYPE>
        Histogram<TYPE, COUNT_TYPE> operator/(const Histogram<TYPE, OTHER_COUNT_TYPE>& aOther) const{
            return Histogram<TYPE, COUNT_TYPE>(*this) /= aOther;
        }

        ////

        Histogram<TYPE, COUNT_TYPE>& operator+=(const COUNT_TYPE aScalar){
            for(const std::pair<TYPE, COUNT_TYPE>& i : mHistogram){
                i.second += aScalar;
            }
            return *this;
        }

        Histogram<TYPE, COUNT_TYPE>& operator-=(const COUNT_TYPE aScalar){
            for(const std::pair<TYPE, COUNT_TYPE>& i : mHistogram){
                i.second -= aScalar;
            }
            return *this;
        }

        Histogram<TYPE, COUNT_TYPE>& operator*=(const COUNT_TYPE aScalar){
            for(const std::pair<TYPE, COUNT_TYPE>& i : mHistogram){
                i.second += aScalar;
            }
            return *this;
        }

        Histogram<TYPE, COUNT_TYPE>& operator/=(const COUNT_TYPE aScalar){
            for(const std::pair<TYPE, COUNT_TYPE>& i : mHistogram){
                i.second += aScalar;
            }
            return *this;
        }

        ////

        Histogram<TYPE, COUNT_TYPE> operator+(const COUNT_TYPE aScalar) const{
            return Histogram<TYPE, COUNT_TYPE>(*this) += aScalar;
        }

        Histogram<TYPE, COUNT_TYPE> operator-(const COUNT_TYPE aScalar) const{
            return Histogram<TYPE, COUNT_TYPE>(*this) -= aScalar;
        }

        Histogram<TYPE, COUNT_TYPE> operator*(const COUNT_TYPE aScalar) const{
            return Histogram<TYPE, COUNT_TYPE>(*this) *= aScalar;
        }

        Histogram<TYPE, COUNT_TYPE> operator/(const COUNT_TYPE aScalar) const{
            return Histogram<TYPE, COUNT_TYPE>(*this) /= aScalar;
        }

        ////

        Iterator begin(){
            return mHistogram.begin();
        }

        ConstIterator begin() const{
            return mHistogram.begin();
        }

        Iterator end(){
            return mHistogram.end();
        }

        ConstIterator end() const{
            return mHistogram.end();
        }

        ReverseIterator rbegin(){
            return mHistogram.rbegin();
        }

        ConstReverseIterator rbegin() const{
            return mHistogram.rbegin();
        }

        ReverseIterator rend(){
            return mHistogram.rend();
        }

        ConstReverseIterator rend() const{
            return mHistogram.rend();
        }
    };

}


#endif
