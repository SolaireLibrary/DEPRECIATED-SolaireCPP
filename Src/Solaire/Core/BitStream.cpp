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
#include "Solaire\Core\Maths.hpp"

namespace Solaire{

	// BitStream

	BitStream::BitStream(void* aByte) throw() :
		mByte(static_cast<uint8_t*>(aByte)),
		mMask(BIT_0)
	{}

	BitStream::BitStream(void* aByte, const uint8_t aOffset) throw() :
		mByte(static_cast<uint8_t*>(aByte)),
		mMask(1 << aOffset)
	{}

	BitStream::~BitStream() throw() {

	}

	void BitStream::SetBit() throw() {
		*mByte |= mMask;
		IncrementBit();
	}

	void BitStream::ClearBit() throw() {
		*mByte &= ~ mMask;
		IncrementBit();
	}

	bool BitStream::CheckBit() const throw() {
		return (*mByte & mMask) != 0;
	}

	void BitStream::IncrementBit() throw() {
		if(mMask == BIT_7) {
			mMask = BIT_0;
			++mByte;
		}else{
			mMask <<= 1;
		}
	}

	void BitStream::DecrementBit() throw() {
		if(mMask == 1) {
			mMask = BIT_7;
			--mByte;
		}else {
			mMask >>= BIT_0;
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
		case BIT_0:
			return 0;
		case BIT_1:
			return 1;
		case BIT_2:
			return 2;
		case BIT_3:
			return 3;
		case BIT_4:
			return 4;
		case BIT_5:
			return 5;
		case BIT_6:
			return 6;
		case BIT_7:
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

		while(mMask != BIT_0 && aCount > 0) {
			if(CheckBit()) {
				other.SetBit();
			}else {
				other.ClearBit();
			}
			IncrementBit();
			--aCount;
		}

		while(aCount >= 64) {
			other.Write64(Read64());
			aCount -= 64;
		}
		
		if(aCount >= 32) {
			other.Write32(Read32());
			aCount -= 32;
		}
		
		if(aCount >= 16) {
			other.Write16(Read16());
			aCount -= 16;
		}
		
		if(aCount >= 8) {
			other.Write8(Read8());
			aCount -= 8;
		}

		while(aCount != 0) {
			if(CheckBit()) {
				other.SetBit();
			}else {
				other.ClearBit();
			}
			IncrementBit();
			--aCount;
		}
	}

	void BitStream::WriteBits(const void* aByte, const uint8_t aOffset, uint32_t aCount) throw() {
		BitStream other(const_cast<void*>(aByte), aOffset);

		while(mMask != BIT_0 && aCount > 0) {
			if(other.CheckBit()) {
				SetBit();
			}else {
				ClearBit();
			}
			other.IncrementBit();
			--aCount;
		}

		while(aCount >= 64) {
			Write64(other.Read64());
			aCount -= 64;
		}
		
		if(aCount >= 32) {
			Write32(other.Read32());
			aCount -= 32;
		}
		
		if(aCount >= 16) {
			Write16(other.Read16());
			aCount -= 16;
		}
		
		if(aCount >= 8) {
			Write8(other.Read8());
			aCount -= 8;
		}

		while(aCount != 0) {
			if(other.CheckBit()) {
				SetBit();
			}else {
				ClearBit();
			}
			other.IncrementBit();
			--aCount;
		}
	}

	void BitStream::AlignByteBegin() throw() {
		mMask = BIT_0;
	}

	void BitStream::AlignByteEnd() throw() {
		mMask = BIT_7;
	}

	uint8_t BitStream::Read8() throw() {
		uint8_t buf = 0;

		if(mMask == BIT_0) {
			buf = *mByte;
			++mByte;
		}else {
			for(uint32_t i = 0; i < 8; ++i) {
				buf |= static_cast<uint8_t>(CheckBit());
				IncrementBit();
				if(i != 7) buf <<= 1;
			}
		}

		return buf;
	}

	uint16_t BitStream::Read16() throw() {
		uint16_t buf = 0;

		if(mMask == BIT_0) {
			buf = *reinterpret_cast<const uint16_t*>(mByte);
			mByte += 2;
		}else {
			for(uint32_t i = 0; i < 16; ++i) {
				buf |= static_cast<uint16_t>(CheckBit());
				IncrementBit();
				if (i != 15) buf <<= 1;
			}
		}

		return buf;
	}

	uint32_t BitStream::Read32() throw() {
		uint32_t buf = 0;

		if(mMask == BIT_0) {
			buf = *reinterpret_cast<const uint32_t*>(mByte);
			mByte += 4;
		}else {
			for(uint32_t i = 0; i < 32; ++i) {
				buf |= static_cast<uint32_t>(CheckBit());
				IncrementBit();
				if (i != 31) buf <<= 1;
			}
		}

		return buf;
	}

	uint64_t BitStream::Read64() throw() {
		uint64_t buf = 0;

		if(mMask == BIT_0) {
			buf = *reinterpret_cast<const uint64_t*>(mByte);
			mByte += 8;
		}else {
			for(uint32_t i = 0; i < 64; ++i) {
				buf |= static_cast<uint16_t>(CheckBit());
				IncrementBit();
				if (i != 63) buf <<= 1;
			}
		}

		return buf;
	}

	void BitStream::Write8(const uint8_t aValue) throw() {
		if(mMask == BIT_0) {
			*mByte = aValue;
			++mByte;
		}else {
			uint8_t buf = aValue;
			for(uint32_t i = 0; i < 8; ++i) {
				if((aValue & 1) != 0) {
					SetBit();
				}else {
					ClearBit();
				}
				buf >>= 1;
			}
		}
	}

	void BitStream::Write16(const uint16_t aValue) throw() {
		if(mMask == BIT_0) {
			*reinterpret_cast<uint16_t*>(mByte) = aValue;
			mByte += 2;
		}else {
			uint16_t buf = aValue;
			for(uint32_t i = 0; i < 16; ++i) {
				if((aValue & 1) != 0) {
					SetBit();
				}else {
					ClearBit();
				}
				buf >>= 1;
			}
		}
	}

	void BitStream::Write32(const uint32_t aValue) throw() {
		if(mMask == BIT_0) {
			*reinterpret_cast<uint32_t*>(mByte) = aValue;
			mByte += 4;
		}else {
			uint32_t buf = aValue;
			for(uint32_t i = 0; i < 32; ++i) {
				if((aValue & 1) != 0) {
					SetBit();
				}else {
					ClearBit();
				}
				buf >>= 1;
			}
		}
	}

	void BitStream::Write64(const uint64_t aValue) throw() {
		if(mMask == BIT_0) {
			*reinterpret_cast<uint64_t*>(mByte) = aValue;
			mByte += 8;
		}else {
			uint64_t buf = aValue;
			for(uint32_t i = 0; i < 64; ++i) {
				if((aValue & 1) != 0) {
					SetBit();
				}else {
					ClearBit();
				}
				buf >>= 1L;
			}
		}
	}

}
