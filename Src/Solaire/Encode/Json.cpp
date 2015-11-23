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
		char buf = '[';
		mOutputStream.Write(&buf, sizeof(char));
		return true;
	}

	bool Json::Writer::EndArray() throw() {
		if(mState.IsEmpty() || mState.Back() != STATE_ARRAY) return false;
		mState.PopBack();
		char buf = ']';
		mOutputStream.Write(&buf, sizeof(char));
		if(! mState.IsEmpty()) {
			buf = ',';
			mOutputStream.Write(&buf, sizeof(char));
		}
		return true;
	}

	bool Json::Writer::BeginObject() throw() {
		if(mState.IsEmpty() || mState.Back() != STATE_ARRAY) return false;
		mState.PushBack(STATE_OBJECT);
		char buf = '{';
		mOutputStream.Write(&buf, sizeof(char));
		return true;
	}

	bool Json::Writer::EndObject() throw() {
		if(mState.IsEmpty() || mState.Back() != STATE_OBJECT) return false;
		mState.PopBack();
		char buf = '}';
		mOutputStream.Write(&buf, sizeof(char));
		if(! mState.IsEmpty()) {
			buf = ',';
			mOutputStream.Write(&buf, sizeof(char));
		}
		return true;
	}

	bool Json::Writer::AddValueNull() throw() {
		if((!mState.IsEmpty()) || mState.Back() != STATE_ARRAY) return false;

		char buf[] = "null,";
		mOutputStream.Write(&buf, sizeof(char) * std::strlen(buf));

		return true;
	}

	bool Json::Writer::AddValueBool(const bool aValue) throw() {
		if((!mState.IsEmpty()) || mState.Back() != STATE_ARRAY) return false;

		if(aValue) {
			char buf[] = "true,";
			mOutputStream.Write(&buf, sizeof(char) * std::strlen(buf));
		}else {
			char buf[] = "false,";
			mOutputStream.Write(&buf, sizeof(char) * std::strlen(buf));
		}

		return true;
	}

	bool Json::Writer::AddValueNumber(const double aValue) throw() {
		if((!mState.IsEmpty()) || mState.Back() != STATE_ARRAY) return false;

		String buf = WriteNumber(aValue);
		buf += ',';
		mOutputStream.Write(buf.CString(), sizeof(char) * buf.Size());

		return true;
	}

	bool Json::Writer::AddValueString(const ConstStringFragment aValue) throw() {
		if((! mState.IsEmpty()) || mState.Back() != STATE_ARRAY) return false;

		String buf = String(DEFAULT_ALLOCATOR, aValue);
		buf += ',';
		mOutputStream.Write(buf.CString(), sizeof(char) * buf.Size());

		return true;
	}

	bool Json::Writer::BeginArray(const ConstStringFragment aValue) throw() {
		if(mState.IsEmpty() || mState.Back() != STATE_OBJECT) return false;
		mState.PushBack(STATE_ARRAY);

		char buf = '[';
		mOutputStream.Write(&buf, sizeof(char));

		return true;
	}

	bool Json::Writer::BeginObject(const ConstStringFragment aName) throw() {
		if(mState.IsEmpty() || mState.Back() != STATE_OBJECT) return false;
		mState.PushBack(STATE_OBJECT);

		char buf = '{';
		mOutputStream.Write(&buf, sizeof(char));

		return true;
	}

	bool Json::Writer::AddValueNull(const ConstStringFragment aName) throw() {
		if((! mState.IsEmpty()) || mState.Back() != STATE_OBJECT) return false;

		String buf(DEFAULT_ALLOCATOR, "\""); 
		buf += String(DEFAULT_ALLOCATOR, aName);
		buf +=  "\" : null,";
		mOutputStream.Write(buf.CString(), sizeof(char) * buf.Size());

		return true;
	}

	bool Json::Writer::AddValueBool(const ConstStringFragment aName, const bool aValue) throw() {
		if((! mState.IsEmpty()) || mState.Back() != STATE_OBJECT) return false;

		String buf(DEFAULT_ALLOCATOR, "\"");
		buf += String(DEFAULT_ALLOCATOR, aName);

		if(aValue) {
			buf += "\" : true,";
		}else {
			buf += "\" : false,";
		}
		mOutputStream.Write(buf.CString(), sizeof(char) * buf.Size());

		return true;
	}

	bool Json::Writer::AddValueNumber(const ConstStringFragment aName, const double aValue) throw() {
		if((! mState.IsEmpty()) || mState.Back() != STATE_OBJECT) return false;

		String buf(DEFAULT_ALLOCATOR, "\"");
		buf += String(DEFAULT_ALLOCATOR, aName);
		buf += "\" : ";
		buf += WriteNumber(aValue);
		buf += ',';
		mOutputStream.Write(buf.CString(), sizeof(char) * buf.Size());

		return true;
	}

	bool Json::Writer::AddValueString(const ConstStringFragment aName, const ConstStringFragment aValue) throw() {
		if((! mState.IsEmpty()) || mState.Back() != STATE_OBJECT) return false;

		String buf(DEFAULT_ALLOCATOR, "\"");
		buf += String(DEFAULT_ALLOCATOR, aName);
		buf += "\" : ";
		buf += aValue;
		buf += ',';
		mOutputStream.Write(buf.CString(), sizeof(char) * buf.Size());

		return true;
	}

	// Json

	static bool WriteArray(const Value& aValue, Json::Writer& aWriter);

	static bool WriteObject(const String& aName, const Value& aValue, Json::Writer& aWriter) {
		switch (aValue.GetType()) {
		case Value::TYPE_BOOL:
			return aWriter.AddValueBool(aName, aValue.GetBool());
		case Value::TYPE_CHAR:
			{
				const char buf = aValue.GetChar();
				return aWriter.AddValueString(aName, String(DEFAULT_ALLOCATOR, &buf, 1));
			}
		case Value::TYPE_INT:
		case Value::TYPE_UINT:
		case Value::TYPE_DOUBLE:
			return aWriter.AddValueNumber(aName, aValue.GetDouble());
		case Value::TYPE_STRING:
			return aWriter.AddValueString(aName, aValue.GetString());
		case Value::TYPE_ARRAY:
			{
				const Array& _array = aValue.GetArray();
				const uint32_t length = _array.Size();
				
				if(! aWriter.BeginArray(aName)) return false;
				for(uint32_t i = 0; i < length; ++i) {
					if (!WriteArray(_array[i], aWriter)) return false;
				}
				if(! aWriter.EndArray()) return false;

				return true;
			}
		case Value::TYPE_OBJECT:
			{
				const Object& object = aValue.GetObject();
				const uint32_t length = object.Size();
				
				if(! aWriter.BeginObject(aName)) return false;
				for(uint32_t i = 0; i < length; ++i) {
					if (!WriteObject(object.GetMemberName(i), object[i], aWriter)) return false;
				}
				if(! aWriter.EndObject()) return false;

				return true;
			}
		default:
			return false;
		}
	}

	static bool WriteArray(const Value& aValue, Json::Writer& aWriter) {
		switch (aValue.GetType()) {
		case Value::TYPE_BOOL:
			return aWriter.AddValueBool(aValue.GetBool());
		case Value::TYPE_CHAR:
			{
				const char buf = aValue.GetChar();
				return aWriter.AddValueString(String(DEFAULT_ALLOCATOR, &buf, 1));
			}
		case Value::TYPE_INT:
		case Value::TYPE_UINT:
		case Value::TYPE_DOUBLE:
			return aWriter.AddValueNumber(aValue.GetDouble());
		case Value::TYPE_STRING:
			return aWriter.AddValueString(aValue.GetString());
		case Value::TYPE_ARRAY:
			{
				const Array& _array = aValue.GetArray();
				const uint32_t length = _array.Size();

				if(! aWriter.BeginArray()) return false;
				for(uint32_t i = 0; i < length; ++i) {
					if(! WriteArray(_array[i], aWriter)) return false;
				}
				if(! aWriter.EndArray()) return false;

				return true;
			}
		case Value::TYPE_OBJECT:
			{
				const Object& object = aValue.GetObject();
				const uint32_t length = object.Size();
				
				if(! aWriter.BeginObject()) return false;
				for(uint32_t i = 0; i < length; ++i) {
					if (!WriteObject(object.GetMemberName(i), object[i], aWriter)) return false;
				}
				if(! aWriter.EndObject()) return false;

				return true;
			}
		default:
			return false;
		}
	}

	bool SOLAIRE_EXPORT_CALL Json::Write(const Value& aValue, Writer& aWriter){

		switch(aValue.GetType()) {
		case Value::TYPE_ARRAY:
			{
				const Array& _array = aValue.GetArray();
				const uint32_t length = _array.Size();
				
				if(! aWriter.BeginArray()) return false;
				for(uint32_t i = 0; i < length; ++i) {
					if (! WriteArray(_array[i], aWriter)) return false;
				}
				if(! aWriter.EndArray()) return false;

				return true;
			}
		case Value::TYPE_OBJECT:
			{
				const Object& object = aValue.GetObject();
				const uint32_t length = object.Size();
				
				if(! aWriter.BeginObject()) return false;
				for(uint32_t i = 0; i < length; ++i) {
					if (!WriteObject(object.GetMemberName(i), object[i], aWriter)) return false;
				}
				if(! aWriter.EndObject()) return false;

				return true;
			}
		default:
			return false;
		}
	}

}}
