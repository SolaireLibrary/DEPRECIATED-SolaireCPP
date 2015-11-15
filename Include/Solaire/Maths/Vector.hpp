#ifndef SOLAIRE_VECTOR_HPP
#define SOLAIRE_VECTOR_HPP

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
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 23rd September 2015
	Last Modified	: 11th November 2015
*/

#include "VectorMaths.hpp"

namespace Solaire{

	template<class T, const uint32_t LENGTH>
	class Vector{
	public:
		typedef T Scalar;
		typedef typename TypeTraits<Scalar>::ConstPassType ScalarPass;
	private:
		typedef VectorMaths<T, LENGTH> MathsImplementation;
	private:
		Scalar mVector[LENGTH];
	public:
		Vector() throw() {

		}

		Vector(ScalarPass aScalar) throw() {
			Fill(aScalar);
		}

		Vector(const std::initializer_list<Scalar> aScalars) throw() {
			uint32_t i = 0;

			for(ScalarPass i scalar : aScalars) {
				mVector[i] = scalar;
				++i;
				if(i == LENGTH) return;
			}

			for(i; i < LENGTH; ++i) {
				mVector[i] = static_cast<T>(0);
			}
		}

		Vector(const std::initializer_list<Scalar> aScalars, ScalarPass aDefault) throw() {
			uint32_t i = 0;

			for (ScalarPass i scalar : aScalars) {
				mVector[i] = scalar;
				++i;
				if(i == LENGTH) return;
			}

			for (i; i < LENGTH; ++i) {
				mVector[i] = aDefault;
			}
		}

		inline ScalarPass operator[](const uint32_t aIndex) const throw() {
			return mVector[aIndex];
		}

		inline Scalar& operator[](const uint32_t aIndex) throw() {
			return mVector[aIndex];
		}

		inline uint32_t Size() const throw() {
			return LENGTH;
		}

		inline const Scalar* AsPointer() const throw() {
			return mVector;
		}

		inline Scalar* AsPointer() throw() {
			return mVector;
		}

		inline Vector<Scalar, LENGTH> Lerp(const Vector<Scalar, LENGTH> aOther, const double aWeight) const throw() {
			Vector<Scalar, LENGTH> tmp;
			MathsImplementation::Lerp(tmp.mVector, mVector, aOther.mVector, aWeight);
			return tmp;
		}

		inline Scalar Sum() const throw() {
			return MathsImplementation::Sum(mVector);
		}

		inline Scalar Average() const throw() {
			return MathsImplementation::Avg(mVector);
		}

		inline double MagnitudeSquared() const throw() {
			return MathsImplementation::MagSq(mVector);
		}

		inline double Magnitude() const throw() {
			return MathsImplementation::Mag(mVector);
		}

		inline Scalar Dot(const Vector<Scalar, LENGTH> aOther) const throw() {
			return MathsImplementation::Dot(mVector, aOther.mVector);
		}

		template<const uint32_t L = LENGTH, typename ENABLE = typename std::enable_if<L == 3>::type>
		inline Vector<Scalar, LENGTH> Cross(const Vector<Scalar, LENGTH> aOther) const throw() {
			Vector<Scalar, LENGTH> tmp;
			MathsImplementation::Cross(tmp.mVector, mVector, aOther.mVector);
			return tmp;
		}

		inline Vector<Scalar, LENGTH> Swizzle(const Vector<uint32_t, LENGTH> aSwizzle) const throw() {
			Vector<Scalar, LENGTH> tmp;
			MathsImplementation::Swizzle(tmp.mVector, mVector, aSwizzle.mVector);
			return tmp;
		}

		inline Vector<Scalar, LENGTH>& Normalise() throw() {
			MathsImplementation::NormaliseEq(mVector);
			return *this;
		}

		inline Vector<Scalar, LENGTH>& Fill(const ScalarPass aScalar) throw() {
			MathsImplementation::Fill(mVector, aScalar);
			return *this;
		}

		template<const uint32_t L = LENGTH, typename ENABLE = typename std::enable_if<L >= 1>::type>
		inline ScalarPass X() const throw() {
			return mVector[0];
		}

		template<const uint32_t L = LENGTH, typename ENABLE = typename std::enable_if<L >= 1>::type>
		inline Scalar& X() throw() {
			return mVector[0];
		}

		template<const uint32_t L = LENGTH, typename ENABLE = typename std::enable_if<L >= 2>::type>
		inline ScalarPass Y() const throw() {
			return mVector[1];
		}

		template<const uint32_t L = LENGTH, typename ENABLE = typename std::enable_if<L >= 2>::type>
		inline Scalar& Y() throw() {
			return mVector[1];
		}

		template<const uint32_t L = LENGTH, typename ENABLE = typename std::enable_if<L >= 3>::type>
		inline ScalarPass Z() const throw() {
			return mVector[2];
		}

		template<const uint32_t L = LENGTH, typename ENABLE = typename std::enable_if<L >= 3>::type>
		inline Scalar& Z() throw() {
			return mVector[2];
		}

		template<const uint32_t L = LENGTH, typename ENABLE = typename std::enable_if<L >= 4>::type>
		inline ScalarPass W() const throw() {
			return mVector[3];
		}

		template<const uint32_t L = LENGTH, typename ENABLE = typename std::enable_if<L >= 4>::type>
		inline Scalar& W() throw() {
			return mVector[3];
		}
	};

	template<class T>
	using Vector2 = Vector<T, 2>;

	template<class T>
	using Vector3 = Vector<T, 3>;

	template<class T>
	using Vector4 = Vector<T, 4>;

	typedef Vector2<uint8_t>	Vector2UB;
	typedef Vector2<int8_t>		Vector2IB;
	typedef Vector2<uint32_t>	Vector2U;
	typedef Vector2<int32_t>	Vector2I;
	typedef Vector2<float>		Vector2F;
	typedef Vector2<double>		Vector2D;

	typedef Vector3<uint8_t>	Vector3UB;
	typedef Vector3<int8_t>		Vector3IB;
	typedef Vector3<uint32_t>	Vector3U;
	typedef Vector3<int32_t>	Vector3I;
	typedef Vector3<float>		Vector3F;
	typedef Vector3<double>		Vector3D;

	typedef Vector4<uint8_t>	Vector4UB;
	typedef Vector4<int8_t>		Vector4IB;
	typedef Vector4<uint32_t>	Vector4U;
	typedef Vector4<int32_t>	Vector4I;
	typedef Vector4<float>		Vector4F;
	typedef Vector4<double>		Vector4D;

	template<class T, class T2, const uint32_t LENGTH>
	static inline Solaire::Vector<T, LENGTH> VectorCast(const Solaire::Vector<T2, LENGTH> aVector) throw() {
		Solaire::Vector<T, LENGTH> tmp;
		Solaire::VectorMaths<T, LENGTH>::Cast<T2>(aVector.AsPointer(), tmp.AsPointer());
		return tmp;
	}
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH>& operator+=(Solaire::Vector<T, LENGTH>& aFirst, const Solaire::Vector<T, LENGTH> aSecond) throw() {
	Solaire::VectorMaths<T, LENGTH>::AddEq(aFirst.AsPointer(), aSecond.AsPointer());
	return aFirst;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH>& operator-=(Solaire::Vector<T, LENGTH>& aFirst, const Solaire::Vector<T, LENGTH> aSecond) throw() {
	Solaire::VectorMaths<T, LENGTH>::SubEq(aFirst.AsPointer(), aSecond.AsPointer());
	return aFirst;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH>& operator*=(Solaire::Vector<T, LENGTH>& aFirst, const Solaire::Vector<T, LENGTH> aSecond) throw() {
	Solaire::VectorMaths<T, LENGTH>::MulEq(aFirst.AsPointer(), aSecond.AsPointer());
	return aFirst;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH>& operator/=(Solaire::Vector<T, LENGTH>& aFirst, const Solaire::Vector<T, LENGTH> aSecond) throw() {
	Solaire::VectorMaths<T, LENGTH>::DivEq(aFirst.AsPointer(), aSecond.AsPointer());
	return aFirst;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH>& operator+=(Solaire::Vector<T, LENGTH>& aFirst, const typename Solaire::Vector<T, LENGTH>::ScalarPass aScalar) throw() {
	Solaire::VectorMaths<T, LENGTH>::AddEq(aFirst.AsPointer(), aScalar);
	return aFirst;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH>& operator-=(Solaire::Vector<T, LENGTH>& aFirst, const typename Solaire::Vector<T, LENGTH>::ScalarPass aScalar) throw() {
	Solaire::VectorMaths<T, LENGTH>::SubEq(aFirst.AsPointer(), aScalar);
	return aFirst;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH>& operator*=(Solaire::Vector<T, LENGTH>& aFirst, const typename Solaire::Vector<T, LENGTH>::ScalarPass aScalar) throw() {
	Solaire::VectorMaths<T, LENGTH>::MulEq(aFirst.AsPointer(), aScalar);
	return aFirst;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH>& operator/=(Solaire::Vector<T, LENGTH>& aFirst, const typename Solaire::Vector<T, LENGTH>::ScalarPass aScalar) throw() {
	Solaire::VectorMaths<T, LENGTH>::DivEq(aFirst.AsPointer(), aScalar);
	return aFirst;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH> operator+(const Solaire::Vector<T, LENGTH> aFirst, const Solaire::Vector<T, LENGTH> aSecond) throw() {
	Solaire::Vector<T, LENGTH> tmp;
	Solaire::VectorMaths<T, LENGTH>::Add(tmp.AsPointer(), aFirst.AsPointer(), aSecond.AsPointer());
	return tmp;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH> operator-(const Solaire::Vector<T, LENGTH> aFirst, const Solaire::Vector<T, LENGTH> aSecond) throw() {
	Solaire::Vector<T, LENGTH> tmp;
	Solaire::VectorMaths<T, LENGTH>::Sub(tmp.AsPointer(), aFirst.AsPointer(), aSecond.AsPointer());
	return tmp;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH> operator*(const Solaire::Vector<T, LENGTH> aFirst, const Solaire::Vector<T, LENGTH> aSecond) throw() {
	Solaire::Vector<T, LENGTH> tmp;
	Solaire::VectorMaths<T, LENGTH>::Mul(tmp.AsPointer(), aFirst.AsPointer(), aSecond.AsPointer());
	return tmp;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH> operator/(const Solaire::Vector<T, LENGTH> aFirst, const Solaire::Vector<T, LENGTH> aSecond) throw() {
	Solaire::Vector<T, LENGTH> tmp;
	Solaire::VectorMaths<T, LENGTH>::Div(tmp.AsPointer(), aFirst.AsPointer(), aSecond.AsPointer());
	return tmp;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH> operator+(const Solaire::Vector<T, LENGTH> aFirst, const typename Solaire::Vector<T, LENGTH>::ScalarPass aScalar) throw() {
	Solaire::Vector<T, LENGTH> tmp;
	Solaire::VectorMaths<T, LENGTH>::Add(tmp.AsPointer(), aFirst.AsPointer(), aScalar);
	return aFirst;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH> operator-(const Solaire::Vector<T, LENGTH> aFirst, const typename Solaire::Vector<T, LENGTH>::ScalarPass aScalar) throw() {
	Solaire::Vector<T, LENGTH> tmp;
	Solaire::VectorMaths<T, LENGTH>::Sub(tmp.AsPointer(), aFirst.AsPointer(), aScalar);
	return aFirst;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH> operator*(const Solaire::Vector<T, LENGTH> aFirst, const typename Solaire::Vector<T, LENGTH>::ScalarPass aScalar) throw() {
	Solaire::Vector<T, LENGTH> tmp;
	Solaire::VectorMaths<T, LENGTH>::Mul(tmp.AsPointer(), aFirst.AsPointer(), aScalar);
	return aFirst;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH> operator/(const Solaire::Vector<T, LENGTH> aFirst, const typename Solaire::Vector<T, LENGTH>::ScalarPass aScalar) throw() {
	Solaire::Vector<T, LENGTH> tmp;
	Solaire::VectorMaths<T, LENGTH>::Div(tmp.AsPointer(), aFirst.AsPointer(), aScalar);
	return aFirst;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH> operator+(const typename Solaire::Vector<T, LENGTH>::ScalarPass aScalar, const Solaire::Vector<T, LENGTH> aFirst) throw() {
	Solaire::Vector<T, LENGTH> tmp;
	Solaire::VectorMaths<T, LENGTH>::Add(tmp.AsPointer(), aScalar, aFirst.AsPointer());
	return aFirst;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH> operator-(const typename Solaire::Vector<T, LENGTH>::ScalarPass aScalar, const Solaire::Vector<T, LENGTH> aFirst) throw() {
	Solaire::Vector<T, LENGTH> tmp;
	Solaire::VectorMaths<T, LENGTH>::Sub(tmp.AsPointer(), aScalar, aFirst.AsPointer());
	return aFirst;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH> operator*(const typename Solaire::Vector<T, LENGTH>::ScalarPass aScalar, const Solaire::Vector<T, LENGTH> aFirst) throw() {
	Solaire::Vector<T, LENGTH> tmp;
	Solaire::VectorMaths<T, LENGTH>::Mul(tmp.AsPointer(), aScalar, aFirst.AsPointer());
	return aFirst;
}

template<class T, const uint32_t LENGTH>
static inline Solaire::Vector<T, LENGTH> operator/(const typename Solaire::Vector<T, LENGTH>::ScalarPass aScalar, const Solaire::Vector<T, LENGTH> aFirst) throw() {
	Solaire::Vector<T, LENGTH> tmp;
	Solaire::VectorMaths<T, LENGTH>::Div(tmp.AsPointer(), aScalar, aFirst.AsPointer());
	return aFirst;
}


#endif
