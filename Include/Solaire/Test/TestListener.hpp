#ifndef SOLAIRE_TEST_TEST_LISTENER_HPP
#define SOLAIRE_TEST_TEST_LISTENER_HPP

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
	\file TestListener.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 14th September 2015
	Last Modified	: 14th September 2015
*/

#include <stdexcept>
#include <string>

namespace Solaire{ namespace Test{

	class Test;

	class TestListener{
	public:
		virtual ~TestListener(){

		}

		virtual void OnTestPass(const Test& aTest, const std::string& aMessage) = 0;
		virtual void OnTestFail(const Test& aTest, const std::string& aMessage) = 0;
		virtual void OnTestError(const Test& aTest, const std::string& aMessage) = 0;
	};
}}


#endif
