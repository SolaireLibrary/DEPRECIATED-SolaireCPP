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

	SOLAIRE_TEST(Mass, SelfConvert,
		// Create converter
		Mass<double> converter;

		// Initialise with a value
		converter.Pounds = 100.0;

		// Check that the same value is returned
		if(converter.Pounds != 100.0){
			Fail("Failed to convert POUND to POUND");
		}else{
			Pass("");
		}
	);

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

	SOLAIRE_TEST(Mass, Integer,
		// Test that the unit is converted correctly
		double threshold = 1.0;
		double expectedResult = 0.0;
		double actualResult = 0.0;

		expectedResult = 64;
		actualResult = MassD::Convert(MassD::unit_t::POUND, MassD::unit_t::OUNCE, 4);
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to static convert POUND to OUNCE using integer values");

		Pass("");
	);

	SOLAIRE_TEST(Mass, DefaultConstructor,
		// Create converter
		Mass<double> converter;

		// Check that intial value is 0
		if(converter.Pounds != 0.0){
			Fail("Inital value is not 0");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Mass, UnitConstructor,
		// Create converter
		Mass<double> converter(MassD::unit_t::POUND, 5);

		// Check that intial value is 0
		if(converter.Pounds != 5){
			Fail("Returned value is different than given in constructor");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Mass, CopyConstructor,
		// Create converter
		Mass<double> converterA(MassD::unit_t::POUND, 5);

		// Copy converter
		Mass<double> converterB = converterA;

		// Use converterA
		converterA.Pounds = 10;

		// Check that intial value is 0
		if(converterA.Pounds == converterB.Pounds || converterB.Pounds != 5){
			Fail("Value was not coppied correctly");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Mass, Add,
		// Create converters
		const Mass<double> converterA(MassD::unit_t::POUND, 5);
		const Mass<double> converterB(MassD::unit_t::POUND, 10);

		// Perform operation
		Mass<double> converterC = converterA + converterB;
		Mass<double> converterD = converterA;
		converterD += converterB;
		Mass<double> converterE = converterA;
		converterE.Pounds += converterB.Pounds;

		// Check that intial value is 0
		if(converterC.Pounds != converterD.Pounds || converterC.Pounds != converterE.Pounds || converterC.Pounds != 15){
			Fail("Operation returned incorrect value");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Mass, Subtract,
		// Create converters
		const Mass<double> converterA(MassD::unit_t::POUND, 10);
		const Mass<double> converterB(MassD::unit_t::POUND, 5);

		// Perform operation
		Mass<double> converterC = converterA - converterB;
		Mass<double> converterD = converterA;
		converterD -= converterB;
		Mass<double> converterE = converterA;
		converterE.Pounds -= converterB.Pounds;

		// Check that intial value is 0
		if(converterC.Pounds != converterD.Pounds || converterC.Pounds != converterE.Pounds || converterC.Pounds != 5){
			Fail("Operation returned incorrect value");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Mass, Multiply,
		// Create converters
		const Mass<double> converterA(MassD::unit_t::POUND, 5);
		const Mass<double> converterB(MassD::unit_t::POUND, 2);

		// Perform operation
		Mass<double> converterC = converterA * converterB;
		Mass<double> converterD = converterA;
		converterD *= converterB;
		Mass<double> converterE = converterA;
		converterE.Pounds *= converterB.Pounds;

		// Check that intial value is 0
		if(converterC.Pounds != converterD.Pounds || converterC.Pounds != converterE.Pounds || converterC.Pounds != 10){
			Fail("Operation returned incorrect value");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Mass, Divide,
		// Create converters
		const Mass<double> converterA(MassD::unit_t::POUND, 10);
		const Mass<double> converterB(MassD::unit_t::POUND, 2);

		// Perform operation
		Mass<double> converterC = converterA / converterB;
		Mass<double> converterD = converterA;
		converterD /= converterB;
		Mass<double> converterE = converterA;
		converterE.Pounds /= converterB.Pounds;

		// Check that intial value is 0
		if(converterC.Pounds != converterD.Pounds || converterC.Pounds != converterE.Pounds || converterC.Pounds != 5){
			Fail("Operation returned incorrect value");
		}else{
			Pass("");
		}
	);

	template<typename T>
	static void MassLConvertTests(Test::TestManager& aManager){
		typedef ConversionTest<T, Mass<T>> ConversionTest;
		typedef ConversionTest::ConversionDescription Conversion;
		typedef Mass<T>::prefix_t prefix_t;
		typedef Mass<T>::unit_t unit_t;

		aManager.Add(std::shared_ptr<Test::Test>(new ConversionTest("Mass", "Conversions", {
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"grams",		prefix_t::NONE,	unit_t::GRAM,		1814.37,	1),
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"ounces",		prefix_t::NONE,	unit_t::OUNCE,		64,			1),
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"stones",		prefix_t::NONE,	unit_t::STONE,		0.285714,	1),
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"metric tons",	prefix_t::NONE,	unit_t::METRIC_TON,	0.00181437,	1),
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"short tons",	prefix_t::NONE,	unit_t::SHORT_TON,	0.002,		1),
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"short tons",	prefix_t::NONE,	unit_t::LONG_TON,	0.00178571,	1),
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"troy ounces",	prefix_t::NONE,	unit_t::TROY_OUNCE,	58.3333333,	1),
			Conversion("pounds",	prefix_t::NONE,	unit_t::POUND,	4,			"troy pounds",	prefix_t::NONE,	unit_t::TROY_POUND,	4.8612,		1),
			Conversion("kilograms",	prefix_t::KILO,	unit_t::GRAM,	2,			"ounces",		prefix_t::NONE,	unit_t::OUNCE,		70.5479,	1),
			Conversion("ounces",	prefix_t::NONE,	unit_t::OUNCE,	70.5479,	"kilograms",	prefix_t::KILO,	unit_t::GRAM,		2,			1),
			Conversion("kilograms",	prefix_t::KILO,	unit_t::GRAM,	1.989e+30,	"solar masses",	prefix_t::NONE,	unit_t::SOLAR_MASS,	1,			1)
		})));
	}

	static void MassTests(Test::TestManager& aManager){

		MassLConvertTests<double>(aManager);

		aManager.Add(std::shared_ptr<Test::Test>(new TestMassSelfConvert()));

		aManager.Add(std::shared_ptr<Test::Test>(new TestMassStaticConvert()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestMassInteger()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestMassDefaultConstructor()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestMassUnitConstructor()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestMassCopyConstructor()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestMassAdd()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestMassSubtract()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestMassMultiply()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestMassDivide()));
	}
}}}


#endif