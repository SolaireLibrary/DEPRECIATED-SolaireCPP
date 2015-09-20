#ifndef SOLAIRE_GRAPHICS_IMAGE_HPP
#define SOLAIRE_GRAPHICS_IMAGE_HPP

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
\file Image.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 20th September 2015
Last Modified	: 20th September 2015
*/

#include <functional>
#include "Texture.hpp"

namespace Solaire{ namespace Graphics{

    template<class FORMAT>
    class Image{
    public:
        typedef FORMAT pixel_t;
        typedef std::function<pixel_t(const pixel_t)> Filter;
    private:
        Image(const Image<pixel_t>& aImage) = delete;
        Image<pixel_t>& operator=(const Image<pixel_t>& aImage) = delete;

        pixel_t* mPixels;
        size_t mPixelCount;
    protected:
        pixel_t* GetPixels(){
            return mPixels;
        }

        const pixel_t* GetPixels() const{
            return mPixels;
        }

        Image<pixel_t>& operator=(Image<pixel_t>&& aOther){
            std::swap(mPixels, aOther.mPixels);
            std::swap(mPixelCount, aOther.mPixelCount);
            return *this;
        }
    public:
        Image(const size_t aPixelCount) :
            mPixels(new pixel_t[aPixelCount]),
            mPixelCount(aPixelCount)
        {
            for(size_t i = 0; i < aPixelCount; ++i){
                mPixels[i] = DefaultColour<pixel_t>();
            }
        }

        Image(Image<pixel_t>&& aOther) :
           mPixels(aOther.mPixels),
           mPixelCount(aOther.mPixelCount)
        {
            aOther.mPixels = nullptr;
            aOther.mPixelCount = 0;
        }

        virtual ~Image(){
            if(mPixels != nullptr) delete[] mPixels;
        }

        void Apply(const Filter& aFilter){
            const pixel_t* const end = mPixels + mPixelCount;
            pixel_t* i = mPixels;
            while(i != end){
                *i = aFilter(*i);
                ++i;
            }
        }
    };

    /*class Image1D : public Image<ColourRGBA>, public Texture1DFactory{
    private:

    public:
        virtual ~Image1D(){

        }
    };*/

    template<class FORMAT>
    class Image2D : public Image<FORMAT>, public Texture2DFactory{
    private:
        TextureCoord2D mSize;

        size_t EncodeCoords(const TextureCoord2D aCoords) const{
            return aCoords.X + mSize.X * aCoords.Y;
        }
    protected:
        // Inherited from Texture2DFactory

        void Lock() override{

        }

        void Unlock() override{

        }
    public:
        Image2D(const TextureCoord2D aSize) :
            Image<FORMAT>(aSize.X * aSize.Y),
            mSize(aSize)
        {}

        Image2D(Image2D<FORMAT>&& aOther) :
           Image<FORMAT>(aOther),
           mSize(aOther.mSize)
        {}

       Image2D<FORMAT>& operator=(Image2D<FORMAT>&& aOther){
            Image<FORMAT>::operator=(std::move(aOther));
            std::swap(mSize, aOther.mSize);
        }

        void Set(const TextureCoord2D aCoords, const FORMAT aPixel){
            Image<FORMAT>::GetPixels()[EncodeCoords(aCoords)] = aPixel;
        }

        FORMAT& Get(const TextureCoord2D aCoords){
            return Image<FORMAT>::GetPixels()[EncodeCoords(aCoords)];
        }

        FORMAT Get(const TextureCoord2D aCoords) const{
            return Image<FORMAT>::GetPixels()[EncodeCoords(aCoords)];
        }

        FORMAT& operator[](const TextureCoord2D aCoords){
            return Get(aCoords);
        }

        FORMAT operator[](const TextureCoord2D aCoords) const{
            return Get(aCoords);
        }

        void FlipHorisontal(){
            //! \TODO Implement
            throw std::runtime_error("Not implemented");
        }

        void FlipVertical(){
            //! \TODO Implement
            throw std::runtime_error("Not implemented");
        }

        void RotateClockwise(){
            //! \TODO Implement
            throw std::runtime_error("Not implemented");
        }

        void RotateCounterClockwise(){
            //! \TODO Implement
            throw std::runtime_error("Not implemented");
        }

        // Inherited from Texture2DFactory

        TextureCoord2D Size() const override{
            return mSize;
        }

        ColourRGBA GetColour(const TextureCoord2D aIndex) const override{
            return Get(aIndex);
        }
    };

    /*class Image3D : public Image<ColourRGBA>, public Texture3DFactory{
    public:
        virtual ~Image3D(){

        }
    };*/

}}

#endif
