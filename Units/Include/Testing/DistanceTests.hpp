#ifndef SOLAIRE_UNITS_DISTANCE_TEST_HPP
#define SOLAIRE_UNITS_DISTANCE_TEST_HPP

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
#include "..\Distance.hpp"
#include "Testing.hpp"

namespace Solaire{ namespace Units { namespace Testing{

	SOLAIRE_TEST(Distance, SelfConvert,
		// Create converter
		Distance<double> converter;

		// Initialise with a value
		converter.Metres = 100.0;

		// Check that the same value is returned
		if(converter.Metres != 100.0){
			Fail("Failed to convert METRE to METRE");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Distance, SmallUnits,
		// Create converter
		Distance<double> converter;

		// Initialise with a value
		converter.Metres = 100.0;

		// Test that the unit is converted correctly
		double threshold = 1.0;
		double expectedResult = 0.0;
		double actualResult = 0.0;

		expectedResult = 3937.00787;
		actualResult = converter.Inches;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert METRE to INCH");

		expectedResult = 328.08399;
		actualResult = converter.Feet;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert METRE to FOOT");

		expectedResult = 109.36133;
		actualResult = converter.Yards;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert METRE to Yard");

		expectedResult = 0.0621371192;
		actualResult = converter.Miles;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert METRE to MILE");

		expectedResult = 4.97096954;
		actualResult = converter.Chains;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert METRE to CHAIN");

		expectedResult = 19.8838782;
		actualResult = converter.Rods;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert METRE to ROD");

		//! \TODO Test Links

		Pass("METRE to LINK was not tested");
	);

	SOLAIRE_TEST(Distance, MediumUnits,
		// Create converter
		Distance<double> converter;

		// Initialise with a value
		converter.Miles = 5.0;

		// Test that the unit is converted correctly
		double threshold = 1.0;
		double expectedResult = 0.0;
		double actualResult = 0.0;

		expectedResult = 40.0;
		actualResult = converter.Furlongs;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MILE to FURLONG");

		expectedResult = 4.34488;
		actualResult = converter.NauticalMiles;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MILE to MILE_NAUTICAL");

		expectedResult = 1.44829374;
		actualResult = converter.NauticalLeagues;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert MILE to LEAUGE_NAUTICAL");

		//! \TODO Test LeagueUK & LeaugesUS

		Pass("MILE to LEAGUGE_UK / LEAUGE_US were not tested");
	);

	SOLAIRE_TEST(Distance, BigUnits,
		// Create converter
		Distance<double> converter;

		// Initialise with a value
		converter.Lightyears = 1.0;

		// Test that the unit is converted correctly
		double threshold = 1.0;
		double expectedResult = 0.0;
		double actualResult = 0.0;

		expectedResult = 9.4605284e+15;
		actualResult = converter.Metres;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert LIGHTYEAR to METRE");

		expectedResult = 0.306594845;
		actualResult = converter.Parsecs;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to convert LIGHTYEAR to PARSEC");

		expectedResult = 9.4605284e+12;
		actualResult = converter.Kilometres;
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) return Fail("Failed to convert LIGHTYEAR to KILO | METRE");

		Pass("");
	);

	SOLAIRE_TEST(Distance, StaticConvert,
		// Test that the unit is converted correctly
		double threshold = 1.0;
		double expectedResult = 0.0;
		double actualResult = 0.0;

		expectedResult = 2;
		actualResult = DistanceD::Convert(DistanceD::unit_t::YARD, DistanceD::unit_t::MILE, 3520);
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to static convert YARD to MILE using double values");

		Pass("");
	);

	SOLAIRE_TEST(Distance, Integer,
		// Test that the unit is converted correctly
		double threshold = 1.0;
		double expectedResult = 0.0;
		double actualResult = 0.0;

		expectedResult = 2;
		actualResult = DistanceI::Convert(DistanceD::unit_t::YARD, DistanceD::unit_t::MILE, 3520);
		if (PercentageDifference<double>(expectedResult, actualResult) > threshold) Fail("Failed to static convert YARD to MILE using integer values");

		Pass("");
	);

	SOLAIRE_TEST(Distance, DefaultConstructor,
		// Create converter
		Distance<double> converter;

		// Check that intial value is 0
		if(converter.Metres != 0.0){
			Fail("Inital value is not 0");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Distance, UnitConstructor,
		// Create converter
		Distance<double> converter(DistanceD::unit_t::YARD, 5);

		// Check that intial value is 0
		if(converter.Yards != 5){
			Fail("Returned value is different than given in constructor");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Distance, CopyConstructor,
		// Create converter
		Distance<double> converterA(DistanceD::unit_t::YARD, 5);

		// Copy converter
		Distance<double> converterB = converterA;

		// Use converterA
		converterA.Yards = 10;

		// Check that intial value is 0
		if(converterA.Yards == converterB.Yards || converterB.Yards != 5){
			Fail("Value was not coppied correctly");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Distance, Add,
		// Create converters
		const Distance<double> converterA(DistanceD::unit_t::YARD, 5);
		const Distance<double> converterB(DistanceD::unit_t::YARD, 10);

		// Perform operation
		Distance<double> converterC = converterA + converterB;
		Distance<double> converterD = converterA;
		converterD += converterB;
		Distance<double> converterE = converterA;
		converterE.Yards += converterB.Yards;

		// Check that intial value is 0
		if(converterC.Yards != converterD.Yards || converterC.Yards != converterE.Yards || converterC.Yards != 15){
			Fail("Operation returned incorrect value");
		}else{
			Pass("");
		}
	);

	SOLAIRE_TEST(Distance, Subtract,
		// Create converters
		const Distance<double> converterA(DistanceD::unit_t::YARD, 10);
		const Distance<double> converterB(DistanceD::unit_t::YARD, 5);

		// Perform operation
		Distance<double> converterC = converterA - converterB;
		Distance<double> converterD = converterA;
		converterD -= converterB;
		Distance<double> converterE = converterA;
		converterE.Yards -= converterB.Yards;

		// Check that intial value is 0
		if(converterC.Yards != converterD.Yards || converterC.Yards != converterE.Yards || converterC.Yards != 5){
			Fail("Operation returned incorrect value");
		}else{
			Pass("");
		}
	);

	

	SOLAIRE_TEST(Distance, Multiply,
		// Create converters
		const Distance<double> converterA(DistanceD::unit_t::YARD, 5);
		const Distance<double> converterB(DistanceD::unit_t::YARD, 2);

		// Perform operation
		Distance<double> converterC = converterA * converterB;
		Distance<double> converterD = converterA;
		converterD *= converterB;
		Distance<double> converterE = converterA;
		converterE.Yards *= converterB.Yards;

		// Check that intial value is 0
		if(converterC.Yards != converterD.Yards || converterC.Yards != converterE.Yards || converterC.Yards != 10){
			Fail("Operation returned incorrect value");
		}else{
			Pass("");
		}
	);

	

	SOLAIRE_TEST(Distance, Divide,
		// Create converters
		const Distance<double> converterA(DistanceD::unit_t::YARD, 10);
		const Distance<double> converterB(DistanceD::unit_t::YARD, 2);

		// Perform operation
		Distance<double> converterC = converterA / converterB;
		Distance<double> converterD = converterA;
		converterD /= converterB;
		Distance<double> converterE = converterA;
		converterE.Yards /= converterB.Yards;

		// Check that intial value is 0
		if(converterC.Yards != converterD.Yards || converterC.Yards != converterE.Yards || converterC.Yards != 5){
			Fail("Operation returned incorrect value");
		}else{
			Pass("");
		}
	);

	static void DistanceTests(Test::TestManager& aManager){
		aManager.Add(std::shared_ptr<Test::Test>(new TestDistanceSelfConvert()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDistanceSmallUnits()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDistanceMediumUnits()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDistanceBigUnits()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDistanceStaticConvert()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDistanceInteger()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDistanceDefaultConstructor()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDistanceUnitConstructor()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDistanceCopyConstructor()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDistanceAdd()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDistanceSubtract()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDistanceMultiply()));
		aManager.Add(std::shared_ptr<Test::Test>(new TestDistanceDivide()));
	}
}}}


#endif