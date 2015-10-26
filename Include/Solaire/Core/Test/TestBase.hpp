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
	\file TestBase.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.1
	\date
	Created			: 14th September 2015
	Last Modified	: 26th October 2015
*/

#include <typeinfo>
#include <stdexcept>
#include "../Strings/String.hpp"
#include "TestListener.hpp"

namespace Solaire{ namespace Test{

	class Test{
	public:
		TestListener* Listener;
	protected:
		virtual void Run() const = 0;

		void Pass(const ConstStringFragment aMessage) const{
			if(Listener == nullptr) throw std::runtime_error("Test : Listener was not set");
			Listener->OnTestPass(*this, aMessage);
		}

		void Fail(const ConstStringFragment aMessage) const{
			if(Listener == nullptr) throw std::runtime_error("Test : Listener was not set");
			Listener->OnTestFail(*this, aMessage);
		}
	public:
		Test() :
			Listener(nullptr)
		{}

		Test(TestListener& aListener) :
			Listener(&aListener)
		{}

		virtual ~Test(){

		}

		virtual ConstStringFragment GetClassName() const = 0;
		virtual ConstStringFragment GetTestName() const = 0;

		void operator()() const{
			try{
				Run();
			}catch(std::exception& e){
			    String tmp(GetDefaultAllocator(), e.what());
				Listener->OnTestError(*this, tmp);
			}catch (...) {
			    String tmp(GetDefaultAllocator(), "Non-exception was thrown");
				Listener->OnTestError(*this, tmp);
			}
		}
	};

	static std::shared_ptr<Test> BuildTest(const ConstStringFragment aClassName, const ConstStringFragment aTestName, std::function<bool(String&)> aFunction){
		class InternalTest : public Test{
		private:
			const String mClass;
			const String mTest;
			const std::function<bool(String&)> mFunction;
		protected:

			// Inherited from Test

			void Run() const override{
				String message(GetDefaultAllocator());
				if(mFunction(message)){
					Pass(message);
				}else{
					Fail(message);
				}
			}
		public:
			InternalTest(const ConstStringFragment aClassName, ConstStringFragment aTestName, std::function<bool(String&)> aFunction) :
				mClass(GetDefaultAllocator(), aClassName),
				mTest(GetDefaultAllocator(), aTestName),
				mFunction(aFunction)
			{

			}

			// Inherited from Test

			ConstStringFragment GetClassName() const override{
				return mClass;
			}

			ConstStringFragment GetTestName() const override{
				return mTest;
			}
		};

		return std::shared_ptr<Test>(new InternalTest(aClassName, aTestName, aFunction));
	}

	template<class T>
	static ConstStringFragment GetClassName(){
	    return ConstStringFragment(typeid(T).name());
	}

    template<class T>
    static std::shared_ptr<Test> BuildTest(const ConstStringFragment aTestName, std::function<bool(String&)> aFunction){
		return BuildTest(GetClassName<T>(), aTestName, aFunction);
	}
}}


#endif
