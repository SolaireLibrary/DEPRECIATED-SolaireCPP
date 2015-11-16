#ifndef SOLAIRE_MATRIX_HPP
#define SOLAIRE_MATRIX_HPP

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
	\file Matrix.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 23rd September 2015
	Last Modified	: 11th November 2015
*/

#include <cstdint>
#include "Vector.hpp"

namespace Solaire{

	template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
	class Matrix{
	public:
		typedef T Scalar;
		typedef typename TypeTraits<Scalar>::ConstPassType ScalarPass;
		typedef Vector<Scalar, WIDTH> Row;
		typedef Vector<Scalar, HEIGHT> Column;
	private:
		Scalar mMatrix[WIDTH * HEIGHT];
	public:
		Matrix() throw() {

		}

		Matrix(const std::initializer_list<Row> aRows) throw() {
			uint32_t i = 0;
			for(Row row : aRows) {
				SetRow(i, row);
				++i;
				if(i == HEIGHT) return;
			}
		}

		inline bool operator==(const Matrix<Scalar, WIDTH, HEIGHT>& aOther) const throw() {
			return Solaire::VectorMaths<T, WIDTH * HEIGHT>::Equals(aFirst.mMatrix, aSecond.mMatrix);
		}

		inline bool operator!=(const Matrix<Scalar, WIDTH, HEIGHT>& aOther) const throw() {
			return Solaire::VectorMaths<T, WIDTH * HEIGHT>::NotEquals(aFirst.mMatrix, aSecond.mMatrix);
		}

		inline const Scalar* operator[](const uint32_t aIndex) const throw() {
			return mMatrix + (aIndex * WIDTH);
		}

		inline Scalar* operator[](const uint32_t aIndex) throw() {
			return mMatrix + (aIndex * WIDTH);
		}

		inline const Scalar* AsPointer() const throw() {
			return mMatrix;
		}

		inline Scalar* AsPointer() throw() {
			return mMatrix;
		}

		inline Row GetRow(const uint32_t aIndex) const throw() {
			return *reinterpret_cast<const Row*>(operator[](aIndex));
		}

		inline Row& GetRow(const uint32_t aIndex) throw() {
			return *reinterpret_cast<Row*>(operator[](aIndex));
		}

		Column GetColumn(const uint32_t aIndex) const throw() {
			Column tmp;
			for (uint32_t i = 0; i < HEIGHT; ++i) {
				tmp[i] = mMatrix[i * aIndex];
			}
			return tmp;
		}

		inline void SetRow(const uint32_t aIndex, const Row aRow) throw() {
			std::memcopy(operator[](aIndex), aRow.AsPointer(), sizeof(Scalar) * WIDTH);
		}

		void SetColumn(const uint32_t aIndex, const Column aColumn) throw() {
			for (uint32_t i = 0; i < HEIGHT; ++i) {
				mMatrix[i * aIndex] = aColumn[i];
			}
		}

		Matrix<Scalar, HEIGHT, WIDTH> Transpose() const throw() {
			Matrix<Scalar, HEIGHT, WIDTH> tmp;
			for(uint32_t i = 0; i < WIDTH; ++i) {
				tmp.SetColumn(i, GetRow(i));
			}
			return tmp;
		}

		Matrix<Scalar, HEIGHT, WIDTH> Inverse() const throw() {
			//! \todo Implement matrix inversion
			throw std::runtime_error("Solaire::Matrix : Inversion not implemented");
		}
	};

	template<class T>
	using Matrix2 = Matrix<T, 2, 2>;

	template<class T>
	using Matrix3 = Matrix<T, 3, 3>;

	template<class T>
	using Matrix4 = Matrix<T, 4, 4>;

	typedef Matrix2<uint8_t>	Matrix2UB;
	typedef Matrix2<int8_t>		Matrix2IB;
	typedef Matrix2<uint32_t>	Matrix2U;
	typedef Matrix2<int32_t>	Matrix2I;
	typedef Matrix2<float>		Matrix2F;
	typedef Matrix2<double>		Matrix2D;

	typedef Matrix3<uint8_t>	Matrix3UB;
	typedef Matrix3<int8_t>		Matrix3IB;
	typedef Matrix3<uint32_t>	Matrix3U;
	typedef Matrix3<int32_t>	Matrix3I;
	typedef Matrix3<float>		Matrix3F;
	typedef Matrix3<double>		Matrix3D;

	typedef Matrix4<uint8_t>	Matrix4UB;
	typedef Matrix4<int8_t>		Matrix4IB;
	typedef Matrix4<uint32_t>	Matrix4U;
	typedef Matrix4<int32_t>	Matrix4I;
	typedef Matrix4<float>		Matrix4F;
	typedef Matrix4<double>		Matrix4D;

}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static inline Solaire::Matrix<T, WIDTH, HEIGHT>& operator+=(Solaire::Matrix<T, WIDTH, HEIGHT>& aFirst, const Solaire::Matrix<T, WIDTH, HEIGHT>& aSecond) throw() {
	Solaire::VectorMaths<T, WIDTH * HEIGHT>::AddEq(aFirst.AsPointer(), aSecond.AsPointer());
	return aFirst;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static inline Solaire::Matrix<T, WIDTH, HEIGHT>& operator-=(Solaire::Matrix<T, WIDTH, HEIGHT>& aFirst, const Solaire::Matrix<T, WIDTH, HEIGHT>& aSecond) throw() {
	Solaire::VectorMaths<T, WIDTH * HEIGHT>::SubEq(aFirst.AsPointer(), aSecond.AsPointer());
	return aFirst;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static Solaire::Matrix<T, WIDTH, HEIGHT>& operator*=(Solaire::Matrix<T, WIDTH, HEIGHT>& aFirst, const Solaire::Matrix<T, WIDTH, HEIGHT>& aSecond) throw() {
	return aFirst = aFirst * aSecond;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static inline Solaire::Matrix<T, WIDTH, HEIGHT>& operator+=(Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix, typename Solaire::Matrix<T, WIDTH, HEIGHT>::ScalarPass aScalar) throw() {
	Solaire::VectorMaths<T, WIDTH * HEIGHT>::AddEq(aMatrix.AsPointer(), aScalar);
	return aMatrix;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static inline Solaire::Matrix<T, WIDTH, HEIGHT>& operator-=(Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix, typename Solaire::Matrix<T, WIDTH, HEIGHT>::ScalarPass aScalar) throw() {
	Solaire::VectorMaths<T, WIDTH * HEIGHT>::SubEq(aMatrix.AsPointer(), aScalar);
	return aMatrix;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static inline Solaire::Matrix<T, WIDTH, HEIGHT>& operator*=(Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix, typename Solaire::Matrix<T, WIDTH, HEIGHT>::ScalarPass aScalar) throw() {
	Solaire::VectorMaths<T, WIDTH * HEIGHT>::MulEq(aMatrix.AsPointer(), aScalar);
	return aMatrix;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static inline Solaire::Matrix<T, WIDTH, HEIGHT>& operator/=(Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix, typename Solaire::Matrix<T, WIDTH, HEIGHT>::ScalarPass aScalar) throw() {
	Solaire::VectorMaths<T, WIDTH * HEIGHT>::DivEq(aMatrix.AsPointer(), aScalar);
	return aMatrix;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static Solaire::Vector<T, HEIGHT>& operator+=(Solaire::Vector<T, HEIGHT>& aVector, const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix) throw() {
	for(uint32_t i = 0; i < HEIGHT) {
		aVector[i] += aMatrix.GetRow(i).Sum();
	}
	return aVector;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static Solaire::Vector<T, HEIGHT>& operator-=(Solaire::Vector<T, HEIGHT>& aVector, const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix) throw() {
	for(uint32_t i = 0; i < HEIGHT) {
		aVector[i] -= aMatrix.GetRow(i).Sum();
	}
	return aVector;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static Solaire::Vector<T, WIDTH>& operator*=(Solaire::Vector<T, WIDTH>& aVector, const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix) throw() {
	return aVector = aVector * aMatrix;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static inline Solaire::Matrix<T, WIDTH, HEIGHT> operator+(const Solaire::Matrix<T, WIDTH, HEIGHT>& aFirst, const Solaire::Matrix<T, WIDTH, HEIGHT>& aSecond) throw() {
	Solaire::Matrix<T, WIDTH, HEIGHT> tmp;
	Solaire::VectorMaths<T, WIDTH * HEIGHT>::Add(tmp.AsPointer(), aFirst.AsPointer(), aSecond.AsPointer());
	return aFirst;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static inline Solaire::Matrix<T, WIDTH, HEIGHT> operator-(const Solaire::Matrix<T, WIDTH, HEIGHT>& aFirst, const Solaire::Matrix<T, WIDTH, HEIGHT>& aSecond) throw() {
	Solaire::Matrix<T, WIDTH, HEIGHT> tmp;
	Solaire::VectorMaths<T, WIDTH * HEIGHT>::Sub(tmp.AsPointer(), aFirst.AsPointer(), aSecond.AsPointer());
	return aFirst;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT, const uint32_t WIDTH2, const uint32_t HEIGHT2, typename ENABLE = typename std::enable_if<WIDTH == HEIGHT2>::type>
static Solaire::Matrix<T, Solaire::Max(WIDTH, WIDTH2), Solaire::Max(HEIGHT, HEIGHT2)> operator*(const Solaire::Matrix<T, WIDTH, HEIGHT>& aFirst, const Solaire::Matrix<T, WIDTH2, HEIGHT2>& aSecond) throw() {
	enum : uint32_t {
		W = Solaire::Max(WIDTH, WIDTH2),
		H = Solaire::Max(HEIGHT, HEIGHT2)
	};

	Solaire::Matrix<T, W, H> tmp;

	for(uint32_t i = 0; i < W; ++i) {
		const Solaire::Vector<T, WIDTH> row = aFirst.GetRow(i);
		for (uint32_t j = 0; j < H; ++j) {
			tmp[i][j] = row.Dot(aSecond.GetColumn(j));
		}
	}

	return tmp;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static inline Solaire::Matrix<T, WIDTH, HEIGHT> operator+(const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix, typename Solaire::Matrix<T, WIDTH, HEIGHT>::ScalarPass aScalar) throw() {
	Solaire::Matrix<T, WIDTH, HEIGHT> tmp;
	Solaire::VectorMaths<T, WIDTH * HEIGHT>::Add(tmp.AsPointer(), aMatrix.AsPointer(), aScalar);
	return aMatrix;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static inline Solaire::Matrix<T, WIDTH, HEIGHT> operator-(const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix, typename Solaire::Matrix<T, WIDTH, HEIGHT>::ScalarPass aScalar) throw() {
	Solaire::Matrix<T, WIDTH, HEIGHT> tmp;
	Solaire::VectorMaths<T, WIDTH * HEIGHT>::Sub(tmp.AsPointer(), aMatrix.AsPointer(), aScalar);
	return aMatrix;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static inline Solaire::Matrix<T, WIDTH, HEIGHT> operator*(const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix, typename Solaire::Matrix<T, WIDTH, HEIGHT>::ScalarPass aScalar) throw() {
	Solaire::Matrix<T, WIDTH, HEIGHT> tmp;
	Solaire::VectorMaths<T, WIDTH * HEIGHT>::Mul(tmp.AsPointer(), aMatrix.AsPointer(), aScalar);
	return aMatrix;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static inline Solaire::Matrix<T, WIDTH, HEIGHT> operator/(const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix, typename Solaire::Matrix<T, WIDTH, HEIGHT>::ScalarPass aScalar) throw() {
	Solaire::Matrix<T, WIDTH, HEIGHT> tmp;
	Solaire::VectorMaths<T, WIDTH * HEIGHT>::Div(tmp.AsPointer(), aMatrix.AsPointer(), aScalar);
	return aMatrix;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static inline Solaire::Matrix<T, WIDTH, HEIGHT> operator+(typename Solaire::Matrix<T, WIDTH, HEIGHT>::ScalarPass aScalar, const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix) throw() {
	Solaire::Matrix<T, WIDTH, HEIGHT> tmp;
	Solaire::VectorMaths<T, WIDTH * HEIGHT>::Add(tmp.AsPointer(), aScalar, aMatrix.AsPointer());
	return aMatrix;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static inline Solaire::Matrix<T, WIDTH, HEIGHT> operator-(typename Solaire::Matrix<T, WIDTH, HEIGHT>::ScalarPass aScalar, const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix) throw() {
	Solaire::Matrix<T, WIDTH, HEIGHT> tmp;
	Solaire::VectorMaths<T, WIDTH * HEIGHT>::Sub(tmp.AsPointer(), aScalar, aMatrix.AsPointer());
	return aMatrix;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static inline Solaire::Matrix<T, WIDTH, HEIGHT> operator*(typename Solaire::Matrix<T, WIDTH, HEIGHT>::ScalarPass aScalar, const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix) throw() {
	Solaire::Matrix<T, WIDTH, HEIGHT> tmp;
	Solaire::VectorMaths<T, WIDTH * HEIGHT>::Mul(tmp.AsPointer(), aScalar, aMatrix.AsPointer());
	return aMatrix;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static inline Solaire::Matrix<T, WIDTH, HEIGHT> operator/(typename Solaire::Matrix<T, WIDTH, HEIGHT>::ScalarPass aScalar, const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix) throw() {
	Solaire::Matrix<T, WIDTH, HEIGHT> tmp;
	Solaire::VectorMaths<T, WIDTH * HEIGHT>::Div(tmp.AsPointer(), aScalar, aMatrix.AsPointer());
	return aMatrix;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static Solaire::Vector<T, HEIGHT> operator+(const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix, const Solaire::Vector<T, HEIGHT> aVector) throw() {
	Solaire::Vector<T, HEIGHT> tmp;
	for(uint32_t i = 0; i < HEIGHT) {
		tmp[i] = aMatrix.GetRow().Sum() + aVector[i];
	}
	return tmp;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static Solaire::Vector<T, HEIGHT> operator-(const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix, const Solaire::Vector<T, HEIGHT> aVector) throw() {
	Solaire::Vector<T, HEIGHT> tmp;
	for (uint32_t i = 0; i < HEIGHT) {
		tmp[i] = aMatrix.GetRow().Sum() - aVector[i];
	}
	return tmp;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static Solaire::Vector<T, WIDTH> operator*(const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix, const Solaire::Vector<T, WIDTH> aVector) throw() {
	Solaire::Vector<T, WIDTH> tmp;

	for(uint32_t i = 0; i < WIDTH) {
		tmp[i] = aMatrix.GetRow(i).Dot(aVector);
	}

	return tmp;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static Solaire::Vector<T, HEIGHT> operator+(const Solaire::Vector<T, HEIGHT> aVector, const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix) throw() {
	Solaire::Vector<T, HEIGHT> tmp;
	for(uint32_t i = 0; i < HEIGHT) {
		tmp[i] = aVector[i] + aMatrix.GetRow().Sum();
	}
	return tmp;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static Solaire::Vector<T, HEIGHT> operator-(const Solaire::Vector<T, HEIGHT> aVector, const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix) throw() {
	Solaire::Vector<T, HEIGHT> tmp;
	for(uint32_t i = 0; i < HEIGHT) {
		tmp[i] = aVector[i] - aMatrix.GetRow().Sum();
	}
	return tmp;
}

template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
static Solaire::Vector<T, HEIGHT> operator*(const Solaire::Vector<T, HEIGHT> aVector, const Solaire::Matrix<T, WIDTH, HEIGHT>& aMatrix) throw() {
	Solaire::Vector<T, HEIGHT> tmp;

	for (uint32_t i = 0; i < HEIGHT) {
		tmp[i] = aVector.Dot(aMatrix.GetColumn(i));
	}

	return tmp;
}

#endif
