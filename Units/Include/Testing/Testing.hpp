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
#include "..\Distance.hpp"

namespace Solaire{ namespace Units{ namespace Testing{

	template<class T>
	static T Difference(T aFirst, T aSecond){
		return aFirst < aSecond ? aSecond - aFirst : aFirst - aSecond;
	}

	template<class T>
	static double PercentageDifference(T aFirst, T aSecond){
		return (100.0 / aFirst)  * Difference<T>(aFirst, aSecond);
	}

	template<class T, class Converter>
	class ConversionTest : public Solaire::Test::Test {
	public:
		typedef std::tuple<
			std::string,
			typename Converter::prefix_t, 
			typename Converter::unit_t, 
			T,
			std::string,
			typename Converter::prefix_t, 
			typename Converter::unit_t, 
			T,
			T
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
			desc << typeid(T).name() << " conversion from " << solaire_inputValue << ' ' << solaire_inputName << " to " << solaire_outputValue << ' ' << solaire_outputName;

			return desc.str();
		}

		bool PerformConversion(const ConversionDescription& aDescription) const{
			Converter converter;
			converter.Set(solaire_inputPrefix, solaire_inputUnit, solaire_inputValue);
			const T realOutputValue = converter.Get(solaire_outputPrefix, solaire_outputUnit);
			return PercentageDifference<T>(solaire_outputValue, realOutputValue) <= solaire_threshold;
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
}}}


#endif