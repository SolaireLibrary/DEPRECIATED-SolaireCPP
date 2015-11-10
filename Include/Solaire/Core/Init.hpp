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

	struct TypeTraitData{
		const uint16_t Size;
		const struct {
			// Solaire
			uint8_t IsSerialisable : 1;
		};
		const struct {
			// STL Primary
			uint16_t IsArray : 1;
			uint16_t IsClass : 1;
			uint16_t IsEnum : 1;
			uint16_t IsFunction : 1;
			uint16_t IsIntegral : 1;
			uint16_t IsLValue : 1;
			uint16_t IsRValue : 1;
			uint16_t IsMemberFunctionPointer : 1;
			uint16_t IsMemberObjectPointer : 1;
			uint16_t IsPointer : 1;
			uint16_t IsUnion : 1;
			uint16_t IsVoid : 1;
			uint16_t IsFloatingPoint : 1;
		};
		const struct {
			// STL Composite
			uint8_t IsArithmetic : 1;
			uint8_t IsCompound : 1;
			uint8_t IsFundamental : 1;
			uint8_t IsMemberPointer : 1;
			uint8_t IsReference : 1;
			uint8_t IsScalar : 1;
			uint8_t IsObject : 1;
		};
		const struct {
			// STL Properties
			uint16_t IsAbstract : 1;
			uint16_t IsConst : 1;
			uint16_t IsEmpty : 1;
			uint16_t IsLiteralType : 1;
			uint16_t IsPod : 1;
			uint16_t IsPolymorphic : 1;
			uint16_t IsSigned : 1;
			uint16_t IsUnsigned : 1;
			uint16_t IsStandardLayout : 1;
			uint16_t IsTrivial : 1;
			uint16_t IsVolatile : 1;
		};
		const struct {
			// STL Features
			uint16_t HasVirtualDestructor : 1;
			uint16_t IsAssignable : 1;
			uint16_t IsConstructable : 1;
			uint16_t IsCopyAssignable : 1;
			uint16_t IsCopyConstructable : 1;
			uint16_t IsMoveAssignable : 1;
			uint16_t IsMoveConstructable : 1;
			uint16_t IsDestructable : 1;
			uint16_t IsDefaultDestructable : 1;
			uint16_t IsTriviallyAssignable : 1;
			uint16_t IsTriviallyConstructable : 1;
			uint16_t IsTriviallyCopyAssignable : 1;
			uint16_t IsTriviallyCopyConstructable : 1;
			uint16_t IsTriviallyMoveAssignable : 1;
			uint16_t IsTriviallyMoveConstructable : 1;
			uint16_t IsTriviallyDestructable : 1;
			uint16_t IsTriviallyDefaultDestructable : 1;
			uint16_t IsNoThrowAssignable : 1;
			uint16_t IsNoThrowConstructable : 1;
			uint16_t IsNoThrowCopyAssignable : 1;
			uint16_t IsNoThrowCopyConstructable : 1;
			uint16_t IsNoThrowMoveAssignable : 1;
			uint16_t IsNoThrowMoveConstructable : 1;
			uint16_t IsNoThrowDestructable : 1;
			uint16_t IsNoThrowDefaultDestructable : 1;
		};

		constexpr TypeTraitData(
			const uint16_t aSize, 
			const uint32_t aFlagsSolaire, 
			const uint32_t aFlagsStlPrimary,
			const uint32_t aFlagsStlComposite,
			const uint32_t aFlagsStlProperties,
			const uint32_t aFlagsStlFeatures
		):
			Size(aSize),

			// Solaire
			IsSerialisable(					(aFlagsSolaire >> 0) & 1),

			// STL Primary
			IsArray(						(aFlagsStlPrimary >> 0) & 1),
			IsClass(						(aFlagsStlPrimary >> 1) & 1),
			IsEnum(							(aFlagsStlPrimary >> 2) & 1),
			IsFunction(						(aFlagsStlPrimary >> 3) & 1),
			IsIntegral(						(aFlagsStlPrimary >> 4) & 1),
			IsLValue(						(aFlagsStlPrimary >> 5) & 1),
			IsRValue(						(aFlagsStlPrimary >> 6) & 1),
			IsMemberFunctionPointer(		(aFlagsStlPrimary >> 7) & 1),
			IsMemberObjectPointer(			(aFlagsStlPrimary >> 8) & 1),
			IsPointer(						(aFlagsStlPrimary >> 9) & 1),
			IsUnion(						(aFlagsStlPrimary >> 10) & 1),
			IsVoid(							(aFlagsStlPrimary >> 11) & 1),
			IsFloatingPoint(				(aFlagsStlPrimary >> 12) & 1),

			// STL Composite
			IsArithmetic(					(aFlagsStlComposite >> 0) & 1),
			IsCompound(						(aFlagsStlComposite >> 1) & 1),
			IsFundamental(					(aFlagsStlComposite >> 2) & 1),
			IsMemberPointer(				(aFlagsStlComposite >> 3) & 1),
			IsReference(					(aFlagsStlComposite >> 4) & 1),
			IsScalar(						(aFlagsStlComposite >> 5) & 1),
			IsObject(						(aFlagsStlComposite >> 6) & 1),
			
			// STL Properties
			IsAbstract(						(aFlagsStlProperties >> 0) & 1),
			IsConst(						(aFlagsStlProperties >> 1) & 1),
			IsEmpty(						(aFlagsStlProperties >> 2) & 1),
			IsLiteralType(					(aFlagsStlProperties >> 3) & 1),
			IsPod(							(aFlagsStlProperties >> 4) & 1),
			IsPolymorphic(					(aFlagsStlProperties >> 5) & 1),
			IsSigned(						(aFlagsStlProperties >> 6) & 1),
			IsUnsigned(						(aFlagsStlProperties >> 7) & 1),
			IsStandardLayout(				(aFlagsStlProperties >> 8) & 1),
			IsTrivial(						(aFlagsStlProperties >> 9) & 1),
			IsVolatile(						(aFlagsStlProperties >> 10) & 1),

			// STL Features
			HasVirtualDestructor(			(aFlagsStlFeatures >> 0) & 1),
			IsAssignable(					(aFlagsStlFeatures >> 1) & 1),
			IsConstructable(				(aFlagsStlFeatures >> 2) & 1),
			IsCopyAssignable(				(aFlagsStlFeatures >> 3) & 1),
			IsCopyConstructable(			(aFlagsStlFeatures >> 4) & 1),
			IsMoveAssignable(				(aFlagsStlFeatures >> 5) & 1),
			IsMoveConstructable(			(aFlagsStlFeatures >> 6) & 1),
			IsDestructable(					(aFlagsStlFeatures >> 7) & 1),
			IsDefaultDestructable(			(aFlagsStlFeatures >> 8) & 1),
			IsTriviallyAssignable(			(aFlagsStlFeatures >> 9) & 1),
			IsTriviallyConstructable(		(aFlagsStlFeatures >> 10) & 1),
			IsTriviallyCopyAssignable(		(aFlagsStlFeatures >> 11) & 1),
			IsTriviallyCopyConstructable(	(aFlagsStlFeatures >> 12) & 1),
			IsTriviallyMoveAssignable(		(aFlagsStlFeatures >> 13) & 1),
			IsTriviallyMoveConstructable(	(aFlagsStlFeatures >> 14) & 1),
			IsTriviallyDestructable(		(aFlagsStlFeatures >> 15) & 1),
			IsTriviallyDefaultDestructable(	(aFlagsStlFeatures >> 16) & 1),
			IsNoThrowAssignable(			(aFlagsStlFeatures >> 17) & 1),
			IsNoThrowConstructable(			(aFlagsStlFeatures >> 18) & 1),
			IsNoThrowCopyAssignable(		(aFlagsStlFeatures >> 19) & 1),
			IsNoThrowCopyConstructable(		(aFlagsStlFeatures >> 20) & 1),
			IsNoThrowMoveAssignable(		(aFlagsStlFeatures >> 21) & 1),
			IsNoThrowMoveConstructable(		(aFlagsStlFeatures >> 22) & 1),
			IsNoThrowDestructable(			(aFlagsStlFeatures >> 23) & 1),
			IsNoThrowDefaultDestructable(	(aFlagsStlFeatures >> 24) & 1)
		{}

		template<class T>
		static constexpr TypeTraitData GetData(const bool aSerialiseable){
			return TypeTraitData(
				sizeof(T),

				// Solaire
				(static_cast<uint32_t>(aSerialiseable)								<< 0),

				// STL Primary
				(static_cast<uint32_t>(std::is_array<T>::value)						<< 0) |
				(static_cast<uint32_t>(std::is_class<T>::value)						<< 1) |
				(static_cast<uint32_t>(std::is_enum<T>::value)						<< 2) |
				(static_cast<uint32_t>(std::is_function<T>::value)					<< 3) |
				(static_cast<uint32_t>(std::is_integral<T>::value)					<< 4) |
				(static_cast<uint32_t>(std::is_lvalue_reference<T>::value)			<< 5) |
				(static_cast<uint32_t>(std::is_rvalue_reference<T>::value)			<< 6) |
				(static_cast<uint32_t>(std::is_member_function_pointer<T>::value)	<< 7) |
				(static_cast<uint32_t>(std::is_member_object_pointer<T>::value)		<< 8) |
				(static_cast<uint32_t>(std::is_pointer<T>::value)					<< 9) |
				(static_cast<uint32_t>(std::is_union<T>::value)						<< 10) |
				(static_cast<uint32_t>(std::is_void<T>::value)						<< 11) |
				(static_cast<uint32_t>(std::is_floating_point<T>::value)			<< 12),

				// STL Composite
				(static_cast<uint32_t>(std::is_arithmetic<T>::value)				<< 0) |
				(static_cast<uint32_t>(std::is_compound<T>::value)					<< 1) |
				(static_cast<uint32_t>(std::is_fundamental<T>::value)				<< 2) |
				(static_cast<uint32_t>(std::is_member_pointer<T>::value)			<< 3) |
				(static_cast<uint32_t>(std::is_reference<T>::value)					<< 4) |
				(static_cast<uint32_t>(std::is_scalar<T>::value)					<< 5) |
				(static_cast<uint32_t>(std::is_object<T>::value)					<< 6),

				// STL Properties
				/*uint64_t IsAbstract : 1;
				uint64_t IsConst : 1;
				uint64_t IsEmpty : 1;
				uint64_t IsLiteralType : 1;
				uint64_t IsPod : 1;
				uint64_t IsPolymorphic : 1;
				uint64_t IsSigned : 1;
				uint64_t IsUnsigned : 1;
				uint64_t IsStandardLayout : 1;
				uint64_t IsTrivial : 1;
				uint64_t IsVolatile : 1;*/
				0,
				
				// STL Features
				/*uint64_t HasVirtualDestructor : 1;
			uint64_t IsAssignable : 1;
			uint64_t IsConstructable : 1;
			uint64_t IsCopyAssignable : 1;
			uint64_t IsCopyConstructable : 1;
			uint64_t IsMoveAssignable : 1;
			uint64_t IsMoveConstructable : 1;
			uint64_t IsDestructable : 1;
			uint64_t IsDefaultDestructable : 1;
			uint64_t IsTriviallyAssignable : 1;
			uint64_t IsTriviallyConstructable : 1;
			uint64_t IsTriviallyCopyAssignable : 1;
			uint64_t IsTriviallyCopyConstructable : 1;
			uint64_t IsTriviallyMoveAssignable : 1;
			uint64_t IsTriviallyMoveConstructable : 1;
			uint64_t IsTriviallyDestructable : 1;
			uint64_t IsTriviallyDefaultDestructable : 1;
			uint64_t IsNoThrowAssignable : 1;
			uint64_t IsNoThrowConstructable : 1;
			uint64_t IsNoThrowCopyAssignable : 1;
			uint64_t IsNoThrowCopyConstructable : 1;
			uint64_t IsNoThrowMoveAssignable : 1;
			uint64_t IsNoThrowMoveConstructable : 1;
			uint64_t IsNoThrowDestructable : 1;
			uint64_t IsNoThrowDefaultDestructable : 1; */
				0
			);
		}
	};

    template<class T, class Enable = void>
    struct TypeTraits{
		typedef T Type;
		typedef const T ConstType;
		typedef T& PassType;
		typedef const T& ConstPassType;

		static constexpr TypeTraitData GetData() {
			return TypeTraitData::GetData<T>(false);
		}
    };

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

#include "Init.inl"

#endif
