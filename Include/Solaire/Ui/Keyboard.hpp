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
	\brief Contains code for N dimentional vector maths.
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 26th September 2015
	Last Modified	: 26th September 2015
*/

#include "..\Core\Listener.hpp"

namespace Solaire{ namespace Ui{

    enum Keycode : uint32_t{

    };

    enum{
        KEY_COUNT = 128 //! \TODO Set value
    };

    class KeySource;

    class KeyListener : public Core::Listener<KeySource, KeyListener>{
    protected:
        virtual void OnKeyPressed(const Keycode aKey, const uint64_t aTime) = 0;
        virtual void OnKeyReleased(const Keycode aKey, const uint64_t aTime) = 0;
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
                (**i).OnKeyPressed(aKey, aTime);
            }
        }

        void ReleaseKey(const Keycode aKey, const uint64_t aTime){
            const ListenerIterator end = ListenerEnd();
            for(ListenerIterator i = ListenerBegin(); i != end; ++i){
                (**i).OnKeyReleased(aKey, aTime);
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
            uint64_t timePressed;
            uint64_t timeReleased;
            bool isPressed;

            KeyInfo() :
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

        void OnKeyPressed(const Keycode aKey, const uint64_t aTime) override{
            PressKey(aKey, aTime);
            GetKeyInfo(aKey).Press(aTime);
        }

        void OnKeyReleased(const Keycode aKey, const uint64_t aTime){
            ReleaseKey(aKey, aTime);
            GetKeyInfo(aKey).Release(aTime);
        }

    public:
        Keyboard()//Core::Allocator<void>& aAllocator = Core::GetDefaultAllocator<void>() :
            //KeySource(reinterpret_cast<Core::WrapperAllocator<ListenerBase*>&>(mAllocator)),
           // KeyAllocator(reinterpret_cast<Core::WrapperAllocator<SourceBase*>&>(mAllocator)),
            //mAllocator(aAllocator)
        {}

        bool WasKeyPressed(const Keycode aKey, const uint64_t aTime) const{
            const KeyInfo& info = GetKeyInfo(aKey);
            return info.timePressed >= aTime && aTime < info.timeReleased;
        }

        bool IsKeyPressed(const Keycode aKey) const{
            return GetKeyInfo(aKey).isPressed;
        }

        uint64_t GetTimePressed(const Keycode aKey) const{
            return GetKeyInfo(aKey).timePressed;
        }

        uint64_t GetTimeReleased(const Keycode aKey) const{
            return GetKeyInfo(aKey).timeReleased;
        }

    };

}}


#endif
