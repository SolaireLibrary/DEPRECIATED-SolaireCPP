#ifndef SOLAIRE_BIT_STREAM_HPP
#define SOLAIRE_BIT_STREAM_HPP

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
	\file BinaryContainer.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 19th November 2015
	Last Modified	: 19th November 2015
*/

#include <cstdint>

namespace Solaire{

	namespace Implementation {
		static constexpr int8_t BORROW_POSITION_TABLE[16][16] = {
			{ -1,	0,	1,	0,	2,	0,	1,	0,	3,	0,	1,	0,	2,	0,	1,	0 },
			{ -1,	-1,	1,	1,	2,	2,	1,	1,	3,	3,	1,	1,	2,	2,	1,	1 },
			{ -1,	0,	-1,	0,	2,	0,	2,	0,	3,	0,	3,	0,	2,	0,	2,	0 },
			{ -1,	-1,	-1,	-1,	2,	2,	2,	2,	3,	3,	3,	3,	2,	2,	2,	2 },
			{ -1,	0,	1,	0,	-1,	0,	1,	0,	3,	0,	1,	0,	3,	0,	1,	0 },
			{ -1,	-1,	1,	1,	-1,	-1,	1,	1,	3,	3,	1,	1,	3,	3,	1,	1 },
			{ -1,	0,	-1,	0,	-1,	0,	-1,	0,	3,	0,	3,	0,	3,	0,	3,	0 },
			{ -1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	3,	3,	3,	3,	3,	3,	3,	3 },
			{ -1,	0,	1,	0,	2,	0,	1,	0,	-1,	0,	1,	0,	2,	0,	1,	0 },
			{ -1,	-1,	1,	1,	2,	2,	1,	1,	-1,	-1,	1,	1,	2,	2,	1,	1 },
			{ -1,	0,	-1,	0,	2,	0,	2,	0,	-1,	0,	-1,	0,	2,	0,	2,	0 },
			{ -1,	-1,	-1,	-1,	2,	2,	2,	2,	-1,	-1,	-1,	-1,	2,	2,	2,	2 },
			{ -1,	0,	1,	0,	-1,	0,	1,	0,	-1,	0,	1,	0,	-1,	0,	1,	0 },
			{ -1,	-1,	1,	1,	-1,	-1,	1,	1,	-1,	-1,	1,	1,	-1,	-1,	1,	1 },
			{ -1,	0,	-1,	0,	-1,	0,	-1,	0,	-1,	0,	-1,	0,	-1,	0,	-1,	0 },
			{ -1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1 }
		};

		template<class T>
		static constexpr int8_t BorrowPosition(const T aValueA, const T aB) throw();

		template<>
		constexpr int8_t BorrowPosition<uint8_t>(const uint8_t aValueA, const uint8_t aValueB) throw() {
			return
				BORROW_POSITION_TABLE[aValueA & 15][aValueB & 15] != -1 ? BORROW_POSITION_TABLE[aValueA & 15][aValueB & 15] :
				BORROW_POSITION_TABLE[aValueA >> 4][aValueB >> 4] != -1 ? BORROW_POSITION_TABLE[aValueA >> 4][aValueB >> 4] + 4 :
				-1;
		}

		template<>
		constexpr int8_t BorrowPosition<uint16_t>(const uint16_t aValueA, const uint16_t aValueB) throw() {
			return
				BorrowPosition<uint8_t>(aValueA & UINT8_MAX, aValueB & UINT8_MAX) != -1 ? BorrowPosition<uint8_t>(aValueA & 255, aValueB & 255) :
				BorrowPosition<uint8_t>(aValueA >> 8, aValueB >> 8) != -1 ? BorrowPosition<uint8_t>(aValueA >> 8, aValueB >> 8) + 8 :
				-1;
		}

		template<>
		constexpr int8_t BorrowPosition<uint32_t>(const uint32_t aValueA, const uint32_t aValueB) throw() {
			return
				BorrowPosition<uint16_t>(aValueA & UINT16_MAX, aValueB & UINT16_MAX) != -1 ? BorrowPosition<uint16_t>(aValueA & UINT16_MAX, aValueB & UINT16_MAX) :
				BorrowPosition<uint16_t>(aValueA >> 16, aValueB >> 16) != -1 ? BorrowPosition<uint16_t>(aValueA >> 16, aValueB >> 16) + 16 :
				-1;
		}

		template<>
		constexpr int8_t BorrowPosition<uint64_t>(const uint64_t aValueA, const uint64_t aValueB) throw() {
			return
				BorrowPosition<uint32_t>(aValueA & UINT32_MAX, aValueB & UINT32_MAX) != -1 ? BorrowPosition<uint32_t>(aValueA & UINT32_MAX, aValueB & UINT32_MAX) :
				BorrowPosition<uint32_t>(aValueA >> 32L, aValueB >> 32L) != -1 ? BorrowPosition<uint32_t>(aValueA >> 32L, aValueB >> 32L) + 32 :
				-1;
		}
	}

	template<const uint32_t BYTES>
	class BinaryContainer {
	private:
		template<class T, const uint32_t COUNT>
		static void AddFn(void*& aFirst, const void*& aSecond, bool& aCarryBit) throw() {
			T*& first = reinterpret_cast<T*&>(aFirst);
			const T*& second = reinterpret_cast<const T*&>(aSecond);

			enum : T {
				MIN_BIT = 1,
				MAX_BIT = static_cast<T>(MIN_BIT) << static_cast<T>((sizeof(T) * 8) - 1),
			};

			for(uint32_t i = 0; i < COUNT; ++i) {
				T& a = first[i];
				const T& b = second[i];
				const bool tmp = aCarryBit;

				aCarryBit = (a & MAX_BIT) != 0 && (b & MAX_BIT) != 0;
				if(tmp) {
					if((a & MIN_BIT) == 0) {
						a |= MIN_BIT;
						a += b;
					}else if((a & MIN_BIT) == 0) {
						a += b | MIN_BIT;
					}else{
						a += a;
						a |= MIN_BIT;
					}
				}else{
					a += b;
				}
			}

			first += COUNT;
			second += COUNT;
		}

		template<class T, const uint32_t COUNT>
		static void OrFn(void*& aFirst, const void*& aSecond) throw() {
			T*& first = reinterpret_cast<T*&>(aFirst);
			const T*& second = reinterpret_cast<const T*&>(aSecond);

			for (uint32_t i = 0; i < COUNT; ++i) {
				first[i] |= second[i];
			}

			first += COUNT;
			second += COUNT;
		}

		template<class T, const uint32_t COUNT>
		static void AndFn(void*& aFirst, const void*& aSecond) throw() {
			T*& first = reinterpret_cast<T*&>(aFirst);
			const T*& second = reinterpret_cast<const T*&>(aSecond);

			for(uint32_t i = 0; i < COUNT; ++i) {
				first[i] &= second[i];
			}

			first += COUNT;
			second += COUNT;
		}

		template<class T, const uint32_t COUNT>
		static void XorFn(void*& aFirst, const void*& aSecond) throw() {
			T*& first = reinterpret_cast<T*&>(aFirst);
			const T*& second = reinterpret_cast<const T*&>(aSecond);

			for(uint32_t i = 0; i < COUNT; ++i) {
				first[i] ^= second[i];
			}

			first += COUNT;
			second += COUNT;
		}
	private:
		enum : uint32_t {
			DATA_64 = BYTES / 8,
			DATA_32 = BYTES - (DATA_64 * 8) >= 8 ? 1 : 0,
			DATA_16 = BYTES - (DATA_32 * 4) >= 4 ? 1 : 0,
			DATA_8 = BYTES - (DATA_16 * 2) > 2 ? 1 : 0,

			TOTAL_BYTES = (DATA_64 * 64) + (DATA_32 * 32) + (DATA_16 * 16) + (DATA_8 * 8)
		};
		static_assert(TOTAL_BYTES == BYTES, "SolaireCPP : BinaryContainer size was miscalculated")
	private:
		uint8_t mBytes[TOTAL_BYTES / 8];
	public:
		BinaryContainer() throw() {

		}

		BinaryContainer(const uint8_t aByte) throw() {
			for(uint32_t i = 0; i < TOTAL_BYTES; ++i) {
				mBytes[i] = aByte;
			}
		}

		BinaryContainer<BYTES>& operator+=(const BinaryContainer<BYTES>& aOther) throw() {
			void* thisBytes = mBytes;
			void* const otherBytes = aOther.mBytes;
			bool carryBit = false;

			AddFn<uint64_t, DATA_64>(thisBytes, otherBytes, carryBit);
			AddFn<uint32_t, DATA_32>(thisBytes, otherBytes, carryBit);
			AddFn<uint16_t, DATA_16>(thisBytes, otherBytes, carryBit);
			AddFn<uint8_t, DATA_8>(thisBytes, otherBytes, carryBit);

			return *this;
		}

		BinaryContainer<BYTES>& operator-=(const BinaryContainer<BYTES>& aOther) throw() {
			uint8_t* thisBytes = mBytes;
			uint8_t* const otherBytes = aOther.mBytes;

			for(int32_t i = TOTAL_BYTES; i >= 0; --i) {
				uint8_t& a = thisBytes[i];
				const uint8_t& b = otherBytes[i];

				const int8_t borrowPos = Implementation::BorrowPosition<uint8_t>(a, b);

				if(borrowPos != -1) {
					bool borrowed = false;
					//! borrow bit from current byte
					for(uint32_t j = borrowPos; j < 8; ++i) {
						const uint8_t mask = 1 << j;
						if((a & mask) != 0) {
							a -= mask;
							borrowed = true;
							break;
						}
					}

					// borrow bit from left byte
					if(! borrowed) {
						for (uint32_t j = i; j < TOTAL_BYTES; ++i) {
							uint8_t& byte = thisBytes[j];
							if (byte != 0) {
								--byte;
								borrowed = true;
								break;
							}
						}
					}
					
					if(borrowed) {
						//! \todo perform subtraction with borrowed bit
					}else{
						a -= b;
					}
				}else {
					a -= b;
				}
			}

			return *this;
		}

		BinaryContainer<BYTES>& operator|=(const BinaryContainer<BYTES>& aOther) throw() {
			void* thisBytes = mBytes;
			void* const otherBytes = aOther.mBytes;

			OrFn<uint64_t, DATA_64>(thisBytes, otherBytes);
			OrFn<uint32_t, DATA_32>(thisBytes, otherBytes);
			OrFn<uint16_t, DATA_16>(thisBytes, otherBytes);
			OrFn<uint8_t, DATA_8>(thisBytes, otherBytes);

			return *this;
		}

		BinaryContainer<BYTES>& operator&=(const BinaryContainer<BYTES>& aOther) throw() {
			void* thisBytes = mBytes;
			void* const otherBytes = aOther.mBytes;

			AndFn<uint64_t, DATA_64>(thisBytes, otherBytes);
			AndFn<uint32_t, DATA_32>(thisBytes, otherBytes);
			AndFn<uint16_t, DATA_16>(thisBytes, otherBytes);
			AndFn<uint8_t, DATA_8>(thisBytes, otherBytes);

			return *this;
		}

		BinaryContainer<BYTES>& operator^=(const BinaryContainer<BYTES>& aOther) throw() {
			void* thisBytes = mBytes;
			void* const otherBytes = aOther.mBytes;

			XorFn<uint64_t, DATA_64>(thisBytes, otherBytes);
			XorFn<uint32_t, DATA_32>(thisBytes, otherBytes);
			XorFn<uint16_t, DATA_16>(thisBytes, otherBytes);
			XorFn<uint8_t, DATA_8>(thisBytes, otherBytes);

			return *this;
		}

		bool operator==(const BinaryContainer<BYTES>& aOther) const throw() {
			return std::memcmp(mBytes, aOther.mBytes, TOTAL_BYTES) == 0;
		}

		bool operator!=(const BinaryContainer<BYTES>& aOther) const throw() {
			return std::memcmp(mBytes, aOther.mBytes, TOTAL_BYTES) != 0;
		}

		bool operator<(const BinaryContainer<BYTES>& aOther) const throw() {
			return std::memcmp(mBytes, aOther.mBytes, TOTAL_BYTES) < 0;
		}

		bool operator>(const BinaryContainer<BYTES>& aOther) const throw() {
			return std::memcmp(mBytes, aOther.mBytes, TOTAL_BYTES) > 0;
		}

		bool operator<=(const BinaryContainer<BYTES>& aOther) const throw() {
			return std::memcmp(mBytes, aOther.mBytes, TOTAL_BYTES) <= 0;
		}

		bool operator>=(const BinaryContainer<BYTES>& aOther) const throw() {
			return std::memcmp(mBytes, aOther.mBytes, TOTAL_BYTES) >= 0;
		}

		BinaryContainer<BYTES> operator+(const BinaryContainer<BYTES>& aOther) const throw() {
			return BinaryContainer<BYTES>(*this) += aOther;
		}

		BinaryContainer<BYTES> operator-(const BinaryContainer<BYTES>& aOther) const throw() {
			return BinaryContainer<BYTES>(*this) -= aOther;
		}

		BinaryContainer<BYTES> operator|(const BinaryContainer<BYTES>& aOther) const throw() {
			return BinaryContainer<BYTES>(*this) |= aOther;
		}

		BinaryContainer<BYTES> operator&(const BinaryContainer<BYTES>& aOther) const throw() {
			return BinaryContainer<BYTES>(*this) &= aOther;
		}

		BinaryContainer<BYTES> operator^(const BinaryContainer<BYTES>& aOther) const throw() {
			return BinaryContainer<BYTES>(*this) ^= aOther;
		}

	};
    
}


#endif
