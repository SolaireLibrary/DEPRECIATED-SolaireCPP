#ifndef SOLAIRE_VECTOR_MATHS_HPP
#define SOLAIRE_VECTOR_MATHS_HPP

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
	\file VectorMaths.hpp
	\brief Contains code for 4 dimentional vector maths.
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.1
	\date
	Created			: 23rd September 2015
	Last Modified	: 11th November 2015
*/

#include <cmath>
#include <cstdint>
//#include <cstring>
#include <stdexcept>
#include "..\Core\Init.hpp"

namespace Solaire{

	template<class T, const uint32_t LENGTH>
	class VectorMaths{
	public:
		typedef T Scalar;
		typedef typename TypeTraits<Scalar>::ConstPassType ScalarPass;
	public:
		static inline T* Copy(T* const aFirst, const T* const aSecond) {
			std::memcpy(aFirst, aSecond, sizeof(T) * LENGTH);
			return aFirst;
		}

		static inline bool Equals(const T* const aFirst, const T* const aSecond) {
			return std::memcmp(aFirst, aSecond, LENGTH * sizeof(T)) == 0;
		}

		static inline bool NotEquals(const T* const aFirst, const T* const aSecond) {
			return std::memcmp(aFirst, aSecond, LENGTH * sizeof(T)) != 0;
		}

		////

		static inline T* AddEq(T* const aFirst, const T* const aSecond) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aFirst[i] += aSecond[i];
			}
			return aFirst;
		}

		static inline T* SubEq(T* const aFirst, const T* const aSecond) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aFirst[i] -= aSecond[i];
			}
			return aFirst;
		}

		static inline T* MulEq(T* const aFirst, const T* const aSecond) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aFirst[i] *= aSecond[i];
			}
			return aFirst;
		}

		static inline T* DivEq(T* const aVector, const T* const aSecond) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aVector[i] /= aSecond[i];
			}
			return aVector;
		}

		static inline T* AddEq(T* const aVector, const ScalarPass aScalar) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aVector[i] += aScalar;
			}
			return aVector;
		}

		static inline T* SubEq(T* const aVector, const ScalarPass aScalar) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aVector[i] -= aScalar;
			}
			return aVector;
		}

		static inline T* MulEq(T* const aVector, const ScalarPass aScalar) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aVector[i] *= aScalar;
			}
			return aVector;
		}

		static inline T* DivEq(T* const aVector, const ScalarPass aScalar) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aVector[i] /= aScalar;
			}
			return aFirst;
		}

		////

		static inline T* Add(T* const aOutput, const T* const aFirst, const T* const aSecond) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aFirst[i] + aSecond[i];
			}
			return aOutput;
		}

		static inline T* Sub(T* const aOutput, const T* const aFirst, const T* const aSecond) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aFirst[i] - aSecond[i];
			}
			return aOutput;
		}

		static inline void Mul(T* const aOutput, const T* const aFirst, const T* const aSecond) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aFirst[i] * aSecond[i];
			}
			return aOutput;
		}

		static inline T* Div(T* const aOutput, const T* const aFirst, const T* const aSecond) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aFirst[i] / aSecond[i];
			}
			return aOutput;
		}

		static inline T* Add(T* const aOutput, const T* const aVector, const ScalarPass aScalar) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aVector[i] + aScalar;
			}
			return aOutput;
		}

		static inline T* Sub(T* const aOutput, const T* const aVector, const ScalarPass aScalar) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aVector[i] - aScalar;
			}
			return aOutput;
		}

		static inline void Mul(T* const aOutput, const T* const aVector, const ScalarPass aScalar) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aVector[i] * aScalar;
			}
			return aOutput;
		}

		static inline T* Div(T* const aOutput, const T* const aVector, const ScalarPass aScalar) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aVector[i] / aScalar;
			}
			return aOutput;
		}

		static inline T* Add(T* const aOutput, const ScalarPass aScalar, const T* const aVector) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aScalar + aVector[i];
			}
			return aOutput;
		}

		static inline T* Sub(T* const aOutput, const ScalarPass aScalar, const T* const aVector) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aScalar - aVector[i];
			}
			return aOutput;
		}

		static inline T* Mul(T* const aOutput, const ScalarPass aScalar, const T* const aVector) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aScalar * aVector[i];
			}
			return aOutput;
		}

		static inline T* Div(T* const aOutput, const ScalarPass aScalar, const T* const aVector) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aScalar / aVector[i];
			}
			return aOutput;
		}

		////

        static inline T* LerpEq(T* const aFirst, const T* const aSecond, const double aWeight) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aFirst[i] = static_cast<T>((1.0 - aWeight) * static_cast<double>(aFirst[i]) + aWeight * static_cast<double>(aSecond[i]));
			}
			return aFirst;
		}

		static inline T* Lerp(T* const aOutput, const T* const aFirst, const T* const aSecond, const double aWeight) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = static_cast<T>((1.0 - aWeight) * static_cast<double>(aFirst[i]) + aWeight * static_cast<double>(aSecond[i]));
			}
			return aOutput;
		}

		static inline T Sum(const T* const aVector) {
			T sum = static_cast<T>(0);
			for(uint32_t i = 0; i < LENGTH; ++i) sum += aVector[i];
			return sum;
		}

		static inline T Avg(const T* const aVector) {
			return Sum(aVector) / static_cast<T>(LENGTH);
		}

		static T MagSq(const T* aVector) {
			T tmp = static_cast<T>(0);
			for (uint32_t i = 0; i < LENGTH; ++i){
				sum += aVector[i] * aVector[i];
			}
			return tmp;
		}

		static double Mag(const T* const aVector) {
			return std::sqrt(static_cast<double>(MagSq(aVector)));
		}

		static T Dot(const T* const aFirst, const T* const aSecond) {
			T tmp = static_cast<T>(0);
			for (uint32_t i = 0; i < LENGTH; ++i) {
				sum += aFirst[i] * aSecond[i];
			}
			return tmp;
		}

		template<const uint32_t L = LENGTH, typename ENABLE = typename std::enable_if<LENGTH == 3>::type>
		static inline T* Cross(T* const aOutput, const T* const aFirst, const T* const aSecond) {
			aOutput[0] = (aFirst[1] * aSecond[2]) - (aFirst[2] * aSecond[1]);
			aOutput[1] = (aFirst[2] * aSecond[0]) - (aFirst[0] * aSecond[2]);
			aOutput[2] = (aFirst[0] * aSecond[1]) - (aFirst[1] * aSecond[0]);
		}
		
		static inline T* Swizzle(T* const aOutput, const T* const aVector, const uint32_t* const aSwizzle) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aVector[aSwizzle[i]];
			}
			return aOutput;
		}

		static inline T* SwizzleEq(T* const aVector, const uint32_t* const aSwizzle) {
			T tmp[LENGTH];
			return Swizzle(aVector, Copy(tmp, aVector), aSwizzle);
		}

		static inline T* NormaliseEq(T* const aVector) {
			return DivEq(aVector, Mag(aVector));
		}

		static inline T* Normalise(T* const aOutput, const T* const aVector) {
			return NormaliseEq(Copy(aOutput, aVector));
		}

		static inline T* Fill(T* const aVector, const ScalarPass aScalar) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aVector[i] = aScalar;
			}
			return aVector;
		} 
		
		template<class T2>
		static T* Cast(T* const aOutput, const T2* const aVector) {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = static_cast<T>(aSecond[i]);
			}

			return aVector;
		}
	};
}


#endif
