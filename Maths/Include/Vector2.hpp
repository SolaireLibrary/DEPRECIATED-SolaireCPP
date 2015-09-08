#ifndef SOLAIRE_MATHS_VECTOR2_HPP
#define SOLAIRE_MATHS_VECTOR2_HPP

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

/*!
	\file Vector2.hpp
	\brief Contains code for 2 dimentional vector maths.
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
#include "VectorUtility.hpp"

namespace Solaire{ namespace Maths{

	/*!
		\class Vector2
		\tparam TYPE The data type used by the vector.
		\brief A 2 dimentional vector implementation.
		\version 1.0
		\author
		Created			: Adam Smith
		Last modified	: Adam Smith
		\date 
		Created			: 8th September 2015
		Last Modified	: 8th September 2015
	*/
	template<typename TYPE>
	class Vector2{ 
	public:
		typedef TYPE value_t;	//!< The data type used by the vector.

		value_t X;	//!< The X axis component of this vector.
		value_t Y;	//!< The Y axis component of this vector.

		// Constructors

		/*!
			 \brief Create a vector initialised to 0.
		*/ 
		Vector2 () :
			X( static_cast<value_t>(0)),
			Y (static_cast<value_t>(0)),
			 Z(static_cast<value_t>(0))
		{ 
			 
		} 
		 
		/*! 
			\brief Create a vector with specific values.
			\param aX The X axis component.
			\param aY The Y axis component.
		*/
		Vector2(const value_t aX, const value_t aY) :
			X(aX),
			Y(aY)
		{  
			  
		} 
		 
		// Operator overloads
			 
		/*! 
			 \brief Check if two vectors are equal.
			\ param aOther The vector to compare this vector against.
			\return True if all vector components are equal.
		*/  
		bool operator==(const Vector2<value_t> aOther) const{
			return X == aOther.X && Y == aOther.Y;
		}
		
		/*!
			\brief Check if two vectors are not equal.
			\param aOther The vector to compare this vector against.
			\return True if at least one vector component is not equal.
		*/
		bool operator!=(const Vector2<value_t> aOther) const{
			return X != aOther.X || Y != aOther.Y;
		}

		/*!
			\brief Add two vectors.
			\param aOther The vector to add to this vector.
			\return A reference to this vector.
		*/
		 Vector2<value_t>& operator+=(const Vector2<value_t> aOther){
			X += aOther.X;
			Y += aOther.Y;
			return *this;
		}
		
		/*!
			\brief Add a vector and a scalar value.
			\param aScalar The scalar to add to this vector.
			\return A reference to this vector.
		*/
		 Vector2<value_t>& operator+=(const value_t aScalar){
			X += aScalar;
			Y += aScalar;
			return *this;
		}
		
		/*!
			\brief Add two vectors.
			\param aOther The vector to add to this vector.
			\return The result of the operation.
		*/
		 Vector2<value_t> operator+(const Vector2<value_t> aOther) const{
			return Vector2<value_t>(*this) += aOther;
		}
		
		/*!
			\brief Add a vector and a scalar value.
			\param aScalar The scalar to add to this vector.
			\return The result of the operation.
		*/
		 Vector2<value_t> operator+(const value_t aScalar) const{
			return Vector2<value_t>(*this) += aScalar;
		}
		
		/*!
			\brief Subtract two vectors.
			\param aOther The vector to subtract from this vector.
			\return A reference to this vector.
		*/
		 Vector2<value_t>& operator-=(const Vector2<value_t> aOther){
			X -= aOther.X;
			Y -= aOther.Y;
			return *this;
		}
		
		/*!
			\brief Subtract a scalar value from a vector.
			\param aScalar The scalar to subtract from this vector.
			\return A reference to this vector.
		*/
		 Vector2<value_t>& operator-=(const value_t aScalar){
			X -= aScalar;
			Y -= aScalar;
			return *this;
		}
		
		/*!
			\brief Subtract two vectors.
			\param aOther The vector to subtract from this vector.
			\return The result of the operation.
		*/
		 Vector2<value_t> operator-(const Vector2<value_t> aOther) const{
			return Vector2<value_t>(*this) -= aOther;
		}
		
		/*!
			\brief Subtract a scalar from a vector.
			\param aScalar The scalar to subtract from this vector.
			\return A reference to this vector.
		*/
		 Vector2<value_t> operator-(const value_t aScalar) const{
			return Vector2<value_t>(*this) -= aScalar;
		}
		
		/*!
			\brief Multiply two vectors.
			\param aOther The vector to multiply this vector by.
			\return A reference to this vector.
		*/
		 Vector2<value_t>& operator*=(const Vector2<value_t> aOther){
			X *= aOther.X;
			Y *= aOther.Y;
			return *this;
		}
		
		/*!
			\brief Multiply a vector by a scalar value.
			\param aScalar The scalar to multiply this vector by.
			\return A reference to this vector.
		*/
		 Vector2<value_t>& operator*=(const value_t aScalar){
			X *= aScalar;
			Y *= aScalar;
			return *this;
		}
		
		/*!
			\brief Multiply two vectors.
			\param aOther The vector to multiply this vector by.
			\return The result of the operation.
		*/
		 Vector2<value_t> operator*(const Vector2<value_t> aOther) const{
			return Vector2<value_t>(*this) *= aOther;
		}
		
		
		/*!
			\brief Multiply a vector by a scalar value.
			\param aScalar The scalar to multiply this vector by.
			\return The result of the operation.
		*/
		 Vector2<value_t> operator*(const value_t aScalar) const{
			return Vector2<value_t>(*this) *= aScalar;
		}
		
		/*!
			\brief Divide two vectors.
			\param aOther The vector to divide this vector by.
			\return A reference to this vector.
		*/
		 Vector2<value_t>& operator/=(const Vector2<value_t> aOther){
			X /= aOther.X;
			Y /= aOther.Y;
			return *this;
		}
		
		/*!
			\brief Divide a vector by a scalar value.
			\param aScalar The scalar to divide this vector by.
			\return A reference to this vector.
		*/
		 Vector2<value_t>& operator/=(const value_t aScalar){
			X /= aScalar;
			Y /= aScalar;
			return *this;
		}
		
		/*!
			\brief Divide two vectors.
			\param aOther The vector to divide this vector by.
			\return The result of the operation.
		*/
		 Vector2<value_t> operator/(const Vector2<value_t> aOther) const{
			return Vector2<value_t>(*this) /= aOther;
		}
		
		/*!
			\brief Divide a vector by a scalar.
			\param aScalar The scalar to divide this vector by.
			\return The result of the operation.
		*/
		 Vector2<value_t> operator/(const value_t aScalar) const{
			return Vector2<value_t>(*this) /= aScalar;
		}

		/*!
			\brief Access a vector component by index.
			\detail Will throw std::runtime_error if \a aIndex is out of bounds.
			\param aIndex The index to access.
			\return The value of the component.
		*/
		value_t operator[](const size_t aIndex) const{
			if(aIndex >= 2) throw std::runtime_error(" Vector2 index is out of bounds");
			return (&X)[aIndex];
		}

		/*!
			\brief Access a vector component by index.
			\detail Will throw std::runtime_error if \a aIndex is out of bounds.
			\param aIndex The index to access.
			\return A reference to the value of the component.
		*/
		value_t& operator[](const size_t aIndex){
			if(aIndex >= 2) throw std::runtime_error(" Vector2 index is out of bounds");
			return (&X)[aIndex];
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
			return 2;
		}

		/*!
			\brief Add all of the components in this vector.
			\return The sum of components.
		*/
		value_t Sum() const{
			return X + Y;
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
		value_t DotProduct(const Vector2<value_t> aOther) const{
			return operator*(aOther).Sum();
		}
		
		/*!
			\brief Calculate the cross product of two vectors.
			\param aOther The vector to calculate the cross product with.
			\return The cross product.
		*/
		 Vector2<value_t> CrossProduct(const Vector2<value_t> aOther) const{
			//! \TODO Implement or remove 2 dimentional cross product
			throw std::runtime_error("Vector2.CrossProduct not implemented");
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
		 Vector2<value_t> Normalise() const{
			return operator/(Magnitude());
		}

		// Other

		/*!
			\brief Re-arrange values into a Vector2.
			\detail
			\code
			// Create the source data
			 Vector4F source(1.f, 2.f, 3.f, 4.f);
			 Vector2F swizzled;

			// Swizzle the data
			swizzled = Vector2F::Swizzle(source.begin(), "YW");

			// swizzled will now be the vector [2,4]
			\endcode
			\param aVector A pointer to contiguous data values (length depends on the value of \a aSwizzle).
			\param aSwizzle The axis names to order the values by (must be at least 2 chars).
			\return The resulting vector.
		*/
		static Vector2<value_t> Swizzle(const value_t* const aVector, const char* const aSwizzle){
			return Vector2<value_t>(
				aVector[aSwizzle[0]],
				aVector[aSwizzle[1]]
			);
		}

		/*!
			\brief Return an iterator to the first component in this vector.
			\return A pointer to the X component.
		*/
		value_t* begin(){
			return &X;
		}

		/*!
			\brief Return an read-only iterator to the first component in this vector.
			\return A pointer to the X component.
		*/
		const value_t* begin() const{
			return &X;
		}

		/*!
			\brief Return an iterator that indicates the end of components in this vector.
			\return A pointer past the W component.
		*/
		value_t* end(){
			return (&Y) + 1;
		}

		/*!
			\brief Return an read-only iterator that indicates the end of components in this vector.
			\return A pointer past the W component.
		*/
		const value_t* end() const{
			return (&Y) + 1;
		}
	};

	typedef Vector2<double> Vector2D;	//!< Vector2 storing values using doubles.
	typedef Vector2<float> Vector2F;	//!< Vector2 storing values using floats.
	typedef Vector2<int32_t> Vector2I;	//!< Vector2 storing values using 32 bit signed integers.
	typedef Vector2<uint32_t> Vector2U;	//!< Vector2 storing values using 32 bit unsigned integers.
	
	/*!
		\brief Cast a vector of one type to a vector of another type.
		\tparam A The type being cast into.
		\tparam B The type being cast from.
		\param aVector The vector to cast from.
		\return The casted vector.
	*/
	template<typename A, typename B>
	static Vector2<A> vector_cast(const Vector2<B> aVector){
		return Vector2<A>(
			static_cast<A>(aVector.X),
			static_cast<A>(aVector.Y)
		);
	}

}}


#endif