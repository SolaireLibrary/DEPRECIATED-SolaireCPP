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

#include "Solaire\Core\BitStream.hpp"

namespace Solaire{

	// BitStream

	BitStream::BitStream(void* aByte) throw() :
		mByte(static_cast<uint8_t*>(aByte)),
		mMask(1)
	{}

	BitStream::BitStream(void* aByte, const uint8_t aOffset) throw() :
		mByte(static_cast<uint8_t*>(aByte)),
		mMask(1 << aOffset)
	{}

	BitStream::~BitStream() throw() {

	}

	void BitStream::SetBit() throw() {
		*mByte |= mMask;
	}

	void BitStream::ClearBit() throw() {
		*mByte &= ~ mMask;
	}

	bool BitStream::CheckBit() const throw() {
		return (*mByte & mMask) != 0;
	}

	void BitStream::IncrementBit() throw() {
		if(mMask == 128) {
			mMask = 1;
			++mByte;
		}else{
			mMask <<= 1;
		}
	}

	void BitStream::DecrementBit() throw() {
		if(mMask == 1) {
			mMask = 128;
			--mByte;
		}else {
			mMask >>= 1;
		}
	}

	void BitStream::IncrementBit(uint32_t aOffset) throw() {
		while(aOffset >= 8) {
			++mByte;
			aOffset -= 8;
		}

		for(uint32_t i = 0; i < aOffset; ++i) {
			IncrementBit();
		}
	}

	void BitStream::DecrementBit(uint32_t aOffset) throw() {
		while(aOffset >= 8) {
			--mByte;
			aOffset -= 8;
		}

		for(uint32_t i = 0; i < aOffset; ++i) {
			DecrementBit();
		}
	}

	uint8_t BitStream::Offset() const throw() {
		switch(mMask)
		{
		case 1:
			return 0;
		case 2:
			return 1;
		case 4:
			return 2;
		case 8:
			return 3;
		case 16:
			return 4;
		case 32:
			return 5;
		case 64:
			return 6;
		case 128:
			return 7;
		default:
			return 0;
		}
	}

	void BitStream::ReadBits(void* aByte, uint32_t aCount) throw() {
		ReadBits(aByte, 0, aCount);
	}

	void BitStream::WriteBits(const void* aByte, uint32_t aCount) throw() {
		WriteBits(aByte, 0, aCount);
	}

	void BitStream::ReadBits(void* aByte, const uint8_t aOffset, uint32_t aCount) throw() {
		BitStream other(aByte, aOffset);

		if(mMask == 0 && other.mMask == 0){
			while(aCount > 8) {
				*other.mByte = *mByte;
				++mByte;
				++other.mByte;
				aCount -= 8;
			}
		}

		while(aCount != 0) {
			if(CheckBit()){
				other.SetBit();
			}else {
				other.ClearBit();
			}

			IncrementBit();
			other.IncrementBit();
			--aCount;
		}
	}

	void BitStream::WriteBits(const void* aByte, const uint8_t aOffset, uint32_t aCount) throw() {
		BitStream other(const_cast<void*>(aByte), aOffset);

		if(mMask == 0 && other.mMask == 0) {
			while(aCount > 8) {
				*mByte = *other.mByte;
				++mByte;
				++other.mByte;
				aCount -= 8;
			}
		}

		while(aCount != 0) {
			if(other.CheckBit()) {
				SetBit();
			}else {
				ClearBit();
			}

			IncrementBit();
			other.IncrementBit();
			--aCount;
		}
	}

	void BitStream::AlignByteBegin() throw() {
		mMask = 0;
	}

	void BitStream::AlignByteEnd() throw() {
		mMask = 128;
	}

}
