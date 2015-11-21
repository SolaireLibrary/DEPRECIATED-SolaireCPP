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
	Last Modified	: 20th November 2015
*/

#include <cstdint>
#include "..\Core\Init.hpp"

namespace Solaire {

	class SOLAIRE_EXPORT_API StreamBase {
	public:
		virtual bool SOLAIRE_EXPORT_CALL SetOffset(const uint32_t) const throw() = 0;
		virtual uint32_t SOLAIRE_EXPORT_CALL GetOffset() const throw() = 0;
	};

	class SOLAIRE_EXPORT_API ReadStream : public StreamBase {
	public:
		virtual uint32_t SOLAIRE_EXPORT_CALL Read(void* const, const uint32_t) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL End() const throw() = 0;

		inline uint8_t SOLAIRE_EXPORT_CALL Read8() throw() {
			uint8_t tmp;
			if(Read(&tmp, 1) != 1) return 0;
			return tmp;
		}

		inline uint16_t SOLAIRE_EXPORT_CALL Read16() throw() {
			uint16_t tmp;
			if(Read(&tmp, 2) != 2) return 0;
			return tmp;
		}

		inline uint32_t SOLAIRE_EXPORT_CALL Read32() throw() {
			uint32_t tmp;
			if(Read(&tmp, 4) != 4) return 0;
			return tmp;
		}

		inline uint64_t SOLAIRE_EXPORT_CALL Read64() throw() {
			uint64_t tmp;
			if(Read(&tmp, 8) != 8) return 0;
			return tmp;
		}
	};

	class SOLAIRE_EXPORT_API WriteStream : public StreamBase {
	public:
		virtual uint32_t SOLAIRE_EXPORT_CALL Write(const void* const, const uint32_t) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Flush() throw() = 0;

		inline bool SOLAIRE_EXPORT_CALL Write8(const uint8_t aValue) throw() {
			if(Write(&aValue, 1) == 1) return true;
			return false;
		}

		inline bool SOLAIRE_EXPORT_CALL Write16(const uint16_t aValue) throw() {
			if(Write(&aValue, 2) == 2) return true;
			return false;
		}

		inline bool SOLAIRE_EXPORT_CALL Write32(const uint32_t aValue) throw() {
			if(Write(&aValue, 4) == 4) return true;
			return false;
		}

		inline bool SOLAIRE_EXPORT_CALL Write64(const uint64_t aValue) throw() {
			if(Write(&aValue, 8) == 8) return true;
			return false;
		}
	};
}


#endif
