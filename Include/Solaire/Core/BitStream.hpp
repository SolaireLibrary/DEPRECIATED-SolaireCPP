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
	Created			: 13th September 2015
	Last Modified	: 13th September 2015
*/

#include <cstdint>

namespace Solaire{

	class BitStream {
	private:
		uint8_t* mByte;
		uint8_t mMask;
	public:
		BitStream(void*);
		BitStream(void*, const uint8_t);
		~BitStream();

		void SetBit();
		void ClearBit();
		bool CheckBit() const;

		void IncrementBit();
		void DecrementBit();

		void IncrementBit(uint32_t);
		void DecrementBit(uint32_t);

		uint8_t Offset() const;

		void ReadBits(void*, uint32_t);
		void WriteBits(const void*, uint32_t);

		void ReadBits(void*, const uint8_t, uint32_t);
		void WriteBits(const void*, const uint8_t, uint32_t);

		void AlignByteBegin();
		void AlignByteEnd();
	};
    
}


#endif
