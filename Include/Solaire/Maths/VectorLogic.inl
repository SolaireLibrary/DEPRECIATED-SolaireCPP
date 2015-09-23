#ifndef SOLAIRE_MATHS_VECTOR_LOGIC_INL
#define SOLAIRE_MATHS_VECTOR_LOGIC_INL

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
	\file VectorLogic.hpp
	\brief Contains code for 4 dimentional vector maths.
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
#include <cstring>
#include <stdexcept>
#include "..\Core\Init.hpp"

#if (SOLAIRE_COMPILER == SOLAIRE_GCC || SOLAIRE_COMPILER == SOLAIRE_MINGW || SOLAIRE_COMPILER == SOLAIRE_CLANG) && ! defined(SOLAIRE_ENABLE_VECTOR_ATTRIBUTE)
    #define SOLAIRE_ENABLE_VECTOR_ATTRIBUTE
#endif

namespace Solaire{ namespace Maths{

    template<class T, const uint32_t LENGTH>
    struct VectorLogic{
        #ifdef SOLAIRE_ENABLE_VECTOR_ATTRIBUTE
            typedef T VectorType __attribute__ ((vector_size (sizeof(T) * LENGTH)));
            typedef uint32_t SwizzleType __attribute__ ((vector_size (sizeof(uint32_t) * LENGTH)));
        #endif

        static inline T* Copy(T* const aFirst, const T* const aSecond){
            std::memcpy(aFirst, aSecond, sizeof(T) * LENGTH);
            return aFirst;
        }

        // Vector / Vector

        static inline T* AddEq(T* const aFirst, const T* const aSecond){
            #ifdef SOLAIRE_ENABLE_VECTOR_ATTRIBUTE
                *reinterpret_cast<VectorType*>(aFirst) += *reinterpret_cast<const VectorType*>(aSecond);
            #else
                for(uint32_t i = 0; i < LENGTH; ++i){
                    aFirst[i] += aSecond[i];
                }
            #endif
            return aFirst;
        }

        static inline T* SubEq(T* const aFirst, const T* const aSecond){
            #ifdef SOLAIRE_ENABLE_VECTOR_ATTRIBUTE
                *reinterpret_cast<VectorType*>(aFirst) -= *reinterpret_cast<const VectorType*>(aSecond);
            #else
                for(uint32_t i = 0; i < LENGTH; ++i){
                    aFirst[i] -= aSecond[i];
                }
            #endif
            return aFirst;
        }

        static inline T* MulEq(T* const aFirst, const T* const aSecond){
            #ifdef SOLAIRE_ENABLE_VECTOR_ATTRIBUTE
                *reinterpret_cast<VectorType*>(aFirst) *= *reinterpret_cast<const VectorType*>(aSecond);
            #else
                for(uint32_t i = 0; i < LENGTH; ++i){
                    aFirst[i] *= aSecond[i];
                }
            #endif
            return aFirst;
        }

        static inline T* DivEq(T* const aFirst, const T* const aSecond){
            #ifdef SOLAIRE_ENABLE_VECTOR_ATTRIBUTE
                *reinterpret_cast<VectorType*>(aFirst) /= *reinterpret_cast<const VectorType*>(aSecond);
            #else
                for(uint32_t i = 0; i < LENGTH; ++i){
                    aFirst[i] /= aSecond[i];
                }
            #endif
            return aFirst;
        }

        static inline T* Add(T* const aOutput, const T* const aFirst, const T* const aSecond){
            return AddEq(Copy(aOutput, aFirst), aSecond);
        }

        static inline T* Sub(T* const aOutput, const T* const aFirst, const T* const aSecond){
            return SubEq(Copy(aOutput, aFirst), aSecond);
        }

        static inline void Mul(T* const aOutput, const T* const aFirst, const T* const aSecond){
            return MulEq( Copy(aOutput, aFirst), aSecond);
        }


        static inline T* Div(T* const aOutput, const T* const aFirst, const T* const aSecond){
            return DivEq(Copy(aOutput, aFirst), aSecond);
        }

        static inline bool Equals(const T* const aFirst, const T* const aSecond){
            return std::memcmp(aFirst, aSecond, LENGTH * sizeof(T)) == 0;
        }

        static inline bool NotEquals(const T* const aFirst, const T* const aSecond){
            return ! Equals(aFirst, aSecond);
        }

        // Vector / Scalar

        static inline T* AddEq(T* const aVector, const T aScalar){
            #ifdef SOLAIRE_ENABLE_VECTOR_ATTRIBUTE
                *reinterpret_cast<VectorType*>(aVector) += aScalar;
            #else
                for(uint32_t i = 0; i < LENGTH; ++i){
                    aVector[i] += aScalar;
                }
            #endif
            return aVector;
        }

        static inline T* SubEq(T* const aVector, const T aScalar){
            #ifdef SOLAIRE_ENABLE_VECTOR_ATTRIBUTE
                *reinterpret_cast<VectorType*>(aVector) -= aScalar;
            #else
                for(uint32_t i = 0; i < LENGTH; ++i){
                    aVector[i] -= aScalar;
                }
            #endif
            return aVector;
        }

        static inline T* MulEq(T* const aVector, const T aScalar){
            #ifdef SOLAIRE_ENABLE_VECTOR_ATTRIBUTE
                *reinterpret_cast<VectorType*>(aVector) *= aScalar;
            #else
                for(uint32_t i = 0; i < LENGTH; ++i){
                    aVector[i] *= aScalar;
                }
            #endif
            return aVector;
        }

        static inline T* DivEq(T* const aVector, const T aScalar){
            #ifdef SOLAIRE_ENABLE_VECTOR_ATTRIBUTE
                *reinterpret_cast<VectorType*>(aVector) /= aScalar;
            #else
                for(uint32_t i = 0; i < LENGTH; ++i){
                    aVector[i] /= aScalar;
                }
            #endif
            return aVector;
        }

        static inline T* VectorAdd(T* const aOutput, const T* const aVector, const T aScalar){
            return AddEq(Copy(aOutput, aVector), aScalar);
        }

        static inline T* VectorSub(T* const aOutput, const T* const aVector, const T aScalar){
            return SubEq(Copy(aOutput, aVector), aScalar);
        }

        static inline T* VectorMul(T* const aOutput, const T* const aVector, const T aScalar){
            return MulEq(Copy(aOutput, aVector), aScalar);
        }

        static inline T* VectorDiv(T* const aOutput, const T* const aVector, const T aScalar){
            return DivEq(Copy(aOutput, aVector), aScalar);
        }

        // Vector Misc

        static inline T Sum(const T* const aVector){
            T sum = static_cast<T>(0);
            for(uint32_t i = 0; i < LENGTH; ++i) sum += aVector[i];
            return sum;
        }

        static inline T Avg(const T* const aVector){
            return Sum(aVector) / static_cast<T>(LENGTH);
        }

        static T MagSq(const T* aVector){
            T tmp[LENGTH];
            return Sum(Mul(tmp, aVector, aVector));
        }

        static T Mag(const T* const aVector){
            return std::sqrt(MagSq(aVector));
        }

        static T Dot(const T* const aFirst, const T* const aSecond){
            T tmp[LENGTH];
            return Sum(Mul(tmp, aFirst, aSecond));
        }

        static T* Cross(T* const aOutput, const T* const aFirst, const T* const aSecond){
            if(LENGTH == 3){
                aOutput[0] = (aFirst[1] * aSecond[2]) - (aFirst[2] * aSecond[1]);
                aOutput[1] = (aFirst[2] * aSecond[0]) - (aFirst[0] * aSecond[2]);
                aOutput[2] = (aFirst[0] * aSecond[1]) - (aFirst[1] * aSecond[0]);
            }else{
                if(LENGTH == 3) throw std::runtime_error("Vector::Cross not implemented for vector of this length");
            }
            return aOutput;
        }

        static inline T* Swizzle(T* const aOutput, const T* const aVector, const uint32_t* const aSwizzle){
            //#ifdef SOLAIRE_ENABLE_VECTOR_ATTRIBUTE
            //    VectorType& output = *reinterpret_cast<VectorType*>(aOutput);
            //    const VectorType& vector = *reinterpret_cast<const VectorType*>(aVector);
            //    const SwizzleType& swizzle = *reinterpret_cast<const SwizzleType*>(aSwizzle);
            //
            //    output = __builtin_shuffle (vector, swizzle);
            //#else
                for(uint32_t i = 0; i < LENGTH; ++i){
                    aOutput[i] = aVector[aSwizzle[i]];
                }
            //#endif
            return aOutput;
        }

        static inline T* SwizzleEq(T* const aVector, const uint32_t* const aSwizzle){
            T tmp[LENGTH];
            return Swizzle(aVector, Copy(tmp, aVector), aSwizzle);
        }

        static inline T* NormaliseEq(T* const aVector){
            return DivEq(aVector, Mag(aVector));
        }

        static inline T* Normalise(T* const aOutput, const T* const aVector){
            return NormaliseEq(Copy(aOutput, aVector));
        }

        static inline T* Fill(T* const aVector, const T aScalar){
            for(uint32_t i = 0; i < LENGTH; ++i){
                aVector[i] = aScalar;
            }
            return aVector;
        }

        template<class T2>
        static T* Cast(T* const aFirst, const T2* const aSecond, const uint32_t aLength2, const T aDefault = static_cast<T>(0)){
            const size_t min = LENGTH < aLength2 ? LENGTH : aLength2;

            for(uint32_t i = 0; i < min; ++i){
                aFirst[i] = static_cast<T>(aSecond[i]);
            }

            for(uint32_t i = aLength2; i < LENGTH; ++i){
                aFirst[i] = aDefault;
            }

            return aFirst;
        }

        template<class T2, const uint32_t LENGTH2>
        static T* Cast(T* const aFirst, const T2* const aSecond, const T aDefault = static_cast<T>(0)){
            const size_t min = LENGTH < LENGTH2 ? LENGTH : LENGTH2;

            for(uint32_t i = 0; i < min; ++i){
                aFirst[i] = static_cast<T>(aSecond[i]);
            }

            for(uint32_t i = LENGTH2; i < LENGTH; ++i){
                aFirst[i] = aDefault;
            }

            return aFirst;
        }
    };
}}


#endif
