#ifndef SOLAIRE_INIT_HPP
#define SOLAIRE_INIT_HPP

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
	\file Init.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 22nd September 2015
	Last Modified	: 22nd September 2015
*/

// Misc
#define SOLAIRE_ERROR -1

// SOLAIRE_OS

#define SOLAIRE_WINDOWS 0
#define SOLAIRE_LINUX 1

// SOLAIRE_COMPILER

#define SOLAIRE_MSVC 0
#define SOLAIRE_GCC 1
#define SOLAIRE_MINGW 2
#define SOLAIRE_CLANG 3

// Detect environment

#include "Init/DetectCompiler.inl"
#include "Init/DetectOS.inl"

// Misc
#ifndef SOLAIRE_THREADLOCAL
    #ifdef SOLAIRE_DISABLE_MULTITHREADING
        #define SOLAIRE_THREADLOCAL
    #else
        #define SOLAIRE_THREADLOCAL thread_local
    #endif
#endif

#define SOLAIRE_ENABLE_IF(aMods, aCondition, aReturn)\
template<class RETURN_TYPE = aReturn>\
aMods typename std::enable_if<aCondition, RETURN_TYPE>::type

#include <cstdint>
#include <type_traits>


namespace Solaire{

    enum{
        PASS_BY_VALUE,
        PASS_BY_REFERENCE
    };

    template<class T, class Enable = void>
    struct TypeTraits{
        static constexpr int PassMode = PASS_BY_REFERENCE;
    };

    template<class T, const int Mode = TypeTraits<T>::PassMode>
    struct PassTypes{
        typedef void Type;
        typedef void ConstType;
    };

    ////

    template<class T>
    struct PassTypes<T, PASS_BY_VALUE>{
        typedef T Type;
        typedef const T ConstType;
    };

    template<class T>
    struct PassTypes<T, PASS_BY_REFERENCE>{
        typedef T& Type;
        typedef const T& ConstType;
    };

    ////

    template<class T>
    struct TypeTraits<T, typename std::enable_if<
		std::is_arithmetic<T>::value ||
		std::is_same<T, bool>::value ||
		std::is_same<T, char>::value ||
		std::is_pointer<T>::value
	>::type>{
        static constexpr int PassMode = PASS_BY_VALUE;
    };

	template<class T>
	using PassType = typename PassTypes<T>::Type;

	template<class T>
	using ConstPassType = typename PassTypes<T>::ConstType;

	#define SolaireRuntimeAssert(aCondition, aMessage) if(! (aCondition)) throw std::runtime_error(aMessage)
	#define SolaireStaticAssert static_assert(aCondition, aMessage)

	#ifndef SOLAIRE_DISABLE_MULTITHREADING
	#define SolaireSynchronized(aLock, aCode)\
		{\
			std::lock_guard<decltype(aLock)> _solaire_guard(aLock);\
			aCode\
		}
	#else
	#define SolaireSynchronized(aLock, aCode)\
		{\
			aCode\
		}
	#endif

}

#endif
