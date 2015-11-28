#ifndef SOLAIRE_STREAM_STL_HPP
#define SOLAIRE_STREAM_STL_HPP

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
	\file StreamSTLWrappers.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 20th November 2015
	Last Modified	: 20th November 2015
*/

#include <iostream>
#include "..\IO\Stream.hpp"

namespace Solaire {

	class ReadStreamSTL : public ReadStream {
	private:
		std::istream& mStream;
	public:
		ReadStreamSTL(std::istream&);
		~ReadStreamSTL();

		// Inherited from StreamBase
		bool SOLAIRE_EXPORT_CALL SetOffset(const uint32_t) const throw() override;
		uint32_t SOLAIRE_EXPORT_CALL GetOffset() const throw() override;

		// Inherited from ReadStream

		uint32_t SOLAIRE_EXPORT_CALL Read(void* const, const uint32_t) throw() override;
		bool SOLAIRE_EXPORT_CALL End() const throw() override;
	};

	class WriteStreamSTL : public WriteStream {
	private:
		std::ostream& mStream;
	public:
		WriteStreamSTL(std::ostream&);
		~WriteStreamSTL();

		// Inherited from WriteStream

		bool SOLAIRE_EXPORT_CALL SetOffset(const uint32_t) const throw() override;
		uint32_t SOLAIRE_EXPORT_CALL GetOffset() const throw() override;

		// Inherited from ReadStream

		uint32_t SOLAIRE_EXPORT_CALL Write(const void* const, const uint32_t) throw() override;
		bool SOLAIRE_EXPORT_CALL Flush() throw() override;
	};

	//! \todo ReadStream -> std::istream wrapper
	//! \todo WriteStream -> std::ostream wrapper
}


#endif
