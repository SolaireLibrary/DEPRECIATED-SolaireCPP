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
	\brief Contains code for 4 dimentional vector maths.
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 2.0
	\date
	Created			: 23rd September 2015
	Last Modified	: 5th January 2016
*/

#include <cmath>
#include <cstdint>
#include <cstring>
#include <initializer_list>

namespace Solaire{

	template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
	class Matrix;

	template<class T, const uint32_t LENGTH>
	class Vector{
	public:
		typedef T Scalar;       //!< The scalar element type.
		enum : uint32_t {
		    Length = LENGTH     //!< The number of elements in this Vector.
		};
    private:
        Scalar mData[Length];
	public:
	    // Constructors & Destructor

        /*!
            \brief Create a new vector.
            \detail All elements will be initialised to 0.
        */
	    Vector() throw() {
			for(uint32_t i = 0; i < Length; ++i) {
				mData[i] = static_cast<Scalar>(0);
			}
	    }

        /*!
            \brief Create a new vector.
            \detail All elements will be initialised to \a aScalar.
            \param aScalar The value to fill the vector with.
        */
	    Vector(const Scalar aScalar) throw() {
			for(uint32_t i = 0; i < Length; ++i) {
				mData[i] = aScalar;
			}
	    }

        /*!
            \brief Create a new vector.
            \detail If there are not enough scalars in \a aElements then remaining elements will be initialised to 0.
            Additional scalars will be ignored.
            \param aElements The elements to fill the vector with.
        */
	    Vector(const std::initializer_list<Scalar> aElements) throw() {
	        auto j = aElements.begin();
	        const uint32_t size = aElements.size();
	        const uint32_t min = Length <= size ? Length : size;
	        for(uint32_t i = 0; i < min; ++i){
                mData[i] = *j;
                ++j;
	        }

            for(uint32_t i = min; i < Length; ++i){
                mData[i] = static_cast<Scalar>(0);
	        }
	    }

        template<const uint32_t Length1, const uint32_t Length2, class ENABLE = typename std::enable_if<Length1 + Length2 == Length>::type>
	    Vector(const Vector<Scalar, Length1> aFirst, const Vector<Scalar, Length2> aSecond) throw() {
	        std::memcpy(mData, aFirst.Ptr(), sizeof(Scalar) * Length1);
	        std::memcpy(mData + Length1, aSecond.Ptr(), sizeof(Scalar) * Length2);
	    }

        template<const uint32_t Length1, const uint32_t Length2, const uint32_t Length3, class ENABLE = typename std::enable_if<Length1 + Length2 + Length3 == Length>::type>
	    Vector(const Vector<Scalar, Length1> aFirst, const Vector<Scalar, Length2> aSecond, const Vector<Scalar, Length3> aThird) throw() {
	        std::memcpy(mData, aFirst.Ptr(), sizeof(Scalar) * Length1);
	        std::memcpy(mData + Length1, aSecond.Ptr(), sizeof(Scalar) * Length2);
	        std::memcpy(mData + Length1 + Length2, aThird.Ptr(), sizeof(Scalar) * Length3);
	    }

        template<const uint32_t Length1, const uint32_t Length2, const uint32_t Length3, const uint32_t Length4, class ENABLE = typename std::enable_if<Length1 + Length2 + Length3 + Length4 == Length>::type>
	    Vector(const Vector<Scalar, Length1> aFirst, const Vector<Scalar, Length2> aSecond, const Vector<Scalar, Length3> aThird, const Vector<Scalar, Length4> aFourth) throw() {
	        std::memcpy(mData, aFirst.Ptr(), sizeof(Scalar) * Length1);
	        std::memcpy(mData + Length1, aSecond.Ptr(), sizeof(Scalar) * Length2);
	        std::memcpy(mData + Length1 + Length2, aThird.Ptr(), sizeof(Scalar) * Length3);
	        std::memcpy(mData + Length1 + Length2 + Length3, aFourth.Ptr(), sizeof(Scalar) * Length4);
	    }

	    ~Vector() throw() {

	    }

	    // C++ Operators

        template<class Scalar2>
	    explicit operator Vector<Scalar2, Length>() throw() {
	        Vector<Scalar2, Length> tmp;
	        for(uint32_t i = 0; i < Length; ++i){
                tmp[i] = static_cast<Scalar2>(mData[i]);
	        }
	        return tmp;
	    }

	    operator Matrix<Scalar, Length, 1>() const throw() {
	        return *reinterpret_cast<const Matrix<Scalar, Length, 1>*>(this);
	    }

        template<const uint32_t L = Length, class ENABLE = typename std::enable_if<L != 1>::type>
	    operator Matrix<Scalar, 1, Length>() const throw() {
	        return *reinterpret_cast<const Matrix<Scalar, 1, Length>*>(this);
	    }

        /*!
            \brief Access an element of this vector.
            \detail Does not implement boundary checks.
            \param aIndex The element to access.
            \return A copy of the scalar element.
        */
	    Scalar operator[](const uint32_t aIndex) const throw() {
	        return mData[aIndex];
	    }

        /*!
            \brief Access an element of this vector.
            \detail Does not implement boundary checks.
            \param aIndex The element to access.
            \return A reference to the scalar element.
        */
	    Scalar& operator[](const uint32_t aIndex) throw() {
	        return mData[aIndex];
	    }

        /*!
            \brief Compare this vector to another.
            \param aOther The second vector.
            \return True if this vector is equal to \a aOther.
        */
		bool operator==(const Vector<Scalar, Length> aOther) const throw() {
			return std::memcmp(mData, aOther.mData, Length * sizeof(Scalar)) == 0;
		}

        /*!
            \brief Compare this vector to another.
            \param aOther The second vector.
            \return True if this vector is not equal to \a aOther.
        */
		bool operator!=(const Vector<Scalar, Length> aOther) const throw() {
			return std::memcmp(mData, aOther.mData, Length * sizeof(Scalar)) != 0;
		}

        /*!
            \brief Compare this vector to another.
            \detail This is implemented using the (squared) magnitude.
            \param aOther The second vector.
            \return True if this vector is less than \a aOther.
        */
		bool operator<(const Vector<Scalar, Length> aOther) const throw() {
			return MagnitudeSquared() < aOther.MagnitudeSquared();
		}

        /*!
            \brief Compare this vector to another.
            \detail This is implemented using the (squared) magnitude.
            \param aOther The second vector.
            \return True if this vector is greater than \a aOther.
        */
		bool operator>(const Vector<Scalar, Length> aOther) const throw() {
			return MagnitudeSquared() > aOther.MagnitudeSquared();
		}

        /*!
            \brief Compare this vector to another.
            \detail This is implemented using the (squared) magnitude.
            \param aOther The second vector.
            \return True if this vector is less than, or equal to \a aOther.
        */
		bool operator<=(const Vector<Scalar, Length> aOther) const throw() {
			return MagnitudeSquared() <= aOther.MagnitudeSquared();
		}

        /*!
            \brief Compare this vector to another.
            \detail This is implemented using the (squared) magnitude.
            \param aOther The second vector.
            \return True if this vector is greater than, or equal to \a aOther.
        */
		bool operator>=(const Vector<Scalar, Length> aOther) const throw() {
			return MagnitudeSquared() >= aOther.MagnitudeSquared();
		}

		// Utility / Mathmatical helpers

        /*!
            \brief Access the vector like a C-style array.
            \return The address of the first element.
        */
		Scalar* Ptr() throw() {
		    return mData;
		}

        /*!
            \brief Access the vector like a C-style array.
            \return The address of the first element.
        */
		const Scalar* Ptr() const throw() {
		    return mData;
		}

        /*!
            \brief Calculate the sum of all elements in the vector.
            \return The sum.
        */
		Scalar Sum() const throw() {
			Scalar sum = static_cast<Scalar>(0);
			for(uint32_t i = 0; i < Length; ++i) sum += mData[i];
			return sum;
		}

        /*!
            \brief Calculate the mean element in the vector.
            \return The mean element.
        */
		Scalar Average() const throw() {
			return Sum() / static_cast<Scalar>(Length);
		}

        /*!
            \brief Compute square of the magnitude of this vector.
            \detail This is faster than Magnitude().
            \return The square of the magnitude.
            \see Magnitude
        */
       Scalar MagnitudeSquared() const throw() {
			Scalar tmp = static_cast<Scalar>(0);
			for (uint32_t i = 0; i < Length; ++i){
				tmp += mData[i] * mData[i];
			}
			return tmp;
		}

        /*!
            \brief Compute magnitude of this vector.
            \return The magnitude.
            \see MagnitudeSquared
        */
		Scalar Magnitude(const T* const aVector) const throw() {
			return static_cast<Scalar>(std::sqrt(static_cast<double>(MagnitudeSquared(aVector))));
		}

        /*!
            \brief Compute normalised form of this vector.
            \return The normalised unit vector.
        */
		Vector<Scalar, Length> Normalise() const throw() {
			return Vector<Scalar, Length>(*this) / Magnitude();
		}

        /*!
            \brief Compute the dot product of two vectors.
            \param aOther The second vector.
            \return The dot product.
        */
		Scalar DotProduct(const Vector<Scalar, Length> aOther) const throw() {
			Scalar tmp = static_cast<Scalar>(0);
			for (uint32_t i = 0; i < Length; ++i) {
				tmp += mData[i] * aOther.mData[i];
			}
			return tmp;
		}

        /*!
            \brief Compute the cross product of two vectors.
            \detail Only available for vectors of length 3.
            \tparam L Used for conditional compilation.
            \tparam ENABLE Used for conditional compilation.
            \param aOther The second vector.
            \return The cross product.
        */
		template<const uint32_t L = LENGTH, typename ENABLE = typename std::enable_if<L == 3>::type>
		Vector<Scalar, Length> CrossProduct(const Vector<Scalar, Length> aOther) const throw() {
		    return {
                (mData[1] * aOther.mData[2]) - (mData[2] * aOther.mData[1]),    // (Y1 * Z2) - (Z2 * Y1)
                (mData[2] * aOther.mData[0]) - (mData[0] * aOther.mData[2]),    // (Z1 * X2) - (X2 * Z1)
                (mData[0] * aOther.mData[1]) - (mData[1] * aOther.mData[0])     // (X1 * Y2) - (Y2 * X1)
		    };
		}

        /*!
            \brief Perform linear interpolation between two vectors.
            \detail
            \code
            Vector3F vectorA = {1.f, 10.f, 20.f};
            Vector3F vectorB = {10.f, 20.f, 30.f};
            Vector3F vectorC = vectorA,Lerp(vectorB, 0.5);  // This results in the vector {5.f, 15.f, 25.f}
            \endcode
            \param aOther The other vector.
            \param aWeight The weighting bias for this vector.
            \return The interpolated vector.
        */
		Vector<Scalar, Length> Lerp(const Vector<Scalar, Length> aOther, const double aWeight) const throw() {
		    Vector<Scalar, Length> tmp;
			for(uint32_t i = 0; i < Length; ++i) {
				tmp[i] = static_cast<Scalar>((1.0 - aWeight) * static_cast<double>(mData[i]) + aWeight * static_cast<double>(aOther.mData[i]));
			}
			return tmp;
		}

        /*!
            \brief Create a new vector by re-arranging the scalar elements of this vector.
            \detail
            \code
            Vector3F vectorA = {5.f, 10.f, 15.f};           // The value of this vector does not change
            Vector3F vectorB = vectorA.Swizzle({2, 1, 0});  // Re-arrange to be equivilent to vector {15.f, 10.f, 5.f}
            \endcode
            The size of the new vector can be different than the source
            \code
            Vector4F vectorA = {5.f, 10.f, 15.f, 20.f};
            Vector3F vectorB = vectorA.Swizzle({0, 1, 2});                              // Vector is shortened to only XYZ elements
            Vector<float, 8> vectorB = vectorA.Swizzle({0, 0, 1, 1, 2, 2, 3, 3});       // Vector is extended by duplicating each element
            \endcode
            Helper macros are defined in Swizzle.hpp
            \code
            Vector4F vectorA = {5.f, 10.f, 15.f, 20.f};
            Vector3F vectorB = vectorA.Swizzle(SOLAIRE_XYZ);  // Vector is shortened to only XYZ elements
            Vector3F vectorB = vectorA.Swizzle(SOLAIRE_ZYX);  // Vector is shortened to only XYZ elements, in reverse order
            \endcode
            \
            \tparam Scalar2 The index type.
            \tparam Length2 The length of the new vector, and the length of the index vector.
            \param aSwizzle A vector containing the index values to use.
            \return The new vector.
        */
        template<class Scalar2, const uint32_t Length2>
		Vector<Scalar, Length2> Swizzle(const Vector<Scalar2, Length2> aSwizzle)  throw() {
			Vector<Scalar, Length> tmp;
			for(uint32_t i = 0; i < Length2; ++i) {
				tmp[i] = mData[aSwizzle[i]];
			}
			return tmp;
		}
	};

	// Vector / Vector

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length>& operator+=(Vector<Scalar, Length>& aFirst, const Vector<Scalar, Length> aSecond) {
	    for(uint32_t i = 0; i < Length; ++i) {
            aFirst[i] += aSecond[i];
	    }
	    return aFirst;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length>& operator-=(Vector<Scalar, Length>& aFirst, const Vector<Scalar, Length> aSecond) {
	    for(uint32_t i = 0; i < Length; ++i) {
            aFirst[i] -= aSecond[i];
	    }
	    return aFirst;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length>& operator*=(Vector<Scalar, Length>& aFirst, const Vector<Scalar, Length> aSecond) {
	    for(uint32_t i = 0; i < Length; ++i) {
            aFirst[i] *= aSecond[i];
	    }
	    return aFirst;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length>& operator/=(Vector<Scalar, Length>& aFirst, const Vector<Scalar, Length> aSecond) {
	    for(uint32_t i = 0; i < Length; ++i) {
            aFirst[i] /= aSecond[i];
	    }
	    return aFirst;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length> operator+(const Vector<Scalar, Length> aFirst, const Vector<Scalar, Length> aSecond) {
	    Vector<Scalar, Length> tmp;
	    for(uint32_t i = 0; i < Length; ++i) {
            tmp[i] = aFirst[i] + aSecond[i];
	    }
	    return tmp;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length> operator-(const Vector<Scalar, Length> aFirst, const Vector<Scalar, Length> aSecond) {
        Vector<Scalar, Length> tmp;
	    for(uint32_t i = 0; i < Length; ++i) {
            tmp[i] = aFirst[i] - aSecond[i];
	    }
	    return tmp;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length> operator*(const Vector<Scalar, Length> aFirst, const Vector<Scalar, Length> aSecond) {
        Vector<Scalar, Length> tmp;
	    for(uint32_t i = 0; i < Length; ++i) {
            tmp[i] = aFirst[i] * aSecond[i];
	    }
	    return tmp;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length> operator/(const Vector<Scalar, Length> aFirst, const Vector<Scalar, Length> aSecond) {
	    return Vector<Scalar, Length>(aFirst) /= aSecond;
	}

	// Vector / Scalar

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length>& operator+=(Vector<Scalar, Length>& aFirst, const Scalar aSecond) {
	    for(uint32_t i = 0; i < Length; ++i) {
            aFirst[i] += aSecond;
	    }
	    return aFirst;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length>& operator-=(Vector<Scalar, Length>& aFirst, const Scalar aSecond) {
	    for(uint32_t i = 0; i < Length; ++i) {
            aFirst[i] -= aSecond;
	    }
	    return aFirst;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length>& operator*=(Vector<Scalar, Length>& aFirst, const Scalar aSecond) {
	    for(uint32_t i = 0; i < Length; ++i) {
            aFirst[i] *= aSecond;
	    }
	    return aFirst;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length>& operator/=(Vector<Scalar, Length>& aFirst, const Scalar aSecond) {
	    for(uint32_t i = 0; i < Length; ++i) {
            aFirst[i] /= aSecond;
	    }
	    return aFirst;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length> operator+(const Vector<Scalar, Length> aFirst, const Scalar aSecond) {
	    return Vector<Scalar, Length>(aFirst) += aSecond;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length> operator-(const Vector<Scalar, Length> aFirst, const Scalar aSecond) {
	    return Vector<Scalar, Length>(aFirst) -= aSecond;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length> operator*(const Vector<Scalar, Length> aFirst, const Scalar aSecond) {
	    return Vector<Scalar, Length>(aFirst) *= aSecond;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length> operator/(const Vector<Scalar, Length> aFirst, const Scalar aSecond) {
	    return Vector<Scalar, Length>(aFirst) /= aSecond;
	}

	// Scalar / Vector

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length> operator+(const Scalar aFirst, const Vector<Scalar, Length> aSecond) {
	    Vector<Scalar, Length> tmp;
	    for(uint32_t i = 0; i < Length; ++i) {
            tmp[i] = aFirst + aSecond[i];
	    }
	    return tmp;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length> operator-(const Scalar aFirst, const Vector<Scalar, Length> aSecond) {
	    Vector<Scalar, Length> tmp;
	    for(uint32_t i = 0; i < Length; ++i) {
            tmp[i] = aFirst - aSecond[i];
	    }
	    return tmp;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length> operator*(const Scalar aFirst, const Vector<Scalar, Length> aSecond) {
	    Vector<Scalar, Length> tmp;
	    for(uint32_t i = 0; i < Length; ++i) {
            tmp[i] = aFirst * aSecond[i];
	    }
	    return tmp;
	}

	template<class Scalar, const uint32_t Length>
	Vector<Scalar, Length> operator/(const Scalar aFirst, const Vector<Scalar, Length> aSecond) {
	    Vector<Scalar, Length> tmp;
	    for(uint32_t i = 0; i < Length; ++i) {
            tmp[i] = aFirst / aSecond[i];
	    }
	    return tmp;
	}

	// Typedef

	template<class Scalar>
	using Vector2 = Vector<Scalar, 2>;

	template<class Scalar>
	using Vector3 = Vector<Scalar, 3>;

	template<class Scalar>
	using Vector4 = Vector<Scalar, 4>;

	typedef Vector2<uint8_t>    Vector2U8;
	typedef Vector2<int8_t>     Vector2I8;
	typedef Vector2<uint16_t>   Vector2U16;
	typedef Vector2<int16_t>    Vector2I16;
	typedef Vector2<uint32_t>   Vector2U32;
	typedef Vector2<int32_t>    Vector2I32;
	typedef Vector2<uint64_t>   Vector2U64;
	typedef Vector2<int64_t>    Vector2I64;
	typedef Vector2<float>      Vector2F;
	typedef Vector2<double>     Vector2D;

	typedef Vector3<uint8_t>    Vector3U8;
	typedef Vector3<int8_t>     Vector3I8;
	typedef Vector3<uint16_t>   Vector3U16;
	typedef Vector3<int16_t>    Vector3I16;
	typedef Vector3<uint32_t>   Vector3U32;
	typedef Vector3<int32_t>    Vector3I32;
	typedef Vector3<uint64_t>   Vector3U64;
	typedef Vector3<int64_t>    Vector3I64;
	typedef Vector3<float>      Vector3F;
	typedef Vector3<double>     Vector3D;

	typedef Vector4<uint8_t>    Vector4U8;
	typedef Vector4<int8_t>     Vector4I8;
	typedef Vector4<uint16_t>   Vector4U16;
	typedef Vector4<int16_t>    Vector4I16;
	typedef Vector4<uint32_t>   Vector4U32;
	typedef Vector4<int32_t>    Vector4I32;
	typedef Vector4<uint64_t>   Vector4U64;
	typedef Vector4<int64_t>    Vector4I64;
	typedef Vector4<float>      Vector4F;
	typedef Vector4<double>     Vector4D;
}


#endif
