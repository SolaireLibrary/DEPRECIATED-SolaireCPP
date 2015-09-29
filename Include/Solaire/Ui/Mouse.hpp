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

#include "Input.inl"
#include "..\Core\Listener.hpp"
#include "..\Maths\Vector.hpp"
#include "..\Core\DataStructures\CyclicalDeque.hpp"

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

        MouseListener(Core::Allocator<void>& aAllocator = Core::GetDefaultAllocator<void>()) :
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
        MouseSource(Core::Allocator<void>& aAllocator = Core::GetDefaultAllocator<void>()) :
            Source(aAllocator)
        {}

        virtual ~MouseSource(){

        }
    };

    class Mouse : public MouseSource, public MouseListener{
    private:
        struct ButtonInfo{
            typedef std::pair<uint64_t, uint64_t> TimePair;
            Core::CyclicalDeque<TimePair> history;
            bool isPressed;

            ButtonInfo() :
                history(SOLAIRE_INPUT_HISTORY),
                isPressed(false)
            {}

            void Press(const uint64_t aTime){
                history.PushBack(TimePair(aTime, UINT64_MAX));
                isPressed = true;
            }

            void Release(const uint64_t aTime){
                history.Back().second = aTime;
                isPressed = false;
            }

            bool WasPressed(const uint64_t aTime) const{
                for(const TimePair& i : history){
                    if(aTime >= i.first && aTime < i.second) return true;
                }
                return false;
            }
        };

        //Core::WrapperAllocator<void*> mAllocator;
        ButtonInfo mButtons[BUTTON_COUNT];
        typedef std::pair<uint64_t, float> WheelHistory;
        Core::CyclicalDeque<WheelHistory> mWheelHistory;
        typedef std::pair<uint64_t, MouseCoord> PositionHistory;
        Core::CyclicalDeque<PositionHistory> mPositionHistory;

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
            mWheelHistory.PushBack(WheelHistory(aTime, mWheelHistory.Back().second));
        }

        void OnMove(const MouseCoord aMovement, const uint64_t aTime) override{
            Move(aMovement, aTime);
            mPositionHistory.PushBack(PositionHistory(aTime, mPositionHistory.Back().second));
        }

    public:
        Mouse()://Core::Allocator<void>& aAllocator = Core::GetDefaultAllocator<void>() :
            //KeySource(reinterpret_cast<Core::WrapperAllocator<ListenerBase*>&>(mAllocator)),
           // KeyAllocator(reinterpret_cast<Core::WrapperAllocator<SourceBase*>&>(mAllocator)),
            //mAllocator(aAllocator)
            mWheelHistory(SOLAIRE_INPUT_HISTORY),
            mPositionHistory(SOLAIRE_INPUT_HISTORY)
        {}

        bool WasButtonPressed(const MouseButton aButton, const uint64_t aTime) const{
            return GetButtonInfo(aButton).WasPressed(aTime);
        }

        bool IsKeyPressed(const MouseButton aButton) const{
            return GetButtonInfo(aButton).isPressed;
        }

        uint64_t GetTimePressed(const MouseButton aButton) const{
            return GetButtonInfo(aButton).history.Back().first;
        }

        uint64_t GetTimeReleased(const MouseButton aButton) const{
            return GetButtonInfo(aButton).history.Back().second;
        }

        float GetWheelPosition() const{
            return mWheelHistory.Back().second;
        }

        MouseCoord GetPosition() const{
            return mPositionHistory.Back().second;
        }

        float GetWheelPosition(const uint64_t aTime) const{
            const auto end = mWheelHistory.end();
            auto i = mWheelHistory.begin();
            auto j = i + 1;

            if(i == end) return 0.f;
            if(aTime < i->first || j == end) return i->second;

            while(j != end){
                if(aTime >= i->first && aTime < j->first){
                    const double t0 = i->first;
                    const double t1 = j->first;
                    const double p0 = i->second;
                    const double p1 = j->second;
                    const double w = t0 / t1;
                    return static_cast<float>(Core::Maths::Lerp(p0, p1, w));
                }
                ++i;
                ++j;
            }

            return i->second;
        }

        MouseCoord GetPosition(const uint64_t aTime) const{
            const auto end = mPositionHistory.end();
            auto i = mPositionHistory.begin();
            auto j = i + 1;

            if(i == end) return 0.f;
            if(aTime < i->first || j == end) return i->second;

            while(j != end){
                if(aTime >= i->first && aTime < j->first){
                    const double t0 = i->first;
                    const double t1 = j->first;
                    const double w = t0 / t1;
                    return i->second.Lerp(j->second, w);
                }
                ++i;
                ++j;
            }

            return i->second;
        }
    };

}}


#endif
