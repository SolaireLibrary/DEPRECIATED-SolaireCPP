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

namespace Solaire{ namespace Encode{

	static bool JsonParseNull(ReadStream&, Json::Reader&);
	static bool JsonParseBool(ReadStream&, Json::Reader&);
	static bool JsonParseNumber(ReadStream&, Json::Reader&);
	static bool JsonParseString(ReadStream&, Json::Reader&);
	static bool JsonParseArray(ReadStream&, Json::Reader&);
	static bool JsonParseObject(ReadStream&, Json::Reader&);

	class BaseParser {
	protected :
		enum Type {
			TYPE_NULL,
			TYPE_BOOL,
			TYPE_NUMBER,
			TYPE_STRING,
			TYPE_ARRAY,
			TYPE_OBJECT
		};

		static bool StateSkipWhitespace(ReadStream& aStream) {
			char buf;
			while(! aStream.End()) {
				aStream >> buf;
				switch(buf)
				{
				case ' ':
				case '\t':
				case '\n':
					break;
				default:
					aStream.SetOffset(aStream.GetOffset() - 1);
					return true;
				}
			}

			return true;
		}

		static bool StateParseValue(ReadStream& aStream, Json::Reader& aReader) {
			const uint32_t offset = aStream.GetOffset();
			StateSkipWhitespace(aStream);

			char buf;
			aStream >> buf;

			switch (buf)
			{
			case 'n':
				if(JsonParseNull(aStream, aReader)) {
					aStream.SetOffset(aStream.GetOffset() - 1);
					return true;
				}else{
					aStream.SetOffset(offset);
					return false;
				}
			case 't':
			case 'f':
				if(JsonParseBool(aStream, aReader)) {
					aStream.SetOffset(aStream.GetOffset() - 1);
					return true;
				}else{
					aStream.SetOffset(offset);
					return false;
				}
			case '-':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if(JsonParseNumber(aStream, aReader)) {
					aStream.SetOffset(aStream.GetOffset() - 1);
					return true;
				}else{
					aStream.SetOffset(offset);
					return false;
				}
			case '"':
				if(JsonParseString(aStream, aReader)) {
					aStream.SetOffset(aStream.GetOffset() - 1);
					return true;
				}else{
					aStream.SetOffset(offset);
					return false;
				}
			case '[':
				if(JsonParseArray(aStream, aReader)) {
					aStream.SetOffset(aStream.GetOffset() - 1);
					return true;
				}else{
					aStream.SetOffset(offset);
					return false;
				}
			case '{':
				if(JsonParseObject(aStream, aReader)) {
					aStream.SetOffset(aStream.GetOffset() - 1);
					return true;
				}else{
					aStream.SetOffset(offset);
					return false;
				}
			default:
				return false;
			}
		}
	};

	class StringParser : public BaseParser {
	private:
		static bool StateFirstQuote(ReadStream& aStream) {
			if (!StateSkipWhitespace(aStream)) return false;
			char buf;
			while(! aStream.End()) {
				aStream >> buf;
				if(buf == '"') {
					return true;
				}else{
					return false;
				}
			}
			return false;
		}

		static bool StateBody(ReadStream& aStream, Json::Reader& aReader) {
			bool escaped = false;
			String buffer(DEFAULT_ALLOCATOR);
			char c;

			while(! aStream.End()) {
				aStream >> c;
				switch(c) {
				case '"' :
					if(escaped) {
						escaped = false;
						buffer += c;
					}else {
						if(! aReader.ValueString(buffer)) return false;
						return true;
					}
					break;
				case '\\' :
					if(escaped) {
						escaped = false;
						buffer += c;
					}else {
						escaped = true;
					}
					break;
				default :
					escaped = false;
					buffer += c;
					break;
				}
			}

			return false;
		}
	public:
		static bool Parse(ReadStream& aStream, Json::Reader& aReader) {
			const uint32_t offset = aStream.GetOffset();

			if(! StateFirstQuote(aStream)) {
				aStream.SetOffset(offset);
				return false;
			}

			if(! StateBody(aStream, aReader)) {
				aStream.SetOffset(offset);
				return false;
			}

			return true;
		}
	};

	class NumberParser : public BaseParser {
	private:
		static bool StateValue(ReadStream& aStream, Json::Reader& aReader) {
			String buf(DEFAULT_ALLOCATOR);
			double value;

			while(true) {
				if(aStream.End()) return false;
				char c;
				aStream >> c;
				buf += c;

				switch(c)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				case '.':
				case '-':
				case 'E':
				case 'e':
					break;
				default:
					const char* end = ReadNumber<double>(buf.CString(), buf.CString() + buf.Size(), value);
					if(end == buf.CString()) return false;
					if (!aReader.ValueNumber(value)) return false;
					aStream.SetOffset(aStream.GetOffset() - 1);
					return true;
				}
			}

			return false;
		}
	public:
		static bool Parse(ReadStream& aStream, Json::Reader& aReader) {
			const uint32_t offset = aStream.GetOffset();

			if(! StateSkipWhitespace(aStream)) {
				aStream.SetOffset(offset);
				return false;
			}

			if(! StateValue(aStream, aReader)) {
				aStream.SetOffset(offset);
				return false;
			}

			return true;
		}
	};

	class BoolParser : public BaseParser {
	private:
		static bool StateValue(ReadStream& aStream, Json::Reader& aReader) {
			char buf[4];
			aStream >> buf[0];

			if(buf[0] == 'f') {
				if(aStream.Read(buf, 4) != 4) return false;
				if(std::memcmp(buf, "alse", 4) != 0) return false;
				if(! aReader.ValueBool(false)) return false;
			}else if(buf[0] == 't') {
				if(aStream.Read(buf, 3) != 3) return false;
				if(std::memcmp(buf, "rue", 3) != 0) return false;
				if(! aReader.ValueBool(false)) return false;
			}else {
				return false;
			}
		}
	public:
		static bool Parse(ReadStream& aStream, Json::Reader& aReader) {
			const uint32_t offset = aStream.GetOffset();

			if(! StateSkipWhitespace(aStream)) {
				aStream.SetOffset(offset);
				return false;
			}

			if(! StateValue(aStream, aReader)) {
				aStream.SetOffset(offset);
				return false;
			}

			return true;
		}
	};

	class NullParser : public BaseParser {
	private:
		static bool StateValue(ReadStream& aStream, Json::Reader& aReader) {
			char buf[4];
			if(aStream.Read(buf, 4) != 4) return false;
			if(std::memcmp(buf, "null", 4) != 0) return false;
			if(! aReader.ValueNull()) return false;
			return true;
		}
	public:
		static bool Parse(ReadStream& aStream, Json::Reader& aReader) {
			const uint32_t offset = aStream.GetOffset();

			if(! StateSkipWhitespace(aStream)) {
				aStream.SetOffset(offset);
				return false;
			}

			if(! StateValue(aStream, aReader)) {
				aStream.SetOffset(offset);
				return false;
			}

			return true;
		}
	};

	///*class ArrayParser : public BaseParser {
	//private:
	//	static const char* StateOpenArray(const char* const aBegin, const char* aEnd, Json::Reader& aReader) {
	//		const char* i = StateSkipWhitespace(aBegin, aEnd);
	//		if(*i != '[') return aBegin;
	//		aReader.BeginArray();
	//		return i + 1;
	//	}

	//	static const char* StateCloseArray(const char* const aBegin, const char* aEnd, Json::Reader& aReader) {
	//		const char* i = StateSkipWhitespace(aBegin, aEnd);
	//		if (*i != ']') return aBegin;
	//		aReader.EndArray();
	//		return i + 1;
	//	}

	//	static const char* StateIdentifyChild(const char* const aBegin, const char* aEnd, Json::Reader& aReader) {
	//		return aBegin;
	//	}

	//	static const char* StateParseChild(const char* const aBegin, const char* aEnd, Json::Reader& aReader) {
	//		return aBegin;
	//	}

	//	static const char* StateChildSeperator(const char* const aBegin, const char* aEnd, Json::Reader& aReader) {
	//		const char* i = StateSkipWhitespace(aBegin, aEnd);
	//		while(i != aEnd) {
	//			switch(*i) {
	//			case ',':
	//				return i + 1;
	//			case ']':
	//				return StateCloseArray(aBegin, aEnd, aReader);
	//			default:
	//				return aBegin;
	//			}
	//		}
	//		return aBegin;
	//	}
	//public:
	//	static const char* Parse(const char* const aBegin, const char* aEnd, Json::Reader& aReader) {
	//		return aBegin; 
	//		//const char* const e1 = StateSkipWhitespace(aBegin, aEnd);
	//		//const char* const e2 = StateValue(e1, aEnd);
	//		//return e2 == e1 ? aBegin : e2;
	//	}
	//};*/

	bool JsonParseNull(ReadStream& aStream, Json::Reader& aReader) {
		return false;// NullParser::Parse(aBegin, aEnd, aReader);
	}

	bool JsonParseBool(ReadStream& aStream, Json::Reader& aReader) {
		return false;// BoolParser::Parse(aBegin, aEnd, aReader);
	}

	bool JsonParseNumber(ReadStream& aStream, Json::Reader& aReader) {
		return false;// NumberParser::Parse(aBegin, aEnd, aReader);
	}

	bool JsonParseString(ReadStream& aStream, Json::Reader& aReader){
		return false;// StringParser::Parse(aBegin, aEnd, aReader);
	}

	bool JsonParseArray(ReadStream& aStream, Json::Reader& aReader) {
		return false;// ArrayParser::Parse(aBegin, aEnd, aReader);
	}

	bool JsonParseObject(ReadStream& aStream, Json::Reader& aReader) {
		return false;// ObjectParser::Parse(aBegin, aEnd, aReader);
	}

	//// Json

	//class ValueReader : public Json::Reader {
	//private:
	//	struct State{
	//		Value* value;
	//		String objectName;
	//		bool objectMode;
	//	};

	//	Value mRoot;
	//	DynamicArray<State> mHead;
	//private:
	//	bool ValueValue(const Value& aValue) throw() {
	//		if(mHead.IsEmpty()) return false;
	//		State& state = mHead.Back();

	//		if(state.objectMode) {
	//			if(state.objectName.Size() == 0) return false;
	//			state.value->GetObject().Add(state.objectName, aValue);
	//		}else {
	//			state.value->GetArray().Add(aValue);
	//		}
	//		return true;
	//	}
	//public:
	//	ValueReader(Allocator& aAllocator) :
	//		mRoot(aAllocator),
	//		mHead(aAllocator)
	//	{}

	//	Value GetValue() const throw() {
	//		return mRoot;
	//	}

	//	// Inherited from Reader

	//	bool SOLAIRE_EXPORT_CALL BeginArray() throw() override {
	//		//! \todo Implement BeginArray
	//		return false;
	//	}

	//	bool SOLAIRE_EXPORT_CALL EndArray() throw() override {
	//		//! \todo Implement EndArray
	//		return false;
	//	}

	//	bool SOLAIRE_EXPORT_CALL BeginObject() throw() override {
	//		//! \todo Implement BeginObject
	//		return false;
	//	}

	//	bool SOLAIRE_EXPORT_CALL EndObject() throw() override {
	//		//! \todo Implement EndObject
	//		return false;
	//	}

	//	bool SOLAIRE_EXPORT_CALL MemberName(const ConstStringFragment aName) throw() override {
	//		if(mHead.IsEmpty()) return false;
	//		State& state = mHead.Back();
	//		if(! state.objectMode) return false;
	//		state.objectName = aName;
	//		return true;
	//	}

	//	bool SOLAIRE_EXPORT_CALL ValueNull() throw() override {
	//		return ValueValue(Value(mHead.GetAllocator()));
	//	}

	//	bool SOLAIRE_EXPORT_CALL ValueBool(const bool aValue) throw() override {
	//		return ValueValue(Value(mHead.GetAllocator(), aValue));
	//	}

	//	bool SOLAIRE_EXPORT_CALL ValueNumber(const double aValue) throw() override {
	//		return ValueValue(Value(mHead.GetAllocator(), aValue));
	//	}

	//	bool SOLAIRE_EXPORT_CALL ValueString(const ConstStringFragment aValue) throw() override {
	//		return ValueValue(Value(mHead.GetAllocator(), String(mHead.GetAllocator(), aValue)));
	//	}

	//};

	//bool SOLAIRE_EXPORT_CALL Json::Read(ReadStream& aInputStream, Reader& aReader) {
	//	//! \todo Implement Read
	//	return false;
	//}

	//Value SOLAIRE_EXPORT_CALL Json::Read(ReadStream& aInputStream) {
	//	return Read(DEFAULT_ALLOCATOR, aInputStream);
	//}

	//Value SOLAIRE_EXPORT_CALL Json::Read(Allocator& aAllocator, ReadStream& aInputStream) {
	//	ValueReader reader(aAllocator);
	//	if(! Read(aInputStream, reader)) return Value(aAllocator);
	//	return reader.GetValue();
	//}



	//class ArrayParser : public Parser {
	//private:
	//	DynamicArray<std::shared_ptr<Parser>> mParsers;
	//	Allocator& mAllocator;
	//	enum {
	//		STATE_OPEN_ARRAY,
	//		STATE_CLOSE_ARRAY,
	//		STATE_IDENTIFY_CHILD,
	//		STATE_PARSE_CHILD,
	//		STATE_CHILD_SEPARATOR
	//	};
	//	uint8_t mState;
	//public:
	//	ArrayParser(Allocator& aAllocator) :
	//		mParsers(32, aAllocator),
	//		mAllocator(aAllocator),
	//		mState(STATE_OPEN_ARRAY)
	//	{}

	//	// Inherited from aAllocator

	//	Status Accept(const Flags aFlags, const char aChar) override {
	//	LABEL_START:
	//		switch (mState) {
	//		case STATE_OPEN_ARRAY:
	//			goto LABEL_OPEN_ARRAY;
	//		case STATE_CLOSE_ARRAY:
	//			goto LABEL_CLOSE_ARRAY;
	//		case STATE_IDENTIFY_CHILD:
	//			goto LABEL_IDENTIFY_CHILD;
	//		case STATE_PARSE_CHILD:
	//			goto LABEL_PARSE_CHILD;
	//		case STATE_CHILD_SEPARATOR:
	//			goto LABEL_CHILD_SEPARATOR;
	//		default:
	//			throw std::runtime_error("Json::ArrayParser : Invalid State");
	//		}


	//	LABEL_OPEN_ARRAY:
	//		switch (aChar) {
	//		case ' ':
	//		case '\t':
	//		case '\n':
	//			return STATUS_SUCCESS;
	//		case '[':
	//			mState = STATE_IDENTIFY_CHILD;
	//			return STATUS_SUCCESS;
	//		default:
	//			return STATUS_FAIL;
	//		}

	//	LABEL_CLOSE_ARRAY:
	//		return STATUS_COMPLETE;

	//	LABEL_PARSE_CHILD:
	//		{
	//			const Status  status = mParsers.Back()->Accept(aChar);
	//			if (status == STATUS_SUCCESS) {
	//				return STATUS_SUCCESS;
	//			}
	//			else if (status == STATUS_FAIL) {
	//				return STATUS_FAIL;
	//			}
	//			else if (aChar == ']') {
	//				mState = STATE_CLOSE_ARRAY;
	//				goto LABEL_START;
	//			}
	//			else {
	//				mState = STATE_CHILD_SEPARATOR;
	//				return STATUS_SUCCESS;
	//			}
	//		}

	//	LABEL_CHILD_SEPARATOR:
	//		switch (aChar) {
	//		case ' ':
	//		case '\t':
	//		case '\n':
	//			return STATUS_SUCCESS;
	//		case ',':
	//			mState = STATE_IDENTIFY_CHILD;
	//			return STATUS_SUCCESS;
	//		case ']':
	//			mState = STATE_CLOSE_ARRAY;
	//			goto LABEL_START;
	//		default:
	//			return STATUS_FAIL;
	//		}

	//	LABEL_IDENTIFY_CHILD:
	//		switch (aChar) {
	//		case ' ':
	//		case '\t':
	//		case '\n':
	//			return STATUS_SUCCESS;
	//		case ']':
	//			if (mParsers.IsEmpty()) {
	//				mState = STATE_CLOSE_ARRAY;
	//				goto LABEL_START;
	//			}
	//			else {
	//				return STATUS_FAIL;
	//			}
	//		default:
	//			TypeID id;
	//			try {
	//				id = IdentifyTypeID(aChar);
	//			}
	//			catch (...) {
	//				return STATUS_FAIL;
	//			}
	//			mParsers.PushBack(AllocateParser(mAllocator, id));

	//			mState = STATE_PARSE_CHILD;
	//			goto LABEL_START;
	//		}
	//	}

	//	std::shared_ptr<Value> Get(Allocator& aParseAllocator, Allocator& aDocAllocator) const override {
	//		if (mState != STATE_CLOSE_ARRAY) return nullptr;
	//		std::shared_ptr<Value> ptr = aParseAllocator.SharedAllocate<Value>(TYPE_ARRAY, aDocAllocator);
	//		Array& array_ = *ptr;
	//		for (std::shared_ptr<Parser> i : mParsers) {
	//			array_.PushBack(i->Get(aParseAllocator, aDocAllocator));
	//		}
	//		return ptr;
	//	}
	//};

	//class ObjectParser : public Parser {
	//private:
	//	DynamicArray<std::pair<String, std::shared_ptr<Parser>>> mParsers;
	//	Allocator& mAllocator;
	//	String mName;
	//	struct {
	//		uint8_t openFound : 1;
	//		uint8_t closeFound : 1;
	//		uint8_t nameParsed : 1;
	//		uint8_t nameOpen : 1;
	//		uint8_t nameClose : 1;
	//		uint8_t parseChild : 1;
	//	};
	//	enum {
	//		STATE_OPEN_OBJECT,
	//		STATE_CLOSE_OBJECT,
	//		STATE_OPEN_NAME,
	//		STATE_PARSE_NAME,
	//		STATE_NAME_SEPERATOR,
	//		STATE_IDENTIFY_CHILD,
	//		STATE_PARSE_CHILD,
	//		STATE_CHILD_SEPARATOR
	//	};
	//	uint8_t mState;
	//public:
	//	ObjectParser(Allocator& aAllocator) :
	//		mParsers(32, aAllocator),
	//		mAllocator(aAllocator),
	//		mName(aAllocator),
	//		mState(STATE_OPEN_OBJECT)
	//	{}

	//	// Inherited from aAllocator

	//	Status Accept(const Flags aFlags, const char aChar) override {
	//	LABEL_START:
	//		switch (mState) {
	//		case STATE_OPEN_OBJECT:
	//			goto LABEL_OPEN_OBJECT;
	//		case STATE_CLOSE_OBJECT:
	//			goto LABEL_CLOSE_OBJECT;
	//		case STATE_OPEN_NAME:
	//			goto LABEL_OPEN_NAME;
	//		case STATE_PARSE_NAME:
	//			goto LABEL_PARSE_NAME;
	//		case STATE_NAME_SEPERATOR:
	//			goto LABEL_NAME_SEPERATOR;
	//		case STATE_IDENTIFY_CHILD:
	//			goto LABEL_IDENTIFY_CHILD;
	//		case STATE_PARSE_CHILD:
	//			goto LABEL_PARSE_CHILD;
	//		case STATE_CHILD_SEPARATOR:
	//			goto LABEL_CHILD_SEPARATOR;
	//		default:
	//			throw std::runtime_error("Json::ObjectParser : Invalid state");
	//		}


	//	LABEL_OPEN_OBJECT:
	//		switch (aChar) {
	//		case ' ':
	//		case '\t':
	//		case '\n':
	//			return STATUS_SUCCESS;
	//		case '{':
	//			mState = STATE_OPEN_NAME;
	//			return STATUS_SUCCESS;
	//		default:
	//			return STATUS_FAIL;
	//		}

	//	LABEL_CLOSE_OBJECT:
	//		return STATUS_COMPLETE;

	//	LABEL_OPEN_NAME:
	//		switch (aChar) {
	//		case ' ':
	//		case '\t':
	//		case '\n':
	//			return STATUS_SUCCESS;
	//		case '"':
	//			mState = STATE_PARSE_NAME;
	//			return STATUS_SUCCESS;
	//		case '}':
	//			if (mParsers.IsEmpty()) {
	//				mState = STATE_CLOSE_OBJECT;
	//				goto LABEL_START;
	//			}
	//			else {
	//				return STATUS_FAIL;
	//			}
	//		default:
	//			return STATUS_FAIL;
	//		}

	//	LABEL_PARSE_NAME:
	//		if (aChar == '"' && !(aFlags & FLAG_IS_ESCAPED)) {
	//			mState = STATE_NAME_SEPERATOR;
	//			return STATUS_SUCCESS;
	//		}
	//		mName += aChar;
	//		return STATUS_SUCCESS;

	//	LABEL_NAME_SEPERATOR:
	//		switch (aChar) {
	//		case ' ':
	//		case '\t':
	//		case '\n':
	//			return STATUS_SUCCESS;
	//		case ':':
	//			mState = STATE_IDENTIFY_CHILD;
	//			return STATUS_SUCCESS;
	//		default:
	//			return STATUS_FAIL;
	//		}

	//	LABEL_PARSE_CHILD:
	//		{
	//			const Status status = mParsers.Back().second->Accept(aChar);
	//			if (status == STATUS_SUCCESS) {
	//				return STATUS_SUCCESS;
	//			}
	//			else if (status == STATUS_FAIL) {
	//				return STATUS_FAIL;
	//			}
	//			else if (aChar == '}') {
	//				mState = STATE_CLOSE_OBJECT;
	//				goto LABEL_START;
	//			}
	//			else {
	//				mState = STATE_CHILD_SEPARATOR;
	//				goto LABEL_START;
	//			}
	//		}

	//	LABEL_CHILD_SEPARATOR:
	//		switch (aChar) {
	//		case ' ':
	//		case '\t':
	//		case '\n':
	//			return STATUS_SUCCESS;
	//		case ',':
	//			mState = STATE_OPEN_NAME;
	//			return STATUS_SUCCESS;
	//		case '}':
	//			mState = STATE_CLOSE_OBJECT;
	//			goto LABEL_START;
	//		default:
	//			return STATUS_FAIL;
	//		}

	//	LABEL_IDENTIFY_CHILD:
	//		switch (aChar) {
	//		case ' ':
	//		case '\t':
	//		case '\n':
	//			return STATUS_SUCCESS;
	//		case '}':
	//			mState = STATE_CLOSE_OBJECT;
	//			goto LABEL_START;
	//		default:
	//			TypeID id;
	//			try {
	//				id = IdentifyTypeID(aChar);
	//			}
	//			catch (...) {
	//				return STATUS_FAIL;
	//			}

	//			mParsers.PushBack(std::pair<String, std::shared_ptr<Parser>>(mName, AllocateParser(mAllocator, id)));
	//			mName.Clear();

	//			mState = STATE_PARSE_CHILD;
	//			goto LABEL_START;
	//		}
	//	}

	//	std::shared_ptr<Value> Get(Allocator& aParseAllocator, Allocator& aDocAllocator) const override {
	//		if (mState != STATE_CLOSE_OBJECT) return nullptr;

	//		std::shared_ptr<Value> ptr = aParseAllocator.SharedAllocate<Value>(TYPE_OBJECT, aDocAllocator);
	//		Object& object = *ptr;
	//		for (auto& i : mParsers) {
	//			object.emplace(i.first, i.second->Get(aParseAllocator, aDocAllocator));
	//		}
	//		return ptr;
	//	}
	//};

	//static TypeID IdentifyTypeID(const String::Type aChar) {
	//	switch (aChar) {
	//	case '[':
	//		return GetTypeID<Array>();
	//	case '{':
	//		return GetTypeID<Object>();
	//	case '"':
	//		return GetTypeID<String>();
	//	case '-':
	//	case '0':
	//	case '1':
	//	case '2':
	//	case '3':
	//	case '4':
	//	case '5':
	//	case '6':
	//	case '7':
	//	case '8':
	//	case '9':
	//		return GetTypeID<Number>();
	//	case 't':
	//	case 'f':
	//		return GetTypeID<Bool>();
	//	case 'n':
	//		return GetTypeID<Null>();
	//	default:
	//		break;
	//	}
	//	throw std::runtime_error("Json::IdentifyTypeID : Could not identify TypeID");
	//}

	//static std::shared_ptr<Parser> AllocateParser(Allocator& aAllocator, const TypeID aID) {
	//	switch (aID) {
	//	case GetTypeID<Null>() :
	//		return aAllocator.SharedAllocate<NullParser>();
	//	case GetTypeID<Bool>() :
	//		return aAllocator.SharedAllocate<BoolParser>();
	//	case GetTypeID<Number>() :
	//		return aAllocator.SharedAllocate<NumberParser>();
	//	case GetTypeID<String>() :
	//		return aAllocator.SharedAllocate<StringParser>(aAllocator);
	//	case GetTypeID<Array>() :
	//		return aAllocator.SharedAllocate<ArrayParser>(aAllocator);
	//	case GetTypeID<Object>() :
	//		return aAllocator.SharedAllocate<ObjectParser>(aAllocator);
	//	default:
	//		return nullptr;
	//	}
	//}

}}
