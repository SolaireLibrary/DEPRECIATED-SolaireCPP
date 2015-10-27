#ifndef SOLAIRE_GRAPHICS_BUFFER_HPP
#define SOLAIRE_GRAPHICS_BUFFER_HPP

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
\file Buffer.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 27th October 2015
Last Modified	: 27th October 2015
*/

#include "..\Maths\Vector2.hpp"
#include "Graphics.inl"
#include "GLBindStack.hpp"

namespace Solaire{ namespace Graphics{

    //! \todo Buffer Mapping
    //! \todo Buffer Alignment
    //! \todo Buffer Write Synchronization
    //! \todo Buffer Read Synchronization
    //! \todo Buffer Invalidation
    //! \todo Buffer Streaming

    class Buffer{
    protected:
        static GLBindStack BIND_STACK
    protected:
        GLuint mBytes;
        GLuint mID;
    private:
        static void BindFn(const GLenum aTarget, const void* const aObject){
            glBindBuffer​(aTarget, static_cast<BufferInterface*>(aObject)->mID);
        }
    public:
        Buffer(const GLuint aBytes, const GLuint aID):
            mBytes(aBytes),
            mID(aID)
        {}

        Buffer(Buffer&& aOther):
            mBytes(aOther.mBytes)
            mID(aOther.mID)
        {
            aOther.mBytes = 0;
            aOther.mID = 0;
        }

        virtual ~Buffer(){
            if(mID != 0){
                BIND_STACK.UnbindAll(this);

                glDeleteBuffers(1, &mID);

                mBytes = 0;
                mID = 0;
            }
        }

        ////

        Buffer& operator=(Buffer&& aOther){
            ~Buffer();

            mBytes = aOther.mBytes;
            mID = aOther.mID;

            aOther.mBytes = 0;
            aOther.mID = 0;

            return *this;
        }

        Buffer& operator=(const Buffer& aOther){
            if(mBytes < aOther.mBytes){
                Allocate(aOther.mBytes);
            }

            Copy(aOther, 0, 0, aOther.mBytes);
            return *this;
        }

        ////

        virtual void Allocate(const size_t aBytes) = 0;
        virtual void Allocate(const void* const aData, const size_t aBytes) = 0;

        ////

        GLuint Size() const{
            return mBytes;
        }

        ////

        bool IsBound(const GLenum aTarget) const{
            const DynamicArray<GLEnum targets> targets = BIND_STACK.GetBoundTargets(this);
            return targets.FindFirst(aTarget) != targets.end();
        }

        void Bind(const GLenum aTarget){
            BIND_STACK.Bind(aTarget, this);
        }

        void Unbind(){
            BIND_STACK.Unbind(aTarget, this);
        }

        ////

        void Copy(const Buffer& aOther, const size_t aReadOffet, const size_t aWriteOffset, const size_t aBytes){
            mBytes = aOther.mBytes;
            Allocate(mBytes);

            const_cast<Buffer&>(aOther).Bind(GL_COPY_READ_BUFFER);
            Bind(GL_COPY_WRITE_BUFFER);

            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, aReadOffet, aWriteOffset, mBytes);

            const_cast<Buffer&>(aOther).Unbind(GL_COPY_READ_BUFFER);
            Unbind(GL_COPY_WRITE_BUFFER);
        }
    };

    GLBindStack BufferInterface::BIND_STACK(
        GetDefaultAllocator(),
        &BufferInterface::BindFn,
        {
            GL_ARRAY_BUFFER,
            GL_ELEMENT_ARRAY_BUFFER,
            GL_COPY_READ_BUFFER,
            GL_COPY_WRITE_BUFFER,
            GL_PIXEL_UNPACK_BUFFER ,
            GL_PIXEL_PACK_BUFFER,
            GL_QUERY_BUFFER,
            GL_TEXTURE_BUFFER,
            GL_TRANSFORM_FEEDBACK_BUFFER,
            GL_UNIFORM_BUFFER,
            GL_DRAW_INDIRECT_BUFFER,
            GL_ATOMIC_COUNTER_BUFFER,
            GL_DISPATCH_INDIRECT_BUFFER,
            GL_SHADER_STORAGE_BUFFER
        }
    );

    template<const bool READ_BIT, const bool WRITE_BIT, const bool DYNAMIC_BIT, const bool PERSISTENT_BIT, const bool COHERENT_BIT, const bool CLIENT_STORAGE_BIT>
    class ImmutableBuffer : public Buffer{
    public:
        typedef ImmutableBuffer<READ_BIT, WRITE_BIT, DYNAMIC_BIT, PERSISTENT_BIT, COHERENT_BIT, CLIENT_STORAGE_BIT> Self;

        enum : Gluint{
            ACCESS_FLAGS =
                (READ_BIT           ? GL_MAP_READ_BIT           : 0) |
                (WRITE_BIT          ? GL_MAP_WRITE_BIT          : 0) |
                (DYNAMIC_BIT        ? GL_DYNAMIC_STORAGE_BIT    : 0) |
                (PERSISTENT_BIT     ? GL_PERSISTENT_BIT         : 0) |
                (COHERENT_BIT       ? GL_COHERENT_BIT           : 0) |
                (CLIENT_STORAGE_BIT ? GL_CLIENT_STORAGE_BIT     : 0)
        };
    public:
        ImmutableBuffer():
            Buffer(0, 0)
        {
            glGenBuffers(1, &mID);
        }

        ImmutableBuffer(const size_t aBytes):
            Buffer(0, 0)
        {
            glGenBuffers(1, &mID);

            Bind(GL_COPY_WRITE_BUFFER);
            Allocate(aBytes);
            Unbind(GL_COPY_WRITE_BUFFER);
        }

        ImmutableBuffer(const Buffer& aOther):
            Buffer(0, 0)
        {
            glGenBuffers(1, &mID);
            Bind(GL_COPY_WRITE_BUFFER);
            Allocate(aOther.mBytes);
            Unbind(GL_COPY_WRITE_BUFFER);
            Copy(aOther, 0, 0, mBytes);
        }

        ImmutableBuffer(Buffer&& aOther):
            Buffer(std::move(aOther))
        {}

        ////

        void Allocate(const size_t aBytes) override{
            glBufferStorage(mTarget, aBytes, nullptr, ACCESS_FLAGS);
            mBytes = aBytes;
        }

        void Allocate(const void* const aData, const size_t aBytes) override{
            glBufferStorage(mTarget, aBytes, aData, ACCESS_FLAGS);
            mBytes = aBytes;
        }
    };

    template<const GLenum USAGE>
    class MutableBuffer : public Buffer{
    public:
        typedef MutableBuffer<MUTABLE> Self;

        enum : GLenum{
            USAGE_MODE = USAGE
        };
    public:
        MutableBuffer():
            Buffer(0, 0)
        {
            glGenBuffers(1, &mID);
        }

        MutableBuffer(const size_t aBytes):
            Buffer(0, 0)
        {
            glGenBuffers(1, &mID);

            Bind(GL_COPY_WRITE_BUFFER);
            Allocate(aBytes);
            Unbind(GL_COPY_WRITE_BUFFER);
        }

        MutableBuffer(const Buffer& aOther):
            Buffer(0, 0)
        {
            glGenBuffers(1, &mID);
            Bind(GL_COPY_WRITE_BUFFER);
            Allocate(aOther.mBytes);
            Unbind(GL_COPY_WRITE_BUFFER);
            Copy(aOther, 0, 0, mBytes);
        }

        MutableBuffer(Buffer&& aOther):
            Buffer(std::move(aOther))
        {}

        ////

        void Allocate(const size_t aBytes) override{
            glBufferData(mTarget, aBytes, nullptr, USAGE);
            mBytes = aBytes;
        }

        void Allocate(const void* const aData, const size_t aBytes) override{
            glBufferData(mTarget, aBytes, aData, USAGE);
            mBytes = aBytes;
        }

        ////

        void Buffer(const void* const aData, const size_t aBytes){
            glBufferSubData(mTarget, 0, aBytes, aData);
        }

        void OffsetBuffer(const size_t aOffset, const void* const aData, const size_t aBytes){
            glBufferSubData(mTarget, aOffset, aBytes, aData);
        }

        ////

        void Clear(const GLubyte aValue, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, mBytes, aFormat, GL_UNSIGNED_BYTE, &aValue);
        }

        void Clear(const GLbyte aValue, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, mBytes, aFormat, GL_BYTE, &aValue);
        }

        void Clear(const GLushort aValue, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, mBytes, aFormat, GL_UNSIGNED_SHORT, &aValue);
        }

        void Clear(const GLshort aValue, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, mBytes, aFormat, GL_SHORT, &aValue);
        }

        void Clear(const GLuint aValue, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, mBytes, aFormat, GL_UNSIGNED_INT, &aValue);
        }

        void Clear(const GLint aValue, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, mBytes, aFormat, GL_INT, &aValue);
        }

        ////

        void SubClear(const GLubyte aValue, const size_t aOffset, const size_t aBytes, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, aOffset, aBytes, aFormat, GL_UNSIGNED_BYTE, &aValue);
        }

        void SubClear(const GLbyte aValue, const size_t aOffset, const size_t aBytes, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, aOffset, aBytes, aFormat, GL_BYTE, &aValue);
        }

        void SubClear(const GLushort aValue, const size_t aOffset, const size_t aBytes, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, aOffset, aBytes, aFormat, GL_UNSIGNED_SHORT, &aValue);
        }

        void SubClear(const GLshort aValue, const size_t aOffset, const size_t aBytes, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, aOffset, aBytes, aFormat, GL_SHORT, &aValue);
        }

        void SubClear(const GLuint aValue, const size_t aOffset, const size_t aBytes, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, aOffset, aBytes, aFormat, GL_UNSIGNED_INT, &aValue);
        }

        void SubClear(const GLint aValue, const size_t aOffset, const size_t aBytes, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, aOffset, aBytes, aFormat, GL_INT, &aValue);
        }
    };
}}

#endif
