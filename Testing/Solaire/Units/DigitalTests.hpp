#ifndef SOLAIRE_UNITS_DIGITAL_TEST_HPP
#define SOLAIRE_UNITS_DIGITAL_TEST_HPP

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

#include "Solaire\Test\Test.hpp"
#include "Solaire\Units\Digital.hpp"
#include "Testing.hpp"

namespace Solaire{ namespace Units { namespace Testing{

	
	SOLAIRE_TEST(Digital, SelfConvert,
		// Create converter
		Digital<double> converter;

		// Initialise with a value
		converter.Bytes = 100.0;

		// Check that the same value is returned
		if(converter.Bytes != 100.0){
			Fail("Failed to convert BYTE to BYTE");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Digital, SmallUnits,
		// Create converter
		Digital<double> converter;

		// Initialise with a value
		converter.Bytes = 4;

		// Test that the unit is converted correctly
		double threshold = 1.0;
		double expectedResult = 0.0;
		double actualResult = 0.0;

		expectedResult = 32;
		actualResult = converter.Bits;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert BYTE to BIT");

		expectedResult = 8;
		actualResult = converter.Nybbles;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert BYTE to NYBBLE");

		Pass("");
	);

	SOLAIRE_TEST(Digital, BigUnits,
		// Create converter
		Digital<double> converter;

		// Initialise with a value
		converter.Megabytes = 2;

		// Test that the unit is converted correctly
		double threshold = 1.0;
		double expectedResult = 0.0;
		double actualResult = 0.0;

		expectedResult = 2e+6;
		actualResult = converter.Bytes;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MEGABYTE to BYTE");

		expectedResult = 2000;
		actualResult = converter.Kilobytes;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MEGABYTE to KILOBYTE");

		expectedResult = 0.002;
		actualResult = converter.Gigabytes;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MEGABYTE to GIGABYTE");

		expectedResult = 0.002;
		actualResult = converter.Gigabytes;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MEGABYTE to TERABYTE");

		expectedResult = 2e-6;
		actualResult = converter.Terabytes;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MEGABYTE to KILOBIT");

		expectedResult = 16;
		actualResult = converter.Megabits;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MEGABYTE to MEGABIT");

		expectedResult = 0.016;
		actualResult = converter.Gigabits;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MEGABYTE to GIGABIT");

		expectedResult = 1.6e-5;
		actualResult = converter.Terabits;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MEGABYTE to TERABIT");

		expectedResult = 1953.13;
		actualResult = converter.Kibibytes;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MEGABYTE to KIBIBYTE");

		expectedResult = 1.90735;
		actualResult = converter.Mebibytes;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MEGABYTE to MEBIBYTE");

		expectedResult = 0.00186265;
		actualResult = converter.Gibibytes;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MEGABYTE to GIBIBYTE");

		expectedResult = 1.819e-6;
		actualResult = converter.Tebibytes;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MEGABYTE to TEBIBYTE");

		expectedResult = 15625;
		actualResult = converter.Kibibits;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MEGABIT to KIBIBIT");

		expectedResult = 15.2588;
		actualResult = converter.Mebibits;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MEGABIT to MEBIBIT");

		expectedResult = 0.0149012;
		actualResult = converter.Gibibits;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MEGABIT to GIBIBIT");

		expectedResult = 1.4552e-5;
		actualResult = converter.Tebibits;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MEGABIT to TEBIBIT");


		Pass("");
	);

	SOLAIRE_TEST(Digital, StaticConvert,
		// Test that the unit is converted correctly
		double threshold = 1.0;
		double expectedResult = 0.0;
		double actualResult = 0.0;

		expectedResult = 32;
		actualResult = DigitalD::Convert(DigitalD::unit_t::BYTE, DigitalD::unit_t::BIT, 4);
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to static convert BYTE to BIT using double values");

		Pass("");
	);

	SOLAIRE_TEST(Digital, Integer,
		// Test that the unit is converted correctly
		double threshold = 1.0;
		double expectedResult = 0.0;
		double actualResult = 0.0;

		expectedResult = 32;
		actualResult = DigitalD::Convert(DigitalD::unit_t::BYTE, DigitalD::unit_t::BIT, 4);
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to static convert BYTE to BIT using integer values");

		Pass("");
	);

	SOLAIRE_TEST(Digital, DefaultConstructor,
		// Create converter
		Digital<double> converter;

		// Check that intial value is 0
		if(converter.Bytes != 0.0){
			Fail("Inital value is not 0");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Digital, UnitConstructor,
		// Create converter
		Digital<double> converter(DigitalD::unit_t::BYTE, 5);

		// Check that intial value is 0
		if(converter.Bytes != 5){
			Fail("Returned value is different than given in constructor");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Digital, CopyConstructor,
		// Create converter
		Digital<double> converterA(DigitalD::unit_t::BYTE, 5);

		// Copy converter
		Digital<double> converterB = converterA;

		// Use converterA
		converterA.Bytes = 10;

		// Check that intial value is 0
		if(converterA.Bytes == converterB.Bytes || converterB.Bytes != 5){
			Fail("Value was not coppied correctly");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Digital, Add,
		// Create converters
		const Digital<double> converterA(DigitalD::unit_t::BYTE, 5);
		const Digital<double> converterB(DigitalD::unit_t::BYTE, 10);

		// Perform operation
		Digital<double> converterC = converterA + converterB;
		Digital<double> converterD = converterA;
		converterD += converterB;
		Digital<double> converterE = converterA;
		converterE.Bytes += converterB.Bytes;

		// Check that intial value is 0
		if(converterC.Bytes != converterD.Bytes || converterC.Bytes != converterE.Bytes || converterC.Bytes != 15){
			Fail("Operation returned incorrect value");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Digital, Subtract,
		// Create converters
		const Digital<double> converterA(DigitalD::unit_t::BYTE, 10);
		const Digital<double> converterB(DigitalD::unit_t::BYTE, 5);

		// Perform operation
		Digital<double> converterC = converterA - converterB;
		Digital<double> converterD = converterA;
		converterD -= converterB;
		Digital<double> converterE = converterA;
		converterE.Bytes -= converterB.Bytes;

		// Check that intial value is 0
		if(converterC.Bytes != converterD.Bytes || converterC.Bytes != converterE.Bytes || converterC.Bytes != 5){
			Fail("Operation returned incorrect value");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Digital, Multiply,
		// Create converters
		const Digital<double> converterA(DigitalD::unit_t::BYTE, 5);
		const Digital<double> converterB(DigitalD::unit_t::BYTE, 2);

		// Perform operation
		Digital<double> converterC = converterA * converterB;
		Digital<double> converterD = converterA;
		converterD *= converterB;
		Digital<double> converterE = converterA;
		converterE.Bytes *= converterB.Bytes;

		// Check that intial value is 0
		if(converterC.Bytes != converterD.Bytes || converterC.Bytes != converterE.Bytes || converterC.Bytes != 10){
			Fail("Operation returned incorrect value");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Digital, Divide,
		// Create converters
		const Digital<double> converterA(DigitalD::unit_t::BYTE, 10);
		const Digital<double> converterB(DigitalD::unit_t::BYTE, 2);

		// Perform operation
		Digital<double> converterC = converterA / converterB;
		Digital<double> converterD = converterA;
		converterD /= converterB;
		Digital<double> converterE = converterA;
		converterE.Bytes /= converterB.Bytes;

		// Check that intial value is 0
		if(converterC.Bytes != converterD.Bytes || converterC.Bytes != converterE.Bytes || converterC.Bytes != 5){
			Fail("Operation returned incorrect value");
		}else{
			Pass("");
		}
	);


	static void DigitalTests(Test::TestManager& aManager){
		aManager.Add(std::shared_ptr<Test::Test>(new TestDigitalSelfConvert()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDigitalSmallUnits()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDigitalBigUnits()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDigitalStaticConvert()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDigitalInteger()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDigitalDefaultConstructor()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDigitalUnitConstructor()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDigitalCopyConstructor()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDigitalAdd()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDigitalSubtract()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDigitalMultiply()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDigitalDivide()));
	}
}}}


#endif