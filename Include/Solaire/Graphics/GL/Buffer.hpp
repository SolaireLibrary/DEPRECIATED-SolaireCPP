#ifndef SOLAIRE_GL_BUFFER_HPP
#define SOLAIRE_GL_BUFFER_HPP

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
\file Object.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 9th December 2015
Last Modified	: 9th December 2015
*/

#define SOLAIRE_GL_COPY_READ_BUFFER_VER 3,1
#define SOLAIRE_GL_COPY_WRITE_BUFFER_VER 3,1
#define SOLAIRE_GL_TEXTURE_BUFFER_VER 3,1
#define SOLAIRE_GL_UNIFORM_BUFFER_VER 4,0
#define SOLAIRE_GL_DRAW_INDIRECT_BUFFER_VER 4,1
#define SOLAIRE_GL_ATOMIC_COUNTER_BUFFER_VER 4,2
#define SOLAIRE_GL_GL_DISPATCH_INDIRECT_BUFFER_VER 4,3
#define SOLAIRE_GL_SHADER_STORAGE_BUFFER_VER 4,3
#define SOLAIRE_GL_QUERY_BUFFER_VER 4,4
#define SOLAIRE_GL_ARRAY_BUFFER_VER 3,0
#define SOLAIRE_GL_ELEMENT_ARRAY_BUFFER_VER 3,0
#define SOLAIRE_GL_PIXEL_PACK_BUFFER_VER 3,0
#define SOLAIRE_GL_PIXEL_UNPACK_BUFFER_VER 3,0
#define SOLAIRE_GL_TRANSFORM_FEEDBACK_BUFFER_VER 3,0
	
#include <map>
#include <vector>
#include "GLHeader.hpp"
#include "BufferImplementation.hpp"

namespace Solaire {

	//! \todo Check if buffer is mapped

	class Buffer {
	protected:
		enum : GLenum {
			#if SOLAIRE_GL_VER_GTE(4,2)
				PRIMARY_BUFFER = GL_COPY_WRITE_BUFFER,
				PRIMARY_BUFFER_BINDING = GL_COPY_WRITE_BUFFER_BINDING,
				SECONDARY_BUFFER = GL_COPY_READ_BUFFER,
				SECONDARY_BUFFER_BINDING = GL_COPY_READ_BUFFER_BINDING
			#endif
			#if SOLAIRE_GL_VER_GTE(3,0) && SOLAIRE_GL_VER_LT(4,2)
				PRIMARY_BUFFER = GL_ARRAY_BUFFER,
				PRIMARY_BUFFER_BINDING = GL_ARRAY_BUFFER_BINDING,
				SECONDARY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
				SECONDARY_BUFFER_BINDING = GL_ELEMENT_ARRAY_BUFFER_BINDING
			#endif
			#if SOLAIRE_GL_VER_LT(3,0)
				PRIMARY_BUFFER = GL_INVALID_ENUM,
				PRIMARY_BUFFER_BINDING = GL_INVALID_ENUM,
				SECONDARY_BUFFER = GL_INVALID_ENUM,
				SECONDARY_BUFFER_BINDING = GL_INVALID_ENUM
			#endif
		};
	public:
		typedef GLBufferImplementation::BufferTarget Target;

		enum SynchronisationMode : GLbitfield  {
			SYNCHRONISED 			= 0,
			UNSYNCHRONISED 			= GL_MAP_UNSYNCHRONIZED_BIT
		};
		
		enum InvalidationMode : GLbitfield  {
			NO_INVALIDATION 		= 0,
			INVALIDATE_BUFFER 		= GL_MAP_INVALIDATE_BUFFER_BIT,
			INVALIDATE_RANGE 		= GL_MAP_INVALIDATE_RANGE_BIT
		};
	protected:
		GLID mID;
		GLuint mSize;
		#if SOLAIRE_GL_VER_LT(4,4)
			Target mMapBinding;
		#endif
		bool mIsMapped;
	protected:
		#if SOLAIRE_GL_VER_GTE(4,4)
			virtual GLbitfield SOLAIRE_EXPORT_CALL GetCreationFlags() const throw() = 0;
		#endif
		#if SOLAIRE_GL_VER_GTE(3,0) && SOLAIRE_GL_VER_LT(4,4)
			virtual GLenum SOLAIRE_EXPORT_CALL GetUsage() const throw() = 0;
		#endif

		Buffer(const Buffer& aOther) :
			mID(GLID::NULL_ID),
			mSize(aOther.mSize),
			#if SOLAIRE_GL_VER_LT(4,4)
				mMapBinding(Target::INVALID_TARGET),
			#endif
			mIsMapped(false)
		{}

		Buffer(Buffer&& aOther) :
			mID(aOther.mID),
			mSize(aOther.mSize),
			#if SOLAIRE_GL_VER_LT(4,4)
				mMapBinding(aOther.mMapBinding),
			#endif
			mIsMapped(aOther.mIsMapped)
		{
			aOther.mID = GLID::NULL_ID;
			aOther.mSize = 0;
			#if SOLAIRE_GL_VER_LT(4,4)
				aOther.mMapBinding = Target::INVALID_TARGET;
			#endif
			aOther.mIsMapped = false;
		}

		Buffer& operator=(Buffer&& aOther) {
			std::swap(mID, aOther.mID);
			std::swap(mSize, aOther.mSize);
			#if SOLAIRE_GL_VER_LT(4,4)
				std::swap(mMapBinding, aOther.mMapBinding);
			#endif
			std::swap(mIsMapped, aOther.mIsMapped);
			return *this;
		}

		Buffer& operator=(const Buffer& aOther) {
			if(mSize < aOther.mSize) {
				if(mID != GLID::NULL_ID) {
					Destroy();
					mSize = aOther.mSize;
					Create();
				}else {
					mSize = aOther.mSize;
				}
			}
			return *this;
		}
	public:
		Buffer() :
			mID(GLID::NULL_ID),
			mSize(0),
			#if SOLAIRE_GL_VER_LT(4,4)
				mMapBinding(Target::INVALID_TARGET),
			#endif
			mIsMapped(false)
		{
			GLBufferImplementation::InitialiseBufferData();
		}
	
		virtual SOLAIRE_EXPORT_CALL ~Buffer() throw() {
			Destroy();
		}
	
		GLuint Size() const throw() {
			return mSize;
		}
	
		bool Bind(const Target aTarget) {
			if(mID == GLID::NULL_ID) return false;
			return GLBufferImplementation::Bind(aTarget, *this);
		}

		bool Unbind(const Target aTarget) {
			if (mID == GLID::NULL_ID) return false;
			return GLBufferImplementation::Unbind(aTarget, *this);
		}
	
		GLID GetID() const throw() {
			return mID;
		}
	
		bool Create() throw() {
			if(mID != GLID::NULL_ID) return false;
			glGenBuffers(1, reinterpret_cast<GLuint*>(&mID));
			if(mID == GLID::NULL_ID) return false;
			return true;
		}
		
		bool Allocate(const GLuint aSize) throw() {
			if(mID != GLID::NULL_ID) return false;
			mSize = aSize;

			#if SOLAIRE_GL_VER_GTE(4,5)
				glNamedBufferStorage((GLint) mID, mSize, nullptr, GetCreationFlags());
				return true;
			#endif
			#if SOLAIRE_GL_VER_GTE(3,0) && SOLAIRE_GL_VER_LT(4,5)
				GLint previous = static_cast<GLint>(GLID::NULL_ID);
				glGetIntegerv(PRIMARY_BUFFER_BINDING, &previous);
				glBindBuffer(PRIMARY_BUFFER, (GLint) mID);
				#if SOLAIRE_GL_VER_GTE(4,4)
					glBufferStorage(PRIMARY_BUFFER, mSize, nullptr, GetCreationFlags());
				#else
					glBufferData(PRIMARY_BUFFER, mSize, nullptr, GetUsage());
				#endif
				glBindBuffer(PRIMARY_BUFFER, previous);
				return true;
			#endif
			#if SOLAIRE_GL_VER_LT(3,0)
				#error SolaireCPP : Buffer::Allocate only defined for OpenGL 3.0+
			#endif
		}
	
		bool Destroy() throw() {
			if(mID == GLID::NULL_ID) return false;
			//! \todo unbind all
			mIsMapped = false;
			#if SOLAIRE_GL_VER_LT(4,4)
				mMapBinding = Target::INVALID_TARGET;
			#endif
			glDeleteBuffers(1, reinterpret_cast<GLuint*>(&mID));
			mID = GLID::NULL_ID;
			return true;
		}
	}; 

	//! \todo Persistant mapping

	template<const bool READ, const bool WRITE, const bool PERSISSTENT = false>
	class BufferImplementation : public Buffer {
	public:
		static_assert(READ || WRITE, "SolaireCpp : Buffer must have READ and/or WRITE set");
		static_assert((! PERSISSTENT) || (PERSISSTENT && SOLAIRE_GL_VER_GTE(4, 4)), "SolaireCpp : Persistent Buffers only supported on OpenGL 4.4+");
	protected:
		// Inherited from BufferBase
		#if SOLAIRE_GL_VER_GTE(4,4)
			GLbitfield SOLAIRE_EXPORT_CALL GetCreationFlags() const throw() override {
				enum : GLbitfield{
					CREATION_FLAGS = (READ ? GL_MAP_READ_BIT : 0) | (WRITE ? GL_MAP_WRITE_BIT : 0) | (PERSISTENT ? GL_MAP_PERSISTENT_BIT : 0);
				};
		
				return CREATION_FLAGS;
			}
		#endif
		#if SOLAIRE_GL_VER_GTE(3,0) && SOLAIRE_GL_VER_LT(4,4)
			GLenum SOLAIRE_EXPORT_CALL GetUsage() const throw() override {
				//! \todo Check usage enums are optimal
				if(READ && WRITE) return GL_DYNAMIC_COPY;
				if(READ) return GL_STATIC_READ;
				if(WRITE) return GL_DYNAMIC_DRAW;
				return GL_INVALID_ENUM;
			}
		#endif
	public:
		BufferImplementation() : 
			Buffer()
		{}
	
		template<const bool R, const bool W, const bool P>
		BufferImplementation(typename std::enable_if<(READ >= R) && (WRITE >= W), BufferImplementation<R, W, P>&&>::type aOther) :
			Buffer(std::move(aOther))
		{}
	
		template<const bool R, const bool W, const bool P>
		BufferImplementation(typename std::enable_if<R && WRITE, const BufferImplementation<R, W, P>&>::type aOther) :
			Buffer()
		{
			operator=(aOther);
		}
	
		SOLAIRE_EXPORT_CALL ~BufferImplementation() throw() {
			
		}
	
		template<const bool R, const bool W, const bool P>
		typename std::enable_if<(READ >= R) && (WRITE >= W), BufferImplementation<READ, WRITE, PERSISSTENT>&>::type operator=(BufferImplementation<R, W, P>&& aOther) {
			Buffer::operator=(std::move(aOther));
			return *this;
		}
	
		template<const bool R, const bool W, const bool P>
		typename std::enable_if<R && WRITE, BufferImplementation<READ, WRITE, PERSISSTENT>&>::type operator=(const BufferImplementation<R, W, P>& aOther) {
			Bufer::operator=(aOther);
			if(mID != NULL_ID && aOther.mID != NULL_ID) {
				Create();
			
				#if SOLAIRE_GL_VER_GTE(4,5)
					glCopyNamedBufferSubData((GLint) other.mID, (GLint) mID, 0, 0, mSize);
				#endif
				#if SOLAIRE_GL_VER_GTE(3,0) && SOLAIRE_GL_VER_LT(4,5)
					GLuint previousRead = NULL_ID;  
					GLuint previousWrite = NULL_ID;
					glGetIntegerv(PRIMARY_BUFFER_BINDING, reinterpret_cast<GLint*>(&previousRead));
					glGetIntegerv(SECONDARY_BUFFER_BINDING, reinterpret_cast<GLint*>(&previousWrite));
			
					glBindBuffer(PRIMARY_BUFFER, (GLint) aOther.mID);
					glBindBuffer(SECONDARY_BUFFER, (GLint) mID);

					#if SOLAIRE_GL_VER_EQ(3,0)
						const void* const readMap = glMapBuffer(PRIMARY_BUFFER, GL_READ_ONLY);
						void* const writeMap = glMapBuffer(SECONDARY_BUFFER, GL_WRITE_ONLY);
						std::memcpy(writeMap, readMap, mSize);
						glUnmapBuffer(PRIMARY_BUFFER);
						glUnmapBuffer(SECONDARY_BUFFER);
					#else
						glCopyBufferSubData(PRIMARY_BUFFER, SECONDARY_BUFFER, 0, 0, mSize);
					#endif
			
					glBindBuffer(PRIMARY_BUFFER, previousRead);
					glBindBuffer(SECONDARY_BUFFER, previousWrite);
				#endif
				#if SOLAIRE_GL_VER_LT(3,0)
					#error SolaireCPP : BufferImplementation::operator=(copy) only defined for OpenGL 3.0+
				#endif
				return *this;
			}
		}

		template<const bool FLAG = READ>
		typename std::enable_if<FLAG, void>::type Read(const GLuint aOffset, void* const aData, const GLuint aSize, const SynchronisationMode aSyncMode = SYNCHRONISED) const throw() {
			#if SOLAIRE_GL_VER_GTE(4,4)
				const void* const mapping = glMapNamedBufferRange((GLint) mID, aOffset, aSize, GL_MAP_READ_BIT | aSyncMode);
				std::memcpy(aData, mapping, aSize);
				glUnmapNamedBuffer((GLint) mID);
			#endif
			#if SOLAIRE_GL_VER_GTE(3,0) && SOLAIRE_GL_VER_LT(4,4)
				GLint previous = (GLint) GLID::NULL_ID;
				glGetIntegerv(PRIMARY_BUFFER_BINDING, &previous);
				glBindBuffer(PRIMARY_BUFFER, (GLint) mID);
				const void* const mapping = glMapBufferRange(PRIMARY_BUFFER, aOffset, aSize, GL_MAP_READ_BIT | aSyncMode);
				std::memcpy(aData, mapping, aSize);
				glUnmapBuffer(PRIMARY_BUFFER);
				glBindBuffer(PRIMARY_BUFFER, previous);
			#endif
		}

		template<const bool FLAG = WRITE>
		typename std::enable_if<FLAG, bool>::type Write(const GLuint aOffset, void* const aData, const GLuint aSize, const InvalidationMode aInvMode = NO_INVALIDATION, const SynchronisationMode aSyncMode = SYNCHRONISED) throw() {
			if(mIsMapped) return false;
			#if SOLAIRE_GL_VER_GTE(4,4)
				void* const mapping = glMapNamedBufferRange((GLint) mID, aOffset, aSize, GL_MAP_WRITE_BIT | aInvMode | aSyncMode);
				std::memcpy(mapping, aData, aSize);
				glUnmapNamedBuffer((GLint) mID);
			#endif
			#if SOLAIRE_GL_VER_GTE(3,0) && SOLAIRE_GL_VER_LT(4,4)
				GLint previous = (GLint) GLID::NULL_ID;
				glGetIntegerv(PRIMARY_BUFFER_BINDING, &previous);
				glBindBuffer(PRIMARY_BUFFER, (GLint) mID);
				void* const mapping = glMapBufferRange(PRIMARY_BUFFER, aOffset, aSize, GL_MAP_WRITE_BIT | aSyncMode);
				std::memcpy(mapping, aData, aSize);
				glUnmapBuffer(PRIMARY_BUFFER);
				glBindBuffer(PRIMARY_BUFFER, previous);
			#endif
			return true;
		}
	
		template<const bool FLAG = READ>
		typename std::enable_if<FLAG, const void*>::type MapRangeRead() const throw() {
			if(mIsMapped) return nullptr;
			#if SOLAIRE_GL_VER_GTE(4,4)
				mIsMapped = true;
				if(PERSISTANT) {
					return MapRangeRead(0, mSize);
				}else {
					return glMapNamedBufferRange((GLint) mID, GL_READ_ONLY);
				}
			#else
				mMapBinding = GLBufferImplementation::GetUnusedBuffer();
				if(mMapBinding == Target::INVALID_TARGET) return nullptr;
				Bind(mMapBinding);
				return MapRangeRead(mMapBinding);
			#endif
		}  
		
		template<const bool FLAG = WRITE>
		typename std::enable_if<FLAG, void*>::type MapRangeWrite() throw() {
			if(mIsMapped) return nullptr;
			#if SOLAIRE_GL_VER_GTE(4,4)
				mIsMapped = true;
				if(PERSISTANT) {
					return MapRangeWrite(0, mSize);
				}else {
					enum : GLenum { MODE = READ ? GL_READ_WRITE : GL_WRITE_ONLY };
					return glMapNamedBufferRange((GLint) mID, MODE);
				}
			#else
				mMapBinding = GLBufferImplementation::GetUnusedBuffer();
				if(mMapBinding == Target::INVALID_TARGET) return nullptr;
				Bind(mMapBinding);
				return MapRangeWrite(mMapBinding);
			#endif
		}  
		
		template<const bool FLAG = READ>
		typename std::enable_if<FLAG, const void*>::type MapRangeRead(const GLuint aOffset, const GLuint aSize, const SynchronisationMode aSyncMode = SYNCHRONISED) const throw() {
			if(mIsMapped) return nullptr;
			#if SOLAIRE_GL_VER_GTE(4,4)
				mIsMapped = true;
				const GLbitfield flags = GL_MAP_READ_BIT | aSyncMode | (PERSISTENT ? GL_MAP_PERSISTENT_BIT : 0);
				return glMapNamedBufferRange((GLint) mID, aOffset, aSize, flags);
			#else
				mMapBinding = GLBufferImplementation::GetUnusedBuffer();
				if(mMapBinding == Target::INVALID_TARGET) return nullptr;
				Bind(mMapBinding);
				return MapRangeRead(mMapBinding, aOffset, aSize, aSyncMode);
			#endif
		}  
		
		template<const bool FLAG = WRITE> 
		typename std::enable_if<FLAG, void*>::type MapRangeWrite(const GLuint aOffset, const GLuint aSize, const InvalidationMode aInvMode = NO_INVALIDATION, const SynchronisationMode aSyncMode = SYNCHRONISED) throw() {  
			if(mIsMapped) return nullptr;
			#if SOLAIRE_GL_VER_GTE(4,4)
				mIsMapped = true;
				const GLbitfield flags = GL_MAP_WRITE_BIT | aInvMode | aSyncMode | (aInvMode == NO_INVALIDATION && READ ? GL_MAP_READ_BIT : 0) | (PERSISTENT ? GL_MAP_PERSISTENT_BIT : 0);
				return glMapNamedBufferRange((GLint) mID, aOffset, aSize, flags);
			#else
				mMapBinding = GLBufferImplementation::GetUnusedBuffer();
				if(mMapBinding == Target::INVALID_TARGET) return nullptr;
				Bind(mMapBinding);
				return MapRangeWrite(mMapBinding, aOffset, aSize, aInvMode, aSyncMode);
			#endif
		}
		
		template<const bool R = READ, const bool W = WRITE, const bool P = PERSISSTENT>
		typename std::enable_if<(R || W) && (! P), bool>::type Unmap() throw() { 
			if(mIsMapped) return nullptr;
			#if SOLAIRE_GL_VER_GTE(4,4)
				glUnmapNamedBuffer((GLint) mID);
				mIsMapped = false;
				return true;
			#else
				if(mMapBinding == Target::INVALID_TARGET) return false;
				const bool result = Unbind(mMapBinding);
				mMapBinding = Target::INVALID_TARGET;
				return result;
			#endif
		}

		template<const bool FLAG = READ>
		typename std::enable_if<FLAG, const void*>::type MapRangeRead(const Target aTarget) const throw() {
			if(mIsMapped) return nullptr;
			mIsMapped = true;
			return glMapBufferRange(aTarget, GL_READ_ONLY);
		}

		template<const bool FLAG = WRITE>
		typename std::enable_if<FLAG, void*>::type MapRangeWrite(const Target aTarget) throw() {
			if(mIsMapped) return nullptr;
			return glMapBufferRange(aTarget, READ ? GL_READ_WRITE : GL_WRITE_ONLY);
		}

		template<const bool FLAG = READ>
		typename std::enable_if<FLAG, const void*>::type MapRangeRead(const Target aTarget, const GLuint aOffset, const GLuint aSize, const SynchronisationMode aSyncMode = SYNCHRONISED) const throw() {
			if(mIsMapped) return nullptr;
			mIsMapped = true;
			return glMapBufferRange(aTarget, aOffset, aSize, GL_MAP_READ_BIT | aSyncMode);
		}

		template<const bool FLAG = WRITE>
		typename std::enable_if<FLAG, void*>::type MapRangeWrite(const Target Target, const GLuint aOffset, const GLuint aSize, const InvalidationMode aInvMode = NO_INVALIDATION, const SynchronisationMode aSyncMode = SYNCHRONISED) throw() {
			if(mIsMapped) return nullptr;
			mIsMapped = true;
			return glMapBufferRange(aTarget, aOffset, aSize, GL_MAP_WRITE_BIT | aInvMode | aSyncMode | (aInvMode == NO_INVALIDATION && READ ? GL_MAP_READ_BIT : 0));
		}

		template<const bool R = READ, const bool W = WRITE>
		typename std::enable_if<R || W, bool>::type Unmap(const Target Target) throw() {
			if(! mIsMapped) return false;
			glUnmapBuffer(aTarget);
			mIsMapped = false;
			return true;
		}

		template<const bool R = READ, const bool W = WRITE>
		typename std::enable_if<R || W, bool>::type IsMapped() const throw() {
			return mIsMapped;
		}
	};

	template<const bool WRITE, const bool PERSISSTENT = false>
	using ReadBuffer = BufferImplementation<true, WRITE, PERSISSTENT>;

	template<const bool READ, const bool PERSISSTENT = false>
	using WriteBuffer = BufferImplementation<READ, true, PERSISSTENT>;

	typedef ReadBuffer<false, false>	ReadOnlyBuffer;
	typedef WriteBuffer<false, false>	WriteOnlyBuffer;
	typedef ReadBuffer<true, false>		ReadWriteBuffer;

	typedef ReadBuffer<false, true>		PersisstentReadOnlyBuffer;
	typedef WriteBuffer<false, true>	PersisstentWriteOnlyBuffer;
	typedef ReadBuffer<true, true>		PersisstentReadWriteBuffer;
}

#include "BufferImplementation.inl"

#endif
