#ifndef SOLAIRE_SYNC_BUFFER_HPP
#define SOLAIRE_SYNC_BUFFER_HPP

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
	\file SyncronisedBuffer.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 13th December 2015
	Last Modified	: 13th December 2015
*/

#include <mutex>
#include <vector>
#include <condition_variable>
#include "..\Memory\Allocator.hpp"
#include "..\IO\Stream.hpp"

namespace Solaire {

	class SyncronisedIStream;
	class SyncronisedOStream;

	class SyncronisedBuffer {
	public:
		friend SyncronisedIStream;
		friend SyncronisedOStream;
	private:
		Allocator& mAllocator;
		std::vector<uint8_t> mData;
		mutable std::mutex mLock;
		mutable std::condition_variable mReadCondition;
		bool mEnd;
	public:
		SyncronisedBuffer(Allocator& aAllocator):
			mAllocator(aAllocator),
			mEnd(false)
		{}
	
		~SyncronisedBuffer() {

		}
	};
}


#endif
