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
	Last Modified	: 5th January 2016
*/

#include "Vector.hpp"

namespace Solaire {

    //! \todo Implement Translation / Rotation / Scale helpers

	template<class T, const uint32_t WIDTH, const uint32_t HEIGHT>
	class Matrix{
	public:
		typedef T Scalar;
		enum : uint32_t {
		    Width = WIDTH,
		    Height = HEIGHT,
		    Min = Width < Height ? Width : Height,
		    Max = Width > Height ? Width : Height
		};
		typedef Vector<Scalar, Width> Row;
		typedef Vector<Scalar, Height> Column;
	private:
		Scalar mData[Width * Height];
	public:
	    Matrix() throw() {
	        enum : uint32_t{Size = Width * Height};
            for(uint32_t i = 0; i < Width; ++i) {
                for(uint32_t j = 0; j < Height; ++j) {
                    mData[i * Width + j] = static_cast<Scalar>(i == j ? 1 : 0);
                }
			}
		}

	    Matrix(const Scalar aScalar) throw() {
	        enum : uint32_t{Size = Width * Height};
            for(uint32_t i = 0; i < Width; ++i) {
                for(uint32_t j = 0; j < Height; ++j) {
                    mData[i * Width + j] = static_cast<Scalar>(i == j ? aScalar : 0);
                }
			}
		}

        Matrix(const std::initializer_list<Scalar> aElements) throw() {
	        enum : uint32_t{Size = Width * Height};
	        auto j = aElements.begin();
	        const uint32_t size = aElements.size();
	        const uint32_t min = Size <= size ? Size : size;
	        for(uint32_t i = 0; i < min; ++i){
                mData[i] = *j;
                ++j;
	        }

            for(uint32_t i = min; i < Size; ++i){
                mData[i] = static_cast<Scalar>(0);
	        }
	    }

	    // C++ Operators

		template<class Scalar2>
	    explicit operator Matrix<Scalar2, Width, Height>() const throw() {
	        enum : uint32_t{Size = Width * Height};
	        Matrix<Scalar2, Width, Height> tmp;
	        const Scalar* const ptr = tmp.Ptr();
	        for(uint32_t i = 0; i < Size; ++i){
                ptr[i] = static_cast<Scalar2>(mData[i]);
	        }
	        return tmp;
	    }

        template<const uint32_t W = Width, const uint32_t H = Height, class ENABLE = typename std::enable_if<W != H>::type>
	    explicit operator Matrix<Scalar, Height, Width>() const throw() {
	       return Transpose();
	    }

		const Scalar* operator[](const uint32_t aIndex) const throw() {
			return mData + (aIndex * Width);
		}

		Scalar* operator[](const uint32_t aIndex) throw() {
			return mData + (aIndex * Width);
		}

		bool operator==(const Matrix<Scalar, Width, Height>& aOther) const throw() {
			return std::memcmp(mData, aOther.mData, sizeof(Scalar) * Width * Height) == 0;
		}

		bool operator!=(const Matrix<Scalar, Width, Height>& aOther) const throw() {
			return std::memcmp(mData, aOther.mData, sizeof(Scalar) * Width * Height) != 0;
		}

		// Misc

		const Scalar* Ptr() const throw() {
			return mData;
		}

		Scalar* Ptr() throw() {
			return mData;
		}

		Row GetRow(const uint32_t aIndex) const throw() {
		    return *reinterpret_cast<const Row*>(mData + (aIndex * Width));
		}

		Column GetColumn(const uint32_t aIndex) const throw() {
		    Column tmp;
			for(uint32_t i = 0; i < HEIGHT; ++i) {
				tmp[i] = mData[i * aIndex];
			}
			return tmp;
		}

		void SetRow(const uint32_t aIndex, const Row aRow) throw() {
			std::memcpy(mData, aRow.Ptr(), sizeof(Scalar) * Width);
		}

		void SetColumn(const uint32_t aIndex, const Column aColumn) throw() {
			for(uint32_t i = 0; i < Height; ++i) {
				mData[i * aIndex] = aColumn[i];
			}
		}

		Matrix<Scalar, Height, Width> Inverse() const throw() {
			//! \todo Implement matrix inversion
			return Matrix<Scalar, Height, Width>();
		}

		Matrix<Scalar, Height, Width> Transpose() const throw() {
			Matrix<Scalar, Height, Width> tmp;
			for(uint32_t i = 0; i < Width; ++i) {
				tmp.SetColumn(i, GetRow(i));
			}
			return tmp;
		}
	};

    // Matrix / Matrix

    template<class Scalar, const uint32_t Width, const uint32_t Height>
    Matrix<Scalar, Width, Height>& operator+=(Matrix<Scalar, Width, Height>& aFirst, const Matrix<Scalar, Width, Height>& aSecond) {
        typedef Matrix<Scalar, Width, Height> MatrixType;
        typedef Vector<Scalar, Width * Height> VectorType;
        return reinterpret_cast<MatrixType&>(reinterpret_cast<VectorType&>(aFirst) += reinterpret_cast<const VectorType&>(aSecond));
    }

    template<class Scalar, const uint32_t Width, const uint32_t Height>
    Matrix<Scalar, Width, Height>& operator-=(Matrix<Scalar, Width, Height>& aFirst, const Matrix<Scalar, Width, Height>& aSecond) {
        typedef Matrix<Scalar, Width, Height> MatrixType;
        typedef Vector<Scalar, Width * Height> VectorType;
        return reinterpret_cast<MatrixType&>(reinterpret_cast<VectorType&>(aFirst) -= reinterpret_cast<const VectorType&>(aSecond));
    }

    template<class Scalar, const uint32_t Width, const uint32_t Height, class ENABLe = typename std::enable_if<Width == Height>::type>
    Matrix<Scalar, Width, Height>& operator*=(Matrix<Scalar, Width, Height>& aFirst, const Matrix<Scalar, Width, Height>& aSecond) {
        return aFirst = aFirst * aSecond;;
    }

    template<class Scalar, const uint32_t Width, const uint32_t Height>
    Matrix<Scalar, Width, Height> operator+(const Matrix<Scalar, Width, Height>& aFirst, const Matrix<Scalar, Width, Height>& aSecond) {
        return Matrix<Scalar, Width, Height>(aFirst) += aSecond;
    }

    template<class Scalar, const uint32_t Width, const uint32_t Height>
    Matrix<Scalar, Width, Height> operator-(const Matrix<Scalar, Width, Height>& aFirst, const Matrix<Scalar, Width, Height>& aSecond) {
        return Matrix<Scalar, Width, Height>(aFirst) -= aSecond;
    }

    template<class Scalar, const uint32_t Width, const uint32_t Height, const uint32_t MinSize = Width < Height ? Width : Height>
    Matrix<Scalar, MinSize, MinSize> operator*(const Matrix<Scalar, Width, Height>& aFirst, const Matrix<Scalar, Height, Width>& aSecond) {
        Matrix<Scalar, MinSize, MinSize> tmp;

        for(uint32_t i = 0; i < MinSize; ++i){
            const Vector<Scalar, Width>& srcRow = *reinterpret_cast<const Vector<Scalar, Width>*>(aFirst.Ptr() + Width * i);
            Scalar* const dstRow = tmp.Ptr() + Width * i;
            for(uint32_t j = 0; j < MinSize; ++j){
                dstRow[j] = srcRow.DotProduct(aSecond.GetColumn(j));
            }
        }

        return tmp;
    }

    // Matrix / Scalar

    template<class Scalar, const uint32_t Width, const uint32_t Height>
    Matrix<Scalar, Width, Height>& operator+=(Matrix<Scalar, Width, Height>& aFirst, const Scalar aSecond) {
        typedef Matrix<Scalar, Width, Height> MatrixType;
        typedef Vector<Scalar, Width * Height> VectorType;
        return reinterpret_cast<MatrixType&>(reinterpret_cast<VectorType&>(aFirst) += aSecond);
    }

    template<class Scalar, const uint32_t Width, const uint32_t Height>
    Matrix<Scalar, Width, Height>& operator-=(Matrix<Scalar, Width, Height>& aFirst, const Scalar aSecond) {
        typedef Matrix<Scalar, Width, Height> MatrixType;
        typedef Vector<Scalar, Width * Height> VectorType;
        return reinterpret_cast<MatrixType&>(reinterpret_cast<VectorType&>(aFirst) -= aSecond);
    }

    template<class Scalar, const uint32_t Width, const uint32_t Height>
    Matrix<Scalar, Width, Height>& operator*=(Matrix<Scalar, Width, Height>& aFirst, const Scalar aSecond) {
        typedef Matrix<Scalar, Width, Height> MatrixType;
        typedef Vector<Scalar, Width * Height> VectorType;
        return reinterpret_cast<MatrixType&>(reinterpret_cast<VectorType&>(aFirst) *= aSecond);
    }

    template<class Scalar, const uint32_t Width, const uint32_t Height>
    Matrix<Scalar, Width, Height>& operator/=(Matrix<Scalar, Width, Height>& aFirst, const Scalar aSecond) {
        typedef Matrix<Scalar, Width, Height> MatrixType;
        typedef Vector<Scalar, Width * Height> VectorType;
        return reinterpret_cast<MatrixType&>(reinterpret_cast<VectorType&>(aFirst) /= aSecond);
    }

    template<class Scalar, const uint32_t Width, const uint32_t Height>
    Matrix<Scalar, Width, Height> operator+(const Matrix<Scalar, Width, Height>& aFirst, const Scalar aSecond) {
        return Matrix<Scalar, Width, Height>(aFirst) += aSecond;
    }

    template<class Scalar, const uint32_t Width, const uint32_t Height>
    Matrix<Scalar, Width, Height> operator-(const Matrix<Scalar, Width, Height>& aFirst, const Scalar aSecond) {
        return Matrix<Scalar, Width, Height>(aFirst) -= aSecond;
    }

    template<class Scalar, const uint32_t Width, const uint32_t Height>
    Matrix<Scalar, Width, Height> operator*(const Matrix<Scalar, Width, Height>& aFirst, const Scalar aSecond) {
        return Matrix<Scalar, Width, Height>(aFirst) *= aSecond;
    }

    template<class Scalar, const uint32_t Width, const uint32_t Height>
    Matrix<Scalar, Width, Height> operator/(const Matrix<Scalar, Width, Height>& aFirst, const Scalar aSecond) {
        return Matrix<Scalar, Width, Height>(aFirst) /= aSecond;
    }

    // Scalar / Matrix

    template<class Scalar, const uint32_t Width, const uint32_t Height>
    Matrix<Scalar, Width, Height> operator+(const Scalar aFirst, const Matrix<Scalar, Width, Height>& aSecond) {
        typedef Matrix<Scalar, Width, Height> MatrixType;
        typedef Vector<Scalar, Width * Height> VectorType;
        return reinterpret_cast<MatrixType&>(aFirst + reinterpret_cast<const VectorType&>(aSecond));
    }

    template<class Scalar, const uint32_t Width, const uint32_t Height>
    Matrix<Scalar, Width, Height> operator-(const Scalar aFirst, const Matrix<Scalar, Width, Height>& aSecond) {
        typedef Matrix<Scalar, Width, Height> MatrixType;
        typedef Vector<Scalar, Width * Height> VectorType;
        return reinterpret_cast<MatrixType&>(aFirst - reinterpret_cast<const VectorType&>(aSecond));
    }

    template<class Scalar, const uint32_t Width, const uint32_t Height>
    Matrix<Scalar, Width, Height> operator*(const Scalar aFirst, const Matrix<Scalar, Width, Height>& aSecond) {
        typedef Matrix<Scalar, Width, Height> MatrixType;
        typedef Vector<Scalar, Width * Height> VectorType;
        return reinterpret_cast<MatrixType&>(aFirst * reinterpret_cast<const VectorType&>(aSecond));
    }

    template<class Scalar, const uint32_t Width, const uint32_t Height>
    Matrix<Scalar, Width, Height> operator/(const Scalar aFirst, const Matrix<Scalar, Width, Height>& aSecond) {
        typedef Matrix<Scalar, Width, Height> MatrixType;
        typedef Vector<Scalar, Width * Height> VectorType;
        return reinterpret_cast<MatrixType&>(aFirst / reinterpret_cast<const VectorType&>(aSecond));
    }

    // Matrix / Vector
    //! \todo Implement matrix / vector maths

    // Vector / Matrix
    //! \todo Implement vector / matrix maths

    // Typedefs

	template<class T>
	using Matrix2 = Matrix<T, 2, 2>;

	template<class T>
	using Matrix3 = Matrix<T, 3, 3>;

	template<class T>
	using Matrix4 = Matrix<T, 4, 4>;

	typedef Matrix2<uint8_t>	Matrix2U8;
	typedef Matrix2<int8_t>		Matrix2I8;
	typedef Matrix2<uint16_t>	Matrix2U16;
	typedef Matrix2<int16_t>	Matrix2I16;
	typedef Matrix2<uint32_t>	Matrix2U32;
	typedef Matrix2<int32_t>	Matrix2I32;
	typedef Matrix2<uint64_t>	Matrix2U64;
	typedef Matrix2<int64_t>	Matrix2I64;
	typedef Matrix2<float>		Matrix2F;
	typedef Matrix2<double>		Matrix2D;

	typedef Matrix3<uint8_t>	Matrix3U8;
	typedef Matrix3<int8_t>		Matrix3I8;
	typedef Matrix3<uint16_t>	Matrix3U16;
	typedef Matrix3<int16_t>	Matrix3I16;
	typedef Matrix3<uint32_t>	Matrix3U32;
	typedef Matrix3<int32_t>	Matrix3I32;
	typedef Matrix3<uint64_t>	Matrix3U64;
	typedef Matrix3<int64_t>	Matrix3I64;
	typedef Matrix3<float>		Matrix3F;
	typedef Matrix3<double>		Matrix3D;

	typedef Matrix4<uint8_t>	Matrix4U8;
	typedef Matrix4<int8_t>		Matrix4I8;
	typedef Matrix4<uint16_t>	Matrix4U16;
	typedef Matrix4<int16_t>	Matrix4I16;
	typedef Matrix4<uint32_t>	Matrix4U32;
	typedef Matrix4<int32_t>	Matrix4I32;
	typedef Matrix4<uint64_t>	Matrix4U64;
	typedef Matrix4<int64_t>	Matrix4I64;
	typedef Matrix4<float>		Matrix4F;
	typedef Matrix4<double>		Matrix4D;
}

#endif
