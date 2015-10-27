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

    #define SOLAIRE_BUFFER_ENABLE_IF(aType, aCondition) template<const bool B = (aCondition)> typename std:enable_if<B, aType>::type
    #define SOLAIRE_BUFFER_ENABLE_IF_NOT(aType, aCondition) template<const bool B = !(aCondition)> typename std:enable_if<B, aType>::type

    //! \todo Buffer Mapping
    //! \todo Buffer Alignment
    //! \todo Buffer Write Synchronization
    //! \todo Buffer Read Synchronization
    //! \todo Buffer Invalidation
    //! \todo Buffer Streaming

    class BufferIDInterface{
    public:
        virtual ~BufferIDInterface(){}
        virtual GLuint GetID() const = 0;
    };

    namespace BufferImplementation{
        static void BindFn(const GLenum aTarget, const void* const aObject){
            glBindBuffer​(aTarget, static_cast<BufferIDInterface*>(aObject)->GetID());
        }

        static GLBindStack BIND_STACK(GetDefaultAllocator(), &BindFn, {
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
        });
    }

    template<const bool MUTABLE, const GLenum USAGE, const bool READ_BIT, const bool WRITE_BIT, const bool DYNAMIC_BIT, const bool PERSISTENT_BIT, const bool COHERENT_BIT, const bool CLIENT_STORAGE_BIT>
    class Buffer : public BufferIDInterface{ //: public Utility::Resource{;
    public:
        typedef Buffer<MUTABLE, USAGE, READ_BIT, WRITE_BIT, DYNAMIC_BIT, PERSISTENT_BIT, COHERENT_BIT, CLIENT_STORAGE_BIT> Self;

        enum class ImmutableFlags : bool{
            READ = READ_BIT,
            WRITE = WRITE_BIT,
            DYNAMIC = DYNAMIC_BIT,
            PERSISTENT = PERSISTENT_BIT,
            COHERENT = COHERENT_BIT,
            CLIENT_STORAGE = CLIENT_STORAGE_BIT
        };

        enum MutableUsage : GLenum{
            USAGE_MODE = USAGE
        };
    private:
        GLuint mBytes;
        GLuint mID;
    private:

        static constexpr GLint GetAccessFlags(){
            return
                (READ_BIT           ? GL_MAP_READ_BIT           : 0) |
                (WRITE_BIT          ? GL_MAP_WRITE_BIT          : 0) |
                (DYNAMIC_BIT        ? GL_DYNAMIC_STORAGE_BIT    : 0) |
                (PERSISTENT_BIT     ? GL_PERSISTENT_BIT         : 0) |
                (COHERENT_BIT       ? GL_COHERENT_BIT           : 0) |
                (CLIENT_STORAGE_BIT ? GL_CLIENT_STORAGE_BIT     : 0)
            ;
        }
    public:
        Buffer():
            mBytes(0),
            mID(0)
        {
            glGenBuffers(1, &mID);
        }

        Buffer(const size_t aBytes):
            mBytes(0),
            mID(0)
        {
            glGenBuffers(1, &mID);

            Bind(GL_COPY_WRITE_BUFFER);
            Allocate(aBytes);
            Unbind(GL_COPY_WRITE_BUFFER);
        }

        Buffer(const void* const aData, const size_t aBytes):
            mBytes(0),
            mID(0)
        {
            glGenBuffers(1, &mID);

            Bind(GL_COPY_WRITE_BUFFER);
            AllocateAndCopy(aData, aBytes);
            Unbind(GL_COPY_WRITE_BUFFER);
        }

        template<const bool MUTABLE2, const GLenum USAGE2, const bool READ_BIT2, const bool WRITE_BIT2, const bool DYNAMIC_BIT2, const bool PERSISTENT_BIT2, const bool COHERENT_BIT2, const bool CLIENT_STORAGE_BIT2>
        Buffer(const Buffer<MUTABLE2, USAGE, READ_BIT2, WRITE_BIT2, DYNAMIC_BIT2, PERSISTENT_BIT2, COHERENT_BIT2, CLIENT_STORAGE_BIT2>& aOther):
            mBytes(0),
            mID(0)
        {
            glGenBuffers(1, &mID);
            Bind(GL_COPY_WRITE_BUFFER);
            Allocate(aOther.mBytes);
            Unbind(GL_COPY_WRITE_BUFFER);
            Copy(aOther, 0, 0, mBytes);
        }

        Buffer(Self&& aOther):
            mBytes(aOther.mBytes),
            mID(aOther.mID)
        {
            aOther.mBytes = 0;
            aOther.mID = 0;
        }

        ~Buffer(){
            if(mID != 0){
                BufferImplementation::BIND_STACK::UNBIND_ALL(this);

                glDeleteBuffers(1, &mID);

                mBytes = 0;
                mID = 0;
            }
        }

        template<const bool MUTABLE2, const GLenum USAGE2, const bool READ_BIT2, const bool WRITE_BIT2, const bool DYNAMIC_BIT2, const bool PERSISTENT_BIT2, const bool COHERENT_BIT2, const bool CLIENT_STORAGE_BIT2>
        Self& operator=(const Buffer<MUTABLE2, USAGE2, READ_BIT2, WRITE_BIT2, DYNAMIC_BIT2, PERSISTENT_BIT2, COHERENT_BIT2, CLIENT_STORAGE_BIT2>& aOther){
            if(mBytes < aOther.mBytes){
                Allocate(aOther.mBytes);
            }

            Copy(aOther, 0, 0, aOther.mBytes);
            return *this;
        }

        Self& operator=(Self&& aOther){
            ~Buffer();

            mBytes = aOther.mBytes;
            mID = aOther.mID;

            aOther.mBytes = 0;
            aOther.mID = 0;

            return *this;
        }

        ////

        GLuint GetID() const override{
            return mID();
        }

        GLuint Size() const{
            return mBytes;
        }

        ////

        bool IsBound(const GLenum aTarget) const{
            const DynamicArray<GLEnum targets> targets = BufferImplementation::BIND_STACK::GetBoundTargets(this);
            return targets.FindFirst(aTarget) != targets.end();
        }

        void Bind(const GLenum aTarget){
            BufferImplementation::BIND_STACK::Bind(aTarget, this);
        }

        void Unbind(){
            BufferImplementation::BIND_STACK::Unbind(aTarget, this);
        }

        ////

        SOLAIRE_BUFFER_ENABLE_IF(void, MUTABLE) AllocateAndCopy(const void* const aData, const size_t aBytes){
            glBufferData(mTarget, aBytes, aData, USAGE);
            mBytes = aBytes;
        }

        SOLAIRE_BUFFER_ENABLE_IF(void, MUTABLE) AllocateAndCopy(const void* const aData, const size_t aBytes){
            glBufferStorage(mTarget, aBytes, aData, GetAccessFlags());
            mBytes = aBytes;
        }

        SOLAIRE_BUFFER_ENABLE_IF_NOT(void, MUTABLE) Allocate(const size_t aBytes){
            glBufferData(mTarget, aBytes, nullptr, USAGE);
            mBytes = aBytes;
        }

       SOLAIRE_BUFFER_ENABLE_IF_NOT(void, MUTABLE) Allocate(const size_t aBytes){
            glBufferStorage(mTarget, aBytes, nullptr, GetAccessFlags());
            mBytes = aBytes;
        }

        ////

        SOLAIRE_BUFFER_ENABLE_IF(void, MUTABLE || DYNAMIC_BIT) Buffer(const void* const aData, const size_t aBytes){
            glBufferSubData(mTarget, 0, aBytes, aData);
        }

        SOLAIRE_BUFFER_ENABLE_IF(void, MUTABLE || DYNAMIC_BIT) OffsetBuffer(const size_t aOffset, const void* const aData, const size_t aBytes){
            glBufferSubData(mTarget, aOffset, aBytes, aData);
        }

        ////

        SOLAIRE_BUFFER_ENABLE_IF(void, MUTABLE) Clear(const GLubyte aValue, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, mBytes, aFormat, GL_UNSIGNED_BYTE, &aValue);
        }

        SOLAIRE_BUFFER_ENABLE_IF(void, MUTABLE) Clear(const GLbyte aValue, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, mBytes, aFormat, GL_BYTE, &aValue);
        }

        SOLAIRE_BUFFER_ENABLE_IF(void, MUTABLE) Clear(const GLushort aValue, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, mBytes, aFormat, GL_UNSIGNED_SHORT, &aValue);
        }

        SOLAIRE_BUFFER_ENABLE_IF(void, MUTABLE) Clear(const GLshort aValue, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, mBytes, aFormat, GL_SHORT, &aValue);
        }

        SOLAIRE_BUFFER_ENABLE_IF(void, MUTABLE) Clear(const GLuint aValue, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, mBytes, aFormat, GL_UNSIGNED_INT, &aValue);
        }

        SOLAIRE_BUFFER_ENABLE_IF(void, MUTABLE) Clear(const GLint aValue, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, mBytes, aFormat, GL_INT, &aValue);
        }

        ////

        SOLAIRE_BUFFER_ENABLE_IF(void, MUTABLE) SubClear(const GLubyte aValue, const size_t aOffset, const size_t aBytes, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, aOffset, aBytes, aFormat, GL_UNSIGNED_BYTE, &aValue);
        }

        SOLAIRE_BUFFER_ENABLE_IF(void, MUTABLE) SubClear(const GLbyte aValue, const size_t aOffset, const size_t aBytes, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, aOffset, aBytes, aFormat, GL_BYTE, &aValue);
        }

        SOLAIRE_BUFFER_ENABLE_IF(void, MUTABLE) SubClear(const GLushort aValue, const size_t aOffset, const size_t aBytes, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, aOffset, aBytes, aFormat, GL_UNSIGNED_SHORT, &aValue);
        }

        SOLAIRE_BUFFER_ENABLE_IF(void, MUTABLE) SubClear(const GLshort aValue, const size_t aOffset, const size_t aBytes, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, aOffset, aBytes, aFormat, GL_SHORT, &aValue);
        }

        SOLAIRE_BUFFER_ENABLE_IF(void, MUTABLE) SubClear(const GLuint aValue, const size_t aOffset, const size_t aBytes, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, aOffset, aBytes, aFormat, GL_UNSIGNED_INT, &aValue);
        }

        SOLAIRE_BUFFER_ENABLE_IF(void, MUTABLE) SubClear(const GLint aValue, const size_t aOffset, const size_t aBytes, const GLenum aInternalFormat){
            glClearBufferData(mTarget, aInternalFormat, aOffset, aBytes, aFormat, GL_INT, &aValue);
        }

        ////
        template<const bool MUTABLE2, const GLenum USAGE2, const bool READ_BIT2, const bool WRITE_BIT2, const bool DYNAMIC_BIT2, const bool PERSISTENT_BIT2, const bool COHERENT_BIT2, const bool CLIENT_STORAGE_BIT2>
        void Copy(const Buffer<MUTABLE2, USAGE, READ_BIT2, WRITE_BIT2, DYNAMIC_BIT2, PERSISTENT_BIT2, COHERENT_BIT2, CLIENT_STORAGE_BIT2>& aOther, const size_t aReadOffet, const size_t aWriteOffset, const size_t aBytes){

            Buffer<MUTABLE2, USAGE, READ_BIT2, WRITE_BIT2, DYNAMIC_BIT2, PERSISTENT_BIT2, COHERENT_BIT2, CLIENT_STORAGE_BIT2>& other =
                const_cast<Buffer<MUTABLE2, USAGE, READ_BIT2, WRITE_BIT2, DYNAMIC_BIT2, PERSISTENT_BIT2, COHERENT_BIT2, CLIENT_STORAGE_BIT2>&>(aOther);

            mBytes = aOther.mBytes;
            Allocate(mBytes);

            other.Bind(GL_COPY_READ_BUFFER);
            Bind(GL_COPY_WRITE_BUFFER);

            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, aReadOffet, aWriteOffset, mBytes);

            other.Unbind(GL_COPY_READ_BUFFER);
            Unbind(GL_COPY_WRITE_BUFFER);
        }
    };

    #undef SOLAIRE_BUFFER_ENABLE_IF
    #undef SOLAIRE_BUFFER_ENABLE_IF_NOT

    template<const GLenum USAGE>
    using MutableBuffer = Buffer<true, USAGE, false, false, false, false, false, false>;

    template<const bool READ_BIT, const bool WRITE_BIT, const bool DYNAMIC_BIT, const bool PERSISTENT_BIT, const bool COHERENT_BIT, const bool CLIENT_STORAGE_BIT>
    using ImmutableBuffer = Buffer<false, 0, READ_BIT, WRITE_BIT, DYNAMIC_BIT, PERSISTENT_BIT, COHERENT_BIT, CLIENT_STORAGE_BIT>;
}}

#endif
