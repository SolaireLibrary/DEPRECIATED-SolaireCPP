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
Last Modified	: 21st September 2015
*/

#include <algorithm>
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

    template<class FORMAT, class INDEX, class FACTORY>
    class ImageND : public Image<FORMAT>, public FACTORY{
    public:
        typedef ImageND<FORMAT, INDEX, FACTORY> ImageNDTemplate;
    private:
        INDEX mSize;
    protected:
        virtual size_t Mutiply(const INDEX aCoords) const = 0;
        virtual size_t EncodeCoords(const INDEX aCoords) const = 0;

        // Inherited from FACTORY

        void Lock() override{

        }

        void Unlock() override{

        }
    public:
        ImageND(const FORMAT aSize) :
            Image<FORMAT>(Mutiply(aSize)),
            mSize(aSize)
        {}

        ImageND(ImageNDTemplate&& aOther) :
           Image<FORMAT>(std::move(aOther)),
           mSize(aOther.mSize)
        {}

       ImageNDTemplate& operator=(ImageNDTemplate&& aOther){
            Image<FORMAT>::operator=(std::move(aOther));
            std::swap(mSize, aOther.mSize);
        }

        void Set(const INDEX aCoords, const FORMAT aPixel){
            Image<FORMAT>::GetPixels()[EncodeCoords(aCoords)] = aPixel;
        }

        FORMAT& Get(const INDEX aCoords){
            return Image<FORMAT>::GetPixels()[EncodeCoords(aCoords)];
        }

        FORMAT Get(const INDEX aCoords) const{
            return Image<FORMAT>::GetPixels()[EncodeCoords(aCoords)];
        }

        FORMAT& operator[](const INDEX aCoords){
            return Get(aCoords);
        }

        FORMAT operator[](const INDEX aCoords) const{
            return Get(aCoords);
        }

        // Inherited from Texture2DFactory

        INDEX Size() const override{
            return mSize;
        }

        ColourRGBA GetColour(const INDEX aIndex) const override{
            return Get(aIndex);
        }
    };

    template<class FORMAT>
    class Image1D : public ImageND<FORMAT, TextureCoord1D, Texture1DFactory>{
    public:
        typedef ImageND<FORMAT, TextureCoord1D, Texture1DFactory> ImageNDTemplate;
    protected:
        // Inherited from ImageND

       size_t Mutiply(const TextureCoord1D aCoords) const override{
            return aCoords;
        }

        size_t EncodeCoords(const TextureCoord1D aCoords) const override{
            return aCoords;
        }
    public:
        Image1D(const TextureCoord1D aSize) :
            ImageNDTemplate(aSize)
        {}

        Image1D(Image1D<FORMAT>&& aOther) :
           ImageNDTemplate(std::move(aOther))
        {}

        void Flip(){
            FORMAT* const begin = ImageNDTemplate::GetPixels();
            FORMAT* const last = begin + (ImageNDTemplate::Size() - 1);
            std::reverse(begin, last);
        }
    };

    template<class FORMAT>
    class Image2D : public ImageND<FORMAT, TextureCoord2D, Texture2DFactory>{
    public:
        typedef ImageND<FORMAT, TextureCoord2D, Texture2DFactory> ImageNDTemplate;
    protected:
        // Inherited from ImageND

       size_t Mutiply(const TextureCoord2D aCoords) const override{
            return aCoords.X * aCoords.Y;
        }

        size_t EncodeCoords(const TextureCoord2D aCoords) const override{
            //! \TODO Check if this is correct
            return aCoords.X + ImageNDTemplate::Size().X * aCoords.Y;
        }
    public:
        Image2D(const TextureCoord2D aSize) :
            ImageNDTemplate(aSize)
        {}

        Image2D(Image2D<FORMAT>&& aOther) :
           ImageNDTemplate(std::move(aOther))
        {}

        void FlipHorisontal(){
            const TextureCoord2D size = ImageNDTemplate::Size();
            TextureCoord2D i(0, 0);
            for(i.Y = 0; i.Y < size.Y; ++i.Y){
                FORMAT* const begin = &ImageNDTemplate::Get(i);
                FORMAT* const last = begin + (size.X - 1);
                std::reverse(begin, last);
            }
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
    };

    template<class FORMAT>
    class Image3D : public ImageND<FORMAT, TextureCoord3D, Texture3DFactory>{
    public:
        typedef ImageND<FORMAT, TextureCoord3D, Texture3DFactory> ImageNDTemplate;
    protected:
        // Inherited from ImageND

       size_t Mutiply(const TextureCoord3D aCoords) const override{
            return aCoords.X * aCoords.Y * aCoords.Z;
        }

        size_t EncodeCoords(const TextureCoord3D aCoords) const override{
            const TextureCoord3D size = ImageNDTemplate::Size();
            //! \TODO Check if this is correct
            return aCoords.X + size.X * aCoords.Y + size.Y * aCoords.Z;
        }
    public:
        Image3D(const TextureCoord3D aSize) :
            ImageNDTemplate(aSize)
        {}

        Image3D(Image3D<FORMAT>&& aOther) :
           ImageNDTemplate(std::move(aOther))
        {}

        void FlipX(){
            //! \TODO Implement
            throw std::runtime_error("Not implemented");
        }

        void FlipY(){
            //! \TODO Implement
            throw std::runtime_error("Not implemented");
        }

        void FlipZ(){
            //! \TODO Implement
            throw std::runtime_error("Not implemented");
        }

        void RotateClockwiseX(){
            //! \TODO Implement
            throw std::runtime_error("Not implemented");
        }

        void RotateCounterClockwiseX(){
            //! \TODO Implement
            throw std::runtime_error("Not implemented");
        }

        void RotateClockwiseY(){
            //! \TODO Implement
            throw std::runtime_error("Not implemented");
        }

        void RotateCounterClockwiseY(){
            //! \TODO Implement
            throw std::runtime_error("Not implemented");
        }

        void RotateClockwiseZ(){
            //! \TODO Implement
            throw std::runtime_error("Not implemented");
        }

        void RotateCounterClockwiseZ(){
            //! \TODO Implement
            throw std::runtime_error("Not implemented");
        }
    };

}}

#endif
