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

	#define SOLAIRE_UNITS_TEST_CONVERT_DESCRIPTION(TYPE, inputValue, INPUT_UNIT, outputValue, OUTPUT_UNIT)\
		std::string(TYPE) + " conversion of" + inputValue + " "  + INPUT_UNIT + " to " + outputValue + " " + OUTPUT_UNIT

	#define SOLAIRE_UNITS_TEST_L_CONVERT(ConverterClass, T, INPUT_UNIT, inputValue, OUTPUT_UNIT, outputValue, threshold)\
		const std::string description = SOLAIRE_UNITS_TEST_CONVERT_DESCRIPTION(#T, #inputValue, #INPUT_UNIT, #outputValue, #OUTPUT_UNIT);\
		ConverterClass<T> converter;\
		converter.Set(ConverterClass<T>::unit_t::INPUT_UNIT, inputValue);\
		if (PercentageDifference<T>(outputValue, converter.Get(ConverterClass<T>::unit_t::OUTPUT_UNIT)) > threshold) Fail(description);\
		Pass(description);

	#define SOLAIRE_UNITS_TEST_LP_CONVERT(ConverterClass, T, INPUT_PREFIX, INPUT_UNIT, inputValue, OUTPUT_PREFIX, OUTPUT_UNIT, outputValue, threshold)\
		const std::string inputUnit = std::string(#INPUT_PREFIX) + "/" + #INPUT_UNIT;\
		const std::string outputUnit = std::string(#OUTPUT_PREFIX) + "/" + #OUTPUT_UNIT;\
		const std::string description = SOLAIRE_UNITS_TEST_CONVERT_DESCRIPTION(#T, #inputValue, inputUnit, #outputValue, outputUnit);\
		ConverterClass<T> converter;\
		converter.Set(ConverterClass<T>::prefix_t::INPUT_PREFIX, ConverterClass<T>::unit_t::INPUT_UNIT, inputValue);\
		if (PercentageDifference<T>(outputValue, converter.Get(ConverterClass<T>::prefix_t::OUTPUT_PREFIX, ConverterClass<T>::unit_t::OUTPUT_UNIT)) > threshold) Fail(description);\
		Pass(description);
}}}


#endif