#ifndef SOLAIRE_CORE_TIME_WINDOWS_INL
#define SOLAIRE_CORE_TIME_WINDOWS_INL

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
	\file TimeWindows.inl
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 20th September 2015
	Last Modified	: 20sth September 2015
*/

#include <cstdint>
#include <vector>
#include <stdexcept>
#include "SolaireWindows.inl"

namespace Solaire{ namespace Utility{

    class Timer{
    private:
        typedef std::pair<uint64_t, uint64_t> Frame;
        std::vector<Frame> mFrames;

        enum : uint64_t{
            TIME_UNDEFINED = 0
        };
    public:
        static uint64_t GetCurrentTime(){
            static double MULTIPLIER = 0;
            static bool ONCE = true;

            if(ONCE){
                ONCE = false;

                LARGE_INTEGER frequency;
                if(! QueryPerformanceFrequency(&frequency)) throw std::runtime_error("QueryPerformanceFrequency  failed");
                MULTIPLIER = 1000.0 / static_cast<double>(frequency.QuadPart);
            }

            LARGE_INTEGER tmp;
            if(! QueryPerformanceCounter(&tmp)) throw std::runtime_error("QueryPerformanceCounter failed");
            return static_cast<uint64_t>(static_cast<double>(tmp.QuadPart) * MULTIPLIER);
        }

        bool IsStarted() const{
            return ! mFrames.empty();
        }

        bool IsPaused() const{
            if(! IsStarted()){
                return false;
            }else{
                return mFrames.back().first == TIME_UNDEFINED;
            }
        }

        bool IsRunning() const{
            return IsStarted() && ! IsPaused();
        }

        void Start(){
            if(IsStarted()) throw std::runtime_error("Timer has already been started");

            mFrames.push_back(Frame(GetCurrentTime(), TIME_UNDEFINED));
        }

        void Pause(){
            if(IsPaused()) throw std::runtime_error("Timer has already been paused");

            Frame& frame = mFrames.back();
            frame.second = GetCurrentTime();
            mFrames.push_back(Frame(TIME_UNDEFINED, TIME_UNDEFINED));
        }

        void Resume(){
            if(! IsStarted()) throw std::runtime_error("Timer has not been started");
            if(! IsPaused()) throw std::runtime_error("Timer has not been paused");

            Frame& frame = mFrames.back();
            frame.first = GetCurrentTime();
        }

        uint64_t Stop(){
            if(! IsStarted()) throw std::runtime_error("Timer has not been started");

            if(IsPaused()) mFrames.pop_back();
            Frame& frame = mFrames.back();
            frame.second = GetCurrentTime();

            uint64_t sum = 0;
            for(const Frame& i : mFrames){
                sum += i.second - i.first;
            }

            mFrames.clear();
            return sum;
        }

        uint64_t GetRunningTime() const{
            if(IsPaused()) throw std::runtime_error("Timer has been paused");

            uint64_t sum = 0;
            for(const Frame& i : mFrames){
                sum += (i.second == TIME_UNDEFINED ? GetCurrentTime() : i.second) - i.first;
            }
            return sum;
        }
    };

}}

#endif
