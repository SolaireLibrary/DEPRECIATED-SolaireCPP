#ifndef SOLAIRE_UI_KEYBOARD_HPP
#define SOLAIRE_UI_KEYBOARD_HPP

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
	\file Keyboard.hpp
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
#include "..\Core\DataStructures\CyclicalDeque.hpp"

namespace Solaire{ namespace Ui{

    enum Keycode : uint32_t{
        KEY_0,  KEY_1,  KEY_2,  KEY_3,
        KEY_4,  KEY_5,  KEY_6,  KEY_7,
        KEY_8,  KEY_9,

        KEY_A,  KEY_B,  KEY_C,  KEY_D,
        KEY_E,  KEY_F,  KEY_G,  KEY_H,
        KEY_I,  KEY_J,  KEY_K,  KEY_L,
        KEY_M,  KEY_N,  KEY_O,  KEY_P,
        KEY_Q,  KEY_R,  KEY_S,  KEY_T,
        KEY_U,  KEY_V,  KEY_W,  KEY_X,
        KEY_Y,  KEY_Z,

        KEY_F1,     KEY_F2,     KEY_F3,     KEY_F4,
        KEY_F5,     KEY_F6,     KEY_F7,     KEY_F8,
        KEY_F9,     KEY_F10,    KEY_F11,    KEY_F12,

        KEY_ESCAPE,         KEY_MINUS,                  KEY_EQUALS,                 KEY_BACKSPACE,
        KEY_TAB,            KEY_LEFT_SQUARE_BRACKET,    KEY_RIGHT_SQUARE_BRACKET,   KEY_ENTER,
        KEY_LEFT_CONTROL,   KEY_RIGHT_CONTROL,          KEY_SEMI_COLON,             KEY_QUOTE,
        KEY_TILDE,          KEY_BACKLASH,               KEY_FORWARD_SLASH,          KEY_COMMA,
        KEY_FULL_STOP,      KEY_RIGHT_SHIFT,            KEY_LEFT_SHIFT,             KEY_SPACE,
        KEY_CAP_LOCK,       KEY_NUM_LOCK,               KEY_SCROLL_LOCK,            KEY_INSERT,
        KEY_DELETE,         KEY_HOME,                   KEY_END,                    KEY_PAGE_UP,
        KEY_PAGE_DOWN
    };

    static constexpr char INVALID_KEY_CONVERSION = '\0';

    static constexpr char LayoutUK(const Keycode aKey, const bool aShift){
        return
            aKey >= Keycode::KEY_0 && aKey <= KEY_9 ? aShift ?
                aKey == Keycode::KEY_0 ? ')' :
                aKey == Keycode::KEY_1 ? '!' :
                aKey == Keycode::KEY_2 ? '"' :
                aKey == Keycode::KEY_3 ? '£' :
                aKey == Keycode::KEY_4 ? '$' :
                aKey == Keycode::KEY_5 ? '%' :
                aKey == Keycode::KEY_6 ? '^' :
                aKey == Keycode::KEY_7 ? '&' :
                aKey == Keycode::KEY_8 ? '*' :
                '(' :
                '0' + (aKey - Keycode::KEY_0) :
            aKey >= Keycode::KEY_A && aKey <= KEY_Z ? aShift ?
                'a' + (aKey - Keycode::KEY_A) :
                'A' + (aKey - Keycode::KEY_A) :
            aKey == KEY_MINUS ? aShift ? '_' : '-' :
            aKey == KEY_EQUALS ? aShift ? '+' : '=' :
            aKey == KEY_EQUALS ? '\b' :
            aKey == KEY_TAB ? '\t' :
            aKey == KEY_RIGHT_SQUARE_BRACKET ? aShift ? '{' : '[' :
            aKey == KEY_LEFT_SQUARE_BRACKET ? aShift ? '}' : ']' :
            aKey == KEY_ENTER ? '\n' :
            aKey == KEY_SEMI_COLON ? aShift ? ':' : ';' :
            aKey == KEY_QUOTE ? aShift ? '\'' : '@' :
            aKey == KEY_TILDE ? aShift ? '¬' : '`' :
            aKey == KEY_BACKLASH ? aShift ? '|' : '\\' :
            aKey == KEY_FORWARD_SLASH ? aShift ? '?' : '/' :
            aKey == KEY_COMMA ? aShift ? '<' : ',' :
            aKey == KEY_FULL_STOP ? aShift ? '>' : '.' :
            aKey == KEY_SPACE ? ' ' :
            INVALID_KEY_CONVERSION;
    }

    enum{
        KEY_COUNT = KEY_PAGE_DOWN + 1
    };

    class KeySource;

    class KeyListener : public Core::Listener<KeySource, KeyListener>{
    protected:
        virtual void OnKeyPress(const Keycode aKey, const uint64_t aTime) = 0;
        virtual void OnKeyRelease(const Keycode aKey, const uint64_t aTime) = 0;
    public:
        friend KeySource;

        KeyListener(Core::Allocator<Core::SourceBase*>& aAllocator = Core::GetDefaultAllocator<Core::SourceBase*>()) :
            Listener(aAllocator)
        {}

        virtual ~KeyListener(){

        }
    };

    class KeySource : public Core::Source<KeySource, KeyListener>
    {
    protected:
        void PressKey(const Keycode aKey, const uint64_t aTime){
            const ListenerIterator end = ListenerEnd();
            for(ListenerIterator i = ListenerBegin(); i != end; ++i){
                (**i).OnKeyPress(aKey, aTime);
            }
        }

        void ReleaseKey(const Keycode aKey, const uint64_t aTime){
            const ListenerIterator end = ListenerEnd();
            for(ListenerIterator i = ListenerBegin(); i != end; ++i){
                (**i).OnKeyRelease(aKey, aTime);
            }
        }
    public:
        KeySource(Core::Allocator<Core::ListenerBase*>& aAllocator = Core::GetDefaultAllocator<Core::ListenerBase*>()) :
            Source(aAllocator)
        {}

        virtual ~KeySource(){

        }
    };

    class Keyboard : public KeySource, public KeyListener{
    private:
        struct KeyInfo{
            typedef std::pair<uint64_t, uint64_t> TimePair;

            Core::CyclicalDeque<TimePair> history;
            bool isPressed;

            KeyInfo() :
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
        KeyInfo mKeys[KEY_COUNT];

        KeyInfo& GetKeyInfo(const Keycode aKey){
            return mKeys[aKey];
        }

        const KeyInfo& GetKeyInfo(const Keycode aKey) const{
            return mKeys[aKey];
        }
    protected:
        // Inherited from KeySource

        void OnListenerAdded(KeyListener& aListener) override{

        }

        void OnListenerRemoved(KeyListener& aListener) override{

        }

        // Inherited from KeyListener

        void OnSourceAdded(KeySource& aSource) override{

        }

        void OnSourceRemoved(KeySource& aSource) override{

        }

        void OnKeyPress(const Keycode aKey, const uint64_t aTime) override{
            KeyInfo& info = GetKeyInfo(aKey);
            if(! info.isPressed){
                info.Press(aTime);
                PressKey(aKey, aTime);
            }
        }

        void OnKeyRelease(const Keycode aKey, const uint64_t aTime){
            KeyInfo& info = GetKeyInfo(aKey);
            if(info.isPressed){
                info.Release(aTime);
                ReleaseKey(aKey, aTime);
            }
        }

    public:
        Keyboard()//Core::Allocator<void>& aAllocator = Core::GetDefaultAllocator<void>() :
            //KeySource(reinterpret_cast<Core::WrapperAllocator<ListenerBase*>&>(mAllocator)),
           // KeyAllocator(reinterpret_cast<Core::WrapperAllocator<SourceBase*>&>(mAllocator)),
            //mAllocator(aAllocator)
        {}

        bool WasKeyPressed(const Keycode aKey, const uint64_t aTime) const{
            return GetKeyInfo(aKey).WasPressed(aTime);
        }

        bool IsKeyPressed(const Keycode aKey) const{
            return GetKeyInfo(aKey).isPressed;
        }

        uint64_t GetTimePressed(const Keycode aKey) const{
            GetKeyInfo(aKey).history.Back().first;
        }

        uint64_t GetTimeReleased(const Keycode aKey) const{
            GetKeyInfo(aKey).history.Back().second;
        }

    };

}}


#endif
