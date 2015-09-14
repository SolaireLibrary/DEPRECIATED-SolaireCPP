#ifndef SOLAIRE_TEST_TEST_HPP
#define SOLAIRE_TEST_TEST_HPP

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
	\file Test.hpp
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
#include "../Core/Macros.hpp"
#include "TestListener.hpp"

namespace Solaire{ namespace Test{

	class Test{
	private:
		SOLAIRE_EXCEPTION(InternalException, "This should not be seen");
	public:
		TestListener* Listener;

		SOLAIRE_EXCEPTION(ListenerNotSetException, "A Test tried to run, but had a null TestListener");
	protected:
		virtual void Run() const = 0;

		void Pass(const std::string& aMessage) {
			if(Listener == nullptr) throw ListenerNotSetException();
			Listener->OnTestPass(*this, aMessage);
			throw InternalException();
		}

		void Fail(const std::string& aMessage) {
			if(Listener == nullptr) throw ListenerNotSetException();
			Listener->OnTestPass(*this, aMessage);
			throw InternalException();
		}
	public:
		Test() :
			Listener(nullptr)
		{

		}

		Test(TestListener& aListener) :
			Listener(&aListener)
		{

		}

		virtual ~Test(){

		}

		virtual std::string GetClassName() const = 0;
		virtual std::string GetTestName() const = 0;

		void operator()() const{
			try{
				Run();
			}catch(InternalException&){
				return;
			}catch(std::exception& e){
				Listener->OnTestError(*this, e.what());
			}catch (...) {
				Listener->OnTestError(*this, "Non-exception was thrown");
			}
		}
	};
}}


#endif
