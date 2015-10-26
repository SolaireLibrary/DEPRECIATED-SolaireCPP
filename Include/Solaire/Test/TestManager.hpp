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
\version 1.1
\date
Created			: 14th September 2015
Last Modified	: 26th October 2015
*/

#include <map>
#include <memory>
#include "TestBase.hpp"

namespace Solaire { namespace Test {

    class TestManager;

    template<class T>
    struct TestSource{
        static void AddTests(TestManager& aManager){

        }
    };

	class TestManager{
	private:
		static void ParseTestName(const ConstStringFragment aName, String& aClass, String& aTest){
			const char* begin = aName.FindFirst("::").begin();
			if(begin == nullptr) throw std::runtime_error("TestManager : Test name was invalid");

			const size_t pos = begin - aName.begin();
			aClass = ConstStringFragment(aName.begin(), pos);
			aTest = ConstStringFragment(aName.begin() + pos + 2, aName.Size() - (pos + 2));
		}

		std::map<String, std::map<String, std::shared_ptr<Test>>> mTests;
		TestListener* mListener;

		const std::map<String, std::shared_ptr<Test>>& FindTests(const String& aClassName) const{
			auto it = mTests.find(aClassName);
			if (it == mTests.end()) throw std::runtime_error("TestManager : Could not find named class");
			return it->second;
		}

		const Test& FindTest(const String& aClassName, const String& aTestName) const{
			const auto& tests = FindTests(aClassName);
			auto it = tests.find(aTestName);
			if (it == tests.end()) throw std::runtime_error("TestManager : Could not find named Test in class");
			return *(it->second);
		}

		void ForEachClass(std::function<void(const String&)> aCallback) const{
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

		void ForEachTest(const ConstStringFragment aClassName, std::function<void(Test&)> aCallback) const {
			auto& tests = FindTests(String(GetDefaultAllocator(), aClassName));
			for(auto& i : tests){
				aCallback(*i.second);
			}
		}

	public:
		TestManager() :
			mListener(nullptr)
		{}

		TestManager(TestListener& aListener) :
			mListener(&aListener)
		{}

		void SetListener(TestListener& aListener){
			mListener = &aListener;

			ForEachTest([&](Test& aTest){
				aTest.Listener = mListener;
			});
		}

		void Run(const ConstStringFragment aClassName, const ConstStringFragment aTestName) const{
			FindTest(
                String(GetDefaultAllocator(), aClassName),
                String(GetDefaultAllocator(), aTestName)
            )();
		}

		void Run(const ConstStringFragment aName) const{
			String className(GetDefaultAllocator());
			String testName(GetDefaultAllocator());
			ParseTestName(aName, className, testName);
			Run(className, testName);
		}

		template<class T>
		void RunAllForClass(){
            RunAllForClass(GetClassName<T>());
		}

		void RunAllForClass(const ConstStringFragment aClassName) const{
			ForEachTest(aClassName, [](Test& aTest){
				aTest();
			});
		}

		void RunAll() const {
			ForEachClass([this](ConstStringFragment aName) {
				RunAllForClass(aName);
			});
		}

		void Add(std::shared_ptr<Test> aTest){
			const String className(GetDefaultAllocator(), aTest->GetClassName());
			const String testName(GetDefaultAllocator(), aTest->GetTestName());

			auto i = mTests.find(className);
			if(i == mTests.end()){
				i = mTests.emplace(className, std::map<String, std::shared_ptr<Test>>()).first;
			}

			auto j = i->second.find(testName);
			if(j != i->second.end()) throw std::runtime_error("TestManager : Test name conflict");

			aTest->Listener = mListener;
			i->second.emplace(testName, aTest);
		}

        template<class T>
		void AddTests(){
			TestSource<T>::AddTests(*this);
		}
	};
}}


#endif
