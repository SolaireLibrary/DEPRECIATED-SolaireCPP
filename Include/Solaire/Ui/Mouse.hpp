#ifndef SOLAIRE_UI_MOUSE_HPP
#define SOLAIRE_UI_MOUSE_HPP

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
	\file Mouse.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 26th September 2015
	Last Modified	: 26th September 2015
*/

#include "..\Core\Listener.hpp"
#include "..\Maths\Vector.hpp"

namespace Solaire{ namespace Ui{

    enum MouseButton : uint32_t{
        BUTTON_LEFT,
        BUTTON_RIGHT,
        BUTTON_MIDDLE,
        BUTTON_0,
        BUTTON_1,
        BUTTON_2,
        BUTTON_3,
        BUTTON_4,
        BUTTON_5,
        BUTTON_6,
        BUTTON_7,
        BUTTON_8,
    };

    enum{
        BUTTON_COUNT = BUTTON_8 + 1
    };

    typedef Maths::Vector<uint16_t, 2> MouseCoord;

    class MouseSource;

    class MouseListener : public Core::Listener<MouseSource, MouseListener>{
    protected:
        virtual void OnButtonPress(const MouseButton aButton, const uint64_t aTime) = 0;
        virtual void OnButtonRelease(const MouseButton aButton, const uint64_t aTime) = 0;
        virtual void OnWheelScroll(const float aMovement, const uint64_t aTime) = 0;
        virtual void OnMove(const MouseCoord aMovement, const uint64_t aTime) = 0;
    public:
        friend MouseSource;

        MouseListener(Core::Allocator<Core::SourceBase*>& aAllocator = Core::GetDefaultAllocator<Core::SourceBase*>()) :
            Listener(aAllocator)
        {}

        virtual ~MouseListener(){

        }
    };

    class MouseSource : public Core::Source<MouseSource, MouseListener>
    {
    protected:
        void PressButton(const MouseButton aButton, const uint64_t aTime){
            const ListenerIterator end = ListenerEnd();
            for(ListenerIterator i = ListenerBegin(); i != end; ++i){
                (**i).OnButtonPress(aButton, aTime);
            }
        }

        void ReleaseButton(const MouseButton aButton, const uint64_t aTime){
            const ListenerIterator end = ListenerEnd();
            for(ListenerIterator i = ListenerBegin(); i != end; ++i){
                (**i).OnButtonRelease(aButton, aTime);
            }
        }

        void ScrollWheel(const float aMovement, const uint64_t aTime){
            const ListenerIterator end = ListenerEnd();
            for(ListenerIterator i = ListenerBegin(); i != end; ++i){
                (**i).OnWheelScroll(aMovement, aTime);
            }
        }

        void Move(const MouseCoord aMovement, const uint64_t aTime){
            const ListenerIterator end = ListenerEnd();
            for(ListenerIterator i = ListenerBegin(); i != end; ++i){
                (**i).OnMove(aMovement, aTime);
            }
        }
    public:
        MouseSource(Core::Allocator<Core::ListenerBase*>& aAllocator = Core::GetDefaultAllocator<Core::ListenerBase*>()) :
            Source(aAllocator)
        {}

        virtual ~MouseSource(){

        }
    };

    class Mouse : public MouseSource, public MouseListener{
    private:
        struct ButtonInfo{
            uint64_t timePressed;
            uint64_t timeReleased;
            bool isPressed;

            ButtonInfo() :
                timePressed(0),
                timeReleased(0),
                isPressed(false)
            {}

            void Press(const uint64_t aTime){
                timePressed = aTime;
                isPressed = true;
            }

            void Release(const uint64_t aTime){
                timeReleased = aTime;
                isPressed = false;
            }
        };

        //Core::WrapperAllocator<void*> mAllocator;
        ButtonInfo mButtons[BUTTON_COUNT];
        MouseCoord mPosition;
        float mWheelPosition;

        ButtonInfo& GetButtonInfo(const MouseButton aButton){
            return mButtons[aButton];
        }

        const ButtonInfo& GetButtonInfo(const MouseButton aButton) const{
            return mButtons[aButton];
        }
    protected:
        // Inherited from MouseSource

        void OnListenerAdded(MouseListener& aListener) override{

        }

        void OnListenerRemoved(MouseListener& aListener) override{

        }

        // Inherited from MouseListener

        void OnSourceAdded(MouseSource& aSource) override{

        }

        void OnSourceRemoved(MouseSource& aSource) override{

        }

        void OnButtonPress(const MouseButton aButton, const uint64_t aTime) override{
            ButtonInfo& info = GetButtonInfo(aButton);
            if(! info.isPressed){
                info.Press(aTime);
                PressButton(aButton, aTime);
            }
        }

        void OnButtonRelease(const MouseButton aButton, const uint64_t aTime) override{
            ButtonInfo& info = GetButtonInfo(aButton);
            if(info.isPressed){
                info.Release(aTime);
                ReleaseButton(aButton, aTime);
            }
        }

        void OnWheelScroll(const float aMovement, const uint64_t aTime) override{
            ScrollWheel(aMovement, aTime);
            mWheelPosition += aMovement;
        }

        void OnMove(const MouseCoord aMovement, const uint64_t aTime) override{
            Move(aMovement, aTime);
            mPosition += aMovement;
        }

    public:
        Mouse()//Core::Allocator<void>& aAllocator = Core::GetDefaultAllocator<void>() :
            //KeySource(reinterpret_cast<Core::WrapperAllocator<ListenerBase*>&>(mAllocator)),
           // KeyAllocator(reinterpret_cast<Core::WrapperAllocator<SourceBase*>&>(mAllocator)),
            //mAllocator(aAllocator)
        {}

        bool WasButtonPressed(const MouseButton aButton, const uint64_t aTime) const{
            const ButtonInfo& info = GetButtonInfo(aButton);
            return info.timePressed >= aTime && aTime < info.timeReleased;
        }

        bool IsKeyPressed(const MouseButton aButton) const{
            return GetButtonInfo(aButton).isPressed;
        }

        uint64_t GetTimePressed(const MouseButton aButton) const{
            return GetButtonInfo(aButton).timePressed;
        }

        uint64_t GetTimeReleased(const MouseButton aButton) const{
            return GetButtonInfo(aButton).timeReleased;
        }

        float GetWheelPosition() const{
            return mWheelPosition;
        }

        MouseCoord GetPosition() const{
            return mPosition;
        }
    };

}}


#endif
