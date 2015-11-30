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

#include "Solaire\Encode\Json.hpp"
#include "Solaire\Strings\NumberParser.hpp"
#include "Solaire\Encode\Array.hpp"
#include "Solaire\Encode\Object.hpp"
#include "Solaire\Strings\ConstStringTerminated.hpp"

namespace Solaire{

	// State

	Json::Writer::State::State(Allocator& aAllocator) :
		names(aAllocator),
		values(aAllocator)
	{}

	// Writer

	Json::Writer::Writer(WriteStream& aStream) :
		mOutputStream(aStream),
		mState(GetDefaultAllocator())
	{}

	Json::Writer::Writer(Allocator& aAllocator, WriteStream& aStream) :
		mOutputStream(aStream),
		mState(aAllocator)
	{}

	Json::Writer::~Writer() {
		mOutputStream.Flush();
	}

	bool Json::Writer::IsArray() const throw() {
		return mState.IsEmpty() ? true : mState.Back().type == STATE_ARRAY;
	}

	bool Json::Writer::IsObject() const throw() {
		return mState.IsEmpty() ? false : mState.Back().type == STATE_OBJECT;
	}

	bool Json::Writer::AddName(const ConstString<char>& aName) {
		if(mState.IsEmpty()) return false;
		State& state = mState.Back();
		if(state.type != STATE_OBJECT) return false;
		state.names.PushBack(CString(mState.GetAllocator(), aName));
		return true;
	}

	bool Json::Writer::AddValueInternal(const ConstString<char>& aValue) throw() {
		if(mState.IsEmpty()) return false;
		State& state = mState.Back();
		state.values.PushBack(CString(mState.GetAllocator(), aValue));
		return true;
	}

	bool Json::Writer::BeginArray() throw() {
		State& state = mState.PushBack(State(mState.GetAllocator()));
		state.type = STATE_ARRAY;
		return true;
	}

	bool Json::Writer::EndArray() throw() {
		if(mState.IsEmpty()) return false; 
		State& state = mState.Back();
		if(state.type != STATE_ARRAY) return false;

		CString buffer(mState.GetAllocator());
		const uint32_t size = state.values.Size();

		buffer += '[';
		for(uint32_t i = 0; i < size; ++i) {
			buffer += state.values[i];
			if(i + 1 < size) buffer += ',';
		}
		buffer += ']';

		mState.PopBack();

		if(mState.IsEmpty()) {
			mOutputStream.Write(buffer.GetContiguousPtr(), buffer.Size());
		}else{
			mState.Back().values.PushBack(buffer);
		}

		return true;
	}

	bool Json::Writer::BeginObject() throw() {
		State& state = mState.PushBack(State(mState.GetAllocator()));
		state.type = STATE_OBJECT;
		return true;
	}

	bool Json::Writer::EndObject() throw() {
		if(mState.IsEmpty()) return false; 
		State& state = mState.Back();
		if(state.type != STATE_OBJECT) return false;

		CString buffer(mState.GetAllocator());
		const uint32_t size = state.values.Size();
		if(state.names.Size() != size) return false;

		buffer += '{';
		for(uint32_t i = 0; i < size; ++i) {
			buffer += '"';
			buffer += state.names[i];
			buffer += '"';
			buffer += ':';
			buffer += state.values[i];
			if(i + 1 < size) buffer += ',';
		}
		buffer += '}';

		mState.PopBack();

		if(mState.IsEmpty()) {
			mOutputStream.Write(buffer.GetContiguousPtr(), buffer.Size());
		}else{
			mState.Back().values.PushBack(buffer);
		}

		return true;
	}

	bool Json::Writer::AddNull() throw() {
		return AddValueInternal(ConstCString("null"));
	}

	bool Json::Writer::AddBool(const bool aValue) throw() {
		return AddValueInternal(ConstCString(aValue ? "true" : "false"));
	}

	bool Json::Writer::AddNumber(const double aValue) throw() {
		return AddValueInternal(WriteNumber(mState.GetAllocator(), aValue));
	}

	bool Json::Writer::AddString(const ConstString<char>& aValue) throw() {
		CString buffer(mState.GetAllocator());
		//! \todo Escape quotes in aValue
		buffer += '"';
		buffer += aValue;
		buffer += '"';
		return AddValueInternal(buffer);
	}

	bool Json::Writer::BeginArray(const ConstString<char>& aName) throw() {
		if(! AddName(aName)) return false;
		return BeginArray();
	}

	bool Json::Writer::BeginObject(const ConstString<char>& aName) throw() {
		if(! AddName(aName)) return false;
		return BeginObject();
	}

	bool Json::Writer::AddNull(const ConstString<char>& aName) throw() {
		if(! AddName(aName)) return false;
		return AddNull();
	}

	bool Json::Writer::AddBool(const ConstString<char>& aName, const bool aValue) throw() {
		if(! AddName(aName)) return false;
		return AddBool(aValue);
	}

	bool Json::Writer::AddNumber(const ConstString<char>& aName, const double aValue) throw() {
		if(! AddName(aName)) return false;
		return AddNumber(aValue);
	}

	bool Json::Writer::AddString(const ConstString<char>& aName, const ConstString<char>& aValue) throw() {
		if(! AddName(aName)) return false;
		return AddString(aValue);
	}

	bool Json::Writer::AddValue(const ConstString<char>& aName, const Encode::Value& aValue) throw() {
		if(! AddName(aName)) return false;
		return AddValue(aValue);
	}

	bool Json::Writer::AddValue(const Encode::Value& aValue) throw() {
		switch (aValue.GetType()) {
		case Encode::Value::TYPE_BOOL:
			return AddBool(aValue.GetBool());
		case Encode::Value::TYPE_CHAR:
			{
				char buf[2] = {
					aValue.GetChar(),
					'\0'
				};
				return AddString(CString(mState.GetAllocator(), buf));
			}
		case Encode::Value::TYPE_INT:
		case Encode::Value::TYPE_UINT:
		case Encode::Value::TYPE_DOUBLE:
			return AddNumber(aValue.GetDouble());
		case Encode::Value::TYPE_STRING:
			return AddString(aValue.GetString());
		case Encode::Value::TYPE_ARRAY:
			{
				const Encode::Array& _array = aValue.GetArray();
				const uint32_t length = _array.Size();

				if(! BeginArray()) return false;
				for(uint32_t i = 0; i < length; ++i) {
					if(! AddValue(_array[i])) return false;
				}
				if(! EndArray()) return false;

				return true;
			}
		case Encode::Value::TYPE_OBJECT:
			{
				const Encode::Object& object = aValue.GetObject();
				const uint32_t length = object.Size();
				
				if(! BeginObject()) return false;
				for(uint32_t i = 0; i < length; ++i) {
					if(! AddValue(object.GetMemberName(i), object[i])) return false;
				}
				if(! EndObject()) return false;

				return true;
			}
		default:
			return false;
		}
	}

}
