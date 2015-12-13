#ifndef SOLAIRE_SYNC_ISTREAM_HPP
#define SOLAIRE_SYNC_ISTREAM_HPP

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
	\file SyncronisedIStream.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 13th December 2015
	Last Modified	: 13th December 2015
*/

#include "SyncronisedBuffer.hpp"

namespace Solaire {

	class SyncronisedIStream : public IStream {
	private:
		SharedAllocation<const SyncronisedBuffer> mBuffer;
		mutable std::deque<uint8_t> mData;
	private:
		void WaitForWrite() {
			if(mBuffer->mEnd) return;
			std::unique_lock<std::mutex> lock(mBuffer->mLock);
			mBuffer->mReadCondition.wait_for(lock);
		}
	
		void GetWrite() {
			std::lock_guard<std::mutex> lock(mBuffer->mLock);
			for(const uint8_t byte : mBuffer->mData) mData.push_back(byte);
			mBuffer->mData.clear();
		}
	
		bool ReadByte(uint8_t& aByte) throw() {
			if(mOffset < mData.size()) {
				aByte = mData.front();
				mData.pop_front();
				return true;
			}else if(mBuffer->mEnd) {
				return false;
			}else {
				WaitForWrite();
				GetWrite();
				return ReadByte(aByte);
			}
		}
	public:
		SyncronisedIStream(SharedAllocation<const SyncronisedBuffer> aBuffer) :
			mBuffer(aBuffer),
			mOffset(0)
		{}
	
		SOLAIRE_EXPORT_CALL ~SyncronisedIStream() throw() {
	
		}
	
		// Inherited from Stream	
	
		bool SOLAIRE_EXPORT_CALL IsTraversable() const throw() override {
			return false;
		}
	
		size_t SOLAIRE_EXPORT_CALL GetOffset() const throw() override {
			return 0;
		}
	
		void SOLAIRE_EXPORT_CALL SetOffset(const size_t aOffset) throw() override {
		
		}
	
		void SOLAIRE_EXPORT_CALL ToBegin() throw() {
		
		}
	
		void SOLAIRE_EXPORT_CALL ToEnd() throw() {
	
		}
	
		// Inherited from Istream 
	
		bool SOLAIRE_EXPORT_CALL AtEnd() const throw() override {
			if(mBuffer->mEnd) {
				return mOffset >= mData.size();
			}else {
				if(mOffset < mData.size()) return false;
				WaitForWrite();
				GetWrite();
				return AtEnd();
			}
		}
	
		bool SOLAIRE_EXPORT_CALL Read8(uint8_t& aValue) const throw() override {
			return ReadByte(aValue);
		}
	
		bool SOLAIRE_EXPORT_CALL Read16(uint16_t& aValue) const throw() override {
			uint8_t* const value = reinterpret_cast<uint8_t*>(&aValue);
			if(! ReadByte(value[0]) return false;
			return ReadByte(value[1];
		} 
	
		bool SOLAIRE_EXPORT_CALL Read32(uint32_t& aValue) const throw() override {
			uint8_t* const value = reinterpret_cast<uint8_t*>(&aValue);
			if(! ReadByte(value[0]) return false;
			if(! ReadByte(value[1]) return false;
			if(! ReadByte(value[2]) return false;
			return ReadByte(value[3];
		}
	
		bool SOLAIRE_EXPORT_CALL Read64(uint64_t& aValue) const throw() override {
			uint8_t* const value = reinterpret_cast<uint8_t*>(&aValue);
			if(! ReadByte(value[0]) return false;
			if(! ReadByte(value[1]) return false;
			if(! ReadByte(value[2]) return false;
			if(! ReadByte(value[3]) return false;
			if(! ReadByte(value[4]) return false;
			if(! ReadByte(value[5]) return false;
			if(! ReadByte(value[6]) return false;
			return ReadByte(value[7];
		} 
	};
}


#endif
