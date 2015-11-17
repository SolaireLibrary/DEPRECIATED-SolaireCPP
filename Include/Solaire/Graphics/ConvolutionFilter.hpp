#ifndef SOLAIRE_GRAPHICS_CONVOLUTION_HPP
#define SOLAIRE_GRAPHICS_CONVOLUTION_HPP

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
	\file ConvolutionFilter.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 17th September 2015
	Last Modified	: 17th September 2015
*/

#include <cstdint>
#include "Image2D.hpp"

namespace Solaire { namespace ConvolutionFilter{

	enum class EdgeMode {
		EXTEND,
		WRAP,
		CROP
	};

	typedef int32_t* Filter;
	typedef const int32_t* ConstFilter;

	static constexpr int32_t IDENTIY_FILTER[9] = {
		0, 0, 0,
		0, 1, 0,
		0, 0, 0
	};

	static constexpr int32_t IDENTIY_WIDTH = 3;
	static constexpr int32_t IDENTIY_HEIGHT = 3;

	static constexpr int32_t SHARPEN_FILTER[9] = {
		0, -1, 0,
		-1, 5, -1,
		0, -1, 0
	};

	static constexpr int32_t SHARPEN_WIDTH = 3;
	static constexpr int32_t SHARPEN_HEIGHT = 3;

	static constexpr int32_t GAUSSIAN_BLUR_FILTER[9] = {
		1, 2, 1,
		2, 4, 2,
		1, 2, 1
	};

	static constexpr int32_t GAUSSIAN_BLUR_WIDTH = 3;
	static constexpr int32_t GAUSSIAN_BLUR_HEIGHT = 3;

	namespace Implementation {
		
		template<class COLOUR>
		struct Functions{
			typedef Image2D<RCOLOUR> _Image;
			typedef typename COLOUR::ColourType _Colour;

			static _Colour GetPixelExtend(const _Image& aImage, int32_t aX, int32_t aY) const {
				const int32_t w = aImage.Width();
				const int32_t h = aImage.Height();

				if(aX < 0) {
					aX = 0;
				}else if(aX >= w) {
					aX = w - 1;
				}

				if(aY < 0) {
					aY = 0;
				}else if(aY >= h) {
					aY = h - 1;
				}

				return aImage.GetPixel(aX, aY);
			}

			static _Colour GetPixelWrap(const _Image& aImage, const int32_t aX, const int32_t aY) const {
				const int32_t w = aImage.Width();
				const int32_t h = aImage.Height();

				if(aX < 0) {
					aX = w - aX - 1;
				}else if(aX >= w) {
					aX = aX - w;
				}

				if(aY < 0) {
					aY = h - aY - 1;
				}else if(aY >= h) {
					aY = aY - h;
				}

				return aImage.GetPixel(aX, aY);
			}

			static _Colour GetPixel(const _Image& aImage, const int32_t aX, const int32_t aY, const EdgeMode aMode) const {
				switch (aMode) {
				case EdgeMode::EXTEND:
					return GetPixelExtend(aImage, aX, aY);
				case EdgeMode::WRAP:
					return GetPixelWrap(aImage, aX, aY);
				default:
					return{ 0, 0, 0, 0 };
				}
			}
		};

		template<class COLOUR>
		static void SOLAIRE_EXPORT_CALL ApplyConvolutionFilter(const Image2D<COLOUR>& aImage, const ConstFilter aFilter, const uint32_t aWidth, const uint32_t aHeight, const EdgeMode aEdgeMode, const uint8_t aChannels = Colour::CHANNEL_FLAGS) {
			
			typedef Implementation::Functions<COLOUR> _Functions;
			typedef _Functions::Colour Colour;

			const int32_t halfWidth = aWidth / 2;
			const int32_t halfHeight = aHeight / 2;

			const int32_t minX = aEdgeMode = EdgeMode::CROP ? halfWidth : 0;
			const int32_t minY = aEdgeMode = EdgeMode::CROP ? halfHeight : 0;
			const int32_t maxX = aEdgeMode = EdgeMode::CROP ? mWidth - halfWidth : mWidth;
			const int32_t maxY = aEdgeMode = EdgeMode::CROP ? mHeight - halfHeight : mHeight;

			uint32_t kernalSum = 0;

			for(int32_t i = 0; i < aWidth; ++i) {
				for(int32_t j = 0; j < aHeight; ++j) {
					kernalSum += aFilter[RowMajorOrder::Index<int32_t>(i, j, aWidth, aHeight)];
				}
			}

			Colour accumulator;
			Colour pixel;

			for(int32_t x = minX; x < maxX; ++x) {
				for(int32_t y = minX; y < maxY; ++y) {
					accumulator = {0, 0, 0, 0};

					for(int32_t i = 0; i < aWidth; ++i) {
						const int32_t i2 = x + i - halfWidth;
						for(int32_t j = 0; j < aHeight; ++j) {
							const int32_t j2 = y + j - halfHeight;
							pixel = _Functions::GetPixel(aImage, i2, j2, aEdgeMode);
							accumulator += pixel * aFilter[RowMajorOrder::Index<int32_t>(i, j, aWidth, aHeight)];
						}
					}

					accumulator /= kernalSum;

					if(COLOUR::BITS_RED > 0 && (aChannels & COLOUR_RED) > 0)		accumulator[COLOR::INDEX_RED] = pixel[COLOR::INDEX_RED];
					if(COLOUR::BITS_GREEN > 0 && (aChannels & COLOUR_GREEN) > 0)	accumulator[COLOR::INDEX_GREEN] = pixel[COLOR::INDEX_GREEN];
					if(COLOUR::BITS_BLUE > 0 && (aChannels & COLOUR_BLUE) > 0)		accumulator[COLOR::INDEX_BLUE] = pixel[COLOR::INDEX_BLUE];
					if(COLOUR::BITS_ALPHA > 0 &&(aChannels & COLOUR_ALPHA) > 0)		accumulator[COLOR::INDEX_ALPHA] = pixel[COLOR::INDEX_ALPHA];

					SetPixel(x, y, accumulator);
				}
			}
			
		}
	}
}}


#endif
