#ifndef SOLAIRE_GL_RESOURCE_HPP
#define SOLAIRE_GL_RESOURCE_HPP

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
\file BufferResource.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 12th December 2015
Last Modified	: 12th December 2015
*/
	
#include "Buffer.hpp"
#include "..\..\Resource\Resource.hpp"

namespace Solaire {

	template<typename ID_TYPE, const bool READ, const bool WRITE, const bool PERSISSTENT = false>
	class BufferResource : public Resource<ID_TYPE, BufferImplementation<READ, WRITE, PERSISSTENT>> {
	private:
		Allocator& mAllocator;
		GLuint mSize;
	protected:
		// Inherited from Resource

		virtual SOLAIRE_EXPORT_CALL SharedAllocation<Type> CreateResource() const throw() {
			if(mSize == 0) return SharedAllocation<Type>();

			SharedAllocation<Type> tmp = mAllocator.SharedAllocate<Type>();

			if( ! tmp->Create()) return SharedAllocation<Type>();
			if( ! tmp->Allocate(mSize)) return SharedAllocation<Type>();

			return tmp;
		}
	public:
		BufferResource(Allocator& aAllocator, ID_TYPE aID) :
			Resource(aID),
			mAllocator(aAllocator),
			mSize(0)
		{}

		BufferResource(Allocator& aAllocator, ID_TYPE aID, const GLuint aSize) :
			Resource(aID),
			mAllocator(aAllocator),
			mSize(aSize)
		{}

		SOLAIRE_EXPORT_CALL ~BufferResource() {

		}
	};

	template<typename ID_TYPE, const bool READ, const bool PERSISSTENT = false>
	class BufferResource : public Resource<ID_TYPE, BufferImplementation<READ, true, PERSISSTENT>> {
	private:
		Allocator& mAllocator;
		GLuint mSize;
	protected:
		virtual SOLAIRE_EXPORT_CALL bool HasData() const throw() {return false;}
		virtual SOLAIRE_EXPORT_CALL GLuint GetDataSize() const throw() { return 0; }
		virtual SOLAIRE_EXPORT_CALL bool CopyData(void* const) const throw() { return false; }

		// Inherited from Resource
		virtual SOLAIRE_EXPORT_CALL SharedAllocation<Type> CreateResource() const throw() {
			const bool hasData = HasData();
			const GLuint _size = hasData ? GetDataSize() : 0;
			const GLuint size = _size > mSize ? _size : mSize;

			if(size == 0) return SharedAllocation<Type>();

			SharedAllocation<Type> tmp = mAllocator.SharedAllocate<Type>();

			if( ! tmp->Create()) return SharedAllocation<Type>();
			if( ! tmp->Allocate(size)) return SharedAllocation<Type>();
			if(hasData) {
				void* const mapping = tmp->MapWrite();
				if(mapping == nullptr) return SharedAllocation<Type>();
				if(! CopyData(mapping)) {
					tmp->Unmap();
					return SharedAllocation<Type>();
				}
				//! \todo Persistant buffers cannot be unmapped
				static_assert(! PERSISSTENT, "SolaireCPP : WRITE / PERSISSTENT BufferResource not implemented")
				if(! tmp->Unmap()) return SharedAllocation<Type>();
			}

			return tmp;
		}
	public:
		BufferResource(Allocator& aAllocator, ID_TYPE aID) :
			Resource(aID),
			mAllocator(aAllocator),
			mSize(0)
		{}

		BufferResource(Allocator& aAllocator, ID_TYPE aID, const GLuint aSize) :
			Resource(aID),
			mAllocator(aAllocator),
			mSize(aSize)
		{}

		virtual SOLAIRE_EXPORT_CALL ~BufferResource() {

		}
	};
}
#endif
