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
Last Modified	: 28th October 2015
*/

#include "..\Maths\Vector2.hpp"
#include "Graphics.inl"
#include "GLBindStack.hpp"

namespace Solaire{ namespace Graphics{

    //! \todo Buffer Check if buffer is mapped
    //! \todo Buffer Alignment
    //! \todo Buffer Write Synchronization
    //! \todo Buffer Read Synchronization
    //! \todo Buffer Streaming
    //! \todo Flush SubBuffer


    class Buffer{
    protected:
        static GLBindStack BIND_STACK
    private:
        GLenum mMapTarget;
    protected:
        GLuint mBytes;
        GLuint mID;
    private:
        static void BindFn(const GLenum aTarget, const void* const aObject){
            glBindBuffer​(aTarget, static_cast<BufferInterface*>(aObject)->mID);
        }

        static GLbitfield GetBarrierBit(const GLenum aTarget){
            switch(aTarget){
            case GL_VERTEX_ATTRIB_ARRAY_BUFFER :
                return GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT;
            case GL_ELEMENT_ARRAY_BUFFER:
                return GL_ELEMENT_ARRAY_BARRIER_BIT;
            case GL_DRAW_INDIRECT_BUFFER :
            case GL_DISPATCH_INDIRECT_BUFFER :
                return GL_COMMAND_BARRIER_BIT;
            case GL_PIXEL_PACK_BUFFER:
            case GL_PIXEL_UNPACK_BUFFER:
                return GL_PIXEL_BUFFER_BARRIER_BIT;
            case GL_QUERY_BUFFER:
                return GL_QUERY_BUFFER_BARRIER_BIT;
            default:
                return 0;
            }
        }
    protected:
        void* InternalMap(const GLenum aAccessFlags){
            if(mMapTarget != 0) throw std::runtime_error("Buffer : Buffer has already been mapped");

            const DynamicArray<GLenum> targets = BIND_MAP::GetBoundTargets(this);
            if(targets.IsEmpty()) throw std::runtime_error("Buffer : Buffer is not bound");
            mMapTarget = targets.Back();

            return glMapBuffer(mMapTarget, aAccessFlags);
        }

        void InternalUnmap() const{
            if(mMapTarget == 0) throw std::runtime_error("Buffer : Buffer has not been mapped");
            glUnmapBuffer(mMapTarget);
            mMapTarget = 0;
        }

        ////

        void* InternalMapRange(const size_t aOffset, const size_t aBytes, const GLenum aAccessFlags){
            if(mMapTarget != 0) throw std::runtime_error("Buffer : Buffer has already been mapped");
            if(aAccessFlags & GL_MAP_READ_BIT){
                if(aAccessFlags & GL_MAP_INVALIDATE_RANGE_BIT) throw std::runtime_error("Buffer : GL_MAP_READ_BIT cannot be used with GL_MAP_INVALIDATE_RANGE_BIT");
                if(aAccessFlags & GL_MAP_INVALIDATE_BUFFER_BIT) throw std::runtime_error("Buffer : GL_MAP_READ_BIT cannot be used with GL_MAP_INVALIDATE_BUFFER_BIT");
            }

            if(aAccessFlags & GL_MAP_FLUSH_EXPLICIT_BIT ){
                if(! (aAccessFlags & GL_MAP_WRITE_BIT)) throw std::runtime_error("Buffer : GL_MAP_FLUSH_EXPLICIT_BIT must be used with GL_MAP_WRITE_BIT");
            }

            const DynamicArray<GLenum> targets = BIND_MAP::GetBoundTargets(this);
            if(targets.IsEmpty()) throw std::runtime_error("Buffer : Buffer is not bound");
            mMapTarget = targets.Back();

            return glMapBufferRange(mMapTarget, aOffset, aBytes, aAccessFlags);
        }

        void InternalFlushMappedRange(const size_t aOffset, const size_t aBytes) const{
            if(mMapTarget == 0) throw std::runtime_error("Buffer : Buffer has not been mapped");
            glFlushMappedBufferRange(mMapTarget, aOffset, aBytes);
        }

        ////

        void InternalReadBarrier(){
            if(mMapTarget == 0) throw std::runtime_error("Buffer : Buffer has not been mapped");
            glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);
            //! \todo Create a fence
        }

        void InternalWriteBarrier(){
            if(mMapTarget == 0) throw std::runtime_error("Buffer : Buffer has not been mapped");
            glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);
        }
    public:
        Buffer():
            mMapTarget(0),
            mBytes(0),
            mID(0)
        {
            glGenBuffers(1, &mID);
        }

        Buffer(const size_t aBytes):
            mMapTarget(0),
            mBytes(aBytes),
            mID(0)
        {
            glGenBuffers(1, &mID);
            Allocate(aBytes);
        }

        Buffer(Buffer&& aOther):
            mMapTarget(aOther.mMapTarget),
            mBytes(aOther.mBytes),
            mID(aOther.mID)
        {
            aOther.mMapTarget = 0;
            aOther.mBytes = 0;
            aOther.mID = 0;
        }

        Buffer(const Buffer& aOther):
            mMapTarget(0),
            mBytes(aOther.mBytes),
            mID(0)
        {
            glGenBuffers(1, &mID);
            Bind(GL_COPY_WRITE_BUFFER);
            Allocate(aOther.mBytes);
            Unbind(GL_COPY_WRITE_BUFFER);
            Copy(aOther, 0, 0, mBytes);
        }

        virtual ~Buffer(){
            if(mID != 0){
                if(mMapTarget != 0) InternalUnmap();
                BIND_STACK.UnbindAll(this);

                glDeleteBuffers(1, &mID);

                mBytes = 0;
                mID = 0;
            }
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

        ////

        void Invalidate(){
            glInvalidateBufferData​(mID);
        }

        void Invalidate(const size_t aOffset, const size_t aBytes){
            glInvalidateBufferSubData​(mID, aOffset, aBytes);
        }

        ////

        void Read(const size_t aOffset, const size_t aBytes, void* const aData){
            Bind(GL_COPY_READ_BUFFER);
            glGetBufferSubData(mID, aOffset, aBytes, aData);
            Unbind(GL_COPY_READ_BUFFER);
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
            Buffer()
        {}

        ImmutableBuffer(const size_t aBytes):
            Buffer(aBytes)
        {}

        ImmutableBuffer(const Buffer& aOther):
            Buffer(aOther)
        {}


        ImmutableBuffer(Buffer&& aOther):
            Buffer(std::move(aOther))
        {}

        ////

        Self& operator=(const Buffer& aOther){
            ~Buffer();
            new(this) Self(aOther);
            return *this;
        }

        Self& operator=(Buffer&& aOther){
            ~Buffer();
            new(this) Self(std::move(aOther));
            return *this;
        }

        ////

        void Allocate(const size_t aBytes) override{
            glBufferStorage(mTarget, aBytes, nullptr, ACCESS_FLAGS);
            mBytes = aBytes;
        }

        void Allocate(const void* const aData, const size_t aBytes) override{
            glBufferStorage(mTarget, aBytes, aData, ACCESS_FLAGS);
            mBytes = aBytes;
        }

        ////

        template<bool R = READ_BIT>
        typename std::enable_if<R, const void*> ReadMap() const{
            return const_cast<Buffer*>(this)->InternalMap(GL_READ_ONLY);
        }

        template<bool R = READ_BIT, bool W = WRITE_BIT>
        typename std::enable_if<W && ! R, void*> WriteMap(){
            return InternalMap(GL_WRITE_ONLY);
        }

        template<bool R = READ_BIT, bool W = WRITE_BIT>
        typename std::enable_if<R && W, void*> WriteMap(){
            return InternalMap(GL_READ_WRITE);
        }

        template<bool R = READ_BIT, bool W = WRITE_BIT>
        typename std::enable_if<R || W, void> Unmap() const{
            InternalUnmap();
        }

        ////

        template<bool R = READ_BIT, bool W = WRITE_BIT>
        typename std::enable_if<R, const void*> ReadMap(const size_t aOffset, const size_t aBytes, const GLenum aAccessFlags = 0) const{
            if((aAccessFlags & GL_WRITE_BIT) && ! W) throw std::runtime_error("ImmutableBuffer : GL_WRITE_BIT set on a read only buffer");
            return InternalMapRange(aOffset, aBytes, aAccessFlags | GL_READ_BIT);
        }

        template<bool R = READ_BIT, bool W = WRITE_BIT>
        typename std::enable_if<W, void*> WriteMap(const size_t aOffset, const size_t aBytes, const GLenum aAccessFlags = 0){
            if((aAccessFlags & GL_READ_BIT) && ! R) throw std::runtime_error("ImmutableBuffer : GL_READ_BIT set on a write only buffer");
            return InternalMapRange(aOffset, aBytes, aAccessFlags | GL_WRITE_BIT);
        }

        template<bool R = READ_BIT, bool W = WRITE_BIT>
        typename std::enable_if<R || W, void> FlushMappedRange(const size_t aOffset, const size_t aBytes) const{
            InternalFlushMappedRange(aOffset, aBytes);
        }

        template<bool R = READ_BIT, bool W = WRITE_BIT>
        typename std::enable_if<R, void> ReadBarrier() const{
            InternalReadBarrier(aOffset, aBytes);
        }

        template<bool R = READ_BIT), bool W = WRITE_BIT>
        typename std::enable_if<W, void> WriteBarrier() const{
            InternalWriteBarrier(aOffset, aBytes);
        }
    };

    template<const GLenum USAGE>
    class MutableBuffer : public Buffer{
    public:
        typedef MutableBuffer<MUTABLE> Self;

        enum : GLenum{
            USAGE_MODE = USAGE
        };

        static constexpr bool CanRead(){
            return
                USAGE == STATIC_READ ? true :
                USAGE == DYNAMIC_READ ? true :
                USAGE == STREAM_READ ? true :
                false;
        }

        static constexpr bool CanWrite(){
            return
                USAGE == STATIC_DRAW ? true :
                USAGE == DYNAMIC_DRAW ? true :
                USAGE == STREAM_DRAW ? true :
                false;
        }
    public:
        MutableBuffer():
            Buffer()
        {}

        MutableBuffer(const size_t aBytes):
            Buffer(aBytes)
        {}

        MutableBuffer(const Buffer& aOther):
            Buffer(aOther)
        {}


        MutableBuffer(Buffer&& aOther):
            Buffer(std::move(aOther))
        {}

        ////

        Self& operator=(const Buffer& aOther){
            ~Buffer();
            new(this) Self(aOther);
            return *this;
        }

        Self& operator=(Buffer&& aOther){
            ~Buffer();
            new(this) Self(std::move(aOther));
            return *this;
        }

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

        void Write(const void* const aData, const size_t aBytes){
            glBufferSubData(mTarget, 0, aBytes, aData);
        }

        void Write(const size_t aOffset, const void* const aData, const size_t aBytes){
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

        ////

        template<bool R = CanRead()>
        typename std::enable_if<R, const void*> ReadMap() const{
            return const_cast<Buffer*>(this)->InternalMap(GL_READ_ONLY);
        }

        template<bool R = CanRead(), bool W = CanWrite()>
        typename std::enable_if<W && ! R, void*> WriteMap(){
            return InternalMap(GL_WRITE_ONLY);
        }

        template<bool R = CanRead(), bool W = CanWrite()>
        typename std::enable_if<R && W, void*> WriteMap(){
            return InternalMap(GL_READ_WRITE);
        }

        template<bool R = CanRead(), bool W = CanWrite()>
        typename std::enable_if<R || W, void> Unmap() const{
            InternalUnmap();
        }

        ////

        template<bool R = CanRead(), bool W = CanWrite()>
        typename std::enable_if<R, const void*> ReadMap(const size_t aOffset, const size_t aBytes, const GLenum aAccessFlags = 0) const{
            if((aAccessFlags & GL_WRITE_BIT) && ! W) throw std::runtime_error("MutableBuffer : GL_WRITE_BIT set on a read only buffer");
            return InternalMapRange(aOffset, aBytes, aAccessFlags | GL_READ_BIT);
        }

        template<bool R = CanRead(), bool W = CanWrite()>
        typename std::enable_if<W, void*> WriteMap(const size_t aOffset, const size_t aBytes, const GLenum aAccessFlags = 0){
            if((aAccessFlags & GL_READ_BIT) && ! R) throw std::runtime_error("MutableBuffer : GL_READ_BIT set on a write only buffer");
            return InternalMapRange(aOffset, aBytes, aAccessFlags | GL_WRITE_BIT);
        }

        template<bool R = CanRead(), bool W = CanWrite()>
        typename std::enable_if<R || W, void> FlushMappedRange(const size_t aOffset, const size_t aBytes) const{
            InternalFlushMappedRange(aOffset, aBytes);
        }

        template<bool R = CanRead(), bool W = CanWrite()>
        typename std::enable_if<R, void> ReadBarrier() const{
            InternalReadBarrier(aOffset, aBytes);
        }

        template<bool R = CanRead(), bool W = CanWrite()>
        typename std::enable_if<W, void> WriteBarrier() const{
            InternalWriteBarrier(aOffset, aBytes);
        }
    };
}}

#endif
