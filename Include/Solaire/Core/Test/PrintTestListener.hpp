#ifndef SOLAIRE_TEST_PRINT_TEST_LISTENER_HPP
#define SOLAIRE_TEST_PRINT_TEST_LISTENER_HPP

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
\file PrintTestListener.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.1
\date
Created			: 14th September 2015
Last Modified	: 26th October 2015
*/

#include <ostream>
#include "TestListener.hpp"
#include "TestBase.hpp"

namespace Solaire { namespace Test {

	class PrintTestListener : public TestListener{
	private:
		void Print(const Test& aTest, const ConstStringFragment aMessage, const std::string& aResult){
			if (Stream == nullptr) throw std::runtime_error("PrintTestListener recieved a Test result before Stream was set");
			std::ostream& stream = *Stream;
			stream << aTest.GetClassName() << "::" << aTest.GetTestName() << " : " << aResult << " (" << aMessage << ")" << std::endl;
		}
	public:
		std::ostream* Stream;

		PrintTestListener() :
			Stream(nullptr)
		{

		}

		PrintTestListener(std::ostream& aStream) :
			Stream(&aStream)
		{

		}

		// Inherited from TestListener

		void OnTestPass(const Test& aTest, const ConstStringFragment aMessage) override{
			Print(aTest, aMessage, "PASS");
		}

		void OnTestFail(const Test& aTest, const ConstStringFragment aMessage) override{
			Print(aTest, aMessage, "FAIL");
		}

		void OnTestError(const Test& aTest, const ConstStringFragment aMessage) override{
			Print(aTest, aMessage, "ERROR");
		}
	};
}}


#endif
