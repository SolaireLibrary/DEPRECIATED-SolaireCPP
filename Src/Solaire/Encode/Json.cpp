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

	// Writer

	Json::Writer::Writer(WriteStream& aStream) :
		mOutputStream(aStream),
		mState(DEFAULT_ALLOCATOR)
	{}

	Json::Writer::~Writer() {
		mOutputStream.Flush();
	}

	bool Json::Writer::BeginArray() throw() {
		if(mState.IsEmpty() || mState.Back() != STATE_ARRAY) return false;
		mState.PushBack(STATE_ARRAY);
		mOutputStream << '[';
		return true;
	}

	bool Json::Writer::EndArray() throw() {
		if(mState.IsEmpty() || mState.Back() != STATE_ARRAY) return false;
		mState.PopBack();
		mOutputStream << ']';
		if(! mState.IsEmpty()) {
			mOutputStream << ',';
		}
		return true;
	}

	bool Json::Writer::BeginObject() throw() {
		if(mState.IsEmpty() || mState.Back() != STATE_ARRAY) return false;
		mState.PushBack(STATE_OBJECT);
		mOutputStream << '{';
		return true;
	}

	bool Json::Writer::EndObject() throw() {
		if(mState.IsEmpty() || mState.Back() != STATE_OBJECT) return false;
		mState.PopBack();
		mOutputStream << '}';
		if(! mState.IsEmpty()) {
			mOutputStream << ',';
		}
		return true;
	}

	bool Json::Writer::AddValueNull() throw() {
		if((!mState.IsEmpty()) || mState.Back() != STATE_ARRAY) return false;

		mOutputStream.WriteCString("null,");

		return true;
	}

	bool Json::Writer::AddValueBool(const bool aValue) throw() {
		if((!mState.IsEmpty()) || mState.Back() != STATE_ARRAY) return false;

		if(aValue) {
			mOutputStream.WriteCString("true,");
		}else {
			mOutputStream.WriteCString("false,");
		}

		return true;
	}

	bool Json::Writer::AddValueNumber(const double aValue) throw() {
		if((!mState.IsEmpty()) || mState.Back() != STATE_ARRAY) return false;

		String buf = WriteNumber(aValue);
		buf += ',';
		mOutputStream.WriteCString(buf.CString());

		return true;
	}

	bool Json::Writer::AddValueString(const ConstStringFragment aValue) throw() {
		if((! mState.IsEmpty()) || mState.Back() != STATE_ARRAY) return false;

		String buf = String(DEFAULT_ALLOCATOR, aValue);
		buf += ',';
		mOutputStream.WriteCString(buf.CString());

		return true;
	}

	bool Json::Writer::BeginArray(const ConstStringFragment aValue) throw() {
		if(mState.IsEmpty() || mState.Back() != STATE_OBJECT) return false;
		mState.PushBack(STATE_ARRAY);

		mOutputStream << '[';

		return true;
	}

	bool Json::Writer::BeginObject(const ConstStringFragment aName) throw() {
		if(mState.IsEmpty() || mState.Back() != STATE_OBJECT) return false;
		mState.PushBack(STATE_OBJECT);

		mOutputStream << '{';

		return true;
	}

	bool Json::Writer::AddValueNull(const ConstStringFragment aName) throw() {
		if((! mState.IsEmpty()) || mState.Back() != STATE_OBJECT) return false;

		String buf(DEFAULT_ALLOCATOR, "\""); 
		buf += String(DEFAULT_ALLOCATOR, aName);
		buf +=  "\" : null,";
		mOutputStream.WriteCString(buf.CString());

		return true;
	}

	bool Json::Writer::AddValueBool(const ConstStringFragment aName, const bool aValue) throw() {
		if((! mState.IsEmpty()) || mState.Back() != STATE_OBJECT) return false;

		mOutputStream << '"';
		String buf(DEFAULT_ALLOCATOR, "\"");
		mOutputStream.Write(aName.begin(), aName.Size());
		
		if(aValue) {
			mOutputStream.WriteCString("\" : true,");
		}else {
			mOutputStream.WriteCString("\" : false,");
		}

		return true;
	}

	bool Json::Writer::AddValueNumber(const ConstStringFragment aName, const double aValue) throw() {
		if((! mState.IsEmpty()) || mState.Back() != STATE_OBJECT) return false;

		mOutputStream << '"';
		mOutputStream.Write(aName.begin(), aName.Size());
		mOutputStream.WriteCString("\" : ");
		mOutputStream.WriteCString(WriteNumber(aValue).CString());
		mOutputStream << ',';

		return true;
	}

	bool Json::Writer::AddValueString(const ConstStringFragment aName, const ConstStringFragment aValue) throw() {
		if((! mState.IsEmpty()) || mState.Back() != STATE_OBJECT) return false;

		mOutputStream << '"';
		mOutputStream.Write(aName.begin(), aName.Size());
		mOutputStream.WriteCString("\" : \"");
		mOutputStream.Write(aValue.begin(), aValue.Size());
		mOutputStream << '"';
		mOutputStream << ',';

		return true;
	}

	bool Json::Writer::AddValue(const ConstStringFragment aName, const Value& aValue) throw() {
		switch (aValue.GetType()) {
		case Value::TYPE_BOOL:
			return AddValueBool(aName, aValue.GetBool());
		case Value::TYPE_CHAR:
			{
				const char buf = aValue.GetChar();
				return AddValueString(aName, String(DEFAULT_ALLOCATOR, &buf, 1));
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
				return AddValueString(String(DEFAULT_ALLOCATOR, &buf, 1));
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
