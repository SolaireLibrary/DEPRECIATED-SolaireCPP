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

#include "Solaire\IOUtils\StreamSTLWrappers.hpp"

namespace Solaire {

	// ReadStreamSTL
	ReadStreamSTL::ReadStreamSTL(std::istream& aStream) :
		mStream(aStream)
	{}

	ReadStreamSTL::~ReadStreamSTL() {
		Destructor();
	}

	bool SOLAIRE_EXPORT_CALL ReadStreamSTL::SetOffset(const uint32_t aOffset) const throw() {
		mStream.seekg(aOffset, std::ios::beg);
		//! \todo check for bad seek
		return true;
	}

	uint32_t SOLAIRE_EXPORT_CALL ReadStreamSTL::GetOffset() const throw() {
		return static_cast<uint32_t>(mStream.tellg());
	}

	uint32_t SOLAIRE_EXPORT_CALL ReadStreamSTL::Read(void* const aDst, const uint32_t aBytes) throw() {
		mStream.read(static_cast<char*>(aDst), aBytes);
		//! \todo check how many bytes were read
		return aBytes;
	}

	bool SOLAIRE_EXPORT_CALL ReadStreamSTL::End() const throw() {
		return mStream.eof();
	}

	void SOLAIRE_EXPORT_CALL ReadStreamSTL::Destructor() throw() {

	}

	// WriteStreamSTL

	WriteStreamSTL::WriteStreamSTL(std::ostream& aStream) :
		mStream(aStream)
	{}

	WriteStreamSTL::~WriteStreamSTL() {
		Destructor();
	}

	bool SOLAIRE_EXPORT_CALL WriteStreamSTL::SetOffset(const uint32_t aOffset) const throw() {
		mStream.seekp(aOffset, std::ios::beg);
		//! \todo check for bad seek
		return true;
	}

	uint32_t SOLAIRE_EXPORT_CALL WriteStreamSTL::GetOffset() const throw() {
		return static_cast<uint32_t>(mStream.tellp());
	}

	uint32_t SOLAIRE_EXPORT_CALL WriteStreamSTL::Write(const void* const aSrc, const uint32_t aBytes) throw() {
		mStream.write(static_cast<const char*>(aSrc), aBytes);
		//! \todo check how many bytes were written
		return aBytes;
	}

	bool SOLAIRE_EXPORT_CALL WriteStreamSTL::Flush() throw() {
		mStream.flush();
		return true;
	}

	void SOLAIRE_EXPORT_CALL WriteStreamSTL::Destructor() throw() {

	}

}