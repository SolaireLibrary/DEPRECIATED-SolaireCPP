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
	\file BitStream.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 13th November 2015
	Last Modified	: 13th November 2015
*/

#include <cstdint>
#include "Init.hpp"

namespace Solaire{

	class BitStream {
	private:
		uint8_t* mByte;
		uint8_t mMask;
	public:
		BitStream(void*) throw();
		BitStream(void*, const uint8_t) throw();
		~BitStream() throw();

		void SetBit() throw();
		void ClearBit() throw();
		bool CheckBit() const throw();

		void IncrementBit() throw();
		void DecrementBit() throw();

		void IncrementBit(uint32_t) throw();
		void DecrementBit(uint32_t) throw();

		uint8_t Offset() const throw();

		void ReadBits(void*, uint32_t) throw();
		void WriteBits(const void*, uint32_t) throw();

		void ReadBits(void*, const uint8_t, uint32_t) throw();
		void WriteBits(const void*, const uint8_t, uint32_t) throw();

		uint8_t Read8() throw();
		uint16_t Read16() throw();
		uint32_t Read32() throw();
		uint64_t Read64() throw();

		void Write8(const uint8_t) throw();
		void Write16(const uint16_t) throw();
		void Write32(const uint32_t) throw();
		void Write64(const uint64_t) throw();

		void AlignByteBegin() throw();
		void AlignByteEnd() throw();
	};
    
}


#endif
