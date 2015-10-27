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

namespace Solaire{ namespace Graphics{

    namespace BufferImplementation{
        static Mutex LOCK;
    }

    template<const bool READ_BIT, const bool WRITE_BIT, const bool DYNAMIC_BIT, const bool PERSISTENT_BIT, const bool COHERENT_BIT, const bool CLIENT_STORAGE_BIT>
    class Buffer{ //: public Utility::Resource{;
    private:
        GLuint mID;
        GLuint mPreviousID;
        bool mIsBound;
    private:
        Buffer(const Buffer& aOther) = delete;
        Buffer(Buffer&& aOther) = delete;
        Buffer& operator=(const Buffer& aOther) = delete;
        Buffer& operator=(Buffer&& aOther) = delete;
    protected:
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

        static constexpr GLenum GetBufferBinding(const GLenum aTarget){
            return
                aTarget == GL_ARRAY_BUFFER ? GL_ARRAY_BUFFER_BINDING :
                aTarget == GL_ELEMENT_ARRAY_BUFFER ? GL_ELEMENT_ARRAY_BUFFER_BINDING :
                aTarget == GL_COPY_READ_BUFFER ? GL_COPY_READ_BUFFER_BINDING :
                aTarget == GL_COPY_WRITE_BUFFER ? GL_COPY_WRITE_BUFFER_BINDING :
                aTarget == GL_PIXEL_PACK_BUFFER ? GL_PIXEL_PACK_BUFFER_BINDING :
                aTarget == GL_PIXEL_UNPACK_BUFFER ? GL_PIXEL_UNPACK_BUFFER_BINDING :
                aTarget == GL_QUERY_BUFFER ? GL_QUERY_BUFFER_BINDING :
                aTarget == GL_TEXTURE_BUFFER ? GL_TEXTURE_BUFFER_BINDING :
                aTarget == GL_TRANSFORM_FEEDBACK_BUFFER ? GL_TRANSFORM_FEEDBACK_BUFFER_BINDING :
                aTarget == GL_UNIFORM_BUFFER ? GL_UNIFORM_BUFFER_BINDING :
                aTarget == GL_DRAW_INDIRECT_BUFFER ? GL_DRAW_INDIRECT_BUFFER_BINDING :
                aTarget == GL_ATOMIC_COUNTER_BUFFER ? GL_ATOMIC_COUNTER_BUFFER_BINDING :
                aTarget == GL_DISPATCH_INDIRECT_BUFFER ? GL_DISPATCH_INDIRECT_BUFFER_BINDING :
                aTarget == GL_SHADER_STORAGE_BUFFER ? GL_SHADER_STORAGE_BUFFER_BINDING :
                0
            ;
        }

        static GLuint GetCurrentlyBoundBuffer(const GLenum aTarget){
            GLuint id = 0;
            glGetIntegerv(GetBufferBinding(aTarget), &id);
            return id;
        }
    protected:
        bool IsBound() const{
            return mIsBound;
        }

        void Bind(const GLenum aTarget){
            BufferImplementation::LOCK.lock();

            if(IsBound()){
                BufferImplementation::LOCK.unlock();
                throw std::runtime_error("Buffer: Buffer is already bound");
            }

            mPreviousID = GetCurrentlyBoundBuffer(aTarget);
            glBindBuffer​(aTarget, mID);
            mIsBound = true;
        }

        void Unbind(const GLenum aTarget){
            if(! IsBound()) throw std::runtime_error("Buffer: Buffer is not bound");

            glBindBuffer​(aTarget, mPreviousID);
            mIsBound = false;

            BufferImplementation::LOCK.unlock();
        }
    public:
        Buffer():
            mID(0),
            mPreviousID(0),
            mIsBound(0)
        {
            glGenBuffers(1, &mID);
        }

        virtual ~Buffer(){
            glDeleteBuffers(1, &mID);
        }
    };
}}

#endif
