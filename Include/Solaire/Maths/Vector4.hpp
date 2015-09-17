#ifndef SOLAIRE_MATHS_VECTOR4_HPP
#define SOLAIRE_MATHS_VECTOR4_HPP

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
	\file Vector4.hpp
	\brief Contains code for 4 dimentional vector maths.
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 8th September 2015
	Last Modified	: 8th September 2015
*/

#include <cmath>
#include <cstdint>
#include <stdexcept>
#include "VectorUtility.hpp"

namespace Solaire{ namespace Maths{

	/*!
		\class Vector4
		\tparam TYPE The data type used by the vector.
		\brief A 4 dimentional vector implementation.
		\version 1.0
		\author
		Created			: Adam Smith
		Last modified	: Adam Smith
		\date
		Created			: 8th September 2015
		Last Modified	: 8th September 2015
	*/
	template<typename TYPE>
	class Vector4{
	public:
		typedef TYPE value_t;	//!< The data type used by the vector.

		value_t X;	//!< The X axis component of this vector.
		value_t Y;	//!< The Y axis component of this vector.
		value_t Z;	//!< The Z axis component of this vector.
		value_t W;	//!< The W axis component of this vector.

		// Constructors

		/*!
			\brief Create a vector initialised to 0.
		*/
		constexpr Vector4() :
			X(static_cast<value_t>(0)),
			Y(static_cast<value_t>(0)),
			Z(static_cast<value_t>(0)),
			W(static_cast<value_t>(0))
		{

		}

		/*!
			\brief Create a vector with specific values.
			\param aX The X axis component.
			\param aY The Y axis component.
			\param aZ The Z axis component.
			\param aW The W axis component.
		*/
		constexpr Vector4(const value_t aX, const value_t aY, const value_t aZ, const value_t aW) :
			X(aX),
			Y(aY),
			Z(aZ),
			W(aW)
		{

		}

		// Operator overloads

		/*!
			\brief Check if two vectors are equal.
			\param aOther The vector to compare this vector against.
			\return True if all vector components are equal.
		*/
		constexpr bool operator==(const Vector4<value_t> aOther) const{
			return X == aOther.X && Y == aOther.Y && Z == aOther.Z && W == aOther.W;
		}

		/*!
			\brief Check if two vectors are not equal.
			\param aOther The vector to compare this vector against.
			\return True if at least one vector component is not equal.
		*/
		constexpr bool operator!=(const Vector4<value_t> aOther) const{
			return X != aOther.X || Y != aOther.Y || Z != aOther.Z || W != aOther.W;
		}

		/*!
			\brief Add two vectors.
			\param aOther The vector to add to this vector.
			\return A reference to this vector.
		*/
		Vector4<value_t>& operator+=(const Vector4<value_t> aOther){
			X += aOther.X;
			Y += aOther.Y;
			Z += aOther.Z;
			W += aOther.W;
			return *this;
		}

		/*!
			\brief Add a vector and a scalar value.
			\param aScalar The scalar to add to this vector.
			\return A reference to this vector.
		*/
		Vector4<value_t>& operator+=(const value_t aScalar){
			X += aScalar;
			Y += aScalar;
			Z += aScalar;
			W += aScalar;
			return *this;
		}

		/*!
			\brief Add two vectors.
			\param aOther The vector to add to this vector.
			\return The result of the operation.
		*/
		constexpr Vector4<value_t> operator+(const Vector4<value_t> aOther) const{
			return Vector4<value_t>(
                X + aOther.X,
                Y + aOther.Y,
                Z + aOther.Z,
                W + aOther.W
            );
		}

		/*!
			\brief Add a vector and a scalar value.
			\param aScalar The scalar to add to this vector.
			\return The result of the operation.
		*/
		constexpr Vector4<value_t> operator+(const value_t aScalar) const{
			return Vector4<value_t>(
                X + aScalar,
                Y + aScalar,
                Z + aScalar,
                W + aScalar
            );
		}

		/*!
			\brief Subtract two vectors.
			\param aOther The vector to subtract from this vector.
			\return A reference to this vector.
		*/
		Vector4<value_t>& operator-=(const Vector4<value_t> aOther){
			X -= aOther.X;
			Y -= aOther.Y;
			Z -= aOther.Z;
			W -= aOther.W;
			return *this;
		}

		/*!
			\brief Subtract a scalar value from a vector.
			\param aScalar The scalar to subtract from this vector.
			\return A reference to this vector.
		*/
		Vector4<value_t>& operator-=(const value_t aScalar){
			X -= aScalar;
			Y -= aScalar;
			Z -= aScalar;
			W -= aScalar;
			return *this;
		}

		/*!
			\brief Subtract two vectors.
			\param aOther The vector to subtract from this vector.
			\return The result of the operation.
		*/
		constexpr Vector4<value_t> operator-(const Vector4<value_t> aOther) const{
			return Vector4<value_t>(
                X - aOther.X,
                Y - aOther.Y,
                Z - aOther.Z,
                W - aOther.W
            );
		}

		/*!
			\brief Subtract a scalar from a vector.
			\param aScalar The scalar to subtract from this vector.
			\return A reference to this vector.
		*/
		constexpr Vector4<value_t> operator-(const value_t aScalar) const{
			return Vector4<value_t>(
                X - aScalar,
                Y - aScalar,
                Z - aScalar,
                W - aScalar
            );
		}

		/*!
			\brief Multiply two vectors.
			\param aOther The vector to multiply this vector by.
			\return A reference to this vector.
		*/
		Vector4<value_t>& operator*=(const Vector4<value_t> aOther){
			X *= aOther.X;
			Y *= aOther.Y;
			Z *= aOther.Z;
			W *= aOther.W;
			return *this;
		}

		/*!
			\brief Multiply a vector by a scalar value.
			\param aScalar The scalar to multiply this vector by.
			\return A reference to this vector.
		*/
		Vector4<value_t>& operator*=(const value_t aScalar){
			X *= aScalar;
			Y *= aScalar;
			Z *= aScalar;
			W *= aScalar;
			return *this;
		}

		/*!
			\brief Multiply two vectors.
			\param aOther The vector to multiply this vector by.
			\return The result of the operation.
		*/
		constexpr Vector4<value_t> operator*(const Vector4<value_t> aOther) const{
			return Vector4<value_t>(
                X * aOther.X,
                Y * aOther.Y,
                Z * aOther.Z,
                W * aOther.W
            );
		}


		/*!
			\brief Multiply a vector by a scalar value.
			\param aScalar The scalar to multiply this vector by.
			\return The result of the operation.
		*/
		constexpr Vector4<value_t> operator*(const value_t aScalar) const{
			return Vector4<value_t>(
                X * aScalar,
                Y * aScalar,
                Z * aScalar,
                W * aScalar
            );
		}

		/*!
			\brief Divide two vectors.
			\param aOther The vector to divide this vector by.
			\return A reference to this vector.
		*/
		Vector4<value_t>& operator/=(const Vector4<value_t> aOther){
			X /= aOther.X;
			Y /= aOther.Y;
			Z /= aOther.Z;
			W /= aOther.W;
			return *this;
		}

		/*!
			\brief Divide a vector by a scalar value.
			\param aScalar The scalar to divide this vector by.
			\return A reference to this vector.
		*/
		Vector4<value_t>& operator/=(const value_t aScalar){
			X /= aScalar;
			Y /= aScalar;
			Z /= aScalar;
			W /= aScalar;
			return *this;
		}

		/*!
			\brief Divide two vectors.
			\param aOther The vector to divide this vector by.
			\return The result of the operation.
		*/
		constexpr Vector4<value_t> operator/(const Vector4<value_t> aOther) const{
			return Vector4<value_t>(
                X / aOther.X,
                Y / aOther.Y,
                Z / aOther.Z,
                W / aOther.W
            );
		}

		/*!
			\brief Divide a vector by a scalar.
			\param aScalar The scalar to divide this vector by.
			\return The result of the operation.
		*/
		constexpr Vector4<value_t> operator/(const value_t aScalar) const{
			return Vector4<value_t>(
                X / aScalar,
                Y / aScalar,
                Z / aScalar,
                W / aScalar
            );
		}

		/*!
			\brief Access a vector component by index.
			\detail Will throw std::runtime_error if \a aIndex is out of bounds.
			\param aIndex The index to access.
			\return The value of the component.
		*/
		value_t operator[](const size_t aIndex) const{
			if(aIndex >= 4) throw std::runtime_error("Vector4 index is out of bounds");
			return (&X)[aIndex];
		}

		/*!
			\brief Access a vector component by index.
			\detail Will throw std::runtime_error if \a aIndex is out of bounds.
			\param aIndex The index to access.
			\return A reference to the value of the component.
		*/
		value_t& operator[](const size_t aIndex){
			if(aIndex >= 4) throw std::runtime_error("Vector4 index is out of bounds");
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
		constexpr size_t Length() const{
			return 4;
		}

		/*!
			\brief Add all of the components in this vector.
			\return The sum of components.
		*/
		constexpr value_t Sum() const{
			return X + Y + Z + W;
		}

		/*!
			\brief Calculate the average of all components in this vector.
			\return The average component.
		*/
		constexpr value_t Average() const{
			return Sum() / static_cast<value_t>(Length());
		}

		/*!
			\brief Calculate the dot product of two vectors.
			\param aOther The vector to calculate the dot product with.
			\return The dot product.
		*/
		constexpr value_t DotProduct(const Vector4<value_t> aOther) const{
			return operator*(aOther).Sum();
		}

		/*!
			\brief Calculate the cross product of two vectors.
			\param aOther The vector to calculate the cross product with.
			\return The cross product.
		*/
		Vector4<value_t> CrossProduct(const Vector4<value_t> aOther) const{
			//! \TODO Implement or remove 4 dimentional cross product
			throw std::runtime_error("Vector4.CrossProduct not implemented");
		}

		/*!
			\brief Calculate the square of the magnitude of this vector.
			\detail This is faster than calculating the exact magnitude.
			\return The square of the magnitude.
			\see Magnitude
		*/
		constexpr value_t MagnitudeSquared() const{
			return operator*(*this).Sum();
		}

		/*!
			\brief Calculate the magnitude of this vector.
			\return The magnitude.
			\see MagnitudeSquared
		*/
		constexpr value_t Magnitude() const{
			return static_cast<value_t>(std::sqrt(static_cast<double>(MagnitudeSquared())));
		}

		/*!
			\brief Normalise this vector to a unit vector.
			\return The normalised form of this vector.
		*/
		constexpr Vector4<value_t> Normalise() const{
			return operator/(Magnitude());
		}

		// Other

        /*!
            \brief Check if this vector has been normalised.
            \return True if this vector is a unit vector.
        */
		constexpr bool IsUnitVector() const{
		    return Sum() == static_cast<value_t>(1);
		}

		/*!
			\brief Re-arrange values into a Vector4.
			\detail
			\code
			// Create the source data
			Vector4F source(1.f, 2.f, 3.f, 4.f);
			Vector4F swizzled;

			// Swizzle the data
			swizzled = Vector4F::Swizzle(source.begin(), "ZYXW");

			// swizzled will now be the vector [3,2,1,4]
			\endcode
			\param aVector A pointer to contiguous data values (length depends on the value of \a aSwizzle).
			\param aSwizzle The axis names to order the values by (must be at least 4 chars).
			\return The resulting vector.
		*/
		static Vector4<value_t> Swizzle(const value_t* const aVector, const char* const aSwizzle){
			return Vector4<value_t>(
				aVector[aSwizzle[0]],
				aVector[aSwizzle[1]],
				aVector[aSwizzle[2]],
				aVector[aSwizzle[3]]
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
		constexpr const value_t* begin() const{
			return &X;
		}

		/*!
			\brief Return an iterator that indicates the end of components in this vector.
			\return A pointer past the W component.
		*/
		value_t* end(){
			return (&W) + 1;
		}

		/*!
			\brief Return an read-only iterator that indicates the end of components in this vector.
			\return A pointer past the W component.
		*/
		constexpr const value_t* end() const{
			return (&W) + 1;
		}
	};

	typedef Vector4<double> Vector4D;	//!< Vector4 storing values using doubles.
	typedef Vector4<float> Vector4F;	//!< Vector4 storing values using floats.
	typedef Vector4<int32_t> Vector4I;	//!< Vector4 storing values using 32 bit signed integers.
	typedef Vector4<uint32_t> Vector4U;	//!< Vector4 storing values using 32 bit unsigned integers.

	/*!
		\brief Cast a vector of one type to a vector of another type.
		\tparam A The type being cast into.
		\tparam B The type being cast from.
		\param aVector The vector to cast from.
		\return The casted vector.
	*/
	template<typename A, typename B>
	static constexpr Vector4<A> vector_cast(const Vector4<B> aVector){
		return Vector4<A>(
			static_cast<A>(aVector.X),
			static_cast<A>(aVector.Y),
			static_cast<A>(aVector.Z),
			static_cast<A>(aVector.W)
		);
	}

}}


#endif
