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
//#include "VectorUtility.hpp"
#include "VectorLogic.inl"

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
		Last Modified	: 22nd September 2015
	*/
	template<typename TYPE>
	class Vector4{
    private:
        typedef VectorLogic<TYPE, 4> Logic;
	public:
		typedef TYPE Element;	//!< The data type used by the vector.

		Element X;	//!< The X axis component of this vector.
		Element Y;	//!< The Y axis component of this vector.
		Element Z;	//!< The Z axis component of this vector.
		Element W;	//!< The W axis component of this vector.

		// Constructors

		/*!
			\brief Create a vector initialised to 0.
		*/
		constexpr Vector4() :
			X(static_cast<Element>(0)),
			Y(static_cast<Element>(0)),
			Z(static_cast<Element>(0)),
			W(static_cast<Element>(0))
		{}

		/*!
			\brief Create a vector initialised to \a aScalar.
			\param aScalar The scalar value to fill the vector with
		*/
		constexpr Vector4(const Element aScalar) :
			X(aScalar),
			Y(aScalar),
			Z(aScalar),
			W(aScalar)
		{}

		/*!
			\brief Create a vector with specific values.
			\param aX The X axis component.
			\param aY The Y axis component.
			\param aZ The Z axis component.
			\param aW The W axis component.
		*/
		constexpr Vector4(const Element aX, const Element aY, const Element aZ, const Element aW) :
			X(aX),
			Y(aY),
			Z(aZ),
			W(aW)
		{}

		// Operator overloads

		/*!
			\brief Check if two vectors are equal.
			\param aOther The vector to compare this vector against.
			\return True if all vector components are equal.
		*/
		inline bool operator==(const Vector4<Element> aOther) const{
			return Logic::Equals(&X, &aOther.X);
		}

		/*!
			\brief Check if two vectors are not equal.
			\param aOther The vector to compare this vector against.
			\return True if at least one vector component is not equal.
		*/
		inline bool operator!=(const Vector4<Element> aOther) const{
			return Logic::NotEquals(&X, &aOther.X);
		}

		/*!
			\brief Add two vectors.
			\param aOther The vector to add to this vector.
			\return A reference to this vector.
		*/
		inline Vector4<Element>& operator+=(const Vector4<Element> aOther){
			return *reinterpret_cast<Vector4<Element>*>(Logic::AddEq(&X, &aOther.X));
		}

		/*!
			\brief Add a vector and a scalar value.
			\param aScalar The scalar to add to this vector.
			\return A reference to this vector.
		*/
		inline Vector4<Element>& operator+=(const Element aScalar){
			return *reinterpret_cast<Vector4<Element>*>(Logic::AddEq(&X, aScalar));
		}

		/*!
			\brief Add two vectors.
			\param aOther The vector to add to this vector.
			\return The result of the operation.
		*/
		inline Vector4<Element> operator+(const Vector4<Element> aOther) const{
		    Vector4<Element> tmp(*this);
			return tmp += aOther;
		}

		/*!
			\brief Add a vector and a scalar value.
			\param aScalar The scalar to add to this vector.
			\return The result of the operation.
		*/
		inline Vector4<Element> operator+(const Element aScalar) const{
			Vector4<Element> tmp(*this);
			return tmp += aScalar;
		}

		/*!
			\brief Subtract two vectors.
			\param aOther The vector to subtract from this vector.
			\return A reference to this vector.
		*/
		inline Vector4<Element>& operator-=(const Vector4<Element> aOther){
			return *reinterpret_cast<Vector4<Element>*>(Logic::SubEq(&X, &aOther.X));
		}

		/*!
			\brief Subtract a scalar value from a vector.
			\param aScalar The scalar to subtract from this vector.
			\return A reference to this vector.
		*/
		inline Vector4<Element>& operator-=(const Element aScalar){
			return *reinterpret_cast<Vector4<Element>*>(Logic::SubEq(&X, aScalar));
		}

		/*!
			\brief Subtract two vectors.
			\param aOther The vector to subtract from this vector.
			\return The result of the operation.
		*/
		inline Vector4<Element> operator-(const Vector4<Element> aOther) const{
			Vector4<Element> tmp(*this);
			return tmp -= aOther;
		}

		/*!
			\brief Subtract a scalar from a vector.
			\param aScalar The scalar to subtract from this vector.
			\return A reference to this vector.
		*/
		inline Vector4<Element> operator-(const Element aScalar) const{
			Vector4<Element> tmp(*this);
			return tmp -= aScalar;
		}

		/*!
			\brief Multiply two vectors.
			\param aOther The vector to multiply this vector by.
			\return A reference to this vector.
		*/
		inline Vector4<Element>& operator*=(const Vector4<Element> aOther){
			return *reinterpret_cast<Vector4<Element>*>(Logic::MulEq(&X, &aOther.X));
		}

		/*!
			\brief Multiply a vector by a scalar value.
			\param aScalar The scalar to multiply this vector by.
			\return A reference to this vector.
		*/
		inline Vector4<Element>& operator*=(const Element aScalar){
			return *reinterpret_cast<Vector4<Element>*>(Logic::MulEq(&X, aScalar));
		}

		/*!
			\brief Multiply two vectors.
			\param aOther The vector to multiply this vector by.
			\return The result of the operation.
		*/
		inline Vector4<Element> operator*(const Vector4<Element> aOther) const{
			Vector4<Element> tmp(*this);
			return tmp *= aOther;
		}


		/*!
			\brief Multiply a vector by a scalar value.
			\param aScalar The scalar to multiply this vector by.
			\return The result of the operation.
		*/
		inline Vector4<Element> operator*(const Element aScalar) const{
			Vector4<Element> tmp(*this);
			return tmp *= aScalar;
		}

		/*!
			\brief Divide two vectors.
			\param aOther The vector to divide this vector by.
			\return A reference to this vector.
		*/
		inline Vector4<Element>& operator/=(const Vector4<Element> aOther){
			return *reinterpret_cast<Vector4<Element>*>(Logic::DivEq(&X, &aOther.X));
		}

		/*!
			\brief Divide a vector by a scalar value.
			\param aScalar The scalar to divide this vector by.
			\return A reference to this vector.
		*/
		inline Vector4<Element>& operator/=(const Element aScalar){
			return *reinterpret_cast<Vector4<Element>*>(Logic::DivEq(&X, aScalar));
		}

		/*!
			\brief Divide two vectors.
			\param aOther The vector to divide this vector by.
			\return The result of the operation.
		*/
		inline Vector4<Element> operator/(const Vector4<Element> aOther) const{
			Vector4<Element> tmp(*this);
			return tmp /= aOther;
		}

		/*!
			\brief Divide a vector by a scalar.
			\param aScalar The scalar to divide this vector by.
			\return The result of the operation.
		*/
		inline Vector4<Element> operator/(const Element aScalar) const{
			Vector4<Element> tmp(*this);
			return tmp /= aScalar;
		}

		/*!
			\brief Access a vector component by index.
			\detail Will throw std::runtime_error if \a aIndex is out of bounds.
			\param aIndex The index to access.
			\return The value of the component.
		*/
		inline Element operator[](const size_t aIndex) const{
			if(aIndex >= 4) throw std::runtime_error("Vector4 index is out of bounds");
			return (&X)[aIndex];
		}

		/*!
			\brief Access a vector component by index.
			\detail Will throw std::runtime_error if \a aIndex is out of bounds.
			\param aIndex The index to access.
			\return A reference to the value of the component.
		*/
		inline Element& operator[](const size_t aIndex){
			if(aIndex >= 4) throw std::runtime_error("Vector4 index is out of bounds");
			return (&X)[aIndex];
		}

		// Vector operators

		/*!
			\brief Return the number of components in this vector.
			\return The component count.
		*/
		inline size_t Length() const{
			return 4;
		}

		/*!
			\brief Add all of the components in this vector.
			\return The sum of components.
		*/
		inline Element Sum() const{
			return Logic::Sum(&X);
		}

		/*!
			\brief Calculate the average of all components in this vector.
			\return The average component.
		*/
		inline Element Average() const{
			return Logic::Avg(&X);
		}

		/*!
			\brief Calculate the dot product of two vectors.
			\param aOther The vector to calculate the dot product with.
			\return The dot product.
		*/
		inline Element DotProduct(const Vector4<Element> aOther) const{
			return Logic::Dot(&X, &aOther.X);
		}

		/*!
			\brief Calculate the cross product of two vectors.
			\param aOther The vector to calculate the cross product with.
			\return The cross product.
		*/
		inline Vector4<Element> CrossProduct(const Vector4<Element> aOther) const{
			Vector4<Element> tmp;
			return Logic::Cross(&tmp.X, &X, &aOther.X);
			return tmp;
		}

		/*!
			\brief Calculate the square of the magnitude of this vector.
			\detail This is faster than calculating the exact magnitude.
			\return The square of the magnitude.
			\see Magnitude
		*/
		inline Element MagnitudeSquared() const{
			return Logic::MagSq(&X);
		}

		/*!
			\brief Calculate the magnitude of this vector.
			\return The magnitude.
			\see MagnitudeSquared
		*/
		inline Element Magnitude() const{
			return Logic::Mag(&X);
		}

		/*!
			\brief Normalise this vector to a unit vector.
			\return The normalised form of this vector.
		*/
		inline Vector4<Element> Normalise() const{
			Vector4<Element> tmp(*this);
			Logic::NormaliseEq(&tmp.X);
			return tmp;
		}

		// Other

        /*!
            \brief Check if this vector has been normalised.
            \return True if this vector is a unit vector.
        */
		inline bool IsUnitVector() const{
		    return Sum() == static_cast<Element>(1);
		}

		/*!
			\brief Re-arrange values into a Vector4.
			\detail
			\code
			// Create the source data
			Vector4U swizzle(2, 1, 0, 3);
			Vector4F source(1.f, 2.f, 3.f, 4.f);

			// Swizzle the data
			Vector4F swizzled = source.Swizzle(swizzle);

			// swizzled will now be the vector [3,2,1,4]
			\endcode
			\param aSwizzle The indices to order the values by (must be at least 4 chars).
			\return The resulting vector.
		*/
		inline Vector4<Element> Swizzle(const Vector4<uint32_t> aSwizzle){
			Vector4<Element> tmp;
			Logic::Swizzle(&tmp.X, &X, &aSwizzle.X);
			return tmp;
		}

		/*!
			\brief Return an iterator to the first component in this vector.
			\return A pointer to the X component.
		*/
		inline Element* begin(){
			return &X;
		}

		/*!
			\brief Return an read-only iterator to the first component in this vector.
			\return A pointer to the X component.
		*/
		inline const Element* begin() const{
			return &X;
		}

		/*!
			\brief Return an iterator that indicates the end of components in this vector.
			\return A pointer past the W component.
		*/
		inline Element* end(){
			return begin() + Length();
		}

		/*!
			\brief Return an read-only iterator that indicates the end of components in this vector.
			\return A pointer past the W component.
		*/
		inline const Element* end() const{
			return begin() + Length();
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
	static Vector4<A> vector_cast(const Vector4<B> aVector){
		return Vector4<A>(
			static_cast<A>(aVector.X),
			static_cast<A>(aVector.Y),
			static_cast<A>(aVector.Z),
			static_cast<A>(aVector.W)
		);
	}

}}


#endif
