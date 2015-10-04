#ifndef SOLAIRE_STREAM_ITERATOR_HPP
#define SOLAIRE_STREAM_ITERATOR_HPP

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
	\file StreamIterator.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 2.0
	\date
	Created			: 4th October 2015
	Last Modified	: 4th October 2015
*/

#include <istream>

namespace Solaire{

	class IStreamIterator{
	private:
	    enum{
	        END_OF_STREAM = UINT16_MAX,
	    };

		std::istream* mStream;
		uint16_t mPos;
	public:

		IStreamIterator() :
			mStream(nullptr),
			mPos(END_OF_STREAM)
		{}

		IStreamIterator(std::istream& aStream) :
			mStream(&aStream),
			mPos(aStream.tellg())
		{}

		inline size_t operator-(const IStreamIterator aOther) const{
		    if(mStream != aOther.mStream) throw std::runtime_error("IStreamIterator : Cannot operate on iterators of different streams");
			return mPos - aOther.mPos;
		}

		IStreamIterator& operator++(){
		    if(mPos == END_OF_STREAM) throw std::runtime_error("IStreamIterator : End of stream reached");
		    uint16_t pos = mStream->tellg();
		    ++mPos;
		    mStream->seekg(mPos, std::ios::beg);
		    if(mStream->rdstate() & std::ifstream::failbit){
                mPos = END_OF_STREAM;
		    }
		    mStream->seekg(pos, std::ios::beg);
			return *this;
		}

		inline IStreamIterator operator++(int){
		    IStreamIterator tmp(*this);
		    operator++();
			return tmp;
		}

		inline IStreamIterator& operator--(){
		    if(mPos == 0) throw std::runtime_error("IStreamIterator : Start of stream reached");
			--mPos;
			return *this;
		}

		inline IStreamIterator operator--(int){
		    IStreamIterator tmp(*this);
		    operator--();
			return tmp;
		}

		IStreamIterator& operator+=(size_t aNumber){
			while(aNumber > 0){
                operator++();
                --aNumber;
			}
			return *this;
		}

		IStreamIterator operator+(const size_t aNumber) const{
		    IStreamIterator tmp(*this);
		    tmp.operator+=(aNumber);
			return tmp;
		}

		IStreamIterator& operator-=(size_t aNumber){
			while(aNumber > 0){
                operator--();
                --aNumber;
			}
			return *this;
		}

		inline IStreamIterator operator-(const size_t aNumber) const{
		    IStreamIterator tmp(*this);
		    tmp.operator-=(aNumber);
			return tmp;
		}

		char operator*() const{
		    if(mPos == END_OF_STREAM) throw std::runtime_error("IstreamIterator : Cannot read past end of stream");
            mStream->seekg(mPos, std::ios::beg);
            return mStream->peek();
		}

		//inline Pointer operator->(){
		//
		//}

		inline bool operator==(const IStreamIterator aOther) const{
			return mPos == aOther.mPos;
		}

		inline bool operator!=(const IStreamIterator aOther) const{
			return mPos != aOther.mPos;
		}

		inline bool operator<(const IStreamIterator aOther) const{
			return mPos < aOther.mPos;
		}

		inline bool operator>(const IStreamIterator aOther) const{
			return mPos > aOther.mPos;
		}

		inline bool operator<=(const IStreamIterator aOther) const{
			return mPos <= aOther.mPos;
		}

		inline bool operator>=(const IStreamIterator aOther) const{
			return mPos >= aOther.mPos;
		}
	};
}


#endif
