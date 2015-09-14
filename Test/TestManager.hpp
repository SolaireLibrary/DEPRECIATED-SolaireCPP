#ifndef SOLAIRE_TEST_PRINT_TEST_MANAGER_HPP
#define SOLAIRE_TEST_PRINT_TEST_MANAGER_HPP

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
\file TestManager.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 14th September 2015
Last Modified	: 14th September 2015
*/

#include <map>
#include <memory>
#include "Test.hpp"

namespace Solaire { namespace Test {
	
	class TestManager{
	private:
		static void ParseTestName(const std::string& aName, std::string& aClass, std::string& aTest){
			const char* begin = std::strstr(aName.c_str(), "::");
			if(begin == nullptr) throw InvalidTestName();

			const size_t pos = begin - aName.c_str();
			aClass = aName.substr(0, pos);
			aTest = aName.substr(pos + 2, aName.size() - (pos + 2));
		}

		std::map<std::string, std::map<std::string, std::shared_ptr<Test>>> mTests;
		TestListener* mListener;

		const std::map<std::string, std::shared_ptr<Test>>& FindTests(const std::string& aClassName) const{
			auto it = mTests.find(aClassName);
			if (it == mTests.end()) throw ClassNotFound();
			return it->second;
		}

		const Test& FindTest(const std::string& aClassName, const std::string& aTestName) const{
			const auto& tests = FindTests(aClassName);
			auto it = tests.find(aTestName);
			if (it == tests.end()) throw TestNotFound();
			return *(it->second);
		}

		void ForEachClass(std::function<void(const std::string&)> aCallback) const{
			for(auto& i : mTests){
				aCallback(i.first);
			}
		}

		void ForEachTest(std::function<void(Test&)> aCallback) const{
			for(auto& i : mTests){
				for(auto& j : i.second){
					aCallback(*j.second);
				}
			}
		}

		void ForEachTest(const std::string& aClassName, std::function<void(Test&)> aCallback) const {
			auto& tests = FindTests(aClassName);
			for(auto& i : tests){
				aCallback(*i.second);
			}
		}

	public:
		SOLAIRE_EXCEPTION(ClassNotFound, "Could not find named class");
		SOLAIRE_EXCEPTION(TestNotFound, "Could not find named Test in class");
		SOLAIRE_EXCEPTION(InvalidTestName, "Test name was invalid");
		SOLAIRE_EXCEPTION(TestNameConflict, "A test with this name is already a member of named class");

		TestManager() :
			mListener(nullptr)
		{

		}

		TestManager(TestListener& aListener) :
			mListener(&aListener)
		{

		}

		void SetListener(TestListener& aListener){
			mListener = &aListener;

			ForEachTest([&](Test& aTest){
				aTest.Listener = mListener;
			});
		}

		void Run(const std::string& aClassName, const std::string& aTestName) const{
			FindTest(aClassName, aTestName)();
		}

		void Run(const std::string& aName) const{
			std::string className;
			std::string testName;
			ParseTestName(aName, className, testName);
			Run(className, testName);
		}

		void RunAll(const std::string& aClassName) const{
			ForEachTest(aClassName, [](Test& aTest){
				aTest();
			});
		}

		void RunAll() const {
			ForEachClass([this](const std::string& aName) {
				RunAll(aName);
			});
		}

		void Add(std::shared_ptr<Test> aTest){
			const std::string className = aTest->GetClassName();
			const std::string testName = aTest->GetTestName();

			auto i = mTests.find(className);
			if(i == mTests.end()){
				i = mTests.emplace(className, std::map<std::string, std::shared_ptr<Test>>()).first;
			}

			auto j = i->second.find(testName);
			if(j != i->second.end()) throw TestNameConflict();

			aTest->Listener = mListener;
			i->second.emplace(testName, aTest);
		}
	};
}}


#endif
