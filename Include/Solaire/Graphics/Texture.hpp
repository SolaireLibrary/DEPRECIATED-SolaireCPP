#ifndef SOLAIRE_GRAPHICS_TEXTURE_HPP
#define SOLAIRE_GRAPHICS_TEXTURE_HPP

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
\file Texture.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 2.0
\date
Created			: 20th September 2015
Last Modified	: 3rd November 2015
*/

#include <map>
#include <algorithm>
#include <vector>
#include "..\Utility\ResourceFactory.hpp"
#include "..\Maths\Vector2.hpp"
#include "Graphics.inl"
#include "GLBindStack.hpp"

namespace Solaire{ namespace Graphics{

    //! \todo Stencil Textures
    //! \todo Mipmap specification
    //! \todo Mipmap generation
    //! \todo Texture Units
    //! \todo Texture Storage

    //! \todo GL_TEXTURE_1D
    //! \todo GL_TEXTURE_2D
    //! \todo GL_TEXTURE_3D
    //! \todo GL_TEXTURE_RECTANGLE
    //! \todo GL_TEXTURE_BUFFER
    //! \todo GL_TEXTURE_CUBE_MAP
    //! \todo GL_TEXTURE_1D_ARRAY
    //! \todo GL_TEXTURE_2D_ARRAY
    //! \todo GL_TEXTURE_CUBE_MAP_ARRAY
    //! \todo GL_TEXTURE_2D_MULTISAMPLE
    //! \todo GL_TEXTURE_2D_MULTISAMPLE_ARRAY

    //! \todo GL_TEXTURE_COMPARE_FUNC
    //! \todo GL_TEXTURE_COMPARE_MODE
    //! \todo GL_TEXTURE_BORDER_COLOR
    //! \todo GL_TEXTURE_MIN_LOD
    //! \todo GL_TEXTURE_MAX_LOD
    //! \todo GL_TEXTURE_LOD_BIAS
    //! \todo GL_TEXTURE_MAX_LEVEL
    //! \todo GL_TEXTURE_BASE_LEVEL
    //! \todo GL_DEPTH_STENCIL_TEXTURE_MODE

    class Texture{
    public:
        enum class Channel : GLint{
            RED = GL_RED,
            GREEN = GL_GREEN,
            BLUE = GL_BLUE,
            ALPHA = GL_ALPHA,
            ZERO = GL_ZERO,
            ONE = GL_ONE
        };

        enum class Filter : GLenum{
            NEAREST = GL_NEAREST,
            LINEAR = GL_LINEAR
        };

        enum class Wrap : GLenum{
            CLAMP_TO_BORDER = CLAMP_TO_BORDER,
            CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
            MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
            MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
            REPEAT = GL_REPEAT,
        };
    protected:
        static GLBindStack BIND_STACK;
    private:
        GLint mSwizzle[4];
        GLuint mID;
        GLenum mMinFilter;
        GLenum mMagfilter;
        GLenum mTWrap;
        GLenum mSWrap;
    protected:
        static constexpr bool CanHaveMipMaps(const GLenum aTarget){
            return
                aTarget == GL_TEXTURE_1D ?                      true :
                aTarget == GL_TEXTURE_2D ?                      true :
                aTarget == GL_TEXTURE_3D ?                      true :
                aTarget == GL_TEXTURE_1D_ARRAY ?                true :
                aTarget == GL_TEXTURE_2D_ARRAY ?                true :
                aTarget == GL_TEXTURE_CUBE_MAP ?                true :
                aTarget == GL_TEXTURE_CUBE_MAP_ARRAY ?          true :
                aTarget == GL_TEXTURE_RECTANGLE ?               false :
                aTarget == GL_TEXTURE_BUFFER ?                  false :
                aTarget == GL_TEXTURE_2D_MULTISAMPLE ?          false :
                aTarget == GL_TEXTURE_2D_MULTISAMPLE_ARRAY ?    false :
                false;
        }

        static constexpr bool HasArrayLayers(const GLenum aTarget){
            return
                aTarget == GL_TEXTURE_1D ?                      false :
                aTarget == GL_TEXTURE_2D ?                      false :
                aTarget == GL_TEXTURE_3D ?                      false :
                aTarget == GL_TEXTURE_1D_ARRAY ?                true :
                aTarget == GL_TEXTURE_2D_ARRAY ?                true :
                aTarget == GL_TEXTURE_CUBE_MAP ?                false :
                aTarget == GL_TEXTURE_CUBE_MAP_ARRAY ?          true :
                aTarget == GL_TEXTURE_RECTANGLE ?               false :
                aTarget == GL_TEXTURE_BUFFER ?                  false :
                aTarget == GL_TEXTURE_2D_MULTISAMPLE ?          false :
                aTarget == GL_TEXTURE_2D_MULTISAMPLE_ARRAY ?    true :
                false;
        }

        static constexpr bool HasCubemapFaces(const GLenum aTarget){
            return
                aTarget == GL_TEXTURE_1D ?                      false :
                aTarget == GL_TEXTURE_2D ?                      false :
                aTarget == GL_TEXTURE_3D ?                      false :
                aTarget == GL_TEXTURE_1D_ARRAY ?                false :
                aTarget == GL_TEXTURE_2D_ARRAY ?                false :
                aTarget == GL_TEXTURE_CUBE_MAP ?                true :
                aTarget == GL_TEXTURE_CUBE_MAP_ARRAY ?          true :
                aTarget == GL_TEXTURE_RECTANGLE ?               false :
                aTarget == GL_TEXTURE_BUFFER ?                  false :
                aTarget == GL_TEXTURE_2D_MULTISAMPLE ?          false :
                aTarget == GL_TEXTURE_2D_MULTISAMPLE_ARRAY ?    false :
                false;
        }

        static constexpr uint8_t GetDimentions(const GLenum aTarget){
            return
                aTarget == GL_TEXTURE_1D ?                      1 :
                aTarget == GL_TEXTURE_2D ?                      2 :
                aTarget == GL_TEXTURE_3D ?                      3 :
                aTarget == GL_TEXTURE_1D_ARRAY ?                1 :
                aTarget == GL_TEXTURE_2D_ARRAY ?                2 :
                aTarget == GL_TEXTURE_CUBE_MAP ?                2 :
                aTarget == GL_TEXTURE_CUBE_MAP_ARRAY ?          2 :
                aTarget == GL_TEXTURE_RECTANGLE ?               2 :
                aTarget == GL_TEXTURE_BUFFER ?                  1 :
                aTarget == GL_TEXTURE_2D_MULTISAMPLE ?          2 :
                aTarget == GL_TEXTURE_2D_MULTISAMPLE_ARRAY ?    2 :
                0;
        }
    private:
        Texture(const Texture&) = delete;
        Texture(Texture&&) = delete;
        Texture& operator=(const Texture&) = delete;
        Texture& operator=(Texture&&) = delete;
    public:
        Texture();
        virtual ~Texture();

        GLuint GetID() const;

        void Create();
        void Destroy();

        void SetMinFilter(const Filter aFilter);
        Filter GetMinFilter() const;

        void SetMaxFilter(const Filter aFilter);
        Filter GetMaxFilter() const;

        void SetFilter(const Filter aFilter);

        virtual GLEnum GetTarget() const = 0;

        virtual GLuint GetMipmapLevels() const = 0;
        virtual Filter GetMipmapFilter() = 0;

        void SetWrapT(const Wrap aWrap);
        Wrap GetWrapT() const;

        void SetWrapR(const Wrap aWrap);
        Wrap GetWrapR() const;

        void SetWrap(const Wrap aWrap);
    };

    class MipmapedTexture : public Texture{
    private:
        GLuint mLevels;
        Filter mFilter;
    public:
        virtual ~MipmapedTexture(){}

        void SetMipmapLevels(const GLuint aLevels) const;
        void SetMipmapFilter(const Filter aFilter);

        // Inherited from Texture

        GLuint GetMipmapLevels() const override;
        Filter GetMipmapFilter() override;
    };

    class NonMipmapedTexture : public Texture{
    public:
        virtual ~NonMipmapedTexture(){}

        // Inherited from Texture

        GLuint GetMipmapLevels() const override;
        Filter GetMipmapFilter() override;
    };

    class Texture1D : public MipmappedTexture{};
    class Texture2D : public MipmappedTexture{};
    class Texture3D : public MipmappedTexture{};
    class TextureRectangle : public NonMipmapedTexture{};
    class TextureBuffer : public NonMipmapedTexture{};
    class TextureCubemap : public MipmapedTexture{};
    class Texture2DMultisample : public NonMipmapedTexture{};

    class Texture1DArray : public MipmapedTexture{};
    class Texture2DArray : public MipmapedTexture{};
    class Texture3DArray : public MipmapedTexture{};
    class TextureCubemapArray : public MipmapedTexture{};
    class Texture2DMultisampleArray : public NonMipmapedTexture{};

#endif
