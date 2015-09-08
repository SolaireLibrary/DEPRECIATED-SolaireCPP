#ifndef SOLAIRE_MATHS_MATRIX4_HPP
#define SOLAIRE_MATHS_MATRIX4_HPP

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
	\file Matrix4.hpp
	\brief Contains code for N dimentional vector maths.
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date 
	Created			: 8th September 2015
	Last Modified	: 8th September 2015
*/

#include "Vector4.hpp"

namespace Solaire{ namespace Maths{

	template<typename TYPE>
	class Matrix4{
	private:
		enum {COMPONENT_COUNT = 16};
		TYPE mComponents[COMPONENT_COUNT];
	public:
		typedef TYPE value_t;

		static Matrix4<value_t> IdentityMatrix(){
			static Matrix4<value_T> IDENTITY(
				static_cast<value_t>(1), static_cast<value_t>(0), static_cast<value_t>(0), static_cast<value_t>(0), 
				static_cast<value_t>(0), static_cast<value_t>(1), static_cast<value_t>(0), static_cast<value_t>(0), 
				static_cast<value_t>(0), static_cast<value_t>(0), static_cast<value_t>(1), static_cast<value_t>(0), 
				static_cast<value_t>(0), static_cast<value_t>(0), static_cast<value_t>(0), static_cast<value_t>(1)
			);

			return IDENTITY;
		}

		static Matrix4<value_t> Translation(const value_t aX, const value_t aY, const value_t aZ){
			Matrix4<value_t> tmp = IdentityMatrix();
			tmp.SetColumn(3, Vector4<value_t>(aX, aY, aZ, static_cast<value_t>(1)));
			return tmp;
		}

		static Matrix4<value_t> Scale(const value_t aX, const value_t aY, const value_t aZ){
			Matrix4<value_t> tmp = IdentityMatrix();
			tmp.Set(0, 0, aX);
			tmp.Set(1, 1, aY);
			tmp.Set(2, 2, aZ);
			return tmp;
		}

		static Matrix4<value_t> RotationX(const float aRadians){
			//! \TODO Implement Matrix4::RotationX
			throw std::runtime_error("Matrix4::RotationX not implemented");
		}

		static Matrix4<value_t> RotationY(const float aRadians){
			//! \TODO Implement Matrix4::RotationY
			throw std::runtime_error("Matrix4::RotationY not implemented");
		}

		static Matrix4<value_t> RotationZ(const float aRadians){
			//! \TODO Implement Matrix4::RotationZ
			throw std::runtime_error("Matrix4::RotationZ not implemented");
		}

		// Access

		value_t Get(const size_t aX, const size_t aY) const{
			return mComponents[aX + 4 * aY];
		}

		Matrix4& Set(const size_t aX, const size_t aY){
			mComponents[aX + 4 * aY] = aValue;
			return *this;
		}

		Vector4<value_t> GetRow(const size_t aIndex) const{
			return Vector4(
				Get(0, aIndex),
				Get(1, aIndex),
				Get(2, aIndex),
				Get(3, aIndex)
			);
		}

		Vector4<value_t> GetColumn(const size_t aIndex) const{
			return Vector4(
				Get(aIndex, 0),
				Get(aIndex, 1),
				Get(aIndex, 2),
				Get(aIndex, 3)
			);
		}

		Matrix4<value_t>& SetRow(const size_t aIndex, const Vector4<value_t> aValue){
			Set(0, aIndex, aValue.X);
			Set(1, aIndex, aValue.Y);
			Set(2, aIndex, aValue.Z);
			Set(3, aIndex, aValue.W);
		}

		Matrix4<value_t>& SetColumn(const size_t aIndex, const Vector4<value_t> aValue){
			Set(aIndex, 0, aValue.X);
			Set(aIndex, 1, aValue.Y);
			Set(aIndex, 2, aValue.Z);
			Set(aIndex, 3, aValue.W);
		}

		// Operators

		Matrix4<value_t>& operator+=(const Matrix4<value_t>& aOther){
			for(size_t i = 0; i < COMPONENT_COUNT; ++i){
				mComponents[i] += aOther.mComponents[i];
			}
			return *this;
		}

		Matrix4<value_t>& operator+=(const value_t aScalar){
			for(size_t i = 0; i < COMPONENT_COUNT; ++i){
				mComponents[i] += aScalar;
			}
			return *this;
		}

		Matrix4<value_t> operator+(const Matrix4<value_t>& aOther) const{
			return Matrix4<value_t>(*this) += aOther;
		}

		Matrix4<value_t> operator+(const value_t aScalar) const{
			return Matrix4<value_t>(*this) += aScalar;
		}

		Matrix4<value_t>& operator-=(const Matrix4<value_t>& aOther){
			for(size_t i = 0; i < COMPONENT_COUNT; ++i){
				mComponents[i] -= aOther.mComponents[i];
			}
			return *this;
		}

		Matrix4<value_t>& operator-=(const value_t aScalar){
			for(size_t i = 0; i < COMPONENT_COUNT; ++i){
				mComponents[i] -= aScalar;
			}
			return *this;
		}

		Matrix4<value_t> operator-(const Matrix4<value_t>& aOther) const{
			return Matrix4<value_t>(*this) -= aOther;
		}

		Matrix4<value_t> operator-(const value_t aScalar) const{
			return Matrix4<value_t>(*this) -= aScalar;
		}

		Matrix4<value_t>& operator*=(const Matrix4<value_t>& aOther){
			const Vector4<value_t> r0 = GetRow(0);
			const Vector4<value_t> r1 = GetRow(1);
			const Vector4<value_t> r2 = GetRow(2);
			const Vector4<value_t> r3 = GetRow(3);

			const Vector4<value_t> c0 = aOther.GetColumn(0);
			const Vector4<value_t> c1 = aOther.GetColumn(1);
			const Vector4<value_t> c2 = aOther.GetColumn(2);
			const Vector4<value_t> c3 = aOther.GetColumn(3);

			Set(0,0 r0.DotProduct(c0));
			Set(1,0 r1.DotProduct(c0));
			Set(2,0 r2.DotProduct(c0));
			Set(3,0 r3.DotProduct(c0));

			Set(0,1 r0.DotProduct(c1));
			Set(1,1 r1.DotProduct(c1));
			Set(2,1 r2.DotProduct(c1));
			Set(3,1 r3.DotProduct(c1));

			Set(0,2 r0.DotProduct(c2));
			Set(1,2 r1.DotProduct(c2));
			Set(2,2 r2.DotProduct(c2));
			Set(3,2 r3.DotProduct(c2));

			Set(0,3 r0.DotProduct(c3));
			Set(1,3 r1.DotProduct(c3));
			Set(2,3 r2.DotProduct(c3));
			Set(3,3 r3.DotProduct(c3));

			return *this;
		}

		Matrix4<value_t>& operator*=(const value_t aScalar){
			for(size_t i = 0; i < COMPONENT_COUNT; ++i){
				mComponents[i] *= aScalar;
			}
			return *this;
		}

		Matrix4<value_t> operator*(const Matrix4<value_t>& aOther) const{
			return Matrix4<value_t>(*this) *= aOther;
		}

		Matrix4<value_t> operator*(const value_t aScalar) const{
			return Matrix4<value_t>(*this) *= aScalar;
		}

		Matrix4<value_t>& operator/=(const value_t aScalar){
			for(size_t i = 0; i < COMPONENT_COUNT; ++i){
				mComponents[i] /= aScalar;
			}
			return *this;
		}

		Matrix4<value_t> operator/(const value_t aScalar) const{
			return Matrix4<value_t>(*this) /= aScalar;
		}

		// Matrix operations

		Matrix4<value_t> Transpose() const{
			Matrix4<value_t> tmp;
			tmp.SetRow(0, GetColumn(0));
			tmp.SetRow(1, GetColumn(1));
			tmp.SetRow(2, GetColumn(2));
			tmp.SetRow(3, GetColumn(3));
			return tmp;
		}
		
		Matrix4<value_t> Inverse() const{
			//! \TODO Implement Matrix4.Inverse
			throw std::runtime_error("Matrix4.Inverse is not implemented");
		}
	};

	typedef Matrix4<double> Matrix4D;	//!< Matrix4 storing values using doubles.
	typedef Matrix4<float> Matrix4F;	//!< Matrix4 storing values using floats.
	typedef Matrix4<int32_t> Matrix4I;	//!< Matrix4 storing values using 32 bit signed integers.
	typedef Matrix4<uint32_t> Matrix4U;	//!< Matrix4 storing values using 32 bit unsigned integers.
	
	/*!
		\brief Cast a matrix of one type to a matrix of another type.
		\tparam A The type being cast into.
		\tparam B The type being cast from.
		\param aMatrix The vector to cast from.
		\return The casted vector.
	*/
	template<typename A, typename B>
	static Matrix4<A> matrix_cast(const Matrix4<B> aMatrix){
		Matrix4<A> tmp;
		for(size_t i = 0; i < 4; ++i;){
			for(size_t j = 0; j < 4; ++j;){
				tmp.Set(i, j, static_cast<A>(aMatrix.Get(i, j)));
			}
		}
	}

}}


#endif