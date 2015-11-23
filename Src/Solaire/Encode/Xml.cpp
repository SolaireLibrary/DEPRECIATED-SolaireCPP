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

#include "Solaire\Encode\Xml.hpp"
#include "Solaire\Memory\DefaultAllocator.hpp"
#include "Solaire\Strings\NumberParser.hpp"
#include "Solaire\Encode\Array.hpp"
#include "Solaire\Encode\Object.hpp"

namespace Solaire{ namespace Encode{

	// Writer

	static bool WriteObject(const ConstStringFragment aName, const Value& aValue, Xml::Writer& aWriter) {
		switch (aValue.GetType()) {
		case Value::TYPE_BOOL:
			if(aValue.GetBool()) {
				return aWriter.AddAttribute(aName, "true");
			}else {
				return aWriter.AddAttribute(aName, "false");
			}
		case Value::TYPE_CHAR:
			{
				const char buf = aValue.GetChar();
				return aWriter.AddAttribute(aName, String(DEFAULT_ALLOCATOR, &buf, 1));
			}
		case Value::TYPE_INT:
		case Value::TYPE_UINT:
		case Value::TYPE_DOUBLE:
			return aWriter.AddAttribute(aName, WriteNumber(aValue.GetDouble()));
		case Value::TYPE_STRING:
			return aWriter.AddAttribute(aName, aValue.GetString());
		case Value::TYPE_ARRAY:
			{
				const Array& _array = aValue.GetArray();
				const uint32_t length = _array.Size();

				if(! aWriter.BeginElement(aName)) return false;
				for(uint32_t i = 0; i < length; ++i) {
					if(! aWriter.Write(_array[i])) return false;
				}
				if(! aWriter.EndElement()) return false;

				return true;
			}
		case Value::TYPE_OBJECT:
			{
				const Object& object = aValue.GetObject();
				const uint32_t length = object.Size();
				
				if(! aWriter.BeginElement(aName)) return false;
				for(uint32_t i = 0; i < length; ++i) {
					if(! WriteObject(object.GetMemberName(i), object[i], aWriter)) return false;
				}
				if(! aWriter.EndElement()) return false;

				return true;
			}
		default:
			return false;
		}
	}
	
	bool Xml::Writer::Write(const Value& aValue) {
		switch (aValue.GetType()) {
		case Value::TYPE_BOOL:
			if(! BeginElement("bool")) return false;
			if(aValue.GetBool()) {
				if(! SetBody("true")) return false;
			}else {
				if(! SetBody("false")) return false;
			}
			if(! EndElement()) return false;
			return true;
		case Value::TYPE_CHAR:
			{
				const char buf = aValue.GetChar();
				if(! BeginElement("char")) return false;
				if(! SetBody(String(DEFAULT_ALLOCATOR, &buf, 1))) return false;
				if(! EndElement()) return false;
				return true;
			}
		case Value::TYPE_INT:
		case Value::TYPE_UINT:
		case Value::TYPE_DOUBLE:
			if(! BeginElement("number")) return false;
			if(! SetBody(WriteNumber(aValue.GetDouble()))) return false;
			if(! EndElement()) return false;
			return true;
		case Value::TYPE_STRING:
			if(! BeginElement("string")) return false;
			if(! SetBody(aValue.GetString())) return false;
			if(! EndElement()) return false;
		case Value::TYPE_ARRAY:
			{
				const Array& _array = aValue.GetArray();
				const uint32_t length = _array.Size();

				if(! BeginElement("array")) return false;
				for(uint32_t i = 0; i < length; ++i) {
					if(! Write(_array[i])) return false;
				}
				if(! EndElement()) return false;

				return true;
			}
		case Value::TYPE_OBJECT:
			{
				const Object& object = aValue.GetObject();
				const uint32_t length = object.Size();
				
				if(! BeginElement("object")) return false;
				for(uint32_t i = 0; i < length; ++i) {
					if(! WriteObject(object.GetMemberName(i), object[i], *this)) return false;
				}
				if(! EndElement()) return false;

				return true;
			}
		default:
			return false;
		}
	}

}}
