#ifndef SOLAIRE_LOGIC_STATE_MACHINE_HPP
#define SOLAIRE_LOGIC_STATE_MACHINE_HPP

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
\file StateMachine.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 25th September 2015
Last Modified	: 25th September 2015
*/

#include <vector>
#include <type_traits>

namespace Solaire{ namespace Logic{

    template<class T, class IMPLEMENTER>
    class StateMachine{
    public:
        typedef uint8_t StateID;
        typedef void(IMPLEMENTER:: *StateFunction)();
    private:
        std::vector<StateFunction> mStates;
        T mResult;
        StateID mState;
        StateID mInitialState;
        bool mAccepted;
    protected:
        StateID AddState(const StateFunction aFn){
            const StateID id = mStates.size();
            mStates.push_back(aFn);
            return id;
        }

        void SetInitialState(const StateID aState){
            mInitialState = aState;
        }

        void Accept(const T aResult){
            mAccepted = true;
            mResult = aResult;
        }

        void StateTransition(const StateID aState){
            mState = aState;
        }

        virtual void Reset() = 0;
    public:
        virtual ~StateMachine(){
            static_assert(std::is_base_of<StateMachine, IMPLEMENTER>::value, "Logic::StateMachine IMPLEMENTER template must inherit from StateMachine");
        }

        T operator()(){
            mAccepted = false;
            mState = mInitialState;
            Reset();

            while(! mAccepted){
                (reinterpret_cast<IMPLEMENTER*>(this)->*mStates[mState])();
            }

            return mResult;
        }
    };

}}

#endif
