#ifndef SOLAIRE_UNITS_MASS_TEST_HPP
#define SOLAIRE_UNITS_MASS_TEST_HPP

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

#include "..\..\..\Test\TestManager.hpp"
#include "..\Mass.hpp"
#include "Testing.hpp"

namespace Solaire{ namespace Units { namespace Testing{

	SOLAIRE_TEST(Mass, StaticConvert,
		// Test that the unit is converted correctly
		double threshold = 1.0;
		double expectedResult = 0.0;
		double actualResult = 0.0;

		expectedResult = 64;
		actualResult = MassD::Convert(MassD::unit_t::POUND, MassD::unit_t::OUNCE, 4);
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to static convert POUND to OUNCE using double values");

		Pass("");
	);

	std::shared_ptr<Test::Test> MassDefaultConstructor(){
		return Test::BuildTest(
			"Mass",
			"DefaultConstructor",
			[](std::string& aMessage)->bool{
				// Create converter
				Mass<double> converter;

				// Check that intial value is 0
				if(converter.Pounds != 0.0){
					aMessage = "Inital value is not 0";
					return false;
				}else{
					aMessage = "";
					return true;
				}
			}
		);
	}

	std::shared_ptr<Test::Test> MassUnitConstructor(){
		return Test::BuildTest(
			"Mass",
			"UnitConstructor",
			[](std::string& aMessage)->bool{
				// Create converter
				Mass<double> converter(MassD::unit_t::POUND, 5);

				// Check that intial value is 0
				if(converter.Pounds != 5){
					aMessage = "Returned value is different than given in constructor";
					return false;
				}else{
					aMessage = "";
					return true;
				}
			}
		);
	}

	std::shared_ptr<Test::Test> MassCopyConstructor(){
		return Test::BuildTest(
			"Mass",
			"CopyConstructor",
			[](std::string& aMessage)->bool{
				// Create converter
				Mass<double> converterA(MassD::unit_t::POUND, 5);

				// Copy converter
				Mass<double> converterB = converterA;

				// Use converterA
				converterA.Pounds = 10;

				// Check that intial value is 0
				if(converterA.Pounds == converterB.Pounds || converterB.Pounds != 5){
					aMessage = "Value was not coppied correctly";
					return false;
				}else{
					aMessage = "";
					return true;
				}
			}
		);
	}

	template<typename T>
	static std::shared_ptr<Test::Test> MassLConvertTest(){
		typedef ConversionTest<Mass<T>> ConversionTest;
		typedef ConversionTest::ConversionDescription Conversion;
		typedef Mass<T>::prefix_t prefix_t;
		typedef Mass<T>::unit_t unit_t;

		return std::shared_ptr<Test::Test>(new ConversionTest("Mass", std::string("Conversions") + typeid(T).name(), {
			// Self conversion
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"pounds",		prefix_t::NONE,	unit_t::POUND,		4,			1),
			// Small units
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"grams",		prefix_t::NONE,	unit_t::GRAM,		1814.37,	1),
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"ounces",		prefix_t::NONE,	unit_t::OUNCE,		64,			1),
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"stones",		prefix_t::NONE,	unit_t::STONE,		0.285714,	1),
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"metric tons",	prefix_t::NONE,	unit_t::METRIC_TON,	0.00181437,	1),
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"short tons",	prefix_t::NONE,	unit_t::SHORT_TON,	0.002,		1),
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"short tons",	prefix_t::NONE,	unit_t::LONG_TON,	0.00178571,	1),
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"troy ounces",	prefix_t::NONE,	unit_t::TROY_OUNCE,	58.3333333,	1),
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"troy pounds",	prefix_t::NONE,	unit_t::TROY_POUND,	4.8612,		1),
			// Large units
			Conversion("kilograms",	prefix_t::KILO,	unit_t::GRAM,	2,			"ounces",		prefix_t::NONE,	unit_t::OUNCE,		70.5479,	1),
			Conversion("ounces",	prefix_t::NONE,	unit_t::OUNCE,	70.5479,	"kilograms",	prefix_t::KILO,	unit_t::GRAM,		2,			1),
			Conversion("kilograms",	prefix_t::KILO,	unit_t::GRAM,	1.989e+30,	"solar masses",	prefix_t::NONE,	unit_t::SOLAR_MASS,	1,			1)
		}));
	}

	template<typename T>
	static std::shared_ptr<Test::Test> MassMathsTest() {
		typedef MathsTest<Mass<T>> MathsTest;
		typedef MathsTest::Test Test;
		typedef Mass<T>::prefix_t prefix_t;
		typedef Mass<T>::unit_t unit_t;

		return std::shared_ptr<Solaire::Test::Test>(new MathsTest(
			"Mass", 
			std::string("Maths") + typeid(T).name(),
			Test("pounds",	unit_t::POUND,	5,	10,	15,	1),
			Test("pounds",	unit_t::POUND,	15,	10,	5,	1),
			Test("pounds",	unit_t::POUND,	5,	2,	10,	1),
			Test("pounds",	unit_t::POUND,	10,	2,	5,	1)
		));
	}

	static void MassTests(Test::TestManager& aManager){
		aManager.Add(MassLConvertTest<double>());
		aManager.Add(MassLConvertTest<float>());
		aManager.Add(MassLConvertTest<int>());

		aManager.Add(MassDefaultConstructor());
		aManager.Add(MassUnitConstructor());
		aManager.Add(MassCopyConstructor());

		aManager.Add(MassMathsTest<double>());
		aManager.Add(MassMathsTest<float>());
		aManager.Add(MassMathsTest<int>());

		aManager.Add(std::shared_ptr<Test::Test>(new TestMassStaticConvert()));
	}
}}}


#endif