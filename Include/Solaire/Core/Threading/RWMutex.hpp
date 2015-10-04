#ifndef SOLAIRE_RW_MUTEX_HPP
#define SOLAIRE_RW_MUTEX_HPP

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
	\file RWMutex.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 2.0
	\date
	Created			: 4th October 2015
	Last Modified	: 4th October 2015
*/

#include "ThreadTypes.hpp"


namespace Solaire{

    class RWMutex{
    private:
        RWMutex(const RWMutex&) = delete;
        RWMutex(RWMutex&&) = delete;

        RWMutex operator=(const RWMutex&) = delete;
        RWMutex operator=(RWMutex&&) = delete;

        Mutex mWriteLock;
        RecursiveMutex mStateLock;
        int8_t mReadCount;
    public:
        void ReadLock(){
            mStateLock.lock();
            if(mReadCount == 0){
                ++mReadCount;
            }else if(mReadCount > 0){
                ++mReadCount;
            }else{
                mStateLock.unlock();
                mWriteLock.lock();
                mStateLock.lock();
                mWriteLock.unlock();
                ++mReadCount;
            }
            mStateLock.unlock();
        }

        bool TryReadLock(){
            mStateLock.lock();
            if(mReadCount >= 0){
                ++mReadCount;
                mStateLock.unlock();
                return true;
            }else{
                mStateLock.unlock();
                return false;
            }
        }

        void ReadUnlock(){
            mStateLock.lock();
            if(mReadCount > 0){
                --mReadCount;
                if(mReadCount == 0){
                    mStateLock.unlock();
                }else{
                    //! \TODO Notify that read lock has been released
                    mStateLock.unlock();
                }
            }else{
                mStateLock.unlock();
                throw std::runtime_error("RWMutex : Is not read locked");
            }
        }

        void WriteLock(){
            mStateLock.lock();
            if(mReadCount == 0){
                --mReadCount;
                mWriteLock.lock();
            }else if(mReadCount > 0){
                mStateLock.unlock();
                //! \TODO Wait for read lock to be released
                mStateLock.lock();
                --mReadCount;
                mWriteLock.lock();
            }else{
                mStateLock.unlock();
                mWriteLock.lock();
                mStateLock.lock();
                --mReadCount;
            }
            mStateLock.unlock();
        }

        bool TryWriteLock(){
            mStateLock.lock();
            if(mReadCount == 0){
                --mReadCount;
                mWriteLock.lock();
                mStateLock.unlock();
                return true;
            }else{
                mStateLock.unlock();
                return false;
            }
        }

        void WriteUnlock(){
            mStateLock.lock();
            if(mReadCount < 0){
                ++mReadCount;
                mWriteLock.unlock();
                mStateLock.unlock();
            }else{
                mStateLock.unlock();
                throw std::runtime_error("RWMutex : Is not write locked");
            }
        }

    };

}


#endif
