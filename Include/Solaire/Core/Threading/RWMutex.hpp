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
    public:
        class ReadLockWrapper{
        private:
            RWMutex& mParent;
        public:
            ReadLockWrapper(RWMutex& aParent) :
                mParent(aParent)
            {}

            void lock(){
                mParent.ReadLock();
            }

            void unlock(){
                mParent.ReadUnlock();
            }

            bool try_lock(){
                return mParent.TryReadLock();
            }
        };

        class WriteLockWrapper{
        private:
            RWMutex& mParent;
        public:
            WriteLockWrapper(RWMutex& aParent) :
                mParent(aParent)
            {}

            void lock(){
                mParent.WriteLock();
            }

            void unlock(){
                mParent.WriteUnlock();
            }

            bool try_lock(){
                return mParent.TryWriteLock();
            }
        };
    private:
        RWMutex(const RWMutex&) = delete;
        RWMutex(RWMutex&&) = delete;

        RWMutex operator=(const RWMutex&) = delete;
        RWMutex operator=(RWMutex&&) = delete;

        Mutex mWriteLock;
        RecursiveMutex mStateLock;
        union{
            ReadLockWrapper mReadWrapper;
            WriteLockWrapper mWriteWrapper;
        };
        int8_t mReadCount;
    public:
        RWMutex():
            mReadWrapper(*this),
            mReadCount(0)
        {}

        void ReadLock(){
            mStateLock.lock();              // Prevent the state from being modified
            if(mReadCount >= 0){            // If there are no locks or an existing read lock
                ++mReadCount;               //  Add an additional read lock
            }else{                          // If there is a write lock
                mStateLock.unlock();        //  Allow the state to be modified again
                mWriteLock.lock();          //  Wait for the write lock to release
                mStateLock.lock();          //  Prevent the state from being modified again
                mWriteLock.unlock();        //  Release the write lock
                ++mReadCount;               //  Create a new read lock
            }
            mStateLock.unlock();            // Allow the state to be modified again
        }

        bool TryReadLock(){
            mStateLock.lock();              // Prevent the state from being modified
            if(mReadCount >= 0){            // If there are no locks or an existing read lock
                ++mReadCount;               //  Add an additional read lock
                mStateLock.unlock();        //  Allow the state to be modified again
                return true;                //  The lock was successful
            }else{                          // If there is a write lock
                mStateLock.unlock();        //  Allow the state to be modified again
                return false;               //  The lock failed
            }
        }

        void ReadUnlock(){
            mStateLock.lock();              // Prevent the state from being modified
            if(mReadCount > 0){             // If there is an existing read lock
                --mReadCount;               //  Remove a shared lock
                if(mReadCount == 0){        //  If there are no more shared locks
                    mStateLock.unlock();    //      Allow the state to be modified again
                }else{                      //  If there is at least one more shared lock
                    //! \TODO Notify that read lock has been released
                    mStateLock.unlock();    //      Allow the state to be modified again
                }
            }else{                          // If there are no locks, or a write lock
                mStateLock.unlock();        //  Allow the state to be modified again
                throw std::runtime_error("RWMutex : Is not read locked");
            }
        }

        void WriteLock(){
            mStateLock.lock();              // Prevent the state from being modified
            if(mReadCount == 0){            // If there are no locks
                --mReadCount;               //  Create a write lock
                mWriteLock.lock();
                mStateLock.unlock();        //  Allow the state to be modified again
            }else if(mReadCount > 0){       // If there is a read lock
                mStateLock.unlock();        //  Allow the state to be modified again
                //! \TODO Wait for read lock to be released
                mStateLock.lock();          //  Prevent the state from being modified again
                --mReadCount;               //  Create a write lock
                mWriteLock.lock();
                mStateLock.unlock();        //  Allow the state to be modified again
            }else{                          // If there is a write lock
                mStateLock.unlock();        //  Allow the state to be modified again
                mWriteLock.lock();          //  Wait for the write lock to release
                mStateLock.lock();          //  Prevent the state from being modified again
                --mReadCount;               //  Create a write lock
                mStateLock.unlock();        //  Allow the state to be modified again
            }
        }

        bool TryWriteLock(){
            mStateLock.lock();              // Prevent the state from being modified
            if(mReadCount == 0){            // If there are no locks
                --mReadCount;               //  Create a write lock
                mWriteLock.lock();
                mStateLock.unlock();        //  Allow the state to be modified again
                return true;                //  The lock was successful
            }else{                          // If there is a write or read lock
                mStateLock.unlock();        //  Allow the state to be modified again
                return false;               //  The lock failed
            }
        }

        void WriteUnlock(){
            mStateLock.lock();              // Prevent the state from being modified
            if(mReadCount < 0){             // If there is a write lock
                ++mReadCount;               //  Remove the write lock
                mWriteLock.unlock();
                mStateLock.unlock();        //  Allow the state to be modified again
            }else{                          // If there is no lock, or a read lock
                mStateLock.unlock();        //  Allow the state to be modified again
                throw std::runtime_error("RWMutex : Is not write locked");
            }
        }

        ReadLockWrapper& GetReadLock(){
            return mReadWrapper;
        }

        WriteLockWrapper& GetWriteLock(){
            return mWriteWrapper;
        }

    };

}


#endif
