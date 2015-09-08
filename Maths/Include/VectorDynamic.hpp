#ifndef SOLAIRE_MATHS_VECTOR_DYNAMIC_HPP
#define SOLAIRE_MATHS_VECTOR_DYNAMIC_HPP

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
	\file VectorDynamic.hpp
	\brief Contains code for N dimentional vector maths.
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date 
	Created			: 8th September 2015
	Last Modified	: 8th September 2015
*/

#include <cstdint>
#include <stdexcept>
#include <vector>
#include "VectorUtility.hpp"

namespace Solaire{ namespace Maths{

	/*!
		\class VectorDynamic
		\tparam TYPE The data type used by the vector.
		\brief A N dimentional vector implementation.
		\version 1.0
		\author
		Created			: Adam Smith
		Last modified	: Adam Smith
		\date 
		Created			: 8th September 2015
		Last Modified	: 8th September 2015
	*/
	template<typename TYPE>
	class VectorDynamic{
	private:
		std::vector<TYPE> mComponents;
	public:
		typedef TYPE value_t;	//!< The data type used by the vector.

		// Constructors

		/*!
			\brief Create a vector initialised to 0.
			\param aSize The number of components to initialise.
			\param aValue The default value to initialise.
		*/
		VectorDynamic(size_t aSize, value_t aValue = static_cast<value_t>(0)) :
			mComponents(aSize, aValue)
		{

		}

		// Operator overloads

		/*!
			\brief Check if two vectors are equal.
			\param aOther The vector to compare this vector against.
			\return True if all vector components are equal.
		*/
		bool operator==(const VectorDynamic<value_t>& aOther) const{
			if(Length() != aOther.Length()) return false;

			const value_t* i = begin();
			const value_t* j = aOther.begin();
			const value_t* const end = end();

			while(i != end){
				if(*i != *j) return false;
				++i;
				++j;
			}
			return true;
		}
		
		/*!
			\brief Check if two vectors are not equal.
			\param aOther The vector to compare this vector against.
			\return True if at least one vector component is not equal.
		*/
		bool operator!=(const VectorDynamic<value_t>& aOther) const{
			return ! operator==(aOther);
		}

		/*!
			\brief Add two vectors.
			\param aOther The vector to add to this vector.
			\return A reference to this vector.
		*/
		VectorDynamic<value_t>& operator+=(const VectorDynamic<value_t>& aOther){
			if(Length() != aOther.Length()) throw std::runtime_error("Incompatible vector lengths");

			value_t* i = begin();
			const value_t* j = aOther.begin();
			const value_t* const end = end();

			while(i != end){
				*i += *j;
				++i;
				++j;
			}

			return *this;
		}
		
		/*!
			\brief Add a vector and a scalar value.
			\param aScalar The scalar to add to this vector.
			\return A reference to this vector.
		*/
		VectorDynamic<value_t>& operator+=(const value_t aScalar){
			value_t* i = begin();
			const value_t* const end = end();

			while(i != end){
				*i += aScalar;
				++i;
			}

			return *this;
		}
		
		/*!
			\brief Add two vectors.
			\param aOther The vector to add to this vector.
			\return The result of the operation.
		*/
		VectorDynamic<value_t> operator+(const VectorDynamic<value_t>& aOther) const{
			return VectorDynamic<value_t>(*this) += aOther;
		}
		
		/*!
			\brief Add a vector and a scalar value.
			\param aScalar The scalar to add to this vector.
			\return The result of the operation.
		*/
		VectorDynamic<value_t> operator+(const value_t aScalar) const{
			return VectorDynamic<value_t>(*this) += aScalar;
		}
		
		/*!
			\brief Subtract two vectors.
			\param aOther The vector to subtract from this vector.
			\return A reference to this vector.
		*/
		VectorDynamic<value_t>& operator-=(const VectorDynamic<value_t>& aOther){
			if(Length() != aOther.Length()) throw std::runtime_error("Incompatible vector lengths");

			value_t* i = begin();
			const value_t* j = aOther.begin();
			const value_t* const end = end();

			while(i != end){
				*i -= *j;
				++i;
				++j;
			}

			return *this;
		}
		
		/*!
			\brief Subtract a scalar value from a vector.
			\param aScalar The scalar to subtract from this vector.
			\return A reference to this vector.
		*/
		VectorDynamic<value_t>& operator-=(const value_t aScalar){
			value_t* i = begin();
			const value_t* const end = end();

			while(i != end){
				*i -= aScalar;
				++i;
			}

			return *this;
		}
		
		/*!
			\brief Subtract two vectors.
			\param aOther The vector to subtract from this vector.
			\return The result of the operation.
		*/
		VectorDynamic<value_t> operator-(const VectorDynamic<value_t>& aOther) const{
			return VectorDynamic<value_t>(*this) -= aOther;
		}
		
		/*!
			\brief Subtract a scalar from a vector.
			\param aScalar The scalar to subtract from this vector.
			\return A reference to this vector.
		*/
		VectorDynamic<value_t> operator-(const value_t aScalar) const{
			return VectorDynamic<value_t>(*this) -= aScalar;
		}
		
		/*!
			\brief Multiply two vectors.
			\param aOther The vector to multiply this vector by.
			\return A reference to this vector.
		*/
		VectorDynamic<value_t>& operator*=(const VectorDynamic<value_t>& aOther){
			if(Length() != aOther.Length()) throw std::runtime_error("Incompatible vector lengths");

			value_t* i = begin();
			const value_t* j = aOther.begin();
			const value_t* const end = end();

			while(i != end){
				*i *= *j;
				++i;
				++j;
			}

			return *this;
		}
		
		/*!
			\brief Multiply a vector by a scalar value.
			\param aScalar The scalar to multiply this vector by.
			\return A reference to this vector.
		*/
		VectorDynamic<value_t>& operator*=(const value_t aScalar){
			value_t* i = begin();
			const value_t* const end = end();

			while(i != end){
				*i *= aScalar;
				++i;
			}

			return *this;
		}
		
		/*!
			\brief Multiply two vectors.
			\param aOther The vector to multiply this vector by.
			\return The result of the operation.
		*/
		VectorDynamic<value_t> operator*(const VectorDynamic<value_t>& aOther) const{
			return VectorDynamic<value_t>(*this) *= aOther;
		}
		
		
		/*!
			\brief Multiply a vector by a scalar value.
			\param aScalar The scalar to multiply this vector by.
			\return The result of the operation.
		*/
		VectorDynamic<value_t> operator*(const value_t aScalar) const{
			return VectorDynamic<value_t>(*this) *= aScalar;
		}
		
		/*!
			\brief Divide two vectors.
			\param aOther The vector to divide this vector by.
			\return A reference to this vector.
		*/
		VectorDynamic<value_t>& operator/=(const VectorDynamic<value_t>& aOther){
			if(Length() != aOther.Length()) throw std::runtime_error("Incompatible vector lengths");

			value_t* i = begin();
			const value_t* j = aOther.begin();
			const value_t* const end = end();

			while(i != end){
				*i /= *j;
				++i;
				++j;
			}

			return *this;
		}
		
		/*!
			\brief Divide a vector by a scalar value.
			\param aScalar The scalar to divide this vector by.
			\return A reference to this vector.
		*/
		VectorDynamic<value_t>& operator/=(const value_t aScalar){
			value_t* i = begin();
			const value_t* const end = end();

			while(i != end){
				*i /= aScalar;
				++i;
			}

			return *this;
		}
		
		/*!
			\brief Divide two vectors.
			\param aOther The vector to divide this vector by.
			\return The result of the operation.
		*/
		VectorDynamic<value_t> operator/(const VectorDynamic<value_t>& aOther) const{
			return VectorDynamic<value_t>(*this) /= aOther;
		}
		
		/*!
			\brief Divide a vector by a scalar.
			\param aScalar The scalar to divide this vector by.
			\return The result of the operation.
		*/
		VectorDynamic<value_t> operator/(const value_t aScalar) const{
			return VectorDynamic<value_t>(*this) /= aScalar;
		}

		/*!
			\brief Access a vector component by index.
			\detail Will throw std::runtime_error if \a aIndex is out of bounds.
			\param aIndex The index to access.
			\return The value of the component.
		*/
		value_t operator[](const size_t aIndex) const{
			return mComponents[aIndex];
		}

		/*!
			\brief Access a vector component by index.
			\detail Will throw std::runtime_error if \a aIndex is out of bounds.
			\param aIndex The index to access.
			\return A reference to the value of the component.
		*/
		value_t& operator[](const size_t aIndex){
			return mComponents[aIndex];
		}
		
		/*!
			\brief Access a vector component by axis name.
			\detail Will throw std::runtime_error if \a aAxis is not a valid axis name.
			\param aAxis The index to access.
			\return The value of the component.
			\see AxisToIndex
		*/
		value_t operator[](const char aAxis) const{
			return operator[](Maths::AxisToIndex(aAxis));
		}
		
		/*!
			\brief Access a vector component by axis name.
			\detail Will throw std::runtime_error if \a aAxis is not a valid axis name.
			\param aAxis The index to access.
			\return A reference to the value of the component.
			\see AxisToIndex
		*/
		value_t& operator[](const char aAxis){
			return operator[](Maths::AxisToIndex(aAxis));
		}

		// Vector operators

		/*!
			\brief Return the number of components in this vector.
			\return The component count.
		*/
		size_t Length() const{
			return mComponents.size();
		}

		/*!
			\brief Add all of the components in this vector.
			\return The sum of components.
		*/
		value_t Sum() const{
			value_t sum = static_cast<value_t>(0);
			const value_t* i = begin();
			const value_t* const end = end();

			while(i != end){
				sum += *i;
				++i;
			}
		}

		/*!
			\brief Calculate the average of all components in this vector.
			\return The average component.
		*/
		value_t Average() const{
			return Sum() / static_cast<value_t>(Length());
		}
		
		/*!
			\brief Calculate the dot product of two vectors.
			\param aOther The vector to calculate the dot product with.
			\return The dot product.
		*/
		value_t DotProduct(const VectorDynamic<value_t>& aOther) const{
			return operator*(aOther).Sum();
		}
		
		/*!
			\brief Calculate the cross product of two vectors.
			\param aOther The vector to calculate the cross product with.
			\return The cross product.
		*/
		VectorDynamic<value_t> CrossProduct(const VectorDynamic<value_t>& aOther) const{
			//! \TODO Implement or remove N dimentional cross product
			throw std::runtime_error("VectorDynamic.CrossProduct not implemented");
		}
		
		/*!
			\brief Calculate the square of the magnitude of this vector.
			\detail This is faster than calculating the exact magnitude.
			\return The square of the magnitude.
			\see Magnitude
		*/
		value_t MagnitudeSquared() const{
			return operator*(*this).Sum();
		}
		
		/*!
			\brief Calculate the magnitude of this vector.
			\return The magnitude.
			\see MagnitudeSquared
		*/
		value_t Magnitude() const{
			return static_cast<value_t>(std::sqrt(static_cast<double>(MagnitudeSquared())));
		}

		/*!
			\brief Normalise this vector to a unit vector.
			\return The normalised form of this vector.
		*/
		VectorDynamic<value_t> Normalise() const{
			return operator/(Magnitude());
		}

		// Other

		/*!
			\brief Return an iterator to the first component in this vector.
			\return A pointer to the X component.
		*/
		value_t* begin(){
			return &mComponents[0];
		}

		/*!
			\brief Return an read-only iterator to the first component in this vector.
			\return A pointer to the X component.
		*/
		const value_t* begin() const{
			return &mComponents[0];
		}

		/*!
			\brief Return an iterator that indicates the end of components in this vector.
			\return A pointer past the W component.
		*/
		value_t* end(){
			return (&mComponents[mComponents.size() - 1]) + 1;
		}

		/*!
			\brief Return an read-only iterator that indicates the end of components in this vector.
			\return A pointer past the W component.
		*/
		const value_t* end() const{
			return (&mComponents[mComponents.size() - 1]) + 1;
		}
	};

	typedef VectorDynamic<double> VectorDynamicD;	//!< VectorDynamic storing values using doubles.
	typedef VectorDynamic<float> VectorDynamicF;	//!< VectorDynamic storing values using floats.
	typedef VectorDynamic<int32_t> VectorDynamicI;	//!< VectorDynamic storing values using 32 bit signed integers.
	typedef VectorDynamic<uint32_t> VectorDynamicU;	//!< VectorDynamic storing values using 32 bit unsigned integers.
	
	/*!
		\brief Cast a vector of one type to a vector of another type.
		\tparam A The type being cast into.
		\tparam B The type being cast from.
		\param aVector The vector to cast from.
		\return The casted vector.
	*/
	template<typename A, typename B>
	static VectorDynamic<A> vector_cast(const VectorDynamic<B>& aVector){
		VectorDynamic<A> tmp(aVector.Length());

		auto j = aVector.begin();
		const auto end = tmp.end();
		for(auto i = tmp.begin(); i != end; ++i){
			*i = static_cast<A>(*j);
			++j;
		}

		return tmp;
	}

}}


#endif