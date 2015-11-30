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
#include "Solaire\Strings\NumberParser.hpp"
#include "Solaire\Encode\Array.hpp"
#include "Solaire\Encode\Object.hpp"

namespace Solaire{ namespace Encode{

	// ElementData
	
	Xml::Writer::ElementData::ElementData() : 
		name(GetDefaultAllocator()),
		body(GetDefaultAllocator()),
		attributeNames(GetDefaultAllocator()),
		attributeValues(GetDefaultAllocator()),
		children(GetDefaultAllocator())
	{}

	// Writer
	
	Xml::Writer::Writer(WriteStream& aStream) :
		mOutputStream(aStream),
		mHead(GetDefaultAllocator())
	{}

	Xml::Writer::~Writer() {

	}

	bool Xml::Writer::WriteElement(const Xml::Writer::ElementData& aData) throw(){
		char buf;

		buf = '<';
		mOutputStream.Write(&buf, sizeof(char));
		mOutputStream.Write(aData.name.CString(), sizeof(char) * aData.name.Size());

		const uint32_t attributeCount = aData.attributeNames.Size();
		for (uint32_t i = 0; i < attributeCount; ++i) {
			buf = ' ';
			mOutputStream.Write(&buf, sizeof(char));
			mOutputStream.Write(aData.attributeNames[i].CString(), sizeof(char) * aData.attributeNames[i].Size());
			buf = '=';
			mOutputStream.Write(&buf, sizeof(char));
			buf = '"';
			mOutputStream.Write(&buf, sizeof(char));
			mOutputStream.Write(aData.attributeValues[i].CString(), sizeof(char) * aData.attributeValues[i].Size());
			buf = '"';
			mOutputStream.Write(&buf, sizeof(char));
		}

		if (aData.body.Size() == 0 && aData.children.Size() == 0) {
			buf = '/';
			mOutputStream.Write(&buf, sizeof(char));
			buf = '>';
			mOutputStream.Write(&buf, sizeof(char));
		}
		else {
			buf = '>';
			mOutputStream.Write(&buf, sizeof(char));

			if (aData.body.Size() != 0) {
				mOutputStream.Write(aData.body.CString(), sizeof(char) * aData.body.Size());
			}
			else {
				const uint32_t childCount = aData.children.Size();
				for (uint32_t i = 0; i < childCount; ++i) {
					if (!WriteElement(aData.children[i])) return false;
				}
			}

			buf = '<';
			mOutputStream.Write(&buf, sizeof(char));
			buf = '/';
			mOutputStream.Write(&buf, sizeof(char));
			mOutputStream.Write(aData.name.CString(), sizeof(char) * aData.name.Size());
			buf = '>';
			mOutputStream.Write(&buf, sizeof(char));
		}

		return true;
	}

	bool Xml::Writer::BeginElement(const ConstString<char>& aName) {
		if(mHead.IsEmpty()) {
			mRoot.name = aName;
			mHead.PushBack(&mRoot);
			return true;
		}else {
			ElementData& parent = *mHead.Back();
			if(parent.body.Size() != 0) return false;
			ElementData& child = parent.children.PushBack(ElementData());
			child.name = aName;
			mHead.PushBack(&child);
			return true;
		}
	}

	bool Xml::Writer::EndElement() {
		if(mHead.IsEmpty()) return false;
		mHead.PopBack();

		if(mHead.IsEmpty()) {
			if(! WriteElement(mRoot)) return false;
			mOutputStream.Flush();
		}

		return true;
	}

	bool Xml::Writer::SetBody(const ConstString<char>& aValue) {
		if(mHead.IsEmpty()) return false;
		ElementData& element = *mHead.Back();
		if(element.children.Size() != 0) return false;
		element.body = aValue;
		return true;
	}

	bool Xml::Writer::AddAttribute(const ConstString<char>& aName, const ConstString<char>& aValue) {
		if(mHead.IsEmpty()) return false;
		ElementData& element = *mHead.Back();
		element.attributeNames.PushBack(String(GetDefaultAllocator(), aName));
		element.attributeValues.PushBack(String(GetDefaultAllocator(), aValue));
		return true;
	}

	static bool WriteObject(const ConstString<char>& aName, const Value& aValue, Xml::Writer& aWriter) {
		switch (aValue.GetType()) {
		case Value::TYPE_BOOL:
			if(aValue.GetBool()) {
				return aWriter.AddAttribute(aName, ConstCString("true"));
			}else {
				return aWriter.AddAttribute(aName, ConstCString("false"));
			}
		case Value::TYPE_CHAR:
			{
				const char buf = aValue.GetChar();
				return aWriter.AddAttribute(aName, String(GetDefaultAllocator(), &buf, 1));
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
			if(! BeginElement(ConstCString("bool"))) return false;
			if(aValue.GetBool()) {
				if(! SetBody(ConstCString("true"))) return false;
			}else {
				if(! SetBody(ConstCString("false"))) return false;
			}
			if(! EndElement()) return false;
			return true;
		case Value::TYPE_CHAR:
			{
				const char buf = aValue.GetChar();
				if(! BeginElement(ConstCString("char"))) return false;
				if(! SetBody(String(GetDefaultAllocator(), &buf, 1))) return false;
				if(! EndElement()) return false;
				return true;
			}
		case Value::TYPE_INT:
		case Value::TYPE_UINT:
		case Value::TYPE_DOUBLE:
			if(! BeginElement(ConstCString("number"))) return false;
			if(! SetBody(WriteNumber(aValue.GetDouble()))) return false;
			if(! EndElement()) return false;
			return true;
		case Value::TYPE_STRING:
			if(! BeginElement(ConstCString("string"))) return false;
			if(! SetBody(aValue.GetString())) return false;
			if(! EndElement()) return false;
		case Value::TYPE_ARRAY:
			{
				const Array& _array = aValue.GetArray();
				const uint32_t length = _array.Size();

				if(! BeginElement(ConstCString("array"))) return false;
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
				
				if(! BeginElement(ConstCString("object"))) return false;
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
