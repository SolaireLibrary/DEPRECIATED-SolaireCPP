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


template<class T>
class TemplatedComponentIterator{
public:
	typedef TemplatedComponentIterator<T> this_t;
private:
	_vector_it mBegin;
	_vector_it mEnd;
	_vector_it mIterator;

	TemplatedComponentIterator() = delete;

	static bool Condition(const _vector_it aIterator){
		return Component::TypeCheck<T>(**aIterator);
	}
public:
	TemplatedComponentIterator(const _vector_it aIterator, const _vector_it aBegin, const _vector_it aEnd) :
		mBegin(aBegin),
		mEnd(aEnd),
		mIterator(aIterator)
	{
		if(mIterator < mEnd){
			while(Condition(mIterator) && mIterator < mEnd) ++mIterator;
		}
	}

	size_t operator-(const this_t aOther) const {
		if (mBegin != aOther.mBegin) throw std::runtime_error("Iterators are not matched");

		size_t count = 0;
		this_t tmp(*this);
		while(tmp != aOther){
			--tmp;
			++count;
		}

		return count;
	}

	this_t& operator++(){
		if(mIterator < mEnd){
			++mIterator;
			while(mIterator < mEnd && ! Condition(mIterator)) ++mIterator;
		}
		return *this;
	}

	this_t operator++(int){
		this_t tmp(*this);
		++*this;
		return tmp;
	}

	this_t& operator--(){
		if(mIterator > mBegin){
			--mIterator;
			while(mIterator > mBegin && ! Condition(mIterator)) --mIterator;
		}
		return *this;
	}

	this_t operator--(int){
		this_t tmp(*this);
		--*this;
		return tmp;
	}

	this_t& operator+=(const size_t aNumber){
		mIterator += aNumber;
		return *this;
	}

	this_t operator+(const size_t aNumber) const{
		return this_t(mIterator, mCondition) += aNumber;
	}

	this_t& operator-=(const size_t aNumber){
		mIterator -= aNumber;
		return *this;
	}

	this_t operator-(const size_t aNumber) const{
		return this_t(mIterator, mCondition) -= aNumber;
	}

	T& operator*(){
		return **mIterator;
	}

	const T& operator*() const {
		return **mIterator;
	}

	T& operator->(){
		return **mIterator;
	}

	const T& operator->() const {
		return **mIterator;
	}

	bool operator==(const this_t aOther) const{
		return mIterator == aOther.mIterator;
	}

	bool operator!=(const this_t aOther) const{
		return mIterator == aOther.mIterator;
	}

	bool operator<(const this_t aOther) const{
		return mIterator < aOther.mIterator;
	}

	bool operator>(const this_t aOther) const{
		return mIterator > aOther.mIterator;
	}

	bool operator<=(const this_t aOther) const{
		return mIterator <= aOther.mIterator;
	}

	bool operator>=(const this_t aOther) const{
		return mIterator >= aOther.mIterator;
	}
};

template<class T>
class ConstTemplatedComponentIterator{
public:
	typedef ConstTemplatedComponentIterator<T> this_t;
private:
	_const_vector_it mBegin;
	_const_vector_it mEnd;
	_const_vector_it mIterator;

	ConstTemplatedComponentIterator() = delete;

	static bool Condition(const _const_vector_it aIterator){
		return Component::TypeCheck<T>(**aIterator);
	}
public:
	ConstTemplatedComponentIterator(const _const_vector_it aIterator, const _const_vector_it aBegin, const _const_vector_it aEnd) :
		mBegin(aBegin),
		mEnd(aEnd),
		mIterator(aIterator)
	{
		if(mIterator < mEnd){
			while(Condition(mIterator) && mIterator < mEnd) ++mIterator;
		}
	}

	ConstTemplatedComponentIterator(const TemplatedComponentIterator<T> aOther) :
		mBegin(reinterpret_cast<const ConstTemplatedComponentIterator<T>&>(aOther).mBegin),
		mEnd(reinterpret_cast<const ConstTemplatedComponentIterator<T>&>(aOther).mEnd),
		mIterator(reinterpret_cast<const ConstTemplatedComponentIterator<T>&>(aOther).mIterator)
	{
		if(mIterator < mEnd){
			while(Condition(mIterator) && mIterator < mEnd) ++mIterator;
		}
	}

	size_t operator-(const this_t aOther) const {
		if (mBegin != aOther.mBegin) throw std::runtime_error("Iterators are not matched");

		size_t count = 0;
		this_t tmp(*this);
		while(tmp != aOther){
			--tmp;
			++count;
		}

		return count;
	}

	this_t& operator++(){
		if(mIterator < mEnd){
			++mIterator;
			while(mIterator < mEnd && ! Condition(mIterator)) ++mIterator;
		}
		return *this;
	}

	this_t operator++(int){
		this_t tmp(*this);
		++*this;
		return tmp;
	}

	this_t& operator--(){
		if(mIterator > mBegin){
			--mIterator;
			while(mIterator > mBegin && ! Condition(mIterator)) --mIterator;
		}
		return *this;
	}

	this_t operator--(int){
		this_t tmp(*this);
		--*this;
		return tmp;
	}

	this_t& operator+=(const size_t aNumber){
		mIterator += aNumber;
		return *this;
	}

	this_t operator+(const size_t aNumber) const{
		return this_t(mIterator, mCondition) += aNumber;
	}

	this_t& operator-=(const size_t aNumber){
		mIterator -= aNumber;
		return *this;
	}

	this_t operator-(const size_t aNumber) const{
		return this_t(mIterator, mCondition) -= aNumber;
	}

	const T& operator*() const {
		return **mIterator;
	}

	const T& operator->() const {
		return **mIterator;
	}

	bool operator==(const this_t aOther) const{
		return mIterator == aOther.mIterator;
	}

	bool operator!=(const this_t aOther) const{
		return mIterator == aOther.mIterator;
	}

	bool operator<(const this_t aOther) const{
		return mIterator < aOther.mIterator;
	}

	bool operator>(const this_t aOther) const{
		return mIterator > aOther.mIterator;
	}

	bool operator<=(const this_t aOther) const{
		return mIterator <= aOther.mIterator;
	}

	bool operator>=(const this_t aOther) const{
		return mIterator >= aOther.mIterator;
	}
};
