#ifndef SOLAIRE_CORE_TIME_HPP
#define SOLAIRE_CORE_TIME_HPP

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
	\file Time.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 20th September 2015
	Last Modified	: 23rd September 2015
*/

#include <stdexcept>
#include <chrono>

namespace Solaire{ namespace Core{

    class Timer{
    private:
        typedef std::pair<uint64_t, uint64_t> Frame;
        std::vector<Frame> mFrames;

        enum : uint64_t{
            TIME_UNDEFINED = 0
        };
    public:
        static uint64_t GetSystemTimeMilli(){
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }

        static uint64_t GetSystemTimeMicro(){
            return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
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

            mFrames.push_back(Frame(GetSystemTimeMicro(), TIME_UNDEFINED));
        }

        void Pause(){
            if(IsPaused()) throw std::runtime_error("Timer has already been paused");

            Frame& frame = mFrames.back();
            frame.second = GetSystemTimeMicro();
            mFrames.push_back(Frame(TIME_UNDEFINED, TIME_UNDEFINED));
        }

        void Resume(){
            if(! IsStarted()) throw std::runtime_error("Timer has not been started");
            if(! IsPaused()) throw std::runtime_error("Timer has not been paused");

            Frame& frame = mFrames.back();
            frame.first = GetSystemTimeMicro();
        }

        uint64_t Stop(){
            if(! IsStarted()) throw std::runtime_error("Timer has not been started");

            if(IsPaused()) mFrames.pop_back();
            Frame& frame = mFrames.back();
            frame.second = GetSystemTimeMicro();

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
                sum += (i.second == TIME_UNDEFINED ? GetSystemTimeMicro() : i.second) - i.first;
            }
            return sum;
        }
    };

}}

#endif