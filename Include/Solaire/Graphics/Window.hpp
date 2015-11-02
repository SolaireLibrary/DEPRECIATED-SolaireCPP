#ifndef SOLAIRE_GRAPHICS_WINDOW_HPP
#define SOLAIRE_GRAPHICS_WINDOW_HPP

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
\file Window.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 2nd November 2015
Last Modified	: 2nd November 2015
*/

#include "..\Core\Strings\String.hpp"
#include "..\Maths\Vector2.hpp"
#include "..\Core\Listener.hpp"
#include "Graphics.inl"

namespace Solaire{ namespace Graphics{

    typedef Vector2<int16_t> WindowCoord;

    class WindowListener;

    class Window : public Source<Window, WindowListener>{
    public:
        enum Mode{
            FULLSCREEN,
            BORDERLESS,
            WINDOW,
        };
    public:
        virtual ~Window(){}

        virtual WindowCoord GetSize() const = 0;
        virtual void SetSize(const WindowCoord aSize) = 0;

        virtual WindowCoord GetPosition() const = 0;
        virtual void SetPosition(const WindowCoord aPosition) = 0;

        virtual ConstStringFragment GetTitle() const = 0;
        virtual void SetTitle(const ConstStringFragment aTitle) = 0;

        virtual Mode GetMode() const = 0;
        virtual void SetMode(const Mode aMode) = 0;

        virtual bool HasFocus() const = 0;
        virtual bool IsVisible() const = 0;
    };

    class WindowListener : public Listener<Window, WindowListener>{
    public:
        virtual ~WindowListener(){}

        virtual void OnMove(Window& aWindow, const WindowCoord aOld, const WindowCoord aNew) = 0;
        virtual void OnResize(Window& aWindow, const WindowCoord aOld, const WindowCoord aNew) = 0;

        virtual void OnFocus(Window& aWindow) = 0;
        virtual void OnUnfocus(Window& aWindow) = 0;

        virtual void OnVisible(Window& aWindow) = 0;
        virtual void OnInvisible(Window& aWindow) = 0;
    };

    typedef Window::Mode WindowMode;
}}

#endif
