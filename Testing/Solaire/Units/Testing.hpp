#ifndef SOLAIRE_UNITS_TESTING_HPP
#define SOLAIRE_UNITS_TESTING_HPP

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

#include <string>
#include <sstream>
#include <typeinfo>
#include "Solaire\Units\Distance.hpp"

namespace Solaire{ namespace Units{ namespace Testing{

	template<class T>
	static T Difference(T aFirst, T aSecond){
		return aFirst < aSecond ? aSecond - aFirst : aFirst - aSecond;
	}

	template<class T>
	static double PercentageDifference(T aFirst, T aSecond){
		return (100.0 / aFirst)  * Difference<T>(aFirst, aSecond);
	}

	template<class Converter>
	class ConversionTest : public Solaire::Test::Test {
	public:
		typedef typename Converter::prefix_t prefix_t;
		typedef typename Converter::unit_t unit_t;
		typedef typename Converter::conversion_t conversion_t;

		typedef std::tuple<
			std::string,
			prefix_t,
			unit_t,
			conversion_t,
			std::string,
			prefix_t,
			unit_t,
			conversion_t,
			conversion_t
		> ConversionDescription;
	
		enum{
			INPUT_NAME,
			INPUT_PREFIX,
			INPUT_UNIT,
			INPUT_VALUE,
			OUTPUT_NAME,
			OUTPUT_PREFIX,
			OUTPUT_UNIT,
			OUTPUT_VALUE,
			THRESHOLD
		};
	private:
		const std::string mClassName;
		const std::string mTestName;
		const std::vector<ConversionDescription> mConversions;

#define solaire_inputName std::get<INPUT_NAME>(aDescription)
#define solaire_inputPrefix std::get<INPUT_PREFIX>(aDescription)
#define solaire_inputUnit std::get<INPUT_UNIT>(aDescription)
#define solaire_inputValue std::get<INPUT_VALUE>(aDescription)
#define solaire_outputName std::get<OUTPUT_NAME>(aDescription)
#define solaire_outputPrefix std::get<OUTPUT_PREFIX>(aDescription)
#define solaire_outputUnit std::get<OUTPUT_UNIT>(aDescription)
#define solaire_outputValue std::get<OUTPUT_VALUE>(aDescription)
#define solaire_threshold std::get<THRESHOLD>(aDescription)

		std::string GetDescription(const ConversionDescription& aDescription) const{
			std::stringstream desc;
			desc << typeid(conversion_t).name() << " conversion from " << solaire_inputValue << ' ' << solaire_inputName << " to " << solaire_outputValue << ' ' << solaire_outputName;

			return desc.str();
		}

		bool PerformConversion(const ConversionDescription& aDescription) const{
			Converter converter;
			converter.Set(solaire_inputPrefix, solaire_inputUnit, solaire_inputValue);
			const conversion_t realOutputValue = converter.Get(solaire_outputPrefix, solaire_outputUnit);
			return PercentageDifference<conversion_t>(solaire_outputValue, realOutputValue) <= solaire_threshold;
		}

#undef solaire_inputName
#undef solaire_inputPrefix
#undef solaire_inputUnit
#undef solaire_inputValue
#undef solaire_outputName
#undef solaire_outputPrefix
#undef solaire_outputUnit
#undef solaire_outputValue
#undef solaire_threshold

	protected:
		// Inherited from test

		void Run() const override{
			std::string description = "\n";

			for(const ConversionDescription& aDescription : mConversions){
				const std::string desc = GetDescription(aDescription);
				bool result = PerformConversion(aDescription);
				if(! result){
					Fail(desc);
				}else{
					description += desc;
					description += '\n';
				}
			}

			Pass(description);
		}
	public:
		ConversionTest(const std::string aClassName, const std::string aTestName, const std::initializer_list<ConversionDescription> aConversions) :
			mClassName(aClassName),
			mTestName(aTestName),
			mConversions(aConversions)
		{

		}

		// Inherited from test

		std::string GetClassName() const override{
			return mClassName;
		}

		std::string GetTestName() const override{
			return mTestName;
		}
	};

	template<class Converter>
	class MathsTest : public Solaire::Test::Test {
	public:
		typedef typename Converter::unit_t unit_t;
		typedef typename Converter::conversion_t conversion_t;

		typedef std::tuple<std::string, unit_t, conversion_t, conversion_t, conversion_t, double> Test;

		enum {
			UNIT_NAME,
			UNIT,
			FIRST_VALUE,
			SECOND_VALUE,
			RESULT_VALUE,
			THRESHOLD
		};
	private:
		const std::string mClassName;
		const std::string mTestName;
		const Test mAdd;
		const Test mSubtract;
		const Test mMultiply;
		const Test mDivide;

		static std::string GetDescription(const Test& aTest, const std::string aOperation){
			std::stringstream ss;
			const std::string unit = std::get<UNIT_NAME>(aTest);

			ss << typeid(conversion_t).name();
			ss << ' ';
			ss << aOperation;
			ss << " of ";
			ss << std::get<FIRST_VALUE>(aTest);
			ss << ' ';
			ss << unit;
			ss << " and ";
			ss << std::get<SECOND_VALUE>(aTest);
			ss << ' ';
			ss << unit;
			ss << " for result of ";
			ss << std::get<RESULT_VALUE>(aTest);
			ss << ' ';
			ss << unit;

			return ss.str();
		}

		static bool Compare(const conversion_t aFirst, const conversion_t aSecond, double aThreshold){
			return PercentageDifference<conversion_t>(aFirst, aSecond) <= aThreshold;
		}

		static bool TestAdd(const Test& aTest, std::string& aMessage){
			const unit_t unit = std::get<UNIT>(aTest);

			const Converter a(unit, std::get<FIRST_VALUE>(aTest));
			const Converter b(unit, std::get<SECOND_VALUE>(aTest));

			Converter c = a;
			c += b;
			const Converter d = a + b;

			const conversion_t result = c.Get(unit);
			const double threshold = std::get<THRESHOLD>(aTest);
			aMessage = GetDescription(aTest, "addition");
			return Compare(result, d.Get(unit), threshold) && Compare(result, std::get<RESULT_VALUE>(aTest), threshold);
		}

		static bool TestSubtract(const Test& aTest, std::string& aMessage){
			const unit_t unit = std::get<UNIT>(aTest);

			const Converter a(unit, std::get<FIRST_VALUE>(aTest));
			const Converter b(unit, std::get<SECOND_VALUE>(aTest));

			Converter c = a;
			c -= b;
			const Converter d = a - b;

			const conversion_t result = c.Get(unit);
			const double threshold = std::get<THRESHOLD>(aTest);
			aMessage = GetDescription(aTest, "subtraction");
			return Compare(result, d.Get(unit), threshold) && Compare(result, std::get<RESULT_VALUE>(aTest), threshold);
		}

		static bool TestMultiply(const Test& aTest, std::string& aMessage){
			const unit_t unit = std::get<UNIT>(aTest);

			const Converter a(unit, std::get<FIRST_VALUE>(aTest));
			const Converter b(unit, std::get<SECOND_VALUE>(aTest));

			Converter c = a;
			c *= b;
			const Converter d = a * b;

			const conversion_t result = c.Get(unit);
			const double threshold = std::get<THRESHOLD>(aTest);
			aMessage = GetDescription(aTest, "multiplication");
			return Compare(result, d.Get(unit), threshold) && Compare(result, std::get<RESULT_VALUE>(aTest), threshold);
		}

		static bool TestDivide(const Test& aTest, std::string& aMessage){
			const unit_t unit = std::get<UNIT>(aTest);

			const Converter a(unit, std::get<FIRST_VALUE>(aTest));
			const Converter b(unit, std::get<SECOND_VALUE>(aTest));

			Converter c = a;
			c /= b;
			const Converter d = a / b;

			const conversion_t result = c.Get(unit);
			const double threshold = std::get<THRESHOLD>(aTest);
			aMessage = GetDescription(aTest, "division");
			return Compare(result, d.Get(unit), threshold) && Compare(result, std::get<RESULT_VALUE>(aTest), threshold);
		}

		void RunTest(std::string& aMessage, bool(*aFn)(const Test& aTest, std::string& aMessage), const Test& aTest) const{
			std::string tmp;

			if(aFn(aTest, tmp)){
				aMessage += tmp;
				aMessage += '\n';
			}else{
				Fail(tmp);
			}
		}

	protected:
		// Inherited from test

		void Run() const override{
			std::string description = "\n";
			
			RunTest(description, TestAdd, mAdd);
			RunTest(description, TestSubtract, mSubtract);
			RunTest(description, TestMultiply, mMultiply);
			RunTest(description, TestDivide, mDivide);

			Pass(description);
		}
	public:
		MathsTest(const std::string aClassName, const std::string aTestName, const Test aAdd, const Test aSubtract, const Test aMultiply, const Test aDivide) :
			mClassName(aClassName),
			mTestName(aTestName),
			mAdd(aAdd),
			mSubtract(aSubtract),
			mMultiply(aMultiply),
			mDivide(aDivide)
		{

		}

		// Inherited from test

		std::string GetClassName() const override{
			return mClassName;
		}

		std::string GetTestName() const override{
			return mTestName;
		}
	};
}}}


#endif