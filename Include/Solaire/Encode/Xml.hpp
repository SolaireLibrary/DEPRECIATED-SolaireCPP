#ifndef SOLAIRE_ENCODE_XML_HPP
#define SOLAIRE_ENCODE_XML_HPP

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
\file Xml.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 23rd November 2015
Last Modified	: 23rd November 2015
*/

#include "..\Core\Init.hpp"
#include "..\Strings\StringFragment.hpp"
#include "..\IO\Stream.hpp"
#include "Value.hpp"

namespace Solaire { namespace Encode {

	struct Xml {
		class SOLAIRE_EXPORT_API Reader {
			virtual bool SOLAIRE_EXPORT_CALL BeginElement(const ConstStringFragment) const = 0;
			virtual bool SOLAIRE_EXPORT_CALL EndElement() const = 0;

			virtual bool SOLAIRE_EXPORT_CALL ElementBody(const ConstStringFragment) const = 0;

			virtual bool SOLAIRE_EXPORT_CALL AttributeName(const ConstStringFragment) const = 0;
			virtual bool SOLAIRE_EXPORT_CALL AttributeValue(const ConstStringFragment) const = 0;

		};

		class Writer {
		private:
			WriteStream& mOutputStream;
		public:
			Writer(WriteStream&);
			~Writer();

			bool BeginElement(const ConstStringFragment);
			bool EndElement();

			bool SetBody(const ConstStringFragment);
			bool AddAttribute(const ConstStringFragment, const ConstStringFragment);

		};

		static bool SOLAIRE_EXPORT_CALL Write(const Value&, Writer&);
		static bool SOLAIRE_EXPORT_CALL Read(WriteStream&, Reader&);
		static Value SOLAIRE_EXPORT_CALL Read(Allocator&, ReadStream&);
		static Value SOLAIRE_EXPORT_CALL Read(ReadStream&);
	};

}}

#endif
