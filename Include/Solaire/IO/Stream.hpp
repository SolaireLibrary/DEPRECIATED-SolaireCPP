#ifndef SOLAIRE_STREAM_HPP
#define SOLAIRE_STREAM_HPP

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
	\file Stream.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 20th November 2015
	Last Modified	: 13th December 2015
*/

#include <cstdint>
#include "..\Core\ModuleHeader.hpp"

namespace Solaire {

	SOLAIRE_EXPORT_INTERFACE Stream {
	public:
		virtual bool SOLAIRE_EXPORT_CALL IsTraversable() const throw() = 0;
		virtual size_t SOLAIRE_EXPORT_CALL GetOffset() const throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL SetOffset(const size_t) throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL ToBegin() throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL ToEnd() throw() = 0;
		virtual SOLAIRE_EXPORT_CALL ~Stream() throw() {}
	};

	SOLAIRE_EXPORT_INTERFACE IStream : public Stream {
	public:	
		virtual bool SOLAIRE_EXPORT_CALL AtEnd() const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Read8(uint8_t&) const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Read16(uint16_t&) const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Read32(uint32_t&) const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Read64(uint64_t&) const throw() = 0;
		virtual SOLAIRE_EXPORT_CALL ~IStream() throw() {}
	
		SOLAIRE_FORCE_INLINE bool Read(void* const aData, const size_t aBytes) throw() {
			uint8_t* begin = static_cast<uint8_t*>(aData);
			const uint8_t* const end = begin + aBytes;
		
			while(end - begin >= 8) {
				if(! Read64(*reinterpret_cast<uint64_t*>(begin))) return false;
				begin += 8;
			}
		
			if(end - begin >= 4) {
				if(! Read32(*reinterpret_cast<uint32_t*>(begin))) return false;
				begin += 4;
			}
		
			if(end - begin >= 2) {
				if(! Read16(*reinterpret_cast<uint16_t*>(begin))) return false;
				begin += 2;
			}
		
			if(begin != end) {
				if(! Read8(*begin)) return false;
			}
		
			return true;
		}
	
		SOLAIRE_FORCE_INLINE IStream& operator>>(uint8_t& aValue) throw() {
			Read8(aValue);
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE IStream& operator>>(uint16_t& aValue) throw() {
			Read16(aValue);
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE IStream& operator>>(uint32_t& aValue) throw() {
			Read32(aValue);
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE IStream& operator>>(uint64_t& aValue) throw() {
			Read64(aValue);
			return *this;
		}
		SOLAIRE_FORCE_INLINE IStream& operator>>(int8_t& aValue) throw() {
			Read8(reinterpret_cast<uint8_t&>(aValue));
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE IStream& operator>>(int16_t& aValue) throw() {
			Read16(reinterpret_cast<uint16_t&>(aValue));
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE IStream& operator>>(int32_t& aValue) throw() {
			Read32(reinterpret_cast<uint32_t&>(aValue));
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE IStream& operator>>(int64_t& aValue) throw() {
			Read64(reinterpret_cast<uint64_t&>(aValue));
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE IStream& operator>>(char& aValue) throw() {
			Read8(reinterpret_cast<uint8_t&>(aValue));
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE IStream& operator>>(float& aValue) throw() {
			Read(&aValue, sizeof(float));
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE IStream& operator>>(double& aValue) throw() {
			Read(&aValue, sizeof(double));
			return *this;
		}
	};

	SOLAIRE_EXPORT_INTERFACE OStream : public Stream {
	public:
		virtual bool SOLAIRE_EXPORT_CALL Write8(const uint8_t) const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Write16(const uint16_t) const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Write32(const uint32_t) const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Write64(const uint64_t) const throw() = 0;
		virtual SOLAIRE_EXPORT_CALL ~OStream() throw() {}
	
		SOLAIRE_FORCE_INLINE bool Write(const void* const aData, const size_t aBytes) throw() {
			const uint8_t* begin = static_cast<const uint8_t*>(aData);
			const uint8_t* const end = begin + aBytes;
		
			while(end - begin >= 8) {
				if(! Write64(*reinterpret_cast<const uint64_t*>(begin))) return false;
				begin += 8;
			}
		
			if(end - begin >= 4) {
				if(! Write32(*reinterpret_cast<const uint32_t*>(begin))) return false;
				begin += 4;
			}
		
			if(end - begin >= 2) {
				if(! Write16(*reinterpret_cast<const uint16_t*>(begin))) return false;
				begin += 2;
			}
		
			if(begin != end) {
				if(! Write8(*begin)) return false;
			}
		
			return true;
		}
	
		SOLAIRE_FORCE_INLINE OStream& operator<<(const uint8_t aValue) throw() {
			Write8(aValue);
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE OStream& operator<<(const uint16_t aValue) throw() {
			Write16(aValue);
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE OStream& operator<<(const uint32_t aValue) throw() {
			Write32(aValue);
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE OStream& operator<<(const uint64_t aValue) throw() {
			Write64(aValue);
			return *this;
		}
		SOLAIRE_FORCE_INLINE OStream& operator<<(const int8_t aValue) throw() {
			Write8(*reinterpret_cast<const uint8_t*>(&aValue));
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE OStream& operator<<(const int16_t aValue) throw() {
			Write16(*reinterpret_cast<const uint16_t*>(&aValue));
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE OStream& operator<<(const int32_t aValue) throw() {
			Write32(*reinterpret_cast<const uint32_t*>(&aValue));
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE OStream& operator<<(const int64_t aValue) throw() {
			Write64(*reinterpret_cast<const uint64_t*>(&aValue));
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE OStream& operator<<(const char aValue) throw() {
			Write8(*reinterpret_cast<const uint8_t*>(&aValue));
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE OStream& operator<<(const float aValue) throw() {
			Write(&aValue, sizeof(float));
			return *this;
		}
	
		SOLAIRE_FORCE_INLINE OStream& operator<<(const double aValue) throw() {
			Write(&aValue, sizeof(double));
			return *this;
		}
	};

}


#endif
