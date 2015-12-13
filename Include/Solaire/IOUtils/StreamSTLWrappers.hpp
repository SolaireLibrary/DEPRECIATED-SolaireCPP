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
	Last Modified	: 13th December 2015
*/

#include <iostream>
#include "..\IO\Stream.hpp"

namespace Solaire {

	template<class T>
	class IStreamSTL : public IStream {
	private:
		enum {
			BAD_STATE = std::ios::eofbit | std::ios::failbit | std::ios::badbit
		};
	private:
		T mStream;
	public:
		template<typename... PARAMS>
		IStreamSTL(PARAMS&& ...aParams) :
			mStream(aParams...)
		{}
	
		SOLAIRE_EXPORT_CALL ~IStreamSTL() throw() {
	
		}
	
		// Inherited from Stream	
	
		bool SOLAIRE_EXPORT_CALL IsTraversable() const throw() override {
			//! \todo Check if stream is traversable
			return true;
		}
	
		size_t SOLAIRE_EXPORT_CALL GetOffset() const throw() override {
			return mStream.tellg();
		}
	
		void SOLAIRE_EXPORT_CALL SetOffset(const size_t aOffset) throw() override {
			mStream.seekg(aOffset, mStream.beg);
		}
	
		void SOLAIRE_EXPORT_CALL ToBegin() throw() override {
			mStream.seekg(0, mStream.beg);
		}
	
		void SOLAIRE_EXPORT_CALL ToEnd() throw() override {
			mStream.seekg(0, mStream.end);
		}
	
		// Inherited from Istream 
	
		bool SOLAIRE_EXPORT_CALL AtEnd() const throw() override {
			return mStream.eof();
		}
	
		bool SOLAIRE_EXPORT_CALL Read8(uint8_t& aValue) const throw() override {
			mStream >> aValue;
			return (mStream.rdstate() & BAD_STATE) != 0;
		}
	
		bool SOLAIRE_EXPORT_CALL Read16(uint16_t& aValue) const throw() override{
			mStream >> aValue;
			return (mStream.rdstate() & BAD_STATE) != 0;
		} 
	
		bool SOLAIRE_EXPORT_CALL Read32(uint32_t& aValue) const throw() override {
			mStream >> aValue;
			return (mStream.rdstate() & BAD_STATE) != 0;
		}
	
		bool SOLAIRE_EXPORT_CALL Read64(uint64_t& aValue) const throw() override {
			mStream >> aValue;
			return (mStream.rdstate() & BAD_STATE) != 0;
		} 
	};

	template<class T>
	class OStreamSTL : public OStream {
	private:
		enum {
			BAD_STATE = std::ios::eofbit | std::ios::failbit | std::ios::badbit
		};
	private:
		T mStream;
	public:
		template<typename... PARAMS>
		OStreamSTL(PARAMS&& ...aParams) :
			mStream(aParams...)
		{}
	
		SOLAIRE_EXPORT_CALL ~OStreamSTL() throw() {
	
		}
	
		// Inherited from Stream	
	
		bool SOLAIRE_EXPORT_CALL IsTraversable() const throw() override {
			//! \todo Check if stream is traversable
			return true;
		}
	
		size_t SOLAIRE_EXPORT_CALL GetOffset() const throw() override {
			return mStream.tellp();
		}
	
		void SOLAIRE_EXPORT_CALL SetOffset(const size_t aOffset) throw() override {
			mStream.seekp(aOffset, mStream.beg);
		}
	
		void SOLAIRE_EXPORT_CALL ToBegin() throw() override {
			mStream.seekp(0, mStream.beg);
		}
	
		void SOLAIRE_EXPORT_CALL ToEnd() throw() override {
			mStream.seekp(0, mStream.end);
		}
	
		// Inherited from OStream
	
		bool SOLAIRE_EXPORT_CALL Write8(const uint8_t aValue) const throw() override {
			mStream << aValue;
			return (mStream.rdstate() & BAD_STATE) != 0;
		}
	
		bool SOLAIRE_EXPORT_CALL Write16(const uint16_t aValue) const throw() override{
			mStream << aValue;
			return (mStream.rdstate() & BAD_STATE) != 0;
		} 
	
		bool SOLAIRE_EXPORT_CALL Write32(const uint32_t aValue) const throw() override {
			mStream << aValue;
			return (mStream.rdstate() & BAD_STATE) != 0;
		}
	
		bool SOLAIRE_EXPORT_CALL Write64(const uint64_t aValue) const throw() override {
			mStream << aValue;
			return (mStream.rdstate() & BAD_STATE) != 0;
		} 
	};
	
}


#endif
