#ifndef SOLAIRE_GRAPICS_INL
#define SOLAIRE_GRAPICS_INL

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
\file Graphics.inl
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 20th September 2015
Last Modified	: 20th September 2015
*/

#include "..\Maths\Vector3.hpp"
#include "..\Maths\Vector4.hpp"

#ifndef SOLAIRE_OPENGL_DETECTED
    #include "OpenGLPlaceholder.inl"
#endif

namespace Solaire{ namespace Graphics{
    typedef Maths::Vector3<uint8_t> ColourRGB;
    typedef Maths::Vector4<uint8_t> ColourRGBA;
    typedef Maths::Vector4<GLfloat> HardwareColour;

    static ColourRGB DEFAULT_RGB(0, 0, 0);
    static ColourRGBA DEFAULT_RGBA(0, 0, 0, 255);

    template<class FORMAT>
    static FORMAT DefaultColour() = delete;

    template<>
    ColourRGB DefaultColour<ColourRGB>(){
        return DEFAULT_RGB;
    }

    template<>
    ColourRGBA DefaultColour<ColourRGBA>(){
        return DEFAULT_RGBA;
    }

    template<class FORMAT>
    static HardwareColour ConvertToHardwareColour(const FORMAT aColour) = delete;

    template<>
    HardwareColour ConvertToHardwareColour<ColourRGBA>(const ColourRGBA aColour){
        return HardwareColour(aColour.X, aColour.Y, aColour.Z, aColour.W) / 255.f;
    }

    template<>
    HardwareColour ConvertToHardwareColour<ColourRGB>(const ColourRGB aColour){
        return ConvertToHardwareColour<ColourRGBA>(ColourRGBA(aColour.X, aColour.Y, aColour.Z, 255));
    }
}}

#endif
