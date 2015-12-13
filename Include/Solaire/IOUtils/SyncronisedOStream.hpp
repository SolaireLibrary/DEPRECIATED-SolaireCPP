#ifndef SOLAIRE_SYNC_OSTREAM_HPP
#define SOLAIRE_SYNC_OSTREAM_HPP

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
	\file SyncronisedOStream.hpp
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

	class SyncronisedOStream : public OStream {
	private:
		SharedAllocation<SyncronisedBuffer> mBuffer;
	public:
		SyncronisedOStream(SharedAllocation<SyncronisedBuffer> aBuffer) :
			mBuffer(aBuffer)
		{}
	
		SOLAIRE_EXPORT_CALL ~SyncronisedOStream() throw() {
			mBuffer->mEnd = true;
			mBuffer->mReadCondition.notify_all();
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
	
		// Inherited from OStream
	
		bool SOLAIRE_EXPORT_CALL Write8(const uint8_t aValue) const throw() override {
			{
				std::unique_lock<std::mutex> lock(mBuffer->mLock);
				mBuffer->mData.push_back(aValue);
			}
			mBuffer->mReadCondition.notify_all();
			return true;
		}
	
		bool SOLAIRE_EXPORT_CALL Write16(const uint16_t aValue) const throw() override{
			{
				const uint8_t* const value = reinterpret_cast<const uint8_t*>(&aValue);
				std::unique_lock<std::mutex> lock(mBuffer->mLock);
				mBuffer->mData.push_back(value[0]);
				mBuffer->mData.push_back(value[1]);
			}
			mBuffer->mReadCondition.notify_all();
			return true;
		} 
	
		bool SOLAIRE_EXPORT_CALL Write32(const uint32_t aValue) const throw() override {
			{
				const uint8_t* const value = reinterpret_cast<const uint8_t*>(&aValue);
				std::unique_lock<std::mutex> lock(mBuffer->mLock);
				mBuffer->mData.push_back(value[0]);
				mBuffer->mData.push_back(value[1]);
				mBuffer->mData.push_back(value[2]);
				mBuffer->mData.push_back(value[3]);
			}
			mBuffer->mReadCondition.notify_all();
			return true;
		}
	
		bool SOLAIRE_EXPORT_CALL Write64(const uint64_t aValue) const throw() override {
			{
				const uint8_t* const value = reinterpret_cast<const uint8_t*>(&aValue);
				std::unique_lock<std::mutex> lock(mBuffer->mLock);
				mBuffer->mData.push_back(value[0]);
				mBuffer->mData.push_back(value[1]);
				mBuffer->mData.push_back(value[2]);
				mBuffer->mData.push_back(value[3]);
				mBuffer->mData.push_back(value[4]);
				mBuffer->mData.push_back(value[5]);
				mBuffer->mData.push_back(value[6]);
				mBuffer->mData.push_back(value[7]);
			}
			mBuffer->mReadCondition.notify_all();
			return true;
		} 
	};
}


#endif
