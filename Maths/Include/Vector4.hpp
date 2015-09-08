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

#include <cstdint>
#include <stdexcept>
#include "VectorShared.hpp"

namespace Solaire{ namespace Maths{

	template<typename TYPE>
	class Vector4{
	public:
		typedef TYPE value_t;

		value_t X;
		value_t Y;
		value_t Z;
		value_t W;

		// Constructors

		Vector4() :
			X(static_cast<value_t>(0)),
			Y(static_cast<value_t>(0)),
			Z(static_cast<value_t>(0)),
			W(static_cast<value_t>(0))
		{

		}

		Vector4(const value_t aX, const value_t aY, const value_t aZ, const value_t aW) :
			X(aX),
			Y(aY),
			Z(aZ),
			W(aW)
		{

		}

		// Operator overloads

		bool operator==(const Vector4<value_t> aOther) const{
			return X == aOther.X && Y == aOther.Y && Z == aOther.Z && W == aOther.W;
		}

		bool operator!=(const Vector4<value_t> aOther) const{
			return X != aOther.X || Y != aOther.Y || Z != aOther.Z || W != aOther.W;
		}

		Vector4<value_t>& operator+=(const Vector4<value_t> aOther){
			X += aOther.X;
			Y += aOther.Y;
			Z += aOther.Z;
			W += aOther.W;
			return *this;
		}
		
		Vector4<value_t>& operator+=(const value_t aScalar){
			X += aScalar;
			Y += aScalar;
			Z += aScalar;
			W += aScalar;
			return *this;
		}
		
		Vector4<value_t> operator+(const Vector4<value_t> aOther) const{
			return Vector4<value_t>(*this) += aOther;
		}
		
		Vector4<value_t> operator+(const value_t aScalar) const{
			return Vector4<value_t>(*this) += aScalar;
		}

		Vector4<value_t>& operator-=(const Vector4<value_t> aOther){
			X -= aOther.X;
			Y -= aOther.Y;
			Z -= aOther.Z;
			W -= aOther.W;
			return *this;
		}
		
		Vector4<value_t>& operator-=(const value_t aScalar){
			X -= aScalar;
			Y -= aScalar;
			Z -= aScalar;
			W -= aScalar;
			return *this;
		}
		
		Vector4<value_t> operator-(const Vector4<value_t> aOther) const{
			return Vector4<value_t>(*this) -= aOther;
		}
		
		Vector4<value_t> operator-(const value_t aScalar) const{
			return Vector4<value_t>(*this) -= aScalar;
		}
		
		Vector4<value_t>& operator*=(const Vector4<value_t> aOther){
			X *= aOther.X;
			Y *= aOther.Y;
			Z *= aOther.Z;
			W *= aOther.W;
			return *this;
		}
		
		Vector4<value_t>& operator*=(const value_t aScalar){
			X *= aScalar;
			Y *= aScalar;
			Z *= aScalar;
			W *= aScalar;
			return *this;
		}
		
		Vector4<value_t> operator*(const Vector4<value_t> aOther) const{
			return Vector4<value_t>(*this) *= aOther;
		}
		
		Vector4<value_t> operator*(const value_t aScalar) const{
			return Vector4<value_t>(*this) *= aScalar;
		}

		Vector4<value_t>& operator/=(const Vector4<value_t> aOther){
			X /= aOther.X;
			Y /= aOther.Y;
			Z /= aOther.Z;
			W /= aOther.W;
			return *this;
		}
		
		Vector4<value_t>& operator/=(const value_t aScalar){
			X /= aScalar;
			Y /= aScalar;
			Z /= aScalar;
			W /= aScalar;
			return *this;
		}
		
		Vector4<value_t> operator/(const Vector4<value_t> aOther) const{
			return Vector4<value_t>(*this) /= aOther;
		}
		
		Vector4<value_t> operator/(const value_t aScalar) const{
			return Vector4<value_t>(*this) /= aScalar;
		}

		value_t operator[](const size_t aIndex) const{
			if(aIndex >= 4) throw std::runtime_error("Vector4 index is out of bounds");
			return (&X)[aIndex];
		}

		value_t& operator[](const size_t aIndex){
			if(aIndex >= 4) throw std::runtime_error("Vector4 index is out of bounds");
			return (&X)[aIndex];
		}

		value_t operator[](const char aAxis) const{
			return operator[](Maths::AxisToIndex(aAxis));
		}

		value_t& operator[](const char aAxis){
			return operator[](Maths::AxisToIndex(aAxis));
		}

		// Vector operators

		size_t Length() const{
			return 4;
		}

		value_t Sum() const{
			return X + Y + Z + W;
		}

		value_t Average() const{
			return Sum() / static_cast<value_t>(Length());
		}

		value_t DotProduct(const Vector4<value_t> aOther) const{
			return operator*(aOther).Sum();
		}

		Vector4<value_t> CrossProduct(const Vector4<value_t> aOther) const{
			//! \TODO Implement or remove 4 dimentional cross product
			throw std::runtime_error("Vector4.CrossProduct not implemented");
		}

		value_t MagnitudeSquared() const{
			return operator*(*this).Sum();
		}

		value_t Magnitude() const{
			return static_cast<value_t>(std::sqrt(static_cast<double>(MagnitudeSquared())));
		}

		Vector4<value_t> Normalise() const{
			return operator/(Magnitude());
		}

		// Other

		static Vector4<value_t> Swizzle(const Vector4<value_t>& aVector, const char* const aSwizzle){
			return Vector4<value_t>(
				aVector[aSwizzle[0]],
				aVector[aSwizzle[1]],
				aVector[aSwizzle[2]],
				aVector[aSwizzle[3]]
			);
		}

		value_t* begin(){
			return &X;
		}

		const value_t* begin() const{
			return &X;
		}

		value_t* end(){
			return (&W) + 1;
		}

		const value_t* end() const{
			return (&W) + 1;
		}
	};

	typedef Vector4<double> Vector4D;
	typedef Vector4<float> Vector4F;
	typedef Vector4<int32_t> Vector4I;
	typedef Vector4<uint32_t> Vector4U;
	
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