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
        GLint mID;
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
    protected:
        void Bind(const GLenum aTarget);
        void Unbind(const GLenum aTarget);
    public:
        Buffer():
            mID(0)
        {
            glGenBuffers(1, &mID);
        }

        virtual ~Buffer(){
            glDeleteBuffers(1, &mID);
        }
    };
}}

#endif
