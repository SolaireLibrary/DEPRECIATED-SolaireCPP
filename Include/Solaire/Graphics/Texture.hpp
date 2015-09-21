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
\version 1.0
\date
Created			: 20th September 2015
Last Modified	: 20th September 2015
*/

#include <map>
#include <algorithm>
#include <vector>
#include "..\Utility\ResourceFactory.hpp"
#include "..\Maths\Vector2.hpp"
#include "Graphics.inl"

namespace Solaire{ namespace Graphics{

    typedef uint16_t TextureCoord1D;
    typedef Maths::Vector2<uint16_t> TextureCoord2D;
    typedef Maths::Vector3<uint16_t> TextureCoord3D;

    class TextureDataFactory : public  Utility::ResourceFactory{
    public:
        virtual ~TextureDataFactory(){

        }
    };

    class Texture1DFactory : public TextureDataFactory{
    public:
        virtual ~Texture1DFactory(){

        }

        virtual TextureCoord1D Size() const = 0;
        virtual ColourRGBA GetColour(const TextureCoord1D aIndex) const = 0;
    };

    class Texture2DFactory : public TextureDataFactory{
    public:
        virtual ~Texture2DFactory(){

        }

        virtual TextureCoord2D Size() const = 0;
        virtual ColourRGBA GetColour(const TextureCoord2D aIndex) const;

        ColourRGBA GetColour(const TextureCoord2D::value_t aX, const TextureCoord2D::value_t aY) const{
            return GetColour(TextureCoord2D(aX, aY));
        }
    };

    class Texture3DFactory : public TextureDataFactory{
    public:
        virtual ~Texture3DFactory(){

        }

        virtual TextureCoord3D Size() const = 0;
        virtual ColourRGBA GetColour(const TextureCoord3D aIndex) const;

        ColourRGBA GetColour(const TextureCoord3D::value_t aX, const TextureCoord3D::value_t aY, const TextureCoord3D::value_t aZ) const{
            return GetColour(TextureCoord3D(aX, aY, aZ));
        }
    };

    enum class TextureFilter : GLenum{
        NEAREST_NEIGHBOUR = GL_NEAREST,
        LINEAR = GL_LINEAR
    };

    enum class TextureWrap : GLenum{
        REPEAT = GL_REPEAT,
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE
    };

    class TextureParamFactory : public Utility::ResourceFactory{
    public:
        static TextureFilter DEFAULT_FILTER;
        static TextureWrap DEFAULT_WRAP_MODE;

        virtual ~TextureParamFactory(){

        }

        virtual TextureFilter Filter() const{
            return DEFAULT_FILTER;
        }

        virtual TextureWrap WrapMode() const{
            return DEFAULT_WRAP_MODE;
        }

        virtual TextureFilter MipMapFilter() const{
            return DEFAULT_FILTER;
        }

        virtual bool MipmapsEnabled() const{
            return false;
        }

    };

    TextureFilter TextureParamFactory::DEFAULT_FILTER = TextureFilter::NEAREST_NEIGHBOUR;
    TextureWrap TextureParamFactory::DEFAULT_WRAP_MODE = TextureWrap::CLAMP_TO_EDGE;

    //! \TODO Implement Texture classes

    class TextureBase : public Utility::Resource{
    private:
        static std::map<GLenum, std::vector<TextureBase*>> BINDING_MAP;

        static std::vector<TextureBase*>& GetBoundTextures(const GLenum aTarget){
            auto i = BINDING_MAP.find(aTarget);
            if(i == BINDING_MAP.end()){
                i = BINDING_MAP.emplace(aTarget, std::vector<TextureBase*>()).first;
            }
            return i->second;
        }

        static std::vector<GLenum> GetBoundTargets(const TextureBase& aTexture){
            std::vector<GLenum> bindings;
            for(const std::pair<GLenum, std::vector<TextureBase*>>& i : BINDING_MAP){
                auto it = std::find(i.second.begin(), i.second.end(), &aTexture);
                if(it != i.second.end()) bindings.push_back(i.first);
            }
            return bindings;
        }
    protected:
        enum : GLuint{
            INVALID_TEXTURE_ID = 0
        };

        virtual void CreateTexture() = 0;
        virtual void DestroyTexture() = 0;
        virtual bool IsCreated() const = 0;
        virtual GLuint GetTextureID() const = 0;

        void ForceUnbindAll(){
            // Unbind all instances of this texture from the bind stacks
            const std::vector<GLenum> list = GetBoundTargets(*this);
            for(const GLenum target : list){
                std::vector<TextureBase*>& bindings = GetBoundTextures(target);
                    while(bindings.back() == this){
                        Unbind(target);
                    }
                auto it = std::find(bindings.begin(), bindings.end(), this);
                while(it != bindings.end()){
                    bindings.erase(it);
                    OnForcedUnbind(target);
                    it = std::find(bindings.begin(), bindings.end(), this);
                }
            }
        }

        virtual void OnForcedUnbind(const GLenum aTarget) = 0;

    public:
        virtual ~TextureBase(){
            if(IsCreated()){
                DestroyTexture();
            }
        }

        virtual void Bind(const GLenum aTarget){
            if(IsCreated()){
                GetBoundTextures(aTarget).push_back(this);
                //glBindTexture(aTarget, getTextureID());
            }else{
                throw std::runtime_error("Texture must be created before it can be bound");
            }
        }

       virtual void Unbind(const GLenum aTarget){
            if(IsCreated()){
                std::vector<TextureBase*>& list = GetBoundTextures(aTarget);
                if(list.back() != this) throw std::runtime_error("This texture is not at the top of the binding stack for this target");
                list.pop_back();
                //glBindTexture(aTarget, list.empty() ? INVALID_TEXTURE_ID : list.back());
            }else{
                throw std::runtime_error("Texture must be created before it can be unbound");
            }
        }

        virtual bool IsBound(const GLenum aTarget) const{
            const std::vector<TextureBase*>& list = GetBoundTextures(aTarget);
            return std::find(list.begin(), list.end(), this) != list.end();
        }

        // Inherited from Resource
        virtual void Reload() override{
            if(IsCreated()){
                const std::vector<GLenum> list = GetBoundTargets(*this);
                for(const GLenum i : list) Unbind(i);
                DestroyTexture();
                CreateTexture();
                for(const GLenum i : list) Bind(i);
            }
        }
    };

    class TextureBindGuard{
    private:
        TextureBase& mTexture;
        const GLenum mTarget;
    public:
        TextureBindGuard(TextureBase& aTexture, const GLenum aTarget) :
            mTexture(aTexture),
            mTarget(aTarget)
        {
            mTexture.Bind(mTarget);
        }

        ~TextureBindGuard(){
            mTexture.Unbind(mTarget);
        }
    };

    class Texture1D : public TextureBase{
    private:
        Texture1DFactory* mDataFactory;
        TextureParamFactory* mParamFactory;
    public:
        virtual ~Texture1D(){

        }
    };

    class Texture2D : public TextureBase{
    private:
        Texture2DFactory* mDataFactory;
        TextureParamFactory* mParamFactory;

        GLuint mID;
    protected:
        // Inherited from TextureBase

        GLuint GetTextureID() const override{
            return mID;
        };

        void OnForcedUnbind(const GLenum aTarget) override{

        }

        void CreateTexture() override{

            const TextureCoord2D size = mDataFactory->Size();
            ColourRGBA* const pixels = new ColourRGBA[size.X * size.Y];

            TextureCoord2D i(0,0);
            for(i.X; i.X < size.X; ++i.X){
                for(i.Y; i.Y < size.Y; ++i.Y){
                    //! \TODO Check row major order
                    pixels[i.X + size.X * size.Y] = mDataFactory->GetColour(i);
                }
            }

            //glGenTextures(1& mID);

            TextureBindGuard guard(*this, GL_TEXTURE_2D);

            //glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, size.X, size.Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

            delete[] pixels;

            const TextureFilter filter = mParamFactory->Filter();
            if(mParamFactory->MipmapsEnabled()){
                TextureFilter mipmapFilter = mParamFactory->MipMapFilter();

                if(mipmapFilter == TextureFilter::NEAREST_NEIGHBOUR){
                    if(filter == TextureFilter::NEAREST_NEIGHBOUR){
                        mipmapFilter = static_cast<TextureFilter>(GL_NEAREST_MIPMAP_NEAREST);
                    }else if(filter == TextureFilter::LINEAR){
                        mipmapFilter = static_cast<TextureFilter>(GL_NEAREST_MIPMAP_LINEAR);
                    }
                }else if(mipmapFilter == TextureFilter::LINEAR){
                    if(filter == TextureFilter::NEAREST_NEIGHBOUR){
                        mipmapFilter = static_cast<TextureFilter>(GL_LINEAR_MIPMAP_NEAREST);
                    }else if(filter == TextureFilter::LINEAR){
                        mipmapFilter = static_cast<TextureFilter>(GL_LINEAR_MIPMAP_LINEAR);
                    }
                }
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(filter));
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(mipmapFilter));
                //glGenerateMipmap(GL_TEXTURE_2D);
            }else{
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(filter));
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(filter));
            }
            const TextureWrap wrapMode = mParamFactory->WrapMode();
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(wrapMode));
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLenum>(wrapMode));

        }

        void DestroyTexture() override{
            ForceUnbindAll();
            //glDeleteTextures (1, &mID);
        }

        bool IsCreated() const override{
            return mID != INVALID_TEXTURE_ID;
        }

    public:
        virtual ~Texture2D(){

        }

    };

    class Texture3D : public TextureBase{
    private:
        Texture3DFactory* mDataFactory;
        TextureParamFactory* mParamFactory;
    public:
        virtual ~Texture3D(){

        }
    };

    class CubeMap : public TextureBase{
    private:
        Texture2DFactory* mFront;
        Texture2DFactory* mBack;
        Texture2DFactory* mLeft;
        Texture2DFactory* mRight;
        Texture2DFactory* mTop;
        Texture2DFactory* mBottom;
        TextureParamFactory* mParamFactory;
    public:
        virtual ~CubeMap(){

        }
    };
}}

#endif
