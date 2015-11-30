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
#include <cstring>

namespace Solaire {

	class StreamBase{
	public:
		virtual bool SOLAIRE_EXPORT_CALL SetOffset(const uint32_t) const throw() = 0;
		virtual uint32_t SOLAIRE_EXPORT_CALL GetOffset() const throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL Destructor() throw() = 0;
	};

	class ReadStream : public StreamBase {
	public:
		virtual uint32_t SOLAIRE_EXPORT_CALL Read(void* const, const uint32_t) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL End() const throw() = 0;

		inline ReadStream& SOLAIRE_EXPORT_CALL operator>>(bool& aValue) throw() {
			Read(&aValue, sizeof(bool));
			return *this;
		}

		inline ReadStream& SOLAIRE_EXPORT_CALL operator>>(char& aValue) throw() {
			Read(&aValue, sizeof(char));
			return *this;
		}

		inline ReadStream& SOLAIRE_EXPORT_CALL operator>>(uint8_t& aValue) throw() {
			Read(&aValue, sizeof(uint8_t));
			return *this;
		}

		inline ReadStream& SOLAIRE_EXPORT_CALL operator>>(uint16_t& aValue) throw() {
			Read(&aValue, sizeof(uint16_t));
			return *this;
		}

		inline ReadStream& SOLAIRE_EXPORT_CALL operator>>(uint32_t& aValue) throw() {
			Read(&aValue, sizeof(uint32_t));
			return *this;
		}

		inline ReadStream& SOLAIRE_EXPORT_CALL operator>>(uint64_t& aValue) throw() {
			Read(&aValue, sizeof(uint64_t));
			return *this;
		}

		inline ReadStream& SOLAIRE_EXPORT_CALL operator>>(int8_t& aValue) throw() {
			Read(&aValue, sizeof(int8_t));
			return *this;
		}

		inline ReadStream& SOLAIRE_EXPORT_CALL operator>>(int16_t& aValue) throw() {
			Read(&aValue, sizeof(int16_t));
			return *this;
		}

		inline ReadStream& SOLAIRE_EXPORT_CALL operator>>(int32_t& aValue) throw() {
			Read(&aValue, sizeof(int32_t));
			return *this;
		}

		inline ReadStream& SOLAIRE_EXPORT_CALL operator>>(int64_t& aValue) throw() {
			Read(&aValue, sizeof(int64_t));
			return *this;
		}

		inline ReadStream& SOLAIRE_EXPORT_CALL operator>>(float& aValue) throw() {
			Read(&aValue, sizeof(float));
			return *this;
		}

		inline ReadStream& SOLAIRE_EXPORT_CALL operator>>(double& aValue) throw() {
			Read(&aValue, sizeof(double));
			return *this;
		}
	};

	class WriteStream : public StreamBase {
	public:
		virtual uint32_t SOLAIRE_EXPORT_CALL Write(const void* const, const uint32_t) throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL Flush() throw() = 0;

		inline bool SOLAIRE_EXPORT_CALL WriteCString(const char* const aString) throw() {
			const uint32_t length = std::strlen(aString);
			return Write(aString, length) == length;
		}

		inline WriteStream& SOLAIRE_EXPORT_CALL operator<<(const bool aValue) throw() {
			Write(&aValue, sizeof(bool));
			return *this;
		}

		inline WriteStream& SOLAIRE_EXPORT_CALL operator<<(const char aValue) throw() {
			Write(&aValue, sizeof(char));
			return *this;
		}

		inline WriteStream& SOLAIRE_EXPORT_CALL operator<<(const uint8_t aValue) throw() {
			Write(&aValue, sizeof(uint8_t));
			return *this;
		}

		inline WriteStream& SOLAIRE_EXPORT_CALL operator<<(const uint16_t aValue) throw() {
			Write(&aValue, sizeof(uint16_t));
			return *this;
		}

		inline WriteStream& SOLAIRE_EXPORT_CALL operator<<(const uint32_t aValue) throw() {
			Write(&aValue, sizeof(uint32_t));
			return *this;
		}

		inline WriteStream& SOLAIRE_EXPORT_CALL operator<<(const uint64_t aValue) throw() {
			Write(&aValue, sizeof(uint64_t));
			return *this;
		}

		inline WriteStream& SOLAIRE_EXPORT_CALL operator<<(const int8_t aValue) throw() {
			Write(&aValue, sizeof(int8_t));
			return *this;
		}

		inline WriteStream& SOLAIRE_EXPORT_CALL operator<<(const int16_t aValue) throw() {
			Write(&aValue, sizeof(int16_t));
			return *this;
		}

		inline WriteStream& SOLAIRE_EXPORT_CALL operator<<(const int32_t aValue) throw() {
			Write(&aValue, sizeof(int32_t));
			return *this;
		}

		inline WriteStream& SOLAIRE_EXPORT_CALL operator<<(const int64_t aValue) throw() {
			Write(&aValue, sizeof(int64_t));
			return *this;
		}

		inline WriteStream& SOLAIRE_EXPORT_CALL operator<<(const float aValue) throw() {
			Write(&aValue, sizeof(float));
			return *this;
		}

		inline WriteStream& SOLAIRE_EXPORT_CALL operator<<(const double aValue) throw() {
			Write(&aValue, sizeof(double));
			return *this;
		}
	};
}


#endif
