#ifndef SOLAIRE_MATHS_VECTOR_HPP
#define SOLAIRE_MATHS_VECTOR_HPP

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
	\file Vector.hpp
	\brief Contains code for vector maths.
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 23rd September 2015
	Last Modified	: 23rd September 2015
*/

#include <cmath>
#include <cstdint>
#include <stdexcept>
#include "VectorLogic.inl"

namespace Solaire{ namespace Maths{

	/*!
		\class Vector
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
	template<typename TYPE, const uint32_t LENGTH>
	class Vector{
    private:
        static_assert(LENGTH >= 2, "Vector must contain at least 2 elements");

        TYPE mElements[LENGTH];
        typedef VectorLogic<TYPE, LENGTH> Logic;
	public:
		typedef TYPE Element;	//!< The data type used by the vector.

		// Constructors

		/*!
			\brief Create a vector initialised to 0.
		*/
		Vector(){
		    Logic::Fill(mElements, static_cast<Element>(0));
		}

		/*!
			\brief Create a vector initialised to \a aScalar.
			\param aScalar The scalar value to fill the vector with
		*/
		Vector(const Element aScalar){
		    Logic::Fill(mElements, aScalar);
        }

		/*!
			\brief Create a vector with specific values.
			\detail Leftover elements will be initialised as 0.
			\param aElements The elements to add.
		*/
		template<class T2>
		Vector(const std::initializer_list<T2> aElements){
		    const size_t size = aElements.size();
		    const T2* const elements = &(*aElements.begin());
		    Logic::Cast(mElements, elements, size);
		}

		/*!
			\brief Convert a vector from one type to another type.
			\tparam T2 The element type of \a aOther.
			\tparam LENGTH2 The element count of \a aOther.
			\param aOther The other vector.
		*/
		template<class T2, const uint32_t LENGTH2>
		Vector(const Vector<T2, LENGTH2>& aOther){
		    Logic::Cast(mElements, aOther.mElements);
		}

		// Operator overloads

		/*!
			\brief Check if two vectors are equal.
			\param aOther The vector to compare this vector against.
			\return True if all vector components are equal.
		*/
		inline bool operator==(const Vector<Element, LENGTH>& aOther) const{
			return Logic::Equals(mElements, aOther.mElements);
		}

		/*!
			\brief Check if two vectors are not equal.
			\param aOther The vector to compare this vector against.
			\return True if at least one vector component is not equal.
		*/
		inline bool operator!=(const Vector<Element, LENGTH>& aOther) const{
			return Logic::NotEquals(mElements, aOther.mElements);
		}

		/*!
			\brief Add two vectors.
			\param aOther The vector to add to this vector.
			\return A reference to this vector.
		*/
		inline Vector<Element, LENGTH>& operator+=(const Vector<Element, LENGTH>& aOther){
			return *reinterpret_cast<Vector<Element, LENGTH>*>(Logic::AddEq(mElements, aOther.mElements));
		}

		/*!
			\brief Add a vector and a scalar value.
			\param aScalar The scalar to add to this vector.
			\return A reference to this vector.
		*/
		inline Vector<Element, LENGTH>& operator+=(const Element aScalar){
			return *reinterpret_cast<Vector<Element, LENGTH>*>(Logic::AddEq(mElements, aScalar));
		}

		/*!
			\brief Add two vectors.
			\param aOther The vector to add to this vector.
			\return The result of the operation.
		*/
		inline Vector<Element, LENGTH> operator+(const Vector<Element, LENGTH>& aOther) const{
		    Vector<Element, LENGTH> tmp(*this);
			return tmp += aOther;
		}

		/*!
			\brief Add a vector and a scalar value.
			\param aScalar The scalar to add to this vector.
			\return The result of the operation.
		*/
		inline Vector<Element, LENGTH> operator+(const Element aScalar) const{
			Vector<Element, LENGTH> tmp(*this);
			return tmp += aScalar;
		}

		/*!
			\brief Subtract two vectors.
			\param aOther The vector to subtract from this vector.
			\return A reference to this vector.
		*/
		inline Vector<Element, LENGTH>& operator-=(const Vector<Element, LENGTH>& aOther){
			return *reinterpret_cast<Vector<Element, LENGTH>*>(Logic::SubEq(mElements, aOther.mElements));
		}

		/*!
			\brief Subtract a scalar value from a vector.
			\param aScalar The scalar to subtract from this vector.
			\return A reference to this vector.
		*/
		inline Vector<Element, LENGTH>& operator-=(const Element aScalar){
			return *reinterpret_cast<Vector<Element, LENGTH>*>(Logic::SubEq(mElements, aScalar));
		}

		/*!
			\brief Subtract two vectors.
			\param aOther The vector to subtract from this vector.
			\return The result of the operation.
		*/
		inline Vector<Element, LENGTH> operator-(const Vector<Element, LENGTH>& aOther) const{
			Vector<Element, LENGTH> tmp(*this);
			return tmp -= aOther;
		}

		/*!
			\brief Subtract a scalar from a vector.
			\param aScalar The scalar to subtract from this vector.
			\return A reference to this vector.
		*/
		inline Vector<Element, LENGTH> operator-(const Element aScalar) const{
			Vector<Element, LENGTH> tmp(*this);
			return tmp -= aScalar;
		}

		/*!
			\brief Multiply two vectors.
			\param aOther The vector to multiply this vector by.
			\return A reference to this vector.
		*/
		inline Vector<Element, LENGTH>& operator*=(const Vector<Element, LENGTH>& aOther){
			return *reinterpret_cast<Vector<Element, LENGTH>*>(Logic::MulEq(mElements, aOther.mElements));
		}

		/*!
			\brief Multiply a vector by a scalar value.
			\param aScalar The scalar to multiply this vector by.
			\return A reference to this vector.
		*/
		inline Vector<Element, LENGTH>& operator*=(const Element aScalar){
			return *reinterpret_cast<Vector<Element, LENGTH>*>(Logic::MulEq(mElements, aScalar));
		}

		/*!
			\brief Multiply two vectors.
			\param aOther The vector to multiply this vector by.
			\return The result of the operation.
		*/
		inline Vector<Element, LENGTH> operator*(const Vector<Element, LENGTH>& aOther) const{
			Vector<Element, LENGTH> tmp(*this);
			return tmp *= aOther;
		}


		/*!
			\brief Multiply a vector by a scalar value.
			\param aScalar The scalar to multiply this vector by.
			\return The result of the operation.
		*/
		inline Vector<Element, LENGTH> operator*(const Element aScalar) const{
			Vector<Element, LENGTH> tmp(*this);
			return tmp *= aScalar;
		}

		/*!
			\brief Divide two vectors.
			\param aOther The vector to divide this vector by.
			\return A reference to this vector.
		*/
		inline Vector<Element, LENGTH>& operator/=(const Vector<Element, LENGTH>& aOther){
			return *reinterpret_cast<Vector<Element, LENGTH>*>(Logic::DivEq(mElements, aOther.mElements));
		}

		/*!
			\brief Divide a vector by a scalar value.
			\param aScalar The scalar to divide this vector by.
			\return A reference to this vector.
		*/
		inline Vector<Element, LENGTH>& operator/=(const Element aScalar){
			return *reinterpret_cast<Vector<Element, LENGTH>*>(Logic::DivEq(mElements, aScalar));
		}

		/*!
			\brief Divide two vectors.
			\param aOther The vector to divide this vector by.
			\return The result of the operation.
		*/
		inline Vector<Element, LENGTH> operator/(const Vector<Element, LENGTH>& aOther) const{
			Vector<Element, LENGTH> tmp(*this);
			return tmp /= aOther;
		}

		/*!
			\brief Divide a vector by a scalar.
			\param aScalar The scalar to divide this vector by.
			\return The result of the operation.
		*/
		inline Vector<Element, LENGTH> operator/(const Element aScalar) const{
			Vector<Element, LENGTH> tmp(*this);
			return tmp /= aScalar;
		}

		/*!
			\brief Linear interpolation between two vectors.
			\param aOther The vector to interpolation.
			\param aWeight The ratio of this vector to \a aOther.
			\return The result of the interpolation.
		*/
		inline Vector<Element, LENGTH> Lerp(const Vector<Element, LENGTH>& aOther, const double aWeight) const{
			Vector<Element, LENGTH> tmp(*this);
			Logic::LerpEq(tmp.mElements, aOther.mElements, aWeight);
			return tmp;
		}

		/*!
			\brief Access a vector component by index.
			\detail Will throw std::runtime_error if \a aIndex is out of bounds.
			\param aIndex The index to access.
			\return The value of the component.
		*/
		inline Element operator[](const size_t aIndex) const{
			if(aIndex >= 4) throw std::runtime_error("Vector index is out of bounds");
			return (mElements)[aIndex];
		}

		/*!
			\brief Access a vector component by index.
			\detail Will throw std::runtime_error if \a aIndex is out of bounds.
			\param aIndex The index to access.
			\return A reference to the value of the component.
		*/
		inline Element& operator[](const size_t aIndex){
			if(aIndex >= 4) throw std::runtime_error("Vector index is out of bounds");
			return (mElements)[aIndex];
		}

		// Vector operators

		/*!
			\brief Return the number of components in this vector.
			\return The component count.
		*/
		inline size_t Length() const{
			return LENGTH;
		}

		/*!
			\brief Add all of the components in this vector.
			\return The sum of components.
		*/
		inline Element Sum() const{
			return Logic::Sum(mElements);
		}

		/*!
			\brief Calculate the average of all components in this vector.
			\return The average component.
		*/
		inline Element Average() const{
			return Logic::Avg(mElements);
		}

		/*!
			\brief Calculate the dot product of two vectors.
			\param aOther The vector to calculate the dot product with.
			\return The dot product.
		*/
		inline Element DotProduct(const Vector<Element, LENGTH>& aOther) const{
			return Logic::Dot(mElements, aOther.mElements);
		}

		/*!
			\brief Calculate the cross product of two vectors.
			\param aOther The vector to calculate the cross product with.
			\return The cross product.
		*/
		template<class VectorType = Vector<Element, LENGTH>>
        inline typename std::enable_if<LENGTH == 3, VectorType>::type CrossProduct(const Vector<Element, LENGTH>& aOther) const{
			Vector<Element, LENGTH> tmp;
			return Logic::Cross(tmp.mElements, mElements, aOther.mElements);
			return tmp;
		}

		/*!
			\brief Calculate the square of the magnitude of this vector.
			\detail This is faster than calculating the exact magnitude.
			\return The square of the magnitude.
			\see Magnitude
		*/
		inline Element MagnitudeSquared() const{
			return Logic::MagSq(mElements);
		}

		/*!
			\brief Calculate the magnitude of this vector.
			\return The magnitude.
			\see MagnitudeSquared
		*/
		inline Element Magnitude() const{
			return Logic::Mag(mElements);
		}

		/*!
			\brief Normalise this vector to a unit vector.
			\return The normalised form of this vector.
		*/
		inline Vector<Element, LENGTH> Normalise() const{
			Vector<Element, LENGTH> tmp(*this);
			Logic::NormaliseEq(tmp.mElements);
			return tmp;
		}

		// Other

		inline Element& X(){
		    return mElements[0];
		}

		inline const Element& X() const {
		    return mElements[0];
		}

		inline Element& Y(){
		    return mElements[1];
		}

		inline const Element& Y() const {
		    return mElements[1];
		}

        SOLAIRE_ENABLE_IF(inline, LENGTH >= 3, Element&) Z(){
		    return mElements[2];
		}

        SOLAIRE_ENABLE_IF(inline, LENGTH >= 3, const Element&) Z() const {
		    return mElements[2];
		}

        SOLAIRE_ENABLE_IF(inline, LENGTH >= 4, Element&) W(){
		    return mElements[3];
		}

        SOLAIRE_ENABLE_IF(inline, LENGTH >= 4, const Element&) W() const {
		    return mElements[3];
		}

        /*!
            \brief Check if this vector has been normalised.
            \return True if this vector is a unit vector.
        */
		inline bool IsUnitVector() const{
		    return Sum() == static_cast<Element>(1);
		}

		/*!
			\brief Re-arrange values into a Vector.
			\detail
			\code
			// Create the source data
			VectorU swizzle(2, 1, 0, 3);
			VectorF source(1.f, 2.f, 3.f, 4.f);

			// Swizzle the data
			VectorF swizzled = source.Swizzle(swizzle);

			// swizzled will now be the vector [3,2,1,4]
			\endcode
			\param aSwizzle The indices to order the values by (must be at least 4 chars).
			\return The resulting vector.
		*/
		inline Vector<Element, LENGTH> Swizzle(const Vector<uint32_t, LENGTH>& aSwizzle){
			Vector<Element, LENGTH> tmp;
			Logic::Swizzle(tmp.mElements, mElements, &aSwizzle.X);
			return tmp;
		}

		/*!
			\brief Return an iterator to the first component in this vector.
			\return A pointer to the X component.
		*/
		inline Element* begin(){
			return mElements;
		}

		/*!
			\brief Return an read-only iterator to the first component in this vector.
			\return A pointer to the X component.
		*/
		inline const Element* begin() const{
			return mElements;
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

	typedef Vector<double, 2> Vector2D;
	typedef Vector<double, 3> Vector3D;
	typedef Vector<double, 4> Vector4D;

	typedef Vector<float, 2> Vector2F;
	typedef Vector<float, 3> Vector3F;
	typedef Vector<float, 4> Vector4F;

	typedef Vector<int32_t, 2> Vector2I;
	typedef Vector<int32_t, 3> Vector3I;
	typedef Vector<int32_t, 4> Vector4I;

	typedef Vector<uint32_t, 2> Vector2U;
	typedef Vector<uint32_t, 3> Vector3U;
	typedef Vector<uint32_t, 4> Vector4U;

	typedef Vector<int8_t, 2> Vector2IB;
	typedef Vector<int8_t, 3> Vector3IB;
	typedef Vector<int8_t, 4> Vector4IB;

	typedef Vector<uint8_t, 2> Vector2UB;
	typedef Vector<uint8_t, 3> Vector3UB;
	typedef Vector<uint8_t, 4> Vector4UB;

}}


#endif
