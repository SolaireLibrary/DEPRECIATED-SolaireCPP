#ifndef SOLAIRE_THREAD_TYPES_HPP
#define SOLAIRE_THREAD_TYPES_HPP

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
	\file ThreadTypes.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 2.0
	\date
	Created			: 4th October 2015
	Last Modified	: 4th October 2015
*/

#ifndef SOLAIRE_DISABLE_MULTITHREADING
    #include <mutex>
    #include <thread>
#endif


namespace Solaire{
    #ifdef SOLAIRE_DISABLE_MULTITHREADING
        typedef uint32_t ThreadID;

        class Mutex{
        private:
            bool mLocked;
        public:
            Mutex() :
                mLocked(false)
            {}

            void lock(){
                mLocked = true;
            }

            void unlock(){
                mLocked = false;
            }

            bool try_lock(){
                if(mLocked){
                    return false;
                }else{
                    mLocked = true;
                    return false;
                }
            }
        };
    #else
        typedef std::thread::id ThreadID;
        typedef std::mutex Mutex;
    #endif
}


#endif
