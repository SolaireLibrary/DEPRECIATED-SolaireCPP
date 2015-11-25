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
#include "Solaire\Memory\DefaultAllocator.hpp"
#include "Solaire\Encode\Array.hpp"
#include "Solaire\Encode\Object.hpp"

namespace Solaire{ namespace Encode{

	// State

	Json::Writer::State::State(Allocator& aAllocator) :
		names(aAllocator),
		values(aAllocator)
	{}

	// Writer

	Json::Writer::Writer(WriteStream& aStream) :
		mOutputStream(aStream),
		mState(DEFAULT_ALLOCATOR)
	{}

	Json::Writer::Writer(Allocator& aAllocator, WriteStream& aStream) :
		mOutputStream(aStream),
		mState(aAllocator)
	{}

	Json::Writer::~Writer() {
		mOutputStream.Flush();
	}

	bool Json::Writer::AddValueInternal(const ConstStringFragment aValue) throw() {
		if(mState.IsEmpty()) return false;
		State& state = mState.Back();
		if(state.type != STATE_ARRAY) return false;
		state.values.PushBack(String(mState.GetAllocator(), aValue));
		return true;
	}

	bool Json::Writer::AddValueInternal(const ConstStringFragment aName, const ConstStringFragment aValue) throw() {
		if(mState.IsEmpty()) return false;
		State& state = mState.Back();
		if(state.type != STATE_OBJECT) return false;
		state.names.PushBack(String(mState.GetAllocator(), aName));
		state.values.PushBack(String(mState.GetAllocator(), aValue));
		return true;
	}

	bool Json::Writer::BeginArray() throw() {
		if(! mState.IsEmpty()) {
			State& state = mState.Back();
			if(state.type != STATE_ARRAY) return false;
		}
		State& state = mState.PushBack(State(mState.GetAllocator()));
		state.type = STATE_ARRAY;
		return true;
	}

	bool Json::Writer::EndArray() throw() {
		if(mState.IsEmpty()) return false; 
		State& state = mState.Back();
		if(state.type != STATE_ARRAY) return false;

		String buffer(mState.GetAllocator());
		const uint32_t size = state.values.Size();

		buffer += '[';
		for(uint32_t i = 0; i < size; ++i) {
			buffer += state.values[i];
			if(i + 1 < size) buffer += ',';
		}
		buffer += ']';

		mState.PopBack();

		if(mState.IsEmpty()) {
			mOutputStream.Write(buffer.CString(), buffer.Size());
		}else{
			mState.Back().values.PushBack(buffer);
		}

		return true;
	}

	bool Json::Writer::BeginObject() throw() {
		if(! mState.IsEmpty()) {
			State& state = mState.Back();
			if(state.type != STATE_ARRAY) return false;
		}
		State& state = mState.PushBack(State(mState.GetAllocator()));
		state.type = STATE_OBJECT;
		return true;
	}

	bool Json::Writer::EndObject() throw() {
		if(mState.IsEmpty()) return false; 
		State& state = mState.Back();
		if(state.type != STATE_OBJECT) return false;

		String buffer(mState.GetAllocator());
		const uint32_t size = state.values.Size();

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
			mOutputStream.Write(buffer.CString(), buffer.Size());
		}else{
			mState.Back().values.PushBack(buffer);
		}

		return true;
	}

	bool Json::Writer::AddValueNull() throw() {
		return AddValueInternal("null");
	}

	bool Json::Writer::AddValueBool(const bool aValue) throw() {
		return AddValueInternal(aValue ? "true" : "false");
	}

	bool Json::Writer::AddValueNumber(const double aValue) throw() {
		return AddValueInternal(WriteNumber(mState.GetAllocator(), aValue));
	}

	bool Json::Writer::AddValueString(const ConstStringFragment aValue) throw() {
		String buffer(mState.GetAllocator());
		buffer += '"';
		buffer += aValue;
		buffer += '"';
		return AddValueInternal(buffer);
	}

	bool Json::Writer::BeginArray(const ConstStringFragment aName) throw() {
		if(mState.IsEmpty()) return false; 
		State* state = &mState.Back();
		if(state->type != STATE_OBJECT) return false;
		state->names.PushBack(String(mState.GetAllocator(), aName));

		state = &mState.PushBack(State(mState.GetAllocator()));
		state->type = STATE_ARRAY;
		return true;
	}

	bool Json::Writer::BeginObject(const ConstStringFragment aName) throw() {
		if(mState.IsEmpty()) return false; 
		State* state = &mState.Back();
		if(state->type != STATE_OBJECT) return false;
		state->names.PushBack(String(mState.GetAllocator(), aName));

		state = &mState.PushBack(State(mState.GetAllocator()));
		state->type = STATE_OBJECT;
		return true;
	}

	bool Json::Writer::AddValueNull(const ConstStringFragment aName) throw() {
		return AddValueInternal(aName, "null");
	}

	bool Json::Writer::AddValueBool(const ConstStringFragment aName, const bool aValue) throw() {
		return AddValueInternal(aName, aValue ? "true" : "false");
	}

	bool Json::Writer::AddValueNumber(const ConstStringFragment aName, const double aValue) throw() {
		return AddValueInternal(aName, WriteNumber(mState.GetAllocator(), aValue));
	}

	bool Json::Writer::AddValueString(const ConstStringFragment aName, const ConstStringFragment aValue) throw() {
		String buffer(mState.GetAllocator());
		buffer += '"';
		buffer += aValue;
		buffer += '"';
		return AddValueInternal(aName, buffer);
	}

	bool Json::Writer::AddValue(const ConstStringFragment aName, const Value& aValue) throw() {
		switch (aValue.GetType()) {
		case Value::TYPE_BOOL:
			return AddValueBool(aName, aValue.GetBool());
		case Value::TYPE_CHAR:
			{
				const char buf = aValue.GetChar();
				return AddValueString(aName, String(mState.GetAllocator(), &buf, 1));
			}
		case Value::TYPE_INT:
		case Value::TYPE_UINT:
		case Value::TYPE_DOUBLE:
			return AddValueNumber(aName, aValue.GetDouble());
		case Value::TYPE_STRING:
			return AddValueString(aName, aValue.GetString());
		case Value::TYPE_ARRAY:
			{
				const Array& _array = aValue.GetArray();
				const uint32_t length = _array.Size();
				
				if(! BeginArray(aName)) return false;
				for(uint32_t i = 0; i < length; ++i) {
					if(! AddValue(_array[i])) return false;
				}
				if(! EndArray()) return false;

				return true;
			}
		case Value::TYPE_OBJECT:
			{
				const Object& object = aValue.GetObject();
				const uint32_t length = object.Size();
				
				if(! BeginObject(aName)) return false;
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

	bool Json::Writer::AddValue(const Value& aValue) throw() {
		switch (aValue.GetType()) {
		case Value::TYPE_BOOL:
			return AddValueBool(aValue.GetBool());
		case Value::TYPE_CHAR:
			{
				const char buf = aValue.GetChar();
				return AddValueString(String(mState.GetAllocator(), &buf, 1));
			}
		case Value::TYPE_INT:
		case Value::TYPE_UINT:
		case Value::TYPE_DOUBLE:
			return AddValueNumber(aValue.GetDouble());
		case Value::TYPE_STRING:
			return AddValueString(aValue.GetString());
		case Value::TYPE_ARRAY:
			{
				const Array& _array = aValue.GetArray();
				const uint32_t length = _array.Size();

				if(! BeginArray()) return false;
				for(uint32_t i = 0; i < length; ++i) {
					if(! AddValue(_array[i])) return false;
				}
				if(! EndArray()) return false;

				return true;
			}
		case Value::TYPE_OBJECT:
			{
				const Object& object = aValue.GetObject();
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

}}
