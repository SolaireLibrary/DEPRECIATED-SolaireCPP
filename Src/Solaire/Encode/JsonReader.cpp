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

	class ArrayParser : public BaseParser {
	private:
		static bool StateOpenArray(ReadStream& aStream, Json::Reader& aReader) {
			if(! StateSkipWhitespace(aStream)) return false;
			char c;
			aStream >> c;
			if(c != '[') return false;
			if(! aReader.BeginArray()) return false;
			return StateCloseArray(aStream, aReader);
		}

		static bool StateCloseArray(ReadStream& aStream, Json::Reader& aReader) {
			if(! StateSkipWhitespace(aStream)) return false;
			char c;
			aStream >> c;

			switch(c)
			{
			case ']':
				return aReader.EndArray();
			default:
				aStream.SetOffset(aStream.GetOffset() - 1);
				if(! StateParseValue(aStream, aReader)) return false;
				return StateChildSeperator(aStream, aReader);
			}
		}

		static bool StateChildSeperator(ReadStream& aStream, Json::Reader& aReader) {
			if(! StateSkipWhitespace(aStream)) return false;
			char c;
			aStream >> c;

			switch(c) 
			{
			case ']':
				return aReader.EndArray();
			case ',':
				if(! StateParseValue(aStream, aReader)) return false;
				return StateChildSeperator(aStream, aReader);
			default:
				return false;
			}
		}
	public:
		static bool Parse(ReadStream& aStream, Json::Reader& aReader) {
			const uint32_t offset = aStream.GetOffset();

			if(! StateOpenArray(aStream, aReader)) {
				aStream.SetOffset(offset);
				return false;
			}

			return true;
		}
	};

	class ObjectParser : public BaseParser {
	private:
		static bool StateOpenObject(ReadStream& aStream, Json::Reader& aReader) {
			if(! StateSkipWhitespace(aStream)) return false;
			char c;
			aStream >> c;
			if(c != '{') return false;
			if(! aReader.BeginObject()) return false;
			return StateCloseObject(aStream, aReader);
		}

		static bool StateCloseObject(ReadStream& aStream, Json::Reader& aReader) {
			if(! StateSkipWhitespace(aStream)) return false;
			char c;
			aStream >> c;

			switch(c)
			{
			case '}':
				return aReader.EndObject();
			case '"':
				return StateMemberName(aStream, aReader);
			default:
				return false;
			}
		}

		static bool StateMemberName(ReadStream& aStream, Json::Reader& aReader) {
			String name(DEFAULT_ALLOCATOR);
			char c;
			while(! aStream.End()) {
				aStream >> c;
				if(c == '"') {
					if(! aReader.MemberName(name)) return false;
					return StateNameSeperator(aStream, aReader);
				}else {
					name += c;
				}
			}
			return false;
		}

		static bool StateNameSeperator(ReadStream& aStream, Json::Reader& aReader) {
			if(! StateSkipWhitespace(aStream)) return false;
			char c;
			aStream >> c;
			if(c != ':') return false;
			if(! StateParseValue(aStream, aReader)) return false;
			return StateChildSeperator(aStream, aReader);
		}

		static bool StateChildSeperator(ReadStream& aStream, Json::Reader& aReader) {
			if(! StateSkipWhitespace(aStream)) return false;
			char c;
			aStream >> c;

			switch(c)
			{
			case '}':
				return aReader.EndObject();
			case ',':
				if(! StateSkipWhitespace(aStream)) return false;
				aStream >> c;
				if(c != '"') return false;
				return StateMemberName(aStream, aReader);
			default:
				return false;
			}
		}
	public:
		static bool Parse(ReadStream& aStream, Json::Reader& aReader) {
			const uint32_t offset = aStream.GetOffset();

			if(! StateOpenObject(aStream, aReader)) {
				aStream.SetOffset(offset);
				return false;
			}

			return true;
		}
	};

	bool JsonParseNull(ReadStream& aStream, Json::Reader& aReader) {
		return NullParser::Parse(aStream, aReader);
	}

	bool JsonParseBool(ReadStream& aStream, Json::Reader& aReader) {
		return BoolParser::Parse(aStream, aReader);
	}

	bool JsonParseNumber(ReadStream& aStream, Json::Reader& aReader) {
		return NumberParser::Parse(aStream, aReader);
	}

	bool JsonParseString(ReadStream& aStream, Json::Reader& aReader){
		return StringParser::Parse(aStream, aReader);
	}

	bool JsonParseArray(ReadStream& aStream, Json::Reader& aReader) {
		return ArrayParser::Parse(aStream, aReader);
	}

	bool JsonParseObject(ReadStream& aStream, Json::Reader& aReader) {
		return ObjectParser::Parse(aStream, aReader);
	}

}}
