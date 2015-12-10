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

#include "Object.hpp"

namespace Solaire {

	class Buffer : public Object {
	public:
		enum Target : GLenum {
			#if SOLAIRE_GL_VER_GTE(3,1)
				COPY_READ = GL_COPY_READ_BUFFER,
				COPY_WRITE = GL_COPY_WRITE_BUFFER,
				TEXTURE = GL_TEXTURE_BUFFER,
			#endif
			#if SOLAIRE_GL_VER_GTE(4,0)
				UNIFORM = GL_UNIFORM_BUFFER,
			#endif
			#if SOLAIRE_GL_VER_GTE(4,1)
				DRAW_INDIRECT = GL_DRAW_INDIRECT_BUFFER,
			#endif
			#if SOLAIRE_GL_VER_GTE(4,2)
				ATOMIC_COUNTER = GL_ATOMIC_COUNTER_BUFFER,
			#endif
			#if SOLAIRE_GL_VER_GTE(4,3)
				DISPATCH_INDIRECT = GL_DISPATCH_INDIRECT_BUFFER,
				SHADER_STORAGE = GL_SHADER_STORAGE_BUFFER,
			#endif
			#if SOLAIRE_GL_VER_GTE(4,4)
				QUERY = GL_QUERY_BUFFER,
			#endif
			ARRAY = GL_ARRAY_BUFFER,
			ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER,
			PIXEL_PACK = GL_PIXEL_PACK_BUFFER,
			PIXEL_UNPACK = GL_PIXEL_UNPACK_BUFFER,
			TRANSFORM_FEEDBACK = GL_TRANSFORM_FEEDBACK_BUFFER
		};

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
		ID mID;
		GLuint mSize;
	private:
		Buffer(Buffer&&) = delete;
		Buffer(const Buffer&) = delete;
		Buffer& operator=(Buffer&&) = delete;
		Buffer& operator=(const Buffer&) = delete;
	protected:
		#if SOLAIRE_GL_VER_GTE(4,0)
			virtual GLbitfield SOLAIRE_EXPORT_CALL GetCreationFlags() const throw() = 0;
		#endif
		#if SOLAIRE_GL_VER_GTE(3,0) && SOLAIRE_GL_VER_LT(4,0)
			virtual GLenum SOLAIRE_EXPORT_CALL GetUsage() const throw() = 0;
		#endif
	public:
		Buffer(const GLuint aSize) :
			mID(NULL_ID),
			mSize(aSize)
		{}
	
		virtual SOLAIRE_EXPORT_CALL ~Buffer() throw() {
	
		}
	
		GLuint Size() const throw() {
			return mSize;
		}
	
		bool Bind(const Target aTarget) {
			if(mID == NULL_ID) return false;
			glBindBuffer(aTarget, mID);
			return true;
		}
	
		// Inherited from Object
	
		ID SOLAIRE_EXPORT_CALL GetID() const throw() override {
			return mID;
		}
	
		bool SOLAIRE_EXPORT_CALL Create() throw() override {
			if(mID != NULL_ID) return false;
			glGenBuffers(1, reinterpret_cast<GLuint*>(&mID));
			if(mID == NULL_ID) return false;

			#if SOLAIRE_GL_VER_GTE(4,5)
				glNamedBufferStorage(mID, mSize, nullptr, GetCreationFlags());
				return true;
			#endif
			#if SOLAIRE_GL_VER_GTE(4,4) && SOLAIRE_GL_VER_LT(4,5)
				GLuint previous = NULL_ID;
				glGetIntegerv(GL_COPY_WRITE_BUFFER_BINDING, reinterpret_cast<GLint*>(&previous));
				glBindBuffer(GL_COPY_WRITE_BUFFER, mID);
				glBufferStorage(GL_COPY_WRITE_BUFFER, mSize, nullptr, GetCreationFlags());
				glBindBuffer(GL_COPY_WRITE_BUFFER, previous);
				return true;
			#endif
			#if SOLAIRE_GL_VER_LT(4,4)
				GLuint previous = NULL_ID;
				glGetIntegerv(GL_ARRAY_BUFFER_BINDING, reinterpret_cast<GLint*>(&previous));
				glBindBuffer(GL_ARRAY_BUFFER, mID);
				glBufferData(GL_ARRAY_BUFFER, mSize, nullptr, GetUsage());
				glBindBuffer(GL_ARRAY_BUFFER, previous);
				return true;
			#endif
		}
	
		bool SOLAIRE_EXPORT_CALL Destroy() throw() override {
			if(mID == NULL_ID) return false;
			glDeleteBuffers(1, reinterpret_cast<GLuint*>(&mID));
			mID = NULL_ID;
			return true;
		}
	};

	//! \todo Persistant mapping

	template<const bool READ, const bool WRITE>
	class BufferImplementation : public Buffer {
		// Inherited from BufferBase
		#if SOLAIRE_GL_VER_GTE(4,0)
			GLbitfield SOLAIRE_EXPORT_CALL GetCreationFlags() const throw() override {
				enum : GLbitfield{
					CREATION_FLAGS = (READ ? GL_MAP_READ_BIT : 0) | (WRITE ? GL_MAP_WRITE_BIT : 0);
				};
		
				return CREATION_FLAGS;
			}
		#endif
		#if SOLAIRE_GL_VER_GTE(3,0) && SOLAIRE_GL_VER_LT(4,0)
			GLenum SOLAIRE_EXPORT_CALL GetUsage() const throw() override {
				//! \todo Check usage enums are optimal
				if(READ && WRITE) return GL_DYNAMIC_COPY;
				if(READ) return GL_STATIC_READ;
				if(WRITE) return GL_DYNAMIC_DRAW;
				return GL_INVALID_ENUM;
			}
		#endif
	public:
		BufferImplementation(const GLuint aSize) : 
			Buffer(aSize)
		{}
	
		template<const bool R, const bool W>
		BufferImplementation(typename std::enable_if<(READ >= R) && (WRITE >= W), BufferImplementation<R, W>&&>::type aOther) :
			Buffer(aOther.mSize)
		{
			mID = aOther.mID;
			aOther.mID = NULL_ID;
		}
	
		template<const bool R, const bool W>
		BufferImplementation(typename std::enable_if<R && WRITE, BufferImplementation<R, W>&>::type aOther) :
			Buffer(0)
		{
			operator=(aOther);
		}
	
		SOLAIRE_EXPORT_CALL ~BufferImplementation() throw() {
			Destroy();
		}
	
		template<const bool R, const bool W>
		typename std::enable_if<(READ >= R) && (WRITE >= W), BufferImplementation<READ, WRITE>&>::type operator=(BufferImplementation<R, W>&& aOther) {
			std::swap(mID, aOther.mID);
			std::swap(mSize, aOther.mSize);
			return *this;
		}
	
		template<const bool R, const bool W>
		typename std::enable_if<R && WRITE, BufferImplementation<READ, WRITE>&>::type operator=(const BufferImplementation<R, W>& aOther) {
			if(mSize < aOther.mSize) {
				Destroy();
				mSize = aOther.mSize;
			}
			if(aOther.mID != NULL_ID) {
				Create();
			
				#if SOLAIRE_GL_VER_GTE(4,5)
					glCopyNamedBufferSubData(other.mID, mID, 0, 0, mSize);
				#endif
				#if SOLAIRE_GL_VER_GTE(4,0) && SOLAIRE_GL_VER_LT(4,5)
					GLuint previousRead = NULL_ID;  
					GLuint previousWrite = NULL_ID;
					glGetIntegerv(GL_COPY_READ_BUFFER_BINDING, reinterpret_cast<GLint*>(&previousRead));
					glGetIntegerv(GL_COPY_WRITE_BUFFER_BINDING, reinterpret_cast<GLint*>(&previousWrite));
			
					glBindBuffer(GL_COPY_READ_BUFFER, aOther.mID);
					glBindBuffer(GL_COPY_WRITE_BUFFER, mID);
			
					glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, mSize);
			
					glBindBuffer(GL_COPY_READ_BUFFER, previousRead);
					glBindBuffer(GL_COPY_WRITE_BUFFER, previousWrite);
				#endif
				#if SOLAIRE_GL_VER_LT(4,0)
					#error SolaireCPP : BufferImplementation::operator=(copy) only defined for OpenGL 4.0+
				#endif
			}
		}
	
		#if SOLAIRE_GL_VER_GTE(4,4)
			template<const bool FLAG = READ>
			typename std::enable_if<FLAG, void>::type Read(const GLuint aOffset, void* const aData, const GLuint aSize, const SynchronisationMode aSyncMode = SYNCHRONISED) const throw() {
				const void* const mapping = glMapNamedBufferRange(mID, aOffset, aSize, GL_MAP_EAD_BIT | aSyncMode);
				std::memcpy(aData, mapping, aSize);
				glUnmapNamedBuffer(mID);
			}  
	 
			template<const bool FLAG = WRITE> 
			typename std::enable_if<FLAG, void>::type Write(const GLuint aOffset, void* const aData, const GLuint aSize, const InvalidationMode aInvMode = NO_INVALIDATION, const SynchronisationMode aSyncMode = SYNCHRONISED) throw() {
				void* const mapping = glMapNamedBufferRange(mID, aOffset, aSize, GL_MAP_WRITE_BIT | aInvMode | aSyncMode);
				std::memcpy(mapping, aData, aSize);
				glUnmapNamedBuffer(mID);
			}
	
			template<const bool FLAG = READ>
			typename std::enable_if<FLAG, const void*>::type MapRangeRead() const throw() {
				return glMapNamedBufferRange(mID, GL_READ_ONLY);
			}  
		
			template<const bool FLAG = WRITE>
			typename std::enable_if<FLAG, void*>::type MapRangeWrite() throw() {
				return glMapNamedBufferRange(mID, READ ? GL_READ_WRITE : GL_WRITE_ONLY);
			}  
		
			template<const bool FLAG = READ>
			typename std::enable_if<FLAG, const void*>::type MapRangeRead(const GLuint aOffset, const GLuint aSize, const SynchronisationMode aSyncMode = SYNCHRONISED) const throw() {
				return glMapNamedBufferRange(mID, aOffset, aSize, GL_MAP_READ_BIT | aSyncMode);
			}  
		
			template<const bool FLAG = WRITE> 
			typename std::enable_if<FLAG, void*>::type MapRangeWrite(const GLuint aOffset, const GLuint aSize, const InvalidationMode aInvMode = NO_INVALIDATION, const SynchronisationMode aSyncMode = SYNCHRONISED) throw() {  
				return glMapNamedBufferRange(mID, aOffset, aSize, GL_MAP_WRITE_BIT | aInvMode | aSyncMode | (aInvMode == NO_INVALIDATION && READ ? GL_MAP_READ_BIT : 0) );
			}
		
			template<const bool R = READ, const bool W = WRITE> 
			typename std::enable_if<R || W, void>::type Unmap() throw() {  
				return glUnmapNamedBuffer(mID);
			}
		#endif
		#if SOLAIRE_GL_VER_GTE(3,0) && SOLAIRE_GL_VER_LT(4,4)
			template<const bool FLAG = READ>
			typename std::enable_if<FLAG, const void*>::type MapRangeRead(const Target aTarget) const throw() {
				return glMapBufferRange(aTarget, GL_READ_ONLY);
			}

			template<const bool FLAG = WRITE>
			typename std::enable_if<FLAG, void*>::type MapRangeWrite(const Target aTarget) throw() {
				return glMapBufferRange(aTarget, READ ? GL_READ_WRITE : GL_WRITE_ONLY);
			}

			template<const bool FLAG = READ>
			typename std::enable_if<FLAG, const void*>::type MapRangeRead(const Target aTarget, const GLuint aOffset, const GLuint aSize, const SynchronisationMode aSyncMode = SYNCHRONISED) const throw() {
				return glMapBufferRange(aTarget, aOffset, aSize, GL_MAP_READ_BIT | aSyncMode);
			}

			template<const bool FLAG = WRITE>
			typename std::enable_if<FLAG, void*>::type MapRangeWrite(const Target Target, const GLuint aOffset, const GLuint aSize, const InvalidationMode aInvMode = NO_INVALIDATION, const SynchronisationMode aSyncMode = SYNCHRONISED) throw() {
				return glMapBufferRange(aTarget, aOffset, aSize, GL_MAP_WRITE_BIT | aInvMode | aSyncMode | (aInvMode == NO_INVALIDATION && READ ? GL_MAP_READ_BIT : 0));
			}

			template<const bool R = READ, const bool W = WRITE>
			typename std::enable_if<R || W, void>::type Unmap(const Target Target) throw() {
				return glUnmapBuffer(aTarget);
			}
		#endif
	};

	template<const bool WRITE>
	using ReadBuffer = BufferImplementation<true, WRITE>;

	template<const bool READ>
	using WriteBuffer = BufferImplementation<READ, true>;

	typedef ReadBuffer<false>	ReadOnlyBuffer;
	typedef WriteBuffer<false>	WriteOnlyBuffer;
	typedef ReadBuffer<true>	ReadWriteBuffer;
}

#endif
