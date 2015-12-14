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

#include "Solaire\Encode\Array.hpp"
#include "Solaire\Encode\Object.hpp"
#include "Solaire\Encode\Binary.hpp"

namespace Solaire{ namespace Encode{

	// Binary

	bool SOLAIRE_EXPORT_CALL Binary::Write(const Value& aValue, OStream& aStream) {

		const Value::Type type = aValue.GetType();
		aStream.Write(&type, sizeof(Value::Type));

		switch(type){
		case Value::TYPE_BOOL: 
			{
				const bool value = aValue.GetBool();
				aStream.Write(&aValue, sizeof(bool));
			}
			break;
		case Value::TYPE_CHAR:
			{
				const char value = aValue.GetChar();
				aStream.Write(&aValue, sizeof(char));
			}
			break;
		case Value::TYPE_UINT:
			{
				const uint64_t value = aValue.GetUint();
				aStream.Write(&aValue, sizeof(uint64_t));
			}
			break;
		case Value::TYPE_INT:
			{
				const int64_t value = aValue.GetInt();
				aStream.Write(&aValue, sizeof(int64_t));
			}
			break;
		case Value::TYPE_DOUBLE:
			{
				const double value = aValue.GetDouble();
				aStream.Write(&aValue, sizeof(double));
			}
			break;
		case Value::TYPE_STRING:
			{
				const ConstString<char>& string = aValue.GetString();
				const uint16_t length = string.Size();

				aStream << length;
				if(string.IsContiguous()) {
					aStream.Write(string.GetContiguousPtr(), sizeof(char) * length);
				}else {
					for(uint16_t i = 0; i < length; ++i) {
						aStream << string[i];
					}
				}
			}
			break;
		case Value::TYPE_ARRAY:
			{
				const Array& _array = aValue.GetArray();
				const uint16_t length = _array.Size();

				aStream.Write(&length, sizeof(uint16_t));
				for(uint16_t i = 0; i < length; ++i) {
					const Value& value = _array[i];
					if(! Write(value, aStream)) return false;
				}
			}
			break;
		case Value::TYPE_OBJECT:
			{
				const Object& object = aValue.GetObject();
				const uint16_t length = object.Size();

				aStream.Write(&length, sizeof(uint16_t));
				for(uint16_t i = 0; i < length; ++i) {
					const ConstString<char>& name = object.GetMemberName(i);
					const uint16_t nameLength = name.Size();

					aStream << nameLength;
					if(name.IsContiguous()) {
						aStream.Write(name.GetContiguousPtr(), sizeof(char) * nameLength);
					}else {
						for(uint16_t i = 0; i < nameLength; ++i) {
							aStream << name[i];
						}
					}

					const Value& value = object[i];
					if(! Write(value, aStream)) return false;
				}
			}
			break;
		}

		return true;
	}

	Value SOLAIRE_EXPORT_CALL Binary::Read(Allocator& aAllocator, IStream& aStream) {
		Value value(aAllocator);

		Value::Type type;
		aStream.Read(&type, sizeof(Value::Type));

		switch(type){
		case Value::TYPE_BOOL: 
			{
				bool val;
				aStream.Read(&val, sizeof(bool));
			}
			break;
		case Value::TYPE_CHAR:
			{
				char val;
				aStream.Read(&val, sizeof(char));
				value.SetChar(val);
			}
			break;
		case Value::TYPE_UINT:
			{
				uint64_t val;
				aStream.Read(&val, sizeof(uint64_t));
				value.SetUint(val);
			}
			break;
		case Value::TYPE_INT:
			{
				int64_t val;
				aStream.Read(&val, sizeof(int64_t));
				value.SetInt(val);
			}
			break;
		case Value::TYPE_DOUBLE:
			{
				double val;
				aStream.Read(&val, sizeof(double));
				value.SetDouble(val);
			}
			break;
		case Value::TYPE_STRING:
			{
				uint16_t length;
				aStream.Read(&length, sizeof(uint16_t));

				char* buf = static_cast<char*>(aAllocator.Allocate(length + 1));
				aStream.Read(buf, sizeof(char) * length);
				buf[length = '\0'];

				value.SetString() = ConstCString(buf);

				aAllocator.Deallocate(buf);
			}
			break;
		case Value::TYPE_ARRAY:
			{
				Array& _array = value.SetArray();
				uint16_t length;
				aStream.Read(&length, sizeof(uint16_t));

				for(uint16_t i = 0; i < length; ++i) {
					_array.Add(Read(aAllocator, aStream));
				}
			}
			break;
		case Value::TYPE_OBJECT:
			{
				Object& object = value.SetObject();
				uint16_t length;
				aStream.Read(&length, sizeof(uint16_t));

				for(uint16_t i = 0; i < length; ++i) {
					uint16_t nameLength;
					aStream.Read(&nameLength, sizeof(uint16_t));

					char* buf = static_cast<char*>(aAllocator.Allocate(nameLength + 1));
					aStream.Read(buf, sizeof(char) * nameLength);
					buf[nameLength = '\0'];

					object.Add(CString(aAllocator, buf), Read(aStream));

					aAllocator.Deallocate(buf);
				}
			}
			break;
		}

		return value;
	}

	Value SOLAIRE_EXPORT_CALL Binary::Read(IStream& aStream) {
		return Read(GetDefaultAllocator(), aStream);
	}


}}
